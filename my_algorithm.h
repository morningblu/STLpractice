#pragma once
#ifndef MY_ALGORITHM_H
#define MY_ALGORITHM_H
#include"my_algobase.h"
#include"my_heap.h"
#include"my_iterator.h"
_PRC_BEGIN
//adjacent_find版本1,找出第一组满足条件的相邻元素,这里所谓条件就是相等
template<class ForwardIterator>
ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last) {
	if (first == last)return last;
	ForwardIterator next = first;
	while (++next != last) {
		if (*first == *next) return first;
		first = next;
	}
	return last;
}
//adjacent_find版本2,找出第一组满足条件的相邻元素,此处所谓条件是第三个参数binary_pred
template<class ForwardIterator,class BinaryPredicate>
ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last,
    	                      const BinaryPredicate binary_pred) {
	if (first == last)return last;
	ForwardIterator next = first;
	while (++next != last) {
	//如果相邻元素符合外界指定的条件就结束
		if (binary_pred(*first, *next)) return first;
		first = next;
	}
	return last;
}
//count,用以计数在序列[first,last)中有多少个value
template<class InputIterator,class T>
typename Iterator_traits<InputIterator>::difference_type
count(InputIterator first, InputIterator last, const T& value) {
	//计数
	typename Iterator_traits<InputIterator>::difference_type n = 0;
	for (;first != last;++first)
		if (*first == value)
			++n;
	return n;
}

//count_if,通过第三个一元逻辑对象进行逻辑判断,满足pred条件的对象有几个
template<class InputIterator,class Predicate>
typename Iterator_traits<InputIterator>::difference_type
count_if(InputIterator first, InputIterator last,
	     const Predicate pred) {
//以下声明计数
	typename Iterator_traits<InputIterator>::difference_type n = 0;
	for (;first != last;++first)
		if (pred(*first))
			++n;
	return n;
}
//find,从左到右的从区间[first,last)中寻找于value 相等的函数
template<class InputIterator,class T>
InputIterator find(InputIterator first, InputIterator last, const T& value) {
	while (first != last && *first != value)
		++first;
	return first;
}

//find_if,第三个参数是逻辑判断运算,find_if也就是在区间[first,last)中从左到右的寻找满足pred的第一个元素
template<class InputIterator,class Predicate>
InputIterator find_if(InputIterator first, InputIterator last,
	                  const Predicate pred)
{
	while (first != last && !pred(*first)) 
		++first;
	return first;
}
/*此算法在序列[frst1,last1) 所涵盖的区间中,查找序列2[first2,last2)最后一次出现的位置
匹配失败返回 last1
*/
template<class ForwardIterator1,class ForwardIterator2>
inline ForwardIterator1
find_end(ForwardIterator1 first1, ForwardIterator1 last1,
	     ForwardIterator2 first2, ForwardIterator2 last2)
{
	using category1 =typename Iterator_traits<ForwardIterator1>::iterator_category;
	using category2 =typename Iterator_traits<ForwardIterator2>::iterator_category;
	return _find_end(first1, last1, first2, last2, category1(), category2());
}

template<class ForwardIterator1,class ForwardIterator2>
ForwardIterator1 _find_end(ForwardIterator1 first1, ForwardIterator1 last1,
	                       ForwardIterator2 first2, ForwardIterator2 last2,
	                       forward_iterator_tag, forward_iterator_tag) {
	if (first2 == last2) //如果查找目标是空的
		return last1;    //返回last1,表示空序列最后出现地点
	else {
		ForwardIterator1 result = last1;
		while (true) {
		//以下利用search()查找某子序列的首次出现点,找不到就返回last1
			ForwardIterator1 new_result = search(first1, last1, first2, last2);
			if (new_result == last1)//如果没找到
				return result;
			else {//如果找到的话,就移动标兵重新匹配
				result = new_result;
				first1 = new_result;
				++first1;
			}
		}
	}
}

template<class BidirectionalIterator1, class BidirectionalIterator2>
BidirectionalIterator1 _find_end(BidirectionalIterator1 first1, BidirectionalIterator1 last1,
	                             BidirectionalIterator2 first2, BidirectionalIterator2 last2,
	                             bidirectional_iterator_tag, bidirectional_iterator_tag) {
	using reviter1 = std::reverse_iterator<BidirectionalIterator1>;
	using reviter2 = std::reverse_iterator<BidirectionalIterator2>;
	reviter1 rlast1(first1);
	reviter2 rlast2(first2);

	reviter1 rresult = search(reviter1(last1), rlast1, reviter2(last2), rlast2);
	if (rresult==rlast1) //如果没找到
		return last1;    //返回last1,表示空序列最后出现地点
	else {
		BidirectionalIterator1 result = rresult.base();
		std::advance(result, - _STL_OR_PRC distance(first2, last2));
		return result;
	}
}




_PRC_END
#endif // !MY_ALGORITHM
