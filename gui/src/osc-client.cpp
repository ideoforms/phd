#include "osc-client.h"

#include <string>
#include <iostream>
#include <stdarg.h>

#include "oscpack/osc/OscOutboundPacketStream.h"

#define OUTPUT_BUFFER_SIZE 1024

using namespace std;

OSCClient::OSCClient(std::string host, unsigned port)
{
	this->mHost = host;
	this->mPort = port;

	this->mSocket = new UdpTransmitSocket(IpEndpointName(this->mHost.c_str(), this->mPort));
}

OSCClient::~OSCClient()
{
	delete this->mSocket;
}

void OSCClient::send(const string address, const string format, ...)
{
	char buffer[OUTPUT_BUFFER_SIZE];
	osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);

	va_list args;
	va_start (args, format);

	p << osc::BeginMessage(address.c_str());

	for (int i = 0; i < format.length(); i++)
	{
		char type = format.at(i);
		switch (type)
		{
			case 'f': p << (float) va_arg(args, double); break;
			case 'i': p << (int) va_arg(args, int); break;
		}
	}

	p << osc::EndMessage;

	this->mSocket->Send(p.Data(), p.Size());
}
