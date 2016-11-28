#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

void func(int i, mutex * mtx)
{
	mtx->lock();
	cout << "Hello Thread[" << i << "]." << endl;
	mtx->unlock();
}

int main(int argc, char ** argv)
{
	mutex mtx;
	thread th0(func, 0, &mtx);
	thread th1(func, 1, &mtx);

	th1.join();
	th0.join();
	return 0;
}