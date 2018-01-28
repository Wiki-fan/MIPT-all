#pragma once

#include <iostream>
#include <stack>
#include "SuffixArray.h"

using type = long long;

class M {
public:
    void solve() {
        type n, m;
        std::cin >> n >> m;
        std::string s;
        std::vector<type> input(n);
        for (type i = 0; i < n; ++i) {
            std::cin >> input[i];
            s.push_back((char) input[i]);
        }
        SuffixArray sa;
        std::vector<int> suffArray = sa.buildFromLecture(s);
        std::vector<int> lcp = sa.getLCP(s, suffArray);
        std::stack<StElem> stack;
        std::vector<type> numOfOccurences(suffArray.size(), 0);
        type maxH = n;
        type maxW = 0;
        type start = 0;

        lcp[n - 1] = 0;
        for (type index = 0; index < n; index++) {
            type newWidth = 0;
            while (!stack.empty() && lcp[index] <= stack.top().h) {
                StElem e = stack.top();
                //std::cout <<"e.h " << e.h <<" e.w " <<e.w <<" e.index " << e.index <<'\n';
                stack.pop();
                newWidth += e.w;
                if ((newWidth + 1) * e.h > (maxW + 1) * maxH) {
                    //std::cout << "setting new max\n";
                    maxW = newWidth;
                    maxH = e.h;
                    start = suffArray[e.index];
                    ++numOfOccurences[start];
                }
            }
            ++newWidth;
            stack.push(StElem(newWidth, index, lcp[index]));
        }

        std::cout << maxH * (maxW + 1) << '\n' << maxH << '\n';
        // Ответ это подмассив данного массива [start, start + maxH)
        for (type i = start; i < start + maxH; ++i) {
            std::cout << input[i] << " ";
        }
    }

private:
    struct StElem {
        StElem(type w, type id, type h) : w(w), index(id), h(h) {}
        type w, index, h;
    };

};
