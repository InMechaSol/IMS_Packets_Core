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


/*! @}*/
#pragma endregion

namespace IMSPacketsAPICore
{
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
		
		TokenType deSerializedTokenLength;
		int deSerializedPacketSize = 0;

		int deSerializedTokenIndex = 0;		
		bool deSerializeReset = false;
		void ResetdeSerialize();

		
	public:	
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

		/*! \fn getPacketPtr
			\brief Interface Definition of Abstract Packet Accessor
			\return Pointer to the Packet Buffer of the Interface Instance
			\sa PacketInterface:getPacketPtr
			\sa BufferPacket

		*/
		Packet* getPacketPtr();
		int		getTokenSize();

		PacketInterface_Binary(std::iostream* ifaceStreamPtrIn = nullptr);
		PacketInterface_Binary(std::istream* ifaceInStreamPtrIn);
		PacketInterface_Binary(std::ostream* ifaceOutStreamPtrIn);
	};
	
	
	
	/*! \class PacketInterface_ASCII
		\brief API Node ASCII Interface for HDR_Packets
	*/
	class PacketInterface_ASCII : public PacketInterface
	{
	protected:
		int									CharIndex = 0;
		int									CharIndexLast = 0;
		SPDASCIIInterfaceBuffer				TokenBuffer;
		Packet_HDRPACK						BufferPacket;
		
		void WriteToStream();
		void ReadFromStream();
		
		int deSerializedTokenIndex = 0;
		bool deSerializeReset = false;
		void ResetdeSerialize();


	public:
		
		Packet* getPacketPtr();
		int		getTokenSize(); 
		int		getPacketOption();
		enum PacketTypes	getPacketType();
		PacketInterface_ASCII(std::iostream* ifaceStreamPtrIn = nullptr);
		PacketInterface_ASCII(std::istream* ifaceInStreamPtrIn);
		PacketInterface_ASCII(std::ostream* ifaceOutStreamPtrIn);
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
		static void WriteToStream_ASCII(PacketInterface_ASCII* PcktInterface, std::ostream* PcktInterfaceStream);
		static void WriteToStream_ASCII(PacketInterface_ASCII* PcktInterface, std::iostream* PcktInterfaceStream);
		static void ReadFromStream_ASCII(PacketInterface_ASCII* PcktInterface, std::istream* PcktInterfaceStream);
		static void ReadFromStream_ASCII(PacketInterface_ASCII* PcktInterface, std::iostream* PcktInterfaceStream);
		/*! \fn DeSerializePacket
			\brief Default ASCII Deserialization
			\sa DeSerializePacket_ASCII
			\return True at Complete Packet Receiption, False otherwise

			Thin wrapper around static class function.
		*/
		bool DeSerializePacket();
		bool SerializePacket();
	};
	
	
	/*! \class API_NODE
		\brief API Node for HDR_Packets
	*/
	class API_NODE :public AbstractDataExecution
	{
	protected:
		virtual PolymorphicPacketPort* getPacketPortat(int i) = 0;
		virtual int getNumPacketPorts() = 0;
		virtual void CustomLoop() = 0;

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


		static void staticHandler_HDRPACK(Packet* PacketPtr, enum PacketTypes PackType, pSTRUCT(HDRPACK)* dstStruct);

		static bool staticPackager_HDRPACK(Packet* PacketPtr, enum PacketTypes PackType, int PackOption);
		
	};
	
	
	/*! @}*/
}

#endif // !__APINODELINK__
