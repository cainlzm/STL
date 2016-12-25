#include "zm_string.h"
//#include <string>
namespace zm
{
	string::string()
	{
		m_pCharArray = new char[1];
		m_pCharArray[0] = '\0';
		m_size = 0;
		m_capacity = 1;
	}
	string::~string()
	{
		delete m_pCharArray;
		m_pCharArray = 0;
		m_size = 0;
		m_capacity = 0;
	}
	string::string(const string& other)
	{
		size_t size = other.size();
		m_pCharArray = new char[size + 1];
		for (int i = 0; i < size; ++i)
		{
			m_pCharArray[i] = other.m_pCharArray[i];
		}
		m_pCharArray[size] = '\0';
		m_size = size;
		m_capacity = size + 1;
	}
	string::string(pointer pChar)
	{
		size_t size= strlen(pChar);
		m_pCharArray = new char[size+1];
		for (int i = 0; i < size; ++i)
		{
			m_pCharArray[i] = pChar[i];
		}
		m_pCharArray[size] = '\0';
		m_size = size;
		m_capacity = size + 1;
	}
	size_t string::size() const
	{
		return m_size;
	}
	string& string::operator=(const string& other)
	{
		pointer pOrigin = m_pCharArray;
		size_t size = other.size();
		m_pCharArray = new char[size+1];
		for (int i = 0; i < size; ++i)
		{
			m_pCharArray[i] = other.m_pCharArray[i];
		}
		m_pCharArray[size] = '\0';
		m_size = size;
		m_capacity = size + 1;
		delete pOrigin;
		return *this;
	}
	string::iterator string::begin()
	{
		return m_pCharArray;
	}
	string::iterator string::end()
	{
		return m_pCharArray + m_size;
	}
	void string::clear()
	{
		m_pCharArray[0] = '\0';
		m_size = 0;
	}
	bool string::empty()
	{
		return 0 == m_size;
	}
	string::reference string::operator[](size_t index)
	{
		return m_pCharArray[index];
	}
	string& string::operator+=(const string& other)
	{
		size_t size = m_size + other.size();
		//重新分配空间
		if (size >= m_capacity)
		{
			reallocate(size+1);
		}
		for (int i = 0; i < other.m_size; ++i)
		{
			m_pCharArray[m_size + i] = other.m_pCharArray[i];
		}
		m_pCharArray[size] = '\0';
		m_size = size;
		return *this;
	}
	void string::reallocate(size_t n)
	{
		pointer pOrigin = m_pCharArray;
		m_pCharArray = new char[n];
		for (int i = 0; i < m_size; ++i)
		{
			m_pCharArray[i] = pOrigin[i];
		}
		m_pCharArray[m_size] = '\0';
		m_size = n;
		m_capacity = n + 1;
		delete pOrigin;
	}
	void string::push_back(char c)
	{
		if (m_size == m_capacity - 1)
		{
			reallocate(m_capacity*2);
		}
		m_pCharArray[m_size++] = c;
		m_pCharArray[m_size] = '\0';
	}
	std::ostream& operator<<(std::ostream& out, zm::string& s)
	{
		zm::string::size_t size = s.size();
		for (int i = 0; i < size; ++i)
		{
			out << s[i];
		}
		return out;
	}
	
}

