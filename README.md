# C++17/20 utility code
A collection of newly refactored utility code I have written for and used in many personal projects over the years.

## Utilities

- **RingBuffer\<T, N>** is a fixed size circular buffer with an STL compliant interface implemented in a single [header file](include/ring_buffer.hpp). Usage examples and test harnesses are [here](test/ring_buffer/catch_ring_buffer.cpp).

- **TempBuffer\<L>** is a fixed size buffer typically allocated on the stack with dynamic allocation as fall back implemented in a single [header file](include/temp_buffer.hpp). Usage examples and test harnesses are [here](test/temp_buffer/catch_temp_buffer.cpp).