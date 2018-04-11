# libstatic

A header-only implementation of various C++ template containers and algorithms
from the standard library, designed for embedded systems. Absolutely no heap
allocations, no RTTI, and no exceptions. It is designed with an emphasis on
simplicity, with a secondary objective of minimal code and data footprint.


## Getting Started

Those familiar with the C++ standard library will feel right at home. Whenever
possible, compatibility with the STL containers is paramount. Due to the lack of
dynamic memory allocation, sometimes a 100% compatible API will not be possible.


### Prerequisites

Any compiler compatible with at least the C++ 98 standard. Exceptions and RTTI
**must not** be used.


### Installing

Check out the repository, and include the files in your project. It is best if
you setup the `./inc` directory in your compilation include path, so that you
may include the headers without any filename collisions with other libraries.

```c++
#include <static/deque.h>
#include <static/vector.h>

...
```


### Example

A sample program which uses the library might look like this:

```c++
#include <iostream>
#include <static/vector.h>

sstd::vector<int, 100> vec;

size_t ExampleSizeOfVector(const sstd::vector<int>& some_vec) {
	return some_vec.size();
}

int main() {
	// Size: 100
	std::cout << "Size: " << ExampleSizeOfVector(vec) << std::endl;
}
```


## Documentation

You can generate doxygen documentation for the project using the appropriate
make target.

```bash
make docs
x-www-browser doc/html/index.html
```

This will generate the HTML output in the `./doc/html` folder and open in your default browser.


## Testing

The project uses make as its build system. Building and running the tests is
quite simple.

```bash
make test
```

The build output can be found in the `./bin` folder.


### Sanitizers

The Makefile also supports running the unit tests with various sanitizers.

```bash
make SANITIZER=asan test
make SANITIZER=ubsan test
```


### Changing Compiler

The build can be changed to use the clang compiler for compilation and linking.

```bash
make CXX=clang++ test
```


### Built With

[Catch](https://github.com/catchorg/Catch2/tree/Catch1.x) - Unit Test Framework


## Contributing

If you would like to contribute, please submit a pull request. Your pull request
should:

* Merge cleanly into `develop`
* Come with any necessary new tests
* Pass all tests
* Be formatted consistently with the rest of the project

Using `clang-format-6.0` or newer with the supplied style settings is strongly
recommended. There is also a make target to automatically style all code.

```bash
make fmt
```


## Versioning

This project uses [SemVer](https://semver.org/) for versioning. For the versions
available, see the [tags](https://github.com/mdk754/libstatic/tags) on this
repository.


## License

This project is released under the [MIT](LICENSE.md) license.
