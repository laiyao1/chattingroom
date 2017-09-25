//本server程序的目标是创建多个session 线程, 每一个session单独与一个对应的client 对话
//既有接收数据 也有往所有的session中写数据
//使用C11 thread
#include <boost/asio.hpp>
#include <boost/array.hpp>

#include<iostream>
#include<cstdio>
#include<string>

#include <thread>
using namespace std;
using namespace boost::asio;

//线程共享资源
int session_sum = 0;
class session;

vector<session *> se_ptr;
vector<thread *> th_ptr;

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

	session( unsigned short port_number) :
		 port_number(port_number)
	{
		string server_ip;
		//cout << "please input the server ip:" << endl;
		//cin >> server_ip;

		addr = addr.from_string("127.0.0.1");
		//cout << "server starting..." << endl;

		ip::tcp::acceptor acceptor(ios, ip::tcp::endpoint(addr, port_number));
		cout << acceptor.local_endpoint().address() << ":" << acceptor.local_endpoint().port() << endl;

		sock_ptr = new ip::tcp::socket(ios);//建立socket
		acceptor.accept(*sock_ptr);//accept

		cout << "connected to client successfully." << endl;
		std::cout << "client: " << sock_ptr->remote_endpoint().address() << ":" << sock_ptr->remote_endpoint().port() << std::endl;

		//读入用户名
		vector<char> recv_user_name(128);
		size_t len = sock_ptr->read_some(boost::asio::buffer(recv_user_name));
		user_name = recv_user_name.data();
		cout << "the new user's name is" << user_name << endl;
	}

	/* static void input_data(class session* ptr)
	{
		while (1)
		{
			//cout << "input data..." << endl;
			cin >> ptr->send_message;
			ptr->send_message = ptr->user_name + ":" + ptr->send_message;
			ptr->sock_ptr->write_some(boost::asio::buffer(ptr->send_message));
		}
	}*/
	

};

void message_ready(string message_str)
{
	for (auto it = se_ptr.begin(); it != se_ptr.end(); it++)
	{
		(*it)->sock_ptr->write_some(boost::asio::buffer(message_str));
	}
}

void set_up(session *se_ptr)
{
	

	//异步读写
	//thread t_input(input_data, this);
	//t_input.detach();
	while (1)
	{
		vector<char> recv_message(128);
		size_t len = se_ptr->sock_ptr->read_some(boost::asio::buffer(recv_message));
		if (len > 0)
		{
			string new_message = recv_message.data();//se_ptr->user_name + ":" + recv_message.data() + "\n";
			message_ready(new_message);
			//std::cout.write(recv_message.data(), len);
			//cout << endl;
		}
	}
}


int main()
{
	cout << "welcome to chatting room!" << endl;

	//cout<<"please input your username:"<<endl;
	//cin >> user_name;
	string user_name;
	//user_name = "laiyao1";
	//cout << "user name has been set to laiyao1" << endl;
	unsigned short port_number;
	port_number = 9876;
	while (port_number < 1024)
	{
		//cout<<"please input the port number"<<endl;
		//scanf_s("%hd", &port_number);
		port_number = 9876;
		cout << "port number has been set to 9876" << endl;
	}
	

	while (1)
	{
		se_ptr.push_back(new session(port_number));
		session_sum++;
		//session *se(port_number);
		th_ptr.push_back(new thread(set_up,se_ptr[ session_sum - 1 ]));
		th_ptr[session_sum - 1]->detach();
	}
	//session se(user_name, port_number);
	//se.set_up();
	return 0;
}