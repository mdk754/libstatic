#include "static/iterator.h"

#include <catch/catch.hpp>

#include "static/algorithm.h"

TEST_CASE("Iterate in reverse", "[iterator]") {
	const size_t count = 4;
	int a[count]       = {2, 4, 8, 16};
	int b[count]       = {16, 8, 4, 2};

	sstd::reverse_iterator<int*> begin(a + count);
	sstd::reverse_iterator<int*> end(a);

	REQUIRE(sstd::equal(begin, end, b));
}
