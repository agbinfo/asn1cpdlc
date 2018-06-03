#pragma once

#include <utility>
#include <memory>
#include <string>
#include <vector>
#include <iostream>

#pragma warning(disable : 4709)
namespace Asn1 {
  struct bound_exception {};
  struct unassigned_exception {};

  class Serializer
  {
  public:
    void write(std::ostream&) const {} // copies content to ostream
    void read(std::istream&) {}
    void push_back(bool); // write a single bit
    void push_back(uint32_t v); // write 32 bits
    void push_back(uint32_t v, unsigned count); // write count bits
  private:
    std::vector<bool> data;
    static const uint32_t mask[32];
  };

  const uint32_t Serializer::mask[] = {
    0x80000000, 0x40000000, 0x20000000, 0x10000000, 0x08000000, 0x04000000, 0x02000000, 0x01000000,
    0x00800000, 0x00400000, 0x00200000, 0x00100000, 0x00080000, 0x00040000, 0x00020000, 0x00010000,
    0x00008000, 0x00004000, 0x00002000, 0x00001000, 0x00000800, 0x00000400, 0x00000200, 0x00000100,
    0x00000080, 0x00000040, 0x00000020, 0x00000010, 0x00000008, 0x00000004, 0x00000002, 0x00000001,
  };

  void Serializer::push_back(bool v)
  {
    data.push_back(v);
  }
  void Serializer::push_back(uint32_t v)
  {
    uint32_t m = 0x80000000u;
    while (m)
    {
      data.push_back((v & m)!=0);
      m >>= 1;
    }
  }
  void Serializer::push_back(uint32_t v, unsigned count)
  {
    v >>= (32 - count);
    uint64_t m = Serializer::mask[(32-count) & 31];
    while (m)
    {
      data.push_back((v & m) != 0);
      m >>= 1;
    }
  }

  struct AsnNull { int x; };

  template <int LowerBound, int UpperBound>
  class Integer
  {
  public:
    operator const int&() const { return v; }
    Integer& operator=(Integer rhs) { v = rhs.v; return *this; }
    Integer& operator=(int rhs)
    {
      if (rhs<LowerBound || rhs>UpperBound) { throw bound_exception(); }
      v = rhs.v;
      return *this;
    }
  private:
    int v;
  };

  template <int LowerBound, int UpperBound>
  Serializer& operator<<(Serializer& s, Integer<LowerBound, UpperBound> v)
  {
    s.push_back(static_cast<uint32_t>(static_cast<int>(v)-LowerBound), bitwidth(UpperBound - LowerBound));
    return s;
  }

  template <int LowerBound, int UpperBound, typename T>
  class SequenceOf
  {
  public:
    const T& operator[](int pos) const
    {
      if (pos<LowerBound || pos>UpperBound || pos - LowerBound >= vec_.size()) { throw bound_exception(); }
      return vec_[pos - LowerBound];
    }
    T& operator[](int pos)
    {
      if (pos<LowerBound || pos>UpperBound) { throw bound_exception(); }
      while (static_cast<size_t>(pos - LowerBound) >= vec_.size()) { vec_.push_back(T()); }
      return vec_[pos - LowerBound];
    }
    typename std::vector<T>::iterator begin() { return vec_.begin(); }
    typename std::vector<T>::const_iterator begin() const { return vec_.begin(); }
    typename std::vector<T>::iterator cbegin() const { return vec_.cbegin(); }
    typename std::vector<T>::iterator end() { return vec_.end(); }
    typename std::vector<T>::const_iterator end() const { return vec_.end(); }
    typename std::vector<T>::const_iterator cend() const { return vec_.cend(); }
  private:
    std::vector<T> vec_;
  };

  template <typename T>
  class Optional
  {
  public:
    const T& get() const {
      if (!data_) { throw unassigned_exception(); }
      return *data_;
    }
    T& get() {
      if (!data_) { data_ = std::move(std::unique_ptr<T>(new T())); }
      return *data_;
    }
    bool opt() const { return static_cast<bool>(data_); }
  private:
    std::unique_ptr<T> data_;
  };

  class Choice
  {
  public:
    Choice() : choice_(nullptr), choiceIndex_(0) {}
    ~Choice() { delete choice_; }
    void * choice_;
    unsigned choiceIndex_;

    template <typename T>
    typename T::ReturnType & as()
    {
      if (choiceIndex_ != T::Index || !choice_)
      {
        delete choice_;
        choice_ = new T::ReturnType;
        choiceIndex_ = T::Index;
      }
      return *reinterpret_cast<T::ReturnType*>(choice_);
    }

    template <typename T>
    typename const T::ReturnType & as() const
    {
      if (choiceIndex_ != T::Index || !choice_)
      {
        throw unassigned_exception();
      }
      return *reinterpret_cast<T::ReturnType*>(choice_);
    }

    template <typename T>
    bool isa() const
    {
      if (!choice_) return false;
      return T::Index == choiceIndex_;
    }
  };

  // Note : Doesn't handle alphabet restrictions such as FROM("ABCDEF") so encoding is based on entire IA5String alphabet
  template <size_t LowerBound, size_t UpperBound=LowerBound>
  class IA5String
  {
  public:
    IA5String() : s_(LowerBound, ' ') {}
    IA5String& operator=(std::string & s) {
      if (s.length() < LowerBound || s.length() > UpperBound) { throw bound_exception(); }
      s_ = s;
      return *this;
    }
    operator std::string() { return s_; }
    size_t size() const { return s_.size(); }
    auto cbegin() const { return s_.cbegin(); }
    auto cend() const { return s_.cend(); }
    auto begin() { return s_.begin(); }
    const auto begin() const { return s_.cbegin(); }
    auto end() { return s_.end(); }
    const auto end() const { return s_.cend(); }
  private:
    std::string s_;
  };

  template <size_t LowerBound, size_t UpperBound = LowerBound>
  class OctetString
  {
  public:
    OctetString() : s_(LowerBound, ' ') {}
    OctetString& operator=(std::string & s) {
      if (s.length() < LowerBound || s.length() > UpperBound) { throw bound_exception(); }
      s_ = s;
      return *this;
    }
    operator std::string() { return s_; }
    size_t size() const { return s_.size(); }
    auto cbegin() const { return s_.cbegin(); }
    auto cend() const { return s_.cend(); }
    auto begin() { return s_.begin(); }
    const auto begin() const { return s_.cbegin(); }
    auto end() { return s_.end(); }
    const auto end() const { return s_.cend(); }
  private:
    std::string s_;
  };

  unsigned bitwidth(size_t range)
  {
    //    0 1 2 3 4 5 6 7 8 9
    // /2 - 0 1 1 2 2 3 3 4 4
    // /4 - - 0 0 1 1 1 1 2 2
    // w  0 1 2 2 3 3 3 3 
    unsigned bitwidth_ = 0;
    while (range>0) { ++bitwidth_; range /= 2; }
    return bitwidth_;
  }

  template <size_t LowerBound, size_t UpperBound = LowerBound>
  Serializer& operator << (Serializer& os, const IA5String<LowerBound, UpperBound> & s)
  {
#pragma warning(push)
#pragma warning(disable : 4127) // Conditional expression is constant
    if (UpperBound != LowerBound) {
#pragma warning(pop)
      os.push_back(s.size() - LowerBound, bitwidth(UpperBound - LowerBound));
    }
    for (auto c : s)
    {
      os.push_back(c, 5);
    }
    return os;
  }

  template <size_t LowerBound, size_t UpperBound = LowerBound>
  Serializer& operator << (Serializer& os, const OctetString<LowerBound, UpperBound> & s)
  {
#pragma warning(push)
#pragma warning(disable : 4127) // Conditional expression is constant
    if (UpperBound != LowerBound) {
#pragma warning(pop)
      os.push_back(s.size() - LowerBound, bitwidth(UpperBound - LowerBound));
    }
    // for (auto c = s.begin(); c != s.end(); ++c)
    for (auto c : s)
    {
      os.push_back(c, 8);
    }
    return os;
  }

  template <size_t LowerBound, size_t UpperBound = LowerBound, typename T>
  Asn1::Serializer& operator << (Serializer& os, const SequenceOf<LowerBound, UpperBound, T> & s)
  {
    for (auto e : s)
    {
      os << e;
    }
    return os;
  }
}

