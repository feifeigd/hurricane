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
		// ÿ���߳�һ��ʵ��	���߳��˳����Զ�������������
		thread_local static T s_instance;
		return s_instance;
	}
};
