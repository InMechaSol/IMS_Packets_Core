/*! \file  5_IntendedUseTesting.cpp
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
#include "4_APINodePersonalization.cpp"

using namespace IMSPacketsAPICore;
/*! \defgroup IntendedUseTesting
	@{
*/
// define api node with custom input/output interface instances
class CoreTest_CustomConsole_Node :public Test_API_Node_Default
{
public:
	PolymorphicPacketPort*		getPacketPort(int i) { return &TestPortA; };
	const int					getNumPacketPorts() { return 1; }

	// create custom interface instances
	TestASCIIConsole_OutputInterface	PortA_OutputIface;
	TestASCIIConsole_InputInterface		PortA_InputIface;
	PolymorphicPacketPort				TestPortA;

	CoreTest_CustomConsole_Node() :
		Test_API_Node_Default(),
		PortA_OutputIface(&std::cout),
		PortA_InputIface(&std::cin),
		TestPortA(&PortA_InputIface, &PortA_OutputIface, this) {
		;
	}
	bool API_CustomShared_PrepareTx(HDR_Packet* TxPackOutPtr)
	{
		return false; // TODO:
	}
};

// define api node with default ascii spd4 serialization to/from cout/cin
class CoreTest_Console_Node :public Test_API_Node_Default
{
private:

	// create default interface instances
	PacketInterface_ASCII<SPD4>			PortA_OutputIface;
	PacketInterface_ASCII<SPD4>			PortA_InputIface;
	PolymorphicPacketPort				TestPortA;
public:

	PolymorphicPacketPort* getPacketPort(int i) { return &TestPortA; };
	const int							getNumPacketPorts() { return 1; }
	CoreTest_Console_Node() :
		Test_API_Node_Default(),
		TestPortA(&PortA_InputIface, &PortA_OutputIface, this),
		PortA_OutputIface(&std::cout),
		PortA_InputIface(&std::cin)
	{
		;
	}
	bool API_CustomShared_PrepareTx(HDR_Packet* TxPackOutPtr)
	{
		return false; // TODO:
	}
};

/*! @} */
