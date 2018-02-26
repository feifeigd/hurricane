#pragma once

#include <noncopyable.h>
#include <cassert>
#include <mutex>

#ifdef WIN32

#include <windows.h>
#endif

template<typename T>
class ThreadLocal : private noncopyable
{
public:
	ThreadLocal() {
		
	}
	//~ThreadLocal();

	T* operator->()const {
		return get();
	}
	T& operator*()const {
		return *get();
	}

	ThreadLocal& operator=( const T& other) {
		*get() = other;
		return *this;
	}
	T* get() const {
		T* ret = &s_instance;
		return ret;
	}
private:
	/// 每个线程一个实例	
	static __declspec(thread) T s_instance;
};

template<typename T>
T ThreadLocal<T>::s_instance;
