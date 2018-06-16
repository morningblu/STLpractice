
#pragma once
#ifndef MY_VECTOR_H
#define MY_VECTOR_H

#include"my_iterator.h"
#include"my_simple_alloc.h"
#include"my_default_alloc.h"
#include"my_uninitialized.h"
#include<initializer_list>

_PRC_BEGIN

template<typename T,class Alloc=alloc>
class vector {
public:
	using value_type      = T;
	using pointer         = value_type * ;
	using iterator        = value_type* ;
	using reference       = value_type & ;
	using size_type       = size_t;
	using difference_type = ptrdiff_t;
	using const_reference = const value_type&;

protected:
	using data_allocator = _simple_alloc<T, Alloc>;
	iterator start;//�ռ��׵�ַ
	iterator finish;//Ŀǰʹ�õ�β��ַ,׼ȷ������β���ַ
	iterator end_of_storage;//Ŀǰ���ÿռ�Ľ�β
	//���뺯��
	void insert_aus(iterator it, const T& x);
    //�黹�ռ�
	void deallocate() {
		if (start)
			data_allocator::deallocate(start,end_of_storage - start);
	}
	//����ʼ��
	void fill_initialize(size_type n, const T& value) {
		start = allocate_and_fill(n, value);
		finish = start + n;
		end_of_storage = finish;	
	}

	template<class InputIterator>
	void copy_initialize(InputIterator first, InputIterator last) {
		difference_type size =_STL_OR_PRC distance(first, last);
		start = data_allocator::allocate(size);
		finish = start + (size);
		end_of_storage = finish;
		uninitialized_copy(first, last, start);
	}
public:
	iterator begin()const { return start; }
	iterator end() const{ return finish; }
	size_type size()const { return finish - start; }
	size_type capacity() const {
		return size_type(end_of_storage - begin());
	}
	bool empty() const { return begin() == end(); }
	reference operator[](size_type n) { return *(begin() + n); }

	vector() :start(nullptr), finish(nullptr), end_of_storage(nullptr) {}
	vector(size_type n, const T& value) :start(nullptr), finish(nullptr), end_of_storage(nullptr)
	{
		fill_initialize(n, value); 
	}
	explicit vector(size_type n) :start(nullptr), finish(nullptr), end_of_storage(nullptr)
	{
		fill_initialize(n, T());
	}
	vector(const vector<value_type,Alloc>& vec) 
	{
		copy_initialize(vec.begin(), vec.end());
	}

	template<class InputIterator>
	vector(InputIterator first, InputIterator last) {
		copy_initialize(first, last);
	}

	vector(const std::initializer_list<value_type> &il) {
		copy_initialize(il.begin(), il.end());
	}

	vector<value_type,Alloc>& operator=(const vector<value_type,Alloc>& vec) {
		copy_initialize(vec.begin(), vec.end());
		return *this;
	}
	~vector() {
		destroy(start, finish);
		deallocate();
	}
	//��Ԫ��
	reference front() const{ return *begin(); }
	//βԪ��
	reference back()const { return *(end() - 1); }
	//��β��ѹ��
	void push_back(const T& x)
	{
		if (finish != end_of_storage) {
			construct(finish, x);
			++finish;
		}
		else
			insert_aus(end(), x);
	}
	//ɾ��βԪ��
	void pop_back()
	{
		--finish;
		destroy(finish);
	}
	//ɾ����������һ��λ�õĵ�����
	iterator erase(iterator it) {
		if (it + 1 != end())
			_STL_OR_PRC copy(it + 1, finish, it);
		--finish;
		destroy(finish);
		return it;
	}
	iterator erase(iterator first, iterator last)
	{
		iterator i = _STL_OR_PRC copy(last,finish, first);
		destroy(i, finish);
		finish = finish - (last - first);
		return first;
	
	}
	//�������ô�С
	void resize(size_type new_size, const T& x)
	{
		if (new_size < size())
			erase(begin() + new_size, end());
		else
			insert(end(), new_size - size(), x);
	}
	void resize(size_type new_size) { resize(new_size, T()); }
	//���
	void clear() { erase(begin(), end()); }

	void insert(iterator it, size_type n, const T& x);
	void insert(iterator it, const T& x) {
		insert(it, 1, x);
	}
protected:
	//���ÿռ䲢�����
	pointer allocate_and_fill(size_type n, const T& x)
	{
		pointer result = data_allocator::allocate(n);
		uninitialized_fill_n(result, n, x);
		return result;
	}
};




template<typename T, class Alloc>
inline void vector<T, Alloc>::insert_aus(iterator it, const T & x)
{
	if (finish != end_of_storage)//���б��ÿռ�
	{

		construct(finish, *(finish - 1));//����β��λ�ù����βԪ��һ����Ԫ��
		
		++finish;
		T x_copy = x;
		//��[it,finish-2)֮���Ԫ�����Ų��һ��λ��
		_STL_OR_PRC copy_backward(it, finish - 2, finish  - 1);
		*it = x_copy;
	}
	else
	{//�Ѿ�û�б��ÿռ�
		const size_type old_size = size();
		const size_type len = (old_size != 0 ? 2 * old_size : 1);
		iterator  new_start = data_allocator::allocate(len);//��������ռ�
		iterator  new_finish = new_start;
		try {
			//�Ȱ�itλ��֮ǰ�ĸ��ƽ��¿ռ�
			new_finish = uninitialized_copy(start, it, new_start);
			construct(new_finish, x);
		    ++new_finish;
			//��ԭ�ռ�it֮���λ�ø��ƽ��¿ռ�
			new_finish = uninitialized_copy(it, finish,new_finish);
		}
		catch(...){
			//������ڼ���ִ�����,������θ��ƵĽ��
			destroy(new_start, new_finish);
			data_allocator::deallocate(new_start, len);
			throw;
		}
		destroy(begin(), end());
		deallocate();

		start = new_start;
		finish = new_finish;
		end_of_storage = new_start + len;
	}

}

template<typename T, class Alloc>
inline void vector<T, Alloc>::insert(iterator it, size_type n, const T & x)
{
	if (n != 0)
	{
		if (size_type(end_of_storage - finish) >= n)
		{//������ÿռ��㹻��
			T x_copy = x;
			const size_type elems_after =finish - it;
			iterator  old_finish = finish;
			if (elems_after > n)
			{//������Ժ��Ԫ�ظ�����������Ԫ�ظ���
				uninitialized_copy(finish - n, finish, finish);
				finish += n;
				_STL_OR_PRC copy(it, old_finish - n,(it+n));
				_STL_OR_PRC fill(it, it + n, x_copy);
			}
			else
			{
				//�ڲ����֮�������Ԫ�ظ���С�ڵ�������Ԫ�ظ���
				uninitialized_fill_n(finish,n-elems_after, x_copy);
				finish += n-elems_after;
				uninitialized_copy(it, old_finish, finish);
				finish += elems_after;
				_STL_OR_PRC fill(it, old_finish, x_copy);
			}
		}
		else
		{
			//���ÿռ�С������Ԫ�ظ���
			//�����������ÿռ�
			const size_type old_size = size();
			//�¿ռ䳤��
			const size_type len = old_size + _STL_OR_PRC max(old_size, n);
			//���������µ�vector �ռ�
			iterator new_start = data_allocator::allocate(len);
			iterator new_finish = new_start;
			try {
				new_finish = uninitialized_copy(start, it,new_start);
				new_finish = uninitialized_fill_n(new_finish, n, x);
				new_finish = uninitialized_copy(it,finish, new_finish);
			}
			catch (...) {
				destroy(new_start, new_finish);
				data_allocator::deallocate(new_start, len);
				throw;
			}
			destroy(start, finish);
			deallocate();
			start = new_start;
			finish = new_finish;
			end_of_storage = new_start + len;

		}


	}
}
_PRC_END

#endif 

