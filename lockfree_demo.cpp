#include <iostream>
#include <thread>
using namespace std;

#include "smp.hpp"

int main () {
    SharedObject <int> so_counter(88);
    cout << "so_counter->rc " << so_counter.m_Sync->rc.load() << " ->ptr " << so_counter.m_Sync->ptr.load() << endl;

    auto t = std::thread([&]{
        int *new_v = new int(99);

        SharedObject <int> copied = so_counter;
        cout << "copied->rc " << copied.m_Sync->rc.load() << " ->ptr " << copied.m_Sync->ptr.load() << endl;

        copied.m_Sync->ptr.store(new_v, std::memory_order_seq_cst);
        cout << "copied->rc " << copied.m_Sync->rc.load() << " ->ptr " << copied.m_Sync->ptr.load() << endl;
    });


    t.join();
    cout << "so_counter->rc " << so_counter.m_Sync->rc.load() << " ->ptr " << so_counter.m_Sync->ptr.load() << endl;
    return 0;
}