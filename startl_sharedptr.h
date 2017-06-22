//
// Created by simanstar on 16-12-27.
//

#ifndef STARTL_STARTL_SHAREDPTR_H
#define STARTL_STARTL_SHAREDPTR_H

namespace startl {
template <class T>
class SharedPtr {
private:
  T* ptr;
  void set_ptr(T*);

public:
  int* use_count;
  SharedPtr();
  SharedPtr(T*);
  SharedPtr(SharedPtr&);
  ~SharedPtr();

  void operator=(SharedPtr&);
  bool operator==(T*);
  bool operator==(SharedPtr&);
  bool operator<(T*);
  bool operator<(SharedPtr&);
  T& operator*();
  T* operator->();
  explicit operator bool();

  T* get();
  int get_use_count();
  void reset(T*);
  void set_use_count(int);
  bool unique();

};

template <class T>
SharedPtr<T>::SharedPtr() {
  use_count = new int(-1);
  set_ptr(nullptr);
}

template <class T>
SharedPtr<T>::SharedPtr(T* ptr) {
  use_count = new int(1);
  set_ptr(ptr);
}

template <class T>
SharedPtr<T>::SharedPtr(SharedPtr & shared_ptr) {
  use_count = new int(-1);
  *this = shared_ptr;
}

template <class T>
SharedPtr<T>::~SharedPtr() {
  if (get_use_count() == 1) {
    delete use_count;
    delete ptr;
  } else if (get_use_count() == -1) {
    delete use_count;
  } else {
    set_use_count(get_use_count() - 1);
  }
}

template <class T>
void SharedPtr<T>::operator=(SharedPtr & shared_ptr) {
  set_ptr(shared_ptr.get());
  delete use_count;
  use_count = shared_ptr.use_count;
  set_use_count(get_use_count() + 1);
}

template <class T>
bool SharedPtr<T>::operator==(T* cmpPtr) {
  return ptr == cmpPtr;
}

template <class T>
bool SharedPtr<T>::operator==(SharedPtr & shared_ptr) {
  return ptr == shared_ptr.get();
}

template <class T>
bool SharedPtr<T>::operator<(T* cmpPtr) {
  return ptr < cmpPtr;
}

template <class T>
bool SharedPtr<T>::operator<(SharedPtr & shared_ptr) {
  return ptr < shared_ptr.get();
}

template <class T>
T& SharedPtr<T>::operator*() {
  return *ptr;
}

template <class T>
T* SharedPtr<T>::operator->() {
  return ptr;
}

template <class T>
SharedPtr<T>::operator bool() {
  return ptr != nullptr ? true : false;
}

template <class T>
int SharedPtr<T>::get_use_count() {
  return *use_count;
}

template <class T>
void SharedPtr<T>::reset(T *newPtr) {
  delete use_count;
  use_count = new int(1);
  delete ptr;
  set_ptr(newPtr);
}

template <class T>
void SharedPtr<T>::set_ptr(T * ptr) {
  this->ptr = ptr;
}

template <class T>
void SharedPtr<T>::set_use_count(int count) {
  *use_count = count;
}

template <class T>
bool SharedPtr<T>::unique() {
  return get_use_count() == 1;
}

}
#endif //STARTL_STARTL_SHAREDPTR_H
