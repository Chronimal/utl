# C++17/20 utility code
A collection of newly refactored utility code I have written for and used in many personal projects over the years.

## Utilities

- **RingBuffer\<T, N>** is a fixed size circular buffer with an STL compliant interface implemented in a single [header file](include/ring_buffer.hpp). Usage examples and test harnesses are [here](test/ring_buffer/catch_ring_buffer.cpp).

- **TempBuffer\<L>** is a fixed size buffer typically allocated on the stack with dynamic allocation as fall back implemented in a single [header file](include/temp_buffer.hpp). Usage examples and test harnesses are [here](test/temp_buffer/catch_temp_buffer.cpp).

- **ScopeGuard** is a utility class with a long history. My first implementation was based on [Andrei Alexandrescu's](https://en.wikipedia.org/wiki/Andrei_Alexandrescu) excellent book [Modern C++ Design](https://en.wikipedia.org/wiki/Modern_C%2B%2B_Design) and his [Loki](https://sourceforge.net/projects/loki-lib/) library. With the advent of C++11 I wrote a simplified version based on a [talk](https://channel9.msdn.com/Shows/Going+Deep/C-and-Beyond-2012-Andrei-Alexandrescu-Systematic-Error-Handling-in-C) again by Andrei. Later, I learned about [folly](https://github.com/facebook/folly) by Facebook and ever since I use their vastly superior version. A copy with all dependencies on folly removed can be found [here](include/scope_guard.hpp) and the corresponding test harnesses are [here](test/scope_guard/catch_scope_guard.cpp).