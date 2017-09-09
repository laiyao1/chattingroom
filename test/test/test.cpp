#include<iostream>
#include<string>
#include<thread>
#include<random>
using namespace std;

void print(const string& s) {
	// ���ַ����ĵ�һ���ַ���Ϊ�����������������
	default_random_engine dre(s[0]);
	uniform_int_distribution<> id(1, 100); // 1~100�ľ��ȷֲ�
	for (auto c : s) {
		// �ȴ����ʱ��
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