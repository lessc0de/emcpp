//============================================================================
// Name        : main.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <string>
#include <cstring>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <array>
#include <limits>


using namespace std;


#define PERFORMANCE 0
#define PERFORMANCE_LOOPS (1000*1000*1000)
#define EXAMPLE 9

#if EXAMPLE != 6
#include "Lock.h"
#include "CyclicBuffer.h"
#include "CyclicBufferSimple.h"
#include "Stack.h"
#include "Memory.h"
#endif

#if EXAMPLE == 1

/**
 * Dummy lock
 */
class LockDummySimple {
public:

    LockDummySimple() {
        cout << "Locked context" << endl;
    }

    ~LockDummySimple() {
        cout << "Lock is freed" << endl;
    }

protected:

private:

};// class LockDummy

/*
 * Output of this code is going to be
 * Locked context
 * Lock is freed
 */
int main() {
#if (__cplusplus >= 201103) // use "auto" if C++11 or better
    auto myDummyLock = LockDummySimple();
#else
    LockDummySimple myDummyLock = LockDummySimple();
#endif
    return 0;
}

#endif // EXAMPLE == 1

#if EXAMPLE == 2

/**
 * Dummy lock
 */
class LockDummySimple {
public:

    LockDummySimple() {
        cout << "Locked context" << endl;
    }

    ~LockDummySimple() {
        cout << "Lock is freed" << endl;
    }

protected:

private:

};// class LockDummySimple

/*
 * Output of this code is going to be
 * Locked context
 * Lock is freed
 * End of main
 */
int main() {
    {
        auto myDummyLock = LockDummySimple();
    }

    cout << "End of main" << endl;
    return 0;
}

#endif // EXAMPLE == 2

#if EXAMPLE == 3

static inline void interruptDisable(void) {
    cout << "Disable" << endl;
}

static inline void interruptEnable(void) {
    cout << "Enable" << endl;
}

class SynchroObjectInterrupt {
public:
    void inline get() const {
        interruptDisable();
    }

    void inline release() const {
        interruptEnable();
    }
};// class SynchroObject

template<typename Mutex> class LockM {

    // A private member of the class
    const Mutex* mutex;

public:
    inline LockM(const Mutex& mutex) {
        this->mutex = &mutex;
        this->mutex->get();
    }

    inline ~LockM() {
        this->mutex->release();
    }
};// class Lock

static SynchroObjectInterrupt myInterruptDisable;

/*
 * Output of this code is going to be
 * Disable
 * Enable
 * End of main
 */
int main() {
    {
        auto myInterruptLock = LockM<SynchroObjectInterrupt>(
                myInterruptDisable);

    }
    return 0;
}
#endif // EXAMPLE == 3


#if EXAMPLE == 4

static inline void interruptDisable(void) {
    cout << "Disable" << endl;
}

static inline void interruptEnable(void) {
    cout << "Enable" << endl;
}

class SynchroObject {

    /**
     * Default constructor is private - I do not want any objects of this type
     */
    SynchroObject() {};

public:

    static inline void get() {
        interruptDisable();
    }

    static inline void release() {
        interruptEnable();
    }

};// class SynchroObject

/**
 * Declare a new type Lock which uses SynchroObject to
 * disable/enable interrupts
 */
typedef Lock<SynchroObject> Lock_t;

/*
 * Output of this code is going to be
 * Disable
 * Enable
 */
int main() {
    {
        /*
         * Just call a constructor, I skip declaring of a variable here
         */
        Lock_t();
    }
    return 0;
}

#endif // EXAMPLE === 4


#if EXAMPLE == 5




/**
 * Function returns number of elements in the cyclic buffer.
 * Compiler will fail if the value can not be calculated in compilation time.
 */
constexpr size_t calculateCyclicBufferSize() {
    return 10;
}

static CyclicBufferSimple<uint8_t, LockDummy, calculateCyclicBufferSize()> myCyclicBuffer;

int main() {
    for (int i : { 1, 3, 11 }) {
        myCyclicBuffer.add(i);
    }

    uint8_t val;
    while (myCyclicBuffer.remove(val)) {
        cout << (int) val << endl;
    }
    return 0;
}

#if 0
xor %esi,%esi
  inline void add(const ObjectType object) {
mov 0x2009c0(%rip),%rax
          return (index + 1);
mov %rsi,%r8
...............................................
  inline void add(const ObjectType object) {
mov %rcx,%rdx
cmovbe %rdi,%r8
      if (!isFull()) {
cmp %r8,%rax
...............................................
          data[this->tail] = object;
movb $0x0,0x6011a0(%rcx)
          return (index + 1);
mov %r8,%rdx
          this->tail = increment(this->tail);
mov %r8,0x2009a0(%rip)
#endif

#endif // EXAMPLE === 5


#if EXAMPLE == 6

#include "CyclicBufferC.h"

int main() {
    for (int i = 0;i < 4;i++) {
        CyclicBufferAdd(&myCyclicBuffer, i);
    }

    uint8_t val;
    while (CyclicBufferRemove(&myCyclicBuffer, &val)) {
        cout << (int) val << endl;
    };
    return 0;
}

#if 0
        return (index + 1);
  xor %esi,%esi
static inline bool CyclicBufferAdd(CyclicBuffer* cyclicBuffer, const CYCLIC_BUFFER_OBJECT_TYPE object) {
  mov 0x2009be(%rip),%rax        # 0x6011b0 <myCyclicBuffer+16>
        return (index + 1);
  mov %rsi,%r8
int main() {
  push %rbp
        return (index + 1);
  lea 0x1(%rcx),%rdi
...............................................
static inline bool CyclicBufferAdd(CyclicBuffer* cyclicBuffer, const CYCLIC_BUFFER_OBJECT_TYPE object) {
  mov %rcx,%rdx
  push %rbx
        return (index + 1);
  cmovbe %rdi,%r8
...............................................
    if (!CyclicBufferIsFull(cyclicBuffer)) {
  cmp %r8,%rax
...............................................
        cyclicBuffer->data[cyclicBuffer->tail] = object;
  movb $0x0,0x6011a0(%rcx)

        push   %r12
        xor    %esi,%esi
        mov    0x2009be(%rip),%rax
        mov    %rsi,%r8
        push   %rbp
        lea    0x1(%rdx),%rdi
        cmp    $0x9,%rdx
        mov    %rdx,%rcx
        push   %rbx
        cmovbe %rdi,%r8
        cmp    %r8,%rax
        je     40081c <main+0x3c>
        movb   $0x0,0x6011a0(%rdx)
        mov    %r8,%rcx
        mov    %r8,0x20099c(%rip)
        xor    %r12d,%r12d

#endif

#endif // EXAMPLE === 6



#if EXAMPLE == 7



/**
 * Function returns number of elements in the cyclic buffer.
 * Compiler will fail if the value can not be calculated in compilation time.
 */
constexpr size_t calculateCyclicBufferSize() {
    return 10;
}

static CyclicBuffer<uint8_t, LockDummy, calculateCyclicBufferSize()> myCyclicBuffer;

int main() {
#if (PERFORMANCE > 0)
    unsigned int count = PERFORMANCE_LOOPS;
    while (--count)
    {
        uint8_t val;
        myCyclicBuffer.add((uint8_t)count);
        myCyclicBuffer.remove(val);
        myCyclicBuffer.add((uint8_t)count);
        myCyclicBuffer.remove(val);
        myCyclicBuffer.add((uint8_t)count);
        myCyclicBuffer.remove(val);
        myCyclicBuffer.add((uint8_t)count);
        myCyclicBuffer.remove(val);
        myCyclicBuffer.add((uint8_t)count);
        myCyclicBuffer.remove(val);
        myCyclicBuffer.add((uint8_t)count);
        myCyclicBuffer.remove(val);
        myCyclicBuffer.add((uint8_t)count);
        myCyclicBuffer.remove(val);
        myCyclicBuffer.add((uint8_t)count);
        myCyclicBuffer.remove(val);
        myCyclicBuffer.add((uint8_t)count);
        myCyclicBuffer.remove(val);
        myCyclicBuffer.add((uint8_t)count);
        myCyclicBuffer.remove(val);
    }
#else
    // I want to inspect assembler code generated by the C++ compiler
    myCyclicBuffer.add(0);
    // I demonstrate here a range based loop from C++11
    for (int i : { 1, 2, 3 }) {
        myCyclicBuffer.add(i);
    }

    while (!myCyclicBuffer.isEmpty()) {
        uint8_t val;
        myCyclicBuffer.remove(val);
        cout << (int) val << endl;
    }
#endif
    return 0;
}

#endif // EXAMPLE == 7



#if EXAMPLE == 8

constexpr size_t calculateStackSize() {
    return 10;
}

typedef uint8_t DataBlock[64];

static Stack<DataBlock, LockDummy, calculateStackSize()> myMemoryPool;

int main() {
    DataBlock dummyDataBlock[10];

    myMemoryPool.push(&(dummyDataBlock[0]));
    (dummyDataBlock[0])[0] = 0;

    for (int i = 1;i < 10;i++) {
        myMemoryPool.push(&(dummyDataBlock[i]));
        (dummyDataBlock[i])[0] = i;
    }

    while (!myMemoryPool.isEmpty()) {
        DataBlock* dummyDataBlockRes;
        myMemoryPool.pop(&dummyDataBlockRes);
        cout << (int) (*dummyDataBlockRes)[0] << endl;
    }
    return 0;
}

#endif // EXAMPLE == 8


#if EXAMPLE == 9


static uint8_t dmaMemoryDummy[512];
static MemoryRegion dmaMemoryRegion("dmaMem", (uintptr_t)dmaMemoryDummy, sizeof(dmaMemoryDummy));

static_assert((sizeof(dmaMemoryDummy) >= MemoryAllocatorRaw::predictMemorySize(63, 3, 2)), "DmaMemoryDummy region is not large enough");
static MemoryAllocatorRaw dmaAllocator(dmaMemoryRegion, 63, 3, 2);

static MemoryPoolRaw<LockDummy, 7> dmaPool("dmaPool", dmaAllocator);

int main() {

    uint8_t* block;
    cout << "base=" << reinterpret_cast<uintptr_t>(dmaMemoryDummy) << endl;
    bool res;
    for (int i = 0;i < 5;i++)
    {
        res = dmaPool.allocate(&block);
        if (res) {
            cout << "\t" << i << " block=" << reinterpret_cast<uintptr_t>(block) << endl;
            dmaPool.free(block);
        }
    }

    return 0;
}

#endif // EXAMPLE == 9
