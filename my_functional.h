/*
��ʱֻд��Ҫ�õ���һ���
*/
#pragma once
#ifndef MY_FUNCTIONAL_H
#define MY_FUNCTIONAL_H
#include"my_config.h"
_PRC_BEGIN
//һԪ����������ǩ
template<class Arg,class Result>
struct unary_function {
	using argument_type = Arg;
	using result_type = Result;
};
//��Ԫ����������ǩ
template<class Arg1,class Arg2,class Result>
struct binary_function {
	using first_argument_type = Arg1;
	using second_argument_type = Arg2;
	using result_type = Result;
};
//�ӷ��º���
template<class T>
struct plus :public binary_function<T, T, T> {
	T operator()(const T& x, const T& y)const { return x + y; }
};
//�����º���
template<class T>
struct minus :public binary_function<T, T, T> {
	T operator()(const T& x, const T& y)const { return x - y; }
};
//�˷��º���
template<class T>
struct multiplies :public binary_function<T, T, T> {
	T operator()(const T& x, const T& y)const { return x * y; }
};
//�����º���
template<class T>
struct divides :public binary_function<T, T, T> {
	T operator()(const T& x, const T& y)const { return x / y; }
};
//��mod�º���
template<class T>
struct modulus :public binary_function<T, T, T> {
	T operator()(const T& x, const T& y)const { return x % y; }
};
//ȡ�����º���
template<class T>
struct negate :public unary_function<T, T> {
	T operator()(const T& x)const { return -x; }
};
//֤ͬ,�������������ٶ�,����ͨ��ûʲô��Ҳ������STL�淶
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
//��ϵ�����е�
template<class T>
struct equal_to :public binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y)const { return x == y; }
};
//��ϵ���㲻��
template<class T>
struct not_equal_to :public binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y)const { return x != y; }
};
//��ϵ�������
template<class T>
struct  greater :public binary_function <T,T,bool>{
	bool operator()(const T& x, const T& y)const { return x > y; }
};
//��ϵ����С��
template<class T>
struct  less:public binary_function <T, T, bool> {
	bool operator()(const T& x, const T& y)const { return x < y; }
};
//��ϵ������ڵ���
template<class T>
struct  greater_equal :public binary_function <T, T, bool>{
	bool operator()(const T& x, const T& y)const { return x >= y; }
};
//��ϵ����С�ڵ���
template<class T>
struct  less_equal :public binary_function <T, T, bool> {
	bool operator()(const T& x, const T& y)const { return x <= y; }
};
//�߼�������
template<class T>
struct logical_and :public binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y)const { return x && y; }
};
//�߼�������
template<class T>
struct logical_or :public binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y)const { return x || y; }
};
//�߼�������
template<class T>
struct logical_not :public binary_function<T, T, bool> {
	bool operator()(const T& x, const T& y)const { return !x; }
};

//���²�����stl�淶����ͨ��stlʵ���ж���

//֤ͬ�º���
template<class T>
struct identity :public unary_function<T, T> {
	const T& operator()(const T& x)const { return x; }
};

//ѡ������pair����
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
//Ͷ�亯��,��δ��ʹ��
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
