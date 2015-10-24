#include "stdafx.h"
// copyright 2015 me

/*#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <cassert>

namespace my {
template<typename T>
class CVector {
 public:
	CVector();
	explicit CVector(mysize size);
	CVector(mysize size, T* items);
	CVector(const CVector& other);
	~CVector();

	T operator[](const mysize n) const;
	T& operator[](const mysize n);

	CVector& operator=(const CVector& other);

	void PushBack(const T& item);

	T* Begin() const {
		return items;
	}

	T* End() const {
		return &(items[size]);
	}

	const mysize Size() const {
		return size;
	}

	const mysize MaxSize() const {
		return maxSize;
	}

 private:
	mysize size;	// Размер (количество хранящихся элементов).
	mysize maxSize;	// Максимальный размер, a.k.a. размер буфера items.
	T* items;	// Указатель на элементы.
};

}	// namespace my

template<typename T>
my::CVector<T>::CVector() {
	size = 0;
	maxSize = 10;
	items = new T[maxSize];
}

template<typename T>
my::CVector<typename T>::CVector(mysize size) {
	maxSize = size = size;
	items = new T[size];
}

template <typename T>
my::CVector<typename T>::CVector(mysize size, T* items) {
	maxSize = size = size;
	items = new T[size];
	for (int i = 0; i < size; ++i)
		items[i] = items[i];
}

template <typename T>
my::CVector<typename T>::CVector(const CVector& other) {
	if( maxSize < other.size) {
		delete[] items;
		size = other.size;
		maxSize = other.maxSize;
		items = new T[maxSize];
	}
	for (int i = 0; i < size; ++i) {
		items[i] = other.items[i];
	}
}

template <typename T>
my::CVector<typename T>::~CVector() {
	delete[] items;
}

template<typename T>
my::CVector<typename T>& my::CVector<typename T>::operator=(const CVector& other) {
	if( &other == this) {
		return *this;
	} else {
		delete[] items;
		size = other.size;
		maxSize = other.maxSize;
		items = new T[maxSize];

		for (int i = 0; i < size; ++i) {
			items[i] = other.items[i];
		}
	}
}

template <typename T>
T my::CVector<typename T>::operator[] (const mysize n) const {
	assert(n >= 0 && n < size);
	return items[n];
}

template <typename T>
T& my::CVector<typename T>::operator[] (const mysize n) {
	assert(n >= 0 && n < size);
	return items[n];
}

template<typename T>
void my::CVector<typename T>::PushBack(const T& item) {
	if( size == maxSize) {
		maxSize = 2 * maxSize;
		T* new_items = new T[maxSize];

		for (int i = 0; i < size; ++i) {
			new_items[i] = items[i];
		}
		new_items[size++] = item;
		delete[] item_;
		item_ = new_items;
	} else {
		items[size++] = item;
	}
}

namespace my {

template<typename T>
void MergeSort(T *start, T *finish, T* buffer) {
	if( start + 1 == finish)
		return;

	T *mid = start + (finish - start + 1) / 2;
	MergeSort(start, mid, buffer);
	MergeSort(mid, finish, buffer);

	T *start1 = buffer;
	T *mid1 = start1 + (mid - start);
	T *finish1 = start1 + (finish - start);
	for (T *i1 = start, *i2 = start1; i1 != mid; ++i1, ++i2) {
		*i2 = *i1;
	}
	for (T *i1 = mid, *i2 = mid1; i1 != finish; ++i1, ++i2) {
		*i2 = *i1;
	}

	// Deprecated, but fast and unsecure
	// ::memcpy(start1, start, (mid1 - start1)*sizeof(int));
	// ::memcpy(mid1, mid, (finish1 - mid1)*sizeof(int));

	for (T *i = start, *i1 = start1, *i2 = mid1; i != finish; ++i) {
		if( i1 != mid1 && (!(*i2 < *i1) || i2 == finish1)) {
			*i = *i1;
			++i1;
		} else /*if( (*i1 > *i2 && i2 != finish1) || i1 == mid1)*//* {
			*i = *i2;
			++i2;
		}
	}
}

}	// namespace my

struct Point {
	int x_;
	bool b_;

	Point() :x_(0), b_(false) { }
	Point(int x, bool b) :x_(x), b_(b) { }

	bool operator<(const Point& other) {
		if( x_ < other.x_)
			return true;
		else
			return false;
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
	my::CVector<Point> points(n * 2);
	int x;
	for (int i = 0; i < n * 2; i += 2) {
		scanf("%d", &x);
		points[i].Set(x, true);
		scanf("%d", &x);
		points[i + 1].Set(x, false);
	}
	Point *buffer = new Point[points.Size()];
	my::MergeSort<Point>(points.Begin(), points.End(), buffer);
	int cnt = 1;	// points[0] is beginning
	int sum = 0;
	for (mysize i = 1; i < points.Size(); ++i) {
		if( cnt == 1)
			sum += points[i].x_ - points[i - 1].x_;
		if( points[i].b_ == true)
			++cnt;
		else
			--cnt;
	}
	printf("%d", sum);
	return 0;
}
*/