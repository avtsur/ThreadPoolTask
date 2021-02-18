#pragma once

#include "BlockingQueue.h"
#include "IJob.h"
using namespace std;

/// <summary>
/// Thread pool fo executing calculation jobs.
/// </summary>
class ThreadPool
{
private:
	bool jobsCompleted = false;
	vector<std::thread> threads;
	BlockingQueue<IJob*> jobs;
	void consume();

public:
	/// <summary>
	/// Creates thread pool
	/// </summary>
	/// <param name="numOfThreads">thread pool size</param>
	ThreadPool(int numOfThreads);
	
	/// <summary>
	/// Add job to be processed by on of the thread pool threads
	/// </summary>
	/// <param name="job">job to execute</param>
	void add_job(IJob* job);
	
	~ThreadPool();
};