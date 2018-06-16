#pragma once
#ifndef MY_FORWARD_LIST_H
#define MY_FORWARD_LIST_H
#include"my_iterator.h"
#include"my_simple_alloc.h"
#include"my_default_alloc.h"
#include"my_uninitialized.h"
#include<algorithm>
_PRC_BEGIN
//单链表基本结构
struct _forward_list_node_base {
	_forward_list_node_base* next;
};

//单向链表节点结构
template<class T>
struct _forward_list_node :public _forward_list_node_base {
	T data;
};

//插入在某节点之后
inline _forward_list_node_base* _forward_list_make_link(
	_forward_list_node_base* prev_node,
	_forward_list_node_base* new_node
)
{
	new_node->next = prev_node->next;
	prev_node->next = new_node;
	return new_node;
}

//计数
inline size_t _forward_list_size(_forward_list_node_base* node)
{
	size_t result = 0;
	for (;node != nullptr;node = node->next)
		++result;
	return result;
}
//单向链表迭代器基类
struct _forward_list_iterator_base {
	using size_type = size_t;
	using difference_type = ptrdiff_t;
	using iterator_category = forward_iterator_tag;

	_forward_list_node_base* node; //指向节点基本结构

	_forward_list_iterator_base(_forward_list_node_base* x) :node(x) {}

	//前进一个节点
	void incr() { node = node->next; }

	bool operator==(const _forward_list_iterator_base& x)const {
		return node == x.node;
	}

	bool operator!=(const _forward_list_iterator_base& x)const {
		return node != x.node;
	}
};

template<class T,class Ref,class Ptr>
struct _forward_list_iterator :public _forward_list_iterator_base {
	using iterator = _forward_list_iterator<T, T&, T*>;
	using const_iterator = _forward_list_iterator<T, const T&, const T*>;
	using self = _forward_list_iterator<T, Ref, Ptr>;

	using value_type = T;
	using pointer = Ptr;
	using reference = Ref;
	using list_node = _forward_list_node<T>;
	//构造函数
	_forward_list_iterator(list_node* x) :_forward_list_iterator_base(x) {}
	_forward_list_iterator() :_forward_list_iterator_base(nullptr) {}
	_forward_list_iterator(const iterator& x) :_forward_list_iterator_base(x.node) {}

	//基本操作
	reference operator*()const { return dynamic_cast<list_node*>(node)->data; }
	pointer operator->() const { return &(operator*()); }
	self& operator++()
	{
		incr();
		return *this;
	}
	self operator++(int) {
		self tmp = *this;
		incr();
		return tmp;
	}
};

template<class T,class Alloc=alloc>
class forward_list {
public:
	using value_type = T;
	using pointer = value_type * ;
	using const_pointer = const value_type*;
	using reference = value_type & ;
	using const_reference = const value_type&;
	using size_type = size_t;
	using difference_type = ptrdiff_t;

	using iterator = _forward_list_iterator<T, T&, T*>;
	using const_iterator = _forward_list_iterator<T, const T&, const T*>;

private:
	using list_node = _forward_list_node<T>;
	using list_node_base = _forward_list_node_base;
	using iterator_base = _forward_list_iterator_base;
	using list_node_allocator = _simple_alloc<list_node, Alloc>;

	list_node_base head; //头部,非指针

	static list_node* create_node(const value_type& x) {
		list_node* node = list_node_allocator::allocate();//配置空间
		try { 
			construct(&(node->data), x);
			node->next = nullptr;
		}
		catch (...) {
			destroy_node(node);
		}
		return node;
	}

	static void destroy_node(list_node* node) {
		destroy(&(node->data)); //析构元素
		list_node_allocator::deallocate(node); //释放空间
	}
public:
	forward_list() { head.next = nullptr; }
	~forward_list() { clear(); }

	iterator begin() { return iterator(dynamic_cast<list_node*>(head.next)); }
	iterator end() { return iterator(nullptr); }
	size_type size() const { return _forward_list_size(head.next); }
	bool empty()const { return head.next == nullptr; }

	void swap(forward_list& fl) {
		list_node_base* tmp = head.next;
		head.next = fl.head.next;
		l.head.next = tmp;
	}

	//基本元素操作
	reference front() { return dynamic_cast<list_node*>(head.next)->data; }
	//从头部插入元素
	void push_front(const value_type& x) {
		_forward_list_make_link(&head, create_node(x));
	}

	void pop_front() {
		list_node* node = dynamic_cast<list_node*>(head.next);
		head.next = node->next;
		destroy_node(node);
	}
};

_PRC_END
#endif // !MY_FORWARD_LIST_H
