#pragma once
#include <string>
#include <stack>
#include <set>
#include <map>
#include <vector>
#include <limits>
#include <algorithm>

template<typename vtype = int, typename TransitionMark = char>
class Prac {
public:
    bool solve(std::string& s_, TransitionMark x_, int k_) {
        x = x_;
        k = k_;
        fromReversePolishToFsm(s_);

        int size = delta.size();
        colors.resize(size, Color::white);
        depths.resize(size, 0);
        answers.resize(size, 0);
        // Запускаем dfs из всех вершин, ответ пишется в answers[v]
        for (vtype v = 0; v<delta.size(); ++v) {
            DFS(v);
        }
        // Восстанавливаем ответ
        int maxLen = *std::max_element(answers.begin(), answers.end());
        //std::cout << maxLen << std::endl;
        return (maxLen >= k);
    }

private:
    // Подавтомат. Имеет начало и конец. Автомат строится из таких частей по схемам, которые были на лекции.
    struct SubFSM {
        int begin;
        int end;
        SubFSM(int begin_, int end_) : begin(begin_), end(end_) {}
    };

    struct ParseException {
        ParseException(std::string s) {
            what = s;
        }
        std::string what;
    };

    char x;
    int k;

    // Цвет вершины: белый, если не приходили в вершину ни разу,
    // серый, если эта вершина - предок текущей в дереве обхода, чёрный, если вышли.
    enum class Color {
        black, gray, white
    };
    std::vector<Color> colors;
    // Глубина вершины в дереве DFS.
    std::vector<int> depths;
    // Максимальная длина последовательности из x,
    std::vector<int> answers;
    const int INF = std::numeric_limits<int>::max();

    // Обход в глубину из данной вершины, перед этим сбрасываются все нужные массивы.
    void DFS(vtype v) {
        colors.assign(colors.size(), Color::white);
        depths.assign(depths.size(), 0);
        dfs(v, 0);
    }

    // Обход в глубину
    int dfs(vtype v, int curDepth) {
        // Если вершина серая
        if (colors[v] == Color::gray) {
            if (depths[v] < curDepth) {
                // Нашли цикл, идущий по переходам x. Можно получить слово x^k произвольной длины. Победа.
                return INF;
            } else if (depths[v] == curDepth) {
                // Значит, пришли в эту же вершину по циклу только по эпсилон-переходам. Они нас не интересуют.
                return 0; // Нейтральный элемент, который точно не поменяет максимума.
            }
            // depths[mark.second] > curDepth, очевидно, невозможно: при таком условии мы присваиваем depths[mark.second] = curDepth
        }

        // Если вершина чёрная, для неё уже посчитан итоговый ответ.
        if (colors[v] == Color::black)
            return answers[v];

        // Если мы пришли сюда, вершина белая
        depths[v] = curDepth;
        colors[v] = Color::gray;

        int ans = answers[v];
        for (auto&& mark:delta[v]) {
            if (mark.first == x) {
                for (auto&& to: mark.second) { // Переход по искомой букве
                    int ret = 1 + dfs(to, curDepth + 1); // Глубина увеличивается на 1, ответ - тоже
                    if (ans < ret)
                        ans = ret;
                }
            } else if (mark.first == 0) {
                for (auto&& to: mark.second) {
                    // Эпсилон-переход, который ничего не меняет
                    int ret = dfs(to, curDepth);
                    if (ans < ret)
                        ans = ret;
                }
            }
        }

        colors[v] = Color::black;
        answers[v] = ans;
        return answers[v];
    }

    void fromReversePolishToFsm(std::string& rpn) {
        int nextNode = 0;
        start = 0;
        std::stack<SubFSM> st;

        for (char c: rpn) {
            if (isalpha(c)) {
                // Обрабатываем переход, добавляя два состояния с данным переходом между ними
                SubFSM ans(nextNode, nextNode + 1);
                st.push(ans);
                nextNode += 2;

                addEdge(ans.begin, c, ans.end);
                finals.insert(ans.end);
            } else if (c == '1') {
                // Обрабатываем эпсилон-переход аналогично
                SubFSM ans(nextNode, nextNode + 1);
                st.push(ans);
                nextNode += 2;

                addEdge(ans.begin, 0, ans.end);
                finals.insert(ans.end);
            } else if (c == '.') {
                // Обрабатываем конкатенацию, цепляя эпсилон-переходом один подавтомат к другому
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
                // Обрабатываем сумму, добавляя общее стартовое состояние (соединяя его со стартовыми подавтоматов)
                // и общее финальное состояние (соединяя с ним конечные подавтоматов).
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
                // Обрабатываем итерацию Клини, создавая из подавтомата "петлю" вокруг эпсилон-перехода
                // (чьи начало и конец будут началом и концом нового подавтомата).
                if (st.size() < 1) {
                    throw ParseException("* < 1 args");
                }
                SubFSM a = st.top();
                st.pop();

                SubFSM ans(nextNode, nextNode+1);
                nextNode += 2;

                addEdge(ans.end, 0, a.begin);
                addEdge(a.end, 0, ans.begin);
                addEdge(ans.begin, 0, ans.end);
                if (a.begin == start)
                    start = ans.begin;
                finals.erase(a.end);
                finals.insert(ans.end);

                st.push(ans);
            }
        }

        if (st.size() != 1) {
            throw ParseException("Not enough operators");
        }
        start = st.top().begin;
        finals.insert(st.top().end);
    }

    void addEdge(int from, TransitionMark mark, int to) {
        alphabet.insert(mark);
        delta[from][mark].insert(to);
        delta[to];
    }

    int start;
    std::set<TransitionMark> alphabet;
    std::set<vtype> finals;
    std::map<vtype, std::map<TransitionMark, std::set<vtype>>> delta;
};
