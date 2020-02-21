#pragma once

#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <sstream>
#include <memory>

namespace Asn1 {
  class Sequence;
  class Choice;

  const int Unbounded = INT_MIN;

  // Primitive Types
  enum Asn1Type {
    Null,
    Boolean,
    Integer,
    EnumeratedType,
    Real,
    Bitstring,
    OctetString,
    SequenceType,
    SequenceOfType,
    //Set,
    SetOf,
    ChoiceType,
    Open,
    ObjectIdentifier,
    RelativeObjectIdentifier,
    EmbeddedPdv,
    External,
    RestrictedCharacterString,
    UnrestrictedCharacterString,
    OID
  };

  // The effective permitted-alphabet constraints are PER-visible only for these types:
  enum Asn1StringType {
    Numeric,
    Printable,
    Visible,
    ISO646 = Asn1::Visible,
    IA5,
    BMP,
    Universal,
    // not mentionned? should they?
    Bit,
    Octet
  };

  class Asn1Any
  {
  protected:
    virtual ~Asn1Any() {}
  };

  template <typename Derived, Asn1Type BaseType>
  class Asn1Object : public Asn1Any {
  public:
    typedef Asn1Object BaseObjectType;
    static Asn1Type getType() { return BaseType; }
    // This method shall return true iff all elements that
    // are required and without a default value have been assigned values
    // that are initialized as well.
    //virtual bool isInitialized() const = 0;
    //virtual bool extensible() const { return false; }
    //virtual bool extended() const { return false; }
  };

  class Oid : public Asn1Object<Oid, OID> {
  private:
    int oid_;
  public:
    operator int() { return oid_; }
    Oid& operator=(int oid) { oid_ = oid; return *this; }
  };

  // The NULL object holds no value other than its presence/absence.
  class Asn1NULL : public Asn1Object<Asn1NULL, Asn1Type::Null> {
  public:
    Asn1NULL() {}
  };

  template <int Min, int Max, typename PrimitiveType=int>
  class Enumerated : public Asn1Object<Enumerated<Min,Max,PrimitiveType>, EnumeratedType> {
  private:
    PrimitiveType value_;
  public:
    int value() const { return value_; }
    Enumerated(const PrimitiveType& value) : value_(value) { }
    Enumerated() { }
    Enumerated& operator=(const PrimitiveType& value) { value_ = value; return *this; }
  };

  // The base class for an element contained within a CHOICE
  class ChoiceElementBase {
  protected:
    ChoiceElementBase(const ChoiceElementBase&) { }
    ChoiceElementBase() { }
  public:
    virtual const Asn1Any& element() const = 0;
    virtual Asn1Any& element() = 0;
    virtual int index() const = 0;
  };

  namespace Inserter {
    // Defines a type that can be used for adding elements to an ASN.1 composite (SEQUENCE or CHOICE)
    template <typename CompositeT> class Inserter {
      CompositeT& container_;
      std::back_insert_iterator< typename CompositeT::ContainerType_ > it_;
    public:
      explicit Inserter(CompositeT& container) : container_(container), it_(container.elements()) {}
      Inserter& operator*() { return *this; }
      Inserter& operator++() { return *this; }
      Inserter& operator++(int) { return *this; }
      Inserter& operator<<(typename CompositeT::BaseElement_& rhs) {
        return operator=(rhs);
      }
      Inserter& operator=(typename CompositeT::BaseElement_& rhs) {
        rhs.setContainer(container_);
        *it_++ = &rhs;
        return *this;
      }
    };
  }


  // Defines an Asn1 Choice object.
  // Example:
  // class Mychoice : public Asn1::Choice {
  //   Option<0, Asn1Type1>::type firstChoice() { return Option<0, Asn1Type1>::getElement(*this); }
  //   Option<1, Asn1Type2>::type secondChoice() { return Option<1, Asn1Type2>::getElement(*this); }
  //   ...
  //   MyChoice() { }
  // }
  class Choice : public Asn1Object<Choice, ChoiceType> {
  private:
    int selected_;
    std::shared_ptr<Asn1Any> element_;  // The selected element or nullptr if none selected

  private:
    //const ChoiceElementBase& base_element() const {
    //  if (element_) return *element_;
    //  throw std::exception("No Element Set");
    //}
    //ChoiceElementBase& base_element() {
    //  return const_cast<ChoiceElementBase&>( static_cast<const Choice&>(*this).base_element() );
    //}
  protected:
    template <int N, typename T>
    T& select() {
      if (selected_ != N) {
        element_.reset(new T());
      }
      return dynamic_cast<T&>(*element_.get());
    }

  public:
    Choice() : selected_(-1) {}

    bool hasElement() const { return element_ != nullptr; }
    void reset() { element_.reset(); selected_ = -1; }
    int selected() const { return selected_; }

  private: 
    void reset(int i, std::shared_ptr<Asn1Any> && e)
    {
      element_ = std::move(e);
      selected_ = i;
    }

  public:
    template <int X, typename T>
    class Option {
      Choice & parent_;
    public:
      Option& operator=(const Option&) {}
      Option& operator=(const Option&&) {}
      Option(const Option& other) : parent_(other.parent_) {}

      Option(Choice& parent) : parent_(parent) {}
      bool isSelected() const { return parent_.hasElement() && parent_.selected_ == X; }
      bool reset() { if (isSelected()) { parent_.reset(); } }
      const T* get() const
      {
        if (!isSelected()) {
          return nullptr;
        }
        return dynamic_cast<T*>(parent_.element_.get());
      }
      T* get()
      {
        if (!isSelected()) {
          parent_.reset(X, std::shared_ptr<T>(new T()));
        }
        return dynamic_cast<T*>(parent_.element_.get());
      }
      T& operator*() {
        return *get();
      }
      T* operator->() {
        return get();
      }
      Option& operator=(const T& rhs) {
        parent_.reset(X, std::shared_ptr<T>(new T(rhs)));
        return *this;
      }
    };
  };

  // Wraps an object so that it can have an uninitialized state
  // T should be:
  // (a) CopyConstructible:
  //     T u = rv; // Sets the value of u equivalent to the value of rv
  //     T(rv);    // Creates an object equivalent to rv
  // The value of rv is unchanged
  //     
  template <class T>
  struct Optional {
    std::shared_ptr<T> element_;
    Optional() {}
    Optional(const T& value) { element_ = std::shared_ptr<T>(new T(value)); }
    Optional(const Optional& rhs)
    {
      if (rhs.element_)
      {
        element_ = std::shared_ptr<T>(new T(*rhs.element_));
      }
    }
    ~Optional() { }
    Optional& operator=(Optional rhs)
    {
      element_ = std::shared_ptr<T>(new T(rhs.element_));
      return *this;
    }
    void construct() {
      element_ = std::shared_ptr<T>(new T());
    }
    void construct(const T& value) {
      element_ = std::shared_ptr<T>(new T(value));
    }
    void destroy() {
      element_ = nullptr;
    }
    T* operator->() {
      if (!element_)
      {
        element_ = std::shared_ptr<T>(new T());
      }
      return element_.get();
    }
    T& operator()() {
      if (!element_)
      {
        element_ = std::shared_ptr<T>(new T());
      }
      return *element_.get();
    }
    void reset(T* value) {
      element_ = std::shared_ptr<T>(new T(*value));
    }
    T& value() {
      if (!element_) { construct(); }
      return *element_;
    }
    const T& value() const {
      if (!element_) { throw std::exception("element not initialized"); }
      return *element_;
    }
    T& operator*() {
      if (!element_) { construct(); }
      return *element_;
    }
    const T& operator*() const {
      if (!element_) { throw std::exception("element not initialized"); }
      return *element_;
    }
    bool hasElement() const { return element_ != nullptr; }
  };


  // A sequence is simply a grouping of objects
  // If a sequence contains the "..." marker then it is extensible.
  // The serializer encodes a sequence by encoding the extensible bit iff it is present then
  // by encoding the presence of optional+default elements then the elements themselves
  class Sequence : public Asn1Object<Sequence, SequenceType> {
  };

  // Compute field size at compile time when possible
  template<int range>
  struct asn_field_size
  {
    enum {
      size = 1 + asn_field_size<range / 2>::size;
    };
  };
  template<> struct asn_field_size<0>
  {
    enum { size = 0 };
  };

  // A sequence-of is basically a vector with min, max constraints
  // The PER serializer encodes a sequence-of by first writing the number of elements in the
  // sequence then the elements themselves.
  // The number of elements is encoded as a bit field whose size is ceil(log2(max_elements - min_elements + 1))
  // with the value number_of_elements - min_elements.
  template <int LowerBound, int UpperBound>
  class SequenceOfBase {
  //private:
  //  static const int min_index_ = LowerBound;
  //  static const int max_index_ = UpperBound;
  //protected:
  //  SequenceOfBase() { }
  //  SequenceOfBase(const SequenceOfBase& rhs) { }
  //  SequenceOfBase& operator=(const SequenceOfBase& rhs) { }
  public:
    static int min_index() { return LowerBound; }
    static int max_index() { return UpperBound; }
    static int field_size() {
      return asn_field_size<UpperBound - LowerBound>::size;
      //int field_size = 0;
      //int delta = max_index_ - min_index_ + 1;
      //while (delta > 1) { ++field_size; delta = (delta+1) / 2; }
      //return field_size;
    }
  };

  template <typename T, int LowerBound, int UpperBound = LowerBound>
  class SequenceOf : public SequenceOfBase<LowerBound,UpperBound> {
  public:
    typedef T PrimitiveType;
    typedef SequenceOf type;
    typedef typename std::vector<PrimitiveType>::const_iterator const_iterator;
    typedef typename std::vector<PrimitiveType>::iterator iterator;
  private:
    std::vector<T> elements_;
  public:
    SequenceOf() : elements_(LowerBound) {}
    SequenceOf(const SequenceOf& rhs) : SequenceOfBase(rhs)
    {
      for (const_iterator it = rhs.elements_.begin(); it != rhs.elements_.end(); ++it)
      {
        elements_.push_back(T(*it));
      }
    }
    void swap(SequenceOf& other)
    {
      SequenceOfBase::swap(other);
      std::swap(elements_, other.elements_);
    }
    SequenceOf& operator=(SequenceOf rhs)
    {
      swap(rhs);
      return *this;
    }
    T& operator[](unsigned pos) {
      if (pos<0 || pos>UpperBound - LowerBound) {
        throw std::out_of_range("Out of bounds");
      }
      if (pos >= elements_.size()) {
        elements_.resize(pos+1);
      }
      return elements_[pos];
    }
    T& operator[](std::string asn1_index) {
      int i = std::stoi(asn1_index);
      if (i<LowerBound || i>UpperBound) {
        throw std::out_of_range("Out of bounds");
      }
      unsigned pos = i - LowerBound;
      if (pos >= elements_.size()) {
        elements_.resize(pos+1);
      }
      //if (pos < elements_.size()) {
      //  delete elements_[pos];
      //  elements_[pos] = new T;
      //}
      return elements_[pos];
    }
    void assign(iterator first, iterator last)
    {
      elements_.assign(first, last);
    }
    void push_back(const T& elem) {
      elements_.push_back(elem);
    }
    size_t size() const { return elements_.size(); }
    auto begin() const { return elements_.begin(); }
    auto end() const { return elements_.end(); }
    auto begin() { return elements_.begin(); }
    auto end() { return elements_.end(); }
    void construct(unsigned /*i*/) {
      throw std::exception("Not implemented");
    }
    void destroy(unsigned /*i*/) {
      throw std::exception("Not implemented");
    }
    ~SequenceOf() {
      elements_.clear();
    }
  protected:
//    typedef std::vector<SequenceElementBase>::const_iterator const_iterator;
//    typedef std::vector<SequenceElementBase>::iterator iterator;
//    iterator begin() { return elements_.begin(); }
//    const_iterator begin() const { return elements_.begin(); }
//    iterator end() { return elements_.end(); }
//    const_iterator end() const { return elements_.end(); }
//    std::back_insert_iterator< std::vector<SequenceElementBase> > back_insert_iterator() { return std::back_inserter(elements_); }
  };


  class Asn1Boolean : public Asn1Object<Asn1Boolean,Boolean> {
  public:
    typedef bool PrimitiveType;
  private:
    bool initialized_;
    PrimitiveType value_;
  public:
    Asn1Type getType() const { return Asn1::Boolean; }
    PrimitiveType value() const { return value_; }
    bool isInitialized() const { return initialized_; }

    Asn1Boolean() : initialized_(false), value_(false) {}
    Asn1Boolean(const Asn1Boolean& src) : initialized_(src.initialized_), value_(src.value_) {}
    const Asn1Boolean& operator=(const Asn1Boolean& src) {
      initialized_ = src.initialized_;
      value_ = src.value_;
      return *this;
    }
    const Asn1Boolean& operator=(PrimitiveType v) {
      initialized_ = true;
      value_ = v;
    }
    void setValue(PrimitiveType v) {
      initialized_ = true;
      value_ = v;
    }
  };

  class Asn1Integer : public Asn1Object<Asn1Integer,Integer> {
  public:
    typedef int PrimitiveType;
  private:
    bool initialized_;
    int min_;
    int max_;
    PrimitiveType value_;
  public:
    Asn1Type getType() const { return Asn1::Integer; }
    int getMinimum() const { return min_; }
    int getMaximum() const { return max_; }
    PrimitiveType getValue() const { return value_; }
    bool isInitialized() const { return initialized_; }

    Asn1Integer(int min, int max) : min_(min), max_(max), initialized_(false), value_(0) {}
    Asn1Integer(const Asn1Integer& src) : min_(src.min_), max_(src.max_), initialized_(src.initialized_), value_(src.value_) {}
    const Asn1Integer& operator=(const Asn1Integer& src) {
      min_ = src.min_;
      max_ = src.max_;
      initialized_ = src.initialized_;
      value_ = src.value_;
      return *this;
    }
    const Asn1Integer& operator=(PrimitiveType v) {
      initialized_ = true;
      value_ = v;
    }

    void setValue(PrimitiveType v) {
      // assert (min <= v <= max)
      value_ = v;
      initialized_ = true;
    }
  };

  class BitString : public Asn1Object<BitString, Bitstring> {
  public:
    typedef std::string PrimitiveType;
  private:
    bool initialized_;
    int min_size_;
    int max_size_;
    PrimitiveType value_;
  public:
    Asn1Type getType() const { return Bitstring; }
    virtual Asn1StringType getStringType() const { return Octet; }
    int getMinimumSize() const { return min_size_; }
    int getMaximumSize() const { return max_size_; }
    bool isInitialized() const { return initialized_; }

    BitString() : min_size_(0), max_size_(Unbounded), initialized_(false), value_() {}
    BitString(int min, int max) : min_size_(min), max_size_(max), initialized_(false), value_() {}
    BitString(const BitString& src) : min_size_(src.min_size_), max_size_(src.max_size_), initialized_(src.initialized_), value_(src.value_) {}
    ~BitString() {}
    const BitString& operator=(BitString src)
    {
      swap(src);
      return *this;
    }
    void swap(BitString& other)
    {
      if (min_size_ != other.min_size_ || max_size_ != other.max_size_)
      {
        throw std::exception("Cannot swap BitString`s with different boundings");
      }
      std::swap(initialized_, other.initialized_);
      std::swap(value_, other.value_);
    }
    const BitString& operator=(const PrimitiveType v) {
      initialized_ = true;
      value_ = v;
      return *this;
    }
    operator const PrimitiveType() const {
      return value_;
    }
  };

  template <int MinSize = Unbounded, int MaxSize = MinSize>
  class Asn1BitString : public BitString {
  public:
    Asn1BitString() : BitString(MinSize, MaxSize) {}
    Asn1BitString(const Asn1BitString& rhs) : BitString(rhs) { }
    ~Asn1BitString() { }
    Asn1BitString<MinSize, MaxSize>& operator=(const std::string & s) {
      BitString::operator=(s);
      return *this;
    }
  };

  class Asn1OctetString : public Asn1Object<Asn1OctetString, OctetString> {
  public:
    typedef std::string PrimitiveType;
  private:
    bool initialized_;
    int min_size_;
    int max_size_;
    PrimitiveType value_;
  public:
    Asn1Type getType() const { return OctetString; }
    virtual Asn1StringType getStringType() const { return Octet; }
    int getMinimumSize() const { return min_size_; }
    int getMaximumSize() const { return max_size_; }
    bool isInitialized() const { return initialized_; }

    Asn1OctetString(int min, int max) : min_size_(min), max_size_(max), initialized_(false), value_() {}
    Asn1OctetString(const Asn1OctetString& src) : min_size_(src.min_size_), max_size_(src.max_size_), initialized_(src.initialized_), value_(src.value_) {}
    const Asn1OctetString& operator=(Asn1OctetString src) {
      swap(src);
      return *this;
    }
    void swap(Asn1OctetString & other) {
      std::swap(min_size_, other.min_size_);
      std::swap(max_size_, other.max_size_);
      std::swap(initialized_, other.initialized_);
      std::swap(value_, other.value_);
    }
    const Asn1OctetString& operator=(const PrimitiveType & v) {
      initialized_ = true;
      value_ = v;
      return *this;
    }
    operator const PrimitiveType() const {
      return value_;
    }
    const PrimitiveType str() const {
      return value_;
    }
    const char* c_str() const {
      return value_.c_str();
    }
  };

  template <int Min, int Max = Min>
  class Asn1Int : public Asn1Integer {
  public:
    typedef int PrimitiveType;
    Asn1Int() : Asn1Integer(Min, Max) {}
    Asn1Int(int h) : Asn1Integer(Min, Max) { setValue(h); }
  };

  template <int MinSize=Unbounded, int MaxSize = MinSize>
  class Asn1String : public Asn1OctetString {
  public:
    Asn1String() : Asn1OctetString(MinSize, MaxSize) {}
    Asn1String(int h) : Asn1OctetString(MinSize, MaxSize) { setValue(h); }
    ~Asn1String() { }
    virtual Asn1StringType getStringType() const { return Octet; }
    Asn1String<MinSize, MaxSize>& operator=(const std::string & s) {
      Asn1OctetString::operator=(s);
      return *this;
    }
  };

  template <int MinSize=Unbounded, int MaxSize = MinSize>
  class Asn1IA5String : public Asn1OctetString {
  public:
    Asn1IA5String() : Asn1OctetString(MinSize, MaxSize) {}
    Asn1IA5String(const Asn1IA5String& rhs) : Asn1OctetString(rhs) { }
    ~Asn1IA5String() { }
    virtual Asn1StringType getStringType() const { return IA5; }
    Asn1IA5String<MinSize, MaxSize>& operator=(const std::string & s) {
      Asn1OctetString::operator=(s);
      return *this;
    }
  };
}


