#pragma once
#include <iostream>
namespace zm
{
	//第一级配置器_malloc_alloc_template
	template<int inst>
	class _malloc_alloc_template
	{
	private:
		//以下函数是用来处理内存不足的情况
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

}
