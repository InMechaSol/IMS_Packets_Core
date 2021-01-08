/*! \file 3_APINodeLink.h
	\brief Extension of the Packets Core utilizing Header Packets
	\sa APINodeLink
*/
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
static const char IDString_HDRPACK[] = xstr(HDRPACK);

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
static const char IDString_VERSION[] = xstr(VERSION);

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
#define TEMPLATE_RX_HANDLER(tempHDRPack, Packet_Type, Packet_ID, HandlerFunc){\
if(tempHDRPack.isASCIIPacket()){\
	if(tempHDRPack.StringBuffer_IDString_Equals(#Packet_ID)){\
		Packet_Type my##Packet_Type;\
		my##Packet_Type.CopyTokenBufferPtrs(RxPackInPtr);\
		HandlerFunc(&my##Packet_Type);return;}}\
else{\
	if(tempHDRPack.ByteBuffer_ID_Equals(Packet_ID)){\
		Packet_Type my##Packet_Type;\
		my##Packet_Type.CopyTokenBufferPtrs(RxPackInPtr);\
		HandlerFunc(&my##Packet_Type);return;}}\
}

/*! \def TEMPLATE_TX_PACKAGER(tVar, pType, packFunc)
	\brief Code Template for Application Node API EndPoint (Tx) Functions


	This template creates before transmission api endpoint function.  The function
	monitors a trigger variable and if triggerred, packages a particular packet type,
	to the linked Packet Interface buffer, before resetting the trigger variable and
	returning true which will indicate to the port object that a packet is ready for transmission.

*/
#define TEMPLATE_TX_PACKAGER(tVar, pType, packFunc){\
if(tVar){\
	tVar=false;\
	pType tempPack;\
	tempPack.CopyTokenBufferPtrs(TxPackOutPtr);\
	return packFunc(&tempPack); }\
}

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
	/*! \class HDR_Packet
		\brief A Packet with Header Information
	*/
	class HDR_Packet :public Packet
	{	
	public:		
		char*				getPacketIDString()		{ return (char*)&IDString_HDRPACK[0]; }
		static const int	PacketID				= HDRPACK;
		int					getPacketID()			{ return HDR_Packet::PacketID; }
		static const int	NumSPDs					= 4;
		int					getNumSPDs()			{ return HDR_Packet::NumSPDs; }
		
		TEMPLATE_SPDACCESSORS(PacketType, IndexHDR_PackTYPE, typeINT, "%d")
		TEMPLATE_SPDACCESSORS(PacketOption, IndexHDR_PackOPTION, typeINT, "%d")
	};


	/*! \class Packet_Version
		\brief A Version Packet for Application Nodes
	*/
	class Packet_Version :public HDR_Packet
	{
	public:
		char*				getPacketIDString()		{ return (char*)&IDString_VERSION[0]; }
		static const int	PacketID				= VERSION;
		int					getPacketID()			{ return Packet_Version::PacketID; }
		static const int	NumSPDs					= HDR_Packet::NumSPDs+4;
		int					getNumSPDs()			{ return Packet_Version::NumSPDs; }

		TEMPLATE_SPDACCESSORS(MajorVersion, IndexVERSION_MajorVersion, typeINT, "%d")
		TEMPLATE_SPDACCESSORS(MinorVersion, IndexVERSION_MinorVersion, typeINT, "%d")
		TEMPLATE_SPDACCESSORS(BuildNumber, IndexVERSION_BuildNumber, typeINT, "%d")
		TEMPLATE_SPDACCESSORS(DevFlag, IndexVERSION_DevFlag, typeINT, "%d")

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
			ByteIndexLast = 0;
			deSerializedPacketSize = 0;
			deSerializedTokenIndex = 0;
			deSerializedTokenLength.uintVal = 0;
			deSerializeReset = false;
		}
		
		/*! \fn DeSerializePacket_Binary
			\brief Cyclic Non-Blocking Conditional Assembly
			\sa DeSerializePacket

			Static workhorse function to facilitate testing of customization framework
			with single validated function, the one used by default.
		*/
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

					// decide if error, trigger reset
					PcktInterface->deSerializeReset = false;// TODO:

					// if its the length token index
					if (++PcktInterface->deSerializedTokenIndex == Index_PackLEN)
						PcktInterface->BufferPacket.readbuff_PackLength(&PcktInterface->deSerializedTokenLength);

					// decide if complete packet
					// return true or false
					// true will trigger the rx packet handler of the data execution instance
					if (PcktInterface->ByteIndex == (PcktInterface->deSerializedTokenLength.uintVal - 1))
					{
						PcktInterface->ResetdeSerialize();
						return true;
					}
				}
			}

			

			// Reset if triggerred
			if (PcktInterface->deSerializeReset)							
			{
				PcktInterface->ResetdeSerialize();
			}

			// Capture history for change detect
			PcktInterface->ByteIndexLast = PcktInterface->ByteIndex;			
			return false;
		}


		/*! \fn SerializePacket_Binary
			\brief Single Shot Non-Blocking Conditional Assembly
			\sa SerializePacket

			Static workhorse function to facilitate testing of customization framework
			with single validated function, the one used by default.
		*/
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
			\sa DeSerializePacket_Binary

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
		/*! \fn getPacketPtr
			\brief Interface Definition of Abstract Packet Accessor
			\return Pointer to the Packet Buffer of the Interface Instance
			\sa PacketInterface:getPacketPtr
			\sa BufferPacket

		*/
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
		int									CharIndexLast = 0;
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
		
		int deSerializedTokenIndex = 0;
		bool deSerializeReset = false;



		void ResetdeSerialize()
		{
			CharIndex = 0;
			CharIndexLast = 0;
			deSerializedTokenIndex = 0;
			deSerializeReset = false;
		}


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
		static bool DeSerializePacket_ASCII(PacketInterface_ASCII<TokenType>* PcktInterface)
		{
			// called cyclically
			// monitor CharIndex for change
			if (PcktInterface->CharIndexLast != PcktInterface->CharIndex)
			{
				// decide if error, trigger reset
				if (!Packet::isASCIIchar(PcktInterface->TokenBuffer.chars[PcktInterface->CharIndex]))
				{
					PcktInterface->deSerializeReset = true; // TODO:  ??
				}

				// look for delimeter/terminator ?
				else if (Packet::isDelimiterchar(PcktInterface->TokenBuffer.chars[PcktInterface->CharIndex]) || Packet::isTerminatorchar(PcktInterface->TokenBuffer.chars[PcktInterface->CharIndex]))
				{
					// increment token index
					PcktInterface->deSerializedTokenIndex++;

					// "strip" delimiter/terminator
					// 0x00 to all chars from delimiter to next token start
					for (int i = PcktInterface->CharIndex; i < (PcktInterface->deSerializedTokenIndex * STRINGBUFFER_TOKENRATIO); i++)
						PcktInterface->TokenBuffer.chars[i] = 0x00;
					
					// decide if complete packet
					// return true or false
					// true will trigger the rx packet handler of the data execution instance
					if (Packet::isTerminatorchar(PcktInterface->TokenBuffer.chars[PcktInterface->CharIndex]))
					{
						PcktInterface->ResetdeSerialize();
						if(PcktInterface->deSerializedTokenIndex >= HDR_Packet::NumSPDs)
							return true;
					}

					// advance index to next token start
					PcktInterface->CharIndex = (PcktInterface->deSerializedTokenIndex * STRINGBUFFER_TOKENRATIO);
				}
			}
			
			// reset if triggered
			if (PcktInterface->deSerializeReset)
			{
				PcktInterface->ResetdeSerialize();
			}

			// Capture history for change detect
			PcktInterface->CharIndexLast = PcktInterface->CharIndex;			
			return false;
		}


		/*! \fn DeSerializePacket
			\brief Default ASCII Deserialization
			\sa DeSerializePacket_ASCII
			\return True at Complete Packet Receiption, False otherwise

			Thin wrapper around static class function.
		*/
		bool DeSerializePacket()
		{
			return DeSerializePacket_ASCII(this);
		}


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
		static bool SerializePacket_ASCII(PacketInterface_ASCII<TokenType>* PcktInterface)
		{
			// called single-shot
			int lastCharIndexWritten = 0;	// initialized to start of id string
			int SerializedTokenCount = HDR_Packet::NumSPDs;	// initialized to minimum token of HDR packet
			int j;
			int k;

			// iterate through token buffer by token index
			for (int i = 0; i < PACKETBUFFER_TOKENCOUNT; i++)
			{
				// calculate inner loop bounds
				if (i == 0)
				{
					j = 0;
					k = STRINGBUFFER_IDTOKENRATIO;
				}
				else
				{
					j = STRINGBUFFER_IDTOKENRATIO + (i-1) * STRINGBUFFER_TOKENRATIO;
					k = STRINGBUFFER_IDTOKENRATIO + i     * STRINGBUFFER_TOKENRATIO;
				}

				// iterate through token by char index
				for (j; j<k; j++)
				{
					if (i == Index_PackID)	// working the ID string
					{
						// adorn each token at first occurance of 0x00, replace with delimiter
						if (PcktInterface->TokenBuffer.chars[j] == 0x00)
						{
							PcktInterface->TokenBuffer.chars[j] = ASCII_colon;
							lastCharIndexWritten = j;	// latch index of last char "written"
							break; // break from inner loop, token by char, loop
						}
					}					
					else // working all inner tokens (not first or last)
					{
						// adorn each token at first occurance of 0x00
						if (PcktInterface->TokenBuffer.chars[j] == 0x00)
						{
							if (i == Index_PackLEN) // working the SPD Count String
							{
								SerializedTokenCount = atoi(&PcktInterface->TokenBuffer.chars[STRINGBUFFER_IDTOKENRATIO + (i - 1) * STRINGBUFFER_TOKENRATIO]);

								// an error has occurred with the token count string if parsed token count string less than hdr packet token count
								if (SerializedTokenCount < HDR_Packet::NumSPDs)
									return false;
							}

							// add delimiter/terminator to shifted location
							if (i == (SerializedTokenCount - 1)) // working the final token string
							{
								PcktInterface->TokenBuffer.chars[++lastCharIndexWritten] = ASCII_semicolon;
								PcktInterface->serializedPacketSize = lastCharIndexWritten + 1;
								return true;
							}
							else
							{
								PcktInterface->TokenBuffer.chars[++lastCharIndexWritten] = ASCII_colon;
								break; // break from inner loop, token by char, loop
							}
							
							
						}
						else
						{
							// shift chars down to previous token delimiter
							PcktInterface->TokenBuffer.chars[++lastCharIndexWritten] = PcktInterface->TokenBuffer.chars[j];
						}
					}
				}				
			}
			return false;
		}


		bool SerializePacket()
		{
			return SerializePacket_ASCII(this);
		}


	public:
		Packet*				getPacketPtr() { return &BufferPacket; }
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
		virtual int getNumPacketPorts() = 0;
		virtual void CustomLoop() = 0;

		virtual void VERSION_Handler(Packet_Version* inPack) = 0;
		virtual bool VERSION_Packager(Packet_Version* outPack) = 0;

		virtual bool API_CustomShared_PrepareTx(Packet* TxPackOutPtr) = 0;
		virtual void API_CustomShared_HandleRx(Packet* RxPackInPtr) = 0;
	public:
		void Loop()
		{
			CustomLoop();
			for (int i = 0; i < getNumPacketPorts(); i++)
				(getPacketPortat(i))->ServicePort();
		}
		void HandleRxPacket(Packet* RxPackInPtr)
		{ 
			TEMPLATE_RX_HANDLER((*RxPackInPtr), Packet_Version, VERSION, VERSION_Handler)

			API_CustomShared_HandleRx(RxPackInPtr);
		}

		bool PrepareTxPacket(Packet* TxPackOutPtr)
		{
			TEMPLATE_TX_PACKAGER(triggerVERSIONPackager, Packet_Version, VERSION_Packager)

			return API_CustomShared_PrepareTx(TxPackOutPtr);
		}
	};
	/*! @}*/
}

#endif // !__APINODELINK__