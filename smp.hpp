#if !defined(__SMP_HPP__)
#define __SMP_HPP__
#include <iostream>
#include <thread>
#include <atomic>
using namespace std;

template <class T>
struct SyncObject
{
    atomic_uint64_t rc;
    atomic<T *> ptr;

    uint64_t get_refcount()
    {
        return (uint64_t)rc.load();
    }

    uint64_t increment_refcount() 
    {
       return rc.fetch_add(1); 
    }

    uint64_t decrement_refcount() 
    {
       return rc.fetch_sub(1); 
    }

    T *get_object_ptr()
    {
        return ptr.load();
    }

    SyncObject(T&& obj)
    {
        rc = 1;
        ptr = new T(obj);
    }

    SyncObject(SyncObject<T>&& rhs)
    {
#if defined(DEVMODE)
        cout << "[SyncObject] R-Value move constructor" << endl;
#endif // DEVMODE
        
        rhs.increment_refcount();
        this->ptr = rhs.get_object_ptr();
        this->rc = rhs.get_refcount();
    }

    SyncObject<T>& operator=(SyncObject<T>&& rhs) 
    {
#if defined(DEVMODE)
        cout << "[SyncObject] L-Value Move Assignment" << endl;
#endif // DEVMODE
        
        if (this != &rhs)
        {
            rhs.increment_refcount();
            this->ptr = rhs.get_object_ptr();
            this->rc = rhs.get_refcount();
        }

        return *this;
    }

    ~SyncObject() 
    {
#if defined(DEVMODE)
            cout << "[SyncObject] Deleting" << endl;
#endif // DEVMODE
            delete ptr.load();
    }

};

template <class T>
struct SharedObject
{
    SyncObject <T> *m_Sync;

    SharedObject() = default;

    SharedObject(const SharedObject<T>& obj)
    {
#if defined(DEVMODE)
        cout << "[SharedObject] L-Value Copy Constructor" << endl;
#endif // DEVMODE
        
        if (this != &obj) {
            obj.m_Sync->increment_refcount();
            this->m_Sync = obj.m_Sync;
        }
    }

    SharedObject(SharedObject<T>&& obj)
    {
#if defined(DEVMODE)
        cout << "[SharedObject] R-Value Move Constructor" << endl;
#endif // DEVMODE

        if (this != &obj) {
            obj.m_Sync->increment_refcount();
            this->m_Sync = obj.m_Sync;
        }
    }

    SharedObject(T&& obj)
    : m_Sync(new SyncObject<T>(std::move(obj)))
    {
#if defined(DEVMODE)
        cout << "[SharedObject] Inner R-Value Move Constructor" << endl;
#endif // DEVMODE
    }

    ~SharedObject() 
    {
        if (m_Sync->decrement_refcount() == 1) {
            delete m_Sync;
        } 
#if defined(DEVMODE)
        else {
            cout << "[SyncObject] Decremented References -> " << m_Sync->get_refcount() << endl;
        }
#endif // DEVMODE
        
    }
};
#endif // __SMP_HPP__
