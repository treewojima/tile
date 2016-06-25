#ifndef __POOL_HPP__
#define __POOL_HPP__

#include <cassert>
#include <type_traits>

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

// Poolable object that overrides operators new and delete
template <class T, unsigned N>
class PoolableObject
{
public:
    static void *operator new(std::size_t sz)
    {
        void *p = getPool().allocate();
        std::cout << "allocated object at " << p << std::endl;
        return p;
    }

    static void operator delete(void *p)
    {
        if (p)
        {
            std::cout << "releasing object at " << p << std::endl;
            getPool().release(static_cast<T *>(p));
        }
    }

protected:
    static Pool<T, N> &getPool()
    {
        static Pool<T, N> pool;
        return pool;
    }
};

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
    if (!_freeListHead) throw std::bad_alloc();

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
