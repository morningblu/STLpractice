/*
��ͷ�ļ�û������heap ֻ�н���,������,֮����㷨
*/
#pragma once
#ifndef MY_HEAP_H
#define MY_HEAP_H
#include"my_iterator.h"

_PRC_BEGIN
template<class RandomAccessIterator,class compare>
inline void push_heap(RandomAccessIterator first, RandomAccessIterator last,const compare& comp) {
	_push_heap_aux(first, last, distance_type(first), value_type(first),comp);
}

template<class RandomAccessIterator,class Distance,class T,class compare>
inline void _push_heap_aux(RandomAccessIterator first, RandomAccessIterator last,
	Distance*, T*,compare& comp)
{
	_push_heap(first, Distance(last - first - 1), Distance(0), T(*(last - 1)),comp);
}

template<class RandomAccessIterator, class Distance,class T,class compare>
void _push_heap(RandomAccessIterator first, Distance holeIndex, Distance topIndex, T value,compare& comp) {
	Distance parent = (holeIndex - 1) / 2;//���ڵ��λ��
	while (holeIndex > topIndex &&comp(*(first + parent),value) ){
	//����δ���ﶥ��,�Ҹ��ڵ�С����ֵ(���ǲ�����heap�Ĵ�������)
	//�˴�β�󶥶�
		*(first + holeIndex) = *(first + parent);//�Ѹ��ڵ��ֵ��ֵ��holeλ��
		holeIndex = parent;
		parent = (holeIndex - 1) / 2;
	}
	/*ѭ��������ʱ��,holeIndexָ��һ��λ��,���λ���ϵ�ֵ�Ѿ��������ӽڵ�,�����λ��
	*�����λ�õĸ��ڵ�һ��������,��ʱ���λ�þ��ǲ����λ��,���²���Ϊ����
	*/
	*(first + holeIndex) = value;
}

template<class RandomAccessIterator ,class compare>
inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last,const compare& comp) {
	_pop_heap_aux(first, last, value_type(first),comp);
}

template<class RandomAccessIterator ,class T,class compare>
inline void _pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last, T*,compare& comp) {
	_pop_heap(first, last - 1, last-1,T(*(last - 1)), distance_type(first),comp);
}

template<class RandomAccessIterator,class T,class Distance,class compare>
inline void _pop_heap(RandomAccessIterator first, RandomAccessIterator last,
	RandomAccessIterator result, T value, Distance*,compare &comp) {
	*result = *first; //��βֵ������ֵ
	_adjust_heap(first, Distance(0), Distance(last - first), value,comp);
	//���������µ�����
}

template<class RandomAccessIterator,class Distance,class T,class compare>
void _adjust_heap(RandomAccessIterator first, Distance holeIndex, Distance len, T value,compare& comp) {
	Distance topIndex = holeIndex;
	Distance secondChild = 2 * holeIndex + 2; //holeIndex �����ӽڵ�
	while (secondChild < len) {
	//�Ƚ�holeIndex�ڵ�֮����������ֵ,Ȼ����secondChild����ϴ��ӽڵ�
		if (comp(*(first + secondChild) , *(first + (secondChild - 1))))
			--secondChild;
	// percolate down
		*(first + holeIndex) = *(first + secondChild);//���ϴ���ӽڵ��ֵǰ�Ƶ��丸�ڵ�
     //��holeIndex������ֵ,Ҳ����������Ϊ���ڵ����ѭ��
		holeIndex = secondChild;
		secondChild = 2 * (secondChild + 1);
	}
	if (secondChild == len) {
	//û�����ӽڵ�,ֻ�����ӽڵ�
		*(first + holeIndex) = *(first + (secondChild - 1));
		holeIndex = secondChild - 1;
	}
	_push_heap(first, holeIndex, topIndex, value,comp);
}

template<class RandomAccessIterator ,class compare>
void sort_heap(RandomAccessIterator first,
               RandomAccessIterator last,const compare& comp) {
	while (last - first > 1)
	{
		prc::pop_heap(first, last, comp);
		--last;
	}
}

template<class RandomAccessIterator ,class compare>
inline void make_heap(RandomAccessIterator first,
                      RandomAccessIterator last,compare& comp) {
	_make_heap(first, last, value_type(first), distance_type(first),comp);
}

template<class RandomAccessIterator,class T,class Distance,class compare>
void _make_heap(RandomAccessIterator first,
	RandomAccessIterator last, T*,
	Distance*,compare& comp) {
	if (last - first < 2) return; //����С��2 ������������
	Distance len = last - first;
	Distance parent=(len - 2) / 2;

	while (true) { 
		_adjust_heap(first, parent, len, T(*(first + parent)),comp);
		if (parent == 0) return;
		parent--;
	}
}







_PRC_END
#endif // !MY_HEAP_H
