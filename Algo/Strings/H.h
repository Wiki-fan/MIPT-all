#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "SuffixArray.h"

// https://www.e-olymp.com/ru/problems/2464
class H {
public:
    void solve() {
        std::string s;
        std::cin >> s;
        int n = s.length();

        SuffixArray arr;
        std::vector<int> sufArray = arr.buildFromLecture(s);
        std::vector<int> lcp = arr.getLCP(s, sufArray);

        int wordCount = 0;
        // Перебор по длине строки
        for (int l = 1; l < n - 1; ++l) {
            for (int i = 0; i < n - 1;) {
                // Ищем выступы длины LCP за l, по ним ходим внутренним for'ом
                for (; i < n - 1 && lcp[i] < l; ++i) {}
                int first_occurence = 100000, last_occurence = -100000;

                for (; i < n - 1 && lcp[i] >= l; ++i) {
                    if (sufArray[i] < first_occurence) {
                        first_occurence = sufArray[i];
                    }
                    if (sufArray[i + 1] < first_occurence) {
                        first_occurence = sufArray[i + 1];
                    }
                    if (sufArray[i] > last_occurence) {
                        last_occurence = sufArray[i];
                    }
                    if (sufArray[i + 1] > last_occurence) {
                        last_occurence = sufArray[i + 1];
                    }
                }
                // Если первое вхождение и последнее вхождение не перекрываются, то это слово
                if (first_occurence + l <= last_occurence) {
                    ++wordCount;
                }
            }
        }
        std::cout << wordCount;
    }
};
