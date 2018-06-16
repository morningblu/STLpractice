/*�������ʵ�־������⼼��,������������ͷ�ڵ�header,���ڵ��header��Ϊ���ڵ�,��header��������˺������end()
Ҳ���Ǵ���root_>parent==header;  header->parent==root; �˲����ܽϿ�Ļ�ȡ��root
header�����ӽڵ���Զ����Ϊ���ú��������С��ֵ�ڵ�Ҳ���Ǻ��������ڵ�,�����ͺܿ��ȡ��begin()
header�����ӽڵ���Զ����Ϊ���ú����������ֵ�ڵ�,Ҳ���Ǻ�����������ӽڵ�.
*/
#pragma once
#ifndef MY_RB_TREE_H
#define MY_RB_TREE_H
#include"my_iterator.h"
#include"my_simple_alloc.h"
#include"my_default_alloc.h"
#include"my_uninitialized.h"
#include<utility>
_PRC_BEGIN

using _rb_tree_color_type = bool;
const _rb_tree_color_type _rb_tree_red = false;
const _rb_tree_color_type _rb_tree_black = true;
//������ڵ����
struct _rb_tree_node_base {
	using color_type = _rb_tree_color_type;
	using base_ptr = _rb_tree_node_base * ;

	color_type color;//�ڵ���ɫ 
	base_ptr parent;//���ڵ�
	base_ptr left;//���ӽڵ�
	base_ptr right;//���ӽڵ�
	//��������ڵ�Ϊ����ƽ��������С�ڵ�
	static base_ptr minimum(base_ptr x) {
		while (x->left != nullptr) x = x->left;
		return x;
	}
	//��������ڵ�Ϊ����ƽ���������ڵ�
	static base_ptr maximum(base_ptr x){
		while (x->right != nullptr) x = x->right;
		return x;
	}
};
//������ڵ�
template<class Value>
struct _rb_tree_node :public _rb_tree_node_base {
	using link_type = _rb_tree_node<Value>*;
	Value value_field;//�ڵ�ֵ
};

//���������������
struct _rb_tree_base_iterator {
	using base_ptr = _rb_tree_node_base::base_ptr;
	using iterator_category = bidirectional_iterator_tag;
	using difference_type = ptrdiff_t;

	base_ptr node;
	//�˺�����Ϊǰ��,��νǰ��ָ����ƽ�����������������һ��λ��
	void increment() {
		if (node->right != nullptr) {        //��������ӽڵ�
			node = node->right;              //��������
			while (node->left != nullptr)    //Ȼ��һֱ���������ߵ���
				node = node->left;           
			/*
			���ϲ�����ԭ������������������
			����ڵ������������С�ڵ�Ϊ��һ���ڵ�
			*/
		}
		else {                               //���û�����ӽڵ�
			base_ptr y = node->parent;       //�ҳ����ڵ�
			while (node == y->right) {       //������нڵ㱾���Ǹ����ӽڵ�
				node = y;                    //��һֱ����,ֱ����Ϊ���ӽڵ�Ϊֹ
				y = y->parent;             
			}
			if (node->right != y)            //����ʱ�����ӽڵ㲻���ڴ�ʱ�ĸ��ڵ�,���������ڵ�͸��ڵ��й�
				node = y;                   
				/*���ϲ�����ԭ��������������������
				����Լ�û���ҽڵ�,�����Լ������Ǹ��ڵ���ҽڵ�
				�򸸽ڵ��ȻС�ڱ���,�����Լ�����һ���ڵ�,����Ҫ���ϻ���
				ֱ���ڵ㲻Ϊ�ҽڵ�,Ҳ������ڵ�,��ʱ�丸�ڵ��Ȼ�պñ��Լ���
				Ϊ��һ���ڵ�
				�����,��node����Ϊ�����ӽڵ�,����˵��β�ڵ�,�뿪whileѭ��ʱnodeΪ���ڵ�,yΪheader,������
				�����if����������node ����ֵΪheader,���� β�ڵ��ǰ��ͬ���ǻص�header.
				����ص�nodeһ��ʼΪheader��ʱ��,yΪroot,��ʱnode���������whileѭ��,�Ҳ�����������µ�if������
				����node һֱΪnode Ҳ����˵ header�ڵ�� ǰ����Զ���Լ�
				*/
		}                                   
	}
	//�˺���Ϊ����,��ν����ָ����ƽ�����������������µ���һ��λ��
	void decrement() {
		//����Ǻ�ڵ�,�Ҹ��ڵ�ĸ��ڵ�����Լ�, ��ΪheaderĬ���Ǻ�ɫ,
		if (node->color == _rb_tree_red && node->parent->parent == node)
			node = node->right;
		/*
		�������������nodeΪheader�������Ҳ����nodeΪend()
		*/
		else if (node->left != nullptr) {     //��������ӽڵ�
			base_ptr y = node->left;          //��yָ�����ӽڵ�
			while (y->right != nullptr)       //��y�����ӽڵ�ʱ
				y = y->right;                 //һֱ�����ӽڵ��ߵ���
			node = y;
			/*
			���ϲ�����ԭ��������������������
			�������ڵ�������ӽڵ�,�������������ڵ��Ȼ��ǰһ���ڵ�
			*/
		}
		else {                                //�ȷǸ��ڵ�,Ҳû�����ӽڵ�
			base_ptr y = node->parent;        //�ҳ����ڵ�
			while (node == y->left) {         //���䱾��Ϊ���ӽڵ�
				node = y;                     //�򲻶�����
				y = y->parent;                //ֱ����Ϊ���ӽڵ�
			}
			node = y;                         //��ʱ���ڵ�Ϊ����
			/*
			���ϲ�����ԭ��������������������
			��һ���ڵ�Ϊ���ӽڵ�,�������ӽڵ�,���丸�ڵ��Ȼ����������ڵ�
			��������ֱ������ڵ�Ϊ���ӽڵ��ʱ���丸�ڵ��Ȼ�պ�С������ڵ�
			�����,��node����Ϊ����ڵ��ʱ��,Ҳ����begin(),�䰴������,node�Ľ����Ȼ��header
			*/
		}
	}
};

//�����������
template<class Value,class Ref,class Ptr>
struct _rb_tree_iterator :public _rb_tree_base_iterator {
	using value_type = Value;
	using reference = Ref;
	using pointer = Ptr;
	using iterator = _rb_tree_iterator<Value, Value&, Value*>;
	using const_iterator = _rb_tree_iterator<Value, const Value&, const Value*>;
	using self = _rb_tree_iterator<Value, Ref, Ptr>;
	using link_type = _rb_tree_node<Value>*;

	_rb_tree_iterator() {}
	_rb_tree_iterator(link_type x){ node = x; }
	_rb_tree_iterator(const iterator& it)  {node = it.node;}

	reference operator*() const { return ((link_type&)node)->value_field; }
	pointer operator->() { return &(operator*()); }

	self& operator++() { increment();return *this; }
	self operator++(int) { 
		self tmp = *this;
		increment();
		return tmp;
	}
	self& operator--() { decrement(); return *this; }
	self operator--(int) {
		self tmp = *this;
		decrement();
		return tmp;
	}
	bool operator==(const self& it) const{ return node == it.node; }
	bool operator!=(const self& it) const{ return node != it.node; }

};
//ɾ���ڵ�
inline _rb_tree_node_base*
_rb_tree_rebalance_for_erase(_rb_tree_node_base* z,
                           	 _rb_tree_node_base*& root,
	                         _rb_tree_node_base*& leftmost,
	                         _rb_tree_node_base*& rightmost) {
	_rb_tree_node_base* y = z;
	_rb_tree_node_base* x = nullptr;
	_rb_tree_node_base* x_parent = nullptr;
	if (y->left == nullptr)
		x = y->right;   



}






template<class Key,class Value,class KeyOfValue,class Compare,class Alloc=alloc>
class rb_tree {
protected:
	using void_pointer = void*;
	using base_ptr = _rb_tree_node_base * ;
	using rb_tree_node = _rb_tree_node<Value>;
	using rb_tree_node_allocator = _simple_alloc<rb_tree_node, Alloc>;
	using color_type = _rb_tree_color_type;
public:
	using Key_type = Key;
	using value_type = Value;
	using pointer = value_type * ;
	using const_pointer = const value_type*;
	using reference = value_type & ;
	using const_reference = const value_type&;
	using link_type = rb_tree_node * ;
	using size_type = size_t;
	using difference_type = ptrdiff_t;
protected:
	link_type get_node() { return rb_tree_node_allocator::allocate(); }
	void put_node(link_type p) { rb_tree_node_allocator::deallocate(p); }

	link_type create_node(const value_type& x) {
		link_type tmp = get_node();
		try {
			construct(&tmp->value_field, x);
		}
		catch (...) {
			put_node(tmp);
			throw;
		}
		return tmp;
	}
	//����һ���ڵ��ֵ����ɫ
	link_type clone_node(link_type x) {
		link_type tmp = create_node(x->value_field);
		tmp->color = x->color;
		tmp->parent = nullptr;
		tmp->left = nullptr;
		tmp->right = nullptr;
		return tmp;
	}

	void destroy_node(link_type p) {
		destroy(&p->value_field);
		put_node(p);
	}

protected:
	size_type node_count; //׷�ټ�¼���Ĵ�С
	link_type header;//�������ͷ�ڵ�
	Compare key_compare; //�ڵ��ļ�ֵ��С�Ƚ�׼��

	//��ȡ���ڵ�
	link_type& root()const { return reinterpret_cast<link_type&>(header->parent); }
	//��ȡ�����ӽڵ�
	link_type& leftmost()const { return  reinterpret_cast<link_type&>(header->left);}
	//��ȡ�����ӽڵ�
	link_type& rightmost() const{ return reinterpret_cast<link_type&>(header->right); }

	static link_type& left(link_type x) {
		return reinterpret_cast<link_type&>(x->left);
	}
	static link_type& right(link_type x) {
		return reinterpret_cast<link_type&>(x->right);
	}
	static link_type& parent(link_type x) {
		return reinterpret_cast<link_type&>(x->parent);
	}
	static reference value(link_type x) {
		return x->value_field;
	}
	static const Key key(link_type x) {
		return KeyOfValue()(value(x));
	}
	static color_type& color(link_type x) {
		return reinterpret_cast<color_type&>(x->color);
	}

	static link_type& left(base_ptr x) {
		return reinterpret_cast<link_type&>(x->left);
	}
	static link_type& right(base_ptr x) {
		return reinterpret_cast<link_type&>(x->right);
	}
	static link_type& parent(base_ptr x) {
		return reinterpret_cast<link_type&>(x->parent);
	}
	static reference value(base_ptr x) {
		return reinterpret_cast<link_type&>(x)->value_field;
	}
	static const Key key(base_ptr x) {
		return KeyOfValue()(value(x));
	}
	static color_type& color(base_ptr x) {
		return reinterpret_cast<color_type&>(reinterpret_cast<link_type&>(x)->color);
	}

	//ȡ����ֵ�ͼ�Сֵ
	static link_type minimum(link_type& x) {
		return static_cast<link_type>(_rb_tree_node_base::minimum(x));
	}
	static link_type maximum(link_type x) {
		return static_cast<link_type>(_rb_tree_node_base::maximum(x));
	}

public:
	using iterator = _rb_tree_iterator<value_type, reference, pointer>;
private:
	iterator _insert(base_ptr _x, base_ptr _y, const value_type& v);
	link_type _copy(link_type x, link_type p);
	void _erase(link_type x);
	void init() {
		header = get_node();
		color(header) = _rb_tree_red; //header������Ϊ��ɫ��������header��root
		root() = nullptr;
		leftmost() = header; //��header ���ӽڵ�Ϊ�Լ�
		rightmost() = header;//��header ���ӽڵ�Ϊ�Լ�
	}
public:
	rb_tree(const Compare& comp = Compare())
		:node_count(0), key_compare(comp) 
	{
		init();
	}
	rb_tree(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x)
		:node_count(0), key_compare(x.key_compare)
	{
		header = get_node();
		color(header) = _rb_tree_red;
		if (x.root() == nullptr) {
		//���x�ǿ���
			root() = nullptr;
			leftmost() = header;
			rightmost() = header;
		}
		else {
		//x���ǿ���
			try {
				root() = _copy(x.root(), header);       //�����պ���
			}
			catch (...) {
				put_node(header);
				throw;
			}

			leftmost() = minimum(root());//��header������Ϊ��С�ڵ�
			rightmost() = maximum(root());//��header���Һ���Ϊ���ڵ�
		}
		node_count = x.node_count;
	}

	~rb_tree() {
		clear();
		put_node(header);
	}
	rb_tree<Key, Value, KeyOfValue, Compare, Alloc>&
		operator=(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x) {
		if (this != &x) {
		//��ֹ����ֵ
			clear();
			node_count = 0;
			key_compare = x.key_compare;
			if (x.root() == nullptr) {
				root() = nullptr;
				leftmost() = header;
				rightmost() = header;
			}
			else {
				root() = _copy(x.root(), header);
				leftmost() = minimum(root());
				rightmost() = maximum(root());
				node_count = x.node_count;
			}
		}
		return *this;
	}

	Compare key_comp() const { return key_compare; }
	iterator begin() const{ return iterator(leftmost()); }//rb����ʼ��header����ڵ�
	iterator end() const{ return iterator(header); }
	bool empty() const { return node_count == 0; }
	size_type size() const { return node_count; }
	//�Ҹо��������ûɶ��
	size_type max_size() const { return size_type(-1); }

	std::pair<iterator, bool> insert_unique(const value_type& v);
	iterator insert_equal(const value_type & v);
	iterator find(const Key& k);
	void clear() {
		if (node_count != 0) {
			_erase(root());
			leftmost() = header;
			root() = nullptr;
			rightmost() = header;
			node_count = 0;
		}
	}

};


/*����Ϊ��������
��ν��������������ת
���һ������xΪ����ת������������� ���Ϊ�˺�������������y���ӽڵ�
�����ڶ������y �ھ�������������x�ĸ��ڵ�
�ڶ�������root ������ڵ������
*/
inline void
_rb_tree_rotate_left(_rb_tree_node_base* x,
	_rb_tree_node_base*&root) {
	//xΪ����ת��
	_rb_tree_node_base* y = x->right;//��yΪ��ת������ӽڵ�
	x->right = y->left;              
	if (y->left != nullptr)
		y->left->parent = x;
	y->parent = x->parent;     //yȡ��x��ԭ���е�λ��
	//��y��ȫ����x�ĵ�λ���뽫x���丸�ڵ�Ĺ�ϵ��ȫ���ܹ���
	if (x == root)                    //��xΪ���ڵ�
		root = y;                     //���������ø��ڵ�
	else if (x == x->parent->left)    //��xΪ�丸�ڵ�����ӽڵ�
		x->parent->left = y;          //������ֹ�ϵ���̸�y
	else
		x->parent->right = y;         //��������ӽڵ�ͬ������
	y->left = x;                      //����y��x�Ĺ�ϵ
	x->parent = y;
}
/*
�˺���Ϊ����,
��ν��������������ת
��һ������xΪ������
��������������y �������ת������ӽڵ�,Ҳ������ת֮ǰ
xΪy�ĸ��ڵ�,��ת���ϵ����
�ڶ�������rootΪ���ڵ������
*/
inline void 
_rb_tree_rotate_right(_rb_tree_node_base* x,
	_rb_tree_node_base*& root) {
	_rb_tree_node_base*y = x->left;     //yΪ��ת������ӽڵ�
	x->left = y->right;                 //��y���ҽڵ���̸�x,��Ϊy��Ϊ���ڵ�
	if (y->right != nullptr)            //���ԭy���ҽڵ�ǿ�
		y->right->parent = x;           //�������丸�ڵ�
	y->parent = x->parent;              //��x��ԭ���ڵ��y
	
	//��y��ȫ����x�ĵ�λ
	if (x == root)                      //��xΪ���ڵ�
		root = y;                       //����ڵ��޸�Ϊy
	else if (x == x->parent->right)     //��xԭ��Ϊ�丸�ڵ�����ӽڵ�
		x->parent->right = y;           //�������丸�ڵ�����ӽڵ������
	else                                  
		x->parent->left = y;            //���������丸�ڵ�����ӽڵ������
	y->right = x;                       //y��x�Ĺ�ϵ�û�
	x->parent = y;
}
//�����ƽ������
inline void _rb_tree_rebalance(_rb_tree_node_base* x, _rb_tree_node_base*& root) {
	x->color = _rb_tree_red;      //�²���Ľڵ��ȻΪ��
	while (x != root && x->parent->color == _rb_tree_red) {//����Ǹ��ڵ��Ҹ��ڵ�Ϊ��
		if (x->parent == x->parent->parent->left) {
		//������ڵ�Ϊ�游�ڵ�֮���ӽڵ�
			_rb_tree_node_base* y = x->parent->parent->right;//yΪ�����ڵ�
			if (y && y->color == _rb_tree_red) {//�������ڵ����,��Ϊ��
				x->parent->color = _rb_tree_black;//�޸ĸ��ڵ�Ϊ��
				y->color = _rb_tree_red;          //�޸Ĳ����ڵ�Ϊ��
				x->parent->parent->color = _rb_tree_red;//�޸��游�ڵ�Ϊ��
				x = x->parent->parent;             //x�޸�Ϊ���游
			}
			else {//�޲����ڵ���߲����ڵ�Ϊ��
				if (x == x->parent->right) {//����½ڵ�Ϊ���ڵ�֮���ӽڵ�
					x = x->parent;
					_rb_tree_rotate_left(x, root);//��һ������Ϊ������
				}
				x->parent->color = _rb_tree_black;//�޸ĸ��ڵ����ɫΪ��ɫ
				x->parent->parent->color =_rb_tree_red;//�޸��游�ڵ����ɫΪ��ɫ
				_rb_tree_rotate_right(x->parent->parent, root);//��һ������Ϊ������
			}
		}
		else {
		//���ڵ�Ϊ�游�ڵ�֮���ӽڵ�,���²������϶Գ�
			_rb_tree_node_base* y = x->parent->parent->left;//��yΪ�����ڵ�
			if (y && y->color == _rb_tree_red) {
			//�������ڵ������Ϊ��
				x->parent->color = _rb_tree_black; //�޸ĸ��ڵ�Ϊ��
			    y->color = _rb_tree_black;//�޸Ĳ����ڵ�Ϊ��
				x->parent->parent->color = _rb_tree_red;//�޸����游�ڵ�Ϊ��
				x = x->parent->parent;//���ϼ������
			}
			else {//�޲����ڵ���߲����ڵ�Ϊ��
				if (x == x->parent->left) {
				//����½ڵ�Ϊ���ڵ�֮���ӽڵ�
					x = x->parent;
					_rb_tree_rotate_right(x, root); //��һ������Ϊ������
				}
				x->parent->color = _rb_tree_black;//�޸ĸ��ڵ����ɫΪ��ɫ
				x->parent->parent->color = _rb_tree_red;//�޸��游�ڵ����ɫΪ��ɫ;
				_rb_tree_rotate_left(x->parent->parent, root);//��һ������Ϊ������
			}
		}
	}
	root->color = _rb_tree_black;
}



template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator 
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_insert(base_ptr _x, base_ptr _y, const value_type & v)
{
	//����_xΪ��ֵ�Ĳ����,����_yΪ�����֮���ڵ�,����vΪ��ֵ
	link_type x = static_cast<link_type>(_x);
	link_type y = static_cast<link_type>(_y);
	link_type z;
	//��������ĸ��ڵ���ͷ�ڵ� ���� ����㲢�ǿսڵ���߲�����ֵ�ļ�ֵ�Ȳ����ĸ��ڵ�ļ�ֵС
	if (y == header || x!=nullptr || key_compare(KeyOfValue()(v),key(y))) {
		z = create_node(v); //����һ���½ڵ���ֵΪv
		left(y) = z;        //�ر�ĵ�yΪheader��ʱ�� z��ΪleftmostҲ���������ӽڵ�
		if (y == header) {  //���yΪͷ�ڵ�header
			root() = z;     //�޸ĸ��ڵ�Ϊz
			rightmost() = z;//�޸����ҽڵ�Ϊz
		}
		else if (y == leftmost())//�ر��������ڵ�������ڵ�
			leftmost() = z;      //������ڵ��޸�Ϊz
	}
	else {//����,Ҳ����y��ͷ�ڵ�, xΪ�սڵ�, y�ļ�ֵ��������v�ļ�ֵ
		z = create_node(v); //����һ���½ڵ�
		right(y) = z;       //���½ڵ��Ϊ����ڵ㸸�ڵ�y�����ӽڵ�
		if (y == rightmost())//��y�����������ӽڵ�,
			rightmost() = z; //���޸������ӽڵ�
	}
	parent(z) = y;         //�޸��丸�ڵ�
	left(z) = nullptr;     
	right(z) = nullptr;
	_rb_tree_rebalance(z, header->parent); //���к������תƽ��
	++node_count;            //�ڵ�������1
	return iterator(z);      //���ز���Ľڵ�ĵ�����
}
//�˺����ѵ�һ����������������ݸ��ڶ���
template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline
typename rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::link_type
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_copy(link_type x, link_type p)
{
	link_type top = clone_node(x);
	top->parent = p;
	try {
		if (x->right)
			top->right = _copy(right(x), top);
		p = top;
		x = left(x);
		/*��ʵ��������������һ����,��������,���Ҹ���*/
		while (x != nullptr) {
			link_type y = clone_node(x);
			p->left = y;
			y->parent = p;
			if (x->right)
				y->right = _copy(right(x), y);
			p = y;
			x = left(x);
		}
	}
	catch (...) {
		_erase(top);
		throw;
	}
	return top;
}

template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_erase(link_type x)
{//�˲���������������ƽ��
	while (x != nullptr) {
		_erase(right(x));
		link_type y = left(x);
		destroy_node(x);
		x = y;
	}
}

template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
std::pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
	, bool>
	rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(const value_type & v)
{
	link_type y = header;
	link_type x = root();
	bool comp = true;
	while (x != nullptr) {//Ѱ���ʵ��Ĳ����
		y = x;
		comp = key_compare(KeyOfValue()(v), key(x));
		x = comp ? left(x) : right(x);                 //����ֵ�Ƚϲ�������
	}
	//�뿪while ��y��ָҲ���ǲ����֮���ڵ�,xΪ�����

	iterator it= iterator(y);
	if (comp) //����뿪whileѭ����ʱ��compΪ��,compΪ����ָ xΪy�����ӽڵ�
		if (it == begin())//��������֮���ڵ�Ϊ����ڵ�
			return std::pair<iterator, bool >(_insert(x, y, v), true);      //�����true�������ɹ�
	//����,xΪ�����,yΪ�����֮���ڵ�,vΪ��ֵ
		else//����,Ҳ���ǲ����֮���ڵ㲻Ϊ����ڵ�
			--it;           //��it����һ��,�˴�������ָ������it�˺�,ִ��decrement,����ο�decrement˵��
	if (key_compare(key(it.node), KeyOfValue()(v))) {
	/*
	���1:������ compΪtrue,Ҳ���ǲ����Ӧ��Ϊy�����ӽڵ�,����itȴ������==begin(),��ʱ--it֮���it���ֵӦ���Ǳ�key(y)ҪС��
	�������if��������,Ҳ����  v�ļ�ֵ�������ڵ�it�ļ�ֵ С�� y�ļ�ֵ(compΪtrue����) �Ž��в������
	���ϲ����Ǳ��� ��insertunique ��insertequal ���ʹ�õ�ʱ��,��������(������ͨ������)
	ע�⵽���������û�����õ�it������,ʼ��ʹ�õ���x��y
	���2,һ��ʼcompΪfalse,Ҳ���ǲ����Ӧ����y�����ӽڵ�,��ʱit��û�иı��������yһ��,
	������֤y�ļ�ֵ�Ƿ��ϸ���� v�ļ�ֵ,����򲻲���
	*/
		return std::pair<iterator, bool>(_insert(x, y, v), true);
	}
	//�������˱�ʾ��ֵ�ظ�,���Բ�����
	return std::pair<iterator,bool >(it,false);
}

template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::iterator 
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(const value_type & v)
{
	link_type y = header;                 //yʼ�մ���x���ڵ�
	link_type x = root();                 //x��ʱ�����
	while (x != nullptr) {
		y = x;                            
		x = key_compare(KeyOfValue()(v), key(x)) ? left(x) : right(x);   //����ֵ�Ƚ�x��������
	}
	//xΪ��ֵ�����,yΪ�����֮���ڵ�,vΪ��ֵ
	return _insert(x,y,v);
}


template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const  Key& k) {
	link_type y = header;
	link_type x = root();

	while (x != nullptr)
		if (!key_compare(key(x), k)) {
			//���е������ʾx�ļ�ֵ���ڵ���k
			y = x;
			x = left(x);
		}
		else {
			x = right(x);
		}
		iterator it = iterator(y);
		return (it == end() || key_compare(k, key(it.node))) ? end() : it;
}


_PRC_END
#endif // !MY_RB_TREE_H

