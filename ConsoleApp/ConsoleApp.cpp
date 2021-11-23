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

int expires_after = 10; // seconds
auto sleep_for = std::chrono::seconds(2);


void configure()
{
    bool success = WriteTimestamp();
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
    bool valid = CheckTimestamp(expires_after);
    if (!valid)  exit(1);
}

int main()
{
    configure();

    sleep();

    check();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
