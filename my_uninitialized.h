/*以下主要利用POD技术也就是 Plain Old Data
* POD型别必然有 trival(无意义的) ctor(构造函数)/dtor(析构函数)/copy(拷贝构造函数)/assignment(赋值运算)
*所以可以采用最有效率的初值填写手法
*non-POD采用较安全的做法
*
*/

#pragma once
#ifndef MY_UNINITIALIZED_H
#define MY_UNINITIALIZED_H
#include"my_config.h"
#include"my_construct.h"
#ifdef _USE_STL
#include<algorithm>
#else
#include"my_algobase.h"
#endif // USE_STL
_PRC_BEGIN

//若在[result,result+(last-first)) 区域内都是未初始化区域则进行拷贝构造 copy constructor
template<typename InputIterator,typename ForwardIterator>
ForwardIterator
uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result)
{
	return _PRC _uninitialized_copy(first, last, result, value_type(result));
}
//uninitialized_copy 实际调用函数
template<typename InputIterator, typename ForwardIterator,typename signal>
ForwardIterator
_uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result,signal*)
{
	using is_POD = typename _PRC _type_traits<signal>::is_POD_type;
	return _PRC _uninitialized_copy_aus(first, last, result, is_POD());
}

//_uninitialized_copy实际调用的重载之一
template<typename InputIterator, typename ForwardIterator>
ForwardIterator
_uninitialized_copy_aus(InputIterator first, InputIterator last, ForwardIterator result,_true_type)
{
	return _STL_OR_PRC copy(first, last, result);
}

//_uninitialized_copy实际调用的重载之二
template<typename InputIterator, typename ForwardIterator>
ForwardIterator
_uninitialized_copy_aus(InputIterator first, InputIterator last, ForwardIterator result, _false_type)
{
	ForwardIterator cur = result;
	for (;first != last;++first, ++cur)
		_PRC construct(&(*cur), *first);
	return cur;
}
//char* 特化版本
inline char* uninitialized_copy(const char* first, const char* last, char* result)
{
	std::memmove(result, first, last - first);
	return result + (last - first);
}
//wchar_t* 特化版本
inline wchar_t * uninitialized_copy(const wchar_t * first, const wchar_t* last, wchar_t* result)
{
	std::memmove(result, first, sizeof(wchar_t)*(last - first));
	return result + (last - first);
}


//如果[first,last)范围内的每个迭代器都指向未初始化内存则拷贝x
template<typename ForwardIterator,typename T>
void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x) 
{
	_PRC _uninitialized_fill(first, last, x, value_type(first));
}

//uninitialized_fill的实际调用
template<typename ForwardIterator, typename T,typename signal>
void _uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x,signal*)
{
	using is_POD = typename _PRC _type_traits<signal>::is_POD_type;
	_PRC _uninitialized_fill_aus(first, last, x, is_POD());
}

//_uninitialized_fill实际调用,第一重载
template<typename ForwardIterator, typename T>
void _uninitialized_fill_aus(ForwardIterator first, ForwardIterator last, const T& x,_true_type)
{
	_STL_OR_PRC fill(first, last, x);
}

//_uninitialized_fill实际调用,第一重载
template<typename ForwardIterator, typename T>
void _uninitialized_fill_aus(ForwardIterator first, ForwardIterator last, const T& x, _false_type)
{
	ForwardIterator cur = first;
	for (;cur != last;++cur)
	_PRC construct(&(*cur), x);
}

//如果[first,first+n) 内每一个迭代器都指向未初始化元素则进行copy constructor
template<typename ForwardIterator,typename Size, typename T>
ForwardIterator
uninitialized_fill_n(ForwardIterator first, Size n, const T& x)
{
	return _PRC _uninitialized_fill_n(first, n, x, value_type(first));

}


//uninitialized_fill_n的实际调用函数
template<typename ForwardIterator, typename Size, typename T,typename signal>
ForwardIterator
_uninitialized_fill_n(ForwardIterator first, Size n, const T& x,signal*)
{
	using is_POD= typename  _PRC _type_traits<T>::is_POD_type ;
	return _PRC _uninitialized_fill_n_aus(first, n, x, is_POD());
}

//_uninitialized_fill_n的实际调用函数第一重载,也就是满足POD
template<typename ForwardIterator, typename Size, typename T>
ForwardIterator
_uninitialized_fill_n_aus(ForwardIterator first, Size n, const T& x,_true_type)
{
	return _STL_OR_PRC fill_n(first, n, x);
}

//_uninitialized_fill_n的实际调用函数第二重载,也就是不满足POD
template<typename ForwardIterator, typename Size, typename T>
ForwardIterator
_uninitialized_fill_n_aus(ForwardIterator first, Size n, const T& x, _false_type)
{
	ForwardIterator cur = first;
	for (;n > 0;--n, ++cur)
		_PRC construct(&(*cur), x);
	return cur;
}
_PRC_END
#endif // !MY_UNINITIALIZED_H
