#ifndef __POOL_HPP__
#define __POOL_HPP__

#include <cassert>
#include <type_traits>

template <class T>
class Pool
{
public:
    Pool(unsigned capacity);
    ~Pool();

    template <class U = T, class... Args>
    U *allocate(Args&& ... args);

    void release(T *object) noexcept;

private:
    union PoolObject
    {
        T data;
        PoolObject *next;
    };

    unsigned _capacity;
    unsigned char *_rawPool;
    PoolObject *_pool;
    PoolObject *_freeListHead;
};

template <class T>
Pool<T>::Pool(unsigned capacity) :
    _capacity(capacity),
    _pool(nullptr),
    _freeListHead(nullptr)
{
    assert(capacity > 0);

    // Allocate a raw chunk of bytes for memory
    _rawPool = new unsigned char[_capacity * sizeof(PoolObject)];
    _pool = reinterpret_cast<PoolObject *>(_rawPool);

    // Initialize the free list
    _freeListHead = _pool;
    for (unsigned i = 0; i < _capacity - 1; i++)
    {
        _pool[i].next = &_pool[i + 1];
    }
    _pool[_capacity - 1].next = nullptr;
}

template <class T>
Pool<T>::~Pool()
{
    assert(_rawPool);
    delete[] _rawPool;
}

template <class T> template <class U, class... Args>
U *Pool<T>::allocate(Args&& ... args)
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

template <class T>
void Pool<T>::release(T *object) noexcept
{
    assert(object);

    PoolObject *poolObject = reinterpret_cast<PoolObject *>(object);

    assert(poolObject >= _pool);
    assert(poolObject <= (_pool + sizeof(PoolObject) * (_capacity - 1)));

    // Add entry to free list
    PoolObject *oldHead = _freeListHead;
    _freeListHead = poolObject;
    poolObject->next = oldHead;
}

#endif
