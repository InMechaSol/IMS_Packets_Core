#ifndef __PACKET_VERSION__
#define __PACKET_VERSION__

#include "3_Packet_HDRPACK.h"

namespace IMSPacketsAPICore
{
	/*! \def VERSION
		\brief Token ID value for VERSION Packet

		VERSION is of the form major.minor.build with a -dev indication
	*/
	#define VERSION (1)
	enum pENUM(VERSION)
	{
		iVERSION_Major = iHDRPACK_END,
		iVERSION_Minor,
		iVERSION_Build,
		iVERSION_Dev,
		iVERSION_END
	};
	struct pSTRUCT(VERSION)
	{
		int Major = 0;
		int Minor = 0;
		int Build = 0;
		int DevFlag = 0;
	};
	TEMPLATE_STATICPACKETINFO(VERSION, iVERSION_END)


	/*! \class Packet_Version
		\brief A Version Packet for Application Nodes
	*/
	class pCLASS(VERSION) :public Packet_HDRPACK
	{
	public:
		char*		getPacketIDString() { return (char*)&IDString_VERSION[0]; }
		int			getPacketID() { return ID_VERSION; }
		int			getNumSPDs() { return TokenCount_VERSION; }

		TEMPLATE_SPDACCESSORS(MajorVersion, iVERSION_Major, typeINT, "%d")
		TEMPLATE_SPDACCESSORS(MinorVersion, iVERSION_Minor, typeINT, "%d")
		TEMPLATE_SPDACCESSORS(BuildNumber, iVERSION_Build, typeINT, "%d")
		TEMPLATE_SPDACCESSORS(DevFlag, iVERSION_Dev, typeINT, "%d")

	};
}
#endif // !__PACKET_VERSION__