#include<iostream>
#include<cstdio>
#include<string>
#include<thread>
#include <boost/asio.hpp>
#include <boost/array.hpp>

using namespace std;
using namespace boost::asio;

string user_name;
string send_message;
unsigned int port_number;
bool message_flag;
size_t len;

void handler1(const boost::system::error_code& error, // Result of operation.
	std::size_t bytes_transferred           // Number of bytes read.
)
{
	len = bytes_transferred;
}

void input_data()
{
	while (1)
	{
		cout << "input data..." << endl;
		//getline(cin, send_message);
		//cin.getline(send_message);
		cin >> send_message;
		send_message = user_name + ":" + send_message;
		message_flag = true;
		while (message_flag == true);
		send_message = "";
	}
}

void set_up(unsigned int port_number)
{
	io_service ios;
	ip::address addr;
	string server_ip;
	cout << "please input the server ip:" << endl;
	cin >> server_ip;
	addr = addr.from_string(server_ip);

	cout << "connecting to server..." << endl;
	ip::tcp::endpoint ep(addr, port_number);

	ip::tcp::socket sock(ios);//建立socket

	sock.connect(ep);
	std::cout << "server: " << sock.remote_endpoint().address() << ":" << sock.remote_endpoint().port() << std::endl;

	vector<char> recv_message(128);
	/*
	while (1)
	{
		//第一个版本(同步通信)
		//该版本程序必须server & client双方发完信息才能互相看到收到的信息
		getline(cin, send_message);
		send_message = user_name + ":" + send_message;
		sock.write_some( boost::asio::buffer(send_message) );
		size_t len = sock.read_some( boost::asio::buffer(recv_message) );
		std::cout.write(recv_message.data(), len);
		cout << endl;
	}*/
	//异步读写
	thread t_input(input_data);
	t_input.detach();
	while (1)
	{
		if (message_flag == true)
		{
			sock.write_some(boost::asio::buffer(send_message));
			message_flag = false;
		}
		/*size_t len = sock.read_some(boost::asio::buffer(recv_message));
		if (len > 0)
		{
			std::cout.write(recv_message.data(), len);
			cout << endl;
		}*/
		for (auto i = recv_message.begin(); i != recv_message.end(); i++)
			*i = 0;
		sock.async_read_some(boost::asio::buffer(recv_message, 128), handler1);
		if (recv_message[0])
		{
			for (auto i = recv_message.begin(); i != recv_message.end() && *i; i++)
				cout << *i;
			//cout << recv_message.data();
			cout << endl;
		}
	}
}

int main()
{
	cout << "welcome to chatting room! You are a client port." << endl;

	cout << "please input your username:" << endl;
	cin >> user_name;

	while (port_number < 1024)
	{
		cout << "please input the port number:(the number must be the same as the server port number)" << endl;
		cin >> port_number;

	}
	set_up(port_number);

}