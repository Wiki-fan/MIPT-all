#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <conio.h>
#include <cstring>
#include <cmath>
#include <ctime>
#include <iostream>
#include <fstream>
#include <vector>

#include "Sorts.h"
#include "MyVector.h"

/*unsigned findRepetitionsCount(const char* s, char c1, char c2)
{
	unsigned ret = 0;
	for (const char *s1 = s + 1; *s1 != '\0'; ++s, ++s1) {
		if (*s == c1 && *s1 == c2) {
			++ret;
		}
	}
	return ret;
}

int main()
{
	char buf[1000];
	char c1, c2;
	scanf("%s %c %c", buf, &c1, &c2);
	printf("%u", findRepetitionsCount(buf, c1, c2));
	_getch();
	return 0;
}*/

/*void MergeSort(int *start, int *finish, int* buffer)
{
	if (start + 1 == finish)
		return;

	int *mid = start + (finish - start + 1) / 2;
	MergeSort(start, mid, buffer);
	MergeSort(mid, finish, buffer);

	int *start1 = buffer, *mid1 = start1 + (mid - start), *finish1 = start1 + (finish - start);
	//for (int *i1 = start, *i2 = start1; i1 != mid; ++i1, ++i2) {
	//	*i2 = *i1;
	//}
	//for (int *i1 = mid, *i2 = mid1; i1 != finish; ++i1, ++i2) {
	//	*i2 = *i1;
	//}
	memcpy(start1, start, (mid1 - start1)*sizeof(int));
	memcpy(mid1, mid, (finish1 - mid1)*sizeof(int));

	for (int *i = start, *i1 = start1, *i2 = mid1; i != finish; ++i) {
		if ((*i1 <= *i2 && i1 != mid1) || i2 == finish1) {
			*i = *i1;
			++i1;
		}
		else if ((*i1 > *i2 && i2 != finish1) || i1 == mid1) {
			*i = *i2;
			++i2;
		}
	}
}

int main()
{
	int a[8] = { 4, 5, 3, 7, 9, 1, 6, 0 };
	int b[8];
	MergeSort(a, a + 8, b);
	for (int i = 0; i < 8; ++i)
		printf("%d ", a[i]);
	_getch();
	return 0;
}*/

/*int main()
{
	srand(time(NULL));
	clock_t t;

	FILE* outf;
	char buf[15];
	for (int i = 4; i <= 7; ++i) {
		int n = pow(10, i);
		sprintf(buf, "%d.txt", n);
		outf = fopen(buf, "w+");
		for (int i = 0; i < n; ++i) {
			fprintf(outf, "%lf ", (double)rand()+16000.0/(rand()+1));
		}
		fclose(outf);
		
		double a;

		t = clock();
		FILE* inf = fopen(buf, "r");
		while (!feof(inf))
			fscanf(inf, "%lf", &a);
		fclose(inf);
		t = clock() - t;
		printf("%8d: %10.10lf - ", n, (float)t/CLOCKS_PER_SEC);

		t = clock();
		std::ifstream ifs (buf, std::ifstream::in);
		while (ifs.good()) {
			ifs >> a;
		}
		ifs.close();
		t = clock() - t;
		printf("%10.10lf - ", (float)t / CLOCKS_PER_SEC);

		std::ios::sync_with_stdio(false);
		t = clock();
		std::ifstream ifs2(buf, std::ifstream::in);
		while (ifs2.good()) {
			ifs2 >> a;
		}
		ifs2.close();
		t = clock() - t;
		printf("%10.10lf\n", (float)t / CLOCKS_PER_SEC);
	}
	_getch();
	return 0;
}*/

struct Point
{
	int x_;
	bool b_;

	Point() :x_(0), b_(false) {};
	Point(int x, bool b) :x_(x), b_(b) {};

	bool operator<(const Point& other) {
		if (x_ < other.x_)
			return true;
		else return false;
	}

	void Set(int x, bool b) {
		x_ = x;
		b_ = b;
	}

	const Point& operator=(const Point& other) {
		x_ = other.x_;
		b_ = other.b_;
		return *this;
	}
};

int main() {
	int n;
	scanf("%d", &n);
	my::MyVector<Point> points(n*2);
  int x;
	for (int i = 0; i < n*2; i += 2) {
		scanf("%d", &x);
		points[i].Set(x, true);
		scanf("%d", &x);
		points[i+1].Set(x, false);
	}
	Point *buffer = new Point[points.size()];
	MergeSort<Point>(points.begin(), points.end(), buffer);
	int cnt = 1; // points[0] is beginning
	int sum = 0;
	for (size_t i = 1; i < points.size(); ++i) {
		if (cnt == 1)
			sum += points[i].x_ - points[i-1].x_;
		if (points[i].b_ == true)
			++cnt;
		else --cnt;
	}
	printf("%d", sum);
	return 0;
}
