#include "static/deque.h"

#include <catch/catch.hpp>

TEST_CASE("Construct a deque", "[constructor]") {
	SECTION("Default construct") {
		sstd::deque<char, 3> a;

		REQUIRE(a.size() == 0);
		REQUIRE(a.max_size() == 3);
	}

	SECTION("Value initialize") {
		SECTION("Size fits") {
			sstd::deque<int, 3> a(2, 55);

			REQUIRE(a.size() == 2);
			REQUIRE(a.max_size() == 3);
			REQUIRE(a.at(0) == 55);
			REQUIRE(a.at(1) == 55);
		}

		SECTION("Size too large") {
			sstd::deque<int, 3> a(4, 55);

			REQUIRE(a.size() == 3);
			REQUIRE(a.max_size() == 3);
			REQUIRE(a.at(0) == 55);
			REQUIRE(a.at(1) == 55);
			REQUIRE(a.at(2) == 55);
		}

		SECTION("From a range") {
			const size_t count = 3;
			int array[count]   = {0, 1, 2};

			sstd::deque<int, count> a(&array[0], &array[count]);

			REQUIRE(a.size() == count);
			REQUIRE(a.max_size() == count);
			REQUIRE(a.at(0) == 0);
			REQUIRE(a.at(1) == 1);
			REQUIRE(a.at(2) == 2);
		}
	}

	SECTION("Copy construct") {
		sstd::deque<unsigned char, 3> a(3, 8);

		SECTION("From an equivalent deque") {
			sstd::deque<unsigned char, 3> b(a);

			REQUIRE(b.size() == a.size());
			REQUIRE(b.max_size() == 3);
			REQUIRE(b.at(0) == a.at(0));
			REQUIRE(b.at(1) == a.at(1));
			REQUIRE(b.at(2) == a.at(2));
		}

		SECTION("From a smaller deque") {
			sstd::deque<unsigned char, 5> b(a);

			REQUIRE(b.size() == a.size());
			REQUIRE(b.max_size() == 5);
			REQUIRE(b.at(0) == a.at(0));
			REQUIRE(b.at(1) == a.at(1));
			REQUIRE(b.at(2) == a.at(2));
		}

		SECTION("From a larger deque") {
			sstd::deque<unsigned char, 2> b(a);

			REQUIRE(b.size() == 2);
			REQUIRE(b.max_size() == 2);
			REQUIRE(b.at(0) == a.at(0));
			REQUIRE(b.at(1) == a.at(1));
		}

		SECTION("From a compatible type") {
			sstd::deque<unsigned, 4> b(a);

			REQUIRE(b.size() == a.size());
			REQUIRE(b.max_size() == 4);
			REQUIRE(b.at(0) == a.at(0));
			REQUIRE(b.at(1) == a.at(1));
			REQUIRE(b.at(2) == a.at(2));
		}
	}
}

TEST_CASE("Assign a deque", "[assignment]") {
	sstd::deque<char, 3> a(3, 8);

	SECTION("From an equivalent deque") {
		sstd::deque<char, 3> b(1, 16);

		b = a;

		REQUIRE(b.size() == a.size());
		REQUIRE(b.max_size() == 3);
		REQUIRE(b.at(0) == a.at(0));
		REQUIRE(b.at(1) == a.at(1));
		REQUIRE(b.at(2) == a.at(2));
	}

	SECTION("From a smaller deque") {
		sstd::deque<char, 5> b(5, 32);

		b = a;

		REQUIRE(b.size() == a.size());
		REQUIRE(b.max_size() == 5);
		REQUIRE(b.at(0) == a.at(0));
		REQUIRE(b.at(1) == a.at(1));
		REQUIRE(b.at(2) == a.at(2));
	}

	SECTION("From a larger deque") {
		sstd::deque<char, 2> b(2, 48);

		b = a;

		REQUIRE(b.size() == 2);
		REQUIRE(b.max_size() == 2);
		REQUIRE(b.at(0) == a.at(0));
		REQUIRE(b.at(1) == a.at(1));
	}

	SECTION("From a compatible type") {
		sstd::deque<int, 4> b(4, 64);

		b = a;

		REQUIRE(b.size() == a.size());
		REQUIRE(b.max_size() == 4);
		REQUIRE(b.at(0) == a.at(0));
		REQUIRE(b.at(1) == a.at(1));
		REQUIRE(b.at(2) == a.at(2));
	}
}

TEST_CASE("Index into a deque", "[access]") {
	const size_t count = 3;
	sstd::deque<int, count> a(count, 16);

	SECTION("Using random access operator") {
		a[1] = 8;

		REQUIRE(a[0] == 16);
		REQUIRE(a[1] == 8);
		REQUIRE(a[2] == 16);
	}

	SECTION("Using bounds checked method") {
		a.at(1) = 8;

		SECTION("Within bounds") {
			REQUIRE(a.at(0) == 16);
			REQUIRE(a.at(1) == 8);
			REQUIRE(a.at(2) == 16);
		}

		SECTION("Out of bounds") {
			int overflow = a[0];
			a.at(count)  = ~overflow;

			REQUIRE(a.at(count) == a[0]);
		}
	}

	SECTION("Using front and back") {
		a.front() = 12;
		a.back()  = 24;

		REQUIRE(a[0] == 12);
		REQUIRE(a[a.size() - 1] == 24);
	}
}

TEST_CASE("Iterate over a deque", "[iterator]") {
	typedef sstd::deque<int, 3> deque;
	deque a(3, 4);

	SECTION("Forward direction") {
		for (deque::iterator it = a.begin(); it != a.end(); ++it) { *it = 16; }

		REQUIRE(a[0] == 16);
		REQUIRE(a[1] == 16);
		REQUIRE(a[2] == 16);
	}

	SECTION("Reverse direction") {
		int i = 0;

		for (deque::reverse_iterator it = a.rbegin(); it != a.rend();
		     ++it, ++i) {
			*it = i;
		}

		REQUIRE(a[0] == 2);
		REQUIRE(a[1] == 1);
		REQUIRE(a[2] == 0);
	}

	SECTION("With const-qualified array") {
		const deque b(3, 16);

		size_t count = 0;

		for (deque::const_iterator it = b.begin(); it != b.end(); ++it) {
			if (*it == 16) { ++count; }
		}

		REQUIRE(count == 3);
	}
}

TEST_CASE("Check the size and capacity of a deque", "[capacity]") {
	sstd::deque<int, 8> a(2, 16);

	REQUIRE(!a.empty());
	REQUIRE(a.size() == 2);
	REQUIRE(a.max_size() == 8);
}

TEST_CASE("Clear a deque of its contents", "[capacity]") {
	sstd::deque<int, 5> a(5, 128);

	REQUIRE(a.size() == 5);

	a.clear();

	REQUIRE(a.size() == 0);
}

TEST_CASE("Insert values into a deque", "[modifiers]") {
	sstd::deque<char, 16> a(4, 'a');

	SECTION("At the front") {
		a.insert(a.begin(), 'b');

		REQUIRE(a.front() == 'b');
	}

	SECTION("At the back") {
		a.insert(a.end(), 'c');

		REQUIRE(a.back() == 'c');
	}

	SECTION("In the middle") {
		a.insert(a.begin() + 2, 'd');

		REQUIRE(a[2] == 'd');
	}

	SECTION("Many at a time") {
		a.insert(a.begin() + 1, 2, 'e');

		REQUIRE(a[0] == 'a');
		REQUIRE(a[1] == 'e');
		REQUIRE(a[2] == 'e');
		REQUIRE(a[3] == 'a');
	}

	SECTION("With a range") {
		char b[4] = {'b', 'c', 'd', 'e'};

		a.insert(a.begin() + 1, b, b + 4);

		REQUIRE(a[0] == 'a');
		REQUIRE(a[1] == 'b');
		REQUIRE(a[2] == 'c');
		REQUIRE(a[3] == 'd');
		REQUIRE(a[4] == 'e');
	}
}

TEST_CASE("Remove values from a deque", "[modifiers]") {
	sstd::deque<int, 16> a(8, 'a');

	SECTION("Erase the first element") {
		a.insert(a.begin(), 'b');

		REQUIRE(a.size() == 9);
		REQUIRE(a.front() == 'b');

		a.erase(a.begin());

		REQUIRE(a.size() == 8);
		REQUIRE(a.front() == 'a');
	}

	SECTION("Erase a middle element") {
		a.insert(a.begin() + 3, 'b');

		REQUIRE(a.size() == 9);
		REQUIRE(a[3] == 'b');

		a.erase(a.begin() + 3);

		REQUIRE(a.size() == 8);
		REQUIRE(a[3] == 'a');
	}

	SECTION("Erase the last element") {
		a.insert(a.end(), 'b');

		REQUIRE(a.size() == 9);
		REQUIRE(a.back() == 'b');

		a.erase(a.end() - 1);

		REQUIRE(a.size() == 8);
		REQUIRE(a.back() == 'a');
	}

	SECTION("Erase a range of elements") {
		a.insert(a.begin(), 'b');

		a.erase(a.begin() + 1, a.end());

		REQUIRE(a.size() == 1);
		REQUIRE(a.back() == 'b');
	}
}

TEST_CASE("Push and pop values on the deque", "[modifiers]") {
	sstd::deque<int, 8> a;

	a.push_back(1);

	SECTION("On the front") {
		a.push_front(256);
		a.push_front(32);

		REQUIRE(a.size() == 3);
		REQUIRE(a[0] == 32);
		REQUIRE(a[1] == 256);

		a.pop_front();

		REQUIRE(a.size() == 2);
		REQUIRE(a[0] == 256);

		a.pop_front();

		REQUIRE(a.size() == 1);
	}

	SECTION("On the back") {
		a.push_back(256);
		a.push_back(32);

		REQUIRE(a.size() == 3);
		REQUIRE(a[1] == 256);
		REQUIRE(a[2] == 32);

		a.pop_back();
		a.pop_back();

		REQUIRE(a.size() == 1);
	}

	SECTION("Mix and match") {
		a.insert(a.begin(), 3, 2);
		a.push_front(4);
		a.push_back(8);
		a.push_front(16);
		a.push_back(32);

		// Should be ignored, we're full.
		a.push_front(64);

		int result[] = {16, 4, 2, 2, 2, 1, 8, 32};

		REQUIRE(sstd::equal(a.begin(), a.end(), result));
		REQUIRE(a.size() == 8);
	}
}

TEST_CASE("deque can be resized", "[modifiers]") {
	sstd::deque<int, 4> a(2, 16);

	SECTION("Resizing smaller decreases size") {
		a.resize(1);

		REQUIRE(a.size() == 1);
	}

	SECTION("Resizing bigger increases size") {
		SECTION("With default values") {
			a.resize(3);

			REQUIRE(a.size() == 3);
			REQUIRE(a[2] == 0);
		}

		SECTION("With initialized values") {
			a.resize(3, 32);

			REQUIRE(a.size() == 3);
			REQUIRE(a[2] == 32);
		}
	}
}

TEST_CASE("Test deques for equality", "[comparison]") {
	sstd::deque<char, 3> a(3, 16);

	SECTION("With an equal size deque") {
		SECTION("With same contents") {
			sstd::deque<char, 3> b(3, 16);

			REQUIRE(a == b);
		}

		SECTION("With differing contents") {
			sstd::deque<char, 3> b(3, 32);

			REQUIRE(a != b);
		}
	}

	SECTION("With a smaller size deque") {
		sstd::deque<char, 2> b(2, 16);

		REQUIRE(a != b);
	}

	SECTION("With a larger size deque") {
		sstd::deque<char, 5> b(5, 16);

		REQUIRE(a != b);
	}
}

TEST_CASE("Compare deques lexicographically", "[comparison]") {
	sstd::deque<int, 4> a(4, 16);

	SECTION("With an equal size deque") {
		SECTION("With lower value") {
			sstd::deque<int, 4> b(4, 8);

			REQUIRE(a > b);
			REQUIRE(a >= b);
			REQUIRE(b < a);
			REQUIRE(b <= a);
		}

		SECTION("With higher value") {
			sstd::deque<int, 4> b(4, 32);

			REQUIRE(a < b);
			REQUIRE(a <= b);
			REQUIRE(b > a);
			REQUIRE(b >= a);
		}

		SECTION("With equal value") {
			sstd::deque<int, 4> b(4, 16);

			REQUIRE(a <= b);
			REQUIRE(a >= b);
		}
	}

	SECTION("With a smaller size deque") {
		sstd::deque<int, 2> b(2, 16);

		REQUIRE(b < a);
	}

	SECTION("With a larger size deque") {
		sstd::deque<int, 8> b(8, 16);

		REQUIRE(a < b);
	}
}
