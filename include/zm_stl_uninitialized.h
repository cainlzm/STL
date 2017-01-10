#pragma once
#include "zm_stl_iterator.h"
#include "zm_type_traits.h"
#include "zm_stl_construct.h"
#include "zm_stl_algobase.h"
#include <string>
namespace zm
{
	//uninilized_fill_n
	template<class ForwardIterator, class Size, class T>
	inline ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& value)
	{
		_unitialized_fill_n(first, n, value, zm::value_type(first));
	}

	template<class ForwardIterator, class Size, class T, class T1>
	inline ForwardIterator _uninitialized_fill_n(ForwardIterator first, Size n, const T& value, T1*)
	{
		typedef typename zm::_type_traits<T1>::is_POD_type is_POD;
		return _uninitialized_fill_n_aux(first, n, value, is_POD());
	}
	template<class ForwardIterator, class Size, class T>
	inline ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& value, _false_type)
	{
		ForwardIterator cur = first;
		for (; n > 0; --n, ++cur)
		{
			zm::construct(&*cur, value);
		}
		return cur;
	}
	template<class ForwardIterator, class Size, class T>
	inline ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& value, _true_type)
	{
		return fill_n(first, n, value);
	}
	//uninitialized_copy
	template<class InputIterator, class ForwardIterator>
	inline ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result)
	{
		return _uninitialized_copy(first, last, result, zm::value_type(first));
	}
	template<class InputIterator, class ForwardIterator, class T>
	inline ForwardIterator _uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result ,T*)
	{
		typedef typename zm::_type_traits<T>::is_POD_type isPOD;
		return _uninitialized_copy_aux(first, last, result, isPOD());
	}
	template<class InputIterator, class ForwardIterator>
	inline ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, _false_type)
	{
		for (; first != last; ++first, ++result)
		{
			zm::construct(&*result, *first);
		}
		return result;
	}
	template<class InputIterator, class ForwardIterator>
	inline ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, _true_type)
	{
		return std::copy(first, last, result);
	}
	inline char * uninitialized_copy(const char * first, const char * last, char * result)
	{
		std::memmove(result, first, last-first);
		return result + (last - first);
	}
	inline wchar_t * uninitialized_copy(const wchar_t * first, const wchar_t * last, wchar_t * result)
	{
		std::memmove(result, first, sizeof(wchar_t)*(last-first));
		return result + (last-first);
	}
	//uninitialized_fill
	template<class ForwardIterator, class T>
	inline void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& value)
	{
		_uninitialized_fill(first, last, value, zm::value_type(first));
	}
	template<class ForwardIterator, class T , class T1>
	inline void _uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& value, T1*)
	{
		typedef typename zm::_type_traits<T1>::is_POD_type is_POD;
		_uninitialized_fill_aux(first, last , value, is_POD());
	}
	template<class ForwardIterator, class T>
	inline void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& value, _false_type)
	{
		for (; first != last; ++first)
		{
			zm::construct(&*first, value);
		}
	}
	template<class ForwardIterator, class T>
	inline void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& value, _true_type)
	{
		std::fill(first, last, value);
	}
	
}
