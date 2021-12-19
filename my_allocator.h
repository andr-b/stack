#pragma once
#include <memory_resource>
#include <iostream>
template<typename T>
class my_allocator
{
	std::pmr::memory_resource *mr;
	public:
		using value_type = T;
		using size_type = size_t;
		using propagate_on_container_move_assignment = std::true_type;
		my_allocator(std::pmr::memory_resource *m = std::pmr::new_delete_resource()) : mr(m) {}
		template <typename U>
		my_allocator(const my_allocator<U>&) noexcept {}
		T* allocate(size_t n)
		{
			return static_cast<T*>(mr->allocate(n*sizeof(T)));
		}
		void deallocate(T* p, size_t bytes)
		{
			mr->deallocate(static_cast<void*>(p), bytes*sizeof(T));
		}
};
template <typename T, typename U>
bool operator ==(const my_allocator<T>&, const my_allocator<U>&)
{
	return std::is_same<T, U>::value;
}

template <typename T, typename U>
bool operator !=(const my_allocator<T>& x, const my_allocator<U>& y)
{
	return !(x==y);
}
