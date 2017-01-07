#pragma once
namespace zm
{
	template <class T1, class T2>
	inline void construct(T1* p, const T2& value)
	{
		new (p) T1(value);
	}
	template<class T>
	inline void destroy(T * p)
	{
		p->~T();
	}

	/*template<class ForwardIterator>
	inline void destroy(ForwardIterator first, ForwardIterator last)
	{
		_destroy(first, last, value_type(first));
	}

	template<class ForwardIterator, class T>
	inline void _destroy(ForwardIterator first, ForwardIterator last, T*)
	{
		typedef typename _type_traits<T>::has_trival_destructor trivial_destructor;
		_destroy_aux(first, last, trivial_destructor());
	}
	template<class ForwardIterator>
	inline void _destroy_aux(ForwardIterator first, ForwardIterator last, _false_type)
	{
		for (; first < last; ++first)
		{
			destroy(&(*first));
		}
	}

	template<class ForwardIterator>
	inline void _destroy_aux(ForwardIterator first, ForwardIterator last, _true_type) {}*/

	inline void destroy(char *, char *) {}
	inline void destroy(wchar_t *, wchar_t *) {}
}
