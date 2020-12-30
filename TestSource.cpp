/*! \file  TestSource.cpp
	\brief Examples of Intended Use Cases

	The IMS Packets Core is intended for use by developers of distributed
	computing systems.  It provides a base of code that can be used in a 
	"default" configuration or modified easily to custom needs.  

	TestSource.cpp defines cross platform reusable test objects that demonstrate
	the intended use cases (for developers) and provided testable objects to be
	instantiated on Console Applications, Unit Test Applications, and Node Firmware.

	As developments move from default to fully custom implementations, a hierarchy of 
	developer types emerges.  System developers are responsible for understanding the 
	communication partners within an ecosystem.  Application developers are responsible for
	understanding the application requirements of a particular node, or set of nodes,
	within an ecosystem.
	- System developers would first define a common node language and api enpoints.  
	- Application developers would then define application nodes which rely on the definitions of common node language and api endpoints.

*/
#include "5_IntendedUseTesting.cpp"
