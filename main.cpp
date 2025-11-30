#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <future>
#include <chrono>
#include <syncstream>

void quick(const std::string& name)
{
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::osyncstream(std::cout) << name << " completed (1s)." << std::endl;
}

void slow(const std::string& name)
{
    std::this_thread::sleep_for(std::chrono::seconds(7));

    std::osyncstream(std::cout) << name << " completed (7s)." << std::endl;
}

void work()
{
    std::cout << "The work has begun." << std::endl;
    auto start = std::chrono::steady_clock::now();

    auto chainB_future = std::async(std::launch::async, []()
        {
            quick("B1");
            slow("B2");
            quick("B3");
        });

    quick("A1");
    slow("A2");
    quick("A3");

    chainB_future.get();

    quick("C");

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> diff = end - start;

    std::cout << "Total time: " << diff.count() << " seconds." << std::endl;
    std::cout << "Work is done!" << std::endl;
}

int main()
{
    work();

    return 0;
}