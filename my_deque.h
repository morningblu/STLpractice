#pragma once
#ifndef MY_DEQUE_H
#define MY_DEQUE_H
#include"my_iterator.h"
#include"my_simple_alloc.h"
#include"my_default_alloc.h"
#include"my_uninitialized.h"
#include<initializer_list>
_PRC_BEGIN
/*如果 n不为0 则其代表使用自定义大小
*如果为0则代表使用默认设置
*如果sz<512 则 传回512/n 个空间
*如果sz>=512 则传回1
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

	T* cur;//此指针指向现在的元素
	T* first;//此指针指向缓冲区的头部
	T* last;//此指针指向缓冲区的尾部
	map_pointer node; //指向管控中心
    
	//跳转到新的缓冲区
	void set_node(map_pointer new_node) {
		node = new_node;
		first = *new_node;
		last = first + difference_type(buffer_size());
	}
	                                                                                                 
	//运算符的重载

	reference operator*()const { return *cur; }
	pointer operator->() const { return &(operator*()); }
	
	//deque对外表现为连续空间
	difference_type operator-(const self& x) const {
		return difference_type(buffer_size())*(node - x.node-1 ) 
			+(cur - first) + (x.last - x.cur);
	}

	//如果当前指向的是这个缓冲区的结尾则跳转到下一个缓冲区
	self& operator++() {
		++cur;
		if (cur == last) {
			set_node(node + 1);
			cur = first;
		}
		return *this;
	}

	//后置自增
	self operator++(int) {
		self tmp = *this;
		++(*this);
		return tmp;
	}

	self& operator--() {
		if (cur == first) {
			set_node(node - 1);
			cur = last;//这里暂时是指向尾后
		}
		--cur;//这里才指向尾元素
		return *this;
	}
	//后置自减
	self operator--(int) {
		self tmp = *this;
		--(*this);
		return tmp;
	}

	//以下是随机访问运算

	//迭代器直接跳跃 n个距离
	self& operator+=(difference_type n) {
		difference_type offset = n + (cur - first);//偏移到的位置
		//如果偏移大于0并且偏移小于 缓冲区大小也就是目标地址在同一个缓冲区
		if (offset >= 0 && offset < difference_type(buffer_size()))
			cur += n;
		else {
		//目标位置不在同一个缓冲区
		//中控指针的偏移量
			difference_type node_offset =
				offset > 0 ? offset / difference_type(buffer_size())//如果offset大于0则返回其与缓冲区大小的商
				: (-difference_type((-offset - 1) / buffer_size()) - 1);//否则返回与上面概念类似的反结果
		//切换至新的缓冲区
			set_node(node + node_offset);
	     //切换至正确的元素
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

	//实现迭代器的下标访问
	reference operator[](difference_type n)const { return *(*this + n); }

	//实现迭代器的逻辑判断

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
	map_pointer map; //指向map的指针,map是连续空间其中每个元素都是个指针,指向缓冲区
	size_type map_size; //map的大小
	iterator start;
	iterator finish;

	//deque专属的配置元素的配置器
	using data_allocator = _simple_alloc<value_type, Alloc>;
	//deque专用的配置map大小的配置器
	using map_allocator  = _simple_alloc<pointer, Alloc>;

	pointer allocate_node() { return data_allocator::allocate(iterator::buffer_size()); }

	// 释放内存, 不进行析构
	void deallocate_node(pointer n)
	{
		data_allocator::deallocate(n, iterator::buffer_size());
	}

	//负责安排好deque的结构
	void create_map_and_nodes(size_type num_elements) {
    // 需要节点的数 = (元素个数/每个缓冲区可容乃的元素个数 )+1 
		size_type num_nodes = num_elements / iterator::buffer_size() + 1;
	//一个map要管理几个节点,最少八个,最多是所需节点数量加2
	//前后各一个以备扩充
		map_size = 8>(num_nodes + 2)?8:(num_nodes+2);
		map = map_allocator::allocate(map_size);
    //以上配置出具有map_size个节点的map
	//以下令nstart,nfinish 指向map 所拥有之全部节点的最中央的区段
	//保持在最中央,可使头尾两端的扩充量一样大,每一个节点指向一个缓冲区
		map_pointer nstart = map + (map_size - num_nodes) / 2;
		map_pointer nfinish = nstart + num_nodes - 1;

		map_pointer cur;
		try {
		//为map内的每一个现用节点配置缓冲区,所有缓冲区加起来就是deque的空间
		//可用空间(最后一个缓冲区可能留有一些余裕)
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
			//如果原 map有足够的空间则仅在原空间上调整
			new_nstart = map + (map_size - new_num_nodes) / 2
				+ (add_at_front ? nodes_to_add : 0);
			if (new_nstart < start.node)
				_STL_OR_PRC copy(start.node, finish.node + 1, new_nstart);
			else
				_STL_OR_PRC copy_backward(start.node, finish.node + 1, new_nstart + old_num_nodes);
		
		}
		else {
			size_type new_map_size = map_size + _STL_OR_PRC max(map_size, nodes_to_add);
		//配置一块空间,准备给新map使用
			map_pointer new_map = map_allocator::allocate(new_map_size);
			new_nstart = new_map + (new_map_size - new_num_nodes) / 2
				+ (add_at_front ? nodes_to_add : 0);
			_STL_OR_PRC copy(start.node, finish.node + 1, new_nstart);
			map_allocator::deallocate(map, map_size);
			//设定新的map的起始地址大小
			map = new_map;
			map_size = new_map_size;

		
		}
		start.set_node(new_nstart);
		finish.set_node(new_nstart + old_num_nodes - 1);
	}




	void reserve_map_at_back(size_type nodes_to_add=1) {
		if (nodes_to_add + 1 > map_size - (finish.node -map))
			//如果尾端节点备用空间不足
			//符合以上条件必须重新配置map
			reallocate_map(nodes_to_add, false);
	}
	void reserve_map_at_front(size_type nodes_to_add = 1) {
		if (nodes_to_add >size_type( start.node - map))
			//如果map前端备用节点不足
			reallocate_map(nodes_to_add, true);
	}



	void push_back_aux(const value_type& t) {
		value_type t_copy = t;
		reserve_map_at_back();
		*(finish.node + 1) = allocate_node(); //配置一个新节点
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
		reserve_map_at_front();//若符合某种条件则重新更换map
		*(start.node - 1) = allocate_node();
		try {
			start.set_node(start.node - 1);//改变start,令他的node指向新节点
			start.cur = start.last - 1;
			construct(start.cur, t_copy);
		}
		catch (...) {
			//如果构造出现错误  则
			start.set_node(start.node + 1);
			start.cur = start.first;
			deallocate_node(*(start.node - 1));
			throw;
		}
	}

	void pop_back_aux() {
		deallocate_node(finish.first);  //释放这个缓冲区
		finish.set_node(finish.node - 1); //调整其node指向
		finish.cur = finish.last - 1;  //当前指向尾部
		destroy(finish.cur);    //将元素析构
	}

	void pop_front_aux() {
		destroy(start.cur);
		deallocate_node(start.first);
		start.set_node(start.node + 1);
		start.cur = start.first;
	}

	iterator insert_aux(iterator position, const value_type& x) {
		difference_type index = position - start;   //插入点之前的元素个数
		value_type x_copy = x;
		if (index < difference_type( size() / 2)) {
		//如果插入点之前的元素个数比较少
			push_front(front());       //首前构造原来的首部
			iterator front1 = start; 
			++front1;                //使得front1指向原来的头部    
			iterator front2 = front1;
			++front2;                 //使得front2指向原来的头部的下一个
			position = start + index;  //防止迭代器失效
			iterator postion1 = position;
			++postion1;              
			_STL_OR_PRC copy(front2, postion1, front1);//元素移动
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
	//这个函数没啥意义
	size_type max_size() const { return size_type(-1); }
	bool empty() const { return finish == start; }

	//元素操作

	void push_back(const value_type& x) {
		//如果有大于一个空间
		if (finish.cur != finish.last - 1) {
			construct(finish.cur, x);
			++finish.cur;
		}
		else
			//如果只有一个空间
			push_back_aux(x);
	}

	void push_front(const value_type& x) {
		if (start.cur != start.first) {//缓冲区尚有备用空间
			construct(start.cur - 1, x);//直接在备用空间上构造元素
			--start.cur;
		}
		else//仅当start.cur==start.first,也就是没有任何备用空间的时候
			push_front_aux(x);
	}

	void pop_back() {
		if (finish.cur != finish.first) {
		//最后缓冲区有一个或更多的元素
			--finish.cur;        //finish.cur 是尾后,自减后指向尾元素
			destroy(finish.cur); //将最后一个元素析构
		}
		else
			//缓冲区没有任何元素
			pop_back_aux(); //将进行缓冲区的释放工作
	}

	void pop_front() {
		if (start.cur != start.last - 1) {
			//如果start.cur 不在尾部
			destroy(start.cur); //将元素析构
			++start.cur;
		}
		else
			//start.cur在尾部也就是第一缓冲区仅有一个元素
			pop_front_aux();      //进行缓冲区的释放
	}

	void clear() {
	//以下仅针对头尾以外的每一个缓冲区
		for (map_pointer node = start.node + 1;node < finish.node;++node) {
		//将缓冲区内的所有元素析构
			destroy(*node, *node + iterator::buffer_size());
		//归还缓冲区
			data_allocator::deallocate(*node, iterator::buffer_size());
		}
		if (start.node != finish.node) {
		//至少有头尾两个缓冲区
			destroy(start.cur, start.last); //析构头部
			destroy(finish.first, finish.cur);//析构尾部
			//仅释放尾缓冲区
			data_allocator::deallocate(finish.first, iterator::buffer_size());
		}
		else
		{//只有一个缓冲区
			destroy(start.cur, finish.cur); //将此缓冲区内所有元素析构
			//并不释放缓冲区
			finish = start;
		}
	}

	iterator erase(iterator pos) {
		iterator next = pos;
		++next;
		difference_type index = pos - start; //删除点之前元素的个数
		if (index < (size() >> 1)) {//如果清楚点之前的元素个数较小则移动前部
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
		//如果清楚区间就是整个deque,则转调用clear()
			clear();
			return finish;
		}
		else {
			difference_type n = last - first;    //清除区间长度
			difference_type elems_before = first - start; //清除区间前方元素的个数
			if (elems_before < difference_type((size() - n) / 2)) {//如果前部元素较少,
				_STL_OR_PRC copy_backward(start, first, last);
				iterator new_start = start + n;    //新起点
				destroy(start, new_start);
				//以下将冗余的缓冲区释放
				for (map_pointer cur = start.node;cur < new_start.node;++cur)
					data_allocator::deallocate(*cur, iterator::buffer_size());
				start = new_start;
			}
			else {
			//如果清楚区间后方的元素比较少
				_STL_OR_PRC copy(last, finish, first);  //将后方元素前移
				iterator new_finish = finish - n;  //标记deque新的尾部
				destroy(new_finish, finish);
			    //以下将冗余的缓冲区释放
				for (map_pointer cur = new_finish.node + 1;cur <= finish.node;++cur)
					data_allocator::deallocate(*cur, iterator::buffer_size());
				finish = new_finish;
			}
			return start + elems_before;
		}
	}
	
	iterator insert(iterator position, const value_type& x) {
		if (position.cur == start.cur) {
		//如果插入点是deque最前端
			push_front(x);
			return start;
		}
		else if (position.cur == finish.cur) {
		//如果插入点是尾部
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
