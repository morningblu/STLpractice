#pragma once
#ifndef MY_NUMERIC_H
#define MY_NUMERIC_H
#include"my_iterator.h"
_PRC_BEGIN

//accumulate版本1,采用默认的加法运算将[first,last)元素相加再加到init上返回
template<class  InputIterator,class T>
T accumulate(InputIterator first, InputIterator last, T init)
{//init 为初值
	for (;first != last;++first)
		init = init + *first;
	return init;
}
//accumulate版本2,采用binary_op 也就是二元操作函数对象,作为类似加法的语义,将[first,last)内的元素与init进行binary_op(init,*first);
template<class InputIterator ,class T,class BinaryOperation>
T accumulate(InputIterator first, InputIterator last, T init, const BinaryOperation binary_op)
{
	for (;first != last;++first)
		init = binary_op(init, *first);
	return init;
}


/*adjacent_difference版本1,采用默认减法,所求为相邻元素之差
其中首元素保持不变输出到outputiterator,其他输出到相应迭代器
的值是其与前一个值的差,函数返回值是输出的最后一个位置的下一个位置
*/
template<class InputIterator ,class OutputIterator>
OutputIterator adjacent_difference(InputIterator first, InputIterator last,
	                               OutputIterator result)
{
	if (first == last)return result;
	*result = *first;   //将first处的值赋给result;
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


/*adjacent_difference版本2,采用函数对象进行类似减法语义,所求为相邻元素之差
其中首元素保持不变输出到outputiterator,其他输出到相应迭代器
的值是其与前一个值的差,函数返回值是输出的最后一个位置的下一个位置
*/
template<class InputIterator, class OutputIterator,class BinaryOperation>
OutputIterator adjacent_difference(InputIterator first, InputIterator last,
	                               OutputIterator result,const BinaryOperation binary_op)
{
	if (first == last)return result;
	*result = *first;   //将first处的值赋给result;
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

//inner_product版本1用以求两个容器元素的内积,其中init代表初值
template<class InputIterator1,class InputIterator2,class T>
T inner_product(InputIterator1 first1, InputIterator2 last1,
	            InputIterator2 first2, T init)
{
	for (;first1 != last1;++first1, ++first2)
		init = init + ((*first1)* (*first2));
	return init;
}

//inner_product版本2,用以求两个容器的内积,其中第一个函数对象参数代表加法语义,第二个为乘法
template<class InputIterator1,class InputIterator2,class T,
         class BinaryOperation1,class BinaryOperation2>
T inner_product(InputIterator1 first1, InputIterator1 last1,
		        InputIterator2 first2, T init, 
	            const BinaryOperation1 binary_op1,
		        const BinaryOperation2 binary_op2)
{
	for (;first1 != last1;++first1, ++first2)
		//binary_op1代表加法语义的函数对象,binary_op2代表乘法语义的函数对象
		init = binary_op1(init, binary_op2(*first1, *first2));
	return init;
}


/*partial_sum版本1,所求对每一个outputiterator it,
并且假设最开始的outputiterator 为result
其值为[first,(it-result)+first+1);内元素的和
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

/*partial_sum版本2,BinaryOperation 代表求和语义
所求对每一个outputiterator it,
并且假设最开始的outputiterator 为result
其值为[first,(it-result)+first+1);内元素的和
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

