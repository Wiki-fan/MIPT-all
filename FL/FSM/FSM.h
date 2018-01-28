#pragma once
#include <set>
#include <map>
#include <sstream>
#include <vector>
#include <queue>
#include <stack>

template<typename vtype = int, typename TransitionMark = char>
class FSM {
    struct ParseException {
        ParseException(std::string s) {
            what = s;
        }
        std::string what;
    };

public:
    FSM() : edgesCount(0) {
    }

    FSM(std::string s) : edgesCount(0) {
        deserialize(s);
    }

    void addEdge(int from, TransitionMark mark, int to) {
        alphabet.insert(mark);
        ++edgesCount;
        delta[from][mark].insert(to);
        delta[to];
    }

    void makeFinalState(int vertex) {
        finals.insert(vertex);
    }

    std::string serialize() {
        std::ostringstream oss;
        //oss << edgesCount << " " << finals.size() << std::endl;
        for (auto&& vertexIt : delta) {
            if (vertexIt.first == start) {
                oss << " S";
            }
            if (finals.find(vertexIt.first) != finals.end()) {
                oss << " F";
            }
            if (vertexIt.first != start && finals.find(vertexIt.first) == finals.end())
                oss << "  ";
            oss << vertexIt.first << ' ';

            bool fl = false;
            for (auto&& markIt :vertexIt.second) {
                for (auto&& toIt : markIt.second) {
                    if (fl)
                        oss << "    ";
                    if (!fl)
                        fl = true;


                    oss << markIt.first << ' ' << toIt << std::endl;
                }
            }
        }
        return oss.str();
    }

    std::string serializeForGraphEditor() {
        std::ostringstream oss;
        oss << "////";
        for (auto&& vertexIt : delta) {

            for (auto&& markIt :vertexIt.second) {
                for (auto&& toIt : markIt.second) {
                    oss << vertexIt.first << ' ' << toIt << ' ' << markIt.first << std::endl;
                }
            }
        }
        oss << "\\\\\\\\\n";
        return oss.str();
    }

    struct SubFSM {
        int begin;
        int end;
        SubFSM(int begin_, int end_) : begin(begin_), end(end_) {}
    };

    void fromReversePolishToFsm(std::string& rpn) {
        int nextNode = 0;
        start = 0;
        std::stack<SubFSM> st;
        for (char c: rpn) {
            if (isalpha(c)) {
                SubFSM ans(nextNode, nextNode + 1);
                st.push(ans);
                nextNode += 2;

                addEdge(ans.begin, c, ans.end);
                finals.insert(ans.end);
            } else if (c == '1') {
                SubFSM ans(nextNode, nextNode + 1);
                st.push(ans);
                nextNode += 2;

                addEdge(ans.begin, 0, ans.end);
                finals.insert(ans.end);
            } else if (c == '.') {
                if (st.size() < 2) {
                    throw ParseException(". < 2 args");
                }
                SubFSM a2 = st.top();
                st.pop();
                SubFSM a1 = st.top();
                st.pop();
                SubFSM ans(a1.begin, a2.end);

                if (a1.begin == start) {
                    start = ans.begin;
                }
                if (a2.begin == start) {
                    throw ParseException("2 part of concatenation starts with start");
                }

                finals.erase(a1.end);
                finals.erase(a2.end);
                finals.insert(ans.end);

                addEdge(a1.end, 0, a2.begin);

                st.push(ans);
            } else if (c == '+') {
                if (st.size() < 2) {
                    throw ParseException("+ < 2 args");
                }
                SubFSM a2 = st.top();
                st.pop();
                SubFSM a1 = st.top();
                st.pop();
                SubFSM ans(nextNode, nextNode + 1);
                nextNode += 2;

                addEdge(ans.begin, 0, a1.begin);
                addEdge(ans.begin, 0, a2.begin);
                addEdge(a1.end, 0, ans.end);
                addEdge(a2.end, 0, ans.end);

                if (a1.begin == start || a2.begin == start) {
                    start = ans.begin;
                }

                finals.erase(a1.end);
                finals.erase(a2.end);
                finals.insert(ans.end);

                st.push(ans);
            } else if (c == '*') {
                if (st.size() < 1) {
                    throw ParseException("* < 1 args");
                }
                SubFSM a = st.top();
                st.pop();

                SubFSM ans(nextNode, nextNode);
                nextNode += 1;

                addEdge(ans.begin, 0, a.begin);
                addEdge(a.end, 0, ans.end);
                if (a.begin == start)
                    start = ans.begin;
                finals.erase(a.end);
                finals.insert(ans.end);

                st.push(ans);
            }
        }
    }

    void removeEpsilonTransitions() {
        vtype n = delta.size();
        std::vector<std::vector<std::set<TransitionMark>>> m(n);
        for (auto&& row:m)
            row.resize(n);

        for (auto&& v: delta)
            for (auto&& trans: v.second)
                for (auto&& target: trans.second)
                    m[v.first][target].insert(trans.first);

        //std::cout << serialize();

        // транзитивное замыкание эпсилон-переходов
        for (vtype k = 0; k < n; ++k)
            for (vtype i = 0; i < n; ++i)
                for (vtype j = 0; j < n; ++j)
                    if (m[i][k].count(0) > 0 && m[k][j].count(0) > 0) {
                        m[i][j].insert(0);
                        addEdge(i, 0, j);
                    }

        //std::cout << serialize();

        // делаем завершающими те, из которых можно по эпсилон придти в завершающее
        for (auto&& v: delta) {
            for (auto&& trans: v.second) {
                for (auto&& target: trans.second) {
                    if (trans.first == 0 && isFinal(target)) {
                        finals.insert(v.first);
                    }
                }
            }
        }

        //std::cout << serialize();

        for (auto&& v: delta)
            for (auto&& trans: v.second)
                if (trans.first == 0)
                    for (auto&& target: trans.second)
                        for (auto&& trans2: delta[target])
                            for (auto&& target2: trans2.second)
                                // Если пришли в какую-то вершину по эпсилон, а из неё какую-либо ещё, проведём это ребро напрямую.
                                addEdge(v.first, trans2.first, target2);


        //std::cout << serialize();

        // удаляем эпсилон-переходы
        for (auto&& v: delta)
            v.second.erase(0);

        //std::cout << serialize();

        // помечаем вершины, до которых нельзя добраться
        std::vector<bool> used(n, false);
        for (auto&& v: delta)
            for (auto&& es: v.second)
                for (auto&& target: es.second)
                    used[target] = true;

        // Удаляем их
        for (auto&& v: delta)
            if (used[v.first] == false && v.first != start)
                delta.erase(v.first);

        //std::cout << serialize();
    }

    int priority(char c) {
        switch (c) {
            case '(':
            case ')':
                return 0;
            case '+':
                return 1;
            case '.':
                return 2;
            case '*':
                return 3;
        }
    }

    enum Associativity {
        left, right
    };

    Associativity associativity(char c) {
        switch (c) {
            case '+':
            case '.':
                return Associativity::left;
            case '*':
                return Associativity::right;
        }
    }

    std::string fromInfixToRpn(const std::string& infix) {
        std::stack<char> oper;
        std::string ans;
        std::istringstream is(infix);
        char c;
        //char needAddDot = 0;
        while (is >> c) {
            if (isalpha(c)) {
                /*if (needAddDot)
                    ans += '.';*/
                ans += c;
                while (is >> c) {
                    if (isalpha(c)) {
                        ans += '.';
                        ans += c;
                    } else {
                        is.putback(c);
                        break;
                    }
                }
            } else if (c == '(') {
                oper.push('(');
            } else if (c == ')') {
                if (oper.size() < 1)
                    throw ParseException(") < 1 args");

                while (oper.top() != '(') {
                    if (oper.size() < 1)
                        throw ParseException(") < 1 args");
                    ans += oper.top();
                    oper.pop();
                }
                oper.pop(); // Удаляем '(' из стека.
                //needAddDot = 2;
            } else if (associativity(c) == Associativity::left) {
                while (!oper.empty() && oper.top() != '(' && priority(c) <= priority(oper.top())) {
                    ans += oper.top();
                    oper.pop();
                }
                oper.push(c);
            } else if (associativity(c) == Associativity::right) {
                while (!oper.empty() && oper.top() != '(' && priority(c) < priority(oper.top())) {
                    ans += oper.top();
                    oper.pop();
                }
                oper.push(c);
                //needAddDot = 2;
            }
            /*if (needAddDot>0)
                --needAddDot;*/
        }
        while (!oper.empty()) {
            ans += oper.top();
            oper.pop();
        }
        return ans;
    }

    // Старый формат, не использовать
    void deserialize(std::string& s) {
        clear();
        std::istringstream iss(s);
        size_t edgesCount, finalCount;
        iss >> edgesCount >> finalCount;
        for (size_t i = 0; i < finalCount; ++i) {
            vtype v;
            iss >> v;
            finals.insert(v);
        }
        for (size_t i = 0; i < edgesCount; ++i) {
            vtype from, to;
            TransitionMark mark;
            iss >> from >> mark >> to;
            addEdge(from, mark, to);
        }
    }

    void clear() {
        finals.clear();
        delta.clear();
        alphabet.clear();
        edgesCount = 0;
    }

    FSM determinize() {
        // Создаём пустой автомат.
        FSM ans;
        int next = 0; // номер вершины
        std::map<std::set<vtype>, vtype> vertexID; // Соответствие между множеством старых вершин, представляемых данной новой, и её номером

        // Кладём в очередь стартовую вершину
        std::queue<std::set<vtype>> neededToProcess;
        neededToProcess.push({start});
        vertexID[{start}] = next++;

        while (!neededToProcess.empty()) {
            std::set<vtype> currentVertex = neededToProcess.front();
            neededToProcess.pop();
            for (auto&& mark:alphabet) {
                // Для каждого символа алфавита
                std::set<vtype> canGo;
                for (auto&& vertexIt:currentVertex) {
                    // Для каждой старой вершины, входящей в новую, дописываем то, куда можем попасть по символу mark
                    canGo.insert(delta[vertexIt][mark].begin(), delta[vertexIt][mark].end());
                }

                if (canGo.size()) {
                    // Если такой вершины раньше не встречали, добавляем её
                    if (vertexID.find(canGo) == vertexID.end()) {
                        vertexID[canGo] = next++;
                        neededToProcess.push(canGo);
                    }

                    ans.addEdge(vertexID[currentVertex], mark, vertexID[canGo]);
                }
            }
        }

        // Сделать состояния конечными, если надо.
        // Проще в отдельном цикле, чем как-то искать по .first
        for (auto&& v :vertexID) {
            for (auto&& finIt :finals) {
                if (v.first.find(finIt) != v.first.end()) {
                    ans.makeFinalState(v.second);
                    break;
                }
            }
        }
        ans.start = vertexID[{start}];
        //*this = ans;
        return ans;
    }

    FSM complete() {
        FSM ans(*this);
        bool added = false;
        vtype newVertex;

        for (auto&& v :ans.delta)
            for (auto&& mark : ans.alphabet)
                //Если нашли ребро, которого нет, добавляем его. Для этого первый раз мы создаём дополнительную вершину
                if (v.second.find(mark) == v.second.end()) {
                    if (!added) {
                        newVertex = ans.delta.rbegin()->first + 1;
                        for (auto&& a: alphabet)
                            ans.addEdge(newVertex, a, newVertex);
                        added = true;
                    }

                    ans.addEdge(v.first, mark, newVertex);
                }

        return ans;
    }

    // Внимание: не забыть вручную удалить вершину, которую добавили при пополнении.
    FSM minimize() {
        FSM a = determinize().complete();
        FSM ans;
        // table[номер вершины][по какому ребру] = куда доберёмся
        std::vector<std::vector<TransitionMark>> table(a.delta.size(), std::vector<TransitionMark>(a.alphabet.size()));
        // prevClasses[вершина][метка перехода] = номер класса эквивалентности.
        // По индексу 0 хранится номер класса эквивалентности вершины. И индексация нормальная.
        std::vector<std::vector<int>> prevClasses(a.delta.size(), std::vector<int>(1 + a.alphabet.size(), 0));
        std::vector<bool> isFinal(a.delta.size(), false);
        std::vector<int> newClasses(a.delta.size(), 0);

        // Заполняем table
        int i = 0;
        for (auto&& vertexIt :a.delta) {
            int j = 0;
            for (auto&& mark: a.alphabet) {
                if (vertexIt.second[mark].size() != 1) {
                    if (vertexIt.second[mark].size() == 0)
                        throw ParseException("FSM was not completed");
                    else throw ParseException("FSM was not minimized");
                }
                // У нас автомат детерминированный и полный, то есть в table[i][j] гарантированно один элемент
                table[i][j] = *(vertexIt.second[mark].begin());
                ++j;
            }

            if (a.finals.find(vertexIt.first) != a.finals.end())
                isFinal[i] = true;
            else isFinal[i] = false;

            ++i;
        }

        // Делаем конечным вершинам класс 1. У остальных будет 0.
        for (auto&& fin : a.finals) {
            prevClasses[fin][0] = 1;
        }

        // Повторяем, пока классы не равны
        bool notEqual;
        do {
            for (vtype v = 0; v < prevClasses.size(); ++v) {
                for (TransitionMark mark = 0; mark < table[v].size(); ++mark) {
                    prevClasses[v][mark + 1] = prevClasses[table[v][mark]][0];
                }
            }

            int nextClass = 0;
            std::map<std::vector<vtype>, int> classNumber; // Сопоставляет классу (он set) его номер.
            // Проходим по предыдущим классам, добавляем их в classNumber
            // и прописываем в массив newClasses[i] номер нового класса i-й вершины.
            for (vtype i = 0; i < prevClasses.size(); ++i) {
                if (classNumber.find(prevClasses[i]) == classNumber.end()) {
                    classNumber[prevClasses[i]] = nextClass;
                    newClasses[i] = nextClass++;
                } else {
                    newClasses[i] = classNumber[prevClasses[i]];
                }
            }

            // Ставим флаг, если не равны.
            notEqual = false;
            for (vtype i = 0; i < prevClasses.size(); ++i) {
                if (prevClasses[i][0] != newClasses[i]) {
                    notEqual = true;
                }
                prevClasses[i][0] = newClasses[i];
            }
        } while (notEqual);

        std::set<std::vector<int>> newVertices;
        for (vtype v = 0; v < prevClasses.size(); ++v) {
            // Если вершину, которая получается из данного класса эквивалентности, ещё не добавили, добавляем.
            if (newVertices.find(prevClasses[v]) == newVertices.end()) {
                newVertices.insert(prevClasses[v]);

                // И проводим все рёбра, которые были между её составляющими и другими вершинами.
                vtype i = 1;
                for (auto mark = a.alphabet.begin(); i < prevClasses[v].size(); ++i, ++mark) {
                    ans.addEdge(prevClasses[v][0], *mark, prevClasses[v][i]);

                    if (isFinal[v]) {
                        ans.makeFinalState(prevClasses[v][0]);
                    }
                }
            }
        }

        ans.start = 0;
        return ans;
    }

    /*FSM checkEqual(FSM& other)
    {
        std::vector<std::vector<std::set>> a;
        FSM ans;
        int next = 0; // номер вершины
        std::map<std::set<vtype>, vtype> vertexID; // Соответствие между множеством старых вершин, представляемых данной новой, и её номером

        // Кладём в очередь стартовую вершину
        std::queue<std::pair<vtype, vtype>> neededToProcess;
        neededToProcess.push({start});
        vertexID[{start}] = next++;

        while (!neededToProcess.empty()) {
            std::set<vtype> currentVertex = neededToProcess.front();
            neededToProcess.pop();
            for (auto&& mark:alphabet) {
                // Для каждого символа алфавита
                std::set<vtype> canGo;
                for (auto&& vertexIt:currentVertex) {
                    // Для каждой старой вершины, входящей в новую, дописываем то, куда можем попасть по символу mark
                    canGo.insert(delta[vertexIt][mark].begin(), delta[vertexIt][mark].end());
                }

                if (canGo.size()) {
                    // Если такой вершины раньше не встречали, добавляем её
                    if (vertexID.find(canGo) == vertexID.end()) {
                        vertexID[canGo] = next++;
                        neededToProcess.push(canGo);
                    }

                    ans.addEdge(vertexID[currentVertex], mark, vertexID[canGo]);
                }
            }
        }
    }*/

    bool isFinal(vtype v) {
        return (finals.find(v) != finals.end());
    }

private:
    int start;
    std::set<TransitionMark> alphabet;
    std::set<vtype> finals;
    std::map<vtype, std::map<TransitionMark, std::set<vtype>>> delta;
    size_t edgesCount;
};

