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

namespace Cm {

  class AircraftFlightIdentification : public Asn1::IA5String<2, 8>
  {
  public:
    AircraftFlightIdentification& operator=(std::string s) {
      IA5String::operator=(s);
      return *this;
    }
  };
  class Airport : public Asn1::IA5String<4> {};

  class ShortTsap
  {
  public:
      // --the aRS contains the ICAO 24 bit aircraft address when the ShortTsap belongs to an aircraft;
      // -- or a ground address when the Short Tsap belongs to a ground system
      Asn1::Optional<Asn1::OctetString<3>> aRs;
      Asn1::OctetString<10, 11> locSysNselTsel;
  };

  Asn1::Serializer& operator << (Asn1::Serializer& os, const ShortTsap& v)
  {
    os.push_back(v.aRs.opt());
    if (v.aRs.opt()) os << v.aRs.get();
    os << v.locSysNselTsel;
    return os;
  }

  class LongTsap
  {
  public:
    Asn1::OctetString<5> rDP;
    ShortTsap shortTsap;
  };

  Asn1::Serializer& operator << (Asn1::Serializer& os, const LongTsap& v)
  {
    os << v.rDP;
    os << v.shortTsap;
    return os;
  }

  typedef Asn1::Integer<0, 255> AEQualifier; // Should it be its own type?
  typedef Asn1::Integer<1, 255> VersionNumber;

  class APAddress : private Asn1::Choice
  {
  public:
    struct longTsap { typedef LongTsap ReturnType; static const int Index = 0; };
    struct shortTsap { typedef ShortTsap ReturnType; static const int Index = 1; };

    template <typename T>
    typename T::ReturnType & operator[](const T&) { return Choice::as<T>(); }

    template <typename T>
    typename T::ReturnType & as() { return Choice::as<T>(); }

    template <typename T>
    typename const T::ReturnType & as() const { return Choice::as<T>(); }

    template <typename T>
    bool isa() const { return Choice::isa<T>(); }

    static const size_t first_index = 0;
    static const size_t last_index = 1;
    bool isassigned() const { return Choice::choice_ != nullptr; }
    size_t index() const { return Choice::choiceIndex_ - first_index; }
    size_t bitwidth() const { return Asn1::bitwidth(last_index - first_index); }  // number of bits needed to encode choice
  };

  Asn1::Serializer& operator<<(Asn1::Serializer& s, const APAddress & v)
  {
    if (!v.isassigned() || v.index() > APAddress::shortTsap::Index)
    {
      throw Asn1::unassigned_exception();
    }
    else
    {
      s.push_back(v.index(), Asn1::bitwidth(APAddress::last_index - APAddress::first_index));
    }
    switch (v.index())
    {
    case APAddress::longTsap::Index:
      s << v.as<APAddress::longTsap>();
      break;
    case APAddress::shortTsap::Index:
      s << v.as<APAddress::shortTsap>();
      break;
    default:
      break;
    }
    return s;
  }

  class AEQualifierVersionAddress
  {
  public:
    AEQualifier aeQualifier;
    VersionNumber apVersion;
    APAddress apAddress;
  };

  Asn1::Serializer& operator << (Asn1::Serializer& os, const AEQualifierVersionAddress& e)
  {
    os << e.aeQualifier;
    os << e.apVersion;
    os << e.apAddress;
    return os;
  }

  class AEQualifierVersion {};
  class FacilityDesignation {};
  class DateTime {};

  class CMLogonRequest
  {
  public:
    AircraftFlightIdentification aircraftFlightIdentification;
    LongTsap cMLongTSAP;
    Asn1::Optional<Asn1::SequenceOf<1, 256, AEQualifierVersionAddress>> groundInitiatedApplications;
    Asn1::Optional<Asn1::SequenceOf<1, 256, AEQualifierVersion>> airOnlyInitiatedApplications;
    Asn1::Optional<FacilityDesignation> facilityDesignation;
    Asn1::Optional<Airport> airportDeparture;
    Asn1::Optional<Airport> airportDestination;
    Asn1::Optional<DateTime> dateTimeDepartureETD;
  };

  Asn1::Serializer& operator << (Asn1::Serializer& os, const CMLogonRequest& e)
  {
    os.push_back(e.groundInitiatedApplications.opt());
    os.push_back(e.airOnlyInitiatedApplications.opt());
    os.push_back(e.facilityDesignation.opt());
    os.push_back(e.airportDeparture.opt());
    os.push_back(e.airportDestination.opt());
    os.push_back(e.dateTimeDepartureETD.opt());
    os << e.aircraftFlightIdentification;
    os << e.cMLongTSAP;
    if (e.groundInitiatedApplications.opt())
    {
      os << e.groundInitiatedApplications.get();
    }
    return os;
  }

  class CMContactResponse {};
  class CMAbortReason {};
  class CMServerFacilityQueryRequest {};

  class CMAircraftMessage : private Asn1::Choice
  {
  public:
    struct cmLogonRequest { typedef CMLogonRequest ReturnType; static const int Index = 0; };
    struct cmContactResponse { typedef CMContactResponse ReturnType; static const int Index = 1; };
    struct cmAbortReason { typedef CMAbortReason ReturnType; static const int Index = 2; };
    // CM version 2 only
    struct cmServerFacilityQueryRequest { typedef CMServerFacilityQueryRequest ReturnType; static const int Index = 3; };

    template <typename T>
    typename T::ReturnType & operator[](const T&) { return Choice::as<T>(); }

    template <typename T>
    typename T::ReturnType & as() { return Choice::as<T>(); }

    template <typename T>
    typename const T::ReturnType & as() const { return Choice::as<T>(); }

    template <typename T>
    bool isa() const { return Choice::isa<T>(); }

    static const size_t first_index = 0;
    static const size_t last_index = 3;
    bool isassigned() const { return Choice::choice_ != nullptr; }
    size_t index() const { return Choice::choiceIndex_ - first_index; }
    size_t bitwidth() const { return Asn1::bitwidth(last_index - first_index); }  // number of bits needed to encode choice
  };

  Asn1::Serializer& operator << (Asn1::Serializer& os, const CMAircraftMessage& e)
  {
    if (e.index() >= CMAircraftMessage::cmServerFacilityQueryRequest::Index) // extended
    {
      os.push_back(true); // extended
      os.push_back(e.index(), Asn1::bitwidth(CMAircraftMessage::cmServerFacilityQueryRequest::Index - 0));
    }
    else
    {
      os.push_back(false); // not extended
      os.push_back(e.index(), Asn1::bitwidth(CMAircraftMessage::cmAbortReason::Index - CMAircraftMessage::first_index));
    }
    switch (e.index())
    {
    case CMAircraftMessage::cmLogonRequest::Index:
      os << e.as<CMAircraftMessage::cmLogonRequest>();
      break;
    case CMAircraftMessage::cmContactResponse::Index:
    case CMAircraftMessage::cmAbortReason::Index:
    case CMAircraftMessage::cmServerFacilityQueryRequest::Index:
      throw std::string("Not Implemented");
    default:
      break;
    }
    return os;
  }

  class CMGroundMessage : private Asn1::Choice
  {
  public:
    //struct cmLogonResponse { typedef CMLogonResponse ReturnType; static const int Index = 0; };
    //struct cmUpdate { typedef CMUpdate ReturnType; static const int Index = 0; };
    //struct cmContactRequest { typedef CMontactRequest ReturnType; static const int Index = 0; };
    //struct cmForwardRequest { typedef CMForwardRequest ReturnType; static const int Index = 0; };
    struct cmAbortReason { typedef CMAbortReason ReturnType; static const int Index = 0; };
    // ... -- All PDUs after extensibility are CM Version 2 only
    //struct cmServerFacilityQueryResponse { typedef CMServerFacilityQueryResponse ReturnType; static const int Index = 0; };
    //struct cmServerFacilityUpdate { typedef CMAbortReason ReturnType; static const int Index = 0; };
    //struct cmSecureLogonResponse { typedef CMSecureLogonResponse ReturnType; static const int Index = 0; };
    //struct cmSecureUpdate { typedef CMSecureUpdate ReturnType; static const int Index = 0; };
    //struct cmEnhancedForwardRequest { typedef CMEnhancedForwardRequest ReturnType; static const int Index = 0; };

    template <typename T>
    typename T::ReturnType & operator[](const T&) { return Choice::as<T>(); }

    template <typename T>
    typename T::ReturnType & as() { return Choice::as<T>(); }

    template <typename T>
    typename const T::ReturnType & as() const { return Choice::as<T>(); }

    template <typename T>
    bool isa() const { return Choice::isa<T>(); }

    static const size_t first_index = 0;
    static const size_t last_index = 3;
    bool isassigned() const { return Choice::choice_ != nullptr; }
    size_t index() const { return Choice::choiceIndex_ - first_index; }
    size_t bitwidth() const { return Asn1::bitwidth(last_index - first_index); }  // number of bits needed to encode choice
  };

  class Level : private Asn1::Choice
  {
  public:
  };

  class ATCUplinkMsgElementId
  {
  public:
    Asn1::AsnNull uM0NULL;
    Asn1::AsnNull uM1NULL;
    Asn1::AsnNull uM2NULL;
    Asn1::AsnNull uM3NULL;
    Asn1::AsnNull uM4NULL;
    Asn1::AsnNull uM5NULL;
    Level uM6Level;
  };

  class RouteClearance
  {
  public:
    int y;
  };

  class ATCUplinkMessageData
  {
  public:
    Asn1::SequenceOf<1, 5, ATCUplinkMsgElementId> elementIds;
    // Optional Sequence
    class OptionalSequence_ {
    public:
      Asn1::Optional<Asn1::SequenceOf<1, 2, RouteClearance>> routeClearanceData;
      // ...
    };
    Asn1::Optional<OptionalSequence_> constrainedData;
  };


  // Serialize
  //Asn1::Serializer& operator << (Asn1::Serializer& os, AircraftFlightIdentification& e)
  //{
  //  os << static_cast<std::string>(e);
  //  return is;
  //}

}

//template <int Choice>
//class DownlinkMessage2 : public Inner::DownlinkMessage2
//{
//
//};

void main()
{
  //Cm::CMLogonRequest r;
  //std::cin >> r;

  Cm::CMAircraftMessage aircraftMessage;
  aircraftMessage[Cm::CMAircraftMessage::cmContactResponse()];
  aircraftMessage[Cm::CMAircraftMessage::cmLogonRequest()];
  aircraftMessage.as<Cm::CMAircraftMessage::cmLogonRequest>();
  aircraftMessage.as<decltype(aircraftMessage)::cmLogonRequest>().aircraftFlightIdentification = "AC764";

  Cm::ATCUplinkMessageData messageData;
  messageData.elementIds[5].uM0NULL.x = 5;
  messageData.constrainedData.get().routeClearanceData.get()[1].y = 6;

  std::cout << messageData.elementIds[5].uM0NULL.x << std::endl;
  std::cout << messageData.constrainedData.get().routeClearanceData.get()[1].y << std::endl;

  Cm::AEQualifierVersionAddress aeQVA;

  aircraftMessage[Cm::CMAircraftMessage::cmLogonRequest()].groundInitiatedApplications.get()[5] = aeQVA;

  Asn1::Serializer s;
  s << aircraftMessage;
}