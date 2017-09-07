#include<iostream>
#include<cstdio>
#include<string>
#include <boost/asio.hpp>
#include <boost/array.hpp>

using namespace std;
using namespace boost::asio;

string user_name;
unsigned int port_number;

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

	ip::tcp::socket sock(ios);//½¨Á¢socket

	sock.connect(ep);
	std::cout << "server: " << sock.remote_endpoint().address() << ":" << sock.remote_endpoint().port() << std::endl;

	//sock.write_some(buffer("hello asio"));
	//while (1);
	//boost::array<char,128>
	string send_message;
	boost::array<char, 128> recv_message;

	while (1)
	{
		cin >> send_message;
		send_message = user_name + send_message;
		sock.write_some( boost::asio::buffer(send_message) );
		size_t len = sock.read_some( boost::asio::buffer(recv_message) );
		std::cout.write(recv_message.data(), len);
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