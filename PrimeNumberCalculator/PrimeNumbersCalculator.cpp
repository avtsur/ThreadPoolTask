// PrimeNumberCalculator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "BlockingQueue.h"
#include "IJob.h"
#include "ThreadPool.h"
#include "IsPrimeCalcRangeJob.h"

using namespace std;

const int expected_params = 4;
const int threads_param_index = 1;
const int lower_limit_param_index = 2;
const int higher_limit_param_index = 3;

const int rangeSingleJob = 300; //one calc job range of integers


int CalcNumOfPrimeInRange(int numOfThreads, int low, int high)
{
	ThreadPool threadPool(numOfThreads);
	int vectorSize = ((high - low + 1) / rangeSingleJob) + ((high - low + 1) % rangeSingleJob != 0 ? 1 : 0);
	vector<shared_ptr<IJob>> primeJobs(vectorSize);
	int num_of_primes = 0;
	auto start = std::chrono::high_resolution_clock::now();

	int i = low;

	for (; i <= high; i += rangeSingleJob)
	{
		int current_range_high = std::min(i + rangeSingleJob - 1, high);
		auto job_ptr = std::make_shared<IsPrimeCalcRangeJob>(i, current_range_high);
		threadPool.add_job(job_ptr.get());
		primeJobs[(i - low) / rangeSingleJob] = job_ptr;
	}
	if (i < high) {
		auto job_ptr = std::make_shared<IsPrimeCalcRangeJob>(i, high);
		threadPool.add_job(job_ptr.get());
		primeJobs[(i - low) / rangeSingleJob] = job_ptr;
	}

	//get results
	for (auto& jobToExecute : primeJobs)
	{
		auto res = (int*)(jobToExecute->get_result());
		num_of_primes += (*res);
	}

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

	std::cout << "Calc primes duration:" << duration << "ms" << endl;
	return num_of_primes;
}


int main(int argc, char* argv[])
{
	int num_of_threads = thread::hardware_concurrency();
	int range_low = 1;
	int range_high = 10000;
	if (argc == expected_params) {
		num_of_threads = atoi(argv[threads_param_index]);
		range_low = atoi(argv[lower_limit_param_index]);
		range_high = atoi(argv[higher_limit_param_index]);
	}
	cout << "Beginning calculation of primes in range:" << range_low << "->" << range_high << ", threads to use:" << num_of_threads << endl;

	int res = CalcNumOfPrimeInRange(num_of_threads, range_low, range_high);

	cout << "Primes in range:" << res << endl;
}

