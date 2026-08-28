/**
 * Author: simon lindholm
 * Date: 2016-07-23
 * License: CC0
 * Source: me
 * Description: bump\_allocator for STL containers.
 * Usage: vector<vector<int, SmallAllocator<int>>> edges(n + 1);
 * Status: tested
 */
#pragma once

char buf[450 << 20] alignas(16);
size_t buf_ind = sizeof buf;

template<class T> struct SmallAllocator {
	typedef T value_type;
	SmallAllocator() {}
	template<class U> SmallAllocator(const U&) {}
	T* allocate(size_t n) {
		buf_ind -= n * sizeof(T);
		buf_ind &= 0 - alignof(T);
		return (T*)(buf + buf_ind);
	}
	void deallocate(T*, size_t) {}
};
