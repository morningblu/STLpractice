/*红黑树的实现具有特殊技巧,首先是设置了头节点header,根节点和header互为父节点,且header本身代表了红黑树的end()
也就是存在root_>parent==header;  header->parent==root; 此操作能较快的获取到root
header的左子节点永远设置为整棵红黑树的最小键值节点也就是红黑树最左节点,这样就很快的取得begin()
header的右子节点永远设置为整棵红黑树的最大键值节点,也就是红黑树的最右子节点.
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
//红黑树节点基类
struct _rb_tree_node_base {
	using color_type = _rb_tree_color_type;
	using base_ptr = _rb_tree_node_base * ;

	color_type color;//节点颜色 
	base_ptr parent;//父节点
	base_ptr left;//左子节点
	base_ptr right;//右子节点
	//求以这个节点为根的平衡树的最小节点
	static base_ptr minimum(base_ptr x) {
		while (x->left != nullptr) x = x->left;
		return x;
	}
	//求以这个节点为根的平衡树的最大节点
	static base_ptr maximum(base_ptr x){
		while (x->right != nullptr) x = x->right;
		return x;
	}
};
//红黑树节点
template<class Value>
struct _rb_tree_node :public _rb_tree_node_base {
	using link_type = _rb_tree_node<Value>*;
	Value value_field;//节点值
};

//红黑树迭代器基类
struct _rb_tree_base_iterator {
	using base_ptr = _rb_tree_node_base::base_ptr;
	using iterator_category = bidirectional_iterator_tag;
	using difference_type = ptrdiff_t;

	base_ptr node;
	//此函数意为前进,所谓前进指的是平衡树的中序遍历的下一个位置
	void increment() {
		if (node->right != nullptr) {        //如果有右子节点
			node = node->right;              //就向右走
			while (node->left != nullptr)    //然后一直往左子树走到底
				node = node->left;           
			/*
			以上操作的原因是中序遍历的情况下
			这个节点的右子树的最小节点为下一个节点
			*/
		}
		else {                               //如果没有右子节点
			base_ptr y = node->parent;       //找出父节点
			while (node == y->right) {       //如果现行节点本身是个右子节点
				node = y;                    //就一直上溯,直到不为右子节点为止
				y = y->parent;             
			}
			if (node->right != y)            //若此时的右子节点不等于此时的父节点,此情况与根节点和父节点有关
				node = y;                   
				/*以上操作的原因是在中序遍历的情况下
				如果自己没有右节点,并且自己本身是父节点的右节点
				则父节点必然小于本身,不是自己的下一个节点,所以要不断回溯
				直到节点不为右节点,也就是左节点,此时其父节点必然刚好比自己大
				为下一个节点
				特殊地,当node本身为最右子节点,或者说是尾节点,离开while循环时node为根节点,y为header,其满足
				下面的if条件句所以node 被赋值为header,所以 尾节点的前进同样是回到header.
				特殊地当node一开始为header的时候,y为root,此时node并不会进入while循环,且并不会符合其下的if条件句
				所以node 一直为node 也就是说 header节点的 前进永远是自己
				*/
		}                                   
	}
	//此函数为后退,所谓后退指的是平衡树的中序遍历情况下的上一个位置
	void decrement() {
		//如果是红节点,且父节点的父节点等于自己, 因为header默认是红色,
		if (node->color == _rb_tree_red && node->parent->parent == node)
			node = node->right;
		/*
		以上情况发生于node为header的情况下也就是node为end()
		*/
		else if (node->left != nullptr) {     //如果有左子节点
			base_ptr y = node->left;          //令y指向左子节点
			while (y->right != nullptr)       //当y有右子节点时
				y = y->right;                 //一直往右子节点走到底
			node = y;
			/*
			以上操作的原因是在中序遍历的情况下
			如果这个节点存在左子节点,则左子树的最大节点必然是前一个节点
			*/
		}
		else {                                //既非根节点,也没有左子节点
			base_ptr y = node->parent;        //找出父节点
			while (node == y->left) {         //当其本身为左子节点
				node = y;                     //则不断上溯
				y = y->parent;                //直到不为左子节点
			}
			node = y;                         //此时父节点为所求
			/*
			以上操作的原因是在中序遍历的情况下
			若一个节点为左子节点,且无左子节点,则其父节点必然都大于这个节点
			不断上溯直到这个节点为右子节点的时候其父节点必然刚好小于这个节点
			特殊地,当node本身为最左节点的时候,也就是begin(),其按上运行,node的结果必然是header
			*/
		}
	}
};

//红黑树迭代器
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
//删除节点
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
	//复制一个节点的值和颜色
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
	size_type node_count; //追踪记录树的大小
	link_type header;//红黑树的头节点
	Compare key_compare; //节点间的键值大小比较准则

	//获取根节点
	link_type& root()const { return reinterpret_cast<link_type&>(header->parent); }
	//获取最左子节点
	link_type& leftmost()const { return  reinterpret_cast<link_type&>(header->left);}
	//获取最右子节点
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

	//取极大值和极小值
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
		color(header) = _rb_tree_red; //header被设置为红色用以区分header和root
		root() = nullptr;
		leftmost() = header; //令header 左子节点为自己
		rightmost() = header;//令header 右子节点为自己
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
		//如果x是空树
			root() = nullptr;
			leftmost() = header;
			rightmost() = header;
		}
		else {
		//x不是空树
			try {
				root() = _copy(x.root(), header);       //拷贝空黑树
			}
			catch (...) {
				put_node(header);
				throw;
			}

			leftmost() = minimum(root());//令header的左孩子为做小节点
			rightmost() = maximum(root());//令header的右孩子为最大节点
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
		//防止自身赋值
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
	iterator begin() const{ return iterator(leftmost()); }//rb树开始于header的左节点
	iterator end() const{ return iterator(header); }
	bool empty() const { return node_count == 0; }
	size_type size() const { return node_count; }
	//我感觉这个函数没啥用
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


/*以下为左旋操作
所谓左旋就是向左旋转
其第一个参数x为左旋转点代表经过左旋后 其成为了函数内声明变量y的子节点
函数内定义变量y 在经过左旋后变成了x的父节点
第二个参数root 代表根节点的引用
*/
inline void
_rb_tree_rotate_left(_rb_tree_node_base* x,
	_rb_tree_node_base*&root) {
	//x为左旋转点
	_rb_tree_node_base* y = x->right;//令y为旋转点的右子节点
	x->right = y->left;              
	if (y->left != nullptr)
		y->left->parent = x;
	y->parent = x->parent;     //y取代x在原树中的位置
	//令y完全顶替x的地位必须将x对其父节点的关系完全接受过来
	if (x == root)                    //若x为根节点
		root = y;                     //则重新设置根节点
	else if (x == x->parent->left)    //若x为其父节点的左子节点
		x->parent->left = y;          //则把这种关系过继给y
	else
		x->parent->right = y;         //如果是右子节点同样过继
	y->left = x;                      //修正y和x的关系
	x->parent = y;
}
/*
此函数为右旋,
所谓右旋就是向右旋转
第一个参数x为右旋点
函数内声明参数y 其代表旋转点的左子节点,也就是旋转之前
x为y的父节点,旋转后关系反置
第二个参数root为根节点的引用
*/
inline void 
_rb_tree_rotate_right(_rb_tree_node_base* x,
	_rb_tree_node_base*& root) {
	_rb_tree_node_base*y = x->left;     //y为旋转点的左子节点
	x->left = y->right;                 //将y的右节点过继给x,因为y变为父节点
	if (y->right != nullptr)            //如果原y的右节点非空
		y->right->parent = x;           //则修正其父节点
	y->parent = x->parent;              //把x的原父节点给y
	
	//令y完全顶替x的地位
	if (x == root)                      //若x为根节点
		root = y;                       //则根节点修改为y
	else if (x == x->parent->right)     //若x原本为其父节点的右子节点
		x->parent->right = y;           //则修正其父节点对右子节点的引用
	else                                  
		x->parent->left = y;            //否则修正其父节点对左子节点的引用
	y->right = x;                       //y与x的关系置换
	x->parent = y;
}
//红黑树平衡修正
inline void _rb_tree_rebalance(_rb_tree_node_base* x, _rb_tree_node_base*& root) {
	x->color = _rb_tree_red;      //新插入的节点必然为红
	while (x != root && x->parent->color == _rb_tree_red) {//若其非根节点且父节点为红
		if (x->parent == x->parent->parent->left) {
		//如果父节点为祖父节点之左子节点
			_rb_tree_node_base* y = x->parent->parent->right;//y为伯父节点
			if (y && y->color == _rb_tree_red) {//若伯父节点存在,且为红
				x->parent->color = _rb_tree_black;//修改父节点为黑
				y->color = _rb_tree_red;          //修改伯父节点为黑
				x->parent->parent->color = _rb_tree_red;//修改祖父节点为红
				x = x->parent->parent;             //x修改为其祖父
			}
			else {//无伯父节点或者伯父节点为黑
				if (x == x->parent->right) {//如果新节点为父节点之右子节点
					x = x->parent;
					_rb_tree_rotate_left(x, root);//第一个参数为左旋点
				}
				x->parent->color = _rb_tree_black;//修改父节点的颜色为黑色
				x->parent->parent->color =_rb_tree_red;//修改祖父节点的颜色为红色
				_rb_tree_rotate_right(x->parent->parent, root);//第一个参数为右旋点
			}
		}
		else {
		//父节点为祖父节点之右子节点,其下操作与上对称
			_rb_tree_node_base* y = x->parent->parent->left;//令y为伯父节点
			if (y && y->color == _rb_tree_red) {
			//若伯父节点存在且为红
				x->parent->color = _rb_tree_black; //修改父节点为黑
			    y->color = _rb_tree_black;//修改伯父节点为黑
				x->parent->parent->color = _rb_tree_red;//修改其祖父节点为红
				x = x->parent->parent;//往上继续检查
			}
			else {//无伯父节点或者伯父节点为黑
				if (x == x->parent->left) {
				//如果新节点为父节点之左子节点
					x = x->parent;
					_rb_tree_rotate_right(x, root); //第一个参数为右旋点
				}
				x->parent->color = _rb_tree_black;//修改父节点的颜色为黑色
				x->parent->parent->color = _rb_tree_red;//修改祖父节点的颜色为红色;
				_rb_tree_rotate_left(x->parent->parent, root);//第一个参数为左旋点
			}
		}
	}
	root->color = _rb_tree_black;
}



template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator 
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_insert(base_ptr _x, base_ptr _y, const value_type & v)
{
	//参数_x为新值的插入点,参数_y为插入点之父节点,参数v为新值
	link_type x = static_cast<link_type>(_x);
	link_type y = static_cast<link_type>(_y);
	link_type z;
	//如果插入点的父节点是头节点 或者 插入点并非空节点或者插入新值的键值比插入点的父节点的键值小
	if (y == header || x!=nullptr || key_compare(KeyOfValue()(v),key(y))) {
		z = create_node(v); //产生一个新节点其值为v
		left(y) = z;        //特别的当y为header的时候 z作为leftmost也就是最左子节点
		if (y == header) {  //如果y为头节点header
			root() = z;     //修改根节点为z
			rightmost() = z;//修改最右节点为z
		}
		else if (y == leftmost())//特别的如果父节点是最左节点
			leftmost() = z;      //则最左节点修改为z
	}
	else {//否则,也就是y非头节点, x为空节点, y的键值并不大于v的键值
		z = create_node(v); //产生一个新节点
		right(y) = z;       //令新节点成为插入节点父节点y的右子节点
		if (y == rightmost())//若y本身是最右子节点,
			rightmost() = z; //则修改最右子节点
	}
	parent(z) = y;         //修改其父节点
	left(z) = nullptr;     
	right(z) = nullptr;
	_rb_tree_rebalance(z, header->parent); //进行红黑树旋转平衡
	++node_count;            //节点总数加1
	return iterator(z);      //返回插入的节点的迭代器
}
//此函数把第一个参数代表的树传递给第二个
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
		/*其实上下两个操作是一样的,向左深入,向右复制*/
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
{//此操作并不重新修正平衡
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
	while (x != nullptr) {//寻找适当的插入点
		y = x;
		comp = key_compare(KeyOfValue()(v), key(x));
		x = comp ? left(x) : right(x);                 //按键值比较不断深入
	}
	//离开while 后y所指也就是插入点之父节点,x为插入点

	iterator it= iterator(y);
	if (comp) //如果离开while循环的时候comp为真,comp为真意指 x为y的左子节点
		if (it == begin())//如果插入点之父节点为最左节点
			return std::pair<iterator, bool >(_insert(x, y, v), true);      //其参数true代表插入成功
	//以上,x为插入点,y为插入点之父节点,v为新值
		else//否则,也就是插入点之父节点不为最左节点
			--it;           //则it后退一步,此处后退意指迭代器it退后,执行decrement,具体参考decrement说明
	if (key_compare(key(it.node), KeyOfValue()(v))) {
	/*
	情况1:当满足 comp为true,也就是插入点应当为y的左子节点,但是it却不满足==begin(),此时--it之后的it其键值应该是比key(y)要小的
	若上面的if条件成立,也就是  v的键值大于现在的it的键值 小于 y的键值(comp为true导致) 才进行插入操作
	以上操作是避免 当insertunique 和insertequal 混合使用的时候,出现问题(尽管这通常不会)
	注意到插入操作并没有利用到it迭代器,始终使用的是x和y
	情况2,一开始comp为false,也就是插入点应该是y的右子节点,此时it并没有改变其意义和y一样,
	这里验证y的键值是否严格等于 v的键值,相等则不插入
	*/
		return std::pair<iterator, bool>(_insert(x, y, v), true);
	}
	//进行至此表示键值重复,所以不插入
	return std::pair<iterator,bool >(it,false);
}

template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::iterator 
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(const value_type & v)
{
	link_type y = header;                 //y始终代表x父节点
	link_type x = root();                 //x暂时代表根
	while (x != nullptr) {
		y = x;                            
		x = key_compare(KeyOfValue()(v), key(x)) ? left(x) : right(x);   //按键值比较x不断深入
	}
	//x为新值插入点,y为插入点之父节点,v为新值
	return _insert(x,y,v);
}


template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const  Key& k) {
	link_type y = header;
	link_type x = root();

	while (x != nullptr)
		if (!key_compare(key(x), k)) {
			//进行到这里表示x的键值大于等于k
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

