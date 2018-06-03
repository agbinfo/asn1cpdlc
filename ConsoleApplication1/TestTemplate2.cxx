#include "Asn1.h"

#include <utility>
#include <memory>
#include <string>
#include <vector>
#include <iostream>

#pragma warning(disable : 4709)


namespace Cm {

  class AircraftFlightIdentification : public Asn1::IA5String<2, 8> {
  public:
    AircraftFlightIdentification& operator=(std::string s) {
      IA5String::operator=(s);
      return *this;
    }
  };
  class Airport : public Asn1::IA5String<4> {
  public:
    Airport& operator=(std::string s) {
      IA5String::operator=(s);
      return *this;
    }
  };
  typedef Asn1::Integer<0, 255> AEQualifier;
  typedef Asn1::Integer<1, 31> Day;
  class FacilityDesignation : public Asn1::IA5String<4, 8> {
  public:
    FacilityDesignation& operator=(std::string s) {
      IA5String::operator=(s);
      return *this;
    }
  };

  class CMAbortReason;
  class CMLogonRequest;
  class CMServerFacilityQueryRequest;
  class Response;
  typedef Response CMContactResponse;

  //class CMAircraftMessage : private Asn1::Choice {
  //public:
  //  struct cmLogonRequest { typedef CMLogonRequest ReturnType; static const int Index = 0; };
  //  struct cmContactResponse { typedef CMContactResponse ReturnType; static const int Index = 1; };
  //  struct cmAbortReason { typedef CMAbortReason ReturnType; static const int Index = 2; };
  //  // CM version 2 only
  //  struct cmServerFacilityQueryRequest { typedef CMServerFacilityQueryRequest ReturnType; static const int Index = 3; };

  //  template <typename T>
  //  typename T::ReturnType & operator[](const T&) { return Choice::as<T>(); }

  //  template <typename T>
  //  typename T::ReturnType & as() { return Choice::as<T>(); }

  //  template <typename T>
  //  typename const T::ReturnType & as() const { return Choice::as<T>(); }

  //  template <typename T>
  //  bool isa() const { return Choice::isa<T>(); }

  //  bool isassigned() const { return Choice::choice_ != nullptr; }
  //  size_t index() const { return Choice::choiceIndex_ - first_index; }
  //  size_t bitwidth() const { return Asn1::bitwidth(last_index - first_index); }

  //  static const size_t first_index = 0;
  //  static const size_t last_index = 3;
  //};

  class ShortTsap
  {
  public:
      // --the aRS contains the ICAO 24 bit aircraft address when the ShortTsap belongs to an aircraft;
      // -- or a ground address when the Short Tsap belongs to a ground system
      Asn1::Optional<Asn1::OctetString<3>> aRs;
      Asn1::OctetString<10, 11> locSysNselTsel;
  };

  class LongTsap
  {
  public:
    Asn1::OctetString<5> rDP;
    ShortTsap shortTsap;
  };

  typedef Asn1::Integer<1, 255> VersionNumber;

  class APAddress : private Asn1::Choice {
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

    bool isassigned() const { return Choice::choice_ != nullptr; }
    size_t index() const { return Choice::choiceIndex_ - first_index; }
    size_t bitwidth() const { return Asn1::bitwidth(last_index - first_index); }

    static const size_t first_index = 0;
    static const size_t last_index = 1;
  };

  Asn1::Serializer& operator << (Asn1::Serializer& os, const ShortTsap& v)
  {
    os.push_back(v.aRs.opt());
    if (v.aRs.opt()) os << v.aRs.get();
    os << v.locSysNselTsel;
    return os;
  }

  Asn1::Serializer& operator << (Asn1::Serializer& os, const LongTsap& v)
  {
    os << v.rDP;
    os << v.shortTsap;
    return os;
  }

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

  class AEQualifierVersionAddress {
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

  class Response {};
  class CMAbortReason {};
  class CMServerFacilityQueryRequest {};

  class CMAircraftMessage : private Asn1::Choice {
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

    bool isassigned() const { return Choice::choice_ != nullptr; }
    size_t index() const { return Choice::choiceIndex_ - first_index; }
    size_t bitwidth() const { return Asn1::bitwidth(last_index - first_index); }

    static const size_t first_index = 0;
    static const size_t last_index = 3;
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