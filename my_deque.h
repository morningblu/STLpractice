#pragma once
#ifndef MY_DEQUE_H
#define MY_DEQUE_H
#include"my_iterator.h"
#include"my_simple_alloc.h"
#include"my_default_alloc.h"
#include"my_uninitialized.h"
#include<initializer_list>
_PRC_BEGIN
/*��� n��Ϊ0 �������ʹ���Զ����С
*���Ϊ0�����ʹ��Ĭ������
*���sz<512 �� ����512/n ���ռ�
*���sz>=512 �򴫻�1
*/
inline size_t _deque_buf_size(size_t n,size_t sz)
{
	return n != 0 ? n : (sz < 512 ? size_t(512 / sz) : size_t(1));
}


template<class T, class Ref,class Ptr,size_t BufSiz>
struct _deque_iterator {
	using  iterator= _deque_iterator<T, T&, T*, BufSiz>;
	using  const_iterator = _deque_iterator<T, const T&, const T*, BufSiz>;
	static size_t buffer_size() { return _deque_buf_size(BufSiz, sizeof(T)); }

	using iterator_category = random_access_iterator_tag;
	using value_type = T;
	using difference_type = ptrdiff_t;
	using pointer = Ptr ;
	using reference = Ref ;
	using map_pointer = T**;

	using self = _deque_iterator;

	T* cur;//��ָ��ָ�����ڵ�Ԫ��
	T* first;//��ָ��ָ�򻺳�����ͷ��
	T* last;//��ָ��ָ�򻺳�����β��
	map_pointer node; //ָ��ܿ�����
    
	//��ת���µĻ�����
	void set_node(map_pointer new_node) {
		node = new_node;
		first = *new_node;
		last = first + difference_type(buffer_size());
	}
	                                                                                                 
	//�����������

	reference operator*()const { return *cur; }
	pointer operator->() const { return &(operator*()); }
	
	//deque�������Ϊ�����ռ�
	difference_type operator-(const self& x) const {
		return difference_type(buffer_size())*(node - x.node-1 ) 
			+(cur - first) + (x.last - x.cur);
	}

	//�����ǰָ���������������Ľ�β����ת����һ��������
	self& operator++() {
		++cur;
		if (cur == last) {
			set_node(node + 1);
			cur = first;
		}
		return *this;
	}

	//��������
	self operator++(int) {
		self tmp = *this;
		++(*this);
		return tmp;
	}

	self& operator--() {
		if (cur == first) {
			set_node(node - 1);
			cur = last;//������ʱ��ָ��β��
		}
		--cur;//�����ָ��βԪ��
		return *this;
	}
	//�����Լ�
	self operator--(int) {
		self tmp = *this;
		--(*this);
		return tmp;
	}

	//�����������������

	//������ֱ����Ծ n������
	self& operator+=(difference_type n) {
		difference_type offset = n + (cur - first);//ƫ�Ƶ���λ��
		//���ƫ�ƴ���0����ƫ��С�� ��������СҲ����Ŀ���ַ��ͬһ��������
		if (offset >= 0 && offset < difference_type(buffer_size()))
			cur += n;
		else {
		//Ŀ��λ�ò���ͬһ��������
		//�п�ָ���ƫ����
			difference_type node_offset =
				offset > 0 ? offset / difference_type(buffer_size())//���offset����0�򷵻����뻺������С����
				: (-difference_type((-offset - 1) / buffer_size()) - 1);//���򷵻�������������Ƶķ����
		//�л����µĻ�����
			set_node(node + node_offset);
	     //�л�����ȷ��Ԫ��
			cur = first + (offset - node_offset * difference_type(buffer_size()));
		}
		return *this;
	}

	self operator+(difference_type n)const {
		self tmp = *this;
		return tmp += n;
	}

	self& operator-=(difference_type n) { return *this += -n; }

	self operator-(difference_type n)const {
		self tmp = *this;
		return tmp -= n;
	}

	//ʵ�ֵ��������±����
	reference operator[](difference_type n)const { return *(*this + n); }

	//ʵ�ֵ��������߼��ж�

	bool operator==(const self& x) const { return cur == x.cur; }

	bool operator!=(const self& x)const { return !(*this == x); }

	bool operator<(const self& x)const {
		return (node == x.node) ? (cur < x.cur) : (node < x.node);
	}
};




template<class T,class Alloc =alloc,size_t BufSiz =0>
class deque {
public:
	using value_type = T;
	using pointer = value_type * ;
	using iterator = _deque_iterator<T, T&, T*,BufSiz>;
	using reference = value_type & ;
	using const_reference = const value_type &;
	using size_type = size_t;
	using difference_type = ptrdiff_t;

protected:
	using map_pointer = pointer * ;
	map_pointer map; //ָ��map��ָ��,map�������ռ�����ÿ��Ԫ�ض��Ǹ�ָ��,ָ�򻺳���
	size_type map_size; //map�Ĵ�С
	iterator start;
	iterator finish;

	//dequeר��������Ԫ�ص�������
	using data_allocator = _simple_alloc<value_type, Alloc>;
	//dequeר�õ�����map��С��������
	using map_allocator  = _simple_alloc<pointer, Alloc>;

	pointer allocate_node() { return data_allocator::allocate(iterator::buffer_size()); }

	// �ͷ��ڴ�, ����������
	void deallocate_node(pointer n)
	{
		data_allocator::deallocate(n, iterator::buffer_size());
	}

	//�����ź�deque�Ľṹ
	void create_map_and_nodes(size_type num_elements) {
    // ��Ҫ�ڵ���� = (Ԫ�ظ���/ÿ�������������˵�Ԫ�ظ��� )+1 
		size_type num_nodes = num_elements / iterator::buffer_size() + 1;
	//һ��mapҪ�������ڵ�,���ٰ˸�,���������ڵ�������2
	//ǰ���һ���Ա�����
		map_size = 8>(num_nodes + 2)?8:(num_nodes+2);
		map = map_allocator::allocate(map_size);
    //�������ó�����map_size���ڵ��map
	//������nstart,nfinish ָ��map ��ӵ��֮ȫ���ڵ�������������
	//������������,��ʹͷβ���˵�������һ����,ÿһ���ڵ�ָ��һ��������
		map_pointer nstart = map + (map_size - num_nodes) / 2;
		map_pointer nfinish = nstart + num_nodes - 1;

		map_pointer cur;
		try {
		//Ϊmap�ڵ�ÿһ�����ýڵ����û�����,���л���������������deque�Ŀռ�
		//���ÿռ�(���һ����������������һЩ��ԣ)
			for (cur = nstart; cur <= nfinish;++cur)
				*cur = allocate_node();
		
		}
		catch (...) {
			for (cur = nstart; cur <= nfinish;++cur)
				if (cur)
				{
					destroy(*cur, *cur + iterator::buffer_size());
					data_allocator::deallocate(*cur, iterator::buffer_size());
				}
			throw;
		}
		start.set_node(nstart);
		finish.set_node(nfinish);
		start.cur = start.first;
		finish.cur = finish.first + num_elements % iterator::buffer_size();
	}

	void fill_initialize(size_type n, const value_type & value) {
		create_map_and_nodes(n);
		map_pointer cur;
		try {
			for (cur = start.node;cur < finish.node;++cur)
				uninitialized_fill(*cur, *cur + iterator::buffer_size(), value);
			uninitialized_fill(finish.first, finish.cur, value);
		}
		catch (...) {
			for (cur = start.node; cur < finish.node;++cur)
				if (cur)
				{
					destroy(*cur, *cur + iterator::buffer_size());
					data_allocator::deallocate(*cur, iterator::buffer_size());
				}
			destroy(*cur,finish.cur);
			data_allocator::deallocate(*cur, iterator::buffer_size());
			throw;
		}
	}
     void	reallocate_map(size_type nodes_to_add, bool add_at_front) {
		size_type old_num_nodes = finish.node - start.node + 1;
		size_type new_num_nodes = old_num_nodes + nodes_to_add;
		map_pointer new_nstart;
		if (map_size > 2 * new_num_nodes) {
			//���ԭ map���㹻�Ŀռ������ԭ�ռ��ϵ���
			new_nstart = map + (map_size - new_num_nodes) / 2
				+ (add_at_front ? nodes_to_add : 0);
			if (new_nstart < start.node)
				_STL_OR_PRC copy(start.node, finish.node + 1, new_nstart);
			else
				_STL_OR_PRC copy_backward(start.node, finish.node + 1, new_nstart + old_num_nodes);
		
		}
		else {
			size_type new_map_size = map_size + _STL_OR_PRC max(map_size, nodes_to_add);
		//����һ��ռ�,׼������mapʹ��
			map_pointer new_map = map_allocator::allocate(new_map_size);
			new_nstart = new_map + (new_map_size - new_num_nodes) / 2
				+ (add_at_front ? nodes_to_add : 0);
			_STL_OR_PRC copy(start.node, finish.node + 1, new_nstart);
			map_allocator::deallocate(map, map_size);
			//�趨�µ�map����ʼ��ַ��С
			map = new_map;
			map_size = new_map_size;

		
		}
		start.set_node(new_nstart);
		finish.set_node(new_nstart + old_num_nodes - 1);
	}




	void reserve_map_at_back(size_type nodes_to_add=1) {
		if (nodes_to_add + 1 > map_size - (finish.node -map))
			//���β�˽ڵ㱸�ÿռ䲻��
			//������������������������map
			reallocate_map(nodes_to_add, false);
	}
	void reserve_map_at_front(size_type nodes_to_add = 1) {
		if (nodes_to_add >size_type( start.node - map))
			//���mapǰ�˱��ýڵ㲻��
			reallocate_map(nodes_to_add, true);
	}



	void push_back_aux(const value_type& t) {
		value_type t_copy = t;
		reserve_map_at_back();
		*(finish.node + 1) = allocate_node(); //����һ���½ڵ�
		try {
			construct(finish.cur, t_copy);
			finish.set_node(finish.node + 1);
			finish.cur = finish.first;
		}
		catch (...) {
			deallocate_node(*(finish.node + 1));
		}
	}

	void push_front_aux(const value_type& t) {
		value_type t_copy = t;
		reserve_map_at_front();//������ĳ�����������¸���map
		*(start.node - 1) = allocate_node();
		try {
			start.set_node(start.node - 1);//�ı�start,������nodeָ���½ڵ�
			start.cur = start.last - 1;
			construct(start.cur, t_copy);
		}
		catch (...) {
			//���������ִ���  ��
			start.set_node(start.node + 1);
			start.cur = start.first;
			deallocate_node(*(start.node - 1));
			throw;
		}
	}

	void pop_back_aux() {
		deallocate_node(finish.first);  //�ͷ����������
		finish.set_node(finish.node - 1); //������nodeָ��
		finish.cur = finish.last - 1;  //��ǰָ��β��
		destroy(finish.cur);    //��Ԫ������
	}

	void pop_front_aux() {
		destroy(start.cur);
		deallocate_node(start.first);
		start.set_node(start.node + 1);
		start.cur = start.first;
	}

	iterator insert_aux(iterator position, const value_type& x) {
		difference_type index = position - start;   //�����֮ǰ��Ԫ�ظ���
		value_type x_copy = x;
		if (index < difference_type( size() / 2)) {
		//��������֮ǰ��Ԫ�ظ����Ƚ���
			push_front(front());       //��ǰ����ԭ�����ײ�
			iterator front1 = start; 
			++front1;                //ʹ��front1ָ��ԭ����ͷ��    
			iterator front2 = front1;
			++front2;                 //ʹ��front2ָ��ԭ����ͷ������һ��
			position = start + index;  //��ֹ������ʧЧ
			iterator postion1 = position;
			++postion1;              
			_STL_OR_PRC copy(front2, postion1, front1);//Ԫ���ƶ�
		}
		else {
			push_back(back());
			iterator back1 = finish;
			--back1;
			iterator back2 = back1;
			--back2;
			position = start + index;
			_STL_OR_PRC copy_backward(position, back2, back1);
		}
		*position = x_copy;
		return position;
	}
	template<class InputIterator>
	void copy_initialize(InputIterator first, InputIterator last) {
		difference_type size = _STL_OR_PRC distance(first,last);
		create_map_and_nodes(size);
		uninitialized_copy(first, last, start);
	}

public:

	deque() { create_map_and_nodes(0); }
	template<class InputIterator>
	deque(InputIterator first, InputIterator last) { copy_initialize(first, last); }
	deque(const deque<value_type,Alloc,BufSiz>&_deque) { copy_initialize(_deque.begin(), _deque.end()); }
	deque(const std::initializer_list<value_type>& il) { copy_initialize(il.begin(), il.end()); }

	deque(int n, const value_type & value) :start(), finish(), map(nullptr), map_size(0)
	{
		fill_initialize(n, value);
	}
	~deque() { 
		clear();
		data_allocator::deallocate(start.first, iterator::buffer_size());
		destroy(map, map + map_size);
		map_allocator::deallocate(map, map_size);
	}


	deque<value_type, Alloc, BufSiz>& operator=(const deque<value_type, Alloc, BufSiz>& _deque) {
		copy_initialize(_deque.begin(), _deque.end()); 
		return *this;
	}

	iterator begin()const { return start; }
	iterator end() const{ return finish; }

	reference operator[](size_type n) {
		return start[difference_type(n)];
	}

	reference front() { return *start; }
	reference back() {
		iterator tmp = finish;
		--tmp;
		return *tmp;
	}

	size_type size()const { return finish - start; }
	//�������ûɶ����
	size_type max_size() const { return size_type(-1); }
	bool empty() const { return finish == start; }

	//Ԫ�ز���

	void push_back(const value_type& x) {
		//����д���һ���ռ�
		if (finish.cur != finish.last - 1) {
			construct(finish.cur, x);
			++finish.cur;
		}
		else
			//���ֻ��һ���ռ�
			push_back_aux(x);
	}

	void push_front(const value_type& x) {
		if (start.cur != start.first) {//���������б��ÿռ�
			construct(start.cur - 1, x);//ֱ���ڱ��ÿռ��Ϲ���Ԫ��
			--start.cur;
		}
		else//����start.cur==start.first,Ҳ����û���κα��ÿռ��ʱ��
			push_front_aux(x);
	}

	void pop_back() {
		if (finish.cur != finish.first) {
		//��󻺳�����һ��������Ԫ��
			--finish.cur;        //finish.cur ��β��,�Լ���ָ��βԪ��
			destroy(finish.cur); //�����һ��Ԫ������
		}
		else
			//������û���κ�Ԫ��
			pop_back_aux(); //�����л��������ͷŹ���
	}

	void pop_front() {
		if (start.cur != start.last - 1) {
			//���start.cur ����β��
			destroy(start.cur); //��Ԫ������
			++start.cur;
		}
		else
			//start.cur��β��Ҳ���ǵ�һ����������һ��Ԫ��
			pop_front_aux();      //���л��������ͷ�
	}

	void clear() {
	//���½����ͷβ�����ÿһ��������
		for (map_pointer node = start.node + 1;node < finish.node;++node) {
		//���������ڵ�����Ԫ������
			destroy(*node, *node + iterator::buffer_size());
		//�黹������
			data_allocator::deallocate(*node, iterator::buffer_size());
		}
		if (start.node != finish.node) {
		//������ͷβ����������
			destroy(start.cur, start.last); //����ͷ��
			destroy(finish.first, finish.cur);//����β��
			//���ͷ�β������
			data_allocator::deallocate(finish.first, iterator::buffer_size());
		}
		else
		{//ֻ��һ��������
			destroy(start.cur, finish.cur); //���˻�����������Ԫ������
			//�����ͷŻ�����
			finish = start;
		}
	}

	iterator erase(iterator pos) {
		iterator next = pos;
		++next;
		difference_type index = pos - start; //ɾ����֮ǰԪ�صĸ���
		if (index < (size() >> 1)) {//��������֮ǰ��Ԫ�ظ�����С���ƶ�ǰ��
			_STL_OR_PRC copy_backward(start, pos, next);
			pop_front();
		}
		else {
			_STL_OR_PRC copy(next, finish, pos);
			pop_back();
		}
		return start + index;
	}

	iterator erase(iterator first, iterator last) {
		if (first == start && last == finish) {
		//�����������������deque,��ת����clear()
			clear();
			return finish;
		}
		else {
			difference_type n = last - first;    //������䳤��
			difference_type elems_before = first - start; //�������ǰ��Ԫ�صĸ���
			if (elems_before < difference_type((size() - n) / 2)) {//���ǰ��Ԫ�ؽ���,
				_STL_OR_PRC copy_backward(start, first, last);
				iterator new_start = start + n;    //�����
				destroy(start, new_start);
				//���½�����Ļ������ͷ�
				for (map_pointer cur = start.node;cur < new_start.node;++cur)
					data_allocator::deallocate(*cur, iterator::buffer_size());
				start = new_start;
			}
			else {
			//����������󷽵�Ԫ�رȽ���
				_STL_OR_PRC copy(last, finish, first);  //����Ԫ��ǰ��
				iterator new_finish = finish - n;  //���deque�µ�β��
				destroy(new_finish, finish);
			    //���½�����Ļ������ͷ�
				for (map_pointer cur = new_finish.node + 1;cur <= finish.node;++cur)
					data_allocator::deallocate(*cur, iterator::buffer_size());
				finish = new_finish;
			}
			return start + elems_before;
		}
	}
	
	iterator insert(iterator position, const value_type& x) {
		if (position.cur == start.cur) {
		//����������deque��ǰ��
			push_front(x);
			return start;
		}
		else if (position.cur == finish.cur) {
		//����������β��
			push_back(x);
			iterator tmp = finish;
			--tmp;
			return tmp;
		}
		else
			return insert_aux(position, x);
	}

};

_PRC_END
#endif // !MY_DEQUE_H
