/*
set是一种键值和其本身值一模一样的容器,所以不允许通过迭代器对值进行修改
所以迭代器本身是const的
*/
#pragma once
#ifndef MY_SET_H
#define MY_SET_H
#include"my_rb_tree.h"
#include"my_functional.h"// for identity
_PRC_BEGIN
template<class Key
         class Compare=std::less<Key>,
         class Alloc=alloc>
	class set {
	public:
		using key_type = Key;
		using value_type = Key;
		using key_compare = Compare;
		using value_compare = Compare;
	private:
		//采用红黑树做内部容器
		using rep_type = rb_tree<key_type, value_type,
			identity<value_type>, key_compare, Alloc>;
		rep_type t;
	public:
		using pointer = typename rep_type::const_pointer;
		using const_pointer = typename rep_type::const_pointer;
		using reference = typename rep_type::const_reference;
		using const_reference = typename rep_type::const_reference;
		using iterator = typename rep_type::const_iterator;
		/*set不允许通过指针和迭代器对内部元素进行操作*/
		using const_iterator = typename rep_type::const_iterator;
		using size_type = typename rep_type::size_type;
		using difference_type = typename rep_type::difference_type;

		/*set的插入操作采用的是rb_tree 的insertunique,因为不允许存在重复键*/

		set() :t(Compare()) {}
		explicit set(const Compare& comp) :t(comp) {}
		template<class InputIterator>
		set(InputIterator first, InputIterator last)
		:t(Compare()){
			t.insert_unique(first, last);
		}
		template<class InputIterator>
		set(InputIterator first, InputIterator last,const Compare& comp)
			:t(comp) {
			t.insert_unique(first, last);
		}
		set(const set<Key, Compare, Alloc>& x) :t(x.t) {}
		set<Key, Compare, Alloc>& operator=(const set<Key, Compare, Alloc>& x) {
			t = x.t;
			return *this;
		}

		//set基础操作




};











_PRC_END
#endif // !MY_SET_H
