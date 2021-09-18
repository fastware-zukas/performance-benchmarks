# Changelog

This is a list of notable changes to libdivide.

## [3.1](https://github.com/ridiculousfish/libdivide/releases/tag/v3.1) - not yet released
* ENHANCEMENT
  * Add fuzzing support (requires clang) ([#60](https://github.com/ridiculousfish/libdivide/pull/60))

## [3.0](https://github.com/ridiculousfish/libdivide/releases/tag/v3.0) - 2019-10-16
* BREAKING
  * ```libdivide.h``` now requires C++11 or later
* BUG FIXES
  * Support all 32-bit and 64-bit integer types in C++ ([#58](https://github.com/ridiculousfish/libdivide/issues/58))
  * Fix cross compilation ([#59](https://github.com/ridiculousfish/libdivide/issues/59))
* ENHANCEMENT
  * Add support for CMake ```find_package(libdivide)```

## [2.0](https://github.com/ridiculousfish/libdivide/releases/tag/v2.0) - 2019-06-22
* BREAKING
  * Removed unswitch functionality ([#46](https://github.com/ridiculousfish/libdivide/issues/46))
  * Renamed macro ```LIBDIVIDE_USE_SSE2``` to ```LIBDIVIDE_SSE2```
  * Renamed ```divider::recover_divisor()``` to ```divider::recover()```
* BUG FIXES
  * Remove ```_udiv128()``` as not yet supported by clang-cl and icl compilers
  * Fix C++ linker issue caused by anonymous namespace ([#54](https://github.com/ridiculousfish/libdivide/pull/54))
  * Fix clang-cl (Windows) linker issue ([#56](https://github.com/ridiculousfish/libdivide/issues/56))
* ENHANCEMENT
  * Add AVX2 & AVX512 vector division
  * Speed up SSE2 ```libdivide_mullhi_u64_vector()```
  * Support +1 & -1 signed branchfree dividers ([4a1d5a7](https://github.com/ridiculousfish/libdivide/commit/4a1d5a7008af7f401f8f39c2f44f3dd0116a9839))
  * Speed up unsigned branchfull power of 2 dividers ([2422199](https://github.com/ridiculousfish/libdivide/commit/24221996a082774bd486b04f329a1cdd1a80c8df))
  * Simplify C++ templates
  * Simplify ```more``` bit flags of the ```libdivide_*_t``` structs
  * Get rid of ```MAYBE_VECTOR()``` hack
* TESTING
  * ```tester.cpp```: Convert to modern C++
  * ```tester.cpp```: Add more test cases
  * ```benchmark_branchfreee.cpp```: Convert to modern C++
  * ```benchmark.c```: Prevent compilers from optmizing too much
* BUILD
  * Automatically detect SSE2/AVX2/AVX512
* DOCS
  * ```doc/C-API.md```: Add C API reference
  * ```doc/CPP-API.md```: Add C++ API reference
  * ```README.md```: Add [vector division](https://github.com/ridiculousfish/libdivide#vector-division) and [performance tips](https://github.com/ridiculousfish/libdivide#performance-tips) sections
  
## [1.1](https://github.com/ridiculousfish/libdivide/releases/tag/v1.1) - 2019-05-29
* BUG FIXES
  * Fix bug in ```libdivide_128_div_64_to_64()``` ([#45](https://github.com/ridiculousfish/libdivide/issues/45))
  * Fix MSVC ARM 64-bit bug ([07931e9](https://github.com/ridiculousfish/libdivide/commit/07931e9cb670311ba67cd6a16fdbcb67bb97a592))
  * Fix ```-Wshift-count-overflow``` warning on avr CPU architecture ([#41](https://github.com/ridiculousfish/libdivide/pull/41))
  * Fix ```-Wshadow``` warning in ```libdivide_s32_do()```
  * Fix ```-Wignored-attributes``` warnings when compiling SSE2 code using GCC 9
* ENHANCEMENT
  * ```libdivide_128_div_64_to_64()```: optimize using ```_udiv128()``` for MSVC 2019 or later
  * ```libdivide_128_div_64_to_64()```: optimize using ```__uint128_t``` for GCC/Clang on 64-bit CPU architectures
  * Add ```LIBDIVIDE_VERSION``` macro to ```libdivide.h```
  * Clean up SSE2 code in ```libdivide.h```
  * Increase runtime of test cases in ```primes_benchmark.cpp```
* BUILD
  * Remove windows directory with legacy Visual Studio project files
  * Move test programs to test directory

## [1.0](https://github.com/ridiculousfish/libdivide/releases/tag/v1.0) - 2018-01-21
* BREAKING
  * Branchfull divider must not be ```0``` ([#38](https://github.com/ridiculousfish/libdivide/pull/38))
  * Branchfree divider must not be ```-1```, ```0```, ```1``` ([#38](https://github.com/ridiculousfish/libdivide/pull/38))
* ENHANCEMENT
  * Add proper error handling ([#38](https://github.com/ridiculousfish/libdivide/pull/38))
  * Add C++ support for ```/=``` operator
  * Speedup 64-bit divisor recovery by up to 30%
  * Simplify C++ templates
  * Add include guards to ```libdivide.h```!
  * Get rid of ```goto``` in ```libdivide_128_div_64_to_64()```
  * Use ```#if defined(MACRO)``` instead of ```#if MACRO```
  * Silence compiler warnings from crash functions
* TESTING
  * Tests should ```exit(1)``` on error, required by ```make test```
  * Silence unused parameter warnings
  * Silence GCC 7.2.0 maybe uninitialized warnings
  * Silence unused return value warning
* BUILD
  * Port build system from ```make``` to ```CMake```
  * Automatically detect if the CPU and compiler support SSE2
  * Automatically enable C++11
* DOCS
  * Update build instructions in ```README.md```
  * Update benchmark section with branchfree divider
  * Add C example section
  * Add C++ example section
  * Add "Branchfull vs branchfree" section
  * Add section about unswitching
  * New ```CHANGELOG.md```file
