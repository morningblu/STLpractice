#pragma once
#ifndef MY_PRIORITY_QUEUE_H
#define MY_PRIORITY_QUEUE_H
#include"my_heap.h"
#include"my_vector.h"
_PRC_BEGIN
template<class T,class Sequence=vector<T>,
class Compare=std::less<typename Sequence::value_type>>
class priority_queue {
public:
	using value_type = typename Sequence::value_type;
	using size_type = typename Sequence::size_type;
	using reference = typename Sequence::reference;
	using const_reference = typename Sequence::const_reference;
protected:
	Sequence c; //底层容器
	Compare comp;//元素比较大小的标准
public:
	priority_queue():c(),comp(Compare()) {}
	explicit priority_queue(const Compare& _comp):c(), comp(_comp) {}
	priority_queue(const std::initializer_list<value_type>& il) :c(il.begin(), il.end()), comp(Compare()) {
		_STL_OR_PRC  make_heap(c.begin(),c.end(), comp);
	}
	priority_queue(const priority_queue<value_type, Sequence, Compare>& pq) :c(pq.c), comp(pq.comp) {}
	priority_queue<value_type, Sequence, Compare>&
		operator=( const priority_queue<value_type, Sequence, Compare>& pq) { 
		c = pq.c;
		comp = pq.comp;
		return *this;
	}
	template<class InputIterator>
	priority_queue(InputIterator first, InputIterator last, const Compare& _comp=Compare())
		: c(first, last), comp(_comp) {
		_STL_OR_PRC  make_heap(c.begin(), c.end(), comp);
	}

	//基本操作
	bool empty()const { return c.empty(); }
	size_type size() const { return c.size(); }
	const_reference top() const { return c.front(); }
	void push(const value_type& x) {
		try {
			c.push_back(x);
		_STL_OR_PRC push_heap(c.begin(), c.end(), comp);
		}
		catch (...) {
			c.clear();
		}
	}

	void pop() {
		try{
			_STL_OR_PRC  pop_heap(c.begin(), c.end(), comp);
			c.pop_back();
		}
		catch (...) {
			c.clear();
		}
	}
};









_PRC_END
#endif // !MY_PRIORITY_QUEUE_H
