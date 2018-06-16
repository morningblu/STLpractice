/*
���ļ����㷨Ϊ�����㷨,����stl��׼֮��,�����㷨��ȡ;��Ӧ����algorithm
*/
#pragma once
#ifndef MY_ALGOBASE_H
#define MY_ALGOBASE_H
#include"my_iterator.h"
#include"my_type_traits.h"
#include<cstdlib>
_PRC_BEGIN


//equal�汾1,�ж�[first1,last1) ��[first2,last1-first1+first2)��ÿһ��Ԫ���Ƿ����
template<class InputIterator1,class InputIterator2>
inline
bool equal(InputIterator1 first, InputIterator1 last1, InputIterator2 first2)
{
	for (;first1 != last1;++first1, ++first2) 
		if (*first1 != *first2)
			return false;
	return true;
}
//equal�汾2,����BinaryPredicate�����е�����,�ж�[first1,last1) ��[first2,last1-first1+first2)��ÿһ��Ԫ���Ƿ����
template<class InputIterator1,class InputIterator2,
         class BinaryPredicate>
inline 
bool equal(InputIterator1 first1,InputIterator1 last1,
		   InputIterator2 first2,const BinaryPredicate binary_pred ){
	for (;first1 != last1;++first1, ++first2)
		if (!binary_pred(*first1, *first2))
			return false;
	return true;
}

//�˺�����[first,last)��Χ�ڵ�Ԫ�ض����Ϊ value
template<class ForwardIterator,class T>
void fill(ForwardIterator first, ForwardIterator last, const T& value){
	for (;first != last;++first)
		*first = value;
}
//�˺�����[first,first+n)��Χ�ڵ�Ԫ�ض����Ϊ value
template<class OutputIterator,class Size,class T>
OutputIterator fill_n(OutputIterator first, Size n, const T& value) {
	for (;n > 0;--n, ++first)
		*first = value;
	return first;
}

//������������ָ֮����
template<class ForwardIterator1,class ForwardIterator2>
inline 
void iter_swap(ForwardIterator1 a, ForwardIterator2 b) {
	_iter_swap(a, b, value_type(a));
}
template<class ForwardIterator1,class ForwardIterator2,class T>
inline 
void _iter_swap(ForwardIterator1 a, ForwardIterator2 b, T*) {
	T tmp = *a;
	*a = *b;
	*b = tmp;
}

//max�汾1�����������ϴ���Ǹ�
template<class T>
inline const T& max(const T& a, const T& b) {
	return a < b ? b : a;
}
//max�汾2,�ṩ�Ƚϲ����ķ���compare,,����compare�ȽϺ������Ͻϴ���Ǹ�
template<class T, class Compare>
inline
const T& max(const T& a, const T& b, const Compare comp) {
	return comp(a, b) ? b : a;
}
//min�汾1,�������������н�С���Ǹ�
template<class T>
inline const T& min(const T& a, const T& b) {
	return b < a ? b : a;
}
//min�汾2,compare������С����,����ֵΪcompare�ȽϺ������ϱȽ�С���Ǹ�
template<class T, class Compare>
inline const T&min(const T& a, const T& b, const Compare comp) {
	return comp(b, a) ? b: a;
}

/*lexicograophical_compare�汾1��[first1,last1),[first2,last2)�ڵ���ͬλ���ϵ�Ԫ�ؽ��бȽ�
*�˴�λ��,ָ�������������Ե�first
*/
template<class InputIterator1,class InputIterator2>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
                          	 InputIterator2 first2, InputIterator2 last2)
{//�����κ�һ�����е���β��ѭ������
	for (;first != last1 && first2 != last2;++first1, ++first2) {
		if (*first1 < *first2)
			return true;
		if (*first2 < *first1)
			return false;
		//����������������ʾ���
	}
	//����һ�����б������,�ڶ���������δ����򷵻�true,
	return first1 == last1 && first2 != last2;

}
//lexicographical_compare�汾2,compare����������С,������[first1,last1),[first2,last2)�ڵ���ͬλ���ϵ�Ԫ�ؽ��бȽ�
template<class InputIterator1,class InputIterator2,class Compare>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
	                         InputIterator2 first2, InputIterator2 last2,
	                         const Compare comp)
{
	for (;first1 != last1 && first2 != last2; ++first1, ++first2) {
		if (comp(*first1, *first2))
			return true;
		if (comp(*first2, *first1))
			return false;
	
	}
	return first1 == last1 && first2 != last2;
}

//lexicographcal_compare�汾3,����޷����ַ�ָ����ػ�����
inline bool
lexicographical_compare(const unsigned char* first1,
	                    const unsigned char* last1,
                    	const unsigned char* first2,
	                    const unsigned char* last2)
{
	const size_t len1 = last1 - first1; //��һ���еĳ���
	const size_t len2 = last2 - first2; //�ڶ����еĳ���
	//�ȱȽϳ�����ͬ��һ��,��memcmp
	const int result = std::memcmp(first1, first2, min(len1, len2));
	return( result != 0) ? (result < 0 ):( len1<len2);
}
//lexicographcal_compare�汾4,����ַ�ָ����ػ�����
inline bool
lexicographical_compare(const char* first1,
	                    const char* last1,
	                    const char* first2,
	                    const char* last2)
{
	const size_t len1 = last1 - first1; //��һ���еĳ���
	const size_t len2 = last2 - first2; //�ڶ����еĳ���
										//�ȱȽϳ�����ͬ��һ��,��memcmp
	const int result = std::memcmp(first1, first2, min(len1, len2));
	return(result != 0) ? (result < 0) : (len1<len2);
}

//mismatch�汾1,���ص�һ������ȵ�һ�Ե�����
template<class InputIterator1,class InputIterator2>
std::pair<InputIterator1,InputIterator2>
mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2) {
	while (first1 != last1 && *first1 == *first2) {
		++first1;
		++first2;
	}
	return std::pair<InputIterator1, InputIterator2>(first1, first2);
}

//mismatch�汾2.binary_pred�����е�����,���ص�һ������ȵ�һ�Ե�����
template<class InputIterator1,class InputIterator2,class BinaryPredicate>
std::pair<InputIterator1,InputIterator2>
mismatch(InputIterator1 first1, InputIterator1 last1,
	     InputIterator2 first2,
	     const BinaryPredicate binary_pred)
{
	while (first1 != last1 && binary_pred(*first1, *first2)) {
		++first1;
		++first2;
	}
	return std::pair<InputIterator1, InputIterator2>(first1, first2);
}

//�򵥵Ľ�������,ʵ������PJstl����Ը���������ػ�����
template<class T>
inline void swap(T& a, T& b) {
	T& tmp = a;
	a = b;
	b = tmp;
}

//distance ����ûʲô�ý���,������δ�����Ϸ���,�ο�pj stl ʵ��

template<class InputIterator>
typename Iterator_traits<InputIterator>::difference_type
inline distance(InputIterator first, InputIterator last) {
	return _distance(first, last, iterator_category(first));
}
template<class InputIterator>
typename Iterator_traits<InputIterator>::difference_type
inline _distance(InputIterator first, InputIterator last,random_access_iterator_tag) {
	return (last-first);
}
template<class InputIterator>
typename Iterator_traits<InputIterator>::difference_type
inline _distance(InputIterator first, InputIterator last,input_iterator_tag) {
	typename Iterator_traits<InputIterator>::difference_type size = 0;
	for (;first != last;++first)
		++size;
	return size;
}


/*����Ϊcopy��ר��,copy�����Ը������������㵽�Ŀ���,
����������ĸ�ֵ���������ֱ�Ӳ���memmove����
*/

template<class InputIterator, class OutputIterator>
struct _copy_dispatch {
	OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) {
		return _copy(first, last, result, iterator_category(first));
	}
};
template<class T>
struct _copy_dispatch<T*, T*> {
	T* operator()(T* first, T* last, T* result) {
		using t = typename _type_traits<T>::has_trivial_assignment_operator;
		return _copy_t(first, last, result, t());
	}
};
template<class T>
struct _copy_dispatch<const T*, T*> {
	T* operator()(const T* first, const T* last, T* result) {
		using t = typename _type_traits<T>::has_trivial_assignment_operator;
		return _copy_t(first, last, result, t());
	}
};

template<class InputIterator,class OutputIterator>
inline OutputIterator copy(InputIterator first, InputIterator last,
	                       OutputIterator result)
{
	return _copy_dispatch<InputIterator, OutputIterator>()(first, last, result);
}
inline char* copy(const char* first, const char* last, char* result)
{
	std::memmove(result, first, last - first);
	return result + (last - first);
}
inline wchar_t* copy(const wchar_t*first, const wchar_t* last, wchar_t* result) {
	std::memmove(result, first, sizeof(wchar_t)*(last - first));
	return result + (last - first);
}


template<class InputIterator,class OutputIterator>
inline OutputIterator _copy(InputIterator first, InputIterator last,
	                        OutputIterator result,input_iterator_tag) {
	for (;first != last;++result, ++first)
		*result = *first;
	return result;
}

template<class RandomAccessIterator,class OutputIterator>
inline OutputIterator
_copy(RandomAccessIterator first, RandomAccessIterator last,
      OutputIterator result, random_access_iterator_tag) {
	return _copy_d(first, last, result, distance_type(first));
}

template<class RandomAccessIterator,class OutputIterator,class Distance>
inline OutputIterator
_copy_d(RandomAccessIterator first, RandomAccessIterator last,
	    OutputIterator result, Distance*) {
	for (Distance n = last - first;n > 0;--n, ++result, ++first)
		*result = *first;
	return result;
}
template<class T>
inline T* _copy_t(const T* first, const T* last, T* result, _true_type)
{
	std::memmove(result, first, sizeof(T)*(last - first));
	return result + (last - first);
}

template<class T>
inline T* _copy_t(const T* first, const T* last, T* result, _false_type) {
	return _copy_d(first, last, result, reinterpret_cast<ptrdiff_t*>(nullptr));
}

//������copy_backwardר��
//����STLԴ��������δ����Դ����,���´���ο�pj STL,��copy����Ʒ�ʽ��������ʵ��
template<class BidirectionalIterator1,class BidirectionalIterator2>
inline BidirectionalIterator2 copy_backward(BidirectionalIterator1 first,
	                                        BidirectionalIterator1 last,
	                                        BidirectionalIterator2 result) {
	using t = typename _type_traits<BidirectionalIterator1>::has_trivial_assignment_operator;
	return _copy_backward_t(first, last, result, t());
}

template<class BidirectionalIterator1, class BidirectionalIterator2>
inline BidirectionalIterator2 _copy_backward_t(BidirectionalIterator1 first,
	                                           BidirectionalIterator1 last,
	                                           BidirectionalIterator2 result,
                                               _false_type) {
	return _copy_backward(first, last, result);
}
template<class BidirectionalIterator1, class BidirectionalIterator2>
inline BidirectionalIterator2 _copy_backward_t(BidirectionalIterator1 first,
	                                           BidirectionalIterator1 last,
	                                           BidirectionalIterator2 result,
	                                           _true_type) {
	return _copy_backward_memmove(first, last, result);
}

template<class BidirectionalIterator1, class BidirectionalIterator2>
inline BidirectionalIterator2 _copy_backward(BidirectionalIterator1 first,
	                                         BidirectionalIterator1 last,
	                                         BidirectionalIterator2 result) {
	while (first != last)
		*(--result) = *(--last);
	return result;
}
template<class BidirectionalIterator1,class BidirectionalIterator2>
inline BidirectionalIterator2 _copy_backward_memmove(BidirectionalIterator1 first,
	                                                 BidirectionalIterator1 last,
	                                                 BidirectionalIterator2 result) {
	const char* const first_ch = const_cast<const char*>(reinterpret_cast<const volatile char*>(first));
	const char* const last_ch = const_cast<const char*>(reinterpret_cast<const volatile char*>(last));
	char* const result_ch = const_cast<char*>(reinterpret_cast<volatile char*>(result));
	const size_t count = last_ch - first_ch;
	return static_cast<BidirectionalIterator2>(
		std::memmove(result_ch - count, first_ch, count));
}


_PRC_END
#endif // !MY_ALGOBASE_H
