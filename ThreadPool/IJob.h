#pragma once

#include <mutex>
#include <condition_variable>

class IJob {

private:
	bool completed = false;
	std::mutex m;
	std::condition_variable cv;

protected:
	void* result = nullptr;

public:
	void* get_result() {
		wait_completion();
		return result;
	}

	bool is_completed() const {
		return completed;
	}

	void set_completed() {
		{
			std::lock_guard<std::mutex> locker(m);
			completed = true;
		}
		cv.notify_all();
	}

	void wait_completion() {
		std::unique_lock<std::mutex> locker(m);
		while (!completed) {
			cv.wait(locker);
		}
	}
	virtual void calculate() = 0;
};
