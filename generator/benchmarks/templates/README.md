It works with unexpectedly similar performance

Python version employ precompiled and cached template without zip and it's quite fast
it takes 0.0003-0.0004 second per one file one core

C++ version compiled to release build with -O3 option
it takes 0.0001-0.0002 sec in the same conditions

The test machine has ssd and Core i7 CPU

It's about 2800 files per second in Python and more than 6000 files in C++. Nice.