#pragma once
#ifndef ZM_STRING_H_
#define ZM_STRING_H_
#ifdef STL_EXPORTS
#define DllExport __declspec(dllexport)
#else
#define DllExport __declspec(dllimport)
#endif
#include <ostream>
namespace zm
{
	class DllExport string
	{
	public:
		typedef char value_type;
		typedef char* pointer;
		typedef char& reference;
		typedef char* iterator;
		typedef unsigned int size_t;
	public:
		string();
		string(const string& other);
		string(pointer pChar);
		~string();
	public:
		size_t size() const;
		string& operator=(const string& other);
		reference operator[](size_t index);
		string& operator+=(const string& other);
		iterator begin();
		iterator end();
		void clear();
		bool empty();
		void push_back(value_type);
	public:
		void reallocate(size_t n);
	private:
		pointer m_pCharArray;    //字符数组指针
		size_t  m_size;          //字符数
		size_t  m_capacity;      //数组容量
	};
	DllExport std::ostream& operator<<(std::ostream&, zm::string& s);
}




#endif // !string
