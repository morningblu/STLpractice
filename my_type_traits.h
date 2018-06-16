#pragma once
#ifndef MY_TYPE_TRAITS_H
#define MY_TYPE_TRAITS_H

#include"my_config.h"

_PRC_BEGIN
struct _true_type {};//无意义的函数  
struct _false_type {};//有意义的函数  

template<typename T>
struct _type_traits
{
	typedef _false_type has_trivial_default_constructor;//默认构造函数是否有意义？  
	typedef _false_type has_trivial_copy_constructor;//拷贝构造函数是否有意义?  
	typedef _false_type has_trivial_assignment_constructor;//拷贝赋值操作是否有意义?
	typedef _false_type has_trivial_assignment_operator;//拷贝赋值操作运算符是否有意义?
	typedef _false_type has_trivial_destructor;//析构函数是否有意义?  
	typedef _false_type is_POD_type;//是否为Plain Old Data?  
};


_PRC_TEMPLATE_NULL
struct _type_traits <char>
{
	typedef _true_type has_trivial_default_constructor;
	typedef _true_type has_trivial_copy_constructor;
	typedef _true_type has_trivial_assignment_constructor;
	typedef _true_type has_trivial_assignment_operator;
	typedef _true_type has_trivial_destructor;
	typedef _true_type is_POD_type;
};

_PRC_TEMPLATE_NULL
struct _type_traits <unsigned char>
{
	typedef _true_type has_trivial_default_constructor;
	typedef _true_type has_trivial_copy_constructor;
	typedef _true_type has_trivial_assignment_constructor;
	typedef _true_type has_trivial_assignment_operator;
	typedef _true_type has_trivial_destructor;
	typedef _true_type is_POD_type;
};
_PRC_TEMPLATE_NULL
struct _type_traits <short>
{
	typedef _true_type has_trivial_default_constructor;
	typedef _true_type has_trivial_copy_constructor;
	typedef _true_type has_trivial_assignment_constructor;
	typedef _true_type has_trivial_assignment_operator;
	typedef _true_type has_trivial_destructor;
	typedef _true_type is_POD_type;
};
_PRC_TEMPLATE_NULL
struct _type_traits <unsigned short>
{
	typedef _true_type has_trivial_default_constructor;
	typedef _true_type has_trivial_copy_constructor;
	typedef _true_type has_trivial_assignment_constructor;
	typedef _true_type has_trivial_assignment_operator;
	typedef _true_type has_trivial_destructor;
	typedef _true_type is_POD_type;
};
_PRC_TEMPLATE_NULL
struct _type_traits <int>
{
	typedef _true_type has_trivial_default_constructor;
	typedef _true_type has_trivial_copy_constructor;
	typedef _true_type has_trivial_assignment_constructor;
	typedef _true_type has_trivial_assignment_operator;
	typedef _true_type has_trivial_destructor;
	typedef _true_type is_POD_type;
};
_PRC_TEMPLATE_NULL
struct _type_traits <unsigned int>
{
	typedef _true_type has_trivial_default_constructor;
	typedef _true_type has_trivial_copy_constructor;
	typedef _true_type has_trivial_assignment_constructor;
	typedef _true_type has_trivial_assignment_operator;
	typedef _true_type has_trivial_destructor;
	typedef _true_type is_POD_type;
};
_PRC_TEMPLATE_NULL
struct _type_traits <long>
{
	typedef _true_type has_trivial_default_constructor;
	typedef _true_type has_trivial_copy_constructor;
	typedef _true_type has_trivial_assignment_constructor;
	typedef _true_type has_trivial_assignment_operator;
	typedef _true_type has_trivial_destructor;
	typedef _true_type is_POD_type;
};
_PRC_TEMPLATE_NULL
struct _type_traits <float>
{
	typedef _true_type has_trivial_default_constructor;
	typedef _true_type has_trivial_copy_constructor;
	typedef _true_type has_trivial_assignment_constructor;
	typedef _true_type has_trivial_assignment_operator;
	typedef _true_type has_trivial_destructor;
	typedef _true_type is_POD_type;
};
_PRC_TEMPLATE_NULL
struct _type_traits <double>
{
	typedef _true_type has_trivial_default_constructor;
	typedef _true_type has_trivial_copy_constructor;
	typedef _true_type has_trivial_assignment_constructor;
	typedef _true_type has_trivial_assignment_operator;
	typedef _true_type has_trivial_destructor;
	typedef _true_type is_POD_type;
};
_PRC_TEMPLATE_NULL
struct _type_traits <long double>
{
	typedef _true_type has_trivial_default_constructor;
	typedef _true_type has_trivial_copy_constructor;
	typedef _true_type has_trivial_assignment_constructor;
	typedef _true_type has_trivial_assignment_operator;
	typedef _true_type has_trivial_destructor;
	typedef _true_type is_POD_type;
};
template<typename T>
struct _type_traits <T*>
{
	typedef _true_type has_trivial_default_constructor;
	typedef _true_type has_trivial_copy_constructor;
	typedef _true_type has_trivial_assignment_constructor;
	typedef _true_type has_trivial_assignment_operator;
	typedef _true_type has_trivial_destructor;
	typedef _true_type is_POD_type;
};








_PRC_END
#endif // !MY_TYPE_TRAITS_H
