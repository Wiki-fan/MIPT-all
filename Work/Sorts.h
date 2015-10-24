#pragma once
/*void MergeSort(int *start, int *finish, int* buffer)
{
	if( start + 1 == finish)
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
	if( (*i1 <= *i2 && i1 != mid1) || i2 == finish1) {
		*i = *i1;
		++i1;
	}
	else if( (*i1 > *i2 && i2 != finish1) || i1 == mid1) {
		*i = *i2;
		++i2;
	}
	}
}*/

namespace my {

//���������� ��������. ��������� ��������� �� ������, �����, �������������� ����� ���������������� �������.
template<typename T>
void MergeSort( T *start, T *finish, T* buffer )
{
	if( start + 1 == finish )
		return;

	T *mid = start + (finish - start + 1) / 2;
	MergeSort( start, mid, buffer );
	MergeSort( mid, finish, buffer );

	T *start1 = buffer;
	T *mid1 = start1 + (mid - start);
	T *finish1 = start1 + (finish - start);
	for( T *i1 = start, *i2 = start1; i1 != mid; ++i1, ++i2 ) {
		*i2 = *i1;
	}
	for( T *i1 = mid, *i2 = mid1; i1 != finish; ++i1, ++i2 ) {
		*i2 = *i1;
	}

	// ������, �� �������� ������ ��� �����
	// ::memcpy(start1, start, (mid1 - start1)*sizeof(int));
	// ::memcpy(mid1, mid, (finish1 - mid1)*sizeof(int));

	for( T *i = start, *i1 = start1, *i2 = mid1; i != finish; ++i ) {
		if( i1 != mid1 && (!(*i2 < *i1) || i2 == finish1) ) {
			*i = *i1;
			++i1;
		} else /*if( (*i1 > *i2 && i2 != finish1) || i1 == mid1)*/ {
			*i = *i2;
			++i2;
		}
	}
}

// qsort � ������� ������ �� ������ �����
template<typename T>
void qsort( int l, int r, T *a) // start, finish � ��������� �� ������ �������.
{
	if( l >= r )
		return;
	int x = a[(l + r) / 2];
	int i = l, j = r;
	while( i <= j ) {
		while( a[i] < x )
			++i;
		while( a[j] > x )
			--j;
		if( i <= j ) {
			swap( a[i], a[j] );
			++i, --j;
		}
	}
	qsort( a, l, j );
	qsort( a, i, r );
}


} // namespace my

// anti-qsort-debug
/*int qsort( int *a, int l, int r )
{
	int cnt = 0;
	if( l >= r )
		return 0;
	int x = a[(l + r) / 2];
	int i = l, j = r;
	while( i <= j ) {
		while( a[i] < x ) ++i;
		while( a[j] > x ) --j;
		if( i <= j ) {
			my::swap( a[i], a[j] );
			cnt++;
			++i, --j;
		}
	}
	printf( "%d %d\n", l - j, i - r );
	int cnt1 = qsort( a, l, j );
	int cnt2 = qsort( a, i, r );
	return cnt + cnt1 + cnt2;
}

int main()
{
	int a[7];
	for( int i = 0; i < 7; ++i ) {
		scanf( "%d", &(a[i]) );
	}
	printf( "%d", qsort( a, 0, 6 ) );
	return 0;
}*/