#include <iostream>
#include <thread>

using namespace std;

#include "channels.hpp"

class Sender 
{

};

class Receiver
{

};

bool spin_on_condition(char condition, char *ptr) 
{
    while (*ptr != condition);
}

int main() 
{
    char area_0[32] = {};
    char area_1[32] = {};
    char *area = (char *)area_0;
    auto t_hdl = thread([&]
    {
        cout << "thread" << endl;
        auto ptr = area;

        *area = 'A';
        spin_on_condition('B', ptr);
        *ptr = 'C';

        *area = 'A';
        spin_on_condition('B', area);
        *area = 'C';
    });

    cout << "main" << endl;
    spin_on_condition('A', (char*)area_0);
    area = (char *)area_1;
    area_0[0] = 'B';
    spin_on_condition('A', (char*)area_1);
    area_1[0] = 'B';

    t_hdl.join();

    cout << area_0[0] << endl;
    cout << area_1[0] << endl;
}