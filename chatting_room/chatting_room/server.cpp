//有些情况下末位字符丢失，需修改读入的次数，改为多次连续读入，直到收不到字符
#include <boost/asio.hpp>
#include <boost/array.hpp>

#include<iostream>
#include<cstdio>
#include<string>

#include <thread>
using namespace std;
using namespace boost::asio;

class session
{
public:
	string user_name;
	string send_message;
	unsigned short port_number;
	size_t len;
	io_service ios;
	ip::address addr;
	ip::tcp::socket *sock_ptr;

	session(string user_name, unsigned short port_number):
		user_name(user_name), port_number(port_number)
	{
		string server_ip;
		//cout << "please input the server ip:" << endl;
		//cin >> server_ip;

		addr = addr.from_string("127.0.0.1");
		cout << "server starting..." << endl;

		ip::tcp::acceptor acceptor(ios, ip::tcp::endpoint(addr, port_number));
		cout << acceptor.local_endpoint().address() << ":" << acceptor.local_endpoint().port() << endl;

		sock_ptr = new ip::tcp::socket(ios);//建立socket//建立socket
		acceptor.accept(*sock_ptr);//accept

		cout << "connected to client successfully." << endl;
		std::cout << "client: " << sock_ptr->remote_endpoint().address() << ":" << sock_ptr->remote_endpoint().port() << std::endl;
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
		//同步读写
		/*
		while (1)
		{
			getline(cin, send_message);
			//cin >> send_message;
			send_message = user_name + ":" + send_message;
			sock.write_some(boost::asio::buffer(send_message));
			size_t len = sock.read_some(boost::asio::buffer(recv_message));
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
				sock.write_some(boost::asio::buffer(send_message));
				message_flag = false;
			}

			for (auto i = recv_message.begin(); i != recv_message.end(); i++)
				*i = 0;

			boost::asio::async_read(sock, boost::asio::buffer(recv_message, 128), handler);
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
	cout<<"welcome to chatting room!"<<endl;
	
	//cout<<"please input your username:"<<endl;
	//cin >> user_name;
	string user_name;
	user_name = "laiyao1";
	cout << "user name has been set to laiyao1" << endl;
	unsigned short port_number;
	port_number = 9876;
	while (port_number < 1024)
	{
		//cout<<"please input the port number"<<endl;
		//scanf_s("%hd", &port_number);
		port_number = 9876;
		cout << "port number has been set to 9876" << endl;
	}
	session se(user_name, port_number);
	se.set_up();
	return 0;
}