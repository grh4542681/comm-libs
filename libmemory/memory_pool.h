#ifndef __MEMPOOL_MEMPOOL_H__
#define __MEMPOOL_MEMPOOL_H__

#include <stdlib.h>
#include <stdio.h>
#include <utility>
#include <string.h>
#include <new>
#include <iostream>
#include <sstream>

#include "report.h"
#include "mempool_threadcache.h"

namespace infra::infra::file {
class File;
}

namespace infra::infra::mempool {

class Mempool : public report::VReport {
public:

    void* Malloc(size_t size){
        return _malloc(size);
    }

    template < typename T > T* Malloc(T&& other){
        T* ptr = (T*)Malloc(sizeof(T));
        memset((char*)ptr, 0x00, sizeof(T));
        return (new(ptr) T(std::forward<T>(other)));
    }

    template < typename T, typename ... Args> T* Malloc(Args&& ... args){
        T* ptr = (T*)Malloc(sizeof(T));
        memset((char*)ptr, 0x00, sizeof(T));
        return (new(ptr) T(std::forward<Args>(args)...));
    }

    template < typename T > T* Reset(T* ptr, T&& other){
        ptr->~T();
        memset((char*)ptr, 0x00, sizeof(T));
        return (new(ptr) T(std::forward<T>(other)));
    }

    template < typename T, typename ... Args> T* Reset(T* ptr, Args&& ... args){
        ptr->~T();
        memset((char*)ptr, 0x00, sizeof(T));
        return (new(ptr) T(std::forward<Args>(args)...));
    }

    void Free(void* ptr){
        _free(ptr);
    }
    template < typename T > void Free(T* ptr){
        ptr->~T();
        _free(ptr);
    }

    template < typename T, typename ... Args> static T* Construct(void* ptr, T&& other){
        memset((char*)ptr, 0x00, sizeof(T));
        return (new(ptr) T(std::forward<T>(other)));
    }

    template < typename T, typename ... Args> static T* Construct(void* ptr, Args&& ... args){
        memset((char*)ptr, 0x00, sizeof(T));
        return (new(ptr) T(std::forward<Args>(args)...));
    }

    template < typename T > static void* Destruct(T* ptr){
        ptr->~T();
        return ptr;
    }

    static Mempool* getInstance();
    static void freeInstance();

    void Report(std::stringstream& ss);
    void Report(file::File& fd);
    void ReportCenter(file::File& fd);
    void ReportThread(file::File& fd);
private:
    Mempool();
    ~Mempool();
    MempoolThreadCache threadcache_;
    thread_local static Mempool* pInstance;

    void* _malloc(size_t size);
    void _free(void* ptr);
};


}//namespace infra::infra::pub end

#endif
