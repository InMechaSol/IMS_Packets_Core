#include "IMS_PacketsAPI_Core.h"
using namespace IMSPacketsAPICore;

#define USING_DEFAULTS_STREAM_SERIALIZATION


#ifndef USING_DEFAULTS_STREAM_SERIALIZATION
// define interface functions (*link the stream)
class TestASCIIConsole_OutputInterface :public PacketInterface_ASCII<SPD4>
{
public:
	TestASCIIConsole_OutputInterface(std::ostream* ifaceOutStreamPtrIn) :
		PacketInterface_ASCII<SPD4>(ifaceOutStreamPtrIn) {;}
	void CustomWriteTo()
	{
		// manage write state
		// from init goto waiting
		// from waiting goto sending
		// from sending goto sent
		// from any state goto init on error

		// while in sending state...
		// packet level serialization
		// token by token
		// start with packetID at token index 0
		// write to stream
		// then write delimeter (or terminator) to stream
		// loop to next token
		// use delimeter on last token

		// while in sent state...
		// indicate to port and wait for port to set state to waiting

		;// std::cout << TokenBuffer.chars[charIndex++] << std::endl;
	}
	bool SerializePacket() { return false; }
};
class TestASCIIConsole_InputInterface :public PacketInterface_ASCII<SPD4>
{
public:
	TestASCIIConsole_InputInterface(std::istream* ifaceInStreamPtrIn) :
		PacketInterface_ASCII<SPD4>(ifaceInStreamPtrIn) {;}
	void CustomReadFrom()
	{
		;// std::cin >> ;
	}
	bool DeSerializePacket() { return false; }
};
class TestASCIIConsole_OutputInterface :public PacketInterface_ASCII<SPD4>
{
public:
	TestASCIIConsole_OutputInterface(std::ostream* ifaceOutStreamPtrIn = nullptr) :
		PacketInterface_ASCII<SPD4>(ifaceOutStreamPtrIn) {
		BufferPacket.setCharsBuffer(&(TokenBuffer.chars[0]));
	}
	void CustomWriteTo()
	{
		;// std::cout << ;
	}
	bool SerializePacket() { return false; }
};

// define node specifics
class CoreTest_Console_Node :public API_NODE
{
public:
	void						Setup() { ; }
	PolymorphicPacketPort*		getPacketPort(int i)	{ return &TestPortA; };
	const int					getNumPacketPorts()		{ return 1; }
	
	// create interface instances
	TestASCIIConsole_OutputInterface	PortA_OutputIface;
	TestASCIIConsole_InputInterface		PortA_InputIface;
	PolymorphicPacketPort				TestPortA;

	CoreTest_Console_Node() :
		API_NODE(),
		PortA_OutputIface(&std::cout),
		PortA_InputIface(&std::cin),
		TestPortA(&PortA_InputIface, &PortA_OutputIface, this){;}
};

// create api node instance, linked to port object instances
CoreTest_Console_Node theTestNode;
#endif



#ifdef USING_DEFAULTS_STREAM_SERIALIZATION
// define node specifics
class CoreTest_Console_Node :public API_NODE
{
private:

	// create interface instances
	PacketInterface_ASCII<SPD4>			PortA_OutputIface;
	PacketInterface_ASCII<SPD4>			PortA_InputIface;

	PolymorphicPacketPort				TestPortA;
public:
	void								Setup()					{ ; }
	PolymorphicPacketPort*				getPacketPort(int i)	{ return &TestPortA; };
	const int							getNumPacketPorts()		{ return 1; }
	CoreTest_Console_Node() :
		API_NODE(),
		TestPortA(&PortA_InputIface, &PortA_OutputIface, this),
		PortA_OutputIface(&std::cout),
		PortA_InputIface(&std::cin)
	{
		;
	}
};

// create api node instance, linked to port object instances
CoreTest_Console_Node theTestNode;
#endif








