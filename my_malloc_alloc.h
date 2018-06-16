#pragma once
#ifndef MY_MALLOC_ALLOC_H
#define MY_MALLOC_ALLOC_H
#include"my_config.h"

#include<new>
#include<cstdlib>
#ifdef _IS_DEBUG
#include<iostream>
#define _THROW_BAD_ALLOC std::cerr<<"out of memory"<<std::endl; exit(1)
#else
#define _THROW_BAD_ALLOC exit(1)
#endif // _IS_DEBUG

_PRC_BEGIN

class _malloc_alloc {
private:
	//用于处理内存不足  oom也就是out of memory也就是内存不足的意思
	static void *oom_malloc(size_t n);
	static void *oom_realloc(void *p, size_t n);
	static void(*_malloc_alloc_oom_handler) ();
public:
	static void* allocate(size_t n)
	{
	   void * result = std::malloc(n);
       //如果没有足够空间改用oom_malloc()
	   if (result == nullptr) result = oom_malloc(n);
	   return result;
	}
	static void deallocate(void * p, size_t /*n*/)
	{
		std::free(p);
	}
	static void *reallocate(void *p, size_t /* old_sz*/, size_t new_sz)
	{
		void *result = std::realloc(p, new_sz);
	    //无法满需求的时候改用oom_realloc()
		if (nullptr == result) result = oom_realloc(p, new_sz);
		return result;
	}
	//模仿set_new_handler
	/*返回值同样是一个函数指针*/
	static void(* set_malloc_handler(void (*f)()/*参数是函数指针f*/ )   ) ()
	{
		void(*old)() = _malloc_alloc_oom_handler;
		_malloc_alloc_oom_handler = f;
		return (old);
	}
};
void(*_malloc_alloc::_malloc_alloc_oom_handler) () = nullptr;//写不来不想写
void *_malloc_alloc::oom_malloc(size_t n)
{
	void(*my_malloc_handler)();
	void * result;
	while (true)
	{
		my_malloc_handler = _malloc_alloc_oom_handler;
		if (nullptr == my_malloc_handler) { _THROW_BAD_ALLOC; }
		(*my_malloc_handler)();//调用指定方法
		result = std::malloc(n);
		if (result) return (result);
	}
}
void* _malloc_alloc::oom_realloc(void * p, size_t n)
{
	void(*my_malloc_handler)();
	void *result;
	while (true)
	{

		my_malloc_handler = _malloc_alloc_oom_handler;
		if (nullptr == my_malloc_handler) { _THROW_BAD_ALLOC; }
		(*my_malloc_handler)();
		result = std::realloc(p,n);
		if (result) return (result);
	}
}
_PRC_END
#endif