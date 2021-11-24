#include <iostream>
#include "../TimestampLibrary/TimestampLibrary.h"

#include <thread>
#include <iostream>
#include <chrono>
#include <ctime>
#include <string>
#include <sstream>
#include <fstream>

// Configuration. This should all be written to a config file instead of hard-coded.
std::string timestamp_path = "T:\\timestamp.txt"; // where to write the timestamp
auto expires_after = 10; // how long the software trial is in seconds
auto sleep_for = std::chrono::seconds(2); // how long the console will sleep before checking for trial expiry.

/// <summary>
/// Installs the software by writing the current time to a file.
/// </summary>
void install()
{
    bool success = Timestamp::write_timestamp(timestamp_path);
    if (!success) exit(1);
    std::cout << "software configured to expire after " << expires_after << " seconds." << std::endl;
}

/// <summary>
/// Sleeps for the configured amount of time.
/// </summary>
void sleep()
{
    std::cout << "sleeping for " << sleep_for.count() << " seconds" << std::endl;
    std::this_thread::sleep_for(sleep_for);
}

/// <summary>
/// Checks if the software has expired, and exits with error code 1 if the software has expired or there was a problem.
/// </summary>
void check()
{
    bool valid = Timestamp::check_expiry(timestamp_path, expires_after);
    if (!valid) exit(1);
}

int main()
{
    install();

    sleep();

    check();
}
