#pragma once
#ifndef MY_LIST_H
#define MY_LIST_H
#include"my_iterator.h"
#include"my_simple_alloc.h"
#include"my_default_alloc.h"
#include"my_uninitialized.h"
#include<initializer_list>
_PRC_BEGIN
template<class T>
struct _list_node {
	_list_node<T>* prev;
	_list_node<T>* next;
	T data;
};
template<class T,class Ref,class Ptr>
struct _list_iterator {
	using iterator = _list_iterator<T, T&, T*>;
	using self     = _list_iterator<T, Ref, Ptr>;

	using iterator_category = bidirectional_iterator_tag;
	using value_type = T;
	using difference_type = ptrdiff_t;
	using pointer = Ptr* ;
	using reference = Ref  ;
	using size_type = size_t;
	using link_type = _list_node<T>*;

	link_type node;//指向 list节点的指针

	_list_iterator(link_type x=nullptr) :node(x) {}
	_list_iterator(const iterator& x) :node(x.node) {}
 
	bool operator==(const self& x)const { return node == x.node; }
	bool operator!=(const self& x)const { return node != x.node; }

	reference operator*() const { return node->data; }
	pointer   operator->() const{return &operator*();}
	self& operator++() {
		node = node->next;
		return *this;
	}
	self operator++(int) {
		self tmp = *this;
		++(*this);
		return tmp;
	}

	self& operator--() {
		node = node->prev;
		return *this;
	}

	self operator--(int) {
		self tmp = *this;
		--(*this);
		return tmp;
	}

};

template<class T, class Alloc= alloc>
class list {
public:
	using value_type = T;
	using pointer = value_type * ;
	using iterator = _list_iterator<T, T&, T*>;
	using reference = value_type & ;
	using size_type = size_t;
	using difference_type = ptrdiff_t;
protected:
	using list_node = _list_node<T>;
	using link_type = list_node * ;
	link_type node;
	//空间配置器
	using list_node_allocator = _simple_alloc<list_node, Alloc>;
	//申请一个节点的空间
	link_type get_node() { return list_node_allocator::allocate(); }
	//释放一个节点的空间
	void put_node(link_type p) { list_node_allocator::deallocate(p); }
	//产生一个节点,并且构造
	link_type create_node(const T& x){
		link_type p = get_node();
		construct(&p->data, x);
		return p;
	}
	//销毁一个节点
	void destroy_node(link_type p) {
		destroy(&p->data);
		put_node(p);
	}
	//空初始化
	void empty_initialize() {
		node = get_node();
		node->next = node;
		node->prev = node;
	}
	//迁移函数,将某一范围内的节点迁移到指定位置之前
	void transfer(iterator position, iterator first, iterator last) 
	{//沿用网络写法,尽管在这里有些东西比较多余
		if (position != last) {
			(*(link_type((*last.node).prev))).next = position.node;  //（1）
			(*(link_type((*first.node).prev))).next = last.node;  //（2）
			(*(link_type((*position.node).prev))).next = first.node;  //（3）
			link_type tmp = link_type((*position.node).prev);  //（4）
			(*position.node).prev = (*last.node).prev;  //（5）
			(*last.node).prev = (*first.node).prev;  //（6）
			(*first.node).prev = tmp;  //（7）
		}
	}
	template<class InputIterator>
	void copy_initialize(InputIterator first, InputIterator last) {
		empty_initialize();
		for (;first != last;++first) {
			push_back(*first);
		}
	}



public:

	list() { empty_initialize(); }
	list(const list<value_type>& li) { copy_initialize(li.begin(), li.end()); }
	list<value_type, Alloc>& operator=(const list<value_type, Alloc>& li) { 
		copy_initialize(li.begin(), li.end());
		return *this;
	}
	list(const std::initializer_list<value_type>& il) { copy_initialize(il.begin(), il.end()); }

	~list() {
		clear();
		destroy_node(node);
	}
	iterator begin()const { return iterator(node->next); }
	iterator end() const{ return iterator(node); }
	bool empty() const { return node->next == node; }
	size_type size() const {
		size_type result = _STL_OR_PRC  distance(begin(),end());
		return result;
	}

	//基础元素操作
	reference front() { return *begin(); }
	reference back() { return  *(--end()); }
	void push_back(const T& x) { insert(end(), x); }
	iterator insert(iterator it, const T& x) {
		link_type tmp = create_node(x);
		tmp->next = it.node;
		tmp->prev = it.node->prev;
		tmp->prev->next = tmp;
		tmp->next->prev = tmp;
		return iterator(tmp);
	}

	void push_front(const T& x) { insert(begin(), x); }
	iterator erase(iterator it) {
		link_type next_node = it.node->next;
		link_type prev_node = it.node->prev;
		prev_node->next = next_node;
		next_node->prev = prev_node;
		destroy_node(it.node);
		return (iterator(next_node));
	}

	iterator erase(iterator first, iterator last) {
		link_type prev_node = first.node->prev;
		for (;first != last;++first) {
			destroy(first.node);
		}
		prev_node->next = first.node;
		first.node->prev = prev_node;
		return first;
	}

	void pop_front() { erase(begin()); }
	void pop_back() {
		iterator tmp = end();
		erase(--tmp);
	}
	//专属操作

	//清除整个链表
	void clear() {
		link_type cur = node->next;
		while (cur != node) {
			link_type tmp = cur;
			cur = cur->next;
			destroy_node(tmp);
		}
		//恢复原始状态
		node->next = node;
		node->prev = node;
	}
	// 将数值为value 的全部清楚
	void remove(const T& value) {
		iterator first = begin();
		iterator last = end();
		while (first != last) {
			iterator next=first;
			++next;
			if (*first == value) erase(first);//找到就删除
			first = next;
		}
	}

	//移除的是连续且相同的节点
	void unique() {
		iterator first = begin();
		iterator last = end();
		if (first == last) return;//空节点不操作
		iterator next = first;
		while (++next != last) {
			if (*first == *next)
				erase(next);//迭代器失效
			else
				first = next;
			next = first;
		}
	}

	void swap(list & li) {
		link_type tmp = node;
		node = li.node;
		li.node = tmp;
	}



	//将it所指节点接合在position 之前,list的作用仅仅是符合标准规范,增加可读意义
	void splice(iterator position,list&,iterator it) {
		iterator it2 = it;
		++it2;
		if (position == it || position == it2) return;
		transfer(position, it, it2);
	}

	//将[first,last)内的所有节点结合于 position所指位置之前
	void splice(iterator position, iterator first, iterator last) {
		if (first != last)
			transfer(position, first, last);
	}
	//合并链表合并的前提是已经经过排序
	void merge(list<T, Alloc>& li)
	{
		iterator first1 = begin();
		iterator last1 = end();
		iterator first2 = li.begin();
		iterator last2 = li.end();

		while (first1 != last1 && first2 != last2) {
			if (*first2 < *first1) {
				iterator next = first2;
				transfer(first1, first2, (++next));
				first2 = next;
			}
			else
				++first1;
		}
		if (first2 != last2) transfer(last1, first2, last2);
	}
	//逆置,或者说反转
	void reverse() {
		//其意义是if(empty() || size()==1 ) 但是这样做没有效率
		if (node->next==node || node->next->next == node)return;
		iterator first = begin();
		++first;
		while (first != end()) {
			iterator old = first;
			++first;
			transfer(begin(), old, first);
		}
	}
	/*list专有排序算法,因为没有随机访问迭代器
    * 采用归并排序(非递归式)
	*/
	void sort() {
		//其意义是if(empty() || size()==1 ) 但是这样做没有效率
		if (node->next == node || node->next->next == node)return;
	    
		list carry;//缓冲链表
		list counter[64];//链表数组,也是缓冲数组的counter[i]代表第i个位置上元素有2的i次方个

		int fill = 0;//表示在counter[64]这个数组里面用了多少
		while (!empty()) {

			carry.splice(carry.begin(),*this,begin());//把首节点放入缓冲链表
			int i = 0;
			/*
			*一开始carry 获得一个节点,但是不足以启动以下循环,然后这个节点
			*被交换到counter[0],  fill++,第二次carry 获得一个节点(此时仍然
			*只有一个节点),启动下面的循环一次,counter[0]合并和carry,此时counter[0]
			*有两个节点,然后counter[0]的内容被交换给carry,循环结束,经过下面两步后
			*fill=2,carry 重新为空,counter[0]为空,counter[1]有两个节点
			*如此上述过程,每次从list取一个元素,让其存在于counter链表数组中
			*最终归并
			*/
			while (i < fill && !counter[i].empty())
			{
				counter[i].merge(carry);//把缓冲carry合并到这个list
				carry.swap(counter[i++]);//把这个list重新换给缓冲carry
			}
			carry.swap(counter[i]);//循环结束把缓冲carry的内容换到缓冲数组上
			if (i == fill)
				++fill;
		
		}
		for (int i = 1;i < fill;++i)
			counter[i].merge(counter[i - 1]);
		swap(counter[fill - 1]);
	}

};















_PRC_END
#endif
