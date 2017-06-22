//
// Created by simanstar on 16-12-27.
//

#include "string.h"

namespace startl {
string::string() : m_size(0), m_capacity(0), m_data(nullptr){

}

string::string(const char * c) : m_size(0), m_capacity(0), m_data(nullptr){
  for (size_type i = 0; c[i] != '\0'; ++i) {
    if (i >= m_capacity) grow();
    m_data[i] = c[i];
    m_size++;
  }
}


string::string(const char *c, size_type n) {
  for (size_type i = 0; i < n; ++i) {
    if (i >= m_capacity) grow();
    m_data[i] = c[i];
    m_size++;
  }
}

string::string(const string& atr) : m_size(atr.m_size), m_capacity(atr.m_capacity){
  m_data = new char[m_capacity];
  for (size_type i = 0; i < m_size; ++i) {
    m_data[i] = atr.m_data[i];
  }
}

string::~string() {
  m_size = 0;
  m_capacity = 0;
  delete[] m_data;
}

void string::grow() {
  m_capacity = m_capacity == 0 ? 1 : 2 * m_capacity;
  char * tmp = new char[m_capacity];
  for (size_type i = 0; i < m_size; ++i)
    tmp[i] = m_data[i];
  delete[] m_data;
  m_data = tmp;
}

void string::getline(istream &in) {
  //while (in.get() != EOF)
}

void string::clear() {
  m_size = 0;
}

string::size_type string::length() const {
  return m_size;
}

string::size_type string::size() const {
  return m_size;
}

string::size_type string::capacity() const {
  return m_capacity;
}

char &string::operator[](size_type i) {
  return m_data[i];
}

string &string::operator=(const string & atr) {
  m_size = 0;
  for (size_type i = 0; i < atr.m_size; ++i) {
    if (i >= m_capacity) grow();
    m_data[i] = atr.m_data[i];
    m_size++;
  }
  return *this;
}

string &string::operator=(const char *s) {
  m_size = 0;
  for (size_type i = 0; s[i] != '\0'; ++i) {
    if (i >= m_capacity) grow();
    m_data[i] = s[i];
    m_size++;
  }
  return *this;
}

string &string::operator=(char c) {
  m_size = 0;
  if (c != '\0') {
    if (0 >= m_capacity) grow();
    m_data[0] = c;
    m_size++;
  }
  return *this;
}

string &string::operator+=(const string & atr) {
  for (size_type i = 0; i <  atr.m_size; ++i) {
    if ( m_size >= m_capacity) grow();
    m_data[m_size] = atr.m_data[i];
    m_size++;
  }
  return *this;
}

string &string::operator+=(const char *s) {
  for (size_type i = 0; s[i] !=  '\0'; ++i) {
    if ( m_size >= m_capacity) grow();
    m_data[m_size] = s[i];
    m_size++;
  }
  return *this;
}

string &string::operator+=(char c) {
  if(c !=  '\0') {
    if ( m_size >= m_capacity) grow();
    m_data[m_size] = c;
    m_size++;
  }
  return *this;
}

istream & operator >>(istream &input, string &s) {
  for (string::size_type i = 0 ; i < s.m_size; ++i)
    input >> s.m_data[i];
  return input;
}

ostream & operator << (ostream &output, const string &s) {
  for (string::size_type i = 0 ; i < s.m_size; ++i)
    output << s.m_data[i];
  return output;
}

bool operator>(const string &lhs, const string &rhs) {
  for (string::size_type i = 0 ; i < lhs.m_size && i < rhs.m_size ; ++i) {
    if (lhs.m_data[i] != rhs.m_data[i])
      return lhs.m_data[i] > rhs.m_data[i];
  }
  return lhs.m_size > rhs.m_size;

}

bool operator>=(const string &lhs, const string &rhs) {
  return (lhs > rhs || lhs == rhs);
}

bool operator<(const string &lhs, const string &rhs) {
  return rhs > lhs;
}

bool operator<=(const string &lhs, const string &rhs) {
  return rhs >= lhs;
}

bool operator==(const string &lhs, const string &rhs) {
  for (string::size_type i = 0 ; i < lhs.m_size && i < rhs.m_size ; ++i) {
    if (lhs.m_data[i] != rhs.m_data[i])
      return false;
  }
  return lhs.m_size == rhs.m_size;
}

bool operator!=(const string &lhs, const string &rhs) {
  return !(lhs == rhs);
}

string operator+(const string &lhs, const string &rhs) {
  string::size_type total_size = lhs.m_size + rhs.m_size;
  char* tmp = new char[total_size];
  for (string::size_type i = 0; i < lhs.m_size; ++i) {
    tmp[i] = lhs.m_data[i];
  }
  for (string::size_type i = 0; i < rhs.m_size; ++i) {
    tmp[lhs.m_size + i] = rhs.m_data[i];
  }
  return string(tmp, total_size);

}

string operator+(const string &lhs, const char *rhs) {
  string::size_type rhs_len = 0;
  for (; rhs[rhs_len] != '\0'; ++rhs_len);
  string::size_type total_size = lhs.m_size + rhs_len;
  char* tmp = new char[total_size];
  for (string::size_type i = 0; i < lhs.m_size; ++i) {
    tmp[i] = lhs.m_data[i];
  }
  for (string::size_type i = 0; i < rhs_len; ++i) {
    tmp[lhs.m_size + i] = rhs[i];
  }
  return string(tmp, total_size);

}

string operator+(const char *lhs, const string &rhs) {
  return rhs + lhs;
}

string operator+(const string &lhs, char rhs) {
  string::size_type total_size = lhs.m_size + 1;
  char* tmp = new char[total_size];
  for (string::size_type i = 0; i < lhs.m_size; ++i) {
    tmp[i] = lhs.m_data[i];
  }
    tmp[lhs.m_size] = rhs;
  return string(tmp, total_size);
}

string operator+(char lhs, const string &rhs) {
  return rhs + lhs;
}




}



































