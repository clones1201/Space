#include <CppUnitTest.h>

#include "SharedUtility.hpp"
#include "Mesh.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace Space;

namespace UnitTest
{
	TEST_CLASS(UtilityTest)
	{
	public:
		TEST_METHOD(UinGeneratorTest)
		{
			UidGenerator uids;


		}

		TEST_METHOD(NameTest)
		{
			Name name0(TEXT("name 0"));
			Name name1(TEXT("name 1"));
			Name name2(TEXT("name 2"));
			Name name3 = name0;
			Name name4(name0);
			Name name5 = name3;

			Assert::AreEqual(true, name5 == name0);
			Assert::AreEqual(true, name5 == name4);
			Name* name6;
			Name* name7;
			Name* name8;
			{
				Name name9(TEXT("name 9"));
				name6 = new Name(TEXT("name 6"));
				name7 = new Name(TEXT("name 7"));
				name8 = new Name(name9);
				Name name10(TEXT("name 10"));
				Name name11(name9);
				Name name12(*name7);
			}
			Name name13(TEXT("name 13"));
			Name name14(*name8);
			delete name6;
			delete name7;

			Assert::AreEqual(TEXT("name 9"), name8->ToString().c_str());
			delete name8;
			name14 = name13;
			Assert::AreEqual(TEXT("name 13"), name14.ToString().c_str());
		}

		TEST_METHOD(FBXMeshLoadTest)
		{



		}

	};

}


