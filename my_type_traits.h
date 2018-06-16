#pragma once
#ifndef MY_TYPE_TRAITS_H
#define MY_TYPE_TRAITS_H

#include"my_config.h"

_PRC_BEGIN
struct _true_type {};//������ĺ���  
struct _false_type {};//������ĺ���  

template<typename T>
struct _type_traits
{
	typedef _false_type has_trivial_default_constructor;//Ĭ�Ϲ��캯���Ƿ������壿  
	typedef _false_type has_trivial_copy_constructor;//�������캯���Ƿ�������?  
	typedef _false_type has_trivial_assignment_constructor;//������ֵ�����Ƿ�������?
	typedef _false_type has_trivial_assignment_operator;//������ֵ����������Ƿ�������?
	typedef _false_type has_trivial_destructor;//���������Ƿ�������?  
	typedef _false_type is_POD_type;//�Ƿ�ΪPlain Old Data?  
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
