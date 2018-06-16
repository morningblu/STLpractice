/*������Ҫ����POD����Ҳ���� Plain Old Data
* POD�ͱ��Ȼ�� trival(�������) ctor(���캯��)/dtor(��������)/copy(�������캯��)/assignment(��ֵ����)
*���Կ��Բ�������Ч�ʵĳ�ֵ��д�ַ�
*non-POD���ýϰ�ȫ������
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

//����[result,result+(last-first)) �����ڶ���δ��ʼ����������п������� copy constructor
template<typename InputIterator,typename ForwardIterator>
ForwardIterator
uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result)
{
	return _PRC _uninitialized_copy(first, last, result, value_type(result));
}
//uninitialized_copy ʵ�ʵ��ú���
template<typename InputIterator, typename ForwardIterator,typename signal>
ForwardIterator
_uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result,signal*)
{
	using is_POD = typename _PRC _type_traits<signal>::is_POD_type;
	return _PRC _uninitialized_copy_aus(first, last, result, is_POD());
}

//_uninitialized_copyʵ�ʵ��õ�����֮һ
template<typename InputIterator, typename ForwardIterator>
ForwardIterator
_uninitialized_copy_aus(InputIterator first, InputIterator last, ForwardIterator result,_true_type)
{
	return _STL_OR_PRC copy(first, last, result);
}

//_uninitialized_copyʵ�ʵ��õ�����֮��
template<typename InputIterator, typename ForwardIterator>
ForwardIterator
_uninitialized_copy_aus(InputIterator first, InputIterator last, ForwardIterator result, _false_type)
{
	ForwardIterator cur = result;
	for (;first != last;++first, ++cur)
		_PRC construct(&(*cur), *first);
	return cur;
}
//char* �ػ��汾
inline char* uninitialized_copy(const char* first, const char* last, char* result)
{
	std::memmove(result, first, last - first);
	return result + (last - first);
}
//wchar_t* �ػ��汾
inline wchar_t * uninitialized_copy(const wchar_t * first, const wchar_t* last, wchar_t* result)
{
	std::memmove(result, first, sizeof(wchar_t)*(last - first));
	return result + (last - first);
}


//���[first,last)��Χ�ڵ�ÿ����������ָ��δ��ʼ���ڴ��򿽱�x
template<typename ForwardIterator,typename T>
void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x) 
{
	_PRC _uninitialized_fill(first, last, x, value_type(first));
}

//uninitialized_fill��ʵ�ʵ���
template<typename ForwardIterator, typename T,typename signal>
void _uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x,signal*)
{
	using is_POD = typename _PRC _type_traits<signal>::is_POD_type;
	_PRC _uninitialized_fill_aus(first, last, x, is_POD());
}

//_uninitialized_fillʵ�ʵ���,��һ����
template<typename ForwardIterator, typename T>
void _uninitialized_fill_aus(ForwardIterator first, ForwardIterator last, const T& x,_true_type)
{
	_STL_OR_PRC fill(first, last, x);
}

//_uninitialized_fillʵ�ʵ���,��һ����
template<typename ForwardIterator, typename T>
void _uninitialized_fill_aus(ForwardIterator first, ForwardIterator last, const T& x, _false_type)
{
	ForwardIterator cur = first;
	for (;cur != last;++cur)
	_PRC construct(&(*cur), x);
}

//���[first,first+n) ��ÿһ����������ָ��δ��ʼ��Ԫ�������copy constructor
template<typename ForwardIterator,typename Size, typename T>
ForwardIterator
uninitialized_fill_n(ForwardIterator first, Size n, const T& x)
{
	return _PRC _uninitialized_fill_n(first, n, x, value_type(first));

}


//uninitialized_fill_n��ʵ�ʵ��ú���
template<typename ForwardIterator, typename Size, typename T,typename signal>
ForwardIterator
_uninitialized_fill_n(ForwardIterator first, Size n, const T& x,signal*)
{
	using is_POD= typename  _PRC _type_traits<T>::is_POD_type ;
	return _PRC _uninitialized_fill_n_aus(first, n, x, is_POD());
}

//_uninitialized_fill_n��ʵ�ʵ��ú�����һ����,Ҳ��������POD
template<typename ForwardIterator, typename Size, typename T>
ForwardIterator
_uninitialized_fill_n_aus(ForwardIterator first, Size n, const T& x,_true_type)
{
	return _STL_OR_PRC fill_n(first, n, x);
}

//_uninitialized_fill_n��ʵ�ʵ��ú����ڶ�����,Ҳ���ǲ�����POD
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
