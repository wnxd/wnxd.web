using namespace System;
using namespace System::Net;
using namespace System::Net::Sockets;
using namespace System::Collections::Generic;

namespace wnxd
{
	namespace Socket
	{
		ref class ClientSocket
		{
		private:
			System::Net::Sockets::Socket^ _socket;
			NetworkStream^ _stream;
			IPEndPoint^ _ip;
			bool _isconnect, _issend;
			bool Connect();
		public:
			ClientSocket(IPAddress^ ip, int port);
			String^ Send(String^ data);
		};
	}
}