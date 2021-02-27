#include <chrono>
#include <thread>

#include "pch.h"
#include "CppUnitTest.h"
#include "ThreadPool.h"
#include "IJob.h"

using namespace std::chrono_literals;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestThreadPool
{
	class TestJob : public IJob
	{
		bool completeTestJobSign = true;
	private:
		std::chrono::milliseconds sleepTime;

	public:
		TestJob(std::chrono::milliseconds sleepMs) :sleepTime(sleepMs) {
		}

		/// <summary>
		/// Calculate how many primes are in the range between low to high
		/// </summary>
		virtual void calculate() {
			std::this_thread::sleep_for(1000ms);
			set_completed();

			result = &completeTestJobSign;
		}

	};

	TEST_CLASS(UnitTestThreadPool)
	{
	private:
		std::vector<shared_ptr<IJob>> create_jobs(int numOfJobs) {
			std::vector<shared_ptr<IJob>> jobs;
			for (int i = 0; i < numOfJobs; i++)
			{
				std::shared_ptr<TestJob> ptr = std::shared_ptr<TestJob>(new TestJob(200ms));
				jobs.push_back(ptr);
			}
			return jobs;
		}


	public:
		TEST_METHOD(ThreadPoolSanityTest)
		{
			ThreadPool pool(1);
			TestJob job1(50ms);
			TestJob job2(50ms);

			pool.add_job(&job1);
			pool.add_job(&job2);

			Assert::AreEqual(true, *((bool*)job1.get_result()));
			Assert::AreEqual(true, *((bool*)job2.get_result()));
		}


		TEST_METHOD(ThreadPoolParallelRunTest)
		{
			const int numOfJobs = 4;
			ThreadPool pool(1);

			auto start = std::chrono::high_resolution_clock::now();
			auto jobs = create_jobs(numOfJobs);
			std::for_each(jobs.begin(), jobs.end(), [&](shared_ptr<IJob> job) {pool.add_job(job.get()); });
			std::for_each(jobs.begin(), jobs.end(), [](shared_ptr<IJob> job) {job->wait_completion(); });
			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double, std::milli> elapsedOneThread = end - start;
			
			ThreadPool pool4(4);
			jobs = create_jobs(numOfJobs);
			start = std::chrono::high_resolution_clock::now();
			std::for_each(jobs.begin(), jobs.end(), [&](shared_ptr<IJob> job) {pool4.add_job(job.get()); });
			std::for_each(jobs.begin(), jobs.end(), [](shared_ptr<IJob> job) {job->wait_completion(); });
			end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double, std::milli> elapsedFourThread = end - start;
			Assert::IsTrue(std::abs(elapsedFourThread.count() * 4 - elapsedOneThread.count()) < 500.0);
		}
	};
}
