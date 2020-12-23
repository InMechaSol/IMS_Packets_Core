#include "IMS_PacketsAPI_Core.h"
using namespace IMSPacketsAPICore;


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