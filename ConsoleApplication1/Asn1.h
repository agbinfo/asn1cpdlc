#pragma once

#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <sstream>

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

  template <typename Derived, Asn1Type BaseType>
  class Asn1Object {
  public:
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
  };

  template <int Min, int Max, typename PrimitiveType=int>
  class Enumerated : public Asn1Object<Enumerated, EnumeratedType> {
  private:
    PrimitiveType value_;
  public:
    int value() const { return value_; }
    Enumerated(const PrimitiveType& value) : value_(value) { }
    Enumerated() { }
    Enumerated& operator=(const PrimitiveType& value) { value_ = value; return *this; }
  };

  // The base class for an element contained within a CHOICE
  class OptionElementBase {
  protected:
    OptionElementBase(const OptionElementBase&) { }
    OptionElementBase() { }
  public:
    virtual Asn1Object& element() = 0;
    virtual int index() const = 0;
    virtual OptionElementBase* clone() const = 0;
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

  class Choice2
  {

  };


  // Defines an Asn1 Choice object.
  // Example:
  // class Mychoice : public Asn1::Choice<0,1> {
  //   Option<0, Asn1Type1>::type firstChoice() { return Option<0, Asn1Type1>::getElement(*this); }
  //   Option<1, Asn1Type2>::type secondChoice() { return Option<1, Asn1Type2>::getElement(*this); }
  //   ...
  //   MyChoice() { }
  // }
  class Choice : public Asn1Object {
  private:
    int min_;   // The minimum value used for encoding a non-extended choice selection
    int max_;   // The maximum value used for encoding a non-extended choice selection
    int selected_;
    bool extendable_;
    OptionElementBase* element_;  // The selected element or nullptr if none selected

  private:
    OptionElementBase& base_element() {
      if (element_) return *element_;
      throw std::exception("No Element Set");
    }

  public:
    virtual Asn1Type getType() const
    {
      return ChoiceType;
    }
    virtual bool extended() const { return element_ && (element_->index() < min_ || element_->index() > max_); }
    bool hasElement() const
    {
      return element_ != nullptr;
    }

    template <typename T> class ChoiceElement : public OptionElementBase {
    private:
      T& element_;
      const int index_;
    public:
      ChoiceElement(int index) : element_(* new T()), index_(index) {}
      ~ChoiceElement() { delete &element_; }
      T& element() { return element_; }
      virtual int index() const { return index_; }
      virtual ChoiceElement* clone() const {
        ChoiceElement* copy = new ChoiceElement(index_);
        copy->element_ = element_;
        return copy;
      }
    };

    template <int i, typename T> class Option {
    public:
      // Option<cmLogonRequest_idx, CmLogonRequest>::type cmLogonRequest() { return Option<cmLogonRequest_idx, CmLogonRequest>::getElement(*this); }
      typedef T& type;
      static typename type& getElement(Choice& parent) {
        if (parent.hasElement() && parent.selected_ == i) { return dynamic_cast<const type &>(parent.element_->element()); }

        delete parent.element_;
        ChoiceElement<T>* newElement = new ChoiceElement<T>(i);
        parent.element_ = newElement;
        parent.selected_ = i;
        return newElement->element();
      }
      static const type& getElement(const Choice& parent) {
        return dynamic_cast<const type &>(parent.element_->element());
      }

      // typename ChoiceElement<T> choice;
      // typedef Choice type;
      typedef T ElementType;
    private:
      // T* element;
      // int id;
    private:
      Option();
    };

    Choice() : min_(-1), max_(-1), selected_(-1), element_(nullptr) {}
    Choice(int min, int max) : min_(min), max_(max), selected_(-1), element_(nullptr) {}

    Choice(const Choice& rhs) : min_(rhs.min_), max_(rhs.max_), selected_(-1), element_(nullptr) {
      if (rhs.element_ != nullptr) {
        element_ = (*rhs.element_).clone();
        selected_ = rhs.selected_;
      }
    }

    virtual Asn1Object& getElement() {
      return base_element().element();
    }
    virtual bool isInitialized() const { 
      return element_ != nullptr; 
    }
  protected:
    //void extend() {
    //  max_ = elements_.size() - 1;
    //  extendable_ = 1;
    //}
  };

  // Wraps an object so that it can have an uninitialized state
  // T should be:
  // (a) CopyConstructible:
  //     T u = rv; // Sets the value of u equivalent to the value of rv
  //     T(rv);    // Creates an object equivalent to rv
  // The value of rv is unchanged
  //     
  template <class T>
  struct Ref {
    T* element;
    Ref() : element(0) {}
    Ref(const T& value) { element = new T(value); }
    Ref(const Ref& rhs) : element(nullptr) 
    {
      if (rhs.element)
      {
        element = new T(*rhs.element);
      }
    }
    ~Ref() { delete element; element = nullptr; }
    Ref& operator=(Ref rhs) 
    {
      swap(rhs);
      return *this;
    }
    void swap(Ref& other)
    {
      T* temp_element = element;
      element = other.element;
      other.element = temp_element;
    }
    void construct() {
      delete element;
      element = new T;
    }
    void construct(const T& value) {
      T* e = new T(value);
      delete element;
      element = e;
    }
    void destroy() {
      delete element;
      element = nullptr;
    }
    void reset(T* value) {
      delete element;
      element = value;
    }
    T& value() {
      if (!element) { throw std::exception("element not initialized"); }
      return *element;
    }
    const T& value() const {
      if (!element) { throw std::exception("element not initialized"); }
      return *element;
    }
    //T& operator*() {
    //  if (!element) { construct(); }
    //  return *element;
    //}
    //const T& operator*() const {
    //  if (!element) { throw std::exception("element not initialized"); }
    //  return *element;
    //}
    bool has_element() const { return element != nullptr; }
  };


  // A sequence is simply a grouping of objects
  // If a sequence contains the "..." marker then it is extensible.
  // The serializer encodes a sequence by encoding the extensible bit iff it is present then
  // by encoding the presence of optional+default elements then the elements themselves
  template<bool Extensible=false>
  class Sequence {
  };

  template<true>
  class Sequence {
  public:
    extended;
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
    static int min_index() const { return LowerBound; }
    static int max_index() const { return UpperBound; }
    static int field_size() const {
      return asn_field_size<UpperBound - LowerBound>::size;
      //int field_size = 0;
      //int delta = max_index_ - min_index_ + 1;
      //while (delta > 1) { ++field_size; delta = (delta+1) / 2; }
      //return field_size;
    }
  };

  template <typename T, int LowerBound, int UpperBound = LowerBound> class SequenceOf : public SequenceOfBase {
  public:
    typedef T PrimitiveType;
    typedef SequenceOf type;
    typedef typename std::vector<PrimitiveType>::const_iterator const_iterator;
    typedef typename std::vector<PrimitiveType>::iterator iterator;
  private:
    std::vector<T> elements_;
    typedef typename std::vector<typename T*>::const_iterator const_iterator_;
    typedef typename std::vector<typename T*>::iterator iterator_;
  public:
    SequenceOf() : SequenceOfBase(LowerBound, UpperBound) {}
    SequenceOf(const SequenceOf&rhs) : SequenceOfBase(rhs)
    {
      for (const_iterator it = rhs.elements_.begin(); it != rhs.elements_.end(); ++it)
      {
        elements_.push_back(new T(*(*it)));
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
      //SequenceOfBase::operator=(rhs);
      //min_index_ = rhs.min_index_;
      //max_index_ = rhs.max_index_;
      ////for (const_iterator it = rhs.elements_.begin(); it != rhs.elements_.end(); ++it)
      ////{
      ////  T* o = new T(*it);
      ////  elements_.push_back(o);
      ////}
      return *this;
    }
    T& operator[](unsigned pos) {
      if (pos<0 || pos>UpperBound - LowerBound) {
        throw std::out_of_range("Out of bounds");
      }
      while (pos >= elements_.size()) {
        elements_.push_back(new T());
      }
      //if (pos < elements_.size()) {
      //  delete elements_[pos];
      //  elements_[pos] = new T;
      //}
      return *elements_[pos];
    }
    T& operator[](std::string asn1_index) {
      std::stringstream ss(asn1_index);
      unsigned i;
      ss >> i;
      if (i<LowerBound || i>UpperBound) {
        throw std::out_of_range("Out of bounds");
      }
      unsigned pos = i - LowerBound;
      while (pos >= elements_.size()) {
        elements_.push_back(new T());
      }
      //if (pos < elements_.size()) {
      //  delete elements_[pos];
      //  elements_[pos] = new T;
      //}
      return *elements_[pos];
    }
    void assign(Iterator first, Iterator last)
    {
      elements_.assign(first, last);
    }
    void push_back(const T& elem) {
      T* value = new T(elem);
      elements_.push_back(value);
    }
    virtual void construct(unsigned /*i*/) {
      throw std::exception("Not implemented");
    }
    virtual void destroy(unsigned /*i*/) {
      throw std::exception("Not implemented");
    }
    virtual ~SequenceOf() {
      for (std::vector<T*>::iterator it = elements_.begin(); it != elements_.end(); ++it) {
        delete *it;
      }
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


  class Asn1Boolean : public Asn1Object {
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

  class Asn1Integer : public Asn1Object {
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

  class BitString : public Asn1Object {
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

  template <int MinSize = Unbounded, int MaxSize = MinSize> class Asn1BitString : public BitString {
  public:
    Asn1BitString() : BitString(MinSize, MaxSize) {}
    Asn1BitString(const Asn1BitString& rhs) : BitString(rhs) { }
    ~Asn1BitString() { }
    Asn1BitString<MinSize, MaxSize>& operator=(const std::string & s) {
      BitString::operator=(s);
      return *this;
    }
  };

  class Asn1OctetString : public Asn1Object {
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
  };

  template <int Min, int Max = Min> class Asn1Int : public Asn1Integer {
  public:
    typedef int PrimitiveType;
    Asn1Int() : Asn1Integer(Min, Max) {}
    Asn1Int(int h) : Asn1Integer(Min, Max) { setValue(h); }
  };

  template <int MinSize=Unbounded, int MaxSize = MinSize> class Asn1String : public Asn1OctetString {
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

  template <int MinSize=Unbounded, int MaxSize = MinSize> class Asn1IA5String : public Asn1OctetString {
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


