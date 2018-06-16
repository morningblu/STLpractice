#pragma once
#ifndef MY_QUEUE_H
#define MY_QUEUE_H
#include"my_deque.h"
_PRC_BEGIN
template<class T,class Sequence=deque<T>>
class queue {
public:
	using value_type = typename Sequence::value_type;
	using size_type = typename Sequence::size_type;
	using reference = typename Sequence::reference;
	using const_reference = typename Sequence::const_reference;

protected:
	Sequence c;
public:
	queue() :c() {}
	template<class InputIterator >
	queue(InputIterator first, InputIterator last) : c(first, last) {}
	queue(const std::initializer_list<value_type>&il) :c(il) {}
	queue(const stack<value_type, Sequence>& que) :c(que.c) {}
	queue<value_type, Sequence> operator=(const queue<value_type, Sequence>& que) {
		c = que.c;
		return *this;
	}


	bool empty()const { return c.empty(); }
	size_type size() const { return c.size(); }
	reference front() { return c.front(); }
	const_reference front() const { return c.front(); }
	reference back() { return c.back(); }
	const_reference back()const { return c.back(); }
	void push(const value_type& x) { c.push_back(x); }
	void pop() { c.pop_front(); }
	Sequence& _Get_container() { return c; }
};
template<class T, class Sequence>
bool operator==(const queue<T, Sequence>& x, const queue<T, Sequence>& y) {
	return x._Get_container() == y._Get_container();
}
template<class T, class Sequence>
bool operator<(const queue<T, Sequence>& x, const queue<T, Sequence>& y) {
	return x._Get_container()  < y._Get_container();
}

_PRC_END
#endif // !MY_QUEUE_H
