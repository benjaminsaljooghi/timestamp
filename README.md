# Coding Challenge Description

Consider the scenario of you have a new software application that needs to be promoted. You as a software developer is asked to allow your application to be installed run in potential customers’ computer system for a trial period. After the trial period your software application should cease to work.

Now you are tasked to design and implement a simple time expiry library that fulfill that purpose.

**Requirement**

The library API should provide functions for:

- [x]  A function to create a start timestamp when the software product is installed.
    - [x]  The timestamp is encrypted before written into a file to avoid the timestamp be tampered.
    - [x]  The timestamp will be used as reference when the function of check for expiry.
    - [x]  The library should not create a new timestamp file if a timestamp file already exists. If that happens, the function should return a failure condition.
- [x]  A function to check the expiry. This function returns a boolean to indicate the check result. It returns success condition except:
    - [x]  the timestamp file does not exist
    - [x]  the timestamp cannot be decrypted back to its original form (tampered).
    - [x]  the duration between current time and the decrypted timestamp is longer than the trial period (expired).
- [x]  A function to inspect the timestamp
- [x]  The library should be implemented as a dynamic link library (DLL), and its API should be designed and implemented in C++. Microsoft Visual C++ is the suggested tool to provide the solution.
- [x]  A simple console application should be provided to prove the function of the library.

The software developer is suggested to use standard and third-party C++ libraries wherever he/she see fit to accomplish the task.

All unspecified details are to be decided by the software developer who design and implement this library.

All non-requirement decisions made by the developer should be well documented.

The software developer should also provide a test plan for this library.

The library and the console application should be compiled without any warning and error.

# README

Three projects:

- `ConsoleApp` for testing `TimestampLibrary`
- `TimestampLibrary` for writing and reading confidential & authenticated timestamps to and from files.
- 3rd party library [cryptopp](https://github.com/weidai11/cryptopp) for [AES](https://www.cryptopp.com/wiki/Advanced_Encryption_Standard) implementation for confidential and authenticated ciphers of the timestamps.

Instructions:

- Open .sln
- Open `ConsoleApp`
- Adjust the three configuration parameters at the beginning of `ConsoleApp.cpp`
    - `timestamp_path`
    - `expires_after`
    - `sleep_for`
- Compile & run `ConsoleApp`
- Adjusting the configuration parameters as needed, run the following experiments:
    - Timestamp file already exists.
    - Console app sleeps for longer than expiry.
    - Console app sleeps for shorter than expiry.

- In the `TimestampLibrary.cpp`, on line 83 there is a commented attack on the cipher. By uncommenting this, you will notice the program detects the tampering of the cipher and reports this.

Proposed test plan:

- Automated unit tests for testing the above.
