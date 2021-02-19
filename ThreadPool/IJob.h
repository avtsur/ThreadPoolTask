#pragma once
#define DllExport   __declspec( dllexport )

#include <mutex>
#include <condition_variable>

/// <summary>
/// base class for a job that can be proccessed by the thread pool
/// </summary>
class DllExport IJob {

private:
	bool completed = false;
	std::mutex m;
	std::condition_variable cv;

protected:
	void* result = nullptr;

	void set_completed() {
		{
			std::lock_guard<std::mutex> locker(m);
			completed = true;
		}
		cv.notify_all();
	}

public:
	/// <summary>
	/// Get job result, wait till jobs completes
	/// </summary>
	void* get_result() {
		wait_completion();
		return result;
	}
	
	/// <summary>
	/// return true if job calculation completed
	/// </summary>
	bool is_completed() const {
		return completed;
	}

	/// <summary>
	/// wait till job calculation completed
	/// </summary>
	void wait_completion() {
		std::unique_lock<std::mutex> locker(m);
		while (!completed) {
			cv.wait(locker);
		}
	}

	/// <summary>
	/// Starts the job
	/// </summary>
	virtual void calculate() = 0;
};
