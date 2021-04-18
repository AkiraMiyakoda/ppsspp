#pragma once

#include <mutex>

template<class T>
class RustMutexProxy {
public:
	RustMutexProxy(T *data, std::mutex &mutex) : data_(data), mutex_(mutex) {}
	~RustMutexProxy() { mutex_.unlock(); }
	T &operator*() { return *data_; }
	T *operator->() { return data_; }
private:
	T *data_;
	std::mutex &mutex_;
};

template<class T>
class RustMutex {
public:
	RustMutex(T && t) : t_(t) {}

	RustMutexProxy<T> lock() {
		mutex_.lock();
		return RustMutexProxy<T>(&t_, mutex_);
	}

private:
	T t_;
	std::mutex mutex_;
};

// Note that name must be a global string that lives until the end of the process,
// for assertThreadName to work.
void setCurrentThreadName(const char *threadName);
void AssertCurrentThreadName(const char *threadName);
