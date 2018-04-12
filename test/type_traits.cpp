#include "static/type_traits.h"

#include <catch/catch.hpp>

TEST_CASE("Integral constants have value", "[integral]") {
	SECTION("Boolean values") {
		REQUIRE((sstd::integral_constant<bool, true>::value));
		REQUIRE((!sstd::integral_constant<bool, false>::value));
	}

	SECTION("Integral values") {
		REQUIRE((sstd::integral_constant<unsigned char, 8>::value == 8));
		REQUIRE((sstd::integral_constant<int, 127>::value == 127));
	}
}

TEST_CASE("Get the alignment of a type", "[alignment]") {
	REQUIRE((sstd::alignment_of<uint8_t>::value == 1));
	REQUIRE((sstd::alignment_of<uint16_t>::value == 2));
	REQUIRE((sstd::alignment_of<uint32_t>::value == 4));
	REQUIRE((sstd::alignment_of<uint64_t>::value == 8));
}

TEST_CASE("Aligned uninitialized byte storage", "[alignment]") {
	SECTION("Verify the size is correct") {
		REQUIRE(sizeof(sstd::aligned_storage<1, 1>::type) == 1);
		REQUIRE(sizeof(sstd::aligned_storage<4, 1>::type) == 4);
		REQUIRE(sizeof(sstd::aligned_storage<5, 4>::type) == 8);
		REQUIRE(sizeof(sstd::aligned_storage<128, 8>::type) == 128);
	}

	SECTION("Verify the alignement is correct") {
		REQUIRE(
		   (sstd::alignment_of<sstd::aligned_storage<1, 1>::type>::value == 1));
		REQUIRE(
		   (sstd::alignment_of<sstd::aligned_storage<4, 1>::type>::value == 1));
		REQUIRE(
		   (sstd::alignment_of<sstd::aligned_storage<5, 4>::type>::value == 4));
		REQUIRE(
		   (sstd::alignment_of<sstd::aligned_storage<128, 8>::type>::value ==
		    8));
	}
}
