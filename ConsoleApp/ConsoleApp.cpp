// ConsoleApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "../TimestampLibrary/TimestampLibrary.h"

#include <thread>
#include <iostream>
#include <chrono>
#include <ctime>
#include <string>
#include <sstream>
#include <fstream>

std::string timestamp_path = "T:\\timestamp.txt";
auto expires_after = 10; // seconds
auto sleep_for = std::chrono::seconds(2);

void install()
{
    bool success = Timestamp::write_timestamp(timestamp_path);
    if (!success) exit(1);
    std::cout << "software configured to expire after " << expires_after << " seconds." << std::endl;
}

void sleep()
{
    std::cout << "sleeping for " << sleep_for.count() << " seconds" << std::endl;
    std::this_thread::sleep_for(sleep_for);
}

void check()
{
    bool valid = Timestamp::check_expiry(timestamp_path, expires_after);
    if (!valid)  exit(1);
}

int main()
{
    install();

    sleep();

    check();
}
