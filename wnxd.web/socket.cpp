#include "socket.h"

using namespace wnxd::Socket;
using namespace System::Threading;
using namespace System::Text;
using namespace System::IO;
//class Socket
//private
bool ClientSocket::Connect()
{
	if (!this->_isconnect)
	{
		if (this->_ip != nullptr)
		{
			if (this->_socket == nullptr)
			{
				this->_socket = gcnew ::Socket(AddressFamily::InterNetwork, SocketType::Stream, ProtocolType::Tcp);
			}
			try
			{
				this->_socket->Connect(this->_ip);
				this->_stream = gcnew NetworkStream(this->_socket);
				this->_stream->ReadTimeout = 10000;
				this->_stream->WriteTimeout = 10000;
				this->_isconnect = true;
			}
			catch (...)
			{

			}
		}
	}
	return this->_isconnect;
}
//public
ClientSocket::ClientSocket(IPAddress^ ip, int port)
{
	this->_isconnect = false;
	this->_ip = gcnew IPEndPoint(ip, port);
}
String^ ClientSocket::Send(String^ data)
{
	//while (this->_issend) Thread::Sleep(20);
	this->_issend = true;
	try
	{
		if (this->Connect())
		{
			array<Byte>^ d = Encoding::UTF8->GetBytes(data);
			this->_stream->Write(BitConverter::GetBytes(d->Length), 0, 4);
			this->_stream->Write(d, 0, d->Length);
		}
		this->_issend = false;
		if (this->_isconnect)
		{
			array<Byte>^ d = gcnew array<Byte>(4);
			this->_stream->Read(d, 0, 4);
			int len = BitConverter::ToInt32(d, 0);
			d = gcnew array<Byte>(len);
			this->_stream->Read(d, 0, len);
			String^ r = Encoding::UTF8->GetString(d);
			return r;
		}
	}
	catch (...)
	{
		this->_isconnect = false;
		this->_issend = false;
		delete this->_stream;
		delete this->_socket;
		this->_socket = nullptr;
	}
	return nullptr;
}