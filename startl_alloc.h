//
// Created by simanstar on 16-9-13.
//

#ifndef STARTL_STARTL_ALLOC_H
#define STARTL_STARTL_ALLOC_H
#include <new>
#include <iostream>

#define __THROW_BAD_ALLOC std::cerr << "out of memory" << std::endl; exit(1)

//原stl 采用第二级调用第一级空间配置器的方式
//本设计两级完全独立，没有调用关系，有待研究效率问题
//first level
template <int inst>
class __first_level_alloc {
private:
  static void *oom_malloc(size_t);
  static void *oom_realloc(void*, size_t);
  static void (*oom_handler)();

public:
  static  void* allocate(size_t n) {
    void *result = malloc(n);
    if (0 == result) result = oom_malloc(n);
    return result;
  }

  static void deallocate(void* p) {
    free(p);
  }

  static void* reallocate(void* p, size_t new_sz) {
    void* result = realloc(p, new_sz);
    if (0 == result) result = oom_realloc(p, new_sz);
    return result;
  }

  static void (*set_oom_handler(void (*f)()))() {
    void (*old)() = oom_handler;
    oom_handler = f;
    return old;
  }
};

template <int inst>
void (* __first_level_alloc<inst>::oom_handler)() = 0;

template <int inst>
void* __first_level_alloc<inst>::oom_malloc(size_t n) {
  for (;;) {
    if (nullptr == oom_handler) {
      __THROW_BAD_ALLOC;
    }
    (*oom_handler)();
    auto result = malloc(n);
    if (result) return result;
  }
}

template  <int inst>
void* __first_level_alloc<inst>::oom_realloc(void* p, size_t n) {
  for (;;) {
    if (0 == oom_handler) {
      __THROW_BAD_ALLOC;
    }
    (*oom_handler)();
    auto result = realloc(p, n);
    if (result) return result;
  }
}

//second level
enum {__ALIGN = 8};
enum {__MAX_BYTES = 128};
enum {__NFREELISTS = __MAX_BYTES/__ALIGN};

template  <bool threads, int inst>
class __second_level_alloc {

private:
  static size_t round_up(size_t bytes) {
    return ((bytes + __ALIGN-1) & ~(__ALIGN - 1));
  }

  union obj {
    union obj* free_list_link;
    char client_data[1];
  };

  static size_t FREELIST_INDEX(size_t bytes) {
    return ((bytes + __ALIGN-1)/__ALIGN - 1);
  }
  static  obj* volatile free_list[__NFREELISTS];
  static void* refill(size_t n);
  static char* chunk_alloc(size_t size, int& nobjs);
  static void *oom_malloc(size_t);
  static void (*oom_handler)();
  static char* start_free;
  static char* end_free;
  static size_t heap_size;

public:
  static void* allocate(size_t n) {
    obj * volatile * my_free_list; //volatile在这里的作用？什么时候该用volatile
    my_free_list = free_list + FREELIST_INDEX(n);
    auto result = *my_free_list;
    if (nullptr == result) {
      //无可用的free list,准备重新填充free list
      void *r = refill(round_up(n));
      return r;
    }
    //调整free list
    *my_free_list = result->free_list_link;
    return result;
  }

  static void deallocate(void* p, size_t n) {
    auto q = (obj *)p;
    obj * volatile * my_free_list;
    //找对应的free list
    my_free_list = free_list + FREELIST_INDEX(n);
    //调整free list,回收区块
    q->free_list_link = *my_free_list;
    *my_free_list = q;
  }

  static void* reallocate(void* p, size_t old_sz, size_t new_sz);

  static void (*set_oom_handler(void (*f)()))() {
    void (*old)() = oom_handler;
    oom_handler = f;
    return old;
  }
};

template <bool threads, int inst>
char * __second_level_alloc<threads, inst>::start_free = 0;

template  <bool threads, int inst>
char * __second_level_alloc<threads, inst>::end_free = 0;

template  <bool threads, int inst>
size_t __second_level_alloc<threads, inst>::heap_size = 0;

template  <bool threads, int inst>
void (* __second_level_alloc<threads, inst>::oom_handler)() = 0;

template <bool threads, int inst>
typename __second_level_alloc<threads, inst>::obj * volatile __second_level_alloc<threads, inst>::free_list[__NFREELISTS] =
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,};

template <bool threads, int inst>
void * __second_level_alloc<threads, inst>::refill(size_t n) {
  int nobjs = 20;
  auto chunk = chunk_alloc(n, nobjs);
  obj * volatile * my_free_list;
  if (1 == nobjs ) return chunk;
  my_free_list = free_list = FREELIST_INDEX(n);
  auto result = (obj*)chunk; //这一块准备返回给客户端
  auto next_obj = (obj*)(chunk + n);
  *my_free_list = next_obj;
  //将free list的各节点串接起来
  for (auto i = 1; ; i++) {
    auto current_obj = next_obj;
    next_obj = (obj*)((char*)next_obj + n);
    if (nobjs - 1 ==i) {
      current_obj->free_list_link = nullptr;
      break;
    }
    else {
      current_obj->free_list_link = next_obj;
    }
  }
  return result;
}

template <bool threads, int inst>
char * __second_level_alloc<threads, inst>::chunk_alloc(size_t size, int &nobjs) {
  char * result;
  size_t total_bytes = size * nobjs;
  size_t bytes_left = end_free - start_free;

  if (bytes_left >= total_bytes) {
    //内存池剩余空间完全满足需求量
    result = start_free;
    start_free += total_bytes;
    return result;
  } else if (bytes_left >= size) {
    //内存池空间不能完全满足，但可以供应一个以上的区块
    nobjs = bytes_left/size;
    total_bytes = size * nobjs;
    result = start_free;
    start_free += total_bytes;
    return result;
  } else {
    //内存池一个区块的大小都无法提供
    size_t bytes_to_get = 2 * total_bytes + round_up(heap_size >> 4); //附加量？？？
    //以下把内存池剩余空间插入free list (8的倍数)
    if (bytes_left > 0) {
      obj * volatile * my_free_list = free_list + FREELIST_INDEX(bytes_left);
      ((obj*)start_free) -> free_list_link = *my_free_list;
      *my_free_list = (obj*)start_free;
    }
    //从heap空间补充内存池
    start_free = (char *)malloc(bytes_to_get);
    if (nullptr == start_free) {
      //heap空间不足，malloc()失败,尝试从free_list中获取块
      int i;
      obj * volatile * my_free_list, *p;
      for (i = size; i <= __MAX_BYTES; i += __ALIGN) {
        my_free_list = free_list + FREELIST_INDEX(i);
        p = *my_free_list;
        if (nullptr !=p) {
          *my_free_list = p->free_list_link;
          start_free = (char *)p;
          end_free = start_free + i;
          return (chunk_alloc(size, nobjs));
        }
      }
      end_free = nullptr; //？？？为什么为空指针
      //free_list没有块可以获取
      start_free = (char *)oom_malloc(bytes_to_get);
    }
    heap_size += bytes_to_get;
    end_free = start_free + bytes_to_get;

  }
}

template <bool threads, int inst>
void* __second_level_alloc<threads,inst>::oom_malloc(size_t n) {
  for (;;) {
    if (nullptr == oom_handler) {
      __THROW_BAD_ALLOC;
    }
    (*oom_handler)();
    auto result = malloc(n);
    if (result) return result;
  }
}
#endif //STARTL_STARTL_ALLOC_H
