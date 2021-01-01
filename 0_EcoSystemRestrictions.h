/*! \file  0_EcoSystemRestrictions.h
	\brief Design Configuration for a derived micro-EcoSystem

*/

#ifndef __ECOSYSTEMRESTRICTIONS__
#define __ECOSYSTEMRESTRICTIONS__

/*! \defgroup EcosystemRestrictions
	\brief Design Configuration for a derived micro-EcoSystem

	A particular product or application node is fundamentally restricted at compile time by a small
	set of Design Configuration settings.  Any node that "is a" node built from this particular file
	will:
	- have a common token count that is used to instantiate token buffers
	- have a common binary buffer size equal to token count multiplied by token size (configured at application layer)
	And if also using string interfaces nodes will:
	- link the iostream standard library (then optionally use linked library)
	- have a common token string ratio that is used to instantiate string token buffers
	- have a common ID string token size that is used to instantiate ID string token buffers
	- have a common string buffer size equal to the string buffer character count macro

	When reading and writing tokens over a packet interface a buffer
	(on either the stack, heap, or data/bss sections of RAM) is required.
	These limitations provide mathematical gaurantees on static data allocation.

	\par Testable:
	- Functions reading from buffers do not overrun high or low address limits
	- Functions writing to buffers do not overrun high or low address limits
	- in a defined language, no ID string exceeds ID token ratio limit
	- in a defined language, no format string exceeds token ratio limit

	\note
	For now, the Packets Core links a standard library for simplification of common use cases where
	the iostream library can be linked.  If it cannot be linked, cutom interfaces can be used.  The stream
	library can be useful in transferring data to/from physical layer drivers. <A HREF="http://www.cplusplus.com/reference/iostream/">more on iostream</A>
	\code
	#include <iostream> // istream, ostream, and iostream (packet interface objects)
	\endcode
	\note
	On visual studio c++ console applications and cpp test framework applications several
	standard libraries are linked by default.  Depending on node platform, other standard
	libraries may need to be directly linked
	\code
	#include <cstdio>	// snprintf()
	#include <cstdlib>	// atoi() and atof()
	#include <cstdint>	// uint8_t, int8_t, uint16_t, ... etc.
	\endcode

	@{
*/


/*! \def PACKETBUFFER_TOKENCOUNT
	\brief The number of tokens stored in an interface buffer

	This is only the count of tokens to be stored.
	The size in bytes of a buffer depends on other factors like:
	- the token size
	- and serialization type (if string or binary)

*/
#define PACKETBUFFER_TOKENCOUNT (32)


#include <iostream>		// istream, ostream, and iostream (packet interface objects)
//#include <cstdint>	// uint8_t, int8_t, uint16_t, ... etc.


#pragma region String Packets Require char* and binary-string conversion
//#include <cstdio>		// snprintf()
//#include <cstdlib>	// atoi() and atof()


/*! \def STRINGBUFFER_TOKENRATIO
	\brief The number of string characters per token

	This is only the number of characters.
	For now, encoding is restricted to ASCII so there
	is a 1-to-1 ratio for bytes to chars.
*/
#define STRINGBUFFER_TOKENRATIO (10)


/*! \def STRINGBUFFER_IDTOKENRATIO
	\brief The number of string characters per ID string

	This is only the number of characters.
	For now, encoding is restricted to ASCII so there
	is a 1-to-1 ratio for bytes to chars.
*/
#define STRINGBUFFER_IDTOKENRATIO (32)


/*! \def STRINGBUFFER_CHARCOUNT
	\brief The number of characters allocated in a string buffer

	Space for:
	- 1 Token ID String, and
	- TokenRatio * (TokenCount-1) Token String Characters

	This is only the number of characters.
	For now, encoding is restricted to ASCII so there
	is a 1-to-1 ratio for bytes to chars.
*/
#define STRINGBUFFER_CHARCOUNT ((PACKETBUFFER_TOKENCOUNT-1)*STRINGBUFFER_TOKENRATIO+STRINGBUFFER_IDTOKENRATIO)

/*! @}*/

#pragma endregion



#endif // !__ECOSYSTEMRESTRICTIONS__
