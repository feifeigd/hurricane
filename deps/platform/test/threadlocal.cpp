#include <ThreadLocal.hpp>
#include <cstdio>
#include <thread>
#include <vector>

class A {
public:
	A() : value(rand()){
		printf("A address=%p, value=%d\n", this, value);
	}
	~A()
	{
		printf("~A address=%p, value=%d\n", this, value);
	}
	int value;
};
ThreadLocal<A> g_start_time;

void ThreadProc()
{
	//ģ���̵߳Ĺ������� 
	int i = 1000 * 1000 * 100;
	while (i--)
	{
	}
	printf("Thread ID:%-5d, value:%p,ptr=%p\n", std::this_thread::get_id(), &g_start_time->value, g_start_time.get());
}

int main(){

	std::vector<std::thread> threads;

	// ����ʮ���̣߳�����ÿ���߳����е�ʱ��.
	for (int i = 0; i < 5; i++)
	{
		threads.push_back(std::thread(ThreadProc));
	}

	for (int i = 0; i < 5; i++)
	{
		threads[i].join();
	}
	return 0;
}
