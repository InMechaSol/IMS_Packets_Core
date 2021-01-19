/*! \file 3_APINodeLink.h
	\brief Extension of the Packets Core utilizing Header Packets
	\sa APINodeLink
*/
#ifndef __APINODELINK__
#define __APINODELINK__
#include "3_Packet_VERSION.h"

#pragma region HDR Packets Utilize Constant and Code Template Macros 
/*! \defgroup APINodeLink
	\brief API Node and Interface Objects

	@{
*/


/*! \def TEMPLATE_RX_HANDLER(tempHDRPack,Packet_Type, TokenID, HandlerFunc)
	\brief Code Template for Application Node API EndPoint (Rx) Functions

	This template creates an on receipt api endpoint function.  The function
	determines if the linked packet is binary or string based then determines
	if a particular packet ID has been received.  If so, a packet interface
	object of type corresponding to the ID received is instantiated on the stack.
	Finally the stack packet object, which points to the packet interface buffer
	is passed to a polymorphic api endpoint specific to packet type and ID.

*/
#define TEMPLATE_RX_HANDLER(RxPackInPtr, packIDmacro){\
if(RxPackInPtr->isASCIIPacket()){\
	if(RxPackInPtr->StringBuffer_IDString_Equals(#packIDmacro)){\
		Packet_##packIDmacro myPacket_##packIDmacro;\
		myPacket_##packIDmacro.CopyTokenBufferPtrs(RxPackInPtr);\
		Handler_##packIDmacro(&myPacket_##packIDmacro);return;}}\
else{\
	if(RxPackInPtr->ByteBuffer_ID_Equals(packIDmacro)){\
		Packet_##packIDmacro myPacket_##packIDmacro;\
		myPacket_##packIDmacro.CopyTokenBufferPtrs(RxPackInPtr);\
		Handler_##packIDmacro(&myPacket_##packIDmacro);return;}}\
}

/*! \def TEMPLATE_TX_PACKAGER(tVar, pType, packFunc)
	\brief Code Template for Application Node API EndPoint (Tx) Functions


	This template creates before transmission api endpoint function.  The function
	monitors a trigger variable and if triggerred, packages a particular packet type,
	to the linked Packet Interface buffer, before resetting the trigger variable and
	returning true which will indicate to the port object that a packet is ready for transmission.

*/
#define TEMPLATE_TX_PACKAGER(TxPackOutPtr, packIDmacro){\
if(packtrigger##packIDmacro){\
	packtrigger##packIDmacro=false;\
	Packet_##packIDmacro myPacket_##packIDmacro;\
	myPacket_##packIDmacro.CopyTokenBufferPtrs(TxPackOutPtr);\
	return Packager_##packIDmacro(&myPacket_##packIDmacro); }\
}

#define TEMPLATE_PACKNODE_MEMBERS_H(packIDmacro)\
protected:	bool			packtrigger##packIDmacro = false;\
			void			setPackTrigger##packIDmacro();\
			virtual void	Handler_##packIDmacro(Packet_##packIDmacro* inPack) = 0;\
			virtual bool	Packager_##packIDmacro(Packet_##packIDmacro* outPack) = 0;\

#define TEMPLATE_PACKNODE_MEMBERS_CPP(PacketType, packIDmacro)\
void PacketType::setPackTrigger##packIDmacro() { packtrigger##packIDmacro = true; }\




/*! @}*/
#pragma endregion

namespace IMSPacketsAPICore
{
	enum PacketTypes
	{
		ReadComplete,
		ReadTokenAt,
		WriteComplete,
		WriteTokenAt,
		ResponseComplete,
		ResponseTokenAt,
		ResponseHDROnly,
		FullCyclicPartner		
	};

	/*! \addtogroup APINodeLink
		@{
	*/
	
	/*! \class PacketInterface_Binary
		\brief API Node Binary Interface for HDR_Packets
	*/
	template<class TokenType>
	class PacketInterface_Binary : public PacketInterface
	{
	protected:
		int								ByteIndexLast = 0;
		Packet_HDRPACK					BufferPacket;

		int								ByteIndex = 0;
		SPDInterfaceBuffer<TokenType>	TokenBuffer;

		void WriteToStream();
		void ReadFromStream();
		
		int deSerializedPacketSize = 0;
		int deSerializedTokenIndex = 0;
		TokenType deSerializedTokenLength;
		bool deSerializeReset = false;

		void ResetdeSerialize();

		
		
		/*! \fn DeSerializePacket_Binary
			\brief Cyclic Non-Blocking Conditional Assembly
			\sa DeSerializePacket

			Static workhorse function to facilitate testing of customization framework
			with single validated function, the one used by default.
		*/
		static bool DeSerializePacket_Binary(PacketInterface_Binary<TokenType>* PcktInterface);
		

		/*! \fn SerializePacket_Binary
			\brief Single Shot Non-Blocking Conditional Assembly
			\sa SerializePacket

			Static workhorse function to facilitate testing of customization framework
			with single validated function, the one used by default.
		*/
		static bool SerializePacket_Binary(PacketInterface_Binary<TokenType>* PcktInterface);


		/*! \fn DeSerializePacket
			\brief Cyclic Non-Blocking Conditional Assembly
			\sa DeSerializePacket_Binary

			Bytes are intended to enter the system one by one, or at least, token by token.
			As bytes are read into the packet buffer, this function analyzes the bytes to
			determine 
			- the full size of the packet from its length token at index 1, and
			- when either an error in Deserialization has occurred, or
			- when a complete packet has been deserialized into the packet interface buffer

			\return if a complete packet has been deserialized and is ready for handling
		*/
		bool DeSerializePacket();

		 
		/*! \fn SerializePacket
			\brief Single Shot Non-Blocking Conditional Assembly

			A Packet Port Instance ensures this functions is called in once immediately after 
			the positive return of the tx packet handler of the associated data execution instance.

			\return Serialization Status (true - success, false - otherwise)
		*/
		bool SerializePacket();


	public:
		/*! \fn getPacketPtr
			\brief Interface Definition of Abstract Packet Accessor
			\return Pointer to the Packet Buffer of the Interface Instance
			\sa PacketInterface:getPacketPtr
			\sa BufferPacket

		*/
		Packet* getPacketPtr();
		int		getTokenSize();

		PacketInterface_Binary(int PortIDin, std::iostream* ifaceStreamPtrIn = nullptr);
		PacketInterface_Binary(int PortIDin, std::istream* ifaceInStreamPtrIn);
		PacketInterface_Binary(int PortIDin, std::ostream* ifaceOutStreamPtrIn);
	};
	
	
	
	/*! \class PacketInterface_ASCII
		\brief API Node ASCII Interface for HDR_Packets
	*/class PacketInterface_ASCII : public PacketInterface
	{
	protected:
		int									CharIndex = 0;
		int									CharIndexLast = 0;
		SPDASCIIInterfaceBuffer				TokenBuffer;
		Packet_HDRPACK						BufferPacket;

		static void WriteToStream_ASCII(PacketInterface_ASCII* PcktInterface, std::ostream* PcktInterfaceStream);
		static void WriteToStream_ASCII(PacketInterface_ASCII* PcktInterface, std::iostream* PcktInterfaceStream);
		void WriteToStream();
		
		
		static void ReadFromStream_ASCII(PacketInterface_ASCII* PcktInterface, std::istream* PcktInterfaceStream);
		static void ReadFromStream_ASCII(PacketInterface_ASCII* PcktInterface, std::iostream* PcktInterfaceStream);
		void ReadFromStream();
		
		int deSerializedTokenIndex = 0;
		bool deSerializeReset = false;



		void ResetdeSerialize();


		/*! \fn DeSerializePacket_ASCII
			\brief Default ASCII Deserialization
			\sa Packet
			\sa DeSerializePacket
			\param PcktInterface
			\return True at Complete Packet Receiption, False otherwise

			This static function is designed to be called cyclically on receipt of each
			new character into the interface buffer.  On receipt, characters are analyzed for
			token boundaries or errors.  If an error occurs, a reset is triggered.  Once the terminator
			character is received, return true to trigger data execution instance handling.
		*/
		static bool DeSerializePacket_ASCII(PacketInterface_ASCII* PcktInterface);

		/*! \fn DeSerializePacket
			\brief Default ASCII Deserialization
			\sa DeSerializePacket_ASCII
			\return True at Complete Packet Receiption, False otherwise

			Thin wrapper around static class function.
		*/
		bool DeSerializePacket();


		/*! \fn SerializePacket_ASCII
			\brief Default ASCII Serialization
			\sa SerializePacket
			\return True if no error, clear to send.  False otherwise.

			Is called after the tx packager of the api node instance and before the writeto 
			function of the PacketInterface instance.  Its purpose is to 
			- adorn the interface buffer token strings with serialization specific delimeters and terminator, and
			- collapse packet for transmission by removing 0x00's between tokens, and
			- calculate serialized packet length.
			
			It will then indicate success or failure which in-turn will permit, or not permit, the
			interface instance writeto function.
		*/
		static bool SerializePacket_ASCII(PacketInterface_ASCII* PcktInterface);


		bool SerializePacket();
		


	public:
		
		Packet* getPacketPtr();
		int		getTokenSize(); 
		PacketInterface_ASCII(int PortIDin, std::iostream* ifaceStreamPtrIn = nullptr);
		PacketInterface_ASCII(int PortIDin, std::istream* ifaceInStreamPtrIn);
		PacketInterface_ASCII(int PortIDin, std::ostream* ifaceOutStreamPtrIn);

	};
	
	
	/*! \class API_NODE
		\brief API Node for HDR_Packets
	*/
	class API_NODE :public AbstractDataExecution
	{
	protected:
		int		currentPortIndex = 0;
		virtual PolymorphicPacketPort* getPacketPortat(int i) = 0;
		virtual int getNumPacketPorts() = 0;
		virtual void CustomLoop() = 0;

		virtual bool API_CustomShared_PrepareTx(Packet* TxPackOutPtr) = 0;
		virtual void API_CustomShared_HandleRx(Packet* RxPackInPtr) = 0;


	public:

		static const int ECOSYSTEM_MajorVersion = ECOSYSTEM_MAJORVERSION;
		static const int ECOSYSTEM_MinorVersion = ECOSYSTEM_MINORVERSION;
		static const int ECOSYSTEM_BuildNumber = ECOSYSTEM_BUILDNUMBER;
#ifdef ECOSYSTEM_RELEASEBUILD
		static const bool   ECOSYSTEM_isReleaseBuild = true;
#endif
#ifndef ECOSYSTEM_RELEASEBUILD
		static const bool   ECOSYSTEM_isReleaseBuild = false;
#endif

		static void ServiceSynchronousPorts(API_NODE* nodePtr);
		void Loop();


		void HandleRxPacket(Packet* RxPackInPtr);

		bool PrepareTxPacket(Packet* TxPackOutPtr);

		TEMPLATE_PACKNODE_MEMBERS_H(VERSION)



	public:
		

		template<class TokenType>
		static bool staticPackager_VERSION(Packet_VERSION* outPack, API_NODE* nodePtr, Struct_VERSION* srcStruct);


		template<class TokenType>
		static void staticHandler_VERSION(Packet_VERSION* inPack, API_NODE* nodePtr, Struct_VERSION* dstStruct = nullptr);


	};
	/*! @}*/
}

#endif // !__APINODELINK__
