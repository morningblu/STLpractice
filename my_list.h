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

	link_type node;//ָ�� list�ڵ��ָ��

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
	//�ռ�������
	using list_node_allocator = _simple_alloc<list_node, Alloc>;
	//����һ���ڵ�Ŀռ�
	link_type get_node() { return list_node_allocator::allocate(); }
	//�ͷ�һ���ڵ�Ŀռ�
	void put_node(link_type p) { list_node_allocator::deallocate(p); }
	//����һ���ڵ�,���ҹ���
	link_type create_node(const T& x){
		link_type p = get_node();
		construct(&p->data, x);
		return p;
	}
	//����һ���ڵ�
	void destroy_node(link_type p) {
		destroy(&p->data);
		put_node(p);
	}
	//�ճ�ʼ��
	void empty_initialize() {
		node = get_node();
		node->next = node;
		node->prev = node;
	}
	//Ǩ�ƺ���,��ĳһ��Χ�ڵĽڵ�Ǩ�Ƶ�ָ��λ��֮ǰ
	void transfer(iterator position, iterator first, iterator last) 
	{//��������д��,������������Щ�����Ƚ϶���
		if (position != last) {
			(*(link_type((*last.node).prev))).next = position.node;  //��1��
			(*(link_type((*first.node).prev))).next = last.node;  //��2��
			(*(link_type((*position.node).prev))).next = first.node;  //��3��
			link_type tmp = link_type((*position.node).prev);  //��4��
			(*position.node).prev = (*last.node).prev;  //��5��
			(*last.node).prev = (*first.node).prev;  //��6��
			(*first.node).prev = tmp;  //��7��
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

	//����Ԫ�ز���
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
	//ר������

	//�����������
	void clear() {
		link_type cur = node->next;
		while (cur != node) {
			link_type tmp = cur;
			cur = cur->next;
			destroy_node(tmp);
		}
		//�ָ�ԭʼ״̬
		node->next = node;
		node->prev = node;
	}
	// ����ֵΪvalue ��ȫ�����
	void remove(const T& value) {
		iterator first = begin();
		iterator last = end();
		while (first != last) {
			iterator next=first;
			++next;
			if (*first == value) erase(first);//�ҵ���ɾ��
			first = next;
		}
	}

	//�Ƴ�������������ͬ�Ľڵ�
	void unique() {
		iterator first = begin();
		iterator last = end();
		if (first == last) return;//�սڵ㲻����
		iterator next = first;
		while (++next != last) {
			if (*first == *next)
				erase(next);//������ʧЧ
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



	//��it��ָ�ڵ�Ӻ���position ֮ǰ,list�����ý����Ƿ��ϱ�׼�淶,���ӿɶ�����
	void splice(iterator position,list&,iterator it) {
		iterator it2 = it;
		++it2;
		if (position == it || position == it2) return;
		transfer(position, it, it2);
	}

	//��[first,last)�ڵ����нڵ����� position��ָλ��֮ǰ
	void splice(iterator position, iterator first, iterator last) {
		if (first != last)
			transfer(position, first, last);
	}
	//�ϲ�����ϲ���ǰ�����Ѿ���������
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
	//����,����˵��ת
	void reverse() {
		//��������if(empty() || size()==1 ) ����������û��Ч��
		if (node->next==node || node->next->next == node)return;
		iterator first = begin();
		++first;
		while (first != end()) {
			iterator old = first;
			++first;
			transfer(begin(), old, first);
		}
	}
	/*listר�������㷨,��Ϊû��������ʵ�����
    * ���ù鲢����(�ǵݹ�ʽ)
	*/
	void sort() {
		//��������if(empty() || size()==1 ) ����������û��Ч��
		if (node->next == node || node->next->next == node)return;
	    
		list carry;//��������
		list counter[64];//��������,Ҳ�ǻ��������counter[i]�����i��λ����Ԫ����2��i�η���

		int fill = 0;//��ʾ��counter[64]��������������˶���
		while (!empty()) {

			carry.splice(carry.begin(),*this,begin());//���׽ڵ���뻺������
			int i = 0;
			/*
			*һ��ʼcarry ���һ���ڵ�,���ǲ�������������ѭ��,Ȼ������ڵ�
			*��������counter[0],  fill++,�ڶ���carry ���һ���ڵ�(��ʱ��Ȼ
			*ֻ��һ���ڵ�),���������ѭ��һ��,counter[0]�ϲ���carry,��ʱcounter[0]
			*�������ڵ�,Ȼ��counter[0]�����ݱ�������carry,ѭ������,��������������
			*fill=2,carry ����Ϊ��,counter[0]Ϊ��,counter[1]�������ڵ�
			*�����������,ÿ�δ�listȡһ��Ԫ��,���������counter����������
			*���չ鲢
			*/
			while (i < fill && !counter[i].empty())
			{
				counter[i].merge(carry);//�ѻ���carry�ϲ������list
				carry.swap(counter[i++]);//�����list���»�������carry
			}
			carry.swap(counter[i]);//ѭ�������ѻ���carry�����ݻ�������������
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
