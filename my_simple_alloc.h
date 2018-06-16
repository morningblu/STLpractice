#pragma once
#ifndef MY_SIMPLE_ALLOC_H
#define MY_SIMPLE_ALLOC_H

#include"my_config.h"

#include<cstddef>
#include<cstdlib>
#include<climits>

_PRC_BEGIN
template<typename T,typename Alloc>
class _simple_alloc {
public:
	using value_type      =        T;
	using pointer         =       T*;
	using const_pointer   = const T*;
	using reference       =       T&;
	using const_reference = const T&;
	using size_type       =   size_t;
	using difference_type =ptrdiff_t;

	static T *allocate(size_t n)
	{
		return 0 == n ? nullptr : reinterpret_cast<T*>(Alloc::allocate(n * sizeof(T)));
	}
	static T* allocate()
	{
		return reinterpret_cast<T*>(Alloc::allocate(sizeof(T)));
	}
	static void deallocate(T *p, size_t n) 
	{
		if (0 != n) Alloc::deallocate(p, n * sizeof(T));
	}
	static void deallocate(T *p)
	{
		Alloc::deallocate(p, sizeof(T));
	}
};

_PRC_END
#endif