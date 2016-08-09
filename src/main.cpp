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
#include <atomic>
#include <thread>
#include <cstdint>
#include <memory>

#include <stdarg.h>
#ifndef __APPLE__
#include <omp.h>
#endif

#include <sys/time.h>
using namespace std;


#define PERFORMANCE 0
#define PERFORMANCE_LOOPS (1000*1000*1000)
#define EXAMPLE 10

#if (EXAMPLE != 6) && (EXAMPLE != 10)
#include "Lock.h"
#include "CyclicBuffer.h"
#include "CyclicBufferSimple.h"
#include "Stack.h"
#include "Memory.h"
#include "HardwareC.h"
#include "Hardware.h"
#include "Timers.h"
#include "Log.h"
#include "OpenMP.h"
#include "Pipeline.h"
#include "FixedPoint.h"
#endif

#if (EXAMPLE == 10)
#include "HashTable.h"
#endif

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
int testDummyLock1() {
#if (__cplusplus >= 201103) // use "auto" if C++11 or better
    auto myDummyLock = LockDummySimple();
#else
    LockDummySimple myDummyLock = LockDummySimple();
#endif
    cout << "Protected context" << endl;
    return 0;
}


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

/**
 * Function returns number of elements in the cyclic buffer.
 * Compiler will fail if the value can not be calculated in compilation time.
 */
constexpr size_t calculateCyclicBufferSize() {
    return 10;
}

static CyclicBufferSimple<uint8_t, LockDummy, calculateCyclicBufferSize()> myCyclicBufferSimple;

int testCyclicBufferSimple() {
    for (int i : { 1, 3, 11 }) {
        myCyclicBufferSimple.add(i);
    }

    uint8_t val;
    while (myCyclicBufferSimple.remove(val)) {
        cout << (int) val << endl;
    }
    return 0;
}

int findFirst(int x)
{
    if (x == 0)
      return 0;

    int t = 1;
    int r = 1;
    while ((x & t) == 0) {
        t = t << 1;
        r = r + 1;
    }
    return r;
}

uint32_t Tick = 0;
int MyTickIsr()
{
    Tick++;

    printf("%s", __FUNCTION__);
    return 0;
}

int negate(int x) {
    int result = 0;
    int sign = x < 0 ? 1 : -1;
    while (x != 0) {
        result += sign;
        x += sign;
    }
    return result;
}

int countBits(int n)
{
    int count=0;

    while(n)
    {
        count++;
        n = n&(n-1);
    }

    return count;

}


void mistake()
{
    unsigned int i;
    for (i = 100; i <= 0; --i) {
        printf("%d\n", i);
    }
}

#if EXAMPLE == 5
#include "CyclicBufferC.h"

int mainExample6() {
    for (int i = 0;i < 4;i++) {
        CyclicBufferAdd(&myCyclicBuffer, i);
    }

    uint8_t val;
    while (CyclicBufferRemove(&myCyclicBuffer, &val)) {
        cout << (int) val << endl;
    };
    return 0;
}
#endif



static CyclicBuffer<uint_fast8_t, LockDummy, calculateCyclicBufferSize()> myCyclicBuffer;

static uint32_t myDynamicCyclicBufferData[calculateCyclicBufferSize()];
CyclicBufferDynamic<uint32_t, LockDummy> myDynamicCyclicBuffer(calculateCyclicBufferSize(), &myDynamicCyclicBufferData);

int testCyclicBuffer() {
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
    return 0;
}

static CyclicBufferFast<uint_fast8_t, LockDummy, calculateCyclicBufferSize()> myCyclicBufferFast;


int testCyclicBufferFast() {
    // I want to inspect assembler code generated by the C++ compiler
    myCyclicBufferFast.add(0);
    // I demonstrate here a range based loop from C++11
    for (int i : { 1, 2, 3 }) {
        myCyclicBuffer.add(i);
    }

    while (!myCyclicBufferFast.isEmpty()) {
        uint8_t val;
        myCyclicBufferFast.remove(val);
        cout << (int) val << endl;
    }
    return 0;
}


constexpr size_t calculateStackSize() {
    return 10;
}

typedef uint8_t DataBlock[64];

static Stack<DataBlock, LockDummy, calculateStackSize()> myMemoryPool;

int mainExample8() {
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


static uint8_t dmaMemoryDummy[512];
static MemoryRegion dmaMemoryRegion("dmaMem", (uintptr_t)dmaMemoryDummy, sizeof(dmaMemoryDummy));

static_assert((sizeof(dmaMemoryDummy) >= MemoryAllocatorRaw::predictMemorySize(63, 3, 2)), "DmaMemoryDummy region is not large enough");
static MemoryAllocatorRaw dmaAllocator(dmaMemoryRegion, 63, 3, 2);

static MemoryPoolRaw<LockDummy, 7> dmaPool("dmaPool", dmaAllocator);

static int mainExample9() {

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


#ifdef REAL_HARDWARE
static struct PIO *pios = (PIO*)0xFFFFF200;
#else
static struct PIO pioDummy[5];
static struct PIO *pios = pioDummy;
#endif


static void enableOutput(PIO_NAME name, int pin, int value) {
    struct PIO *pio = &pios[name];
    uint32_t mask = 1 << pin;
    if (value) {
        pio->PIO_SODR = mask;
    }
    else {
        pio->PIO_CODR = mask;
    }
    pio->PIO_PER = mask;
    pio->PIO_OER = mask;
}

static void mainExample10() {
    enableOutput(PIO_A, 2, 1);
}


class HardwarePIO : HardwareModule {
public:
    HardwarePIO(const uintptr_t address) {
        interface = (struct Interface*)address;
    }
    ~HardwarePIO() {}

    struct Interface {
        HardwareRegister32WO PIO_PER  ;
        HardwareRegister32WO PIO_PDR  ;
        HardwareRegister32RO PIO_PSR  ;
        HardwareRegister32NotUsed RESERVED ;
        HardwareRegister32WO PIO_OER  ;
        HardwareRegister32WO PIO_ODR  ;
        HardwareRegister32RO PIO_OSR  ;
        HardwareRegister32NotUsed RESERVED ;
        HardwareRegister32NotUsed RESERVED ;
        HardwareRegister32NotUsed RESERVED ;
        HardwareRegister32NotUsed RESERVED ;
        HardwareRegister32NotUsed RESERVED ;
        HardwareRegister32WO PIO_SODR ;
        HardwareRegister32WO PIO_CODR ;
    };
    //static_assert((sizeof(struct Interface) == (14*sizeof(uint32_t))), "struct interface is of wrong size, broken alignment?");
    enum Name {A, B, C, D, E, F, LAST};

    inline Interface& getInterface(Name name) const {return interface[name];};

    inline void enableOutput(Name name, int pin, int value);
protected:
    struct Interface* interface;
};

inline void HardwarePIO::enableOutput(Name name, int pin, int value)
{
    struct Interface& interface = getInterface(name);
    uint32_t mask = 1 << pin;
    if (value) {
        interface.PIO_SODR = mask;
    }
    else {
        interface.PIO_CODR = mask;
    }
    interface.PIO_PER = mask;
    interface.PIO_OER = mask;
}

static HardwarePIO hardwarePIO(reinterpret_cast<uintptr_t>(pioDummy));

static void mainExample11() {
    hardwarePIO.enableOutput(HardwarePIO::A, 2, 1);
    //uint32_t per = hardwarePIO.getInterface(HardwarePIO::A).PIO_PER;
    cout << pioDummy[0].PIO_SODR << endl;
    cout << pioDummy[0].PIO_CODR << endl;
    cout << pioDummy[0].PIO_PER << endl;
    cout << pioDummy[0].PIO_OER << endl;
}

static const char *helloWorldStr = 0;

#if 0
class HelloWorld {
public:
    HelloWorld() {
        if (helloWorldStr == 0)
            cout << "Hello, world!" << endl;
        else
            cout << helloWorldStr << endl;
    }
};
#endif

class HelloWorld {
public:
    HelloWorld() {
        printHello();
    }
protected:
    void printHello() {
        cout << "Hello, world!" << endl;
    }
};

static HelloWorld helloWorld;

int mainExample12()
{
    cout << "Hello from main()!" << endl;
    return 0;
}

static void mainExpirationHandler(const Timer& timer) {
    TimerID timerId = timer.getId();
    uintptr_t data = timer.getApplicationData();
    cout << "Expired id=" << timerId << ",appdata=" << data << endl;
}


TimerLockDummy timerLock;
static TimerList timerList(3, 3, mainExpirationHandler, timerLock);
int testTimer()
{
    SystemTime currentTime = 0;
    for (int i = 0;i < 3;i++) {
        SystemTime nearestExpirationTime;
        TimerError err = timerList.startTimer(currentTime, nearestExpirationTime, i);
        if (err == TimerError::Ok) {
            cout << "nearestExpirationTime=" << nearestExpirationTime << endl;
        }
        else {
            cout << "timer start failed for timer " << i << endl;
        }
    }

    timerList.processExpiredTimers(3);
    return 0;
}

template<const uint32_t N> struct factorial
{
    static constexpr uint32_t value = N * factorial<N - 1>::value;
};

template<>struct factorial<0>
{
    static constexpr uint32_t value = 1;
};


void testLog(void) {
    LOG_INFO("This is info %d", 1);
    LOG_ERROR("This is error %d", 2);
}

int sum()
{
    return 0;
}

template<typename ... Types>
int sum (int first, Types ... rest)
{
    return first + sum(rest...);
}

const int SUM = sum(1, 2, 3, 4, 5);





const int TIMERS_COUNT = 3;
const int TIMERS_SIZE = 3;
uint32_t dummyTimers[TIMERS_SIZE][TIMERS_COUNT];

HardwareTimer *myHardwareTimers = new (&dummyTimers[0][0]) HardwareTimer[TIMERS_COUNT];

void testHardwareTimers(void) {
    for (int i = 0;i < TIMERS_COUNT;i++) {
        myHardwareTimers[i].start();
    }
}


void testOpenMP() {
    uint8_t packet[1024];
    uint8_t encPacket[1024];
    encryptPacket(packet, encPacket, sizeof(encPacket));
}

void testOpenMpReduction(void) {
    int a = 0;
    #pragma omp parallel reduction (+:a)
    {
      a = 1;
    }
    cout << a << endl;
}

void testLockOmp() {
    {
        LockOmp();
    }
}

volatile uint8_t myArray[(size_t)8*1024*1024];
uint_fast32_t testOpenMPLoop() {
    uint_fast32_t sum = 0;
    #pragma omp parallel for reduction(+:sum)
    for (uint64_t i=0; i < 100; i++)
    {
        sum += myArray[i];
    }

    #pragma omp parallel for reduction(+:sum)
    for (uint64_t i=0; i < sizeof(myArray); i++)
    {
        sum += myArray[i];
    }
    return sum;
}

typedef PipelineTask<int, LockDummy, 3> MyPipelineTask;

MyPipelineTask pipelineTask3("3");
MyPipelineTask pipelineTask2("2", &pipelineTask3);
MyPipelineTask pipelineTask1("1", &pipelineTask2);

void testPipeline() {
    int data = 0;
    pipelineTask1.addJob(data);
    pipelineTask1.doJob();
    pipelineTask2.doJob();
    pipelineTask3.doJob();
}

constexpr size_t calculateCyclicBufferSizeLage() {
    return 1000*1024*1024;
}

static array<uint8_t, 100> myStlArray;

void testStlArray() {
    array<uint8_t, 100>::iterator myIntVectorIterator;
    for(myIntVectorIterator = myStlArray.begin();
            myIntVectorIterator != myStlArray.end();
            myIntVectorIterator++)
    {
        cout<<*myIntVectorIterator<<" ";
        //Should output 1 4 8
    }
}

//static CyclicBuffer<uint_fast8_t, LockDummy, calculateCyclicBufferSizeLage()> myCyclicBufferLarge;
static CyclicBufferFast<uint_fast8_t, LockDummy, calculateCyclicBufferSizeLage()> myCyclicBufferLarge;
void testCyclicBuffer1() {
    while (!myCyclicBufferLarge.isFull()) {
        myCyclicBufferLarge.add(0);
    };

    while (!myCyclicBufferLarge.isEmpty()) {
        uint8_t val;
        myCyclicBufferLarge.remove(val);
    }
}

template<typename ObjectType, std::size_t Size> class ADC {
public:
    inline ADC(ObjectType initialValue=0);

    typedef ObjectType (*Filter)(ObjectType current,
            ObjectType sample);
    inline void add(ObjectType sample, Filter filter);

    inline ObjectType get();

protected:
    CyclicBuffer<ObjectType, LockDummy, Size> data;
    ObjectType value;
};


template<typename ObjectType, std::size_t Size>
ADC<ObjectType, Size>::
ADC(ObjectType initialValue) {
    value = initialValue;
}

template<typename ObjectType, std::size_t Size>
void ADC<ObjectType, Size>::
add(ObjectType sample, Filter filter) {
    data.add(sample);
    value = filter(value, sample);
}

template<typename ObjectType, std::size_t Size>
ObjectType ADC<ObjectType, Size>::
get() {
    return value;
}



static void testADC()
{
    static ADC<double, 4> myAdc(3.0);
    cout << "ADC=" << myAdc.get() << endl;
    for (int i = 0;i < 10;i++) {
        myAdc.add(4.0,
                [](double current, double sample) {
                     return current+0.5*(sample-current);
                }
        );
    }
    cout << "ADC=" << myAdc.get() << endl;
}



class HardwareModuleADC : HardwareModule {
public:
    double read();
protected:
};


class ReadADC {
public:
    ReadADC() : myAdc(3.0) {}
    void run(void) {
        double sample = hardwareModuleADC.read();
        myAdc.add(sample,
            [](double current, double sample) {
                return current+0.5*(sample-current);
            });
    }
protected:
    ADC<double, 4> myAdc;
    HardwareModuleADC hardwareModuleADC;
};

class ReadADCFixedPoint {
public:
    ReadADCFixedPoint() : myAdc(3.0) {}
    typedef FixedPoint<int_fast16_t, 3> FPADC;
    void run(void) {
        FPADC sample = FPADC(hardwareModuleADC.read());
        myAdc.add(sample,
            [](FPADC current, FPADC sample) {
                return current+(sample-current)/2;
            });
    }
protected:
    ADC<FPADC, 4> myAdc;
    HardwareModuleADC hardwareModuleADC;
};

//JobThread<ReadADC> jobThreadReadADC;

void testADC1() {

}

/**
 * I've had a lot of conversations with people lately based on their experience and my own
 * regarding shared_ptr (either the C++11 standard version,
 * the TR version, the Boost version, or home-grown versions).
 * The almost universal conclusion has been that shared_ptr is best
 * avoided in almost every circumstance when writing software of substantial size.
 *
 * Sean Middleditch, Game developer.
 *
 */
class Test {
public:
    Test(int a = 0 ) : m_a(a)
    {
    }
    ~Test( )
    {
        cout<<"Calling destructor"<<endl;
    }
public:
    int m_a;
};

void testSmartPtr( )
{
    shared_ptr<Test> sptr1( new Test);
}

static void FixedPointTest() {
    typedef FixedPoint<int_fast16_t, 3> FixedPoint_3;
    FixedPoint_3 value(3.43188);
    value = (2*(value+FixedPoint_3(1.4)-FixedPoint_3(1.4)))/2;
    cout << value.toDouble() << endl;
}

void testLogCat()
{
    char *f = "Test %d";
    LOG_CAT("Test %d", 1);
}

#define IS_LITTLE_ENDIAN (*(uint16_t *)"\0\xff" >= 0x100)

int isLittleEndian() {
    short int data = 0x0001;
    char *byte = (char *) &data;
    return (byte[0] ? 1 : 0);
}

int random_5() {
    return 0;
}

int random_7() {
    while (true) {
        int ret = 5 * random_5() + random_5();
        if (ret < 21)
            return (ret % 7);
    }
}

class YouCanNotInheritMe;
class Singleton {
public:
    void p() {
        printf("I am a Singletone\r\n");
    }
private:
    Singleton() {}
    friend YouCanNotInheritMe;
};

class YouCanNotInheritMe : virtual public Singleton {
public:
    YouCanNotInheritMe(){}
};

YouCanNotInheritMe youCanNotInheritMeObject;

class TryToInheritAnyway : YouCanNotInheritMe {
public:
    void p() {
        printf("I am a TryToInheritAnyway\r\n");
    }
//    TryToInheritAnyway() {}
};

/*
TryToInheritAnyway tryToInheritAnywayObject;
*/

class Final final {
public:
    Final() {}
};

class MutexAB {
public:
    MutexAB() {}
};

class A {
public:
    A() : isDone(false) {}
    virtual void m1() = 0;
    void m2() {
        m1();
        MutexAB m;
        isDone = true;
    }
    virtual ~A() {
        MutexAB m;
        while (!isDone) {}
    }
protected:
    bool isDone;
};

class B : public A {
public:
    void m1() {}
    virtual ~B() {}
};

void testAB() {
    B *b = new B;
    std::thread t1{ [=] {
        b->m1();
        b->m2();
    }};

    std::thread t2{ [=] {
        delete b;
    }};
}

typedef struct {
    int f1 : 8;
    int f2 : 8;
} myDataT;

static myDataT myData;
void testMyData() {
    std::thread t1{ [=] {
        myData.f1 = 0;
    }};

    std::thread t2{ [=] {
        myData.f2 = 1;
    }};
}

static struct {
    int a[128];
    int b[128];
    int c[128];
} dataArray;

void initDataArrayB() {
    memset(dataArray.a, 0, sizeof(dataArray.a));
    memset(dataArray.b, 0, sizeof(dataArray.b));
}

void initDataArrayC() {
    memset(dataArray.a, 0, sizeof(dataArray.a));
    memset(dataArray.c, 0, sizeof(dataArray.c));
}

class FastSum {
public:
    FastSum(int delta) : delta(delta) {}
    virtual int sum(int a) { return delta+a;}
    virtual ~FastSum() {}
    protected:
    int delta;
};

void testFastSum(int delta, int *result, int count) {
    FastSum fastSum(delta);
    for (int i = 0;i < count;i++) {
        result[i] = fastSum.sum(i);
    }
}

class LazyInitialization {
    static LazyInitialization *getInstance() {
        if (instance == nullptr) {
            MutexAB m;
            if (instance == nullptr) {
                instance = new LazyInitialization();
            }
        }
        return instance;
    }
private:
    LazyInitialization();
    static atomic<LazyInitialization*> instance;
};

//LazyInitialization *LazyInitialization::instance = nullptr;

atomic<LazyInitialization*> LazyInitialization::instance(nullptr);

#include <vector>
#include <string>
#include <utility>
#include <iostream>

template<typename Container> class NamedContainer;
class SingletonS {
private:
    SingletonS() {}
    template<typename Container> friend  class NamedContainer;
};


template<typename Container> class NamedContainer  : virtual public SingletonS {
public:
    template <typename... Args>
        NamedContainer(const string& name, Args&&... args):
            name(name), c(std::forward<Args>(args)...) {}
    const std::string name;
    Container c;
    Container& operator() () {return c;}
};


void testNamedContainer()
{
    typedef NamedContainer< std::vector<double> > NamedVector;

    NamedVector vec1("vec2", 119);
    cout << "Vector " << vec1.name << ", size " << vec1().size() << endl;
}

template<typename Container> struct NamedContainerS
{
    NamedContainerS(const char* name) : name(name){}
    Container c;
    const char* name;
};

void testNamedContainerS()
{
    typedef NamedContainerS< std::vector<double> > NamedVector;

    NamedVector vec1("vec3");
    vector<double>* vec1v = (vector<double>*)&vec1;
    vec1v->resize(139);
    cout << "Vector " << vec1.name << ", size " << vec1v->size() << endl;
}

template<typename T> class NamedVector final : public vector<T>
{
public:
    template <typename... Args> NamedVector(const string& name, Args&&... args) : vector<T>(std::forward<Args>(args)...), name(name) {}
    const string name;
};

void testNamedContainerFinal()
{
    NamedVector<double> vec4("vec4", 893);

    cout << "Vector " << vec4.name << ", size " << vec4.size() << endl;
}

enum class LetterType {
    complement,
    complaint,
    unknown
};


#include "fastpool.h"

/**
 * return size if Ok
 */
static int itoa(int value, char *s, int size)
{
    int i = 0;
    int chars = size - 1;
    int digits = 0;
    int v = value;
    while (v)
    {
        v = v / 10;
        digits++;
    }
    while (value)
    {
        if (i >= chars)
            break;
        s[digits-1-i] = '0' + (value%10);
        i++;
        value = value / 10;
    }
    if ((digits-1-i) < 0)
    {
        s[digits-1] = 0;
        return size;
    }
    else
    {
        s[chars] = 0;
        return 0;
    }

}

#if (EXAMPLE == 10)
static void hashTableTest(void)
{
    struct MyHashObject
    {
        MyHashObject(const char *name)
        {
            this->name = name;
        }

        static bool equal(const char *s1, const char *s2)
        {
            bool result = strcmp(s1, s2);
            return (result == 0);
        }

        static char* getKey(struct MyHashObject &object)
        {
            return (object.name);
        }

        static const uint_fast32_t hash(const char *s)
        {
            uint_fast32_t result = one_at_a_time((uint8_t*)s, strlen(s));
            return result;
        }
        const char *name;
    };

    typedef HashTable<struct MyHashObject*, char*, LockDummy, AllocatorTrivial> MyHashTable;
    MyHashTable *hashTable = MyHashTable::create("myHashTable", 3);
    MyHashObject o1("o1");
    hashTable->insert(o1.getKey(o1), &o1);
    MyHashTable::destroy(hashTable);
}
#endif

int main()
{
#if (EXAMPLE == 10)
    hashTableTest();
#endif
    vector<int> testArray = {1234, 123456,1234567,12345678};
    for (int i : testArray)
    {
        char s[7];
        int res = itoa(i, s, sizeof(s));
        cout << "val=" << i << ",res=" << res << ",s=" << s << endl;
    }

    fastPoolInitialize();
    fastPoolPrint();
    uint32_t *p1 = fastPoolAllocate();
    fastPoolPrint();
    uint32_t *p2 = fastPoolAllocate();
    fastPoolPrint();
    uint32_t *p3 = fastPoolAllocate();
    fastPoolPrint();
    fastPoolFree(p1);
    fastPoolPrint();
    fastPoolFree(p3);
    fastPoolPrint();
    fastPoolFree(p2);
    fastPoolPrint();

    testNamedContainerFinal();
    testNamedContainer();
    testNamedContainerS();
    youCanNotInheritMeObject.p();
//  tryToInheritAnywayObject.p();
    if (IS_LITTLE_ENDIAN) {
        cout << "Little endian" << endl;
    }
    if (isLittleEndian()) {
        cout << "Little endian" << endl;
    }
    testLogCat();

    FixedPointTest();
    testSmartPtr();
    testADC();

    testStlArray();
    struct timespec t2, t3;
    double dt1;
//    clock_gettime(CLOCK_MONOTONIC,  &t2);
    //testCyclicBuffer1();
    // testPipeline();
//    testDummyLock1();
//    testHardwareTimers();
//    testBinaryLog3();
//    testTimer();
//    testOpenMpReduction();
    // testOpenMPLoop();
    // testOpenMP();
     //testLockOmp();
//    clock_gettime(CLOCK_MONOTONIC,  &t3);
    dt1 = (t3.tv_sec - t2.tv_sec) + (double) (t3.tv_nsec - t2.tv_nsec) * 1e-9;
    cout << "time:  " << dt1 << endl;

    return 0;

}



