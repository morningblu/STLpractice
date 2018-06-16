#pragma once
#ifndef MY_ITERATOR_H
#define MY_ITERATOR_H
#include"my_config.h"
_PRC_BEGIN
//迭代器标签
#ifdef _USE_STL
#include<iterator>
//输入迭代器
struct input_iterator_tag :public std::input_iterator_tag {} ; //为了与stl 接合
//输出迭代器
struct output_iterator_tag:public std::output_iterator_tag {};//为了与stl 接合
//单向迭代器(向前)
struct forward_iterator_tag :public input_iterator_tag{};
//双向迭代器
struct bidirectional_iterator_tag :public forward_iterator_tag {};
//随机访问迭代器
struct random_access_iterator_tag :public bidirectional_iterator_tag{};
#else
//输入迭代器
#include<istream>
struct input_iterator_tag {}; 
//输出迭代器
struct output_iterator_tag  {};
//单向迭代器(向前)
struct forward_iterator_tag :public input_iterator_tag {};
//双向迭代器
struct bidirectional_iterator_tag :public forward_iterator_tag {};
//随机访问迭代器
struct random_access_iterator_tag :public bidirectional_iterator_tag {};
#endif
//基础迭代器
template<typename Category,
         typename T,
	     typename Distance=ptrdiff_t,
	     typename Pointer=T*,
	     typename Reference=T&>
struct iterator {
	using iterator_category = Category;
	using value_type        = T;
	using difference_type   = Distance;
	using pointer           = Pointer;
	using reference         = Reference;
};

//迭代器特性萃取
template<typename I>
struct Iterator_traits {
	using value_type = typename I::value_type;
	using iterator_category = typename I::iterator_category;
	using difference_type = typename I::difference_type;
	using pointer =typename  I::pointer;
	using reference = typename I::reference;
};

template<typename T>
struct Iterator_traits<const T*> {
	using value_type = T;
	using iterator_category = random_access_iterator_tag;
	using difference_type = ptrdiff_t;
	using pointer = const T*;
	using reference = const T&;
};

template<typename T>
struct Iterator_traits<T*> {
	using value_type = T;
	using iterator_category = random_access_iterator_tag;
	using difference_type = ptrdiff_t;
	using pointer = T * ;
	using reference = T & ;
};

template <class Iterator>
inline typename Iterator_traits<Iterator>::value_type*
value_type(const Iterator&) {
	return static_cast<typename Iterator_traits<Iterator>::value_type*>(0);
}

template <class Iterator>
inline typename Iterator_traits<Iterator>::iterator_category
iterator_category(const Iterator&) {
	return typename Iterator_traits<Iterator>::iterator_category();
}

template <class Iterator>
inline typename Iterator_traits<Iterator>::difference_type*
distance_type(const Iterator&) {
	return static_cast<typename Iterator_traits<Iterator>::difference_type*>(0);
}
//向后插入迭代器类
template<class Container>
class back_insert_iterator {
protected:
	Container * container; //底层容器
public:
	using iterator_category = output_iterator_tag;
	using value_type = void;
	using difference_type = void;
	using pointer = void;
	using reference=void;

	explicit back_insert_iterator(Container& x) :container(&x) {}
	back_insert_iterator<Container>&
		operator=(const typename Container::value_type& value) {
		container->push_back(value);
		return *this;
	}
	//以下功能关闭
	back_insert_iterator<Container>& 
		operator*() { return *this; }
	back_insert_iterator<Container>&
		operator++() { return *this; }
	back_insert_iterator<Container>&
		operator++(int) { return *this; }
	back_insert_iterator<Container>&
		operator--() { return *this; }
	back_insert_iterator<Container>&
		operator--(int) { return *this; }
};
//通过这个函数产生向后插入迭代器
template<class Container>
inline back_insert_iterator<Container>
back_inserter(Container& x) {
	return back_insert_iterator<Container>(x);
}
//向后插入迭代器类
template<class Container>
class front_insert_iterator {
protected:
	Container * container;
public:
	using iterator_category = output_iterator_tag;
	using value_type = void;
	using difference_type = void;
	using pointer = void;
	using reference = void;

	explicit front_insert_iterator(Container& x) :container(&x) {}
	front_insert_iterator<Container>&
		operator=(const typename Container::value_type& value) {
		container->push_front(value);
		return *this;
	}
	front_insert_iterator<Container>& operator*() { return *this; }
	front_insert_iterator<Container>& operator++() { return *this; }
	front_insert_iterator<Container>& operator++(int) { return *this; }
	front_insert_iterator<Container>& operator--() { return *this; }
	front_insert_iterator<Container>& operator--(int) { return *this; }
};
//通过此函数获得向后插入迭代器
template<class Container>
inline front_insert_iterator<Container> front_inserter(Container& x) {
	return front_insert_iterator<Container>(x);
}
//插入迭代器类
template<class Container>
class insert_iterator {
protected:
	Container * container;
	typename Container::iterator iter;
public:
	using iterator_category = output_iterator_tag;
	using value_type = void;
	using difference_type = void;
	using pointer = void;
	using reference = void;

	insert_iterator(Container& x, typename Container::iterator i) :
		container(&x), iter(i) {}
	insert_iterator<Container>&
		operator=(const typename Container::value_type& value) {
		iter = container->insert(iter, value);//因为在插入之后迭代器可能失效,所以重新赋值
		++iter;    //此处使得iter永远是指向最初的那个迭代器
		return *this;
	}
	insert_iterator<Container>& operator*() { return *this; }
	insert_iterator<Container>& operator++() { return *this; }
	insert_iterator<Container>& operator++(int) { return *this; }
	insert_iterator<Container>& operator--() { return *this; }
	insert_iterator<Container>& operator--(int) { return *this; }
};

//通过此函数产生insert_iterator对象
template<class Container,class Iterator>
inline insert_iterator<Container> inserter(Container& x, Iterator it) {
	using iter = typename Container::iterator;
	return insert_iterator<Container>(x, iter(it));
}
//反向迭代器
template<class Iterator>
class reverse_iterator {
protected:
	Iterator current;
public:
	using iterator_category = typename Iterator_traits<Iterator>::iteartor_category;
	using value_type = typename Iterator_traits<Iterator>::value_type;
	using difference_type = typename Iterator_traits<Iterator>::value_type;
	using pointer = typename Iterator_traits<Iterator>::pointer;
	using reference = typename Iterator_traits<Iterator>::reference;

	using iterator_type = Iterator;
	using self = reverse_iterator<Iterator>;
public:
	reverse_iterator() {};
	explicit reverse_iterator(iterator_type x) :current(x) {}
	reverse_iterator(const self& x) :current(x.current) {}
	iterator_type base()const { return current; }
	reference operator*()const {
		iterator_type tmp = current;
		return *(--tmp);
	}
	pointer operator->() const{return &(operator*());}
	self& operator++() {
		--current;
		return *this;
	}
	self operator++(int) {
		self tmp = *this;
		--current;
		return tmp;
	}
	self& operator--() {
		++current;
		return *this;
	}
	self operator--(int) {
		self tmp = *this;
		++current;
		return tmp;
	}

	self operator+(difference_type n)const {
		return self(current - n);
	}
	self operator+=(difference_type n) {
		current -=n;
		return *this;
	}
	self operator-(difference_type n)const {
		return self(current + n);
	}
	self& operator-=(difference_type) {
		current += n;
		return *this;
	}
	reference operator[](difference_type n) const { return  *(*this + n); }
};

//此处参考PJstl进行实现,原stl源码剖析上在此处实现的有点傻
template<class T,
	class Elem=char,
	class Traits=std::char_traits<Elem>,
	class Distance=ptrdiff_t>
class istream_iterator {
public:
	using iterator_category = input_iterator_tag;
	using value_type = T;
	using difference_type = Distance;
	using pointer = const T*;
	using reference = const T&;
	using istream_type = std::basic_istream<Elem, Traits>;
protected:
	istream_type * stream;
	value_type value;
	void read() {
		if (stream != nullptr && !(*stream >> value)) {
			stream = nullptr;
		}
	}
public:

	istream_iterator() :stream(nullptr),value() {}
    explicit istream_iterator(istream_type& s) :stream(&s) { read(); }

	reference operator*()const { return value; }
	pointer operator->()const { return &(operator*()); }
	istream_iterator<T,Elem,Traits, Distance>& operator++() {
		read();
		return *this;
	}
	istream_iterator<T, Elem, Traits, Distance> operator++(int) {
		istream_iterator<T, Elem, Traits, Distance> tmp = *this;
		read();
		return tmp;
	}
	bool _equal(const istream_iterator<T, Elem, Traits, Distance>& y)const { return stream == y.stream; }
};
template<class T,
	     class Elem,
	     class Traits,
	     class Distance>
bool
operator==(const istream_iterator<T, Elem, Traits, Distance>& x,
	     const istream_iterator<T, Elem, Traits, Distance>& y) {
	return x._equal(y);
}
template<class T,
	class Elem,
	class Traits,
	class Distance>
bool
operator!=(const istream_iterator<T, Elem, Traits, Distance>& x,
		   const istream_iterator<T, Elem, Traits, Distance>& y) {
	return !(x==y);
}
//此处参考PJstl进行实现,原stl源码剖析上在此处实现的有点傻
template<class T,
	class Elem = char,
	class Traits = std::char_traits<Elem>>
class ostream_iterator {
	public:
		using iterator_category = output_iterator_tag;
		using value_type = void;
		using difference_type = void;
		using pointer = void;
		using reference = void;
		using ostream_type = std::basic_ostream<Elem, Traits>;
	protected:
		ostream_type * stream;
		const Elem *str;
	public:
		ostream_iterator(ostream_type& s, const Elem* const _str) :stream(&s), str(_str) { }
		ostream_iterator<T, Elem, Traits>& operator=(const T& val ) {
			*stream << val;
			if (str != nullptr)
				*stream << str;
			return (*this);
		}

		ostream_iterator<T, Elem, Traits>& operator*(){ return (*this); }
		ostream_iterator<T, Elem, Traits>& operator++() {
			return (*this);
		}
		ostream_iterator<T, Elem, Traits> operator++(int) {
			return (*this);
		}
};
_PRC_END
#endif // !MY_ITERATOR_H
