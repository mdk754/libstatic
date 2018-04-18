#include "static/array.h"

#include <catch/catch.hpp>

TEST_CASE("Construct an array", "[constructor]") {
	SECTION("Default construct") {
		sstd::array<char, 3> a;

		REQUIRE(a.data());
	}

	SECTION("Aggregate initialize") {
		sstd::array<int, 3> a = {{16, 16, 16}};

		REQUIRE(a[0] == 16);
		REQUIRE(a[1] == 16);
		REQUIRE(a[2] == 16);
	}

	SECTION("Copy construct") {
		sstd::array<unsigned char, 3> a = {{0, 1, 2}};
		sstd::array<unsigned char, 3> b(a);

		REQUIRE(a[0] == b[0]);
		REQUIRE(a[1] == b[1]);
		REQUIRE(a[2] == b[2]);
	}
}

TEST_CASE("Assign an array", "[assignment]") {
	sstd::array<char, 3> a = {{0, 1, 2}};
	sstd::array<char, 3> b = {{2, 1, 0}};

	b = a;

	REQUIRE(a[0] == b[0]);
	REQUIRE(a[1] == b[1]);
	REQUIRE(a[2] == b[2]);
}

TEST_CASE("Index into an array", "[access]") {
	const size_t count = 3;
	sstd::array<int, count> a;

	for (size_t i = 0; i < count; ++i) { a.data()[i] = ~i; }

	SECTION("Using random access operator") {
		a[1] = 8;

		REQUIRE(a[0] == ~0);
		REQUIRE(a[1] == 8);
		REQUIRE(a[2] == ~2);
	}

	SECTION("Using bounds checked method") {
		SECTION("Within bounds") {
			REQUIRE(a.at(0) == ~0);
			REQUIRE(a.at(1) == ~1);
			REQUIRE(a.at(2) == ~2);
		}

		SECTION("Out of bounds") {
			int overflow = a.data()[0];
			a.at(count)  = ~overflow;

			REQUIRE(a.at(count) == a.data()[0]);
		}
	}
}

TEST_CASE("Iterate over an array", "[iterator]") {
	typedef sstd::array<int, 3> array;
	array a = {{4, 4, 4}};

	SECTION("Forward direction") {
		for (array::iterator it = a.begin(); it != a.end(); ++it) { *it = 16; }

		REQUIRE(a[0] == 16);
		REQUIRE(a[1] == 16);
		REQUIRE(a[2] == 16);
	}

	SECTION("Reverse direction") {
		int i = 0;

		for (array::reverse_iterator it = a.rbegin(); it != a.rend();
		     ++it, ++i) {
			*it = i;
		}

		REQUIRE(a[0] == 2);
		REQUIRE(a[1] == 1);
		REQUIRE(a[2] == 0);
	}

	SECTION("With const-qualified array") {
		const array b = {{16, 16, 16}};

		size_t count = 0;

		for (array::const_iterator it = b.begin(); it != b.end(); ++it) {
			if (*it == 16) { ++count; }
		}

		REQUIRE(count == 3);
	}
}

TEST_CASE("Check the capacity of an array", "[capacity]") {
	sstd::array<int, 8> a;

	REQUIRE(!a.empty());
	REQUIRE(a.size() == 8);
	REQUIRE(a.max_size() == 8);
}

TEST_CASE("Test arrays for equality", "[comparison]") {
	sstd::array<char, 3> a = {{16, 16, 16}};

	SECTION("With an equal size array") {
		SECTION("With same contents") {
			sstd::array<char, 3> b = {{16, 16, 16}};

			REQUIRE(a == b);
		}

		SECTION("With differing contents") {
			sstd::array<char, 3> b = {{32, 32, 32}};

			REQUIRE(a != b);
		}
	}

	SECTION("With a smaller size array") {
		sstd::array<char, 2> b = {{16, 16}};

		REQUIRE(a != b);
	}

	SECTION("With a larger size array") {
		sstd::array<char, 5> b = {{16, 16, 16, 16, 16}};

		REQUIRE(a != b);
	}
}

TEST_CASE("Compare arrays lexicographically", "[comparison]") {
	sstd::array<int, 4> a = {{16, 16, 16, 16}};

	SECTION("With an equal size array") {
		SECTION("With lower value") {
			sstd::array<int, 4> b = {{8, 8, 8, 8}};

			REQUIRE(a > b);
			REQUIRE(a >= b);
			REQUIRE(b < a);
			REQUIRE(b <= a);
		}

		SECTION("With higher value") {
			sstd::array<int, 4> b = {{32, 32, 32, 32}};

			REQUIRE(a < b);
			REQUIRE(a <= b);
			REQUIRE(b > a);
			REQUIRE(b >= a);
		}

		SECTION("With equal value") {
			sstd::array<int, 4> b = {{16, 16, 16, 16}};

			REQUIRE(a <= b);
			REQUIRE(a >= b);
		}
	}

	SECTION("With s smaller size array") {
		sstd::array<int, 2> b = {{16, 16}};

		REQUIRE(b < a);
	}

	SECTION("With s larger size array") {
		sstd::array<int, 8> b = {{16, 16, 16, 16, 16, 16, 16, 16}};

		REQUIRE(a < b);
	}
}
