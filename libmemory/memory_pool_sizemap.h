#ifndef __MEMPOOL_SIZEMAP_H__
#define __MEMPOOL_SIZEMAP_H__

#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>

namespace infra::mempool {

/**
* @brief - Small obejct size map.
*/
class MempoolSizeMap {
public:
    /**
    * @brief - Small object size map element.
    */
    struct MempoolSizeClass {
        size_t mem_size_;      ///< The memory size corresponding to this object.
        size_t page_size_;     ///< How many pages of memory does an object need.
        size_t num_to_move_;   ///< How many times each application or recycling.
    };
public:
#if defined(MEMPOOL_32K_PAGES)
    static const size_t g_page_shift = 15;  ///< Page size 32K
#elif defined(MEMPOOL_64K_PAGES)
    static const size_t g_page_shift = 16;  ///< Page size 64K
#else
    static const size_t g_page_shift = 13;  ///< Page size 8K
#endif

    static const size_t g_page_size = 1 << g_page_shift;
    static const size_t g_alignment = 8;   ///< Memory alignment default size.(8B)

    static const size_t g_sobj_size_demarcation = 1024; ///< if size little than 1024, it will aligned with 8 byte. Otherwise aligned with 128.
    static const size_t g_sobj_max_size = (256 * 1024);  ///< Largest small object defalue size.(256KB)
    static const size_t g_sobj_array_size = ((g_sobj_max_size + 127 + (120 << 7)) >> 7) + 1;   ///< Size of size class array.

    MempoolSizeMap();
    ~MempoolSizeMap();

private:
    struct MempoolSizeClass size_class_array_[g_sobj_array_size];  ///< Size class array.

    /**
    * @brief SmallSizeClass - Get small size class index.
    *
    * @param [s] - Real size.
    *
    * @returns  Size class array index.
    */
    size_t SmallSizeClass(size_t s) {
        return (static_cast<uint32_t>(s) + 7) >> 3;
    }

    /**
    * @brief LargeSizeClass - Get large size class index.
    *
    * @param [s] - Real size.
    *
    * @returns  Size class array index.
    */
    size_t LargeSizeClass(size_t s) {
        return (static_cast<uint32_t>(s) + 127 + (120 << 7)) >> 7;
    }

    int AlignmentForSize(size_t s) {
    /*
        int alignment = g_alignment;
        if (s > g_sobj_max_size) {
            alignment = g_page_size;
        } else if (s >= 128) {
            
        }
    */
        return 0;
    }
};

} //namespace infra::end


#endif
