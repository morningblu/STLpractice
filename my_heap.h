/*
此头文件没有容器heap 只有建堆,堆排序,之类的算法
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
	Distance parent = (holeIndex - 1) / 2;//父节点的位置
	while (holeIndex > topIndex &&comp(*(first + parent),value) ){
	//当尚未到达顶端,且父节点小于新值(于是不符合heap的次序特性)
	//此处尾大顶堆
		*(first + holeIndex) = *(first + parent);//把父节点的值赋值给hole位置
		holeIndex = parent;
		parent = (holeIndex - 1) / 2;
	}
	/*循环结束的时候,holeIndex指向一个位置,这个位置上的值已经被赋给子节点,且这个位置
	*且这个位置的父节点一定比他大,此时这个位置就是插入的位置,以下操作为插入
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
	*result = *first; //将尾值赋给首值
	_adjust_heap(first, Distance(0), Distance(last - first), value,comp);
	//以上欲重新调整堆
}

template<class RandomAccessIterator,class Distance,class T,class compare>
void _adjust_heap(RandomAccessIterator first, Distance holeIndex, Distance len, T value,compare& comp) {
	Distance topIndex = holeIndex;
	Distance secondChild = 2 * holeIndex + 2; //holeIndex 的右子节点
	while (secondChild < len) {
	//比较holeIndex节点之左右两个子值,然后以secondChild代表较大子节点
		if (comp(*(first + secondChild) , *(first + (secondChild - 1))))
			--secondChild;
	// percolate down
		*(first + holeIndex) = *(first + secondChild);//将较大的子节点的值前移到其父节点
     //将holeIndex赋上新值,也就是重新作为父节点进行循环
		holeIndex = secondChild;
		secondChild = 2 * (secondChild + 1);
	}
	if (secondChild == len) {
	//没有右子节点,只有左子节点
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
	if (last - first < 2) return; //长度小于2 不必重新排列
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
