#ifndef __APINODELINK__
#define __APINODELINK__
#include "2_PacketPortLink.h"

#pragma region HDR Packets Utilize Constant and Code Template Macros 
/*! \defgroup APINodeLink
	\brief API Node and Interface Objects

	@{
*/
/*! \def HDRPACK
	\brief Token ID value for default HDR Packet
*/
#define HDRPACK (0)


/*! \def Index_PackID
	\brief Token Index for Packet ID Token
*/
#define IndexHDR_PackID (0)


/*! \def Index_PackLEN
	\brief Token Index for Packet Length Token
*/
#define IndexHDR_PackLEN (1)


/*! \def Index_PackTYPE
	\brief Token Index for Packet Type Token
*/
#define IndexHDR_PackTYPE (2)


/*! \def Index_PackOPTION
	\brief Token Index for Packet Option Token
*/
#define IndexHDR_PackOPTION (3)


/*! \def VERSION_Offset
	\brief Token Index Offset for Derived Packet Payload Data Tokens
*/
#define VERSION_Offset IndexHDR_PackOPTION


/*! \def VERSION
	\brief Token ID value for VERSION Packet

	VERSION is of the form major.minor.build with a -dev indication
*/
#define VERSION (HDRPACK+1)


/*! \def Index_MajorVersion
	\brief Token Index for Major Version Number Token

	VERSION is of the form major.minor.build with a -dev indication
*/
#define IndexVERSION_MajorVersion (VERSION_Offset+1)


/*! \def Index_MinorVersion
	\brief Token Index for Minor Version Number Token

	VERSION is of the form major.minor.build with a -dev indication
*/
#define IndexVERSION_MinorVersion (VERSION_Offset+2)


/*! \def Index_BuildNumber
	\brief Token Index for Build Number Token

	VERSION is of the form major.minor.build with a -dev indication
*/
#define IndexVERSION_BuildNumber (VERSION_Offset+3)


/*! \def Index_DevFlag
	\brief Token Index for -def indication Token

	VERSION is of the form major.minor.build with a -dev indication
*/
#define IndexVERSION_DevFlag (VERSION_Offset+4)

/*! \def TEMPLATE_RX_HANDLER(tempHDRPack,Packet_Type, TokenID, HandlerFunc)
	\brief Code Template for Application Node API EndPoint (Rx) Functions

	This template creates an on receipt api endpoint function.  The function
	determines if the linked packet is binary or string based then determines
	if a particular packet ID has been received.  If so, a packet interface
	object of type corresponding to the ID received is instantiated on the stack.
	Finally the stack packet object, which points to the packet interface buffer
	is passed to a polymorphic api endpoint specific to packet type and ID.

*/
#define TEMPLATE_RX_HANDLER(tempHDRPack,Packet_Type, TokenID, HandlerFunc){\
if(tempHDRPack.isASCIIPacket()){\
	if(tempHDRPack.StringBuffer_IDString_Equals(#TokenID)){\
		Packet_Type my##Packet_Type;\
		my##Packet_Type.CopyTokenBufferPtrs(RxPackInPtr);\
		HandlerFunc(&my##Packet_Type);return;}}\
else{\
	if(tempHDRPack.ByteBuffer_ID_Equals(TokenID)){\
		Packet_Type my##Packet_Type;\
		my##Packet_Type.CopyTokenBufferPtrs(RxPackInPtr);\
		HandlerFunc(&my##Packet_Type);return;}}\
}

/*! \def TEMPLATE_TX_PACKAGER(tVar, pType, pID, packFunc)
	\brief Code Template for Application Node API EndPoint (Tx) Functions


	This template creates before transmission api endpoint function.  The function
	monitors a trigger variable and if triggerred, packages a particular packet type,
	to the linked Packet Interface buffer, before resetting the trigger variable and
	returning true which will indicate to the port object that a packet is ready for transmission.

*/
#define TEMPLATE_TX_PACKAGER(tVar, pType, pID, packFunc){\
if(tVar){\
	pType tempPack;\
	tempPack.CopyTokenBufferPtrs(TxPackOutPtr);\
	packFunc(&tempPack);\
	tVar=false;\
	return true; }\
}

/*! @}*/
#pragma endregion


namespace IMSPacketsAPICore
{
	/*! \addtogroup APINodeLink
		@{
	*/
	/*! \class HDR_Packet
		\brief A Packet with Header Information
	*/
	class HDR_Packet :public Packet
	{
	public:
		int						getNumSPDs()			const { return 4; }
		virtual const char*		getPacketIDString()		const { return xstr(HDRPACK); }
		virtual int				getPacketID()			const { return HDRPACK; }

		bool					StringBuffer_IDString_Equals(const char* compareStringPtr) { return false; }	// TODO :
		bool					ByteBuffer_ID_Equals(const int compareValue) { return false; }					// TODO :

		TEMPLATE_SPDSET_toVALUE(PacketID, pid, IndexHDR_PackID, pid->intVal = getPacketID())

		int						getPacketLength(SPD1 len)const { return (sizeof(SPD1) * getNumSPDs()); }
		int						getPacketLength(SPD2 len)const { return (sizeof(SPD2) * getNumSPDs()); }
		int						getPacketLength(SPD4 len)const { return (sizeof(SPD4) * getNumSPDs()); }
		int						getPacketLength(SPD8 len)const { return (sizeof(SPD8) * getNumSPDs()); }

		TEMPLATE_SPDSET_toVALUE(PacketLength, len, IndexHDR_PackLEN, len->intVal = getPacketLength(*len))

		TEMPLATE_SPDSET(PacketType, ptype, IndexHDR_PackTYPE)
		TEMPLATE_SPDGET(PacketType, ptype, IndexHDR_PackTYPE)
		TEMPLATE_SPDSET(PacketOption, popt, IndexHDR_PackOPTION)
		TEMPLATE_SPDGET(PacketOption, popt, IndexHDR_PackOPTION)
	};


	/*! \class Packet_Version
		\brief A Version Packet for Application Nodes
	*/
	class Packet_Version :public HDR_Packet
	{
	public:
		int			getNumSPDs()			const { return (HDR_Packet::getNumSPDs() + 4); }
		const char* getPacketIDString()		const { return xstr(VERSION); }
		int			getPacketID()			const { return VERSION; }

		TEMPLATE_SPDSET(MajorVersion, majVer, IndexVERSION_MajorVersion)
		TEMPLATE_SPDGET(MajorVersion, majVer, IndexVERSION_MajorVersion)
		TEMPLATE_SPDSET(MinorVersion, minVer, IndexVERSION_MinorVersion)
		TEMPLATE_SPDGET(MinorVersion, minVer, IndexVERSION_MinorVersion)
		TEMPLATE_SPDSET(BuildNumber, bldNum, IndexVERSION_BuildNumber)
		TEMPLATE_SPDGET(BuildNumber, bldNum, IndexVERSION_BuildNumber)
		TEMPLATE_SPDSET(DevFlag, DevFlg, IndexVERSION_DevFlag)
		TEMPLATE_SPDGET(DevFlag, DevFlg, IndexVERSION_DevFlag)
	};


	/*! \class PacketInterface_Binary
		\brief API Node Binary Interface for HDR_Packets
	*/
	template<class TokenType>
	class PacketInterface_Binary : public PacketInterface
	{
	protected:
		int								ByteIndexLast = 0;
		HDR_Packet						BufferPacket;

		int								ByteIndex = 0;
		SPDInterfaceBuffer<TokenType>	TokenBuffer;

		void WriteToStream()
		{
			if (ifaceStreamPtr != nullptr)
				ifaceStreamPtr->write(&(TokenBuffer.bytes[0]), serializedPacketSize);
			else if (ifaceOutStreamPtr != nullptr)
				ifaceOutStreamPtr->write(&(TokenBuffer.bytes[0]), serializedPacketSize);
		}
		
		void ReadFromStream()
		{
			if (ifaceStreamPtr != nullptr)
			{
				if (ifaceStreamPtr->peek() != EOF)
					ifaceStreamPtr->read(&(TokenBuffer.bytes[ByteIndex++]), 1);
			}
			else if (ifaceInStreamPtr != nullptr)
			{
				if (ifaceInStreamPtr->peek() != EOF)
					ifaceInStreamPtr->read(&(TokenBuffer.bytes[ByteIndex++]), 1);
			}
		}

		int deSerializedPacketSize = 0;
		int deSerializedTokenIndex = 0;
		TokenType deSerializedTokenLength;
		bool deSerializeReset = false;
		void ResetdeSerialize()
		{
			ByteIndex = 0;
			deSerializedPacketSize = 0;
			deSerializedTokenIndex = 0;
			deSerializedTokenLength.uintVal = 0;
			deSerializeReset = false;
		}
		
		template<class TokenType>
		static bool DeSerializePacket_Binary(PacketInterface_Binary<TokenType>* PcktInterface)
		{
			// called cyclically
			// monitor ByteIndex for change
			if (PcktInterface->ByteIndex != PcktInterface->ByteIndexLast)
			{
				// monitor byte array for token boundary
				if ((PcktInterface->ByteIndex % PcktInterface->getTokenSize()) == 0)
				{
					// optionally and conditionally swap byte order of tokens here
					;

					// if its the length token
					if (++PcktInterface->deSerializedTokenIndex == Index_PackLEN)
						PcktInterface->BufferPacket.getPacketLength(&PcktInterface->deSerializedTokenLength);
				}

				// decide if error, then reset
				PcktInterface->deSerializeReset = false;
				if (PcktInterface->deSerializeReset)							// TODO:
				{
					PcktInterface->ResetdeSerialize();
				}
			}

			// Capture history for change detect
			PcktInterface->ByteIndexLast = PcktInterface->ByteIndex;

			// decide if complete packet
			// return true or false
			// true will trigger the rx packet handler of the data execution instance
			if (PcktInterface->ByteIndexLast == (PcktInterface->deSerializedTokenLength.uintVal - 1))
			{
				PcktInterface->ResetdeSerialize();
				return true;
			}
			return false;
		}

		template<class TokenType>
		static bool SerializePacket_Binary(PacketInterface_Binary<TokenType>* PcktInterface)
		{
			// called single-shot after tx packet handler of the data execution instance
				// optionally swap bytes order before sending
			;

			// return true or false as error indication, true means all is well
			// true will permit sending by the output packet interface instance
			return true;
		}


		/*! \fn DeSerializePacket
			\brief Cyclic Non-Blocking Conditional Assembly

			Bytes are intended to enter the system one by one, or at least, token by token.
			As bytes are read into the packet buffer, this function analyzes the bytes to
			determine 
			- the full size of the packet from its length token at index 1, and
			- when either an error in Deserialization has occurred, or
			- when a complete packet has been deserialized into the packet interface buffer

			\return if a complete packet has been deserialized and is ready for handling
		*/
		bool DeSerializePacket()
		{
			return DeSerializePacket_Binary(this);

			//// called cyclically
			//// monitor ByteIndex for change
			//if (ByteIndex != ByteIndexLast)
			//{
			//	// monitor byte array for token boundary
			//	if ((ByteIndex % getTokenSize()) == 0)
			//	{
			//		// optionally and conditionally swap byte order of tokens here
			//		;

			//		// if its the length token
			//		if (++deSerializedTokenIndex == Index_PackLEN)
			//			BufferPacket.getPacketLength(&deSerializedTokenLength);
			//	}

			//	// decide if error, then reset
			//	deSerializeReset = false;
			//	if (deSerializeReset)							// TODO:
			//	{
			//		ResetdeSerialize();
			//	}
			//}

			//// Capture history for change detect
			//ByteIndexLast = ByteIndex;

			//// decide if complete packet
			//// return true or false
			//// true will trigger the rx packet handler of the data execution instance
			//if (ByteIndexLast == (deSerializedTokenLength.uintVal - 1))
			//{
			//	ResetdeSerialize();
			//	return true;
			//}
			//return false;
		}
		 
		/*! \fn SerializePacket
			\brief Single Shot Non-Blocking Conditional Assembly

			A Packet Port Instance ensures this functions is called in once immediately after 
			the positive return of the tx packet handler of the associated data execution instance.

			\return Serialization Status (true - success, false - otherwise)
		*/
		bool SerializePacket()
		{
			return SerializePacket_Binary();
		}
	public:
		Packet*				getPacketPtr() { return &BufferPacket; }
		int					getTokenSize() { return sizeof(TokenType); }


		PacketInterface_Binary(std::iostream* ifaceStreamPtrIn = nullptr) :
			PacketInterface(ifaceStreamPtrIn) {
			BufferPacket.setBytesBuffer(&(TokenBuffer.bytes[0]));
		}
		PacketInterface_Binary(std::istream* ifaceInStreamPtrIn) :
			PacketInterface(ifaceInStreamPtrIn) {
			BufferPacket.setBytesBuffer(&(TokenBuffer.bytes[0]));
		}
		PacketInterface_Binary(std::ostream* ifaceOutStreamPtrIn) :
			PacketInterface(ifaceOutStreamPtrIn) {
			BufferPacket.setBytesBuffer(&(TokenBuffer.bytes[0]));
		}
	};
	
	
	
	/*! \class PacketInterface_ASCII
		\brief API Node ASCII Interface for HDR_Packets
	*/
	template<class TokenType>
	class PacketInterface_ASCII : public PacketInterface
	{
	protected:
		int									CharIndex = 0;
		SPDASCIIInterfaceBuffer				TokenBuffer;
		HDR_Packet							BufferPacket;

		static void WriteToStream_ASCII(PacketInterface_ASCII<TokenType>* PcktInterface, std::ostream* PcktInterfaceStream)
		{
			PcktInterfaceStream->write(&(PcktInterface->TokenBuffer.chars[0]), PcktInterface->serializedPacketSize);
		}
		static void WriteToStream_ASCII(PacketInterface_ASCII<TokenType>* PcktInterface, std::iostream* PcktInterfaceStream)
		{
			PcktInterfaceStream->write(&(PcktInterface->TokenBuffer.chars[0]), PcktInterface->serializedPacketSize);
		}
		void WriteToStream()
		{
			if (ifaceStreamPtr != nullptr)
				WriteToStream_ASCII(this, ifaceStreamPtr);
			else if (ifaceOutStreamPtr != nullptr)
				WriteToStream_ASCII(this, ifaceOutStreamPtr);
		}
		static void ReadFromStream_ASCII(PacketInterface_ASCII<TokenType>* PcktInterface, std::istream* PcktInterfaceStream)
		{
			if (PcktInterfaceStream->peek() != EOF)
				PcktInterfaceStream->read(&(PcktInterface->TokenBuffer.chars[PcktInterface->CharIndex++]), 1);
			
		}
		static void ReadFromStream_ASCII(PacketInterface_ASCII<TokenType>* PcktInterface, std::iostream* PcktInterfaceStream)
		{
			if (PcktInterfaceStream->peek() != EOF)
				PcktInterfaceStream->read(&(PcktInterface->TokenBuffer.chars[PcktInterface->CharIndex++]), 1);

		}
		void ReadFromStream()
		{
			if (ifaceStreamPtr != nullptr)
			{
				ReadFromStream_ASCII(this, ifaceStreamPtr);
			}
			else if (ifaceInStreamPtr != nullptr)
			{
				ReadFromStream_ASCII(this, ifaceInStreamPtr);
			}
		}
		static bool DeSerializePacket_ASCII(PacketInterface_ASCII<TokenType>* PcktInterface)
		{
			// called cyclically
			// monitor CharIndex for change
			// analyze char array
			// decide if complete packet
			// decide if error
			// return true or false
			// true will trigger the packet handler of the data execution instance
			return false;
		}
		bool DeSerializePacket()
		{
			return DeSerializePacket_ASCII(this);
		}
		static bool SerializePacket_ASCII(PacketInterface_ASCII<TokenType>* PcktInterface)
		{
			// called single-shot
			// return true or false
			return false;
		}
		bool SerializePacket()
		{
			return SerializePacket_ASCII(this);
		}
	public:
		Packet* getPacketPtr() { return &BufferPacket; }
		int					getTokenSize() { return sizeof(TokenType); }
		PacketInterface_ASCII(std::iostream* ifaceStreamPtrIn = nullptr) :
			PacketInterface(ifaceStreamPtrIn) {
			BufferPacket.setCharsBuffer(&(TokenBuffer.chars[0]));
		}
		PacketInterface_ASCII(std::istream* ifaceInStreamPtrIn) :
			PacketInterface(ifaceInStreamPtrIn) {
			BufferPacket.setCharsBuffer(&(TokenBuffer.chars[0]));
		}
		PacketInterface_ASCII(std::ostream* ifaceOutStreamPtrIn) :
			PacketInterface(ifaceOutStreamPtrIn) {
			BufferPacket.setCharsBuffer(&(TokenBuffer.chars[0]));
		}

	};
	/*! \class API_NODE
		\brief API Node for HDR_Packets
	*/
	class API_NODE :public AbstractDataExecution
	{
	private:
		bool triggerVERSIONPackager = false;
	protected:
		void setVersionPackTrigger() { triggerVERSIONPackager = true; }
		virtual PolymorphicPacketPort* getPacketPortat(int i) = 0;
		virtual const int getNumPacketPorts() = 0;
		virtual void CustomLoop() = 0;
		virtual void VERSION_Handler(Packet_Version* inPack) = 0;
		virtual void VERSION_Packager(Packet_Version* outPack) = 0;
		virtual bool API_CustomShared_PrepareTx(HDR_Packet* TxPackOutPtr) = 0;
		virtual void API_CustomShared_HandleRx(HDR_Packet* RxPackInPtr) = 0;
	public:
		void Loop()
		{
			CustomLoop();
			for (int i = 0; i < getNumPacketPorts(); i++)
				(getPacketPortat(i))->ServicePort();
		}
		void HandleRxPacket(Packet* RxPackInPtr)
		{
			HDR_Packet tempPack;
			tempPack.CopyTokenBufferPtrs(RxPackInPtr);

			TEMPLATE_RX_HANDLER(tempPack, Packet_Version, VERSION, VERSION_Handler)

			API_CustomShared_HandleRx(&tempPack);
		}

		bool PrepareTxPacket(Packet* TxPackOutPtr)
		{
			HDR_Packet tempPack;
			tempPack.CopyTokenBufferPtrs(TxPackOutPtr);

			TEMPLATE_TX_PACKAGER(triggerVERSIONPackager, Packet_Version, VERSION, VERSION_Packager)

			return API_CustomShared_PrepareTx(&tempPack);
		}
	};
	/*! @}*/
}

#endif // !__APINODELINK__
