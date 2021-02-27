#include <chrono>
#include <thread>

#include "pch.h"
#include "CppUnitTest.h"
#include "BlockingQueue.h"

using namespace std::chrono_literals;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestThreadPool
{
	TEST_CLASS(UnitTestBlockingQueue)
	{
	private:
		static void DefferedEnqueue(BlockingQueue<int>* queue)
		{
			std::this_thread::sleep_for(1000ms);
			queue->enqueue(1);
		}

	public:

		TEST_METHOD(BlockingQueueSanityTest)
		{
			BlockingQueue<int> queue;
			queue.enqueue(1);
			queue.enqueue(2);
			Assert::AreEqual(1, queue.dequeue());
			Assert::AreEqual(2, queue.dequeue());
		}

		TEST_METHOD(BlockingQueueDequeueTest)
		{
			BlockingQueue<int> queue;

			std::thread deferredEnqueueThread(DefferedEnqueue, &queue);

			auto start = std::chrono::high_resolution_clock::now();
			auto number = queue.dequeue();
			auto end = std::chrono::high_resolution_clock::now();

			std::chrono::duration<double, std::milli> elapsed = end - start;
			//make sure that we have waited till queue is not empty
			Assert::IsTrue(elapsed.count() - 500 > 0.0);
			Assert::AreEqual(1, number);
			deferredEnqueueThread.join();
		}
	};
}
