#pragma once
#include <windows.h>

#include "BlockingQueue.h"
#include "IJob.h"
using namespace std;

/// <summary>
/// Thread pool for executing calculation jobs.
/// </summary>
class ThreadPool
{
private:
	bool jobsCompleted = false;
	/// <summary>
	/// all thread pool handles
	/// </summary>
	vector<HANDLE> threadsHandles;

	BlockingQueue<IJob*> jobs;
	/// <summary>
	/// consume work
	/// </summary>
	void consume();

	/// <summary>
	/// consume work
	/// </summary>
	static DWORD WINAPI  consumeWinAPI(LPVOID lpParameter);

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