#include "3_APINodeLink.h"
using namespace IMSPacketsAPICore;

template class PacketInterface_Binary<SPD1>;
template class PacketInterface_Binary<SPD2>;
template class PacketInterface_Binary<SPD4>;
template class PacketInterface_Binary<SPD8>;


template<class TokenType>
void PacketInterface_Binary<TokenType>::WriteToStream()
{
	if (ifaceStreamPtr != nullptr)
		ifaceStreamPtr->write((char*)(&(TokenBuffer.bytes[0])), serializedPacketSize);
	else if (ifaceOutStreamPtr != nullptr)
		ifaceOutStreamPtr->write((char*)(&(TokenBuffer.bytes[0])), serializedPacketSize);
}

template<class TokenType>
void PacketInterface_Binary<TokenType>::ReadFromStream()
{
	if (ifaceStreamPtr != nullptr)
	{
		if (ifaceStreamPtr->peek() != EOF)
			ifaceStreamPtr->read((char*)(&(TokenBuffer.bytes[ByteIndex++])), 1);
	}
	else if (ifaceInStreamPtr != nullptr)
	{
		if (ifaceInStreamPtr->peek() != EOF)
			ifaceInStreamPtr->read((char*)(&(TokenBuffer.bytes[ByteIndex++])), 1);
	}
}

template<class TokenType>
void PacketInterface_Binary<TokenType>::ResetdeSerialize()
{
	ByteIndex = 0;
	ByteIndexLast = 0;
	deSerializedPacketSize = 0;
	deSerializedTokenIndex = 0;
	deSerializedTokenLength.uintVal = 0;
	deSerializeReset = false;
}

template<class TokenType>
bool PacketInterface_Binary<TokenType>::DeSerializePacket_Binary(PacketInterface_Binary<TokenType>* PcktInterface)
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

template<class TokenType>
bool PacketInterface_Binary<TokenType>::SerializePacket_Binary(PacketInterface_Binary<TokenType>* PcktInterface)
{
	// called single-shot after tx packet handler of the data execution instance
		// optionally swap bytes order before sending
	;

	// return true or false as error indication, true means all is well
	// true will permit sending by the output packet interface instance
	return true;
}

template<class TokenType>
bool PacketInterface_Binary<TokenType>::DeSerializePacket()
{
	return DeSerializePacket_Binary(this);
}

template<class TokenType>
bool PacketInterface_Binary<TokenType>::SerializePacket()
{
	return SerializePacket_Binary(this);
}

template<class TokenType>
Packet* PacketInterface_Binary<TokenType>::getPacketPtr() { return &BufferPacket; }

template<class TokenType>
int		PacketInterface_Binary<TokenType>::getTokenSize() { return sizeof(TokenType); }

template<class TokenType>
PacketInterface_Binary<TokenType>::PacketInterface_Binary(int PortIDin, std::iostream* ifaceStreamPtrIn) :
	PacketInterface(PortIDin, ifaceStreamPtrIn) {
	BufferPacket.setBytesBuffer(&(TokenBuffer.bytes[0]));
}
template<class TokenType>
PacketInterface_Binary<TokenType>::PacketInterface_Binary(int PortIDin, std::istream* ifaceInStreamPtrIn) :
	PacketInterface(PortIDin, ifaceInStreamPtrIn) {
	BufferPacket.setBytesBuffer(&(TokenBuffer.bytes[0]));
}
template<class TokenType>
PacketInterface_Binary<TokenType>::PacketInterface_Binary(int PortIDin, std::ostream* ifaceOutStreamPtrIn) :
	PacketInterface(PortIDin, ifaceOutStreamPtrIn) {
	BufferPacket.setBytesBuffer(&(TokenBuffer.bytes[0]));
}



void PacketInterface_ASCII::WriteToStream_ASCII(PacketInterface_ASCII* PcktInterface, std::ostream* PcktInterfaceStream)
{
	PcktInterfaceStream->write(&(PcktInterface->TokenBuffer.chars[0]), PcktInterface->serializedPacketSize);
}
void PacketInterface_ASCII::WriteToStream_ASCII(PacketInterface_ASCII* PcktInterface, std::iostream* PcktInterfaceStream)
{
	PcktInterfaceStream->write(&(PcktInterface->TokenBuffer.chars[0]), PcktInterface->serializedPacketSize);
}
void PacketInterface_ASCII::WriteToStream()
{
	if (ifaceStreamPtr != nullptr)
		WriteToStream_ASCII(this, ifaceStreamPtr);
	else if (ifaceOutStreamPtr != nullptr)
		WriteToStream_ASCII(this, ifaceOutStreamPtr);
}


void PacketInterface_ASCII::ReadFromStream_ASCII(PacketInterface_ASCII* PcktInterface, std::istream* PcktInterfaceStream)
{
	if (PcktInterfaceStream->peek() != EOF)
		PcktInterfaceStream->read(&(PcktInterface->TokenBuffer.chars[PcktInterface->CharIndex++]), 1);

}
void PacketInterface_ASCII::ReadFromStream_ASCII(PacketInterface_ASCII* PcktInterface, std::iostream* PcktInterfaceStream)
{
	if (PcktInterfaceStream->peek() != EOF)
		PcktInterfaceStream->read(&(PcktInterface->TokenBuffer.chars[PcktInterface->CharIndex++]), 1);

}
void PacketInterface_ASCII::ReadFromStream()
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

void PacketInterface_ASCII::ResetdeSerialize()
{
	CharIndex = 0;
	CharIndexLast = 0;
	deSerializedTokenIndex = 0;
	deSerializeReset = false;
}

bool PacketInterface_ASCII::DeSerializePacket_ASCII(PacketInterface_ASCII* PcktInterface)
{
	// called cyclically
	// monitor CharIndex for change
	if (PcktInterface->CharIndexLast != PcktInterface->CharIndex)
	{
		int lastIndex = PcktInterface->CharIndex - 1;

		// decide if error, trigger reset
		if (!Packet::isASCIIchar(PcktInterface->TokenBuffer.chars[lastIndex]) || PcktInterface->TokenBuffer.chars[lastIndex] == ASCII_lf || PcktInterface->TokenBuffer.chars[lastIndex] == ASCII_cr)
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
				int charsWritten = snprintf(&(PcktInterface->TokenBuffer.chars[LastTokenStart]), STRINGBUFFER_TOKENRATIO, "%d%c", PcktInterface->getPortID(), lastChar);
				lastIndex = LastTokenStart + charsWritten - 1;
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
				if (PcktInterface->deSerializedTokenIndex >= Packet_HDRPACK::TokenCount) {
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

bool PacketInterface_ASCII::DeSerializePacket()
{
	return DeSerializePacket_ASCII(this);
}

bool PacketInterface_ASCII::SerializePacket_ASCII(PacketInterface_ASCII* PcktInterface)
{
	// called single-shot
	int lastCharIndexWritten = 0;	// initialized to start of id string
	int SerializedTokenCount = Packet_HDRPACK::TokenCount;	// initialized to minimum token of HDR packet
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
			j = STRINGBUFFER_IDTOKENRATIO + (i - 1) * STRINGBUFFER_TOKENRATIO;
			k = STRINGBUFFER_IDTOKENRATIO + i * STRINGBUFFER_TOKENRATIO;
		}

		// iterate through token by char index
		for (j; j < k; j++)
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
						if (SerializedTokenCount < Packet_HDRPACK::TokenCount)
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
bool PacketInterface_ASCII::SerializePacket()
{
	return SerializePacket_ASCII(this);
}

Packet* PacketInterface_ASCII::getPacketPtr() { return &BufferPacket; }
int		PacketInterface_ASCII::getTokenSize() { return STRINGBUFFER_TOKENRATIO; }
PacketInterface_ASCII::PacketInterface_ASCII(int PortIDin, std::iostream* ifaceStreamPtrIn) :
	PacketInterface(PortIDin, ifaceStreamPtrIn) {
	BufferPacket.setCharsBuffer(&(TokenBuffer.chars[0]));
}
PacketInterface_ASCII::PacketInterface_ASCII(int PortIDin, std::istream* ifaceInStreamPtrIn) :
	PacketInterface(PortIDin, ifaceInStreamPtrIn) {
	BufferPacket.setCharsBuffer(&(TokenBuffer.chars[0]));
}
PacketInterface_ASCII::PacketInterface_ASCII(int PortIDin, std::ostream* ifaceOutStreamPtrIn) :
	PacketInterface(PortIDin, ifaceOutStreamPtrIn) {
	BufferPacket.setCharsBuffer(&(TokenBuffer.chars[0]));
}


void API_NODE::ServiceSynchronousPorts(API_NODE* nodePtr)
{

	for (int i = 0; i < nodePtr->getNumPacketPorts(); i++)
	{
		if (!(nodePtr->getPacketPortat(i))->getAsyncService())
		{
			(nodePtr->getPacketPortat(i))->ServicePort();
		}
	}

}
void API_NODE::Loop()
{
	CustomLoop();
	ServiceSynchronousPorts(this);
}

void API_NODE::HandleRxPacket(Packet* RxPackInPtr)
{
	// Mark Rx packet with port index

	TEMPLATE_RX_HANDLER(RxPackInPtr, VERSION)

	API_CustomShared_HandleRx(RxPackInPtr);
}

bool API_NODE::PrepareTxPacket(Packet* TxPackOutPtr)
{
	TEMPLATE_TX_PACKAGER(TxPackOutPtr, VERSION)

	return API_CustomShared_PrepareTx(TxPackOutPtr);
}

#pragma region Packet_VERSION Members

TEMPLATE_PACKNODE_MEMBERS_CPP(API_NODE,VERSION)

template<class TokenType>
void API_NODE::staticHandler_VERSION(Packet_VERSION* inPack, API_NODE* nodePtr, Struct_VERSION* dstStruct)
{
	TokenType x_SPD;
	if (inPack->isASCIIPacket())
		inPack->getfromStringPacketType(&x_SPD);
	else
		inPack->getPacketType(&x_SPD);

	if (x_SPD.intVal == ReadComplete)
		nodePtr->setPackTriggerVERSION();
	else if (x_SPD.intVal == ResponseComplete && dstStruct != nullptr)
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
bool API_NODE::staticPackager_VERSION(Packet_VERSION* outPack, API_NODE* nodePtr, Struct_VERSION* srcStruct)
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

template void API_NODE::staticHandler_VERSION<SPD1>(Packet_VERSION* inPack, API_NODE* nodePtr, Struct_VERSION* dstStruct);
template void API_NODE::staticHandler_VERSION<SPD2>(Packet_VERSION* inPack, API_NODE* nodePtr, Struct_VERSION* dstStruct);
template void API_NODE::staticHandler_VERSION<SPD4>(Packet_VERSION* inPack, API_NODE* nodePtr, Struct_VERSION* dstStruct);
template void API_NODE::staticHandler_VERSION<SPD8>(Packet_VERSION* inPack, API_NODE* nodePtr, Struct_VERSION* dstStruct);

template bool API_NODE::staticPackager_VERSION<SPD1>(Packet_VERSION* outPack, API_NODE* nodePtr, Struct_VERSION* srcStruct);
template bool API_NODE::staticPackager_VERSION<SPD2>(Packet_VERSION* outPack, API_NODE* nodePtr, Struct_VERSION* srcStruct);
template bool API_NODE::staticPackager_VERSION<SPD4>(Packet_VERSION* outPack, API_NODE* nodePtr, Struct_VERSION* srcStruct);
template bool API_NODE::staticPackager_VERSION<SPD8>(Packet_VERSION* outPack, API_NODE* nodePtr, Struct_VERSION* srcStruct);
#pragma endregion
