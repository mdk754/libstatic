#include "static/utility.h"

#include <catch/catch.hpp>

TEST_CASE("Construct a pair", "[constructor]") {
	SECTION("Default construct") {
		sstd::pair<short, char> test;

		REQUIRE(test.first == 0);
		REQUIRE(test.second == 0);
	}

	SECTION("Value construct") {
		sstd::pair<int, int> test(42, 24);

		REQUIRE(test.first == 42);
		REQUIRE(test.second == 24);
	}

	SECTION("Copy construct") {
		sstd::pair<char, unsigned char> a('a', 0xff);

		SECTION("From an equivalent pair") {
			sstd::pair<char, unsigned char> test(a);

			REQUIRE(test.first == 'a');
			REQUIRE(test.second == 0xff);
		}

		SECTION("From a compatible pair") {
			sstd::pair<short, unsigned> test(a);

			REQUIRE(test.first == 'a');
			REQUIRE(test.second == 0xff);
		}
	}

	SECTION("Via make_pair") {
		REQUIRE(sstd::make_pair('a', 16).first == 'a');
		REQUIRE(sstd::make_pair('b', 32).second == 32);
	}
}

TEST_CASE("Assign a pair", "[assignment]") {
	sstd::pair<unsigned short, short> a(16, 32);

	SECTION("From an equivalent pair") {
		sstd::pair<unsigned short, short> test(2, 8);

		test = a;

		REQUIRE(test.first == 16);
		REQUIRE(test.second == 32);
	}

	SECTION("From a compatible pair") {
		sstd::pair<unsigned, int> test(2, 8);

		test = a;

		REQUIRE(test.first == 16);
		REQUIRE(test.second == 32);
	}

	SECTION("Via make_pair") {
		a = sstd::make_pair(2, 8);

		REQUIRE(a.first == 2);
		REQUIRE(a.second == 8);
	}
}
