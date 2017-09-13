//有些情况下末位字符丢失，需修改读入的次数，改为多次连续读入，直到收不到字符

#include<iostream>
#include<cstdio>
#include<string>
#include<thread>
#include <boost/asio.hpp>
#include <boost/array.hpp>

using namespace std;
using namespace boost::asio;

class session {

public:
	string user_name;
	string send_message;
	unsigned int port_number;
	size_t len;
	io_service ios;
	ip::address addr;
	ip::tcp::socket *sock_ptr;

	session(string user_name,unsigned short port_number):
		user_name(user_name),port_number(port_number)
	{
		string server_ip;
		//cout << "please input the server ip:" << endl;
		//cin >> server_ip;

		server_ip = "127.0.0.1";
		cout << "server ip has been set to 127.0.0.1" << endl;

		addr = addr.from_string(server_ip);

		cout << "connecting to server..." << endl;
		ip::tcp::endpoint ep(addr, port_number);

		sock_ptr = new ip::tcp::socket(ios);//建立socket

		sock_ptr->connect(ep);
		std::cout << "server: " << (sock_ptr->remote_endpoint()).address() << ":" << (sock_ptr->remote_endpoint()).port() << std::endl;
	}

	static void input_data(class session* ptr)
	{
		while (1)
		{
			//cout << "input data..." << endl;
			cin >> ptr->send_message;
			ptr->send_message = ptr->user_name + ":" + ptr->send_message;
			ptr->sock_ptr->write_some(boost::asio::buffer(ptr->send_message));
		}
	}

	void set_up()
	{
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
		thread t_input(input_data, this);
		t_input.detach();

		while (1)
		{
			size_t len = sock_ptr->read_some(boost::asio::buffer(recv_message));
			if (len > 0)
			{
				std::cout.write(recv_message.data(), len);
				cout << endl;
			}
		}
		/*
		while (1)
		{
			if (message_flag == true)
			{

				cout<<sock.write_some(boost::asio::buffer(send_message))<<endl;
				message_flag = false;
			}
			size_t len = sock.read_some(boost::asio::buffer(recv_message));
			if (len > 0)
			{
				std::cout.write(recv_message.data(), len);
				cout << endl;
			}
			for (auto i = recv_message.begin(); i != recv_message.end(); i++)
				*i = 0;
			boost::asio::async_read(sock, boost::asio::buffer(recv_message, 10), handler);
			if (recv_message[0])
			{
				for (auto i = recv_message.begin(); i != recv_message.end() && *i; i++)
						cout << *i;
				cout << endl;
			}
		}*/
	}
};

int main()
{
	cout << "welcome to chatting room! You are a client port." << endl;
	cout << "please input your username:" << endl;
	//cin >> user_name;
	string user_name;
	user_name = "laiyao2";
	cout << "user name has been set to laiyao2" << endl;
	unsigned short port_number;
	port_number = 9876;
	while (port_number < 1024)
	{
		//cout << "please input the port number:(the number must be the same as the server port number)" << endl;
		//cin >> port_number;
		port_number = 9876;
		cout << "port number has been set to 9876" << endl;
	}
	session se(user_name,port_number);
	se.set_up();
}