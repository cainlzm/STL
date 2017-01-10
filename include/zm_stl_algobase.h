#pragma once
namespace zm
{
	template<class OutputIterator, class Size, class T>
	inline OutputIterator fill_n(OutputIterator first, Size n, const T& value)
	{
		for (; n>0; --n, ++first)
		{
			*first = value;
		}
		return first;
	}
}
