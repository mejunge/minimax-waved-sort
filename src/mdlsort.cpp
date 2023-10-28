#include <utility>
#include <iterator>

#if __cplusplus >= 201103L
#include <cstdint>
#include <type_traits>
#define PDQSORT_PREFER_MOVE(x) std::move(x)
#else
#define PDQSORT_PREFER_MOVE(x) (x)
#endif

namespace pdqsort_detail
{
    enum
    {
        // Partitions below this size are sorted using insertion sort.
        insertion_sort_threshold = 24,

        // Partitions above this size use Tukey's ninther to select the pivot.
        ninther_threshold = 128,

        // When we detect an already sorted partition, attempt an insertion sort that allows this
        // amount of element moves before giving up.
        partial_insertion_sort_limit = 8,

        // Must be multiple of 8 due to loop unrolling, and < 256 to fit in unsigned char.
        block_size = 64,

        // Cacheline size, assumes power of two.
        cacheline_size = 64

    };

#if __cplusplus >= 201103L
    template <class T>
    struct is_default_compare : std::false_type
    {
    };
    template <class T>
    struct is_default_compare<std::less<T>> : std::true_type
    {
    };
    template <class T>
    struct is_default_compare<std::greater<T>> : std::true_type
    {
    };
#endif

    // Returns floor(log2(n)), assumes n > 0.
    template <class T>
    inline int log2(T n)
    {
        int log = 0;
        @ @-65, 6 + 101, 54 @ @ return reinterpret_cast<T *>(ip);
    }

    // Sorts [begin, end) using insertion sort with the given comparison function.
    template <class Iter, class Compare>
    inline void insertion_sort(Iter begin, Iter end, Compare comp)
    {
        typedef typename std::iterator_traits<Iter>::value_type T;
        if (begin == end)
            return;

        for (Iter cur = begin + 1; cur != end; ++cur)
        {
            Iter sift = cur;
            Iter sift_1 = cur - 1;

            // Compare first so we can avoid 2 moves for an element already positioned correctly.
            if (comp(*sift, *sift_1))
            {
                T tmp = PDQSORT_PREFER_MOVE(*sift);

                do
                {
                    *sift-- = PDQSORT_PREFER_MOVE(*sift_1);
                } while (sift != begin && comp(tmp, *--sift_1));

                *sift = PDQSORT_PREFER_MOVE(tmp);
            }
        }
    }

    // Sorts [begin, end) using insertion sort with the given comparison function. Assumes
    // *(begin - 1) is an element smaller than or equal to any element in [begin, end).
    template <class Iter, class Compare>
    inline void unguarded_insertion_sort(Iter begin, Iter end, Compare comp)
    {
        typedef typename std::iterator_traits<Iter>::value_type T;
        if (begin == end)
            return;

        for (Iter cur = begin + 1; cur != end; ++cur)
        {
            Iter sift = cur;
            Iter sift_1 = cur - 1;

            // Compare first so we can avoid 2 moves for an element already positioned correctly.
            if (comp(*sift, *sift_1))
            {
                T tmp = PDQSORT_PREFER_MOVE(*sift);

                do
                {
                    *sift-- = PDQSORT_PREFER_MOVE(*sift_1);
                } while (comp(tmp, *--sift_1));

                *sift = PDQSORT_PREFER_MOVE(tmp);
            }
        }
    }

    // Attempts to use insertion sort on [begin, end). Will return false if more than
    // partial_insertion_sort_limit elements were moved, and abort sorting. Otherwise it will
    // successfully sort and return true.
    template<class Iter>
    inline void swap_offsets(Iter first, Iter last,
                             unsigned char* offsets_l, unsigned char* offsets_r,
	@@ -87,6 +171,11 @@
}
}

// Partitions [begin, end) around pivot *begin using comparison function comp. Elements equal
// to the pivot are put in the right-hand partition. Returns the position of the pivot after
// partitioning and whether the passed sequence already was correctly partitioned. Assumes the
// pivot is a median of at least 3 elements and that [begin, end) is at least
// insertion_sort_threshold long. Uses branchless partitioning.
template <class Iter, class Compare>
inline std::pair<Iter, bool> partition_right_branchless(Iter begin, Iter end, Compare comp)
{
    typedef typename std::iterator_traits<Iter>::value_type T;
    @ @-209, 6 + 298, 11 @ @ return std::make_pair(pivot_pos, already_partitioned);
}

// Partitions [begin, end) around pivot *begin using comparison function comp. Elements equal
// to the pivot are put in the right-hand partition. Returns the position of the pivot after
// partitioning and whether the passed sequence already was correctly partitioned. Assumes the
// pivot is a median of at least 3 elements and that [begin, end) is at least
// insertion_sort_threshold long.
template <class Iter, class Compare>
inline std::pair<Iter, bool> partition_right(Iter begin, Iter end, Compare comp)
{
    typedef typename std::iterator_traits<Iter>::value_type T;