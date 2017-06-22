//
// Created by simanstar on 16-12-27.
//

#ifndef STARTL_STRING_H
#define STARTL_STRING_H

#include <glob.h>
#include "iostream"
using std::istream;
using std::ostream;

namespace startl {
class string {

public:
  typedef size_t size_type;

private:
  char* m_data;
  size_type m_size;
  size_type m_capacity;



  void grow();
public:
  string();
  string(const char* c);
  string(const char* c , size_type n);
  string(const string& atr);
  ~string();
  void getline(istream& in);
  void clear();
  size_type size() const;
  size_type length() const;
  size_type capacity() const;

  //operators
  char & operator [] (size_type i);
  string& operator = (const string&);
  string& operator = (const char* s);
  string& operator = (char c);
  string& operator += (const string&);
  string& operator += (const char* s);
  string& operator += (char c);

  friend istream& operator >> (istream& input, string& s);
  friend ostream& operator << (ostream& output, const string& s);
  friend bool operator > (const string& lhs, const string& rhs);
  friend bool operator >= (const string& lhs, const string& rhs);
  friend bool operator < (const string& lhs, const string& rhs);
  friend bool operator <= (const string& lhs, const string& rhs);
  friend bool operator == (const string& lhs, const string& rhs);
  friend bool operator != (const string& lhs, const string& rhs);

  friend string operator + (const string& lhs, const string& rhs);
  friend string operator + (const string& lhs, const char* rhs);
  friend string operator + (const char* lhs, const string& rhs);
  friend string operator + (const string& lhs, char rhs);
  friend string operator + (char lhs, const string& rhs);





};
}

#endif //STARTL_STRING_H
