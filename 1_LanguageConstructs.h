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



#define TEMPLATE_STATICPACKETINFO_H(packID, numTokens)\
static const int ID = packID;\
static const char IDString[];\
static const int TokenCount = numTokens;\
int getNumSPDs();\
int getPacketID();\
char* getPacketIDString();\


#define TEMPLATE_STATICPACKETINFO_CPP(packID)\
const char Packet_##packID::IDString[] = #packID;\
int Packet_##packID::getNumSPDs(){return TokenCount;}\
int Packet_##packID::getPacketID(){return ID;}\
char* Packet_##packID::getPacketIDString(){return (char*)&IDString[0];}\


#define pCLASS(packIDmacro) Packet_##packIDmacro
#define pSTRUCT(packIDmacro) Struct_##packIDmacro
#define pENUM(packIDmacro) TokenIndex_##packIDmacro


/*! \def TEMPLATE_SPDSET(tokenName, SPDindex)
	\brief Code Template for Packet Accessor (SET) Functions

	This template creates a set of accessor functions overloaded for each
	possible token type, currently:
	-SPD1
	-SPD2
	-SPD4
	-SPD8

*/
#define TEMPLATE_SPDSET_H(tokenName)\
void set##tokenName(SPD1* my##tokenName);\
void set##tokenName(SPD2* my##tokenName);\
void set##tokenName(SPD4* my##tokenName);\
void set##tokenName(SPD8* my##tokenName);\


#define TEMPLATE_SPDSET_CPP(PacketType, tokenName, SPDindex)\
void PacketType::set##tokenName(SPD1* my##tokenName){setSPDat(SPDindex,my##tokenName);}\
void PacketType::set##tokenName(SPD2* my##tokenName){setSPDat(SPDindex,my##tokenName);}\
void PacketType::set##tokenName(SPD4* my##tokenName){setSPDat(SPDindex,my##tokenName);}\
void PacketType::set##tokenName(SPD8* my##tokenName){setSPDat(SPDindex,my##tokenName);}\


/*! \def TEMPLATE_SPDSETSTRING(tokenName, SPDindex, dTypeEnum, formatString)
	\brief Code Template for Packet Accessor (SET) Functions

	This template creates a set of accessor functions overloaded for each
	possible token type, currently:
	-SPD1
	-SPD2
	-SPD4
	-SPD8

*/
#define TEMPLATE_SPDSETSTRING_H(tokenName)\
bool set2String##tokenName(SPD1* my##tokenName);\
bool set2String##tokenName(SPD2* my##tokenName);\
bool set2String##tokenName(SPD4* my##tokenName);\
bool set2String##tokenName(SPD8* my##tokenName);\


#define TEMPLATE_SPDSETSTRING_CPP(PacketType, tokenName, SPDindex, dTypeEnum, formatString)\
bool PacketType::set2String##tokenName(SPD1* my##tokenName){return setCharsfromSPDat(SPDindex,my##tokenName,dTypeEnum,formatString);}\
bool PacketType::set2String##tokenName(SPD2* my##tokenName){return setCharsfromSPDat(SPDindex,my##tokenName,dTypeEnum,formatString);}\
bool PacketType::set2String##tokenName(SPD4* my##tokenName){return setCharsfromSPDat(SPDindex,my##tokenName,dTypeEnum,formatString);}\
bool PacketType::set2String##tokenName(SPD8* my##tokenName){return setCharsfromSPDat(SPDindex,my##tokenName,dTypeEnum,formatString);}\

/*! \def TEMPLATE_SPDGET(tokenName, SPDindex)
	\brief Code Template for Packet Accessor (GET) Functions

	This template creates a set of accessor functions overloaded for each
	possible token type, currently:
	-SPD1
	-SPD2
	-SPD4
	-SPD8

*/
#define TEMPLATE_SPDGET_H(tokenName)\
void get##tokenName(SPD1* my##tokenName);\
void get##tokenName(SPD2* my##tokenName);\
void get##tokenName(SPD4* my##tokenName);\
void get##tokenName(SPD8* my##tokenName);\

#define TEMPLATE_SPDGET_CPP(PacketType, tokenName, SPDindex)\
void PacketType::get##tokenName(SPD1* my##tokenName){getSPDat(SPDindex,my##tokenName);}\
void PacketType::get##tokenName(SPD2* my##tokenName){getSPDat(SPDindex,my##tokenName);}\
void PacketType::get##tokenName(SPD4* my##tokenName){getSPDat(SPDindex,my##tokenName);}\
void PacketType::get##tokenName(SPD8* my##tokenName){getSPDat(SPDindex,my##tokenName);}\


#define TEMPLATE_SPDGETSTRING_H(tokenName)\
bool getfromString##tokenName(SPD1* my##tokenName);\
bool getfromString##tokenName(SPD2* my##tokenName);\
bool getfromString##tokenName(SPD4* my##tokenName);\
bool getfromString##tokenName(SPD8* my##tokenName);\

#define TEMPLATE_SPDGETSTRING_CPP(PacketType, tokenName, SPDindex, dTypeEnum)\
bool PacketType::getfromString##tokenName(SPD1* my##tokenName){return getSPDfromcharsAt(SPDindex,my##tokenName,dTypeEnum);}\
bool PacketType::getfromString##tokenName(SPD2* my##tokenName){return getSPDfromcharsAt(SPDindex,my##tokenName,dTypeEnum);}\
bool PacketType::getfromString##tokenName(SPD4* my##tokenName){return getSPDfromcharsAt(SPDindex,my##tokenName,dTypeEnum);}\
bool PacketType::getfromString##tokenName(SPD8* my##tokenName){return getSPDfromcharsAt(SPDindex,my##tokenName,dTypeEnum);}\


#define TEMPLATE_SPDACCESSORS_H(tokenName)\
TEMPLATE_SPDSET_H(tokenName)\
TEMPLATE_SPDGET_H(tokenName)\
TEMPLATE_SPDSETSTRING_H(tokenName)\
TEMPLATE_SPDGETSTRING_H(tokenName)\


#define TEMPLATE_SPDACCESSORS_CPP(PacketType, tokenName, SPDindex, dTypeEnum, formatString)\
TEMPLATE_SPDSET_CPP(PacketType, tokenName, SPDindex)\
TEMPLATE_SPDGET_CPP(PacketType, tokenName, SPDindex)\
TEMPLATE_SPDSETSTRING_CPP(PacketType, tokenName, SPDindex, dTypeEnum, formatString)\
TEMPLATE_SPDGETSTRING_CPP(PacketType, tokenName, SPDindex, dTypeEnum)\


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
		void		getSPDat(int i, SPD1* SPDPtr);
		void		getSPDat(int i, SPD2* SPDPtr);
		void		getSPDat(int i, SPD4* SPDPtr);
		void		getSPDat(int i, SPD8* SPDPtr);

		// byte by byte binary exchange of data
		void		setSPDat(int i, SPD1* SPDPtr);
		void		setSPDat(int i, SPD2* SPDPtr);
		void		setSPDat(int i, SPD4* SPDPtr);
		void		setSPDat(int i, SPD8* SPDPtr);


		// atoi, atof, etc called on char buffer to xfer spd
		bool		getSPDfromcharsAt(int i, SPD1* SPDPtr, enum SPDValTypeEnum dType);
		bool		getSPDfromcharsAt(int i, SPD2* SPDPtr, enum SPDValTypeEnum dType);
		bool		getSPDfromcharsAt(int i, SPD4* SPDPtr, enum SPDValTypeEnum dType);
		bool		getSPDfromcharsAt(int i, SPD8* SPDPtr, enum SPDValTypeEnum dType);
		
		// snprintf called on SPDPtr->value according to dType
		bool		setCharsfromSPDat(int i, SPD1* SPDPtr, enum SPDValTypeEnum dType, const char* fString);
		bool		setCharsfromSPDat(int i, SPD2* SPDPtr, enum SPDValTypeEnum dType, const char* fString);
		bool		setCharsfromSPDat(int i, SPD4* SPDPtr, enum SPDValTypeEnum dType, const char* fString);
		bool		setCharsfromSPDat(int i, SPD8* SPDPtr, enum SPDValTypeEnum dType, const char* fString);

		


	public:
		uint8_t*	getBytesBuffer();
		void		setBytesBuffer(uint8_t* bytesBufferPtrIn);
		char*		getCharsBuffer();
		void		setCharsBuffer(char* charsBufferPtrIn);
		void		CopyTokenBufferPtrs(Packet* copyPacketPtrs);
				
		virtual int				getPacketID() = 0;
		virtual char*			getPacketIDString() = 0;
		virtual int				getNumSPDs() = 0;

		bool					ByteBuffer_ID_Equals(const int compareValue);
		bool					StringBuffer_IDString_Equals(const char* compareStringPtr);

		// PackID Accessors
		void					writebuff_PackID(SPD1* SPDPtr);
		void					writebuff_PackID(SPD2* SPDPtr);
		void					writebuff_PackID(SPD4* SPDPtr);
		void					writebuff_PackID(SPD8* SPDPtr);
		void					writebuff_PackIDString();
		void					readbuff_PackID(SPD1* SPDPtr);
		void					readbuff_PackID(SPD2* SPDPtr);
		void					readbuff_PackID(SPD4* SPDPtr);
		void					readbuff_PackID(SPD8* SPDPtr);

		// PackLen Accessors
		void					writebuff_PackLength(SPD1* SPDPtr);
		void					writebuff_PackLength(SPD2* SPDPtr);
		void					writebuff_PackLength(SPD4* SPDPtr);
		void					writebuff_PackLength(SPD8* SPDPtr);
		void					writebuff_PackLength(SPD1* SPDPtr, int numTokens);
		void					writebuff_PackLength(SPD2* SPDPtr, int numTokens);
		void					writebuff_PackLength(SPD4* SPDPtr, int numTokens);
		void					writebuff_PackLength(SPD8* SPDPtr, int numTokens);
		void					writebuff_TokenCountString();
		void					writebuff_TokenCountString(int numTokens);
		void					readbuff_PackLength(SPD1* SPDPtr);
		void					readbuff_PackLength(SPD2* SPDPtr);
		void					readbuff_PackLength(SPD4* SPDPtr);
		void					readbuff_PackLength(SPD8* SPDPtr);
		
		
		// String/Char Comparison Functions
		bool					isASCIIPacket();

		static bool				isASCIIchar(char inChar);
		static bool				isLetterchar(char inChar);
		static bool				isNumberchar(char inChar);
		static bool				isIntegerchar(char inChar);
		static bool				isUnsignedIntegerchar(char inChar);
		static bool				isDelimiterchar(char inChar);
		static bool				isTerminatorchar(char inChar);
		static bool				isASCIIString(char* inStringPtr);
		static bool				isLetterString(char* inStringPtr);
		static bool				isNumberString(char* inStringPtr);
		static bool				isIntegerString(char* inStringPtr);
		static bool				isUnsignedIntegerString(char* inStringPtr);
		static bool				stringMatchCaseSensitive(char* inStringPtr, const char* matchString);

	};

}

#endif // !__LANGUAGECONSTRUCTS__
