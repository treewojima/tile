#ifndef __POOL_HPP__
#define __POOL_HPP__

#include "defines.hpp"

#include <cassert>
#include <type_traits>

#include "exceptions.hpp"

#ifdef _DEBUG
#   include "logger.hpp"
#   define _DEBUG_POOL
#endif

// Compile-time-sized pool class
template <class T, unsigned N>
class Pool
{
public:
    Pool();
    ~Pool();

    template <class U = T>
    U *allocate();

    void release(T *object) noexcept;

private:
    union PoolObject
    {
        T data;
        PoolObject *next;
    };

    unsigned char *_rawPool;
    PoolObject *_pool;
    PoolObject *_freeListHead;
};

// Simple poolable object that overrides operators new and delete
template <class T, unsigned N>
class PoolableObject
{
public:
    static void *operator new(std::size_t sz)
    {
        void *p = getPool().allocate();  
#ifdef _DEBUG_POOL
        LOG_DEBUG << "allocated object at " << p;
#endif
        return p;
    }

    static void operator delete(void *p)
    {
        if (p)
        {
#ifdef _DEBUG_POOL
            LOG_DEBUG << "releasing object at " << p;
#endif
            getPool().release(static_cast<T *>(p));
        }
#ifdef _DEBUG
        else
        {
            LOG_DEBUG << "deleting null object";
        }
#endif
    }

protected:
    static Pool<T, N> &getPool()
    {
        static Pool<T, N> pool;
        return pool;
    }
};

namespace Exceptions
{
    class PoolOutOfMemory : public std::bad_alloc {};
}

template <class T, unsigned N>
Pool<T, N>::Pool() :
    _rawPool(nullptr),
    _pool(nullptr),
    _freeListHead(nullptr)
{
    static_assert(N > 0,
                  "pool capacity must be greater than zero");

    // Allocate a raw chunk of bytes for memory
    _rawPool = new unsigned char[N * sizeof(PoolObject)];
    _pool = reinterpret_cast<PoolObject *>(_rawPool);

    // Initialize the free list
    _freeListHead = _pool;
    for (unsigned i = 0; i < N - 1; i++)
    {
        _pool[i].next = &_pool[i + 1];
    }
    _pool[N - 1].next = nullptr;
}

template <class T, unsigned N>
Pool<T, N>::~Pool()
{
    assert(_rawPool);
    delete[] _rawPool;
}

template <class T, unsigned N> template <class U>
U *Pool<T, N>::allocate()
{
    static_assert(std::is_base_of<T, U>::value,
                  "can only allocate objects of or subclassed from pool base type");
    static_assert(sizeof(T) == sizeof(U),
                  "can only allocate subclasses of identical size to pool base type");

    // If free list is empty, nothing left to allocate
    if (!_freeListHead) throw Exceptions::PoolOutOfMemory();

    // Pop an entry off the free list
    U *object = reinterpret_cast<U *>(&_freeListHead->data);
    _freeListHead = _freeListHead->next;
    return object;
}

template <class T, unsigned N>
void Pool<T, N>::release(T *object) noexcept
{
    assert(object);

    PoolObject *poolObject = reinterpret_cast<PoolObject *>(object);

    assert(poolObject >= _pool);
    assert(poolObject <= (_pool + sizeof(PoolObject) * (N - 1)));

    // Add entry to free list
    PoolObject *oldHead = _freeListHead;
    _freeListHead = poolObject;
    poolObject->next = oldHead;
}

#endif
