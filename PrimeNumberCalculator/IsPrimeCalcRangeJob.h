#pragma once
#include "IJob.h"

/// <summary>
/// This job is used to calc the number of primes in a range of integers
/// </summary>
class IsPrimeCalcRangeJob : public IJob
{
private:
	int low;
	int high;
	int numOfPrimes = 0;
	
	/// <summary>
	/// Check if given number is prime
	/// </summary>
	/// <param name="number">integer</param>
	/// <returns>true if "number" is prime</returns>
	bool is_prime(int number);

public:
	IsPrimeCalcRangeJob(int low, int high) : low(low), high(high) {
	}

	/// <summary>
	/// Calculate how many primes are in the range between low to high
	/// </summary>
	virtual void calculate();


};