#ifndef __IMS_PACKETSAPI_CORE__
#define __IMS_PACKETSAPI_CORE__
/*! \file  IMS_PacketsAPI_Core.h
	\brief Abstract base of a distributed polymorphic data and execution system

	A core set of structures and classes are declared here to create the abstract
	elements and relationships for an ecosystem of distributed computing nodes,
	interconnected by serial communication links, understanding a common api language, 
	and having common api endpoint functions.  Packets API Core describes abstractly
	the base elements of a message-oriented-middleware for distributed computing systems.

	Each api node inherits from the AbstractDataExecution class.  
	
	Each api node is linked with an application specific number of PolymorphicPacketPort
	class object instances to facilitate sharing of AbstractDataExecution members and functions
	over an application configurable packet communication interface.

	Each application packet port is linked with two PacketInterface object instances, 1 Rx and 1 Tx.
	The PacketInterface object instances link Hardware and Platform specific libraries for
	Physically Reading from the Interface Hardware, Converting to/from the stream's byte or char 
	arrays and the PacketInterface packet buffer.
*/
/*! \defgroup EcosystemRestrictions
	\brief Design Configuration for a derived micro-EcoSystem

	A particular product or application node is fundamentally restricted at compile time by a small
	set of Design Configuration settings.  Any node that "is a" node built from this particular file
	will be:
	- linking the iostream standard library (then optionally using linked standard library)
	- have a common token count that is used to instantiate token buffers
	- have a common token string ratio that is used to instantiate string token buffers
	- have a common ID string token size that is used to instantiate ID string token buffers
	- have a common binary buffer size equal to token count multiplied by token size (configured at application layer)
	- have a common string buffer size equal to the string buffer character count macro
	@{
*/
#include <iostream>
#define PACKETBUFFER_TOKENCOUNT (32)
#define STRINGBUFFER_TOKENRATIO (10)
#define STRINGBUFFER_IDTOKENRATIO (32)
#define STRINGBUFFER_CHARCOUNT ((PACKETBUFFER_TOKENCOUNT-1)*STRINGBUFFER_TOKENRATIO+STRINGBUFFER_IDTOKENRATIO)
/*! @}*/

#define xstr(s) #s
#define str(s) xstr(s)

#define HDRPACK (0)
#define Index_PackID (0)
#define Index_PackLEN (1)
#define Index_PackTYPE (2)
#define Index_PackOPTION (3)
#define HDR_Offset Index_PackOPTION

#define VERSION (1)
#define Index_MajorVersion (HDR_Offset+1)
#define Index_MinorVersion (HDR_Offset+2)
#define Index_BuildNumber (HDR_Offset+3)
#define Index_DevFlag (HDR_Offset+4)

// ASCII Constants for Communication Interfaces
#define ASCII_space             32
#define ASCII_A                 65
#define ASCII_Z                 90
#define ASCII_a                 97
#define ASCII_z                 122
#define ASCII_0                 48
#define ASCII_9                 57
#define ASCII_plus              43
#define ASCII_minus             45
#define ASCII_dot               46
#define ASCII_colon             58
#define ASCII_semicolon         59
#define ASCII_tilda             126

#define TEMPLATE_SPDSET_toVALUE(fID, SPDvar,pID, SPDval_eq_this)\
void set##fID(SPD1* SPDvar){SPDval_eq_this;setSPDat(pID,SPDvar);}\
void set##fID(SPD2* SPDvar){SPDval_eq_this;setSPDat(pID,SPDvar);}\
void set##fID(SPD4* SPDvar){SPDval_eq_this;setSPDat(pID,SPDvar);}\
void set##fID(SPD8* SPDvar){SPDval_eq_this;setSPDat(pID,SPDvar);}\

#define TEMPLATE_SPDSET(fID, SPDvar,pID)\
void set##fID(SPD1* SPDvar){setSPDat(pID,SPDvar);}\
void set##fID(SPD2* SPDvar){setSPDat(pID,SPDvar);}\
void set##fID(SPD4* SPDvar){setSPDat(pID,SPDvar);}\
void set##fID(SPD8* SPDvar){setSPDat(pID,SPDvar);}\

#define TEMPLATE_SPDGET(fID, SPDvar, pID)\
void get##fID(SPD1* SPDvar){getSPDat(pID,SPDvar);}\
void get##fID(SPD2* SPDvar){getSPDat(pID,SPDvar);}\
void get##fID(SPD4* SPDvar){getSPDat(pID,SPDvar);}\
void get##fID(SPD8* SPDvar){getSPDat(pID,SPDvar);}\

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

#define TEMPLATE_TX_PACKAGER(tVar, pType, pID, packFunc){\
if(tVar){\
	pType tempPack;\
	tempPack.CopyTokenBufferPtrs(TxPackOutPtr);\
	packFunc(&tempPack);\
	tVar=false;\
	return true; }\
}
/*! \namespace IMSPacketsAPICore
	\brief Namespace Isolation for the Packets API Core Components

	Useful to avoid name conflicts, Packet (for example) may already be defined in a large
	application space.  Also useful to run multiple instances of polymorphic nodes within
	a single application space.
*/
namespace IMSPacketsAPICore
{	
	/*! \union SPD1
		\brief Data abstraction element

		A 1 byte block, 8 bits, represented in its possible forms
	*/
	
	union SPD1
	{
		int8_t		intVal;
		uint8_t		uintVal;
	};
	/*! \union SPD2
		\brief Data abstraction element

		A 2 byte block, 16 bits, represented in its possible forms
	*/
	union SPD2
	{
		uint8_t		bytes[2];
		short		fpVal;
		int16_t		intVal;
		uint16_t	uintVal;
	};
	/*! \union SPD4
		\brief Data abstraction element

		A 4 byte block, 32 bits, represented in its possible forms
	*/
	union SPD4
	{
		uint8_t		bytes[4];
		float		fpVal;
		int32_t		intVal;
		uint32_t	uintVal;
	};
	/*! \union SPD8
		\brief Data abstraction element

		An 8 byte block, 64 bits, represented in its possible forms
	*/
	union SPD8
	{
		uint8_t		bytes[8];
		double		fpVal;
		int64_t		intVal;
		uint64_t	uintVal;
	};

	template<class TokenType>
	class SPDInterfaceBuffer
	{
	public:
		union  {
			TokenType			SPDs[PACKETBUFFER_TOKENCOUNT];
			uint8_t				bytes[sizeof(TokenType) * PACKETBUFFER_TOKENCOUNT];
		};
	};
	/// <summary>
	/// 
	/// </summary>
	class SPDASCIIInterfaceBuffer
	{
	public:
		char					chars[STRINGBUFFER_CHARCOUNT];
	};

	enum SPDValTypeEnum
	{
		typeUINT,
		typeINT,
		typeFLT
	};
	enum PacketPort_SRCommState
	{
		sr_Init,
		sr_Waiting,
		sr_Sending,
		sr_Sent,
		sr_Reading,
		sr_Handling
	};
	enum PacketPort_FCCommState
	{
		fc_Init,
		fc_Connected
	};
	enum PacketPortPartnerType
	{
		SenderResponder_Responder,
		SenderResponder_Sender,
		FullCylic_Partner
	};
		
	/*!	\class Packet
		\brief An Abstract container for serial parameter data (SPD) objects

		Packets provide data abstraction and api endpoint identification required
		to communicate with common data and execute common api endpoints
		in the distributed computing system.
	*/
	class Packet
	{
	private:
		uint8_t*				bytesBufferPtr		= nullptr;
		char*					charsBufferPtr		= nullptr;
	protected:
		// byte by byte binary exchange of data
		void		getSPDat(int i, SPD1* SPDPtr)
		{
			if(i>-1 && i < getSPDCount())
				SPDPtr->uintVal = (bytesBufferPtr + i)[0];
		}
		void		getSPDat(int i, SPD2* SPDPtr)
		{
			if (i > -1 && i < getSPDCount()) 
			{
				for (int j = 0; j < sizeof(SPD2); j++)
				{
					SPDPtr->bytes[j] = (bytesBufferPtr + (i * sizeof(SPD2)))[j];
				}
			}
		}
		void		getSPDat(int i, SPD4* SPDPtr)
		{
			if (i > -1 && i < getSPDCount())
			{
				for (int j = 0; j < sizeof(SPD4); j++)
				{
					SPDPtr->bytes[j] = (bytesBufferPtr + (i * sizeof(SPD4)))[j];
				}
			}
		}
		void		getSPDat(int i, SPD8* SPDPtr)
		{
			if (i > -1 && i < getSPDCount())
			{
				for (int j = 0; j < sizeof(SPD8); j++)
				{
					SPDPtr->bytes[j] = (bytesBufferPtr + (i * sizeof(SPD8)))[j];
				}
			}
		}
		
		// byte by byte binary exchange of data
		void		setSPDat(int i, SPD1* SPDPtr)
		{
			if (i > -1 && i < getSPDCount())
				(bytesBufferPtr + i)[0] = SPDPtr->uintVal;
		}
		void		setSPDat(int i, SPD2* SPDPtr)
		{
			if (i > -1 && i < getSPDCount())
			{
				for (int j = 0; j < sizeof(SPD2); j++)
				{
					(bytesBufferPtr + i* sizeof(SPD2))[j] = SPDPtr->bytes[j];
				}
			}
				
		}
		void		setSPDat(int i, SPD4* SPDPtr)
		{
			if (i > -1 && i < getSPDCount())
			{
				for (int j = 0; j < sizeof(SPD4); j++)
				{
					(bytesBufferPtr + i*sizeof(SPD4))[j] = SPDPtr->bytes[j];
				}
			}

		}
		void		setSPDat(int i, SPD8* SPDPtr)
		{
			if (i > -1 && i < getSPDCount())
			{
				for (int j = 0; j < sizeof(SPD8); j++)
				{
					(bytesBufferPtr + i*sizeof(SPD8))[j] = SPDPtr->bytes[j];
				}
			}

		}
		
		// atoi, atof, etc called on char buffer to xfer spd
		bool		getSPDfromcharsAt(int i, SPD1* SPDPtr, enum SPDValTypeEnum dType)
		{
			if (i > 0 && i < getSPDCount())
			{
				char* TokenStringPtr = (charsBufferPtr + STRINGBUFFER_IDTOKENRATIO + i * STRINGBUFFER_TOKENRATIO);
				switch (dType)
				{
				case typeUINT:	if(!isUnsignedIntegerString(TokenStringPtr))	return false; SPDPtr->uintVal = atoi(TokenStringPtr); return true;
				case typeINT:	if(!isIntegerString(TokenStringPtr))			return false; SPDPtr->uintVal = atoi(TokenStringPtr); return true;
				}
			}
			return false;
		}
		bool		getSPDfromcharsAt(int i, SPD2* SPDPtr, enum SPDValTypeEnum dType)
		{
			if (i > 0 && i < getSPDCount())
			{
				char* TokenStringPtr = (charsBufferPtr + STRINGBUFFER_IDTOKENRATIO + i * STRINGBUFFER_TOKENRATIO);
				switch (dType)
				{
				case typeUINT:	if (!isUnsignedIntegerString(TokenStringPtr))	return false; SPDPtr->uintVal	= atoi(TokenStringPtr); return true;
				case typeINT:	if (!isIntegerString(TokenStringPtr))			return false; SPDPtr->uintVal	= atoi(TokenStringPtr); return true;
				case typeFLT:	if (!isNumberString(TokenStringPtr))			return false; SPDPtr->fpVal		= atof(TokenStringPtr); return true;
				}
			}
			return false;
		}
		bool		getSPDfromcharsAt(int i, SPD4* SPDPtr, enum SPDValTypeEnum dType)
		{
			if (i > 0 && i < getSPDCount())
			{
				char* TokenStringPtr = (charsBufferPtr + STRINGBUFFER_IDTOKENRATIO + i * STRINGBUFFER_TOKENRATIO);
				switch (dType)
				{
				case typeUINT:	if (!isUnsignedIntegerString(TokenStringPtr))	return false; SPDPtr->uintVal	= atol(TokenStringPtr); return true;
				case typeINT:	if (!isIntegerString(TokenStringPtr))			return false; SPDPtr->uintVal	= atol(TokenStringPtr); return true;
				case typeFLT:	if (!isNumberString(TokenStringPtr))			return false; SPDPtr->fpVal		= atof(TokenStringPtr); return true;
				}
			}
			return false;
		}
		bool		getSPDfromcharsAt(int i, SPD8* SPDPtr, enum SPDValTypeEnum dType)
		{
			if (i > 0 && i < getSPDCount())
			{
				char* TokenStringPtr = (charsBufferPtr + STRINGBUFFER_IDTOKENRATIO + i * STRINGBUFFER_TOKENRATIO);
				switch (dType)
				{
				case typeUINT:	if (!isUnsignedIntegerString(TokenStringPtr))	return false; SPDPtr->uintVal	= atol(TokenStringPtr); return true;
				case typeINT:	if (!isIntegerString(TokenStringPtr))			return false; SPDPtr->uintVal	= atol(TokenStringPtr); return true;
				case typeFLT:	if (!isNumberString(TokenStringPtr))			return false; SPDPtr->fpVal		= atof(TokenStringPtr); return true;
				}
			}
			return false;
		}
		
		// snprintf called on SPDPtr->value according to dType
		bool		setCharsfromSPDat(int i, SPD1* SPDPtr, enum SPDValTypeEnum dType, const char* fString) 
		{ 
			if (i > 0 && i < getSPDCount())
			{
				char* TokenStringPtr = (charsBufferPtr + STRINGBUFFER_IDTOKENRATIO + i * STRINGBUFFER_TOKENRATIO);
				switch (dType)
				{
				case typeUINT:	if (snprintf(TokenStringPtr, STRINGBUFFER_TOKENRATIO, fString, SPDPtr->uintVal)<1)	return false; return true;
				case typeINT:	if (snprintf(TokenStringPtr, STRINGBUFFER_TOKENRATIO, fString, SPDPtr->intVal) < 1) return false; return true;
				}
			}
			return false;
		}
		bool		setCharsfromSPDat(int i, SPD2* SPDPtr, enum SPDValTypeEnum dType, const char* fString)
		{
			if (i > 0 && i < getSPDCount())
			{
				char* TokenStringPtr = (charsBufferPtr + STRINGBUFFER_IDTOKENRATIO + i * STRINGBUFFER_TOKENRATIO);
				switch (dType)
				{
				case typeUINT:	if (snprintf(TokenStringPtr, STRINGBUFFER_TOKENRATIO, fString, SPDPtr->uintVal) < 1)	return false; return true;
				case typeINT:	if (snprintf(TokenStringPtr, STRINGBUFFER_TOKENRATIO, fString, SPDPtr->intVal) < 1)		return false; return true;
				case typeFLT:	if (snprintf(TokenStringPtr, STRINGBUFFER_TOKENRATIO, fString, SPDPtr->fpVal) < 1)		return false; return true;
				}
			}
			return false;
		}
		bool		setCharsfromSPDat(int i, SPD4* SPDPtr, enum SPDValTypeEnum dType, const char* fString)
		{
			if (i > 0 && i < getSPDCount())
			{
				char* TokenStringPtr = (charsBufferPtr + STRINGBUFFER_IDTOKENRATIO + i * STRINGBUFFER_TOKENRATIO);
				switch (dType)
				{
				case typeUINT:	if (snprintf(TokenStringPtr, STRINGBUFFER_TOKENRATIO, fString, SPDPtr->uintVal) < 1)	return false; return true;
				case typeINT:	if (snprintf(TokenStringPtr, STRINGBUFFER_TOKENRATIO, fString, SPDPtr->intVal) < 1)		return false; return true;
				case typeFLT:	if (snprintf(TokenStringPtr, STRINGBUFFER_TOKENRATIO, fString, SPDPtr->fpVal) < 1)		return false; return true;
				}
			}
			return false;
		}
		bool		setCharsfromSPDat(int i, SPD8* SPDPtr, enum SPDValTypeEnum dType, const char* fString)
		{
			if (i > 0 && i < getSPDCount())
			{
				char* TokenStringPtr = (charsBufferPtr + STRINGBUFFER_IDTOKENRATIO + i * STRINGBUFFER_TOKENRATIO);
				switch (dType)
				{
				case typeUINT:	if (snprintf(TokenStringPtr, STRINGBUFFER_TOKENRATIO, fString, SPDPtr->uintVal) < 1)	return false; return true;
				case typeINT:	if (snprintf(TokenStringPtr, STRINGBUFFER_TOKENRATIO, fString, SPDPtr->intVal) < 1)		return false; return true;
				case typeFLT:	if (snprintf(TokenStringPtr, STRINGBUFFER_TOKENRATIO, fString, SPDPtr->fpVal) < 1)		return false; return true;
				}
			}
			return false;
		}

	public:
		uint8_t*				getBytesBuffer() { return bytesBufferPtr; }
		void					setBytesBuffer(uint8_t* bytesBufferPtrIn)			
		{ 
			bytesBufferPtr = bytesBufferPtrIn;
		}
		char*					getCharsBuffer() { return charsBufferPtr; }
		void					setCharsBuffer(char* charsBufferPtrIn)
		{
			charsBufferPtr = charsBufferPtrIn;
		}
		void					CopyTokenBufferPtrs(Packet* copyPacketPtrs)
		{
			bytesBufferPtr = copyPacketPtrs->bytesBufferPtr;
			charsBufferPtr = copyPacketPtrs->charsBufferPtr;
		}
		virtual const char*		getPacketIDString()		const { return ""; }
		virtual int				getSPDCount() { return 0; }
		bool					isASCIIPacket() { return(charsBufferPtr != nullptr); }
		bool					StringBuffer_IDString_Equals(const char* compareStringPtr) { return false; } // TODO :
		bool					ByteBuffer_ID_Equals(const int compareValue) { return false; }					// TODO :
		static bool				isASCIIchar(char inChar) {return ((inChar >= ASCII_space && inChar <= ASCII_tilda));}
		static bool				isLetterchar(char inChar) {return ((inChar >= ASCII_A && inChar <= ASCII_Z) || (inChar >= ASCII_a && inChar <= ASCII_z));}
		static bool				isNumberchar(char inChar) {return ((inChar >= ASCII_0 && inChar <= ASCII_9) || inChar == ASCII_plus || inChar == ASCII_minus || inChar == ASCII_dot);}
		static bool				isIntegerchar(char inChar) { return ((inChar >= ASCII_0 && inChar <= ASCII_9) || inChar == ASCII_plus || inChar == ASCII_minus); }
		static bool				isUnsignedIntegerchar(char inChar) { return ((inChar >= ASCII_0 && inChar <= ASCII_9)); }
		static bool				isDelimiterchar(char inChar) {return (inChar == ASCII_colon);}
		static bool				isTerminatorchar(char inChar) {return (inChar == ASCII_semicolon);}
		static bool				isASCIIString(char* inStringPtr) { int index = 0;  while (inStringPtr[index] != 0x00) if (!isASCIIchar(inStringPtr[index++])) return false; return true; }
		static bool				isLetterString(char* inStringPtr) { int index = 0;  while (inStringPtr[index] != 0x00) if (!isLetterchar(inStringPtr[index++])) return false; return true; }
		static bool				isNumberString(char* inStringPtr) { int index = 0;  while (inStringPtr[index] != 0x00) if (!isNumberchar(inStringPtr[index++])) return false; return true; }
		static bool				isIntegerString(char* inStringPtr) { int index = 0;  while (inStringPtr[index] != 0x00) if (!isIntegerchar(inStringPtr[index++])) return false; return true; }
		static bool				isUnsignedIntegerString(char* inStringPtr) { int index = 0;  while (inStringPtr[index] != 0x00) if (!isUnsignedIntegerchar(inStringPtr[index++])) return false; return true; }
	};
	
	/*! \class PacketInterface
	\brief An Abstraction of the serial interface connecting two api nodes

	A PacketInterface instance links the physical layer and application layer via
	appliation specific serialization/deserialization
	to/from platform specific physical layer drivers using abstract api Packets.

	Bytes are read from and written to physical hardware using devices specific functions.
	Bytes (or Chars) are serialized/deserialized to/from Packet instances by a PacketInterface.
	A PolymorphicPacketPort has two interfaces, 1 input and 1 output.
	*/
	class PacketInterface
	{
	protected:		
		std::iostream*				ifaceStreamPtr;
		std::istream*				ifaceInStreamPtr	= nullptr;
		std::ostream*				ifaceOutStreamPtr	= nullptr;
	
		int					serializedPacketSize		= 0;
		int					tokenIndex					= 0;
		virtual void		CustomWriteTo()				{ ; }
		virtual void		CustomReadFrom()			{ ; }
		virtual void		WriteToStream()				= 0;
		virtual void		ReadFromStream()			= 0;
		
		PacketInterface(std::iostream* ifaceStreamPtrIn)
		{
			ifaceStreamPtr = ifaceStreamPtrIn;
		}
		PacketInterface(std::istream* ifaceInStreamPtrIn)
		{
			ifaceInStreamPtr = ifaceInStreamPtrIn;
		}
		PacketInterface(std::ostream* ifaceOutStreamPtrIn)
		{
			ifaceOutStreamPtr = ifaceOutStreamPtrIn;
		}
	public:		
		virtual int					getTokenSize() = 0;
		virtual Packet*				getPacketPtr() = 0;
		virtual bool				DeSerializePacket() = 0;
		virtual bool				SerializePacket()	= 0;
		//! Abstract Serialize Function
		/*!
			Converts Packet to bytes (or chars) then writes them
			to their stream instance
		*/
		void				WriteTo()
		{
			if (ifaceStreamPtr == nullptr && ifaceOutStreamPtr ==nullptr)
				CustomWriteTo();
			else
			{
				WriteToStream();
			}
		}
		//! Abstract De-Serialize Function
		/*!
			Reads bytes (or chars) from their stream instance then convert to
			Packets in a packet buffer
		*/
		void				ReadFrom()
		{
			if (ifaceStreamPtr == nullptr && ifaceInStreamPtr == nullptr)
				CustomReadFrom();
			else
			{
				ReadFromStream();
			}
		}
	};

	template<class TokenType>
	class PacketInterface_Binary : public PacketInterface
	{
	protected:
		int								ByteIndexLast = 0;
		Packet							BufferPacket;
	
		int								ByteIndex = 0;
		SPDInterfaceBuffer<TokenType>	TokenBuffer;
		
		void WriteToStream()
		{
			if(ifaceStreamPtr!=nullptr)
				ifaceStreamPtr->write(&(TokenBuffer.bytes[0]), serializedPacketSize);
			else if(ifaceOutStreamPtr!=nullptr)
				ifaceOutStreamPtr->write(&(TokenBuffer.bytes[0]), serializedPacketSize);
		}
		void ReadFromStream()
		{
			if (ifaceStreamPtr!=nullptr)
			{
				if (ifaceStreamPtr->peek() != EOF)
					ifaceStreamPtr->read(&(TokenBuffer.bytes[ByteIndex++]), 1);
			}
			else if (ifaceInStreamPtr!=nullptr)
			{
				if (ifaceInStreamPtr->peek() != EOF)
					ifaceInStreamPtr->read(&(TokenBuffer.bytes[ByteIndex++]), 1);
			}			
		}

		int deSerializedPacketSize = 0;
		int deSerializedTokenIndex = 0;
		TokenType deSerializedTokenLength;

		bool DeSerializePacket()
		{
			// called cyclically
			// monitor ByteIndex for change
			if (ByteIndex != ByteIndexLast)
			{
				// analyze byte array
				if ((ByteIndex % getTokenSize()) == 0)
				{
					if (++deSerializedTokenIndex == 2)
						BufferPacket.getSPDat(2, &deSerializedTokenLength);
				}
				
				// decide if error, then reset
				if ()							// TODO:
				{
					deSerializedPacketSize = 0;
					deSerializedTokenIndex = 0;
					deSerializedTokenLength.uintVal = 0;
				}
			}

			// Capture history for change detect
			ByteIndexLast = ByteIndex;

			// decide if complete packet
			// return true or false
			// true will trigger the packet handler of the data execution instance
			if (ByteIndexLast==(deSerializedTokenLength.uintVal-1))
				return true;
			return false;
		}
		bool SerializePacket()
		{
			// called single-shot
			// optionally swap bytes order before sending
			// return true or false
		}
	public:
		Packet* getPacketPtr() { return &BufferPacket; }
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

	template<class TokenType>
	class PacketInterface_ASCII : public PacketInterface
	{
	protected:
		int									CharIndex = 0;
		SPDASCIIInterfaceBuffer				TokenBuffer;
		Packet							BufferPacket;
		void WriteToStream()
		{
			if (ifaceStreamPtr != nullptr)
				ifaceStreamPtr->write(&(TokenBuffer.chars[0]), serializedPacketSize);
			else if (ifaceOutStreamPtr != nullptr)
				ifaceOutStreamPtr->write(&(TokenBuffer.chars[0]), serializedPacketSize);
		}
		void ReadFromStream()
		{
			if (ifaceStreamPtr != nullptr)
			{
				if (ifaceStreamPtr->peek() != EOF)
					ifaceStreamPtr->read(&(TokenBuffer.chars[CharIndex++]), 1);
			}
			else if (ifaceInStreamPtr != nullptr)
			{
				if (ifaceInStreamPtr->peek() != EOF)
					ifaceInStreamPtr->read(&(TokenBuffer.chars[CharIndex++]), 1);
			}

		}
		bool DeSerializePacket()
		{
			// called cyclically
			// monitor CharIndex for change
			// analyze char array
			// decide if complete packet
			// decide if error
			// return true or false
			// true will trigger the packet handler of the data execution instance
		}
		bool SerializePacket()
		{
			// called single-shot
			// return true or false
		}
	public:
		Packet* getPacketPtr() { return &BufferPacket; }
		int					getTokenSize() { return sizeof(TokenType); }
		PacketInterface_ASCII(std::iostream* ifaceStreamPtrIn = nullptr) :
			PacketInterface(ifaceStreamPtrIn) {
			BufferPacket.setBytesBuffer(&(TokenBuffer.chars[0]));
		}
		PacketInterface_ASCII(std::istream* ifaceInStreamPtrIn) :
			PacketInterface(ifaceInStreamPtrIn) {
			BufferPacket.setBytesBuffer(&(TokenBuffer.chars[0]));
		}
		PacketInterface_ASCII(std::ostream* ifaceOutStreamPtrIn) :
			PacketInterface(ifaceOutStreamPtrIn) {
			BufferPacket.setBytesBuffer(&(TokenBuffer.chars[0]));
		}
		
	};
		
	/*! \class AbstractDataExecution
	\brief An Abstraction of the Distributed Data and Execution System

	Provides two public packet handler functions, one to execute on incomming
	packet receipt and another to execute when packaging a packet for transmission.
	An application member must inherit from this base and add application specific
	language, data, and api endpoints.
	*/
	class AbstractDataExecution
	{
	public:
		//! Abstract HandleRxPacket Function
		/*!
			Called by a PacketPort Instance to execute API Endpoints on packet receipt
		*/
		virtual void				HandleRxPacket(Packet* RxPackInPtr) = 0;
		//! Abstract PrepareTxPacket Function
		/*!
			Called by a PacketPort Instance to deque packet objects and write them
			to the PacketInterface buffer
		*/
		virtual bool				PrepareTxPacket(Packet* TxPackOutPtr) = 0;
		//! Abstract Initialization Function
		/*!
			Called by the top level main function of an api node's running application
		*/
		virtual void				Setup() = 0;
		//! Abstract Cyclic Execution Function
		/*!
			Called by the top level main function of an api node's running application.
			Ideally this function will be designed as cyclic and non-blocking, though it
			is a design choice and not a design restriction.
		*/
		virtual void				Loop() = 0;
	};

	/*! \class PolymorphicPacketPort
	\brief An Abstraction of the Distributed Node Link

	Distributed Nodes can exchange shared data structures via a configurable
	serial interface (USB, Ethernet, UART, SPI, IIC, ...) using a PolymorphicPacketPort.

	The PolymorphicPacketPort establishes an abstract link between instances of api nodes
	on either side of a communication link.  It is linked to two instances of PacketInterface
	objects to facilitate (serialization and deserializtion) of packet objects (to and from) a stream
	of (bytes or chars).
	*/
	class PolymorphicPacketPort
	{
	private:
		enum PacketPortPartnerType		PortType			= SenderResponder_Responder;
		PacketInterface*				InputInterface		= nullptr;
		PacketInterface*				OutputInterface		= nullptr;
		AbstractDataExecution*			DataExecution		= nullptr;
		PacketPort_SRCommState			SRCommState			= sr_Init;
		PacketPort_FCCommState			FCCommState			= fc_Init;
	protected:
		void ServicePort_SR_Sender()
		{
			switch (SRCommState)
			{
			case sr_Init: SRCommState = sr_Handling; break;
			case sr_Reading:
				InputInterface->ReadFrom();
				if (InputInterface->DeSerializePacket()) {
					DataExecution->HandleRxPacket(InputInterface->getPacketPtr());
					SRCommState = sr_Handling;
				}
				else
					break;
			case sr_Handling:
				if (DataExecution->PrepareTxPacket(OutputInterface->getPacketPtr()))
					SRCommState = sr_Sending;
				else
					break;
			case sr_Sending:
				if (OutputInterface->SerializePacket()) {
					OutputInterface->WriteTo();
					SRCommState = sr_Sent;
				}
				else
					SRCommState = sr_Init;
				break;
			case sr_Sent:SRCommState = sr_Reading; break;
			}
		}
		void ServicePort_SR_Responder()
		{		
			switch (SRCommState)
			{
			case sr_Init: SRCommState = sr_Reading; break;
			case sr_Reading: 
				InputInterface->ReadFrom(); 
				if (InputInterface->DeSerializePacket()) 
				{
					DataExecution->HandleRxPacket(InputInterface->getPacketPtr());
					SRCommState = sr_Handling;
				}
				else
					break;
			case sr_Handling:
				if (DataExecution->PrepareTxPacket(OutputInterface->getPacketPtr()))
					SRCommState = sr_Sending;
				else
					break;
			case sr_Sending: 
				if(OutputInterface->SerializePacket()){				
					OutputInterface->WriteTo();
					SRCommState = sr_Sent;}
				else
					SRCommState = sr_Init;
				break;
			case sr_Sent:SRCommState = sr_Reading; break;
			}
		}
		void ServicePort_FCP_Partner()
		{
			InputInterface->ReadFrom();
			if (InputInterface->DeSerializePacket())
			{
				DataExecution->HandleRxPacket(InputInterface->getPacketPtr());
			}

			if (DataExecution->PrepareTxPacket(OutputInterface->getPacketPtr()))
			{
				if (OutputInterface->SerializePacket())
					OutputInterface->WriteTo();
			}
		}
	public:
		//! Abstract ServicePort Function
		/*!
			Called cyclically by the loop function of an api node instance.  It Reads/Writes to/from Serial Interfaces
			and calls the Handle/Package functions of the api node instance.
		*/
		void				ServicePort() 
		{ 
			if (InputInterface != nullptr && OutputInterface != nullptr && DataExecution != nullptr)
			{
				switch (PortType)
				{
				case SenderResponder_Responder:		ServicePort_SR_Responder();		break;
				case SenderResponder_Sender:		ServicePort_SR_Sender();		break;
				case FullCylic_Partner:				ServicePort_FCP_Partner();		break;
				}			
			}
		}
		PolymorphicPacketPort(PacketInterface* InputInterfaceIn, PacketInterface* OutputInterfaceIn, AbstractDataExecution* DataExecutionIn)
		{
			InputInterface = InputInterfaceIn;
			OutputInterface = OutputInterfaceIn;
			DataExecution = DataExecutionIn;
		}

	};

	class HDR_Packet :public Packet
	{
	public:
		int						getNumSPDs()			const { return 4; }
		virtual const char*		getPacketIDString()		const { return xstr(HDRPACK); }
		virtual int				getPacketID()			const { return HDRPACK; }

		TEMPLATE_SPDSET_toVALUE(PacketID,		pid, Index_PackID, pid->intVal = getPacketID())

		int						getPacketLength(SPD1 len)const { return (sizeof(SPD1) * getNumSPDs()); }
		int						getPacketLength(SPD2 len)const { return (sizeof(SPD2) * getNumSPDs()); }
		int						getPacketLength(SPD4 len)const { return (sizeof(SPD4) * getNumSPDs()); }
		int						getPacketLength(SPD8 len)const { return (sizeof(SPD8) * getNumSPDs()); }

		TEMPLATE_SPDSET_toVALUE(PacketLength,	len, Index_PackLEN, len->intVal = getPacketLength(*len))

		TEMPLATE_SPDSET(PacketType, ptype, Index_PackTYPE)
		TEMPLATE_SPDGET(PacketType, ptype, Index_PackTYPE)
		TEMPLATE_SPDSET(PacketOption, popt, Index_PackOPTION)
		TEMPLATE_SPDGET(PacketOption, popt, Index_PackOPTION)
	};

	/*! \class ARU_Packet_VERSION
		\brief A Common Versions Packet for ARU FW

	*/

	class Packet_Version :public HDR_Packet
	{
	public:
		int						getNumSPDs()			const { return (HDR_Packet::getNumSPDs() + 4); }
		const char*				getPacketIDString()		const { return xstr(VERSION); }
		int						getPacketID()			const { return VERSION; }

		TEMPLATE_SPDSET(MajorVersion, majVer, Index_MajorVersion)
		TEMPLATE_SPDGET(MajorVersion, majVer, Index_MajorVersion)
		TEMPLATE_SPDSET(MinorVersion, minVer, Index_MinorVersion)
		TEMPLATE_SPDGET(MinorVersion, minVer, Index_MinorVersion)
		TEMPLATE_SPDSET(BuildNumber, bldNum, Index_BuildNumber)
		TEMPLATE_SPDGET(BuildNumber, bldNum, Index_BuildNumber)
		TEMPLATE_SPDSET(DevFlag, DevFlg, Index_DevFlag)
		TEMPLATE_SPDGET(DevFlag, DevFlg, Index_DevFlag)
	};

	class API_NODE :public AbstractDataExecution
	{
	private:
		bool triggerVERSIONPackager = false;
	protected:
		virtual PolymorphicPacketPort* getPacketPort(int i) = 0;
		virtual const int getNumPacketPorts() = 0;
		virtual void CustomLoop() = 0;
		virtual void VERSION_Handler(Packet_Version* inPack) = 0;
		virtual void VERSION_Packager(Packet_Version* outPack) = 0;
		virtual bool API_CustomShared_PrepareTx(Packet* TxPackOutPtr) = 0;
		virtual void API_CustomShared_HandleRx(HDR_Packet* RxPackInPtr) = 0;
	public:
		void Loop()
		{
			CustomLoop();
			for (int i = 0; i < getNumPacketPorts(); i++)
				(getPacketPort(i))->ServicePort();
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
			TEMPLATE_TX_PACKAGER(triggerVERSIONPackager, Packet_Version, VERSION, VERSION_Packager)
		
			return API_CustomShared_PrepareTx(TxPackOutPtr);
		}
	};
} // !IMSPacketsAPICore
#endif //!__IMS_PACKETSAPI_CORE__
