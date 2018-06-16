/*
set��һ�ּ�ֵ���䱾��ֵһģһ��������,���Բ�����ͨ����������ֵ�����޸�
���Ե�����������const��
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
		//���ú�������ڲ�����
		using rep_type = rb_tree<key_type, value_type,
			identity<value_type>, key_compare, Alloc>;
		rep_type t;
	public:
		using pointer = typename rep_type::const_pointer;
		using const_pointer = typename rep_type::const_pointer;
		using reference = typename rep_type::const_reference;
		using const_reference = typename rep_type::const_reference;
		using iterator = typename rep_type::const_iterator;
		/*set������ͨ��ָ��͵��������ڲ�Ԫ�ؽ��в���*/
		using const_iterator = typename rep_type::const_iterator;
		using size_type = typename rep_type::size_type;
		using difference_type = typename rep_type::difference_type;

		/*set�Ĳ���������õ���rb_tree ��insertunique,��Ϊ����������ظ���*/

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

		//set��������




};











_PRC_END
#endif // !MY_SET_H
