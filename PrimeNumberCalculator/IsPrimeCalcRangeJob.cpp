#include "IsPrimeCalcRangeJob.h"

/// <summary>
/// Calculate how many primes are in the range between low to high
/// </summary>
void IsPrimeCalcRangeJob::calculate() {
	for (int i = low; i <= high; i++)
	{
		if (is_prime(i)) {
			numOfPrimes++;
		}
	}
	result = &numOfPrimes;
	set_completed();
}

/// <summary>
/// Check if given number is prime
/// </summary>
/// <param name="number">integer</param>
/// <returns>true if "number" is prime</returns>
bool IsPrimeCalcRangeJob::is_prime(int number) {
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