/*! \file  2_PacketPortLink.h
	\brief Abstract Communication Objects

*/

#ifndef __PACKETPORTLINK__
#define __PACKETPORTLINK__
#include "1_LanguageConstructs.h"



namespace IMSPacketsAPICore
{	
	/*! \defgroup PacketPortLink
		\brief Abstract Communication Objects

		An Application Node creates and services packet port objects to exchange tokens of information
		with another node.  Port objects control the execution of input and output packet interface
		objects to exchange data with physical communication layer drivers.  A packet is exchanged
		as an array of tokens stored in a byte buffer.  Packets are exchanged between two Application
		Nodes and Common API Enpionts are called on reciept or before transmission of Packets.

		@{
	*/

	enum PacketTypes
	{
		packType_ReadComplete,
		packType_ReadTokenAt,
		packType_WriteComplete,
		packType_WriteTokenAt,
		packType_ResponseComplete,
		packType_ResponseTokenAt,
		packType_ResponseHDROnly,
		packType_FullCyclicPartner
	};

	/*! \brief Port Communication States for Sender Receiver Operation */
	enum PacketPort_SRCommState
	{
		sr_Init,
		sr_Waiting,
		sr_Sending,
		sr_Sent,
		sr_Reading,
		sr_Handling
	};

	/*! \brief Port Communication States for Full Cyclic Partner Operation */
	enum PacketPort_FCCommState
	{
		fc_Init,
		fc_Connected
	};

	/*! \brief Packet Port Instance Partner Operation Types */
	enum PacketPortPartnerType
	{
		SenderResponder_Responder,
		SenderResponder_Sender,
		FullCylic_Partner
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
		std::iostream*		ifaceStreamPtr			= nullptr;
		std::istream*		ifaceInStreamPtr		= nullptr;
		std::ostream*		ifaceOutStreamPtr		= nullptr;

		int					serializedPacketSize	= 0;
		int					tokenIndex				= 0;
		virtual void		CustomWriteTo() { ; }
		virtual void		CustomReadFrom() { ; }
		virtual void		WriteToStream()			= 0;
		virtual void		ReadFromStream()		= 0;

		// Constructors
		PacketInterface(std::iostream* ifaceStreamPtrIn);
		PacketInterface(std::istream* ifaceInStreamPtrIn);
		PacketInterface(std::ostream* ifaceOutStreamPtrIn);
		
	public:
		int					getPortID();
		virtual int			getTokenSize()		= 0;

		/*! \fn getPacketPtr
			\brief Abstract Accessor Function for Interface Packet Object Pointer
			\sa Packet

			Intended to be defined in derived classes, this function will return
			a pointer to the Abstract Packet object of the Interface Instance.
		*/
		virtual Packet*				getPacketPtr()		= 0;
		virtual bool				DeSerializePacket() = 0;
		virtual bool				SerializePacket()	= 0;
		//! Abstract Serialize Function
		/*!
			Converts Packet to bytes (or chars) then writes them
			to their stream instance
		*/
		void				WriteTo();
		

		//! Abstract De-Serialize Function
		/*!
			Reads bytes (or chars) from their stream instance then convert to
			Packets in a packet buffer
		*/
		void				ReadFrom();
		
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
		virtual void				HandleRxPacket(PolymorphicPacketPort* PackPortPtr) = 0;
		//! Abstract PrepareTxPacket Function
		/*!
			Called by a PacketPort Instance to deque packet objects and write them
			to the PacketInterface buffer
		*/
		virtual bool				PrepareTxPacket(PolymorphicPacketPort* PackPortPtr) = 0;
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

	struct OutPackQueueStruct
	{
		int PackID = -1;
		enum PacketTypes packTYPE;
		int packOPTION = 0;
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
	protected:
		int								OutPackQueueDepth	= 0;
		int								PortID				= 0;
		enum PacketPortPartnerType		PortType			= SenderResponder_Responder;
		PacketInterface*				InputInterface		= nullptr;
		PacketInterface*				OutputInterface		= nullptr;
		AbstractDataExecution*			DataExecution		= nullptr;
		bool							ServiceAsync		= false;
		struct OutPackQueueStruct		OutPacketQueue[PORTOUTPACK_BUFFERLENGTH];

	public:
		PacketInterface* getInputInterface();
		PacketInterface* getOutputInterface();

		int		getPortID();
		bool	getAsyncService();

		virtual bool	isSupportedInPackType(enum PacketTypes packTYPE) = 0;

		void	enQueueOutPacket(int packID, enum PacketTypes packTYPE, int packOPTION = 0);
		void	deQueueOutPacket();

		int		getNextOutPackID();
		enum	PacketTypes getNextOutPackType();
		int		getNextOutPackOption();
		


		//! Cyclic Non-Blocking Function to Service the Packet Port
		/*!
			Called cyclically by the loop function of an api node instance.  It Reads/Writes to/from Serial Interfaces
			and calls the Handle/Package functions of the api node instance.
		*/
		virtual void	ServicePort() = 0;


		PolymorphicPacketPort(int PortIDin, PacketInterface* InputInterfaceIn, PacketInterface* OutputInterfaceIn, AbstractDataExecution* DataExecutionIn, bool isAsync = false);
		

	};
	
	
	class PacketPort_SR_Sender : public PolymorphicPacketPort
	{
	private:
		enum PacketPort_SRCommState SRCommState = sr_Init;
	public:
		PacketPort_SR_Sender(int PortIDin, PacketInterface* InputInterfaceIn, PacketInterface* OutputInterfaceIn, AbstractDataExecution* DataExecutionIn, bool isAsync = false);
		void	ServicePort();
		bool	isSupportedInPackType(enum PacketTypes packTYPE);
	};

	class PacketPort_SR_Responder : public PolymorphicPacketPort
	{
	private:
		enum PacketPort_SRCommState SRCommState = sr_Init;
	public:
		PacketPort_SR_Responder(int PortIDin, PacketInterface* InputInterfaceIn, PacketInterface* OutputInterfaceIn, AbstractDataExecution* DataExecutionIn, bool isAsync = false);
		void	ServicePort();
		bool	isSupportedInPackType(enum PacketTypes packTYPE);
	};
	
	/*! @}*/
}

#endif // !__PACKETPORTLINK__
