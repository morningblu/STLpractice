#pragma once
#ifndef MY_STACK_H
#define MY_STACK_H

#include "my_deque.h"
_PRC_BEGIN

template<class T,class Sequence=deque<T>>
class stack {
public:
	using value_type = typename Sequence::value_type;
	using size_type = typename Sequence::size_type;
	using reference = typename Sequence::reference;
	using const_reference = typename Sequence::const_reference;
protected:
	Sequence c;
public:
	stack():c() {}
	template<class InputIterator >
	stack(InputIterator first, InputIterator last) : c(first, last) {}
	stack(const std::initializer_list<value_type>&il):c(il) {}
	stack(const stack<value_type, Sequence>& st):c(st.c) {}
	stack<value_type, Sequence> operator=(const stack<value_type, Sequence>& st) { 
		c = st.c; 
		return *this;
	}

	bool empty() const { return c.empty(); }
	size_type size() const { return c.size(); }
	reference top() { return c.back(); }
	const_reference top() const { return  c.back(); }
	void push(const value_type& x) { c.push_back(x); }
	void pop() { c.pop_back(); }
	Sequence& _Get_container() { return c; }
};

template<class T, class Sequence>
bool operator==(const stack<T, Sequence>& x, const stack<T, Sequence>& y) {
	return x._Get_container() == y._Get_container();
}

template<class T, class Sequence>
bool operator<(const stack<T, Sequence>& x, const stack<T, Sequence>& y) {
	return x._Get_container() < y._Get_container();
}


_PRC_END
#endif // !MY_STACK_H

