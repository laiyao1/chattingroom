#include<iostream>
#include<cstdio>
#include<string>
#include <boost/asio.hpp>
#include <boost/array.hpp>
using namespace std;
using namespace boost::asio;

unsigned short port_number;
string user_name;

void set_up(unsigned short port_number)
{
	io_service ios;
	ip::address addr;
	addr = addr.from_string("127.0.0.1");
	cout << "server starting..." << endl;

	ip::tcp::acceptor acceptor(ios, ip::tcp::endpoint(addr, port_number));
	cout << acceptor.local_endpoint().address() << ":" << acceptor.local_endpoint().port() << endl;

	ip::tcp::socket sock(ios);//½¨Á¢socket
	acceptor.accept(sock);//accept

	cout << "connected to client successfully." << endl;
	std::cout << "client: " << sock.remote_endpoint().address() << ":" << sock.remote_endpoint().port() << std::endl;
	
	//sock.write_some(buffer("hello asio"));
	//while (1);

	string send_message;
	boost::array<char, 128> recv_message;

	while (1)
	{
		cin >> send_message;
		send_message = user_name + send_message;
		sock.write_some(boost::asio::buffer(send_message));
		size_t len = sock.read_some(boost::asio::buffer(recv_message));
		std::cout.write(recv_message.data(), len);
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