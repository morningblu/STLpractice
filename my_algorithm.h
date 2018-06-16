#pragma once
#ifndef MY_ALGORITHM_H
#define MY_ALGORITHM_H
#include"my_algobase.h"
#include"my_heap.h"
#include"my_iterator.h"
_PRC_BEGIN
//adjacent_find�汾1,�ҳ���һ����������������Ԫ��,������ν�����������
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
//adjacent_find�汾2,�ҳ���һ����������������Ԫ��,�˴���ν�����ǵ���������binary_pred
template<class ForwardIterator,class BinaryPredicate>
ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last,
    	                      const BinaryPredicate binary_pred) {
	if (first == last)return last;
	ForwardIterator next = first;
	while (++next != last) {
	//�������Ԫ�ط������ָ���������ͽ���
		if (binary_pred(*first, *next)) return first;
		first = next;
	}
	return last;
}
//count,���Լ���������[first,last)���ж��ٸ�value
template<class InputIterator,class T>
typename Iterator_traits<InputIterator>::difference_type
count(InputIterator first, InputIterator last, const T& value) {
	//����
	typename Iterator_traits<InputIterator>::difference_type n = 0;
	for (;first != last;++first)
		if (*first == value)
			++n;
	return n;
}

//count_if,ͨ��������һԪ�߼���������߼��ж�,����pred�����Ķ����м���
template<class InputIterator,class Predicate>
typename Iterator_traits<InputIterator>::difference_type
count_if(InputIterator first, InputIterator last,
	     const Predicate pred) {
//������������
	typename Iterator_traits<InputIterator>::difference_type n = 0;
	for (;first != last;++first)
		if (pred(*first))
			++n;
	return n;
}
//find,�����ҵĴ�����[first,last)��Ѱ����value ��ȵĺ���
template<class InputIterator,class T>
InputIterator find(InputIterator first, InputIterator last, const T& value) {
	while (first != last && *first != value)
		++first;
	return first;
}

//find_if,�������������߼��ж�����,find_ifҲ����������[first,last)�д����ҵ�Ѱ������pred�ĵ�һ��Ԫ��
template<class InputIterator,class Predicate>
InputIterator find_if(InputIterator first, InputIterator last,
	                  const Predicate pred)
{
	while (first != last && !pred(*first)) 
		++first;
	return first;
}
/*���㷨������[frst1,last1) �����ǵ�������,��������2[first2,last2)���һ�γ��ֵ�λ��
ƥ��ʧ�ܷ��� last1
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
	if (first2 == last2) //�������Ŀ���ǿյ�
		return last1;    //����last1,��ʾ�����������ֵص�
	else {
		ForwardIterator1 result = last1;
		while (true) {
		//��������search()����ĳ�����е��״γ��ֵ�,�Ҳ����ͷ���last1
			ForwardIterator1 new_result = search(first1, last1, first2, last2);
			if (new_result == last1)//���û�ҵ�
				return result;
			else {//����ҵ��Ļ�,���ƶ��������ƥ��
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
	if (rresult==rlast1) //���û�ҵ�
		return last1;    //����last1,��ʾ�����������ֵص�
	else {
		BidirectionalIterator1 result = rresult.base();
		std::advance(result, - _STL_OR_PRC distance(first2, last2));
		return result;
	}
}




_PRC_END
#endif // !MY_ALGORITHM
