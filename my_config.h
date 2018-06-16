#pragma once
#ifndef MY_CONFIG_H
#define MY_CONFIG_H

#define _PRC_BEGIN namespace prc{
#define _PRC_END  }
#define _PRC prc::
//#define _IS_DEBUG
#ifndef _PRC_TEMPLATE_NULL  
#define _PRC_TEMPLATE_NULL template<>  
#endif  

#ifndef _PRC_NULL_TEMPL_ARGS
#define _PRC_NULL_TEMPL_ARGS <>
#endif 

#ifdef _USE_STL
#define _STL_OR_PRC std::
#else 
#define _STL_OR_PRC _PRC
#endif

using size_t = unsigned int;//数组下标数据类型
#endif