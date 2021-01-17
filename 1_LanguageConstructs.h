/*! \file  1_LanguageConstructs.h
	\brief Fundamental Elements of Packet Communication

*/

#ifndef __LANGUAGECONSTRUCTS__
#define __LANGUAGECONSTRUCTS__
#include "0_EcoSystemRestrictions.h"

/*! \defgroup LanguageConstructs
	\brief Fundamental Elements of Packet Communications

	An abstract communication link exists between two nodes.  An exchange of information between the two
	nodes is possible when the two nodes understand common languages and actions.  By defining a set of
	language constructs, both nodes and communication components can be built from the common definitions.

	IMS Packets Core relies on a few communication components to exchange information between nodes.
	- A Packet can exhange data tokens to/from buffers with an application node
	- A Packet is defined with message specific data model information
	- A Packet is an interface to a buffer of data
	- A Packet is an interface to an array of tokens
	- Tokens are the fundamental element of information exchange
	- Tokens provide a block size for buffer allocations and encoding strategies
	- Serial Parameter Data (SPD) is a type of data token ranging from 1 to 8 bytes in size
	- Packet Ports are serviced (cyclic and non-blocking) by an application node execution system
	- Packet Ports are created by an application node and contain 2 Packet Interfaces
	- Packet Interfaces are linked to Packet Ports when created by an application node
	- Packet Interfaces contain a physical token buffer, 1 each interface
	- Packet Interfaces define and execute
	-- read(),
	-- write(),
	-- serialize(), and
	-- deserialize() functions on token data
	- Packet Interfaces link the to the physical communication layer

	IMS Packets Core relies on a packet structure with header information followed by payload data.
	All packets begin with:
	- Packet ID token, followed by a
	- Packet Length token, followed by a
	- Packet Type token, followed by a
	- Packet Option token

	An Application Node creates and services packet port objects to exchange tokens of information
	with another node.  Port objects control the execution of input and output packet interface
	objects to exchange data with physical communication layer drivers.  A packet is exchanged
	as an array of tokens stored in a byte buffer.  Packets are exchanged between two Application
	Nodes and Common API Enpionts are called on reciept or before transmission of Packets.

	Languanges are defined by deriving new Packet types.  Deriving a new Packet type involves:
	- Setting Token Index Constants and
	- Implementing Template Code for Token Accessors.

	The VERSION Packet is provided as an example implemenation and useful
	tool to get the version of IMS Core Source by default.  Overriding
	the API enpoint for the VERSION Packet on an application node will override the default action.
	- Overriding Application Node API Endpoint functions and
	- Overloading Packet Accessor functions for Token types

	are two means of personalizing the Packets Core to a specific application.

	Template macros are provided to simplify implementation and reduce error when defining overloaded accessor functions.
	- TEMPLATE_SPDSET(tokenName, SPDvar,SPDindex)
	- TEMPLATE_SPDGET(tokenName, SPDvar, SPDindex)

	Template macros are provided to simplify implementation and reduce error when defining overridden api endpoint functions.
	- TEMPLATE_RX_HANDLER(tempHDRPack,Packet_Type, TokenID, HandlerFunc)
	- TEMPLATE_TX_PACKAGER(tVar, pType, SPDindex, packFunc)

	@{
*/

/*! \def Index_PackID
	\brief Token Index for Packet ID Token
*/
#define Index_PackID (0)


/*! \def Index_PackLEN
	\brief Token Index for Packet Length Token
*/
#define Index_PackLEN (1)


#define TEMPLATE_STATICPACKETINFO(packID, numTokens)\
static const int ID_##packID = packID;\
static const char IDString_##packID[] = #packID;\
static const int TokenCount_##packID = numTokens;\

#define TEMPLATE_STATICPACKETINFOHEADER(packID, numTokens)\
static const int ID_##packID;\
static const char IDString_##packID[];\
static const int TokenCount_##packID;\

#define TEMPLATE_STATICPACKETINFOSOURCE(packID, numTokens)\
static const int ID_##packID = packID;\
static const char IDString_##packID[] = #packID;\
static const int TokenCount_##packID = numTokens;\

// String-ize macros
#define xstr(s) #s
#define str(s) xstr(s)
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
#define ASCII_lf				10
#define ASCII_cr				13
#define ASCII_tab				9



/*! \def TEMPLATE_SPDSET(tokenName, SPDindex)
	\brief Code Template for Packet Accessor (SET) Functions

	This template creates a set of accessor functions overloaded for each
	possible token type, currently:
	-SPD1
	-SPD2
	-SPD4
	-SPD8

*/
#define TEMPLATE_SPDSET(tokenName, SPDindex)\
void set##tokenName(SPD1* my##tokenName){setSPDat(SPDindex,my##tokenName);}\
void set##tokenName(SPD2* my##tokenName){setSPDat(SPDindex,my##tokenName);}\
void set##tokenName(SPD4* my##tokenName){setSPDat(SPDindex,my##tokenName);}\
void set##tokenName(SPD8* my##tokenName){setSPDat(SPDindex,my##tokenName);}\

/*! \def TEMPLATE_SPDSETSTRING(tokenName, SPDindex, dTypeEnum, formatString)
	\brief Code Template for Packet Accessor (SET) Functions

	This template creates a set of accessor functions overloaded for each
	possible token type, currently:
	-SPD1
	-SPD2
	-SPD4
	-SPD8

*/
#define TEMPLATE_SPDSETSTRING(tokenName, SPDindex, dTypeEnum, formatString)\
bool set2String##tokenName(SPD1* my##tokenName){return setCharsfromSPDat(SPDindex,my##tokenName,dTypeEnum,formatString);}\
bool set2String##tokenName(SPD2* my##tokenName){return setCharsfromSPDat(SPDindex,my##tokenName,dTypeEnum,formatString);}\
bool set2String##tokenName(SPD4* my##tokenName){return setCharsfromSPDat(SPDindex,my##tokenName,dTypeEnum,formatString);}\
bool set2String##tokenName(SPD8* my##tokenName){return setCharsfromSPDat(SPDindex,my##tokenName,dTypeEnum,formatString);}\

/*! \def TEMPLATE_SPDGET(tokenName, SPDindex)
	\brief Code Template for Packet Accessor (GET) Functions

	This template creates a set of accessor functions overloaded for each
	possible token type, currently:
	-SPD1
	-SPD2
	-SPD4
	-SPD8

*/
#define TEMPLATE_SPDGET(tokenName, SPDindex)\
void get##tokenName(SPD1* my##tokenName){getSPDat(SPDindex,my##tokenName);}\
void get##tokenName(SPD2* my##tokenName){getSPDat(SPDindex,my##tokenName);}\
void get##tokenName(SPD4* my##tokenName){getSPDat(SPDindex,my##tokenName);}\
void get##tokenName(SPD8* my##tokenName){getSPDat(SPDindex,my##tokenName);}\



#define TEMPLATE_SPDGETSTRING(tokenName, SPDindex, dTypeEnum)\
bool getfromString##tokenName(SPD1* my##tokenName){return getSPDfromcharsAt(SPDindex,my##tokenName,dTypeEnum);}\
bool getfromString##tokenName(SPD2* my##tokenName){return getSPDfromcharsAt(SPDindex,my##tokenName,dTypeEnum);}\
bool getfromString##tokenName(SPD4* my##tokenName){return getSPDfromcharsAt(SPDindex,my##tokenName,dTypeEnum);}\
bool getfromString##tokenName(SPD8* my##tokenName){return getSPDfromcharsAt(SPDindex,my##tokenName,dTypeEnum);}\



#define TEMPLATE_SPDACCESSORS(tokenName, SPDindex, dTypeEnum, formatString)\
TEMPLATE_SPDSET(tokenName, SPDindex)\
TEMPLATE_SPDGET(tokenName, SPDindex)\
TEMPLATE_SPDSETSTRING(tokenName, SPDindex, dTypeEnum, formatString)\
TEMPLATE_SPDGETSTRING(tokenName, SPDindex, dTypeEnum)\


/*! @} */

namespace IMSPacketsAPICore
{
	/*! \brief SPD data type encoding
	*/
	enum SPDValTypeEnum
	{
		typeUINT,
		typeINT,
		typeFLT
	};
	/*! \union SPD1
		\brief Data abstraction element
		\ingroup LanguageConstructs

		A 1 byte block, 8 bits, represented in its possible forms
	*/
	union SPD1
	{
		int8_t		intVal;
		uint8_t		uintVal;
	};
	/*! \union SPD2
		\brief Data abstraction element
		\ingroup LanguageConstructs

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
		\ingroup LanguageConstructs

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
		\ingroup LanguageConstructs

		An 8 byte block, 64 bits, represented in its possible forms
	*/
	union SPD8
	{
		uint8_t		bytes[8];
		double		fpVal;
		int64_t		intVal;
		uint64_t	uintVal;
	};


	/*! \class SPDInterfaceBuffer
		\brief template class for binary token buffers
		\ingroup LanguageConstructs

		Here data space is allocated for token buffers
	*/
	template<class TokenType>
	class SPDInterfaceBuffer
	{
	public:
		union {
			TokenType			SPDs[PACKETBUFFER_TOKENCOUNT];
			uint8_t				bytes[sizeof(TokenType) * PACKETBUFFER_TOKENCOUNT];
		};
	};
	
	
	/*! \class SPDASCIIInterfaceBuffer
		\brief class for string token buffers
		\ingroup LanguageConstructs

		Here data space is allocated for token buffers
	*/
	class SPDASCIIInterfaceBuffer
	{
	public:
		char					chars[STRINGBUFFER_CHARCOUNT];
	};
	
	
	/*!	\class Packet
		\brief A structured interface to a token buffer
		\ingroup LanguageConstructs

		Packets provide data abstraction and api endpoint identification required
		to communicate with common data and execute common api endpoints
		in the distributed computing system.
	*/
	class Packet
	{
	private:
		uint8_t*	bytesBufferPtr = nullptr;
		char*		charsBufferPtr = nullptr;
	protected:
		// byte by byte binary exchange of data
		void		getSPDat(int i, SPD1* SPDPtr)
		{
			if (i > -1 && i < getNumSPDs())
				SPDPtr->uintVal = (bytesBufferPtr + i)[0];
		}
		void		getSPDat(int i, SPD2* SPDPtr)
		{
			if (i > -1 && i < getNumSPDs())
			{
				for (int j = 0; j < sizeof(SPD2); j++)
				{
					SPDPtr->bytes[j] = (bytesBufferPtr + (i * sizeof(SPD2)))[j];
				}
			}
		}
		void		getSPDat(int i, SPD4* SPDPtr)
		{
			if (i > -1 && i < getNumSPDs())
			{
				for (int j = 0; j < sizeof(SPD4); j++)
				{
					SPDPtr->bytes[j] = (bytesBufferPtr + (i * sizeof(SPD4)))[j];
				}
			}
		}
		void		getSPDat(int i, SPD8* SPDPtr)
		{
			if (i > -1 && i < getNumSPDs())
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
			if (i > -1 && i < getNumSPDs())
				(bytesBufferPtr + i)[0] = SPDPtr->uintVal;
		}
		void		setSPDat(int i, SPD2* SPDPtr)
		{
			if (i > -1 && i < getNumSPDs())
			{
				for (int j = 0; j < sizeof(SPD2); j++)
				{
					(bytesBufferPtr + i * sizeof(SPD2))[j] = SPDPtr->bytes[j];
				}
			}

		}
		void		setSPDat(int i, SPD4* SPDPtr)
		{
			if (i > -1 && i < getNumSPDs())
			{
				for (int j = 0; j < sizeof(SPD4); j++)
				{
					(bytesBufferPtr + i * sizeof(SPD4))[j] = SPDPtr->bytes[j];
				}
			}

		}
		void		setSPDat(int i, SPD8* SPDPtr)
		{
			if (i > -1 && i < getNumSPDs())
			{
				for (int j = 0; j < sizeof(SPD8); j++)
				{
					(bytesBufferPtr + i * sizeof(SPD8))[j] = SPDPtr->bytes[j];
				}
			}

		}

		// atoi, atof, etc called on char buffer to xfer spd
		bool		getSPDfromcharsAt(int i, SPD1* SPDPtr, enum SPDValTypeEnum dType)
		{
			if (i > 0 && i < getNumSPDs())
			{
				char* TokenStringPtr = (charsBufferPtr + STRINGBUFFER_IDTOKENRATIO + (i-1) * STRINGBUFFER_TOKENRATIO);
				switch (dType)
				{
				case typeUINT:	if (!isUnsignedIntegerString(TokenStringPtr))	return false; SPDPtr->uintVal = atoi(TokenStringPtr); return true;
				case typeINT:	if (!isIntegerString(TokenStringPtr))			return false; SPDPtr->uintVal = atoi(TokenStringPtr); return true;
				}
			}
			return false;
		}
		bool		getSPDfromcharsAt(int i, SPD2* SPDPtr, enum SPDValTypeEnum dType)
		{
			if (i > 0 && i < getNumSPDs())
			{
				char* TokenStringPtr = (charsBufferPtr + STRINGBUFFER_IDTOKENRATIO + (i - 1) * STRINGBUFFER_TOKENRATIO);
				switch (dType)
				{
				case typeUINT:	if (!isUnsignedIntegerString(TokenStringPtr))	return false; SPDPtr->uintVal = atoi(TokenStringPtr); return true;
				case typeINT:	if (!isIntegerString(TokenStringPtr))			return false; SPDPtr->uintVal = atoi(TokenStringPtr); return true;
				case typeFLT:	if (!isNumberString(TokenStringPtr))			return false; SPDPtr->fpVal = atof(TokenStringPtr); return true;
				}
			}
			return false;
		}
		bool		getSPDfromcharsAt(int i, SPD4* SPDPtr, enum SPDValTypeEnum dType)
		{
			if (i > 0 && i < getNumSPDs())
			{
				char* TokenStringPtr = (charsBufferPtr + STRINGBUFFER_IDTOKENRATIO + (i - 1) * STRINGBUFFER_TOKENRATIO);
				switch (dType)
				{
				case typeUINT:	if (!isUnsignedIntegerString(TokenStringPtr))	return false; SPDPtr->uintVal = atol(TokenStringPtr); return true;
				case typeINT:	if (!isIntegerString(TokenStringPtr))			return false; SPDPtr->uintVal = atol(TokenStringPtr); return true;
				case typeFLT:	if (!isNumberString(TokenStringPtr))			return false; SPDPtr->fpVal = atof(TokenStringPtr); return true;
				}
			}
			return false;
		}
		bool		getSPDfromcharsAt(int i, SPD8* SPDPtr, enum SPDValTypeEnum dType)
		{
			if (i > 0 && i < getNumSPDs())
			{
				char* TokenStringPtr = (charsBufferPtr + STRINGBUFFER_IDTOKENRATIO + (i - 1) * STRINGBUFFER_TOKENRATIO);
				switch (dType)
				{
				case typeUINT:	if (!isUnsignedIntegerString(TokenStringPtr))	return false; SPDPtr->uintVal = atol(TokenStringPtr); return true;
				case typeINT:	if (!isIntegerString(TokenStringPtr))			return false; SPDPtr->uintVal = atol(TokenStringPtr); return true;
				case typeFLT:	if (!isNumberString(TokenStringPtr))			return false; SPDPtr->fpVal = atof(TokenStringPtr); return true;
				}
			}
			return false;
		}

		// snprintf called on SPDPtr->value according to dType
		bool		setCharsfromSPDat(int i, SPD1* SPDPtr, enum SPDValTypeEnum dType, const char* fString)
		{
			if (i > 0 && i < getNumSPDs())
			{
				char* TokenStringPtr = (charsBufferPtr + STRINGBUFFER_IDTOKENRATIO + (i - 1) * STRINGBUFFER_TOKENRATIO);
				switch (dType)
				{
				case typeUINT:	if (snprintf(TokenStringPtr, STRINGBUFFER_TOKENRATIO, fString, SPDPtr->uintVal) < 1)	return false; return true;
				case typeINT:	if (snprintf(TokenStringPtr, STRINGBUFFER_TOKENRATIO, fString, SPDPtr->intVal) < 1) return false; return true;
				}
			}
			return false;
		}
		bool		setCharsfromSPDat(int i, SPD2* SPDPtr, enum SPDValTypeEnum dType, const char* fString)
		{
			if (i > 0 && i < getNumSPDs())
			{
				char* TokenStringPtr = (charsBufferPtr + STRINGBUFFER_IDTOKENRATIO + (i - 1) * STRINGBUFFER_TOKENRATIO);
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
			if (i > 0 && i < getNumSPDs())
			{
				char* TokenStringPtr = (charsBufferPtr + STRINGBUFFER_IDTOKENRATIO + (i - 1) * STRINGBUFFER_TOKENRATIO);
				switch (dType)
				{
				case typeUINT:	if (snprintf(TokenStringPtr, STRINGBUFFER_TOKENRATIO, fString, SPDPtr->uintVal) < 1)	return false; return true;
				case typeINT:
					if (snprintf(TokenStringPtr, STRINGBUFFER_TOKENRATIO, fString, SPDPtr->intVal) < 1)		
						return false; 
					return true;
				case typeFLT:	if (snprintf(TokenStringPtr, STRINGBUFFER_TOKENRATIO, fString, SPDPtr->fpVal) < 1)		return false; return true;
				}
			}
			return false;
		}
		bool		setCharsfromSPDat(int i, SPD8* SPDPtr, enum SPDValTypeEnum dType, const char* fString)
		{
			if (i > 0 && i < getNumSPDs())
			{
				char* TokenStringPtr = (charsBufferPtr + STRINGBUFFER_IDTOKENRATIO + (i - 1) * STRINGBUFFER_TOKENRATIO);
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
		
		
		virtual int				getPacketID() = 0;
		virtual char*			getPacketIDString() = 0;
		virtual int				getNumSPDs() = 0;

		bool					ByteBuffer_ID_Equals(const int compareValue)
		{
			return (getNumSPDs() == compareValue);
		}
		bool					StringBuffer_IDString_Equals(const char* compareStringPtr)
		{
			for (int i = 0; i < STRINGBUFFER_IDTOKENRATIO; i++)
			{
				if (compareStringPtr[i] != charsBufferPtr[i])
					return false;
				if (charsBufferPtr[i] == 0x00)
					break;
			}
			return true;
		}


		void					writebuff_PackID(SPD1* SPDPtr)	{ SPDPtr->intVal = getPacketID(); setSPDat(Index_PackID, SPDPtr); }
		void					writebuff_PackID(SPD2* SPDPtr)	{ SPDPtr->intVal = getPacketID(); setSPDat(Index_PackID, SPDPtr); }
		void					writebuff_PackID(SPD4* SPDPtr)	{ SPDPtr->intVal = getPacketID(); setSPDat(Index_PackID, SPDPtr); }
		void					writebuff_PackID(SPD8* SPDPtr)	{ SPDPtr->intVal = getPacketID(); setSPDat(Index_PackID, SPDPtr); }
		void					writebuff_PackIDString()
		{
			for (int i = 0; i < STRINGBUFFER_IDTOKENRATIO; i++)
			{
				charsBufferPtr[i] = getPacketIDString()[i];
				if (charsBufferPtr[i] == 0x00)
					break;
			}
		}
		void					readbuff_PackID(SPD1* SPDPtr)	{ getSPDat(Index_PackID, SPDPtr); }
		void					readbuff_PackID(SPD2* SPDPtr)	{ getSPDat(Index_PackID, SPDPtr); }
		void					readbuff_PackID(SPD4* SPDPtr)	{ getSPDat(Index_PackID, SPDPtr); }
		void					readbuff_PackID(SPD8* SPDPtr)	{ getSPDat(Index_PackID, SPDPtr); }
		

		
		void					writebuff_PackLength(SPD1* SPDPtr)	{ SPDPtr->intVal = sizeof(SPD1) * getNumSPDs(); setSPDat(Index_PackLEN, SPDPtr); }
		void					writebuff_PackLength(SPD2* SPDPtr)	{ SPDPtr->intVal = sizeof(SPD2) * getNumSPDs(); setSPDat(Index_PackLEN, SPDPtr); }
		void					writebuff_PackLength(SPD4* SPDPtr)	{ SPDPtr->intVal = sizeof(SPD4) * getNumSPDs(); setSPDat(Index_PackLEN, SPDPtr); }
		void					writebuff_PackLength(SPD8* SPDPtr)	{ SPDPtr->intVal = sizeof(SPD8) * getNumSPDs(); setSPDat(Index_PackLEN, SPDPtr); }
		void					writebuff_TokenCountString()		{ snprintf(charsBufferPtr + STRINGBUFFER_IDTOKENRATIO, STRINGBUFFER_TOKENRATIO, "%d", getNumSPDs()); }
		void					readbuff_PackLength(SPD1* SPDPtr) { getSPDat(Index_PackLEN, SPDPtr); }
		void					readbuff_PackLength(SPD2* SPDPtr) { getSPDat(Index_PackLEN, SPDPtr); }
		void					readbuff_PackLength(SPD4* SPDPtr) { getSPDat(Index_PackLEN, SPDPtr); }
		void					readbuff_PackLength(SPD8* SPDPtr) { getSPDat(Index_PackLEN, SPDPtr); }


		bool					isASCIIPacket() { return(charsBufferPtr != nullptr); }


		static bool				isASCIIchar(char inChar)					{ return ((inChar >= ASCII_space && inChar <= ASCII_tilda)||inChar==ASCII_lf||inChar==ASCII_cr||inChar==ASCII_tab||inChar==0x00); }
		static bool				isLetterchar(char inChar)					{ return ((inChar >= ASCII_A && inChar <= ASCII_Z) || (inChar >= ASCII_a && inChar <= ASCII_z)); }
		static bool				isNumberchar(char inChar)					{ return ((inChar >= ASCII_0 && inChar <= ASCII_9) || inChar == ASCII_plus || inChar == ASCII_minus || inChar == ASCII_dot); }
		static bool				isIntegerchar(char inChar)					{ return ((inChar >= ASCII_0 && inChar <= ASCII_9) || inChar == ASCII_plus || inChar == ASCII_minus); }
		static bool				isUnsignedIntegerchar(char inChar)			{ return ((inChar >= ASCII_0 && inChar <= ASCII_9)); }
		static bool				isDelimiterchar(char inChar)				{ return (inChar == ASCII_colon); }
		static bool				isTerminatorchar(char inChar)				{ return (inChar == ASCII_semicolon); }
		static bool				isASCIIString(char* inStringPtr)			{ int index = 0;  while (inStringPtr[index] != 0x00) if (!isASCIIchar(inStringPtr[index++])) return false; return true; }
		static bool				isLetterString(char* inStringPtr)			{ int index = 0;  while (inStringPtr[index] != 0x00) if (!isLetterchar(inStringPtr[index++])) return false; return true; }
		static bool				isNumberString(char* inStringPtr)			{ int index = 0;  while (inStringPtr[index] != 0x00) if (!isNumberchar(inStringPtr[index++])) return false; return true; }
		static bool				isIntegerString(char* inStringPtr)			{ int index = 0;  while (inStringPtr[index] != 0x00) if (!isIntegerchar(inStringPtr[index++])) return false; return true; }
		static bool				isUnsignedIntegerString(char* inStringPtr)	{ int index = 0;  while (inStringPtr[index] != 0x00) if (!isUnsignedIntegerchar(inStringPtr[index++])) return false; return true; }
	};

}

#endif // !__LANGUAGECONSTRUCTS__
