#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class BlockingQueue {
private:
	bool adding_completed = false;
	std::queue<T> items;
	std::mutex m;
	std::condition_variable cv;

public:
	void enqueue(const T& item)
	{
		if (adding_completed)
		{
			return;
		}
		{
			std::lock_guard<std::mutex> locker(m);
			items.push(item);
		}
		cv.notify_all();
	}

	T& dequeue()
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
		return item;
	}

	void release_waiting_threads()
	{
		{
			std::lock_guard<std::mutex> locker(m);
			adding_completed = true;
		}
		cv.notify_all();
	}

	int get_items_number() const {
		std::lock_guard<std::mutex> locker(m);
		return items.size();
	}
};

