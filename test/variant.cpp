#include "static/variant.h"

#include <catch/catch.hpp>

struct monostate_non_default_contructible {
	monostate_non_default_contructible(int) {}
};

TEST_CASE("Construct a variant", "[constructor]") {
	SECTION("Default construct") {
		sstd::variant<int, float> test;

		int* result = sstd::get_if<int>(test);

		REQUIRE(result);
		REQUIRE(*result == 0);
	}

	SECTION("Value construct") {
		sstd::variant<bool, int, float> test1(true);
		sstd::variant<bool, int, float> test2(42);
		sstd::variant<bool, int, float> test3(2.4f);

		REQUIRE(sstd::get<bool>(test1) == true);
		REQUIRE(sstd::get<int>(test2) == 42);
		REQUIRE(sstd::get<float>(test3) == 2.4f);
	}

	SECTION("Copy construct") {
		sstd::variant<unsigned, int, char> a(5);
		sstd::variant<unsigned, int, char> b(a);

		REQUIRE(sstd::get<int>(b) == 5);
	}

	SECTION("With monostate") {
		sstd::variant<sstd::monostate, monostate_non_default_contructible> test;

		REQUIRE(test.index() == 0);
	}
}

TEST_CASE("Assign a variant", "[assignment]") {
	sstd::variant<unsigned, bool, char> a('a');

	SECTION("From an equivalent variant") {
		sstd::variant<unsigned, bool, char> test('b');
		test = a;

		REQUIRE(sstd::get<char>(test) == 'a');
	}

	SECTION("From a compatible variant") {
		sstd::variant<unsigned, bool, char> test(false);
		test = a;

		REQUIRE(sstd::get<char>(test) == 'a');
	}
}

TEST_CASE("Index into a variant", "[access]") {
	sstd::variant<int, bool, char> test(true);

	SECTION("Get the current index") {
		REQUIRE(test.index() == 1);

		REQUIRE(!sstd::get_if<int>(test));
		REQUIRE(sstd::get<bool>(test) == true);
		REQUIRE(!sstd::get_if<char>(test));
	}

	SECTION("Check which alternative is held") {
		REQUIRE(!sstd::holds_alternative<int>(test));
		REQUIRE(sstd::holds_alternative<bool>(test));
		REQUIRE(!sstd::holds_alternative<char>(test));
	}

	SECTION("Get a pointer to the value") {
		SECTION("By type") {
			REQUIRE(!sstd::get_if<int>(test));
			REQUIRE(sstd::get_if<bool>(test));
			REQUIRE(!sstd::get_if<char>(test));
		}

		SECTION("By index") {
			REQUIRE(!sstd::get_if<0>(test));
			REQUIRE(sstd::get_if<1>(test));
			REQUIRE(!sstd::get_if<2>(test));
		}
	}

	SECTION("Get a reference to the value") {
		SECTION("By type") { REQUIRE(sstd::get<bool>(test) == true); }

		SECTION("By index") { REQUIRE(sstd::get<1>(test) == true); }
	}
}

struct variant_visitor_double {
	sstd::variant<int, char> operator()(int val) const { return val * 2; }

	sstd::variant<int, char> operator()(char val) const {
		return static_cast<char>(val * 2);
	}

	template<class T>
	sstd::variant<int, char> operator()(const T& val) const {
		return val;
	}
};

TEST_CASE("Visit variants", "[access]") {
	const size_t count = 4;

	sstd::variant<int, char> vars[count];
	vars[0] = 2;
	vars[1] = '0';
	vars[2] = 16;
	vars[3] = '9';

	for (size_t i = 0; i < count; ++i) {
		vars[i] = sstd::visit(variant_visitor_double(), vars[i]);
	}

	REQUIRE(sstd::get<int>(vars[0]) == 4);
	REQUIRE(sstd::get<char>(vars[1]) == '`');
	REQUIRE(sstd::get<int>(vars[2]) == 32);
	REQUIRE(sstd::get<char>(vars[3]) == 'r');
}

TEST_CASE("Insert values into a variant", "[modifiers]") {
	sstd::variant<int, bool, char, float, unsigned> test;

	test = 5;

	REQUIRE(test.index() == 0);
	REQUIRE(sstd::get<int>(test) == 5);

	test = false;

	REQUIRE(test.index() == 1);
	REQUIRE(sstd::get<bool>(test) == false);

	test = 'b';

	REQUIRE(test.index() == 2);
	REQUIRE(sstd::get<char>(test) == 'b');

	test = 6.8f;

	REQUIRE(test.index() == 3);
	REQUIRE(sstd::get<float>(test) == 6.8f);

	test = 4u;

	REQUIRE(test.index() == 4);
	REQUIRE(sstd::get<unsigned>(test) == 4u);
}

TEST_CASE("Test variants for equality", "[comparison]") {
	sstd::variant<int, char> a(32);
	sstd::variant<int, char> b('b');

	SECTION("Equal to") {
		sstd::variant<int, char> c(a);
		sstd::variant<int, char> d(b);

		REQUIRE(a == c);
		REQUIRE(b == d);
	}

	SECTION("Not equal to") {
		sstd::variant<int, char> c(16);
		sstd::variant<int, char> d('d');

		REQUIRE(a != b);
		REQUIRE(a != c);
		REQUIRE(b != a);
		REQUIRE(b != d);
	}
}

TEST_CASE("Compare variants lexicographically", "[comparison]") {
	sstd::variant<int, char> a(2);
	sstd::variant<int, char> b(8);
	sstd::variant<int, char> c('a');
	sstd::variant<int, char> d('b');

	SECTION("Less than") {
		SECTION("Same type") {
			REQUIRE(a < b);
			REQUIRE(a <= b);
			REQUIRE(c < d);
			REQUIRE(c <= d);
		}

		SECTION("Different type") {
			REQUIRE(a < c);
			REQUIRE(a <= c);
			REQUIRE(b < d);
			REQUIRE(b <= d);
		}
	}

	SECTION("Greater than") {
		SECTION("Same type") {
			REQUIRE(b > a);
			REQUIRE(b >= a);
			REQUIRE(d > c);
			REQUIRE(d >= c);
		}

		SECTION("Different type") {
			REQUIRE(c > a);
			REQUIRE(c >= a);
			REQUIRE(d > b);
			REQUIRE(d >= b);
		}
	}

	SECTION("Equal") {
		sstd::variant<int, char> test1(a);
		sstd::variant<int, char> test2(c);

		REQUIRE(a <= test1);
		REQUIRE(a >= test1);
		REQUIRE(test2 <= c);
		REQUIRE(test2 >= c);
	}
}

TEST_CASE("Get traits of a variant") {
	SECTION("Size") {
		REQUIRE(
		   (sstd::variant_size<sstd::variant<char, bool, int> >::value == 3));
	}

	SECTION("Type") {
		REQUIRE(
		   (sstd::is_same<
		      sstd::variant_alternative<0,
		                                sstd::variant<char, bool, int> >::type,
		      char>::value));
		REQUIRE(
		   (sstd::is_same<
		      sstd::variant_alternative<1,
		                                sstd::variant<char, bool, int> >::type,
		      bool>::value));
		REQUIRE(
		   (sstd::is_same<
		      sstd::variant_alternative<2,
		                                sstd::variant<char, bool, int> >::type,
		      int>::value));
	}
}
