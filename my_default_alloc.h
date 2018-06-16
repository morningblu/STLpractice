#pragma  once
#ifndef MY_DEFAULT_ALLOC
#define MY_DEFAULT_ALLOC
#include"my_malloc_alloc.h"
_PRC_BEGIN

enum { _ALIGN = 8 };//小型区块上调边界
enum {_MAX_BYTES=128};//小型区块上限
enum {_NFREELISTS=_MAX_BYTES/_ALIGN};//自由链表个数

template<bool thread>//虽然看起来像支持多线程,但实际上没有相关内容
class _default_alloc {
private:
	//将bytes 上调到8的倍数
	/*
	*此处可能难以理解以一个字节做演示
	*假设bytes为31也就是 0001 1111 当加上8-1的时候等于38也就是
	*0010 0110	而 7的反运算为 1111 1000
	*上面的二进制相并得 0010 0000 正好是32
	*整个这个运算实际上是忽略+7后的低三位,其他位无论是多少都必然是8的倍数
	*/
	static size_t ROUND_UP(size_t bytes)
	{
		return ((bytes + _ALIGN - 1)& ~(_ALIGN - 1));
	}


	//联合体节省空间一物两用
	union obj
	{
		union obj* free_list_link;//下一个区块
		char client_data[8];
	};


	//16个自由链表
	static obj* volatile free_list[_NFREELISTS];


	//根据区块大小决定第几号自由链表
	static size_t FREELIST_INDEX(size_t bytes) 
	{
		return ((bytes + _ALIGN - 1) / (_ALIGN)-1);//减1是为了从0号开始
	}


	/*返回大小为n的对象并可能加入大小为n的其他区块到free_list
	*此处假设n已经被上调为8的倍数
	*/
	static void* refill(size_t n)
	{
		size_t nobjs = 20;
		/*调用chunk_alloc(),尝试取得nobjs个区块作为free_list 的新节点
		*此处传递的是nobjs的引用,也就是当调用chunk_alloc后才会
		*决定真实申请的区块数量
		*/
		char* chunk = chunk_alloc(n, nobjs);
		obj* volatile * my_free_list;
		obj* result;
		obj* current_obj, * next_obj;
		/*如果只取得一个区块,则free_list并没有更多的申请节点
		*此节点将直接给申请方使用
		*/
		if (1 == nobjs)return (chunk);		
		//否则除了给申请方节点还要把更多的节点给自由链表
		my_free_list = free_list + FREELIST_INDEX(n);
		//以下在chunk空间内建立free_list
		result = reinterpret_cast<obj*>(chunk);//这一块准备返回给申请者
		//以下引导free_list指向新配置的空间
		next_obj = reinterpret_cast<obj*>(chunk + n);
		*my_free_list = next_obj;
	     //将各多申请的节点串接起来
		for (int i = 1;;i++) {//从1开始是因为第0个返回给了申请者
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


	//配置一大块空间,可容纳nobjs 个大小为size的空间
	static char *chunk_alloc(size_t size, size_t & nobjs)
	{
		char* result;
		size_t total_bytes = size * nobjs;
		size_t bytes_left = end_free - start_free;//内存池剩余空间
		if (bytes_left >= total_bytes) {
		//此时内存池剩余空间满足要求
			result = start_free;
			start_free += total_bytes; //内存池剩余起始地址更新
			return (result);
		}
		else if (bytes_left >= size) {
	    //内存池剩余空间不能满足需求,但是至少包含一个空间
			nobjs = bytes_left / size;
			total_bytes = size * nobjs;
			result = start_free;
			start_free += total_bytes;
			return (result);
		}
		else {
		//此时连一个需求空间都没有了就申请至少两倍的申请空间
			size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);//初始时候heap_size为0

			//试着让残余空间有点利用价值也就是将残余空间分配到其他更小的自由链表上
			if (bytes_left > 0) {
			//如果内存池还有一些零头
			//则寻找适当的自由链表
				obj*volatile * my_free_list = free_list + FREELIST_INDEX(bytes_left);
				//调整自由链表将内存池中残余的空间编入
				(reinterpret_cast<obj*>(start_free))->free_list_link = *my_free_list;
				*my_free_list = reinterpret_cast<obj*>(start_free);
			}
		//配置heap 空间用以补充内存池
			start_free = reinterpret_cast<char*>(std::malloc(bytes_to_get));
			if (nullptr == start_free) {
			    //heap空间不足 内存申请失败
				obj* volatile *my_free_list, *p;
			    /*开始尝试查看我们已经申请过的空间
				*以下搜寻适当的自由链表(也就是存在尚未使用的区域且足够大)
				*/
				for (int i = size;i <= _MAX_BYTES;i += _ALIGN) {
					my_free_list = free_list + FREELIST_INDEX(i);
					p = *my_free_list;
					if (nullptr != p) {
					//自由链表内尚有未用的更大的区块则调整为内存池
						*my_free_list = p->free_list_link;
						start_free = reinterpret_cast<char*>(p);
						end_free = start_free + i;
					//递归的修正nobjs
						return (chunk_alloc(size, nobjs));
					}				
				}
				end_free = nullptr;//如果实在是无内存可用
				//转调用第一级空间配置器让其 oom机制进行处理
				start_free = reinterpret_cast<char*>(_PRC _malloc_alloc::allocate(bytes_to_get));
			}

			heap_size += bytes_to_get;
			end_free = start_free + bytes_to_get;
			//递归的修正nobjs
			return (chunk_alloc(size, nobjs));
		}
	}
	

	static char *start_free;//内存池起始位置


	static char *end_free;//内存池结束位置


	static size_t heap_size;


public:
	static void * allocate(size_t n)
	{
		obj* volatile * my_free_list;//指向obj* 指针类型的指针
		obj*result;
		//如果超过128bytes就转调用 第一级配置器
		if (n > static_cast<size_t>(_MAX_BYTES)) {
			return (_malloc_alloc::allocate(n));
		}
	    //在自由链表中寻找适当的一个
		my_free_list = free_list + FREELIST_INDEX(n);//其中FREELIST_INDEX算的是偏移量
		result = *my_free_list;//将我的自由链表变量 my_free_list解引用得到obj* 类型
		if (result == nullptr)//若此处为空指针
		{
			//如果没找到可用的自由链表,就重新填充
			void * r = refill(ROUND_UP(n));
			return r;
		}
	    /*调整自由链表
		*也就是如果此处指针非空就把这个位置的指针重新指向下一个相同大小的区块
		*也就是说自由链表维护的是16个指针,当指针指向的对象被使用后就脱离了
		*这些指针指向的是下一个可用的区块
		*/
		*my_free_list = result->free_list_link;
		return (result);
	}
	static void deallocate(void *p, size_t n)
	{
		obj* q = reinterpret_cast<obj*>(p);
		obj* volatile * my_free_list;
		//大于128就调用第一级配置器
		if (n > static_cast<size_t>(_MAX_BYTES))
		{
			_malloc_alloc::deallocate(p,n);
			return;
		}
		//寻找对应的自由链表
		my_free_list = free_list + FREELIST_INDEX(n);//计算偏移
		/*调整free_list回收区块
		*此处是把要回收的区块置于自由链表的顶端,也就是
		*如果一个区块被回收了,那么他处于下一个将被分配的位置
		*/
		q->free_list_link = *my_free_list;
		*my_free_list = q;	
	}
	/*当自由链表没有空间那么就要重新申请空间了*/
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
