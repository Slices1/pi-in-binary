# pi in base-2 or binary
txt file has 10,000 digits but is only accurate up to the 93rd. I will fix that...

# Usage
- Either use the pi-in-binary.txt
- Or compile and run the algorithm like so:
```c++
g++ -std=c++17 -O2 chudnovsky_pi.cpp -o pi_bin
./pi_bin 5 100   # 5 terms in series, 100 binary digits
```

# What
[](https://en.wikipedia.org/wiki/Chudnovsky_algorithm)
[](https://en.wikipedia.org/wiki/Binary_splitting)

A pure-integer [binary-splitting](https://en.wikipedia.org/wiki/Binary_splitting) implementation of [Chudnovsky algorithm](https://en.wikipedia.org/wiki/Chudnovsky_algorithm) in c++.

# Why binary
Outputs in binary because I couldn't find good sources of pi in binary. I want to look for statistically significant repetitions of single ditits like in 


# Limitations
I have yet to put in a system to check if it is actually accurate to the digits that it displays.
You'll have to repeatedely run the output with increasing terms until the digits stop changing, and even then you can't be certain.
