#pragma once

// Какие-то коды из интернетов, которые не работают. Интересно, почему?
#include "stdafx.h"
std::vector<int> build(const std::string& s) {
    int n = s.length();
    // permutations содержит перестановки циклических сдвигов в лексикографическом порядке
    // Перестановка кодируется индексом начала.
    std::vector<int> permutations(n), cnt(alphabetSize, 0), classes(n);

    // Будем сортировать не циклы, а циклические сдвиги. В конце строки стоит символ \0.
    // Будем сортировать циклические сдвиги длины 2^k.
    // Сортировка подсчтом. Разбиваем символы (подстроки длины 1) на классы эквивалентности.
    for (int i = 0; i < n; ++i)
        ++cnt[s[i]];
    for (int i = 1; i < alphabetSize; ++i)
        cnt[i] += cnt[i - 1];
    for (int i = 0; i < n; ++i)
        permutations[--cnt[s[i]]] = i;
    classes[permutations[0]] = 0;
    int classesNum = 1;
    for (int i = 1; i < n; ++i) {
        if (s[permutations[i]] != s[permutations[i - 1]])
            ++classesNum;
        classes[permutations[i]] = classesNum - 1;
    }

    std::vector<int> pn(n), newClasses(n);
    for (int h = 0; fastpow2(h) < n; ++h) {
        // Каждую итерацию пишем классы эквивалентности для подотрезков 2^k, сравнивая пары из подотрезков 2^(k-1)
        for (int i = 0; i < n; ++i) {
            pn[i] = permutations[i] - fastpow2(h);
            if (pn[i] < 0)
                pn[i] += n;
        }
        cnt.resize(classesNum, 0);
        for (int i = 0; i < n; ++i)
            ++cnt[classes[pn[i]]];
        for (int i = 1; i < classesNum; ++i)
            cnt[i] += cnt[i - 1];
        for (int i = n - 1; i >= 0; --i)
            permutations[--cnt[classes[pn[i]]]] = pn[i];
        newClasses[permutations[0]] = 0;
        classesNum = 1;
        for (int i = 1; i < n; ++i) {
            int mid1 = (permutations[i] + fastpow2(h)) % n, mid2 = (permutations[i - 1] + fastpow2(h)) % n;
            if (classes[permutations[i]] != classes[permutations[i - 1]] || classes[mid1] != classes[mid2])
                ++classesNum;
            newClasses[permutations[i]] = classesNum - 1;
        }
        std::swap(classes, newClasses);
    }
    return classes;

}


int L = 256;
std::vector<int> buildMSK(const std::string& s) {
    //s - исходная строка
    //суффиксный массив
    std::vector<int> arr;
    arr.resize(s.size());
    //массив цветов
    std::vector<int> col;
    col.resize(s.size());
    //массив для временных данных
    std::vector<int> buf;
    buf.resize(s.size());
    //массив для карманов сортировки
    std::vector<int> buck;
    buck.resize(std::max(L, (int) s.size()));
    //Шаг первый - начальная сортировка
    //мы хотим отсортировать буквы строки
    //посчитаем количество всех букв
    for (int i = 0; i < (int) s.size(); i++)
        buck[s[i]]++;
    //преобразуем массив так, чтобы каждый элемент указывал на положение в массиве первой данной буквы
    int sum = 0;
    for (int i = 0; i < L; i++)
        sum += buck[i], buck[i] = sum - buck[i];
    //теперь заполним массив arr: Теперь в нем суффиксы отсортированы по первой букве
    for (int i = 0; i < (int) s.size(); i++)
        arr[buck[s[i]]++] = i;
    //теперь проставляем цвета: цвет увеличивается на 1 если следующая буква - другая
    col[arr[0]] = 0;
    for (int i = 1; i < (int) s.size(); i++)
        col[arr[i]] = col[arr[i - 1]] + (s[arr[i]] != s[arr[i - 1]]);
    int cn = col[arr[s.size() - 1]] + 1;
    //Шаг второй - постепенное расширение подстрок
    //в начале цикла отсортированы подстроки длины l, а в конце - длины 2l
    for (int l = 1; l < (int) s.size(); l *= 2) {
        //обнуляем массив buck  и заполняем для сортировки по col
        for (int i = 0; i < (int) s.size(); i++)
            buck[i] = 0;
        for (int i = 0; i < (int) s.size(); i++)
            buck[col[i]]++;
        sum = 0;
        for (int i = 0; i < cn; i++)
            sum += buck[i], buck[i] = sum - buck[i];
        //строим новый массив в buf (не забываем сдвинуть указатель по модулю на l влево), затем копируем его в arr
        for (int i = 0; i < (int) s.size(); i++)
            buf[buck[col[(arr[i] - l + s.size()) % s.size()]]++] = (arr[i] - l + s.size()) % s.size();
        arr = buf;
        //теперь перекрашиваем массив col: заполняем массив buf, увеличиваем цвет на единицу если один из цветов отличается, затем копируем
        buf[arr[0]] = 0;
        for (int i = 1; i < (int) s.size(); i++)
            buf[arr[i]] = buf[arr[i - 1]] + (col[arr[i]] != col[arr[i - 1]] || col[(arr[i] + l) % s.size()] != col[(arr[i - 1] + l) % s.size()]);
        cn = buf[arr[s.size() - 1]] + 1;
        col = buf;
    }
    //возвращаем результат
    return arr;

}

inline int fastpow2(int n) {
    return 1 << n;
}