#pragma once

#include <functional>
#include <atomic>
#include <mutex>
#include <condition_variable>

#include "Common/Thread/ThreadManager.h"

// Kind of like a semaphore I guess.
struct WaitableCounter {
public:
	WaitableCounter(int maxValue) : maxValue_(maxValue) {}

	void Count() {
		if (count_.fetch_add(1) == maxValue_ - 1) {
			// We were the last one to increment
			cond_.notify_one();
		}
	}

	void Wait() {
		std::unique_lock<std::mutex> lock(mutex_);
		while (count_.load() != maxValue_) {
			cond_.wait(lock);
		}
	}

	void WaitAndRelease() {
		Wait();
		delete this;
	}

	int maxValue_;
	std::atomic<int> count_;
	std::mutex mutex_;
	std::condition_variable cond_;
};

// Note that upper bounds are non-inclusive.
// This one never executes the remainer on the calling thread.
WaitableCounter *ParallelRangeLoopWaitable(ThreadManager *threadMan, const std::function<void(int, int)> &loop, int lower, int upper, int minSize);

// This one optimizes by running the remainder on the calling thread.
void ParallelRangeLoop(ThreadManager *threadMan, const std::function<void(int, int)> &loop, int lower, int upper, int minSize = -1);

// Common utilities for large (!) memory copies.
// Will only fall back to threads if it seems to make sense.

void ParallelMemcpy(ThreadManager *threadMan, void *dst, const void *src, size_t bytes);
void ParallelMemset(ThreadManager *threadMan, void *dst, uint8_t value, size_t bytes);
