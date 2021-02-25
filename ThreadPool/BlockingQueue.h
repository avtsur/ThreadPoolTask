#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

/// <summary>
/// Generic Blocking Queue
/// </summary>
template <typename T>
class BlockingQueue {
private:
	bool adding_completed = false;
	std::queue<T> items;
	std::mutex m;
	std::condition_variable cv;

public:

	/// <summary>
	/// Enqueue element into queue, notify other waiting threads that new element is added.
	/// </summary>
	void enqueue(const T& item)
	{
	
		{
			std::lock_guard<std::mutex> locker(m);
			if (adding_completed)
			{
				return;
			}
			items.push(item);
		}
		cv.notify_all();
	}

	/// <summary>
	/// return first in element, if queue is empty waits till element is enqueued
	/// </summary>
	T dequeue()
	{
		std::unique_lock<std::mutex> locker(m);
		while (items.empty() && !adding_completed) {
			cv.wait(locker);
		}
		T item = nullptr;
		if (!items.empty()) {
			item = items.front();
			items.pop();
		}
		return std::move(item);
	}
	
	/// <summary>
	/// raise adding completed and release waiting threads
	/// </summary>
	void release_waiting_threads()
	{
		{
			std::lock_guard<std::mutex> locker(m);
			adding_completed = true;
		}
		cv.notify_all();
	}

	/// <summary>
	/// return number of elements in the queue
	/// </summary>
	int get_items_number() const {
		std::lock_guard<std::mutex> locker(m);
		return items.size();
	}
};

