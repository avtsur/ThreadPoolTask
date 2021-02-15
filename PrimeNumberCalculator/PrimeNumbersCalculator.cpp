// PrimeNumberCalculator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "BlockingQueue.h"
#include "IJob.h"
#include "ThreadPool.h"

using namespace std;

const int expected_params = 4;
const int threads_param_index = 1;
const int lower_limit_param_index = 2;
const int higher_limit_param_index = 3;

class IsPrimeCalcRangeJob : public IJob {
	int low;
	int high;
	int numOfPrimes = 0;

public:
	IsPrimeCalcRangeJob(int low, int high) : low(low), high(high) {
	}

	virtual void calculate() {
		for (int i = low; i <= high; i++)
		{
			if (is_prime(i)) {
				numOfPrimes++;
			}
		}
		result = &numOfPrimes;
		set_completed();
	}

	bool is_prime(int number) {
		bool is_prime = true;
		if (number == 0 || number == 1) {
			is_prime = false;
		}
		else {
			for (int i = 2; i * i <= number; i++)
			{
				if (number % i == 0) {
					is_prime = false;
					break;
				}
			}
		}
		return is_prime;
	}
};

int CalcNumOfPrimeInRange(int numOfThreads, int low, int high)
{
	ThreadPool threadPool(numOfThreads);
	vector<shared_ptr<IJob>> primeJobs;
	int num_of_primes = 0;
	auto start = std::chrono::high_resolution_clock::now();

	int i = low;
	const int range = 300; //one calc job range of integers
	for (; i <= high; i += range)
	{
		int current_range_high = i + range - 1;
		std::shared_ptr<IJob> job_ptr =
			std::make_shared<IsPrimeCalcRangeJob>(i, current_range_high > high ? high : current_range_high);
		primeJobs.push_back(job_ptr);
		threadPool.add_job(job_ptr.get());
	}
	if (i < high) {
		std::shared_ptr<IJob> job_ptr = std::make_shared<IsPrimeCalcRangeJob>(i, high);
		primeJobs.push_back(job_ptr);
		threadPool.add_job(job_ptr.get());
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
	int  range_high = 10000;
	if (argc == expected_params) {
		num_of_threads = atoi(argv[threads_param_index]);
		range_low = atoi(argv[lower_limit_param_index]);
		range_high = atoi(argv[higher_limit_param_index]);
	}
	cout << "Beginning calculation of primes in range:" << range_low << "->" << range_high << ", threads to use:" << num_of_threads << endl;

	int res = CalcNumOfPrimeInRange(num_of_threads, range_low, range_high);

	cout << "Primes in range:" << res << endl;
}

