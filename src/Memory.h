#pragma once

class MemoryRegion {

public:
    MemoryRegion(const char *name, uintptr_t address, size_t size) :
        name(name), address(address), size(size) {
        data = new (reinterpret_cast<void*>(address)) uint8_t[size];
    }

    size_t getSize() const {
        return size;
    }
    const char* getName() const {
        return name;
    }
    uintptr_t getAddress() const {
        return address;
    }

protected:

    const char* name;
    uintptr_t address;
    size_t size;
    uint8_t *data;
};


class MemoryAllocatorRaw {

public:
    MemoryAllocatorRaw(MemoryRegion memoryRegion, size_t blockSize, size_t count, unsigned int alignment);

    uint8_t* getBlock();

    bool blockBelongs(const void* block) const;

    const MemoryRegion& getRegion() const {
        return memoryRegion;
    }

    void reset() {
        firstNotAllocatedAddress = memoryRegion.getAddress();
    }

    constexpr static size_t predictMemorySize(size_t blockSize, size_t count, unsigned int alignment) {
        return count * alignConst(blockSize, alignment);
    }

protected:
    int alignment;
    size_t blockSize;
    const MemoryRegion& memoryRegion;
    size_t count;
    size_t sizeTotalBytes;
    size_t alignedBlockSize;
    uintptr_t firstNotAllocatedAddress;

    static constexpr size_t alignConst(size_t value, unsigned int alignment) {
        return (value + ((size_t)alignment-1)) & (~((size_t)alignment-1));
    }

    inline static uintptr_t alignAddress(uintptr_t address, unsigned int alignment) {
        uintptr_t res = (address+((uintptr_t)alignment-1)) & (~((uintptr_t)alignment-1));
        return res;
    }
};

MemoryAllocatorRaw::MemoryAllocatorRaw(MemoryRegion memoryRegion, size_t blockSize, size_t count, unsigned int alignment) :
    alignment(alignment), blockSize(blockSize), memoryRegion(memoryRegion), count(count)  {  // initialize internal data

    alignedBlockSize = alignAddress(blockSize, alignment);
    sizeTotalBytes = alignedBlockSize * count;
    if (sizeTotalBytes > memoryRegion.getSize()) {
        // handle error
    }
    reset();
}

uint8_t* MemoryAllocatorRaw::getBlock() {
    uintptr_t block;
    block = alignAddress(firstNotAllocatedAddress, alignment);
    firstNotAllocatedAddress += alignedBlockSize;
    return (uint8_t*)block;
}

bool MemoryAllocatorRaw::blockBelongs(const void* block) const {
    uintptr_t blockPtr = (uintptr_t)block;
    bool res = true;
    res = res && blockPtr >= memoryRegion.getAddress();
    size_t maxAddress = memoryRegion.getAddress()+sizeTotalBytes;
    res = res && (blockPtr <= maxAddress);
    uintptr_t alignedAddress = alignAddress(blockPtr, alignment);
    res = res && (blockPtr == alignedAddress);
    return res;
}

template<typename Lock, size_t Size> class MemoryPoolRaw {

public:

    MemoryPoolRaw(const char* name, MemoryAllocatorRaw& memoryAllocator);

    ~MemoryPoolRaw() {
        memoryAllocator.reset();
    }

    inline void resetMaxInUse() const {
        statictics.maxInUse = 0;
    }

    typedef struct {
        uint32_t inUse;
        uint32_t maxInUse;
        uint32_t errBadBlock;
    } Statistics;

    inline bool allocate(uint8_t** block);

    inline bool free(uint8_t* block);

    inline const Statistics &getStatistics(void) const {return statictics;}

protected:
    mutable Statistics statictics;
    const char* name;
    Stack<uint8_t, LockDummy,  Size> pool;
    MemoryAllocatorRaw& memoryAllocator;
};

template<typename Lock, size_t Size> MemoryPoolRaw<Lock, Size>::MemoryPoolRaw(const char* name, MemoryAllocatorRaw& memoryAllocator) :
    name(name), memoryAllocator(memoryAllocator) {
    memset(&this->statictics, 0, sizeof(this->statictics));
    this->name = name;
    for (size_t i = 0;i < Size;i++) {
        uint8_t *block = memoryAllocator.getBlock();
        pool.push(block);
    }
}

template<typename Lock, size_t Size>
inline bool MemoryPoolRaw<Lock, Size>::allocate(uint8_t** block) {
    bool res;
    Lock();
    res = pool.pop(block);
    if (res) {
        statictics.inUse++;
        if (statictics.inUse > statictics.maxInUse)
            statictics.maxInUse = statictics.inUse;
    }
    return res;
}

template<typename Lock, size_t Size>
inline bool MemoryPoolRaw<Lock, Size>::free(uint8_t* block) {
    bool res;
    Lock();
    res = memoryAllocator.blockBelongs(block);
    if (res) {
        res = pool.push(block);
        statictics.inUse--;
    }
    else {
        statictics.errBadBlock++;
    }
    return res;
}

