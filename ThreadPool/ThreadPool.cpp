#include "pch.h"
#include "ThreadPool.h"


ThreadPool::ThreadPool(int numOfThreads)
{
	if (numOfThreads <= 0)
	{
		throw std::invalid_argument("num of threads must be positive integer");
	}
	for (int i = 0; i < numOfThreads; i++)
	{
		HANDLE threadHandle = CreateThread(NULL,
			0,
			consumeWinAPI,
			this,
			0,
			NULL);
		if (!threadHandle)
		{
			throw exception("Unable to create thread...");
		}
		threadsHandles.push_back(threadHandle);
	}
}

void ThreadPool::add_job(IJob* job)
{
	jobs.enqueue(job);
}

void ThreadPool::consume()
{
	while (true)
	{
		if (jobsCompleted) {
			break;
		}
		auto job = jobs.dequeue();
		if (!jobsCompleted) {
			job->calculate();
		}
	}
}

DWORD WINAPI ThreadPool::consumeWinAPI(LPVOID lpParameter)
{
	auto threadPool = static_cast<ThreadPool*>(lpParameter);
	threadPool->consume();
	return 0;
}

ThreadPool::~ThreadPool()
{
	std::for_each(threadsHandles.begin(), threadsHandles.end(), [](HANDLE h) {  CloseHandle(h); });
}
