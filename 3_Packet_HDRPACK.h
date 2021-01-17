#ifndef __PACKET_HDRPACK__
#define __PACKET_HDRPACK__
#include "2_PacketPortLink.h"


namespace IMSPacketsAPICore
{

	/*! \def HDRPACK
		\brief Token ID value for default HDR Packet
	*/
	#define HDRPACK (0)
	enum pENUM(HDRPACK)
	{
		iHDRPACK_PacketID = Index_PackID,
		iHDRPACK_PacketLength = Index_PackLEN,
		iHDRPACK_PacketType,
		iHDRPACK_PacketOption,
		iHDRPACK_END
	};
	struct pSTRUCT(HDRPACK)
	{
		int PackID = HDRPACK;
		int PackLen = iHDRPACK_END;
		int PackType = 0;
		int PackOpt = 0;
	};
	TEMPLATE_STATICPACKETINFO(HDRPACK, iHDRPACK_END)



	/*! \class HDR_Packet
		\brief A Packet with Header Information
	*/
	class pCLASS(HDRPACK) :public Packet
	{
	public:
		char*		getPacketIDString() { return (char*)&IDString_HDRPACK[0]; }
		int			getPacketID() { return ID_HDRPACK; }
		int			getNumSPDs() { return TokenCount_HDRPACK; }

		TEMPLATE_SPDACCESSORS(PacketType, iHDRPACK_PacketType, typeINT, "%d")
		TEMPLATE_SPDACCESSORS(PacketOption, iHDRPACK_PacketOption, typeINT, "%d")
	};
}

#endif // !__PACKET_HDRPACK__

