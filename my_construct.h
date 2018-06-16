#pragma once
#ifndef MY_CONSTRUCT_H
#define MY_CONSTRUCT_H
#include"my_config.h"
#include"my_type_traits.h"
#include<new>
#include<cstring>

_PRC_BEGIN
//基本版本construct
template<typename T1,typename T2>
inline void construct(T1* p, const T2& value)
{
	new (p) T1(value); //placement_new 在指针p指向的位置调用T1::T1(value)
}


//基本版本destroy
template<class T>
inline void destroy(T* pointer)
{
	pointer->~T(); //显示调用析构函数
}


template<typename ForwardIterator,typename T>
inline void _destroy(ForwardIterator first, ForwardIterator last, T*)
{
	using trival_destructor = typename _type_traits<T>::has_trivial_destructor;
	_PRC _destroy_aus(first, last, trival_destructor());
}


//接受迭代器版本的destroy
template<typename ForwardIterator> //单向迭代器
inline void destroy(ForwardIterator first, ForwardIterator last)
{
	_PRC _destroy(first, last,value_type(first));
}
//如果认为其析构函数有意义non-trival 就反复调用析构函数
template<typename ForwardIterator>
inline void
_destroy_aus(ForwardIterator first, ForwardIterator last, _false_type)
{
	for (;first < last;++first)
		_PRC destroy(&(*first));
}
//如果认为 其析构函数没有意义 trival ,则反复调用析构函数没有意义,则什么也不做
template<typename ForwardIterator>
inline void _destroy_aus(ForwardIterator first, ForwardIterator last, _true_type) {};

//char* 特化版本
inline void destroy(char*, char*) {};
//wchar_t* 特化版本
inline void destroy(wchar_t*, wchar_t*) {};

_PRC_END
#endif