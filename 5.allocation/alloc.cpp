#include "alloc.h"

#include <libdivide.h>

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <new>

#include <memory>
#include <unordered_map>

struct MemBlk {
  void *address;
  size_t size;
};

union memory_address {
  void *ptr;
  uintptr_t addr;
};

using divider_t = libdivide::divider<size_t>;

static constexpr size_t align(size_t size, size_t alignment) noexcept {
  const size_t temp_alignment = alignment - 1;
  return (size + temp_alignment) & ~temp_alignment;
}

static constexpr bool is_aligned(uintptr_t addr, size_t alignment) noexcept {
  return !(addr % alignment);
}

struct Mallocator {
  MemBlk allocate(size_t size) {

    auto addr = malloc(size);
    return {addr, size};
  }
  MemBlk allocate(size_t size, size_t alignment) {

    auto addr = aligned_alloc(alignment, size);
    return {addr, size};
  }
  void deallocate(MemBlk blk) { free(blk.address); }
  bool owns(MemBlk) { return false; }
};

template <typename A, typename B, size_t S> struct Partition {

  A less;
  B more;

  MemBlk allocate(size_t size) {
    return size <= S ? less.allocate(size) : more.allocate(size);
  }
  MemBlk allocate(size_t size, size_t alignment) {
    return size <= S ? less.allocate(size, alignment)
                     : more.allocate(size, alignment);
  }
  void deallocate(MemBlk blk) {
    if (less.owns(blk)) {
      less.deallocate(blk);
    } else {
      more.deallocate(blk);
    }
  }
  bool owns(MemBlk blk) { return less.owns(blk) | more.owns(blk); }
};

template <typename P, typename F> struct Fallback {

  P primary;
  F fallback;

  MemBlk allocate(size_t size) {
    auto blk = primary.allocate(size);
    if (blk.address == nullptr) {
      blk = fallback.allocate(size);
    }
    return blk;
  }
  MemBlk allocate(size_t size, size_t alignment) {
    auto blk = primary.allocate(size, alignment);
    if (blk.address == nullptr) {
      blk = fallback.allocate(size, alignment);
    }
    return blk;
  }
  void deallocate(MemBlk blk) {
    if (primary.owns(blk)) {
      primary.deallocate(blk);
    } else {
      fallback.deallocate(blk);
    }
  }
  bool owns(MemBlk blk) { return primary.owns(blk) | fallback.owns(blk); }
};

template <typename P> struct SizeAllocator {

  P primary;

  MemBlk allocate(size_t size) { return primary.allocate(P::Size); }

  MemBlk allocate(size_t size, size_t alignment) {
    return primary.allocate(P::Size, alignment);
  }

  void deallocate(MemBlk blk) {
    if (owns(blk)) {
      primary.deallocate({blk.address, P::Size});
    }
  }
  bool owns(MemBlk blk) { return primary.owns(blk); }
};

template <typename P, size_t S, size_t A, size_t C> struct PoolAllocator {

  static constexpr size_t ALLOCATOR_ALIGN{64};

  static constexpr size_t Size{align(S, A)};

  struct {
    size_t block_count;
    size_t block_size;
    size_t block_alignment;
    divider_t block_divider;
    memory_address memory_space_start;
    memory_address control_block_start;
    memory_address free_block_head;
    memory_address *control_blocks;
  } internal;

  P parent;
  MemBlk parentAllocation;

  PoolAllocator() {

    const size_t alinged_block_size = align(S, A);
    const size_t aligned_meta_size =
        align(C * sizeof(memory_address), ALLOCATOR_ALIGN);
    const size_t required_size = aligned_meta_size + C * alinged_block_size;

    parentAllocation = parent.allocate(required_size, ALLOCATOR_ALIGN);

    internal.block_count = C;
    internal.block_size = alinged_block_size;
    internal.block_alignment = A;
    internal.block_divider = alinged_block_size;

    memory_address allocator_mem_space = {.ptr = parentAllocation.address};

    internal.memory_space_start.addr =
        allocator_mem_space.addr + aligned_meta_size;
    internal.control_block_start.ptr = parentAllocation.address;
    internal.control_blocks =
        static_cast<memory_address *>(allocator_mem_space.ptr);
    internal.free_block_head.ptr = &(internal.control_blocks[0]);

    for (int32_t i = 0, l = C - 1; i < l; ++i) {
      internal.control_blocks[i].addr =
          allocator_mem_space.addr + (i + 1) * sizeof(memory_address);
    }
    internal.control_blocks[C - 1].ptr = nullptr;
  }

  ~PoolAllocator() { parent.deallocate(parentAllocation); }

  MemBlk allocate(size_t size) {

    const size_t aligned_size = align(size, A);
    if (internal.block_size != aligned_size) {
      assert(!"Requested allocation is incorret size");
      return {nullptr, 0};
    }

    const memory_address start = internal.control_block_start;
    const memory_address current = internal.free_block_head;
    if (current.ptr == nullptr)
      return {nullptr, 0};

    const size_t idx = (current.addr - start.addr) >> 3;
    const memory_address res = {.addr = internal.memory_space_start.addr +
                                        idx * internal.block_size};

    memory_address *next = static_cast<memory_address *>(current.ptr);
    internal.free_block_head.ptr = next->ptr;

    return {res.ptr, internal.block_size};
  }
  MemBlk allocate(size_t size, size_t alignment) {
    const size_t aligned_size = align(size, A);
    if (internal.block_size != aligned_size) {
      assert(!"Requested allocation is incorret size");
      return {nullptr, 0};
    }

    const memory_address start = internal.control_block_start;
    const memory_address current = internal.free_block_head;
    if (current.ptr == nullptr | !is_aligned(current.addr, alignment))
      return {nullptr, 0};

    const size_t idx = (current.addr - start.addr) >> 3;
    const memory_address res = {.addr = internal.memory_space_start.addr +
                                        idx * internal.block_size};

    memory_address *next = static_cast<memory_address *>(current.ptr);
    internal.free_block_head.ptr = next->ptr;
    return {res.ptr, internal.block_size};
  }

  void deallocate(MemBlk blk) {

    assert(blk.size == internal.block_size && "Unexpected dealocation size");
    assert(owns(blk) && "Unexpected not owned block");
    const memory_address current = {blk.address};
    const memory_address start = internal.memory_space_start;

    const size_t idx =
        size_t(current.addr - start.addr) / internal.block_divider;
    internal.control_blocks[idx].ptr = internal.free_block_head.ptr;
    internal.free_block_head.ptr = &internal.control_blocks[idx];
  }

  bool owns(MemBlk blk) {
    const memory_address address{.ptr = blk.address};
    return address.addr >= internal.memory_space_start.addr &&
           address.addr <=
               internal.memory_space_start.addr +
                   ((internal.block_count - 1) * internal.block_size);
  }
};

namespace custom_alloc {

using smallPool =
    Fallback<SizeAllocator<PoolAllocator<Mallocator, 64, 64, 6000000>>,
             Mallocator>;

using mediumPool =
    Fallback<SizeAllocator<PoolAllocator<Mallocator, 40960, 64, 2048>>,
             Mallocator>;

using largePool =
    Fallback<SizeAllocator<PoolAllocator<Mallocator, 209715200, 64, 6>>,
             Mallocator>;

using MyAlloc =
    Partition<smallPool, Partition<mediumPool, largePool, 40960>, 64>;

// Total allocation 1.607 Gb

#ifdef CUSTOM
static MyAlloc &allocator() {
  static MyAlloc alloc;
  return alloc;
}
#endif

}; // namespace custom_alloc

namespace memory_profile {

struct pair_hash {
  template <class T1, class T2>
  std::size_t operator()(const std::pair<T1, T2> &pair) const {
    return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
  }
};

template <typename T> class TempAllocator {
public:
  //    typedefs
  typedef T value_type;
  typedef value_type *pointer;
  typedef const value_type *const_pointer;
  typedef value_type &reference;
  typedef const value_type &const_reference;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;

public:
  //    convert an allocator<T> to allocator<U>
  template <typename U> struct rebind { typedef TempAllocator<U> other; };

public:
  inline explicit TempAllocator() {}
  inline ~TempAllocator() {}
  inline explicit TempAllocator(TempAllocator const &) {}
  template <typename U>
  inline explicit TempAllocator(TempAllocator<U> const &) {}

  //    address
  inline pointer address(reference r) { return &r; }
  inline const_pointer address(const_reference r) { return &r; }

  //    memory allocation
  inline pointer allocate(size_type cnt,
                          typename std::allocator<void>::const_pointer = 0) {
    return static_cast<pointer>(malloc(cnt * sizeof(T)));
  }
  inline void deallocate(pointer p, size_type) { free(p); }

  //    size
  inline size_type max_size() const {
    return std::numeric_limits<size_type>::max() / sizeof(T);
  }

  //    construction/destruction
  inline void construct(pointer p, const T &t) { new (p) T(t); }
  inline void destroy(pointer p) { p->~T(); }

  inline bool operator==(TempAllocator const &) { return true; }
  inline bool operator!=(TempAllocator const &a) { return !operator==(a); }
};

using UnlignedAllocMapper = std::unordered_map<
    std::size_t, std::size_t, std::hash<std::size_t>,
    std::equal_to<std::size_t>,
    TempAllocator<std::pair<const std::size_t, std::size_t>>>;

using AlignedAllocMapper = std::unordered_map<
    std::pair<std::size_t, std::size_t>, std::size_t, pair_hash,
    std::equal_to<std::pair<std::size_t, std::size_t>>,
    TempAllocator<
        std::pair<const std::pair<std::size_t, std::size_t>, std::size_t>>>;

struct {
  UnlignedAllocMapper alloc_without_allognment{};
  AlignedAllocMapper alloc_with_allognment{};
  size_t current_alloc_size{0};
  size_t max_alloc_size{0};
  bool collect{false};
} data;

void begin() {
#ifdef PRINTER
  data = {{}, {}, 0, 0, true};
#endif
}

void clear() {
#ifdef PRINTER
  data.alloc_with_allognment.clear();
  data.alloc_without_allognment.clear();
#endif
}

void end() {
#ifdef PRINTER
  if (data.collect) {
    size_t total_allocation = 0;

    for (const auto &[key, value] : data.alloc_without_allognment) {
      total_allocation += key * value;
      printf("Allocation size: %lu (bytes), count: %lu\n", key, value);
    }
    printf("Total unaligned allocations: %lu (bytes)\n", total_allocation);

    total_allocation = 0;

    for (const auto &[key, value] : data.alloc_with_allognment) {
      total_allocation += key.first * value;
      printf("Allocation size: %lu (bytes), alignment: %lu (bytes), count: "
             "%lu\n",
             key.first, key.second, value);
    }
    printf("Total aligned allocations: %lu (bytes)\n", total_allocation);

    printf("Max memory usage: %lu (bytes)\n", data.max_alloc_size);
    data.collect = false;
  }
#endif
}
} // namespace memory_profile

static void *allocate(size_t size) {
#ifdef CUSTOM
  return custom_alloc::allocator().allocate(size).address;
#else
  return malloc(size);
#endif
}

static void *allocate(size_t al, size_t size) {
#ifdef CUSTOM
  return custom_alloc::allocator().allocate(size, al).address;
#else
  return aligned_alloc(al, size);
#endif
}

static void deallocate(void *ptr, size_t size) {
#ifdef CUSTOM
  custom_alloc::allocator().deallocate({ptr, size});
#else
  free(ptr);
#endif
}

void *operator new(std::size_t count) {
#ifdef PRINTER
  if (memory_profile::data.collect) {
    memory_profile::data.alloc_without_allognment[count]++;
    memory_profile::data.current_alloc_size += count;
    memory_profile::data.max_alloc_size =
        std::max(memory_profile::data.max_alloc_size,
                 memory_profile::data.current_alloc_size);
  }
#endif
  return allocate(count);
}

void *operator new[](std::size_t count) {
#ifdef PRINTER
  if (memory_profile::data.collect) {
    memory_profile::data.alloc_without_allognment[count]++;
    memory_profile::data.current_alloc_size += count;
    memory_profile::data.max_alloc_size =
        std::max(memory_profile::data.max_alloc_size,
                 memory_profile::data.current_alloc_size);
  }
#endif
  return allocate(count);
}

void *operator new(std::size_t count, std::align_val_t al) {

  const size_t alignment = static_cast<std::size_t>(al);
#ifdef PRINTER
  if (memory_profile::data.collect) {
    memory_profile::data
        .alloc_with_allognment[std::make_pair(count, alignment)]++;
    memory_profile::data.current_alloc_size += count;
    memory_profile::data.max_alloc_size =
        std::max(memory_profile::data.max_alloc_size,
                 memory_profile::data.current_alloc_size);
  }
#endif
  return allocate(alignment, count);
}

void *operator new[](std::size_t count, std::align_val_t al) {

  const size_t alignment = static_cast<std::size_t>(al);
#ifdef PRINTER
  if (memory_profile::data.collect) {
    memory_profile::data
        .alloc_with_allognment[std::make_pair(count, alignment)]++;
    memory_profile::data.current_alloc_size += count;
    memory_profile::data.max_alloc_size =
        std::max(memory_profile::data.max_alloc_size,
                 memory_profile::data.current_alloc_size);
  }
#endif
  return allocate(alignment, count);
}

void *operator new(std::size_t count, const std::nothrow_t &) {

#ifdef PRINTER
  if (memory_profile::data.collect) {
    memory_profile::data.alloc_without_allognment[count]++;
    memory_profile::data.current_alloc_size += count;
    memory_profile::data.max_alloc_size =
        std::max(memory_profile::data.max_alloc_size,
                 memory_profile::data.current_alloc_size);
  }
#endif
  return allocate(0, count);
}

void *operator new[](std::size_t count, const std::nothrow_t &) {
#ifdef PRINTER
  if (memory_profile::data.collect) {
    memory_profile::data.alloc_without_allognment[count]++;
    memory_profile::data.current_alloc_size += count;
    memory_profile::data.max_alloc_size =
        std::max(memory_profile::data.max_alloc_size,
                 memory_profile::data.current_alloc_size);
  }
#endif
  return allocate(0, count);
}

void *operator new(std::size_t count, std::align_val_t al,
                   const std::nothrow_t &) {

  const size_t alignment = static_cast<std::size_t>(al);
#ifdef PRINTER
  if (memory_profile::data.collect) {
    memory_profile::data
        .alloc_with_allognment[std::make_pair(count, alignment)]++;
    memory_profile::data.current_alloc_size += count;
    memory_profile::data.max_alloc_size =
        std::max(memory_profile::data.max_alloc_size,
                 memory_profile::data.current_alloc_size);
  }
#endif
  return allocate(alignment, count);
}

void *operator new[](std::size_t count, std::align_val_t al,
                     const std::nothrow_t &) {
  const size_t alignment = static_cast<std::size_t>(al);
#ifdef PRINTER
  if (memory_profile::data.collect) {
    memory_profile::data
        .alloc_with_allognment[std::make_pair(count, alignment)]++;
    memory_profile::data.current_alloc_size += count;
    memory_profile::data.max_alloc_size =
        std::max(memory_profile::data.max_alloc_size,
                 memory_profile::data.current_alloc_size);
  }
#endif
  return allocate(alignment, count);
}

void operator delete(void *ptr) noexcept { deallocate(ptr, 0); }

void operator delete[](void *ptr) noexcept { deallocate(ptr, 0); }

void operator delete(void *ptr, std::size_t sz) noexcept {
#ifdef PRINTER
  if (memory_profile::data.collect) {
    memory_profile::data.current_alloc_size -= sz;
    memory_profile::data.max_alloc_size =
        std::max(memory_profile::data.max_alloc_size,
                 memory_profile::data.current_alloc_size);
  }
#endif
  deallocate(ptr, sz);
}

void operator delete[](void *ptr, std::size_t sz) noexcept {
#ifdef PRINTER
  if (memory_profile::data.collect) {
    memory_profile::data.current_alloc_size -= sz;
    memory_profile::data.max_alloc_size =
        std::max(memory_profile::data.max_alloc_size,
                 memory_profile::data.current_alloc_size);
  }
#endif
  deallocate(ptr, sz);
}

void operator delete(void *ptr, std::align_val_t al) noexcept {
  deallocate(ptr, 0);
}

void operator delete[](void *ptr, std::align_val_t al) noexcept {
  deallocate(ptr, 0);
}

void operator delete(void *ptr, std::size_t sz, std::align_val_t al) noexcept {
#ifdef PRINTER
  if (memory_profile::data.collect) {
    memory_profile::data.current_alloc_size -= sz;
    memory_profile::data.max_alloc_size =
        std::max(memory_profile::data.max_alloc_size,
                 memory_profile::data.current_alloc_size);
  }
#endif
  deallocate(ptr, sz);
}

void operator delete[](void *ptr, std::size_t sz,
                       std::align_val_t al) noexcept {
#ifdef PRINTER
  if (memory_profile::data.collect) {
    memory_profile::data.current_alloc_size -= sz;
    memory_profile::data.max_alloc_size =
        std::max(memory_profile::data.max_alloc_size,
                 memory_profile::data.current_alloc_size);
  }
#endif
  deallocate(ptr, sz);
}