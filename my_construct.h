#pragma once
#ifndef MY_CONSTRUCT_H
#define MY_CONSTRUCT_H
#include"my_config.h"
#include"my_type_traits.h"
#include<new>
#include<cstring>

_PRC_BEGIN
//�����汾construct
template<typename T1,typename T2>
inline void construct(T1* p, const T2& value)
{
	new (p) T1(value); //placement_new ��ָ��pָ���λ�õ���T1::T1(value)
}


//�����汾destroy
template<class T>
inline void destroy(T* pointer)
{
	pointer->~T(); //��ʾ������������
}


template<typename ForwardIterator,typename T>
inline void _destroy(ForwardIterator first, ForwardIterator last, T*)
{
	using trival_destructor = typename _type_traits<T>::has_trivial_destructor;
	_PRC _destroy_aus(first, last, trival_destructor());
}


//���ܵ������汾��destroy
template<typename ForwardIterator> //���������
inline void destroy(ForwardIterator first, ForwardIterator last)
{
	_PRC _destroy(first, last,value_type(first));
}
//�����Ϊ����������������non-trival �ͷ���������������
template<typename ForwardIterator>
inline void
_destroy_aus(ForwardIterator first, ForwardIterator last, _false_type)
{
	for (;first < last;++first)
		_PRC destroy(&(*first));
}
//�����Ϊ ����������û������ trival ,�򷴸�������������û������,��ʲôҲ����
template<typename ForwardIterator>
inline void _destroy_aus(ForwardIterator first, ForwardIterator last, _true_type) {};

//char* �ػ��汾
inline void destroy(char*, char*) {};
//wchar_t* �ػ��汾
inline void destroy(wchar_t*, wchar_t*) {};

_PRC_END
#endif