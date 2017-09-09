#include<iostream>
#include<cstdio>
#include<string>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <thread>
using namespace std;
using namespace boost::asio;

unsigned short port_number;
string user_name;

string send_message;
bool message_flag;
int len;
void input_data()
{
	while (1)
	{
		cout << "input data..." << endl;
		//getline(cin, send_message);
		cin >> send_message;
		send_message = user_name + ":" + send_message;
		message_flag = true;
		while (message_flag == true);
		send_message = "";
	}
}
void handler1(const boost::system::error_code& error, // Result of operation.
	std::size_t bytes_transferred           // Number of bytes read.
)
{
	size_t len = bytes_transferred;
}

void set_up(unsigned short port_number)
{
	io_service ios;
	ip::address addr;
	addr = addr.from_string("127.0.0.1");
	cout << "server starting..." << endl;

	ip::tcp::acceptor acceptor(ios, ip::tcp::endpoint(addr, port_number));
	cout << acceptor.local_endpoint().address() << ":" << acceptor.local_endpoint().port() << endl;

	ip::tcp::socket sock(ios);//建立socket
	acceptor.accept(sock);//accept

	cout << "connected to client successfully." << endl;
	std::cout << "client: " << sock.remote_endpoint().address() << ":" << sock.remote_endpoint().port() << std::endl;

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
	thread t_input( input_data );
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
		sock.async_read_some(boost::asio::buffer(recv_message,128), handler1);
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
	cout<<"welcome to chatting room!"<<endl;
	
	cout<<"please input your username:"<<endl;
	cin >> user_name;
	
	while (port_number < 1024)
	{
		cout<<"please input the port number"<<endl;
		scanf_s("%hd", &port_number);
	}
	set_up(port_number);

	return 0;
}