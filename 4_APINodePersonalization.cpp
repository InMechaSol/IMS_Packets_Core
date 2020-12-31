#include "3_APINodeLink.h"

using namespace IMSPacketsAPICore;

/*! \defgroup APINodePersonalization
	@{
*/
/*! \class Test_API_Node_Default
	\brief An Example with Default Configuration

	System developer would first define a common node language and api endpoints.
	Through Selection of Packet Port Type and Input/Output Interface Combinations,
	default features can be used to exchange packets and execute api endpoints.

	This test node is an example of enterprise level (system level) node definition
	intended to be designed and implemented by system developers.

	In particular, this node is an example with no customization, only default features.
	This test node defines the base cross-platform node structure to then
	be used to derive application nodes and define specific api endpoint functions.

*/
class Test_API_Node_Default : public API_NODE
{
public:
	Test_API_Node_Default() :API_NODE() { ; }
	void	Setup() { ; }
protected:

	void	CustomLoop() { ; }

	void	VERSION_Handler(Packet_Version* inPack) { ; }
	void	VERSION_Packager(Packet_Version* outPack) { ; }

	bool	API_CustomShared_PrepareTx(Packet* TxPackOutPtr) { ; }
	void	API_CustomShared_HandleRx(HDR_Packet* RxPackInPtr) { ; }

};

// define interface functions (*link the stream)
class TestASCIIConsole_InputInterface :public PacketInterface_ASCII<SPD4>
{
public:
	TestASCIIConsole_InputInterface(std::istream* ifaceInStreamPtrIn = nullptr) :
		PacketInterface_ASCII<SPD4>(ifaceInStreamPtrIn) { 
		BufferPacket.setCharsBuffer(&(TokenBuffer.chars[0])); 
	}
	void CustomReadFrom()
	{
		ReadFromStream_ASCII(this, &std::cin);
	}
	bool DeSerializePacket() 
	{ 
		return DeSerializePacket_ASCII(this);
	}
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
		WriteToStream_ASCII(this, &std::cout);
	}
	bool SerializePacket() 
	{ 
		return SerializePacket_ASCII(this); 
	}
};


/*! @}*/