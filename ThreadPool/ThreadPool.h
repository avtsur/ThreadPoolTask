#pragma once

#include "BlockingQueue.h"
#include "IJob.h"
using namespace std;

class ThreadPool
{
private:
	bool jobsCompleted = false;
	vector<std::thread> threads;
	BlockingQueue<IJob*> jobs;
	void consume();

public:
	ThreadPool(int numOfThreads);
	
	void add_job(IJob* job);
	
	~ThreadPool();
};