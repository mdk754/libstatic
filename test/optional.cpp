#include "static/optional.h"

#include <catch/catch.hpp>

TEST_CASE("Construct an optional", "[constructor]") {
	SECTION("Default construct") {
		sstd::optional<char> a;

		REQUIRE(!a);
		REQUIRE(!a.has_value());
	}

	SECTION("NULL construct") {
		sstd::optional<char> a(sstd::nullopt);

		REQUIRE(!a);
		REQUIRE(!a.has_value());
	}

	SECTION("Copy construct") {
		sstd::optional<char> a;
		sstd::optional<char> b('B');

		SECTION("From an equivalent optional") {
			SECTION("Which is not initialized") {
				sstd::optional<char> test(a);

				REQUIRE(!test);
			}

			SECTION("Which is initialized") {
				sstd::optional<char> test(b);

				REQUIRE(test);
				REQUIRE(test.value() == 'B');
			}
		}

		SECTION("From a compatible optional type") {
			SECTION("Which is not initialized") {
				sstd::optional<int> test(a);

				REQUIRE(!test);
			}

			SECTION("Which is initialized") {
				sstd::optional<int> test(b);

				REQUIRE(test);
				REQUIRE(test.value() == 'B');
			}
		}
	}

	SECTION("Value construct") {
		sstd::optional<unsigned> a(0xffffffff);

		REQUIRE(a);
		REQUIRE(a.value() == 0xffffffff);
	}
}

TEST_CASE("Assign an optional type", "[assignment]") {
	sstd::optional<unsigned char> a;
	sstd::optional<unsigned char> b(42);

	SECTION("From null") {
		a = sstd::nullopt;
		b = sstd::nullopt;

		REQUIRE(!a);
		REQUIRE(!b);
	}

	SECTION("From an equivalent optional") {
		SECTION("Which is not initialized") {
			SECTION("If destination is not initialized") {
				sstd::optional<unsigned char> test;

				test = a;

				REQUIRE(!test);
			}

			SECTION("If destination is initialized") {
				b = a;

				REQUIRE(!b);
			}
		}

		SECTION("Which is initialized") {
			SECTION("If destination is not initialized") {
				a = b;

				REQUIRE(a);
				REQUIRE(a.value() == 42);
			}

			SECTION("If destination is initialized") {
				sstd::optional<unsigned char> test(2);

				test = b;

				REQUIRE(test);
				REQUIRE(test.value() == 42);
			}
		}
	}

	SECTION("From a compatible optional type") {
		SECTION("Which is not initialized") {
			SECTION("If destination is not initialized") {
				sstd::optional<unsigned> test;

				test = a;

				REQUIRE(!test);
			}

			SECTION("If destination is initialized") {
				sstd::optional<unsigned> test(2);

				test = a;

				REQUIRE(!test);
			}
		}

		SECTION("Which is initialized") {
			SECTION("If destination is not initialized") {
				sstd::optional<unsigned> test;

				test = b;

				REQUIRE(test);
				REQUIRE(test.value() == 42);
			}

			SECTION("If destination is initialized") {
				sstd::optional<unsigned> test(2);

				test = b;

				REQUIRE(test);
				REQUIRE(test.value() == 42);
			}
		}
	}
}

struct OptionalStruct {
	int value;
};

TEST_CASE("Access an optional type", "[access]") {
	SECTION("As a value") {
		SECTION("By reference") {
			sstd::optional<int> test(42);

			REQUIRE(test.value() == 42);
		}

		SECTION("By copy") {
			SECTION("Valid value") {
				sstd::optional<int> test(42);

				REQUIRE(test.value_or(84) == 42);
			}

			SECTION("Invalid value") {
				sstd::optional<int> test;

				REQUIRE(test.value_or(84) == 84);
			}
		}
	}

	SECTION("As a reference") {
		SECTION("Which is const") {
			const sstd::optional<int> test(2);

			REQUIRE(*test == 2);
		}

		SECTION("Which is non const") {
			sstd::optional<int> test(4);

			REQUIRE(*test == 4);
		}
	}

	SECTION("As a pointer") {
		SECTION("Which is const") {
			const OptionalStruct foo = {8};

			sstd::optional<OptionalStruct> test(foo);

			REQUIRE(test->value == 8);
		}

		SECTION("Which is non const") {
			OptionalStruct foo = {16};

			sstd::optional<OptionalStruct> test(foo);

			REQUIRE(test->value == 16);
		}
	}
}

TEST_CASE("Test an optional type", "[comparison]") {
	sstd::optional<unsigned> test(42);

	SECTION("Validity") {
		REQUIRE(test);
		REQUIRE(test.has_value());
	}

	SECTION("Reset") {
		test.reset();

		REQUIRE(!test);
	}
}

TEST_CASE("Compare for equality", "[comparison]") {
	sstd::optional<short> a;
	sstd::optional<short> b(2);

	SECTION("With optional") {
		SECTION("Same type") {
			sstd::optional<short> c(a);
			sstd::optional<short> d(b);

			REQUIRE(a != b);
			REQUIRE(b != a);
			REQUIRE(a == c);
			REQUIRE(b == d);
		}

		SECTION("Compatible type") {
			sstd::optional<int> c(a);
			sstd::optional<int> d(b);

			REQUIRE(a != b);
			REQUIRE(b != a);
			REQUIRE(a == c);
			REQUIRE(b == d);
		}
	}

	SECTION("With nullopt_t") {
		REQUIRE((a == sstd::nullopt));
		REQUIRE((sstd::nullopt == a));
		REQUIRE((b != sstd::nullopt));
		REQUIRE((sstd::nullopt != b));
	}

	SECTION("With value type") {
		REQUIRE(a != 0);
		REQUIRE(0 != a);
		REQUIRE(b == 2);
		REQUIRE(2 == b);
	}
}

TEST_CASE("Compare for greater/lesser", "[comparison]") {
	sstd::optional<short> a;
	sstd::optional<short> b(2);

	SECTION("With optional") {
		SECTION("Same type") {
			SECTION("Greater than") {
				sstd::optional<short> test;

				REQUIRE(test >= a);
				REQUIRE(a >= test);

				test = 2;

				REQUIRE(test >= b);
				REQUIRE(b >= test);

				test = 4;

				REQUIRE(test > a);
				REQUIRE(!(a > test));
				REQUIRE(test > b);
				REQUIRE(!(b > test));
			}

			SECTION("Less than") {
				sstd::optional<short> test;

				REQUIRE(test <= a);
				REQUIRE(a <= test);

				test = 2;

				REQUIRE(test <= b);
				REQUIRE(b <= test);

				test = 4;

				REQUIRE(!(test < a));
				REQUIRE(a < test);
				REQUIRE(!(test < b));
				REQUIRE(b < test);
			}
		}

		SECTION("Compatible type") {
			SECTION("Greater than") {
				sstd::optional<int> test;

				REQUIRE(test >= a);
				REQUIRE(a >= test);

				test = 2;

				REQUIRE(test >= b);
				REQUIRE(b >= test);

				test = 4;

				REQUIRE(test > a);
				REQUIRE(!(a > test));
				REQUIRE(test > b);
				REQUIRE(!(b > test));
			}

			SECTION("Less than") {
				sstd::optional<int> test;

				REQUIRE(test <= a);
				REQUIRE(a <= test);

				test = 2;

				REQUIRE(test <= b);
				REQUIRE(b <= test);

				test = 4;

				REQUIRE(!(test < a));
				REQUIRE(a < test);
				REQUIRE(!(test < b));
				REQUIRE(b < test);
			}
		}
	}

	SECTION("With nullopt_t") {
		SECTION("Greater than") {
			REQUIRE(!(a > sstd::nullopt));
			REQUIRE(!(sstd::nullopt > a));
			REQUIRE((a >= sstd::nullopt));
			REQUIRE((sstd::nullopt >= a));

			REQUIRE((b > sstd::nullopt));
			REQUIRE(!(sstd::nullopt > b));
			REQUIRE((b >= sstd::nullopt));
			REQUIRE(!(sstd::nullopt >= b));
		}

		SECTION("Less than") {
			REQUIRE(!(a < sstd::nullopt));
			REQUIRE(!(sstd::nullopt < a));
			REQUIRE((a <= sstd::nullopt));
			REQUIRE((sstd::nullopt <= a));

			REQUIRE(!(b < sstd::nullopt));
			REQUIRE((sstd::nullopt < b));
			REQUIRE(!(b <= sstd::nullopt));
			REQUIRE((sstd::nullopt <= b));
		}
	}

	SECTION("With value type") {
		short test = 42;

		SECTION("Greater than") {
			REQUIRE(!(a > test));
			REQUIRE(test > a);
			REQUIRE(!(a >= test));
			REQUIRE(test >= a);

			REQUIRE(!(b > test));
			REQUIRE(test > b);
			REQUIRE(!(b >= test));
			REQUIRE(test >= b);
		}

		SECTION("Less than") {
			REQUIRE(a < test);
			REQUIRE(!(test < a));
			REQUIRE(a <= test);
			REQUIRE(!(test <= a));

			REQUIRE(b < test);
			REQUIRE(!(test < b));
			REQUIRE(b <= test);
			REQUIRE(!(test <= b));
		}

		SECTION("Equal to") {
			const int equivalent = 2;

			REQUIRE(b <= equivalent);
			REQUIRE(equivalent <= b);
			REQUIRE(b >= equivalent);
			REQUIRE(equivalent >= b);
		}
	}
}
