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
		T* allocate(size_t n, size_t alignment = alignof(std::max_align_t))
		{
#ifdef DEBUG
			std::cout << "Custom allocated " << n*sizeof(T) << " bytes\n";
#endif
			return static_cast<T*>(mr->allocate(n*sizeof(T), alignment));
		}
		void deallocate(T* p, size_t num, size_t alignment = alignof(std::max_align_t))
		{
			mr->deallocate(static_cast<void*>(p), num*sizeof(T), alignment);
#ifdef DEBUG
			std::cout << "Custom deallocated " << num*sizeof(T) << " bytes\n";
#endif
		}
		my_allocator(const my_allocator<T>& other) = default;
		my_allocator(my_allocator<T>&& other) = default;
		my_allocator& operator=(const my_allocator<T>& other) = default;
		my_allocator& operator=(my_allocator<T>& other) = default;
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
