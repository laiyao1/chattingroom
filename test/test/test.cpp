#include<iostream>
#include<string>
#include<thread>
#include<random>
using namespace std;

void print(const string& s) {
	// 用字符串的第一个字符作为随机数发生器的种子
	default_random_engine dre(s[0]);
	uniform_int_distribution<> id(1, 100); // 1~100的均匀分布
	for (auto c : s) {
		// 等待随机时间
		this_thread::sleep_for(chrono::milliseconds{ id(dre) });
		cout.put(c).flush();
	}
}
int main() {
	thread t1{ print, "Hello " };
	thread t2{ print, "Parallel World!\n" };
	t1.join();
	t2.join();
}