#include "static/algorithm.h"

#include <catch/catch.hpp>

TEST_CASE("Check for equality", "[comparison]") {
	const size_t count = 3;
	int a[count]       = {0, 1, 2};

	SECTION("When the values are equal") {
		int b[count] = {0, 1, 2};

		REQUIRE(sstd::equal(&a[0], &a[count], &b[0]));
	}

	SECTION("When the values are not equal") {
		int b[count] = {0};

		REQUIRE(!sstd::equal(&a[0], &a[count], &b[0]));
	}
}

TEST_CASE("Lexicographically compare values", "[comparison]") {
	const size_t count = 5;
	int a[count]       = {0, 1, 2, 3, 4};

	SECTION("First mismatching element defines which is less than") {
		SECTION("First array less than") {
			int b[count] = {0, 7, 2, 3, 4};

			REQUIRE(sstd::lexicographical_compare(
			   &a[0], &a[count], &b[0], &b[count]));
		}

		SECTION("Second array less than") {
			int b[count] = {0, 1, 0, 3, 4};

			REQUIRE(!sstd::lexicographical_compare(
			   &a[0], &a[count], &b[0], &b[count]));
		}
	}

	SECTION("If one range is a prefix of the other, the shorter is less than") {
		REQUIRE(sstd::lexicographical_compare(&a[0], &a[2], &a[0], &a[count]));
		REQUIRE(!sstd::lexicographical_compare(&a[0], &a[count], &a[0], &a[1]));
	}

	SECTION("If two ranges have the same elements and length, they are equal") {
		REQUIRE(
		   !sstd::lexicographical_compare(&a[0], &a[count], &a[0], &a[count]));
	}

	SECTION("An empty range is less than any non-empty range") {
		REQUIRE(sstd::lexicographical_compare(&a[0], &a[0], &a[0], &a[count]));
		REQUIRE(!sstd::lexicographical_compare(&a[0], &a[count], &a[0], &a[0]));
	}

	SECTION("Two empty ranges are equal") {
		REQUIRE(!sstd::lexicographical_compare(a, a, &a[3], &a[3]));
		REQUIRE(!sstd::lexicographical_compare(&a[3], &a[3], a, a));
	}
}
