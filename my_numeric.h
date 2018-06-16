#pragma once
#ifndef MY_NUMERIC_H
#define MY_NUMERIC_H
#include"my_iterator.h"
_PRC_BEGIN

//accumulate�汾1,����Ĭ�ϵļӷ����㽫[first,last)Ԫ������ټӵ�init�Ϸ���
template<class  InputIterator,class T>
T accumulate(InputIterator first, InputIterator last, T init)
{//init Ϊ��ֵ
	for (;first != last;++first)
		init = init + *first;
	return init;
}
//accumulate�汾2,����binary_op Ҳ���Ƕ�Ԫ������������,��Ϊ���Ƽӷ�������,��[first,last)�ڵ�Ԫ����init����binary_op(init,*first);
template<class InputIterator ,class T,class BinaryOperation>
T accumulate(InputIterator first, InputIterator last, T init, const BinaryOperation binary_op)
{
	for (;first != last;++first)
		init = binary_op(init, *first);
	return init;
}


/*adjacent_difference�汾1,����Ĭ�ϼ���,����Ϊ����Ԫ��֮��
������Ԫ�ر��ֲ��������outputiterator,�����������Ӧ������
��ֵ������ǰһ��ֵ�Ĳ�,��������ֵ����������һ��λ�õ���һ��λ��
*/
template<class InputIterator ,class OutputIterator>
OutputIterator adjacent_difference(InputIterator first, InputIterator last,
	                               OutputIterator result)
{
	if (first == last)return result;
	*result = *first;   //��first����ֵ����result;
	return _adjacent_difference(first, last, result, value_type(first));
}
template<class InputIterator,class OutputIterator,class T>
inline
OutputIterator _adjacent_difference(InputIterator first, InputIterator last,
	                                OutputIterator result, T*)
{
	T value = *first;
	while (++first != last) {
		T tmp = *first;
		*(++result) = tmp - value;
		value = tmp;
	}
	return ++result;
}


/*adjacent_difference�汾2,���ú�������������Ƽ�������,����Ϊ����Ԫ��֮��
������Ԫ�ر��ֲ��������outputiterator,�����������Ӧ������
��ֵ������ǰһ��ֵ�Ĳ�,��������ֵ����������һ��λ�õ���һ��λ��
*/
template<class InputIterator, class OutputIterator,class BinaryOperation>
OutputIterator adjacent_difference(InputIterator first, InputIterator last,
	                               OutputIterator result,const BinaryOperation binary_op)
{
	if (first == last)return result;
	*result = *first;   //��first����ֵ����result;
	return _adjacent_difference(first, last, result, value_type(first),binary_op);
}
template<class InputIterator, class OutputIterator, class T,class BinaryOperation>
inline
OutputIterator _adjacent_difference(InputIterator first, InputIterator last,
	                                OutputIterator result, T*,
	                                const BinaryOperation binary_op)
{
	T value = *first;
	while (++first != last) {
		T tmp = *first;
		*(++result) = binary_op(tmp, value);
		value = tmp;
	}
	return ++result;
}

//inner_product�汾1��������������Ԫ�ص��ڻ�,����init�����ֵ
template<class InputIterator1,class InputIterator2,class T>
T inner_product(InputIterator1 first1, InputIterator2 last1,
	            InputIterator2 first2, T init)
{
	for (;first1 != last1;++first1, ++first2)
		init = init + ((*first1)* (*first2));
	return init;
}

//inner_product�汾2,�����������������ڻ�,���е�һ�����������������ӷ�����,�ڶ���Ϊ�˷�
template<class InputIterator1,class InputIterator2,class T,
         class BinaryOperation1,class BinaryOperation2>
T inner_product(InputIterator1 first1, InputIterator1 last1,
		        InputIterator2 first2, T init, 
	            const BinaryOperation1 binary_op1,
		        const BinaryOperation2 binary_op2)
{
	for (;first1 != last1;++first1, ++first2)
		//binary_op1����ӷ�����ĺ�������,binary_op2����˷�����ĺ�������
		init = binary_op1(init, binary_op2(*first1, *first2));
	return init;
}


/*partial_sum�汾1,�����ÿһ��outputiterator it,
���Ҽ����ʼ��outputiterator Ϊresult
��ֵΪ[first,(it-result)+first+1);��Ԫ�صĺ�
*/
template<class InputIterator,class OutputIterator>
OutputIterator partial_sum(InputIterator first, InputIterator last,
	                       OutputIterator result)
{
	if (first == last) return result;
	*result = *first;
	return _partial_sum(first, last, result, value_type(first));
}

template<class InputIterator,class OutputIterator,class T>
inline
OutputIterator _partial_sum(InputIterator first, InputIterator last,
	                        OutputIterator result, T*)
{
	T value = *first;
	while (++first != last) {
		value = value + *first;
		*(++result) = value;
	}
	return ++result;
}

/*partial_sum�汾2,BinaryOperation �����������
�����ÿһ��outputiterator it,
���Ҽ����ʼ��outputiterator Ϊresult
��ֵΪ[first,(it-result)+first+1);��Ԫ�صĺ�
*/
template<class InputIterator, class OutputIterator,class BinaryOperation>
OutputIterator partial_sum(InputIterator first, InputIterator last,
	                       OutputIterator result,const BinaryOperation binary_op)
{
	if (first == last) return result;
	*result = *first;
	return _partial_sum(first, last, result, value_type(first),binary_op);
}

template<class InputIterator, class OutputIterator, class T,class BinaryOperation>
inline
OutputIterator _partial_sum(InputIterator first, InputIterator last,
	                        OutputIterator result, T*,
	                        const BinaryOperation binary_op)
{
	T value = *first;
	while (++first != last) {
		value = binary_op(value,*first);
		*(++result) = value;
	}
	return ++result;
}


_PRC_END
#endif  

