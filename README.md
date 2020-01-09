![Build Status][travis-ci-build-status]

[travis-ci-build-status]: https://travis-ci.org/nanolith/homesim.svg?branch=master

Homebrew Digital Logic Simulator
================================

This project contains the Homebrew Digital Logic Simulator.  This simulator was
built to test some homebrew computing circuits built up from gate-level logic.

There are two pieces to this particular project: the homesim SDK and the homesim
binary.  The SDK makes it possible to build custom executables or demos based on
the homesim simulator.  The binary allows a user to read a homesim file and
either run test scenarios or extract a schematic.  Currently, homesim targets
the KiCad EESchema format.  It makes quite a few assumptions, outputs custom
parts, and this may not be useful to others. But, the source code can be
examined and extended to support custom extraction and possibly to other EDA
tools.

This project is primarily to support teaching the 8-bit computing class at Tampa
Hackerspace, but I am releasing it under an MIT license in case it is useful to
others (see [LICENSE.txt][license]).

[license]: LICENSE.txt

Building
-------- 

To build homesim and the SDK, a relatively recent C++ compiler that can support
the 2014 C++ standard is required, as is [cmake][CMake] and [minunit][minunit].
In particular, minunit needs to be installed and its [pkg-config][pkgconfig]
path needs to be available to [cmake][CMake] in order to build this tool.
Because this tool is meant to run tests and simulations of digital circuits,
running the test suites is mandatory for building this tool and the SDK.  A
version of this tool or SDK that failed unit testing wouldn't be especially
useful.

[cmake]: https://cmake.org/
[minunit]: https://github.com/nanolith/minunit
[pkgconfig]: https://www.freedesktop.org/wiki/Software/pkg-config/
