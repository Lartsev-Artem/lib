#include "timeout_process.h"
int main()
{
	auto proc0{ []() { printf("proc 0\n"); } };
	auto proc1{ []() { printf("proc 1\n"); throw  timeout_proc::TimeOutExсeption("Test exception"); } };
	auto proc2{ []() { printf("proc 2\n"); } };

	timeout_proc::TimeOutProcess<3>* t = new timeout_proc::TimeOutProcess<3>;

	t->start_process(0, 100, proc0, timeout_proc::e_timeout_unlimit);
	t->start_process(1, 250, proc1, timeout_proc::e_timeout_unlimit);
	t->start_process(2, 0, proc2, timeout_proc::e_timeout_oneshot);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	delete t;

	return 0;
}