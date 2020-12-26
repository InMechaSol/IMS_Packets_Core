/*! \file  TestSource.cpp
	\brief Examples of Intended Use Cases

	The IMS Packets Core is intended for use by developers of distributed
	computing systems.  It provides a base of code that can be used in a 
	"default" configuration or modified easily to custom needs.  

	TestSource.cpp defines cross platform reusable test objects that demonstrate
	the intended use cases (for developers) and provided testable objects to be
	instantiated on Console Applications, Unit Test Applications, and Node Firmware.

	As developments move from default to fully custom implementations, a hierarchy of 
	developer types emerges.  System developers are responsible for understanding the 
	communication partners within an ecosystem.  Application developers are responsible for
	understanding the application requirements of a particular node, or set of nodes,
	within an ecosystem.
	- System developers would first define a common node language and api enpoints.  
	- Application developers would then define application nodes which rely on the definitions of common node language and api endpoints.

*/
#include "IMS_PacketsAPI_Core.h"
using namespace IMSPacketsAPICore;

/*! \class Test_API_Node_Default
	\brief An Example with Default Configuration

	System developer would first define a common node language and api endpoints.
	Through Selection of Packet Port Type and Input/Output Interface Combinations, 
	default features can be used to exchange packets and execute api endpoints.

	This test node is an example of enterprise level (system level) node definition
	intended to be designed and implemented by system developers.

	In particular, this node is an example with no customization...only default features
	this test node defines the base cross-platform node structure that would then 
	be used to derive application nodes and define specific api endpoint functions.

*/
class Test_API_Node : public API_NODE
{
public:
			Test_API_Node() :API_NODE()							{ ; }
	void	Setup()												{ ; }
protected:

	void	CustomLoop()										{ ; }

	void	VERSION_Handler(Packet_Version* inPack)				{ ; }
	void	VERSION_Packager(Packet_Version* outPack) 			{ ; }

	bool	API_CustomShared_PrepareTx(Packet* TxPackOutPtr)	{ ; }
	void	API_CustomShared_HandleRx(HDR_Packet* RxPackInPtr)	{ ; }

};


// define interface functions (*link the stream)
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
class CoreTest_CustomConsole_Node :public Test_API_Node
{
public:
	PolymorphicPacketPort*		getPacketPort(int i)	{ return &TestPortA; };
	const int					getNumPacketPorts()		{ return 1; }
	
	// create custom interface instances
	TestASCIIConsole_OutputInterface	PortA_OutputIface;
	TestASCIIConsole_InputInterface		PortA_InputIface;
	PolymorphicPacketPort				TestPortA;

	CoreTest_CustomConsole_Node() :
		Test_API_Node(),
		PortA_OutputIface(&std::cout),
		PortA_InputIface(&std::cin),
		TestPortA(&PortA_InputIface, &PortA_OutputIface, this){;}
};

// define node specifics
class CoreTest_Console_Node :public Test_API_Node
{
private:

	// create interface instances
	PacketInterface_ASCII<SPD4>			PortA_OutputIface;
	PacketInterface_ASCII<SPD4>			PortA_InputIface;

	PolymorphicPacketPort				TestPortA;
public:
	
	PolymorphicPacketPort*				getPacketPort(int i)	{ return &TestPortA; };
	const int							getNumPacketPorts()		{ return 1; }
	CoreTest_Console_Node() :
		Test_API_Node(),
		TestPortA(&PortA_InputIface, &PortA_OutputIface, this),
		PortA_OutputIface(&std::cout),
		PortA_InputIface(&std::cin)
	{
		;
	}
};