#include "2_PacketPortLink.h"
using namespace IMSPacketsAPICore;


#pragma region PacketInterface Implementation
PacketInterface::PacketInterface(std::iostream* ifaceStreamPtrIn)
{
	ifaceStreamPtr = ifaceStreamPtrIn;
}
PacketInterface::PacketInterface(std::istream* ifaceInStreamPtrIn)
{
	ifaceInStreamPtr = ifaceInStreamPtrIn;
}
PacketInterface::PacketInterface(std::ostream* ifaceOutStreamPtrIn)
{
	ifaceOutStreamPtr = ifaceOutStreamPtrIn;
}
void	PacketInterface::WriteTo()
{
	if (ifaceStreamPtr == nullptr && ifaceOutStreamPtr == nullptr)
		CustomWriteTo();
	else
	{
		WriteToStream();
	}
}
void	PacketInterface::ReadFrom()
{
	if (ifaceStreamPtr == nullptr && ifaceInStreamPtr == nullptr)
	{
		CustomReadFrom();
	}
	else
	{
		ReadFromStream();
	}
}

#pragma endregion


#pragma region PolymorphicPacketPort Implementation
PacketInterface* PolymorphicPacketPort::getInputInterface()
{
	return InputInterface;
}
PacketInterface* PolymorphicPacketPort::getOutputInterface()
{
	return OutputInterface;
}
int PolymorphicPacketPort::getPortID() { return PortID; }
bool PolymorphicPacketPort::getAsyncService() { return ServiceAsync; }
void PolymorphicPacketPort::enQueueOutPacket(int packID, enum PacketTypes packTYPE, int packOPTION)
{
	if (packID > -1 && OutPackQueueDepth < PORTOUTPACK_BUFFERLENGTH)
	{
		OutPacketQueue[OutPackQueueDepth].PackID = packID;
		OutPacketQueue[OutPackQueueDepth].packTYPE = packTYPE;
		OutPacketQueue[OutPackQueueDepth].packOPTION = packOPTION;
		OutPackQueueDepth++;
	}
}
void PolymorphicPacketPort::deQueueOutPacket()
{
	for (int i = 1; i <= OutPackQueueDepth; i++)
	{
		OutPacketQueue[i - 1].PackID = OutPacketQueue[i].PackID;
		OutPacketQueue[i - 1].packTYPE = OutPacketQueue[i].packTYPE;
		OutPacketQueue[i - 1].packOPTION = OutPacketQueue[i].packOPTION;
	}
	if (OutPackQueueDepth > 0)
	{
		OutPacketQueue[OutPackQueueDepth].PackID = -1;
		OutPackQueueDepth--;
	}
}
int PolymorphicPacketPort::getNextOutPackID()
{
	return OutPacketQueue[0].PackID;
}
enum PacketTypes PolymorphicPacketPort::getNextOutPackType()
{
	return OutPacketQueue[0].packTYPE;
}
int PolymorphicPacketPort::getNextOutPackOption()
{
	return OutPacketQueue[0].packOPTION;
}
int PolymorphicPacketPort::getOutPackQueueDepth()
{
	return OutPackQueueDepth;
}

PolymorphicPacketPort::PolymorphicPacketPort(int PortIDin, PacketInterface* InputInterfaceIn, PacketInterface* OutputInterfaceIn, AbstractDataExecution* DataExecutionIn, bool isAsync)
{
	InputInterface = InputInterfaceIn;
	OutputInterface = OutputInterfaceIn;
	DataExecution = DataExecutionIn;
	ServiceAsync = isAsync;
	PortID = PortIDin;
	for (int i = 0; i < PORTOUTPACK_BUFFERLENGTH; i++)
		OutPacketQueue[i].PackID = -1;
}

#pragma endregion

#pragma region PacketPort_SR_Sender Implementation
PacketPort_SR_Sender::PacketPort_SR_Sender(int PortIDin, PacketInterface* InputInterfaceIn, PacketInterface* OutputInterfaceIn, AbstractDataExecution* DataExecutionIn, int CyclesResetIn, bool isAsync) :
	PolymorphicPacketPort(PortIDin, InputInterfaceIn, OutputInterfaceIn, DataExecutionIn, isAsync)
{
	PortType = SenderResponder_Sender;
	CyclestoReset = CyclesResetIn;
}
void	PacketPort_SR_Sender::ServicePort()
{
	switch (SRCommState)
	{
	case sr_Init: SRCommState = sr_Handling; break;
	case sr_Reading:
		InputInterface->ReadFrom();
		if (InputInterface->DeSerializePacket()) {
			DataExecution->HandleRxPacket(this);
			SRCommState = sr_Handling;
			CyclesSinceReset = 0;
		}
		else
		{ 
			if (++CyclesSinceReset > CyclestoReset)
			{
				CyclesSinceReset = 0;
				ResetStateMachine();
			}				
			break;
		}
			
	case sr_Handling:
		if (DataExecution->PrepareTxPacket(this))
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
bool	PacketPort_SR_Sender::isSupportedInPackType(enum PacketTypes packTYPE)
{
	return (packTYPE == packType_ResponseComplete || packTYPE == packType_ResponseHDROnly);
}
void	PacketPort_SR_Sender::ResetStateMachine()
{
	SRCommState = sr_Init;
}
#pragma endregion

#pragma region PacketPort_SR_Responder Implementation
PacketPort_SR_Responder::PacketPort_SR_Responder(int PortIDin, PacketInterface* InputInterfaceIn, PacketInterface* OutputInterfaceIn, AbstractDataExecution* DataExecutionIn, bool isAsync):
	PolymorphicPacketPort(PortIDin, InputInterfaceIn, OutputInterfaceIn, DataExecutionIn, isAsync)
{
	PortType = SenderResponder_Responder;
}
void	PacketPort_SR_Responder::ServicePort()
{
	switch (SRCommState)
	{
	case sr_Init: SRCommState = sr_Reading; break;
	case sr_Reading:
		InputInterface->ReadFrom();
		if (InputInterface->DeSerializePacket())
		{
			DataExecution->HandleRxPacket(this);
			SRCommState = sr_Handling;
		}
		else
			break;
	case sr_Handling:
		if (DataExecution->PrepareTxPacket(this))
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
bool	PacketPort_SR_Responder::isSupportedInPackType(enum PacketTypes packTYPE)
{
	return (packTYPE == packType_WriteComplete || packTYPE == packType_ReadComplete);
}
void	PacketPort_SR_Responder::ResetStateMachine()
{
	SRCommState = sr_Init;
}
#pragma endregion

#pragma region PacketPort_FileSystem Implementation
PacketPort_FileSystem::PacketPort_FileSystem(int PortIDin, PacketInterface* InputInterfaceIn, PacketInterface* OutputInterfaceIn, AbstractDataExecution* DataExecutionIn, bool isAsync) :
	PolymorphicPacketPort(PortIDin, InputInterfaceIn, OutputInterfaceIn, DataExecutionIn, isAsync)
{
	PortType = FileSystem_Port;
}
void	PacketPort_FileSystem::ServicePort()
{
	switch (FS_State)
	{
	case fs_Init: // Do Nothing
		;
		break;
	case fs_Reading:
		InputInterface->ReadFrom();
		if (InputInterface->DeSerializePacket()) {
			DataExecution->HandleRxPacket(this);

			CyclesSinceReset = 0;
		}
		else
		{
			if(++CyclesSinceReset>CyclestoReset)
				ResetStateMachine();
		}
		break;
	case fs_Writing:
		if (DataExecution->PrepareTxPacket(this))
		{
			if (OutputInterface->SerializePacket()) {
				OutputInterface->WriteTo();
			}
			if (OutPackQueueDepth == 0)
				ResetStateMachine();
		}
		else
			break;
	}
}
bool	PacketPort_FileSystem::isSupportedInPackType(enum PacketTypes packTYPE)
{
	return (packTYPE == packType_WriteComplete);
}
void	PacketPort_FileSystem::ResetStateMachine()
{
	FS_State = fs_Init;
}
void	PacketPort_FileSystem::SetStateMachineRead()
{ 
	FS_State = fs_Reading; 
}
void	PacketPort_FileSystem::SetStateMachineWrite()
{ 
	FS_State = fs_Writing; 
}
enum PacketPort_FS_State PacketPort_FileSystem::getFS_State() 
{ 
	return FS_State; 
}
#pragma endregion





