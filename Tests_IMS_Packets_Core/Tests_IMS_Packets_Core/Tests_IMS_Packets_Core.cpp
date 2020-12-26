#include "pch.h"
#include "CppUnitTest.h"
#include "TestSource.cpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestsIMSPacketsCore
{
	/// <summary>
	/// Focus this test class on directly testing an instance of a Node
	/// </summary>
	TEST_CLASS(TestsIMSPacketsCore_Local)
	{
	public:	
		
		TEST_METHOD(Local_TestMethod1)
		{
			// create api node instance (on the stack)
			CoreTest_Console_Node theTestNode;

			// Then Do Something testable

			// Then return the results
		}

		TEST_METHOD(Local_TestMethod1_Custom)
		{
			// create api node instance (on the stack)
			CoreTest_CustomConsole_Node theCustomTestNode;

			// Then Do Something testable

			// Then return the results
		}
	};


	/// <summary>
	/// Focus this test class on testing an instance of a Node using its API and partner port
	/// </summary>
	TEST_CLASS(TestsIMSPacketsCore_API)
	{
	public:

		TEST_METHOD(API_TestMethod1)
		{
		}
	};

	/// <summary>
	/// Focus this test class on testing multiple instances of Nodes using their API and partners
	/// </summary>
	TEST_CLASS(TestsIMSPacketsCore_Network)
	{
	public:

		TEST_METHOD(Network_TestMethod1)
		{
		}
	};

	/// <summary>
	/// Focus this test class on testing an instance of a Node using its API and various communication partners
	/// </summary>
	TEST_CLASS(TestsIMSPacketsCore_Comms)
	{
	public:

		TEST_METHOD(Comms_TestMethod1)
		{
		}
	};
}
