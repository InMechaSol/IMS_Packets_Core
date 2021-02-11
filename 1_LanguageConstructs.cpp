#include "1_LanguageConstructs.h"
using namespace IMSPacketsAPICore;

template class SPDInterfaceBuffer<SPD1>;
template class SPDInterfaceBuffer<SPD2>;
template class SPDInterfaceBuffer<SPD4>;
template class SPDInterfaceBuffer<SPD8>;

// byte by byte binary exchange of data
void	Packet::getSPDat(int i, SPD1* SPDPtr)
{
	if (i > -1 && i < getNumSPDs())
		SPDPtr->uintVal = (bytesBufferPtr + i)[0];
}
void	Packet::getSPDat(int i, SPD2* SPDPtr)
{
	if (i > -1 && i < getNumSPDs())
	{
		for (int j = 0; j < sizeof(SPD2); j++)
		{
			SPDPtr->bytes[j] = (bytesBufferPtr + (i * sizeof(SPD2)))[j];
		}
	}
}
void	Packet::getSPDat(int i, SPD4* SPDPtr)
{
	if (i > -1 && i < getNumSPDs())
	{
		for (int j = 0; j < sizeof(SPD4); j++)
		{
			SPDPtr->bytes[j] = (bytesBufferPtr + (i * sizeof(SPD4)))[j];
		}
	}
}
void	Packet::getSPDat(int i, SPD8* SPDPtr)
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
void	Packet::setSPDat(int i, SPD1* SPDPtr)
{
	if (i > -1 && i < getNumSPDs())
		(bytesBufferPtr + i)[0] = SPDPtr->uintVal;
}
void	Packet::setSPDat(int i, SPD2* SPDPtr)
{
	if (i > -1 && i < getNumSPDs())
	{
		for (int j = 0; j < sizeof(SPD2); j++)
		{
			(bytesBufferPtr + i * sizeof(SPD2))[j] = SPDPtr->bytes[j];
		}
	}

}
void	Packet::setSPDat(int i, SPD4* SPDPtr)
{
	if (i > -1 && i < getNumSPDs())
	{
		for (int j = 0; j < sizeof(SPD4); j++)
		{
			(bytesBufferPtr + i * sizeof(SPD4))[j] = SPDPtr->bytes[j];
		}
	}

}
void	Packet::setSPDat(int i, SPD8* SPDPtr)
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
bool	Packet::getSPDfromcharsAt(int i, SPD1* SPDPtr, enum SPDValTypeEnum dType)
{
	if (i > 0 && i < getNumSPDs())
	{
		char* TokenStringPtr = (charsBufferPtr + STRINGBUFFER_IDTOKENRATIO + (i - 1) * STRINGBUFFER_TOKENRATIO);
		switch (dType)
		{
		case typeUINT:	if (!isUnsignedIntegerString(TokenStringPtr))	return false; SPDPtr->uintVal = atoi(TokenStringPtr); return true;
		case typeINT:	if (!isIntegerString(TokenStringPtr))			return false; SPDPtr->uintVal = atoi(TokenStringPtr); return true;
		}
	}
	return false;
}
bool	Packet::getSPDfromcharsAt(int i, SPD2* SPDPtr, enum SPDValTypeEnum dType)
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
bool	Packet::getSPDfromcharsAt(int i, SPD4* SPDPtr, enum SPDValTypeEnum dType)
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
bool	Packet::getSPDfromcharsAt(int i, SPD8* SPDPtr, enum SPDValTypeEnum dType)
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
bool	Packet::setCharsfromSPDat(int i, SPD1* SPDPtr, enum SPDValTypeEnum dType, const char* fString)
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
bool	Packet::setCharsfromSPDat(int i, SPD2* SPDPtr, enum SPDValTypeEnum dType, const char* fString)
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
bool	Packet::setCharsfromSPDat(int i, SPD4* SPDPtr, enum SPDValTypeEnum dType, const char* fString)
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
bool	Packet::setCharsfromSPDat(int i, SPD8* SPDPtr, enum SPDValTypeEnum dType, const char* fString)
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


// Buffer Accessors
uint8_t* Packet::getBytesBuffer() { return bytesBufferPtr; }
void	Packet::setBytesBuffer(uint8_t* bytesBufferPtrIn)
{
	bytesBufferPtr = bytesBufferPtrIn;
}
char*	Packet::getCharsBuffer() { return charsBufferPtr; }
void	Packet::setCharsBuffer(char* charsBufferPtrIn)
{
	charsBufferPtr = charsBufferPtrIn;
}
// Buffer Pointer Copier
void	Packet::CopyTokenBufferPtrs(Packet* copyPacketPtrs)
{
	bytesBufferPtr = copyPacketPtrs->bytesBufferPtr;
	charsBufferPtr = copyPacketPtrs->charsBufferPtr;
}


// Packet ID Comparisons
bool	Packet::ByteBuffer_ID_Equals(const int compareValue)
{
	return (getNumSPDs() == compareValue);
}
bool	Packet::StringBuffer_IDString_Equals(const char* compareStringPtr)
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


// Pack ID Accessors
void	Packet::writebuff_PackID(SPD1* SPDPtr) { SPDPtr->intVal = getPacketID(); setSPDat(Index_PackID, SPDPtr); }
void	Packet::writebuff_PackID(SPD2* SPDPtr) { SPDPtr->intVal = getPacketID(); setSPDat(Index_PackID, SPDPtr); }
void	Packet::writebuff_PackID(SPD4* SPDPtr) { SPDPtr->intVal = getPacketID(); setSPDat(Index_PackID, SPDPtr); }
void	Packet::writebuff_PackID(SPD8* SPDPtr) { SPDPtr->intVal = getPacketID(); setSPDat(Index_PackID, SPDPtr); }
void	Packet::writebuff_PackIDString()
{
	for (int i = 0; i < STRINGBUFFER_IDTOKENRATIO; i++)
	{
		charsBufferPtr[i] = getPacketIDString()[i];
		if (charsBufferPtr[i] == 0x00)
			break;
	}
}
void	Packet::readbuff_PackID(SPD1* SPDPtr) { getSPDat(Index_PackID, SPDPtr); }
void	Packet::readbuff_PackID(SPD2* SPDPtr) { getSPDat(Index_PackID, SPDPtr); }
void	Packet::readbuff_PackID(SPD4* SPDPtr) { getSPDat(Index_PackID, SPDPtr); }
void	Packet::readbuff_PackID(SPD8* SPDPtr) { getSPDat(Index_PackID, SPDPtr); }


// Pack Len Accessors
void	Packet::writebuff_PackLength(SPD1* SPDPtr) { SPDPtr->intVal = sizeof(SPD1) * getNumSPDs(); setSPDat(Index_PackLEN, SPDPtr); }
void	Packet::writebuff_PackLength(SPD2* SPDPtr) { SPDPtr->intVal = sizeof(SPD2) * getNumSPDs(); setSPDat(Index_PackLEN, SPDPtr); }
void	Packet::writebuff_PackLength(SPD4* SPDPtr) { SPDPtr->intVal = sizeof(SPD4) * getNumSPDs(); setSPDat(Index_PackLEN, SPDPtr); }
void	Packet::writebuff_PackLength(SPD8* SPDPtr) { SPDPtr->intVal = sizeof(SPD8) * getNumSPDs(); setSPDat(Index_PackLEN, SPDPtr); }
void	Packet::writebuff_PackLength(SPD1* SPDPtr, int numTokens) { SPDPtr->intVal = sizeof(SPD1) * numTokens; setSPDat(Index_PackLEN, SPDPtr); }
void	Packet::writebuff_PackLength(SPD2* SPDPtr, int numTokens) { SPDPtr->intVal = sizeof(SPD2) * numTokens; setSPDat(Index_PackLEN, SPDPtr); }
void	Packet::writebuff_PackLength(SPD4* SPDPtr, int numTokens) { SPDPtr->intVal = sizeof(SPD4) * numTokens; setSPDat(Index_PackLEN, SPDPtr); }
void	Packet::writebuff_PackLength(SPD8* SPDPtr, int numTokens) { SPDPtr->intVal = sizeof(SPD8) * numTokens; setSPDat(Index_PackLEN, SPDPtr); }
void	Packet::writebuff_TokenCountString() { snprintf(charsBufferPtr + STRINGBUFFER_IDTOKENRATIO, STRINGBUFFER_TOKENRATIO, "%d", getNumSPDs()); }
void	Packet::writebuff_TokenCountString(int numTokens) { snprintf(charsBufferPtr + STRINGBUFFER_IDTOKENRATIO, STRINGBUFFER_TOKENRATIO, "%d", numTokens); }
void	Packet::readbuff_PackLength(SPD1* SPDPtr) { getSPDat(Index_PackLEN, SPDPtr); }
void	Packet::readbuff_PackLength(SPD2* SPDPtr) { getSPDat(Index_PackLEN, SPDPtr); }
void	Packet::readbuff_PackLength(SPD4* SPDPtr) { getSPDat(Index_PackLEN, SPDPtr); }
void	Packet::readbuff_PackLength(SPD8* SPDPtr) { getSPDat(Index_PackLEN, SPDPtr); }

// String/Char comparison Functions
bool	Packet::isASCIIPacket() { return(charsBufferPtr != nullptr); }
bool	Packet::isASCIIchar(char inChar) { return ((inChar >= ASCII_space && inChar <= ASCII_tilda) || inChar == ASCII_lf || inChar == ASCII_cr || inChar == ASCII_tab || inChar == 0x00); }
bool	Packet::isLetterchar(char inChar) { return ((inChar >= ASCII_A && inChar <= ASCII_Z) || (inChar >= ASCII_a && inChar <= ASCII_z)); }
bool	Packet::isNumberchar(char inChar) { return ((inChar >= ASCII_0 && inChar <= ASCII_9) || inChar == ASCII_plus || inChar == ASCII_minus || inChar == ASCII_dot); }
bool	Packet::isIntegerchar(char inChar) { return ((inChar >= ASCII_0 && inChar <= ASCII_9) || inChar == ASCII_plus || inChar == ASCII_minus); }
bool	Packet::isUnsignedIntegerchar(char inChar) { return ((inChar >= ASCII_0 && inChar <= ASCII_9)); }
bool	Packet::isDelimiterchar(char inChar) { return (inChar == ASCII_colon); }
bool	Packet::isTerminatorchar(char inChar) { return (inChar == ASCII_semicolon); }
bool	Packet::isASCIIString(char* inStringPtr) { int index = 0;  while (inStringPtr[index] != 0x00) if (!isASCIIchar(inStringPtr[index++])) return false; return true; }
bool	Packet::isLetterString(char* inStringPtr) { int index = 0;  while (inStringPtr[index] != 0x00) if (!isLetterchar(inStringPtr[index++])) return false; return true; }
bool	Packet::isNumberString(char* inStringPtr) { int index = 0; while (inStringPtr[index] == ' ') index++;  while (inStringPtr[index] != 0x00) if (!isNumberchar(inStringPtr[index++])) return false; return true; }
bool	Packet::isIntegerString(char* inStringPtr) { int index = 0;  while (inStringPtr[index] != 0x00) if (!isIntegerchar(inStringPtr[index++])) return false; return true; }
bool	Packet::isUnsignedIntegerString(char* inStringPtr) { int index = 0;  while (inStringPtr[index] != 0x00) if (!isUnsignedIntegerchar(inStringPtr[index++])) return false; return true; }

