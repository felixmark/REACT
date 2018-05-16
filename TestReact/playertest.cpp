#include "stdafx.h"
#include "CppUnitTest.h"
#include "../REACT/Player.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestReact
{		
	TEST_CLASS(Player_test)
	{
	public:
		
		TEST_METHOD(Test_construction)
		{
			Player p{ "Felix", 0 };
			Assert::AreEqual("Felix", p.get_name().c_str(), "Names are different");
			Assert::AreEqual(0, p.get_won_games());
			p.set_name("Nona");
			Assert::AreEqual("Nona", p.get_name().c_str(), "set_name didnt work");
			p.add_won_game();
			Assert::AreEqual(1, p.get_won_games());
			p.reset_won_games();
			Assert::AreEqual(0, p.get_won_games());
		}
		
	};
}