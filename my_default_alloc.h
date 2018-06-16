#pragma  once
#ifndef MY_DEFAULT_ALLOC
#define MY_DEFAULT_ALLOC
#include"my_malloc_alloc.h"
_PRC_BEGIN

enum { _ALIGN = 8 };//С�������ϵ��߽�
enum {_MAX_BYTES=128};//С����������
enum {_NFREELISTS=_MAX_BYTES/_ALIGN};//�����������

template<bool thread>//��Ȼ��������֧�ֶ��߳�,��ʵ����û���������
class _default_alloc {
private:
	//��bytes �ϵ���8�ı���
	/*
	*�˴��������������һ���ֽ�����ʾ
	*����bytesΪ31Ҳ���� 0001 1111 ������8-1��ʱ�����38Ҳ����
	*0010 0110	�� 7�ķ�����Ϊ 1111 1000
	*����Ķ������ಢ�� 0010 0000 ������32
	*�����������ʵ�����Ǻ���+7��ĵ���λ,����λ�����Ƕ��ٶ���Ȼ��8�ı���
	*/
	static size_t ROUND_UP(size_t bytes)
	{
		return ((bytes + _ALIGN - 1)& ~(_ALIGN - 1));
	}


	//�������ʡ�ռ�һ������
	union obj
	{
		union obj* free_list_link;//��һ������
		char client_data[8];
	};


	//16����������
	static obj* volatile free_list[_NFREELISTS];


	//���������С�����ڼ�����������
	static size_t FREELIST_INDEX(size_t bytes) 
	{
		return ((bytes + _ALIGN - 1) / (_ALIGN)-1);//��1��Ϊ�˴�0�ſ�ʼ
	}


	/*���ش�СΪn�Ķ��󲢿��ܼ����СΪn���������鵽free_list
	*�˴�����n�Ѿ����ϵ�Ϊ8�ı���
	*/
	static void* refill(size_t n)
	{
		size_t nobjs = 20;
		/*����chunk_alloc(),����ȡ��nobjs��������Ϊfree_list ���½ڵ�
		*�˴����ݵ���nobjs������,Ҳ���ǵ�����chunk_alloc��Ż�
		*������ʵ�������������
		*/
		char* chunk = chunk_alloc(n, nobjs);
		obj* volatile * my_free_list;
		obj* result;
		obj* current_obj, * next_obj;
		/*���ֻȡ��һ������,��free_list��û�и��������ڵ�
		*�˽ڵ㽫ֱ�Ӹ����뷽ʹ��
		*/
		if (1 == nobjs)return (chunk);		
		//������˸����뷽�ڵ㻹Ҫ�Ѹ���Ľڵ����������
		my_free_list = free_list + FREELIST_INDEX(n);
		//������chunk�ռ��ڽ���free_list
		result = reinterpret_cast<obj*>(chunk);//��һ��׼�����ظ�������
		//��������free_listָ�������õĿռ�
		next_obj = reinterpret_cast<obj*>(chunk + n);
		*my_free_list = next_obj;
	     //����������Ľڵ㴮������
		for (int i = 1;;i++) {//��1��ʼ����Ϊ��0�����ظ���������
			current_obj = next_obj;
			next_obj = reinterpret_cast<obj*>(reinterpret_cast<char*>(next_obj) + n);
			if (nobjs - 1 == i) {
				current_obj->free_list_link = nullptr;
				break;
			}
			else {
				current_obj->free_list_link = next_obj;			
			}
		}
		return (result);
	}


	//����һ���ռ�,������nobjs ����СΪsize�Ŀռ�
	static char *chunk_alloc(size_t size, size_t & nobjs)
	{
		char* result;
		size_t total_bytes = size * nobjs;
		size_t bytes_left = end_free - start_free;//�ڴ��ʣ��ռ�
		if (bytes_left >= total_bytes) {
		//��ʱ�ڴ��ʣ��ռ�����Ҫ��
			result = start_free;
			start_free += total_bytes; //�ڴ��ʣ����ʼ��ַ����
			return (result);
		}
		else if (bytes_left >= size) {
	    //�ڴ��ʣ��ռ䲻����������,�������ٰ���һ���ռ�
			nobjs = bytes_left / size;
			total_bytes = size * nobjs;
			result = start_free;
			start_free += total_bytes;
			return (result);
		}
		else {
		//��ʱ��һ������ռ䶼û���˾�������������������ռ�
			size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);//��ʼʱ��heap_sizeΪ0

			//�����ò���ռ��е����ü�ֵҲ���ǽ�����ռ���䵽������С������������
			if (bytes_left > 0) {
			//����ڴ�ػ���һЩ��ͷ
			//��Ѱ���ʵ�����������
				obj*volatile * my_free_list = free_list + FREELIST_INDEX(bytes_left);
				//�������������ڴ���в���Ŀռ����
				(reinterpret_cast<obj*>(start_free))->free_list_link = *my_free_list;
				*my_free_list = reinterpret_cast<obj*>(start_free);
			}
		//����heap �ռ����Բ����ڴ��
			start_free = reinterpret_cast<char*>(std::malloc(bytes_to_get));
			if (nullptr == start_free) {
			    //heap�ռ䲻�� �ڴ�����ʧ��
				obj* volatile *my_free_list, *p;
			    /*��ʼ���Բ鿴�����Ѿ�������Ŀռ�
				*������Ѱ�ʵ�����������(Ҳ���Ǵ�����δʹ�õ��������㹻��)
				*/
				for (int i = size;i <= _MAX_BYTES;i += _ALIGN) {
					my_free_list = free_list + FREELIST_INDEX(i);
					p = *my_free_list;
					if (nullptr != p) {
					//��������������δ�õĸ�������������Ϊ�ڴ��
						*my_free_list = p->free_list_link;
						start_free = reinterpret_cast<char*>(p);
						end_free = start_free + i;
					//�ݹ������nobjs
						return (chunk_alloc(size, nobjs));
					}				
				}
				end_free = nullptr;//���ʵ�������ڴ����
				//ת���õ�һ���ռ����������� oom���ƽ��д���
				start_free = reinterpret_cast<char*>(_PRC _malloc_alloc::allocate(bytes_to_get));
			}

			heap_size += bytes_to_get;
			end_free = start_free + bytes_to_get;
			//�ݹ������nobjs
			return (chunk_alloc(size, nobjs));
		}
	}
	

	static char *start_free;//�ڴ����ʼλ��


	static char *end_free;//�ڴ�ؽ���λ��


	static size_t heap_size;


public:
	static void * allocate(size_t n)
	{
		obj* volatile * my_free_list;//ָ��obj* ָ�����͵�ָ��
		obj*result;
		//�������128bytes��ת���� ��һ��������
		if (n > static_cast<size_t>(_MAX_BYTES)) {
			return (_malloc_alloc::allocate(n));
		}
	    //������������Ѱ���ʵ���һ��
		my_free_list = free_list + FREELIST_INDEX(n);//����FREELIST_INDEX�����ƫ����
		result = *my_free_list;//���ҵ������������ my_free_list�����õõ�obj* ����
		if (result == nullptr)//���˴�Ϊ��ָ��
		{
			//���û�ҵ����õ���������,���������
			void * r = refill(ROUND_UP(n));
			return r;
		}
	    /*������������
		*Ҳ��������˴�ָ��ǿվͰ����λ�õ�ָ������ָ����һ����ͬ��С������
		*Ҳ����˵��������ά������16��ָ��,��ָ��ָ��Ķ���ʹ�ú��������
		*��Щָ��ָ�������һ�����õ�����
		*/
		*my_free_list = result->free_list_link;
		return (result);
	}
	static void deallocate(void *p, size_t n)
	{
		obj* q = reinterpret_cast<obj*>(p);
		obj* volatile * my_free_list;
		//����128�͵��õ�һ��������
		if (n > static_cast<size_t>(_MAX_BYTES))
		{
			_malloc_alloc::deallocate(p,n);
			return;
		}
		//Ѱ�Ҷ�Ӧ����������
		my_free_list = free_list + FREELIST_INDEX(n);//����ƫ��
		/*����free_list��������
		*�˴��ǰ�Ҫ���յ�����������������Ķ���,Ҳ����
		*���һ�����鱻������,��ô��������һ�����������λ��
		*/
		q->free_list_link = *my_free_list;
		*my_free_list = q;	
	}
	/*����������û�пռ���ô��Ҫ��������ռ���*/
	static void *reallocate(void *p, size_t old_sz, size_t new_sz)
	{	
	}
};
template<bool thread >
char* _default_alloc<thread>::start_free = nullptr;
template<bool thread >
char* _default_alloc<thread>::end_free = nullptr;
template<bool thread>
size_t _default_alloc<thread>::heap_size = 0;
template<bool thread>
typename _default_alloc<thread>::obj* volatile
_default_alloc<thread>::free_list[_NFREELISTS] = {nullptr};


using alloc = _PRC _default_alloc<false>;

_PRC_END
#endif // !MY_DEFAULT_ALLOC
