#include <stdarg.h>
#include <string>

#include "oscpack/ip/UdpSocket.h"

using namespace std;

class OSCClient
{
public:
	OSCClient(const string host, unsigned port);
	~OSCClient();

	void send(const string address, const string format, ...);

	std::string mHost;
	unsigned mPort;
	UdpTransmitSocket *mSocket;
};
