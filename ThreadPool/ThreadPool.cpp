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
		std::thread t(&ThreadPool::consume, this);
		threads.push_back(std::move(t));
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

ThreadPool::~ThreadPool()
{
	jobsCompleted = true;
	jobs.release_waiting_threads();
	std::for_each(threads.begin(), threads.end(), [](thread& t) { t.join(); });
}
