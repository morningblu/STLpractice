/*
暂时只写了要用的这一点点
*/
#pragma once
#ifndef MY_FUNCTIONAL_H
#define MY_FUNCTIONAL_H
#include"my_config.h"
_PRC_BEGIN
//一元操作函数标签
template<class Arg,class Result>
struct unary_function {
	using argument_type = Arg;
	using result_type = Result;
};
//二元操作函数标签
template<class Arg1,class Arg2,class Result>
struct binary_function {
	using first_argument_type = Arg1;
	using second_argument_type = Arg2;
	using result_type = Result;
};
//加法仿函数
template<class T>
struct plus :public binary_function<T, T, T> {
	T operator()(const T& x, const T& y)const { return x + y; }
};
//减法仿函数
template<class T>
struct minus :public binary_function<T, T, T> {
	T operator()(const T& x, const T& y)const { return x - y; }
};
//乘法仿函数
template<class T>
struct multiplies :public binary_function<T, T, T> {
	T operator()(const T& x, const T& y)const { return x * y; }
};
//除法仿函数
template<class T>
struct divides :public binary_function<T, T, T> {
	T operator()(const T& x, const T& y)const { return x / y; }
};
//求mod仿函数
template<class T>
struct modulus :public binary_function<T, T, T> {
	T operator()(const T& x, const T& y)const { return x % y; }
};
//取负数仿函数
template<class T>
struct negate :public unary_function<T, T> {
	T operator()(const T& x)const { return -x; }
};
//证同,如果不明白意义百度,但是通常没什么用也不属于STL规范
template<class T>
inline
T identity_element(plus<T>)
{
	T return T(0);
}
template<class T>
inline
T identity_element(multiplies<T>)
{
	T return T(1);
}
//关系运算判等
template<class T>
struct equal_to :public binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y)const { return x == y; }
};
//关系运算不等
template<class T>
struct not_equal_to :public binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y)const { return x != y; }
};
//关系运算大于
template<class T>
struct  greater :public binary_function <T,T,bool>{
	bool operator()(const T& x, const T& y)const { return x > y; }
};
//关系运算小于
template<class T>
struct  less:public binary_function <T, T, bool> {
	bool operator()(const T& x, const T& y)const { return x < y; }
};
//关系运算大于等于
template<class T>
struct  greater_equal :public binary_function <T, T, bool>{
	bool operator()(const T& x, const T& y)const { return x >= y; }
};
//关系运算小于等于
template<class T>
struct  less_equal :public binary_function <T, T, bool> {
	bool operator()(const T& x, const T& y)const { return x <= y; }
};
//逻辑与运算
template<class T>
struct logical_and :public binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y)const { return x && y; }
};
//逻辑或运算
template<class T>
struct logical_or :public binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y)const { return x || y; }
};
//逻辑非运算
template<class T>
struct logical_not :public binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y)const { return !x; }
};

//以下不属于stl规范但是通常stl实现中都有

//证同仿函数
template<class T>
struct identity :public unary_function<T, T> {
	const T& operator()(const T& x)const { return x; }
};

//选择函数与pair搭配
template<class Pair>
struct select1st :public unary_function<Pair, typename Pair::first_type> {
	const typename Pair::first_type& operator()(const Pair& x)const {
		return x.first;
	}
};
template<class Pair>
struct select2nd :public unary_function<Pair, typename Pair::second_type> {
	const typename Pair::second_type& operator()(const Pair& x)const {
		return x.second;
	}
};
//投射函数,并未被使用
template<class Arg1,class Arg2>
struct project1st :public binary_function<Arg1, Arg2, Arg1> {
	Arg1 operator()(const Arg1& x, const Arg2&)const { return x; }
};

template<class Arg1, class Arg2>
struct project1st :public binary_function<Arg1, Arg2, Arg1> {
	Arg2 operator()(const Arg1& , const Arg2& y)const { return y; }
};













_PRC_END
#endif // !MY_FUNCTIONAL_H
