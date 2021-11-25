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
