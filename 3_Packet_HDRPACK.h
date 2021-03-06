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
	



	/*! \class HDR_Packet
		\brief A Packet with Header Information
	*/
	class pCLASS(HDRPACK) :public Packet
	{
	public:
		TEMPLATE_STATICPACKETINFO_H(HDRPACK, iHDRPACK_END)

		TEMPLATE_SPDACCESSORS_H(PacketType)
		TEMPLATE_SPDACCESSORS_H(PacketOption)
	};
}

#endif // !__PACKET_HDRPACK__

