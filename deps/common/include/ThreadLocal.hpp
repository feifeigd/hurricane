#pragma once

#include <noncopyable.h>

template<typename T>
class ThreadLocal : private noncopyable
{
public:

	T* operator->()const {
		return &get();
	}
	T& operator*()const {
		return get();
	}

	ThreadLocal& operator=( const T& other) {
		get() = other;
		return *this;
	}

	T& get() const {
		// 每个线程一个实例	，线程退出会自动调用析构函数
		thread_local static T s_instance;
		return s_instance;
	}
};
