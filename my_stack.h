#pragma once
#include "my_allocator.h"
#include <iostream>
#include <list>
#include <memory>
#include <utility>

template<typename T, typename Allocator>
struct DataBuf
{
	T* buf = nullptr;
	size_t capacity;
	size_t size = 0;
	Allocator m_allocator;
	using traits = std::allocator_traits<Allocator>;
	DataBuf(size_t n = 0)
	{
		buf = m_allocator.allocate(n, alignof(T));
		capacity = n;
		size = 0;
	}
	DataBuf(const DataBuf<T, Allocator>& other)
	{
		buf = m_allocator.allocate(other.capacity, alignof(T));
		capacity = other.capacity;
		std::uninitialized_copy_n(other.buf, other.size, buf);
		size = other.size;
		//size_t i = 0;
		//try
		//{
		//for(; i < other.size; ++i)
		//	{
		//	buf[i] = other[i];
		//	size += 1;
		//	}
		//} catch (...)
		//{
		//	throw;
		//}
	}



	DataBuf(DataBuf<T, Allocator>&& other) noexcept
	{
		buf = other.buf;
		size = other.size;
		capacity = other.capacity;
		other.buf = nullptr;
		other.size = 0;
		other.capacity = 0;
	}



	DataBuf& operator=(const DataBuf<T, Allocator>& other)
	{
		if(other.size >= capacity)
		{
			DataBuf<T, Allocator> tmp(other);
			tmp.Swap(*this);
		} else {
			DataBuf<T, Allocator> tmp(capacity);
			std::uninitialized_copy_n(other.buf, other.size, tmp.buf);
			tmp.size = other.size;
			//for(size_t i = 0; i < other.size; ++i)
			//{
			//tmp[i] = other[i];
			//tmp.size += 1;
			//}
			//tmp.Swap(*this);
		}
		return *this;
	}


	DataBuf& operator=(DataBuf<T, Allocator>&& other)
	{
		if(other.size >= capacity)
		{
			DataBuf<T, Allocator> tmp(std::move(other));
			tmp.Swap(*this);
		} else {
			DataBuf<T, Allocator> tmp(capacity);
			std::uninitialized_move_n(other.buf, other.size, tmp.buf);
			tmp.size = other.size;
			//for(size_t i = 0; i < other.size; ++i)
			//{
			//tmp[i] = std::move(other[i]);
			//tmp.size += 1;
			//}
			//tmp.Swap(*this);
		}
		return *this;
	}

	void clear()
	{
		for(size_t i = 0; i < size; ++i)
		{
			buf[i].~T();
		}
		size = 0;
	}



	~DataBuf()
	{
		clear();
		m_allocator.deallocate(buf, capacity, alignof(T));
	}

	T* operator+(size_t i)
	{
		return buf + i;
	}



	const T* operator+(size_t i) const
	{
		return buf + i;
	}

	T& operator[](size_t i)
	{
		return buf[i];
	}



	const T& operator[](size_t i) const
	{
		return buf[i];
	}

	//template<typename... Args>
	//void Emplace(Args&&... args);
	
	void Destroy()
	{
		buf[size - 1].~T();
		size -= 1;
	}

	void Emplace(const T& arg);

	void Emplace(T&& arg);

	void Swap(DataBuf<T, Allocator>& other) noexcept
	{
		std::swap(buf, other.buf);
		std::swap(size, other.size);
		std::swap(capacity, other.capacity);
	}

};

template<typename T, typename A = my_allocator<T>>
class my_stack
{
	DataBuf<T, A> data;


	public:
	using allocator_type = A;
	my_stack(size_t n = 1) : data(n)
	{
	for (size_t i = 0; i < n; ++i)
	{
		data.Emplace(T());
	}
	}


	void Resize(size_t i)
	{
		if (data.capacity < i)
		{
			DataBuf<T, A> tmp(i);
			tmp = data;
			data.Swap(tmp);
		} else if (data.capacity > i && data.size <= i)
		{
			data.m_allocator.deallocate(data.buf + i, i - data.size, alignof(T)); 
		} else
		{
			throw std::invalid_argument("Cannot resize to this size, because it less then size of stack\n");
		}
	}



	void Push(const T& elem)
	{
		data.Emplace(elem);
	}


	void Pop()
	{
		if(data.size > 0){
		data.Destroy();
		} else 
		{
			throw std::invalid_argument("Stack is empty\n");
		}

	}


	T& Top()
	{
		if(data.size > 0){
		return data[data.size - 1];
		} else 
		{
			throw std::invalid_argument("Stack is empty\n");
		}
	}

	const T& Top() const
	{
		if(data.size > 0){
		return data[data.size - 1];
		} else 
		{
			throw std::invalid_argument("Stack is empty\n");
		}
	}


	T pop()
	{
		T ret = Top();
		Pop();
		return ret;
	}

	size_t Size()
	{
		return data.capacity;
	}


	size_t Capacity()
	{
		return data.size;
	}



};



//DEFINITIONS:
/*
	template<typename T, typename Allocator>
	template<typename... Args>
        void DataBuf<T, Allocator>::Emplace(Args&&... args)
	{	
#ifdef DEBUG
		std::cout << "Emplace by constructor \n";
#endif
		if(size == capacity)
		{
			size_t newcap = capacity * 2;
			T* buf2 = m_allocator.allocate(newcap, alignof(T));
			size_t i = 0;
			try {
			for(; i < size; ++i)
				{	
				buf2[i] = buf[i];
				}
			} catch(...)
			{
				for (size_t j = 0; j < i; ++j)
				{
					buf2[j].~T();
				}
				m_allocator.deallocate(buf2, newcap, alignof(T));
				throw;
			}
		for (size_t i = 0; i < size; ++i)
		{
			buf[i].~T();
		}
		m_allocator.deallocate(buf, capacity, alignof(T));
		buf = buf2;
		capacity = newcap;
		new (buf + size) T(std::forward<Args>(args)...);
		size += 1;
		} else {
		new (buf + size) T(std::forward<Args>(args)...);
		size += 1;
		}
	}
*/

	template<typename T, typename Allocator>
	void DataBuf<T, Allocator>::Emplace(const T& arg)
	{ 
#ifdef DEBUG
		std::cout << "Emplace by lvalue \n";
#endif
		if(size == capacity)
		{
			size_t newcap = capacity * 2;
			T* buf2 = m_allocator.allocate(newcap, alignof(T));
			std::uninitialized_copy_n(buf, size, buf2);
			//size_t i = 0;
			//for(; i < size; ++i)
			//{
			//buf2[i] = buf[i];
			//}

		for (size_t i = 0; i < size; ++i)
		{
			buf[i].~T();
		}
		m_allocator.deallocate(buf, capacity, alignof(T));
		buf = buf2;
		capacity = newcap;
		new (buf + size) T(arg);
		size += 1;
#ifdef DEBUG
		std::cout << buf + size << " now contain " << arg <<"\n";
#endif
		} else {
#ifdef DEBUG
		std::cout << buf + size << " now contain " << arg <<"\n";
#endif
		new (buf + size) T(arg);
		size += 1;
		}
	}



	template<typename T, typename Allocator>
	void DataBuf<T, Allocator>::Emplace(T&& arg)
	{ 
#ifdef DEBUG
		std::cout << "Emplace by rvalue \n";
#endif
		if(size == capacity)
		{
			size_t newcap = capacity * 2;
			T* buf2 = m_allocator.allocate(newcap, alignof(T));
			std::uninitialized_move_n(buf, size, buf2);
			//size_t i = 0;
			//for(; i < size; ++i)
			//{
			//buf2[i] = std::move(buf[i]);
			//}
			
		for (size_t i = 0; i < size; ++i)
		{
			buf[i].~T();
		}
		m_allocator.deallocate(buf, capacity, alignof(T));
		buf = buf2;
		capacity = newcap;
		new (buf + size) T(std::move(arg));
		size += 1;
#ifdef DEBUG
		std::cout << buf + size << " now contain " << arg << "\n";
#endif
		} else {
#ifdef DEBUG
		std::cout << buf + size << " now contain " << arg << "\n";
#endif
		new (buf + size) T(std::move(arg));
		size += 1;
		}
	}
