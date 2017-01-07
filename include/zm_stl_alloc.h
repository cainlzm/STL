#pragma once
#include <iostream>
namespace zm
{
	//��һ��������_malloc_alloc_template
	template<int inst>
	class _malloc_alloc_template
	{
	private:
		//���º��������������ڴ治������
		static void * oom_malloc(size_t);
		static void * oom_realloc(void *, size_t);
		static void(*_malloc_alloc_oom_handler)();
	public:
		static void * allocate(size_t n)
		{
			void * result = malloc(n);
			if (0 == result)
				result = oom_malloc(n);
			return result;
		}
		static void * dellocate(void * p, size_t n)
		{
			free(p);
		}
		static void * reallocate(void *p, size_t old_size, size_t new_size)
		{
			void * result = realloc(p, new_size);
			if (0 == result)
				result = oom_realloc(p, new_size);
			return result;
		}
		static void (* set_malloc_handler(void(*f)()))()
		{
			void(*old)() = _malloc_alloc_oom_handler;
			_malloc_alloc_oom_handler = f;
			return (old);
		}
	};
	template<int inst>
	void * _malloc_alloc_template<inst>::oom_malloc(size_t n)
	{
		void(*my_malloc_handler)() = _malloc_alloc_oom_handler;
		void * result = 0;
		for (;;)
		{
			if (0 == my_malloc_handler)
			{
				std::cerr << "out of memory";
				exit(0);
			}
			(*my_malloc_handler)();
			result = malloc(n);
			if (result)
				break;
		}
		return result;
	}
	template<int inst>
	void * _malloc_alloc_template<inst>::oom_realloc(void *p, size_t n)
	{
		void(*my_malloc_handler)() = _malloc_alloc_oom_handler;
		void * result = 0;
		for (;;)
		{
			if (0 == my_malloc_handler)
			{
				std::cerr << "out of memory";
				exit(1);
			}
			(*my_malloc_handler)();
			result = realloc(p, n);
			if (result)
				break;
		}
		return result;
	}
	typedef _malloc_alloc_template<0> malloc_alloc;

	//�ڶ��������� _default_alloc_template

	enum {_ALIGN=8};                      //С��������ϵ��߽�
	enum {_MAX_BYTES = 128};              //С��������Ͻ�
	enum {_NFREELISTS=_MAX_BYTES/_ALIGN}; //free-lists ����

	template<bool threads, int inst>
	class _default_alloc_template
	{
	private:
		//ROUND_UP() ��bytes�ϵ���8�ı���
		static size_t ROUND_UP(size_t bytes)
		{
			return (bytes + _ALIGN - 1)&(~(_ALIGN - 1));
		}
	private:
		//free_lists�Ľڵ㹹��
		union obj
		{
			union obj * free_list_link;
			char client_data[1];
		};
	private:
		//16��free_lists
		static obj * volatile free_list[_NFREELISTS];
		//���º������������С������ʹ�õ�n��free-list, n��0����
		static size_t FREELIST_INDEX(size_t bytes)
		{
			return (bytes + _ALIGN - 1) / _ALIGN - 1;
		}
		//����һ����СΪn�Ķ��� �����ܼ����СΪn���������鵽free list
		static void * refill(size_t n);
		//����һ���ռ䣬������nobjs����СΪsize������
		//�������nobjs�������������㣬 nobjs���ܻή��
		static char * chunk_alloc(size_t size, int& nobjs);

		//�ڴ��
		static char * start_free;   //�ڴ�صĿ�ʼλ�ã� ֻ��chunk_alloc�б仯
		static char * end_free;     //�ڴ�صĽ���λ�ã� ֻ��chunk_alloc�б仯
		static size_t heap_size;
	public:
		static void * allocate(size_t n);
		static void deallocate(void * p, size_t n);
		static void * realloc(void * p, size_t old_sz, size_t new_sz);
	};

	//��ֵ
	template<bool threads, int inst>
	char * _default_alloc_template<threads, inst>::start_free = 0;

	template<bool threads, int inst>
	char * _default_alloc_template<threads, inst>::end_free = 0;

	template<bool threads, int inst>
	size_t _default_alloc_template<threads, inst>::heap_size = 0;

	template<bool threads, int inst>
	_default_alloc_template<threads, inst>::obj * volatile
	_default_alloc_template<threads, inst>::free_list[_NFREELISTS] =
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

	template<bool threads, int inst>
	void * _default_alloc_template<threads, inst>::allocate(size_t n)
	{
		obj * volatile * my_free_list;
		obj * result;
		//����128�͵��õ�һ��������
		if (n > _MAX_BYTES)
		{
			return malloc_alloc::allocate(n);
		}
		//Ѱ��16��freelist���ʵ���һ��
		my_free_list = free_list + FREELIST_INDEX(n);
		result = *my_free_list;
		if (0 == result)
		{
			result = refill(ROUND_UP(n));
			return result;
		}
		*my_free_list = result->free_list_link;
		return result;
	}
	template<bool threads, int inst>
	void _default_alloc_template<threads, inst>::deallocate(void * p, size_t n)
	{
		obj * q = (obj *)p;
		obj * volatile * my_free_list;

		//����128�͵��õ�һ��������
		if (n > _MAX_BYTES)
		{
			malloc_alloc::dellocate(p, n);
			return;
		}
		my_free_list = free_list + FREELIST_INDEX(n);
		q->free_list_link = *my_free_list;
		*my_free_list = q;
	}
	template<bool threads, int inst>
	void * _default_alloc_template<threads, inst>::refill(size_t n)
	{
		int nobjs = 20;
		char * chunk = chunk_alloc(n, nobjs);
		obj * volatile * my_free_list;
		obj * result;
		obj * current_obj, *next_obj;
		int i;
		if (nobjs == 1)
			return chunk;
		//׼������free_list�������µĽڵ�
		my_free_list = free_list + FREELIST_INDEX(n);
		result = (obj *)chunk;
		*my_free_list = next_obj = (obj*)(chunk + n);
		for (int i = 1;; i++)
		{
			current_obj = next_obj;
			next_obj = (obj *)((char *)next_obj + n);
			if (nobjs - 1 == i)
			{
				current_obj->free_list_link = 0;
				break;
			}
			else
			{
				current_obj->free_list_link = next_obj;
			}
		}
		return result;
	}
    
	template<bool threads, int inst>
	char * _default_alloc_template<threads, inst>::chunk_alloc(size_t size, int& nobjs)
	{
		char * result;
		size_t total_bytes = size*nobjs;
		size_t bytes_left = end_free - start_free;

		if (bytes_left >= total_bytes)
		{
			result = start_free;
			start_free += total_bytes;
			return result;
		}
		else if (bytes_left >= size)
		{
			nobjs = bytes_left / size;
			result = start_free;
			start_free += size*nobjs;
			return result;
		}
		else
		{
			size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size>>4);
			//��ʣ����ڴ滹�����ü�ֵ
			if (bytes_left > 0)
			{
				obj * volatile * my_free_list = free_list + FREELIST_INDEX(bytes_left);
				(obj*)(start_free)->free_list_link = *my_free_list;
				my_free_list = (obj *)start_free;
			}
			start_free = (char*)malloc(bytes_to_get);
			if (0 == start_free)
			{
				int i;
				obj * volatile * my_free_list, *p;
				for (int i = size; i <= _MAX_BYTES; i += _ALIGN)
				{
					my_free_list = free_list + FREELIST_INDEX(i);
					p = *my_free_list;
					if (0 != p)
					{
						*my_free_list = p->free_list_link;
						start_free = (char *)p;
						end_free = start_free + i;
						return chunk_alloc(size, nobjs);
					}
				}
				end_free = 0;
				start_free = (char *)malloc_alloc::allocate(bytes_to_get);
			}
			heap_size += bytes_to_get;
			end_free = start_free + bytes_to_get;
			return chunk_alloc(size, nobjs);
		}
	}
}
