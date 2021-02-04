#include "2_PacketPortLink.h"
using namespace IMSPacketsAPICore;


// Constructors
PacketInterface::PacketInterface(int PortIDin, std::iostream* ifaceStreamPtrIn)
{
	ifaceStreamPtr = ifaceStreamPtrIn;
	PortID = PortIDin;
}
PacketInterface::PacketInterface(int PortIDin, std::istream* ifaceInStreamPtrIn)
{
	ifaceInStreamPtr = ifaceInStreamPtrIn;
	PortID = PortIDin;
}
PacketInterface::PacketInterface(int PortIDin, std::ostream* ifaceOutStreamPtrIn)
{
	ifaceOutStreamPtr = ifaceOutStreamPtrIn;
	PortID = PortIDin;
}

int		PacketInterface::getPortID() { return PortID; }
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




void PolymorphicPacketPort::ServicePort_SR_Sender()
{
	switch (SRCommState)
	{
	case sr_Init: SRCommState = sr_Handling; break;
	case sr_Reading:
		InputInterface->ReadFrom();
		if (InputInterface->DeSerializePacket()) {
			DataExecution->HandleRxPacket(InputInterface);
			SRCommState = sr_Handling;
		}
		else
			break;
	case sr_Handling:
		if (DataExecution->PrepareTxPacket(OutputInterface))
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



void PolymorphicPacketPort::ServicePort_SR_Responder()
{
	switch (SRCommState)
	{
	case sr_Init: SRCommState = sr_Reading; break;
	case sr_Reading:
		InputInterface->ReadFrom();
		if (InputInterface->DeSerializePacket())
		{
			DataExecution->HandleRxPacket(InputInterface);
			SRCommState = sr_Handling;
		}
		else
			break;
	case sr_Handling:
		if (DataExecution->PrepareTxPacket(OutputInterface))
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
void PolymorphicPacketPort::ServicePort_FCP_Partner()
{
	InputInterface->ReadFrom();
	if (InputInterface->DeSerializePacket())
	{
		DataExecution->HandleRxPacket(InputInterface);
	}

	if (DataExecution->PrepareTxPacket(OutputInterface))
	{
		if (OutputInterface->SerializePacket())
			OutputInterface->WriteTo();
	}
}

int PolymorphicPacketPort::getPortID() { return PortID; }
bool PolymorphicPacketPort::getAsyncService() { return ServiceAsync; }


void PolymorphicPacketPort::ServicePort()
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

PolymorphicPacketPort::PolymorphicPacketPort(int PortIDin, PacketInterface* InputInterfaceIn, PacketInterface* OutputInterfaceIn, AbstractDataExecution* DataExecutionIn, PacketPortPartnerType PortTypeIn, bool isAsync)
{
	InputInterface = InputInterfaceIn;
	OutputInterface = OutputInterfaceIn;
	DataExecution = DataExecutionIn;
	ServiceAsync = isAsync;
	PortType = PortTypeIn;
	PortID = PortIDin;
}