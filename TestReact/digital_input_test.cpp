#include "stdafx.h"
#include "CppUnitTest.h"
#include "../REACT/DigitalInput.h"
#include "../REACT/piproxy.h"
#include "../REACT/Status.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestReact
{
	TEST_CLASS(DigitalInput_test)
	{
	public:

		TEST_METHOD(Test_digital_input)
		{
			Digital_input di("pin_7", 7, PUD_DOWN);
			Assert::AreEqual("pin_7", di.get_name().c_str(), "Different name");
			Assert::AreEqual(7, di.get_pin());
			Assert::AreEqual(static_cast<int>(Status::OFF), static_cast<int>(di.read()));
		}

	};
}