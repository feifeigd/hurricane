#include <ThreadLocal.hpp>
#include <cstdio>
#include <thread>
#include <vector>

class A {
public:
	A() : value(rand()){
		printf("A address=%p, value=%d, ThreadId=%d\n", this, value, std::this_thread::get_id());
	}
	~A()
	{
		printf("~A address=%p, value=%d, ThreadId=%d\n", this, value, std::this_thread::get_id());
	}
	int value;
};

//ThreadLocal<A> g_start_time;
//thread_local A a;

void ThreadProc()
{
	static ThreadLocal<A> g_start_time;
	//static thread_local A a;
	printf("Thread ID:%-5d, ptr:%p,%p\n", std::this_thread::get_id(), &g_start_time.get(), &g_start_time.get());
	//模拟线程的工作过程 
	int i = 1000 * 1000 * 100;
	while (i--)
	{
	}
}

int main(){
	printf("Main Thread ID:%-5d\n", std::this_thread::get_id());
	std::vector<std::thread> threads;

	// 开启十个线程，计算每个线程运行的时间.
	for (int i = 0; i < 2; i++)
	{
		threads.push_back(std::thread(ThreadProc));
	}

	for (int i = 0; i < 2; i++)
	{
		threads[i].join();
	}
	return 0;
}

