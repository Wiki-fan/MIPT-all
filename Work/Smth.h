﻿#pragma once

namespace my {

// Обмен значений двух элементов.
template <typename T>
inline void swap( T& a, T& b );

// Максимальное из значений. Использует operator>= .
template<typename T>
inline T max( const T& val1, const T& val2 );

} // namespace my
