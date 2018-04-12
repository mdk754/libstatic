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

TEST_CASE("Get the distance between two iterators", "[comparison]") {
	const size_t count = 3;
	int a[count];

	REQUIRE(sstd::distance(a, a) == 0);
	REQUIRE(sstd::distance(&a[0], &a[count]) == count);
	REQUIRE(sstd::distance(sstd::reverse_iterator<int*>(&a[count]),
	                       sstd::reverse_iterator<int*>(&a[0])) == count);
}
