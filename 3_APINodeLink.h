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

#define TEMPLATE_PACKNODE_MEMBERS(packIDmacro)\
protected:	bool			packtrigger##packIDmacro = false;\
			void			setPackTrigger##packIDmacro() { packtrigger##packIDmacro = true; }\
			virtual void	Handler_##packIDmacro(Packet_##packIDmacro* inPack) = 0;\
			virtual bool	Packager_##packIDmacro(Packet_##packIDmacro* outPack) = 0;\





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
					if (PcktInterface->deSerializedTokenIndex == Index_PackLEN)
						PcktInterface->BufferPacket.readbuff_PackLength(&PcktInterface->deSerializedTokenLength);
					else if (PcktInterface->deSerializedTokenIndex == iHDRPACK_PacketOption) {
						TokenType x_SPD;
						x_SPD.intVal = PcktInterface->getPortID();
						PcktInterface->BufferPacket.setPacketOption(&x_SPD);
					}


					// decide if complete packet
					// return true or false
					// true will trigger the rx packet handler of the data execution instance
					if (PcktInterface->ByteIndex == PcktInterface->deSerializedTokenLength.uintVal)
					{
						PcktInterface->ResetdeSerialize();
						return true;
					}
					PcktInterface->deSerializedTokenIndex++;
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


		PacketInterface_Binary(int PortIDin, std::iostream* ifaceStreamPtrIn = nullptr) :
			PacketInterface(PortIDin, ifaceStreamPtrIn) {
			BufferPacket.setBytesBuffer(&(TokenBuffer.bytes[0]));
		}
		PacketInterface_Binary(int PortIDin, std::istream* ifaceInStreamPtrIn) :
			PacketInterface(PortIDin, ifaceInStreamPtrIn) {
			BufferPacket.setBytesBuffer(&(TokenBuffer.bytes[0]));
		}
		PacketInterface_Binary(int PortIDin, std::ostream* ifaceOutStreamPtrIn) :
			PacketInterface(PortIDin, ifaceOutStreamPtrIn) {
			BufferPacket.setBytesBuffer(&(TokenBuffer.bytes[0]));
		}
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

		static void WriteToStream_ASCII(PacketInterface_ASCII* PcktInterface, std::ostream* PcktInterfaceStream)
		{
			PcktInterfaceStream->write(&(PcktInterface->TokenBuffer.chars[0]), PcktInterface->serializedPacketSize);
		}
		static void WriteToStream_ASCII(PacketInterface_ASCII* PcktInterface, std::iostream* PcktInterfaceStream)
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
		
		
		static void ReadFromStream_ASCII(PacketInterface_ASCII* PcktInterface, std::istream* PcktInterfaceStream)
		{
			if (PcktInterfaceStream->peek() != EOF)
				PcktInterfaceStream->read(&(PcktInterface->TokenBuffer.chars[PcktInterface->CharIndex++]), 1);
			
		}
		static void ReadFromStream_ASCII(PacketInterface_ASCII* PcktInterface, std::iostream* PcktInterfaceStream)
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
		static bool DeSerializePacket_ASCII(PacketInterface_ASCII* PcktInterface)
		{
			// called cyclically
			// monitor CharIndex for change
			if (PcktInterface->CharIndexLast != PcktInterface->CharIndex)
			{
				int lastIndex = PcktInterface->CharIndex - 1;
				
				// decide if error, trigger reset
				if (!Packet::isASCIIchar(PcktInterface->TokenBuffer.chars[lastIndex]) || PcktInterface->TokenBuffer.chars[lastIndex]==ASCII_lf || PcktInterface->TokenBuffer.chars[lastIndex] == ASCII_cr)
				{
					PcktInterface->deSerializeReset = true; // TODO:  ??
				}
				// look for delimeter/terminator ?
				else if (Packet::isDelimiterchar(PcktInterface->TokenBuffer.chars[lastIndex]) || Packet::isTerminatorchar(PcktInterface->TokenBuffer.chars[lastIndex]))
				{				
					int NextTokenStart = (STRINGBUFFER_IDTOKENRATIO + PcktInterface->deSerializedTokenIndex * STRINGBUFFER_TOKENRATIO);

					// if its the packet option token, mark Rx packet with portID
					if (iHDRPACK_PacketOption == PcktInterface->deSerializedTokenIndex)
					{
						char lastChar = PcktInterface->TokenBuffer.chars[lastIndex];
						int LastTokenStart = NextTokenStart - STRINGBUFFER_TOKENRATIO;
						int charsWritten = snprintf(&(PcktInterface->TokenBuffer.chars[LastTokenStart]), STRINGBUFFER_TOKENRATIO, "%d%c", PcktInterface->getPortID(),lastChar);
						lastIndex = LastTokenStart + charsWritten-1;
					}

					// decide if complete packet
					// return true or false
					// true will trigger the rx packet handler of the data execution instance
					if (Packet::isTerminatorchar(PcktInterface->TokenBuffer.chars[lastIndex]))
					{
						// "strip" terminator
						// 0x00 to all chars from terminator to next token start
						for (int i = lastIndex; i < NextTokenStart; i++)
							PcktInterface->TokenBuffer.chars[i] = 0x00;

						// increment token index
						PcktInterface->deSerializedTokenIndex++;
						if (PcktInterface->deSerializedTokenIndex >= TokenCount_HDRPACK) {
							PcktInterface->ResetdeSerialize();
							return true;
						}
					}
					else
					{
						
						// "strip" delimiter
						// 0x00 to all chars from delimiter to next token start
						for (int i = lastIndex; i < (STRINGBUFFER_IDTOKENRATIO + PcktInterface->deSerializedTokenIndex * STRINGBUFFER_TOKENRATIO); i++)
							PcktInterface->TokenBuffer.chars[i] = 0x00;
							
						
						// advance index to next token start
						PcktInterface->CharIndex = (STRINGBUFFER_IDTOKENRATIO + PcktInterface->deSerializedTokenIndex * STRINGBUFFER_TOKENRATIO);
						// increment token index
						PcktInterface->deSerializedTokenIndex++;
					}
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
		static bool SerializePacket_ASCII(PacketInterface_ASCII* PcktInterface)
		{
			// called single-shot
			int lastCharIndexWritten = 0;	// initialized to start of id string
			int SerializedTokenCount = TokenCount_HDRPACK;	// initialized to minimum token of HDR packet
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
								if (SerializedTokenCount < TokenCount_HDRPACK)
									return false;
							}

							// add delimiter/terminator to shifted location
							if (i == (SerializedTokenCount - 1)) // working the final token string
							{
								PcktInterface->TokenBuffer.chars[++lastCharIndexWritten] = ASCII_semicolon;
								PcktInterface->TokenBuffer.chars[++lastCharIndexWritten] = ASCII_lf;
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
		int					getTokenSize() { return STRINGBUFFER_TOKENRATIO; }
		PacketInterface_ASCII(int PortIDin, std::iostream* ifaceStreamPtrIn = nullptr) :
			PacketInterface(PortIDin, ifaceStreamPtrIn) {
			BufferPacket.setCharsBuffer(&(TokenBuffer.chars[0]));
		}
		PacketInterface_ASCII(int PortIDin, std::istream* ifaceInStreamPtrIn) :
			PacketInterface(PortIDin, ifaceInStreamPtrIn) {
			BufferPacket.setCharsBuffer(&(TokenBuffer.chars[0]));
		}
		PacketInterface_ASCII(int PortIDin, std::ostream* ifaceOutStreamPtrIn) :
			PacketInterface(PortIDin, ifaceOutStreamPtrIn) {
			BufferPacket.setCharsBuffer(&(TokenBuffer.chars[0]));
		}

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

		static void ServiceSynchronousPorts(API_NODE* nodePtr)
		{
			for (int i = 0; i < nodePtr->getNumPacketPorts(); i++)
			{
				if(!(nodePtr->getPacketPortat(i))->getAsyncService())
					(nodePtr->getPacketPortat(i))->ServicePort();
			}
				
		}
		void Loop()
		{
			CustomLoop();
			ServiceSynchronousPorts(this);
		}


		void HandleRxPacket(Packet* RxPackInPtr)
		{
			// Mark Rx packet with port index

			TEMPLATE_RX_HANDLER(RxPackInPtr, VERSION)

			API_CustomShared_HandleRx(RxPackInPtr);
		}

		bool PrepareTxPacket(Packet* TxPackOutPtr)
		{
			TEMPLATE_TX_PACKAGER(TxPackOutPtr, VERSION)

			return API_CustomShared_PrepareTx(TxPackOutPtr);
		}



#pragma region Packet_VERSION Members

		TEMPLATE_PACKNODE_MEMBERS(VERSION)		

		template<class TokenType>
		static void staticHandler_VERSION(Packet_VERSION* inPack, API_NODE* nodePtr, Struct_VERSION* dstStruct = nullptr)
		{
			TokenType x_SPD;
			if (inPack->isASCIIPacket())
				inPack->getfromStringPacketType(&x_SPD);
			else
				inPack->getPacketType(&x_SPD);

			if (x_SPD.intVal == ReadComplete)
				nodePtr->setPackTriggerVERSION();
			else if (x_SPD.intVal == ResponseComplete && dstStruct!=nullptr)
			{
				if (inPack->isASCIIPacket()) inPack->getfromStringMajorVersion(&x_SPD); else inPack->getMajorVersion(&x_SPD);
				dstStruct->Major = x_SPD.intVal;
				if (inPack->isASCIIPacket()) inPack->getfromStringMinorVersion(&x_SPD); else inPack->getMinorVersion(&x_SPD);
				dstStruct->Minor = x_SPD.intVal;
				if (inPack->isASCIIPacket()) inPack->getfromStringBuildNumber(&x_SPD); else inPack->getBuildNumber(&x_SPD);
				dstStruct->Build = x_SPD.intVal;
				if (inPack->isASCIIPacket()) inPack->getfromStringDevFlag(&x_SPD); else inPack->getDevFlag(&x_SPD);
				dstStruct->DevFlag = x_SPD.intVal;
			}

		}

		template<class TokenType>
		static bool staticPackager_VERSION(Packet_VERSION* outPack, API_NODE* nodePtr, Struct_VERSION* srcStruct)
		{
			TokenType x_SPD;
			bool tempBool = true;
			outPack->isASCIIPacket() ? outPack->writebuff_PackIDString() : outPack->writebuff_PackID(&x_SPD);
			outPack->isASCIIPacket() ? outPack->writebuff_TokenCountString() : outPack->writebuff_PackLength(&x_SPD);
			x_SPD.intVal = ResponseComplete;
			if (outPack->isASCIIPacket()) tempBool &= outPack->set2StringPacketType(&x_SPD);	else outPack->setPacketType(&x_SPD);
			x_SPD.intVal = 0;
			if (outPack->isASCIIPacket()) tempBool &= outPack->set2StringPacketOption(&x_SPD);	else outPack->setPacketOption(&x_SPD);
			x_SPD.intVal = srcStruct->Major;
			if (outPack->isASCIIPacket()) tempBool &= outPack->set2StringMajorVersion(&x_SPD);	else outPack->setMajorVersion(&x_SPD);
			x_SPD.intVal = srcStruct->Minor;
			if (outPack->isASCIIPacket()) tempBool &= outPack->set2StringMinorVersion(&x_SPD);	else outPack->setMinorVersion(&x_SPD);
			x_SPD.intVal = srcStruct->Build;
			if (outPack->isASCIIPacket()) tempBool &= outPack->set2StringBuildNumber(&x_SPD);	else outPack->setBuildNumber(&x_SPD);
			x_SPD.intVal = srcStruct->DevFlag;
			if (outPack->isASCIIPacket()) tempBool &= outPack->set2StringDevFlag(&x_SPD);		else outPack->setDevFlag(&x_SPD);

			return tempBool;
		}

#pragma endregion


	};
	/*! @}*/
}

#endif // !__APINODELINK__
