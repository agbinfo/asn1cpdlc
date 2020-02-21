#include "Asn1.h"
#include "Asn1PerSerializer.h"

#include <utility>
#include <memory>
#include <string>
#include <vector>
#include <iostream>

#pragma warning(disable : 4709)


namespace Cm {
  using namespace Asn1;

  class AircraftFlightIdentification : public Asn1IA5String<2, 8> {
  public:
    AircraftFlightIdentification& operator=(std::string s) {
      Asn1IA5String::operator=(s);
      return *this;
    }
  };

  class Airport : public Asn1IA5String<4> {
  public:
    Airport& operator=(std::string s) {
      Asn1IA5String::operator=(s);
      return *this;
    }
  };

  typedef Asn1Int<0, 255> AEQualifier;
  typedef Asn1Int<1, 31> Day;

  class FacilityDesignation : public Asn1IA5String<4, 8> {
  public:
    FacilityDesignation& operator=(std::string s) {
      Asn1IA5String::operator=(s);
      return *this;
    }
  };

#ifdef SERIALIZER
  using namespace Serializers;

  PerSerializer& operator << (PerSerializer& os, const Asn1OctetString& v)
  {
    os << v.getMaximumSize() - v.getMinimumSize();
    if (v.aRs.opt()) os << v.aRs.get();
    os << v.locSysNselTsel;
    return os;
  }
#endif

  class CMAbortReason;
  class CMLogonRequest;
  class CMServerFacilityQueryRequest;

  class Response : public Enumerated<0, 1>
  {
  public:
    enum EnumValue {
      contactSuccess = 0,
      contactNotSuccessful = 1,
    };
  };

  typedef Response CMContactResponse;

  class CMAircraftMessage : public Choice {
  public:
    CMAircraftMessage()
      : cmLogonRequest(*this)
      , cmContactResponse(*this)
      , cmAbortReason(*this)
      , cmServerFacilityQueryRequest(*this)
    {}
    CMLogonRequest& cmLogonRequest_() { return Choice::select<0, CMLogonRequest>(); }
    Choice::Option<0, CMLogonRequest> cmLogonRequest;
    Choice::Option<1, CMContactResponse> cmContactResponse;
    Choice::Option<2, CMAbortReason> cmAbortReason;

    // Extension
    // CM version 2 only
    Choice::Option<3, CMServerFacilityQueryRequest> cmServerFacilityQueryRequest;
  };

  class CMGroundMessage : public Choice {
  public:
    CMGroundMessage()
      : cmAbortReason(*this)
    {}

    //struct cmLogonResponse { typedef CMLogonResponse ReturnType; static const int Index = 0; };
    //struct cmUpdate { typedef CMUpdate ReturnType; static const int Index = 0; };
    //struct cmContactRequest { typedef CMontactRequest ReturnType; static const int Index = 0; };
    //struct cmForwardRequest { typedef CMForwardRequest ReturnType; static const int Index = 0; };
    //struct cmAbortReason { typedef CMAbortReason ReturnType; static const int Index = 0; };
    Choice::Option<0, CMAbortReason> cmAbortReason;

    // ... -- All PDUs after extensibility are CM Version 2 only
    //struct cmServerFacilityQueryResponse { typedef CMServerFacilityQueryResponse ReturnType; static const int Index = 0; };
    //struct cmServerFacilityUpdate { typedef CMAbortReason ReturnType; static const int Index = 0; };
    //struct cmSecureLogonResponse { typedef CMSecureLogonResponse ReturnType; static const int Index = 0; };
    //struct cmSecureUpdate { typedef CMSecureUpdate ReturnType; static const int Index = 0; };
    //struct cmEnhancedForwardRequest { typedef CMEnhancedForwardRequest ReturnType; static const int Index = 0; };

    //template <typename T>
    //typename T::ReturnType & operator[](const T&) { return Choice::as<T>(); }

    //template <typename T>
    //typename T::ReturnType & as() { return Choice::as<T>(); }

    //template <typename T>
    //typename const T::ReturnType & as() const { return Choice::as<T>(); }

    //template <typename T>
    //bool isa() const { return Choice::isa<T>(); }

    //bool isassigned() const { return Choice::choice_ != nullptr; }
    //size_t index() const { return Choice::choiceIndex_ - first_index; }
    //size_t bitwidth() const { return Asn1::bitwidth(last_index - first_index); }

    //static const size_t first_index = 0;
    //static const size_t last_index = 10;
  };

  class ShortTsap : public Sequence
  {
  public:
    // --the aRS contains the ICAO 24 bit aircraft address when the ShortTsap belongs to an aircraft;
    // -- or a ground address when the Short Tsap belongs to a ground system
    Optional<Asn1String<3>> aRs;
    Asn1String<10, 11> locSysNselTsel;
  };

  class LongTsap : public Sequence
  {
  public:
    Asn1String<5> rDP;
    ShortTsap shortTsap;
  };

  typedef Asn1Int<1, 255> VersionNumber;

  class APAddress : public Choice {
  public:
    APAddress()
      : longTsap(*this)
      , shortTsap(*this)
    {}

    Choice::Option<0, LongTsap> longTsap;
    Choice::Option<1, ShortTsap> shortTsap;

    //struct longTsap { typedef LongTsap ReturnType; static const int Index = 0; };
    //struct shortTsap { typedef ShortTsap ReturnType; static const int Index = 1; };

    //template <typename T>
    //typename T::ReturnType & operator[](const T&) { return Choice::as<T>(); }

    //template <typename T>
    //typename T::ReturnType & as() { return Choice::as<T>(); }

    //template <typename T>
    //typename const T::ReturnType & as() const { return Choice::as<T>(); }

    //template <typename T>
    //bool isa() const { return Choice::isa<T>(); }

    //bool isassigned() const { return Choice::choice_ != nullptr; }
    //size_t index() const { return Choice::choiceIndex_ - first_index; }
    //size_t bitwidth() const { return Asn1::bitwidth(last_index - first_index); }

    //static const size_t first_index = 0;
    //static const size_t last_index = 1;
  };

#ifdef SERIALIZER
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
#endif

  class AEQualifierVersionAddress : public Sequence {
  public:
    AEQualifier aeQualifier;
    VersionNumber apVersion;
    APAddress apAddress;
  };

#ifdef SERIALIZER
  Asn1::Serializer& operator << (Asn1::Serializer& os, const AEQualifierVersionAddress& e)
  {
    os << e.aeQualifier;
    os << e.apVersion;
    os << e.apAddress;
    return os;
  }
#endif

  class AEQualifierVersion {};
  class DateTime {};

  class CMLogonRequest : public Sequence {
  public:
    AircraftFlightIdentification aircraftFlightIdentification;
    LongTsap cMLongTSAP;
    Optional< SequenceOf<AEQualifierVersionAddress, 1, 256> > groundInitiatedApplications;
    Optional< SequenceOf<AEQualifierVersion, 1, 256> > airOnlyInitiatedApplications;
    Optional< FacilityDesignation > facilityDesignation;
    Optional< Airport > airportDeparture;
    Optional< Airport > airportDestination;
    Optional< DateTime > dateTimeDepartureETD;
  };

#ifdef SERIALIZER
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
#endif

  class CMAbortReason : public Asn1Int<0,9>
  {
  public:
    enum EnumValue {
      timer_expired = 0,
      undefined_error = 1,
      invalid_PDU = 2,
      protocol_error = 3,
      dialogue_acceptance_not_permitted = 4,
      dialogue_end_not_accepted = 5,
      communication_service_error = 6,
      communication_service_failure = 7,
      invalid_QOS_parameter = 8,
      expected_PDU_missing = 9
    };
    CMAbortReason& operator=(CMAbortReason::EnumValue iValue) { Asn1Int::operator=(iValue); }
  };
  class CMServerFacilityQueryRequest {};

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

#ifdef SERIALIZER

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
#endif

  // class LevelType : public Choice {};
  typedef Asn1::Asn1String<1,5> LevelType;
    
  class Level : public Choice
  {
  public:
    Level() : singleLevel(*this), blockLevel(*this) {}
    Choice::Option<0, LevelType> singleLevel;
    Choice::Option<1, SequenceOf<LevelType,2>> blockLevel;
  };

  class ATCUplinkMsgElementId : public Choice
  {
  public:
    ATCUplinkMsgElementId(ATCUplinkMsgElementId&)
      : uM0NULL(*this)
      , uM1NULL(*this)
      , uM2NULL(*this)
      , uM3NULL(*this)
      , uM4NULL(*this)
      , uM5NULL(*this)
      , uM6Level(*this)
      // ...
      , uM237NULL(*this)
    {}
    ATCUplinkMsgElementId()
      : uM0NULL(*this)
      , uM1NULL(*this)
      , uM2NULL(*this)
      , uM3NULL(*this)
      , uM4NULL(*this)
      , uM5NULL(*this)
      , uM6Level(*this)
      // ...
      , uM237NULL(*this)
    {}
    Choice::Option<0, Asn1NULL> uM0NULL;
    Choice::Option<1, Asn1NULL> uM1NULL;
    Choice::Option<2, Asn1NULL> uM2NULL;
    Choice::Option<3, Asn1NULL> uM3NULL;
    Choice::Option<4, Asn1NULL> uM4NULL;
    Choice::Option<5, Asn1NULL> uM5NULL;
    Choice::Option<6, Level> uM6Level;
    // More root options up to 236

    // Extension
    Choice::Option<237, Asn1NULL> uM237NULL; // -- REQUEST AGAIN WITH NEXT UNIT
                                             // -- Urg(N)/Alr(L)/Resp(N)
  };

  Serializers::PerSerializer& operator<<(Serializers::PerSerializer& os, const ATCUplinkMsgElementId & e) {
    os << Serializers::ExtensibleOptionSelection<237>(e.selected());

    switch (e.selected()) {
    case 0:
      os << *(e.uM0NULL.get());  // optimize away: AsnNULL doesn't get encoded
      break;

    default: // no selection made; default to encoding defaulted first element
    {
      Asn1NULL e0;
      os << e0;
    }
    break;
    }
    return os;
  }

  class RouteClearance : public Sequence
  {
  public:
    Optional<Airport> airportDeparture;
    Optional<Airport> airportDestination;
    // Optional<Runway> runwayDeparture;
    // more
  };

  Serializers::PerSerializer& operator<<(Serializers::PerSerializer& os, const RouteClearance & e) {
    os << Serializers::PerOptionSerializer;
    os << e.airportDeparture;
    os << e.airportDestination;
    // os << e.runwayDeparture;
    // more
    os << Serializers::PerDefaultSerializer;
    os << e.airportDeparture;
    os << e.airportDestination;
    // os << e.runwayDeparture;
    // etc.
    return os;
  }

  class ATCUplinkMessageData : public Sequence
  {
  public: // Unnamed Sequence(s)
    class InnerSequence_1 : public Sequence /*extensible*/ {
    public:
      Optional<Asn1::SequenceOf<RouteClearance, 1, 2>> routeClearanceData;
      // ...
    };

  public:
    SequenceOf<ATCUplinkMsgElementId, 1, 5> elementIds;
    Optional<InnerSequence_1> constrainedData;
  };

  Serializers::PerSerializer& operator<<(Serializers::PerSerializer& os, const ATCUplinkMessageData::InnerSequence_1 & e) {
    os.stream << false; // not extended
    os << Serializers::PerOptionSerializer;
    os << e.routeClearanceData;
    os << Serializers::PerDefaultSerializer;
    os << e.routeClearanceData;
    return os;
  }

  Serializers::PerSerializer& operator<<(Serializers::PerSerializer& os, const ATCUplinkMessageData & e) {
    os << Serializers::PerOptionSerializer;
    os << e.constrainedData;
    os << Serializers::PerDefaultSerializer;
    os << e.elementIds;
    os << e.constrainedData;
    return os;
  }

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
  aircraftMessage.cmContactResponse = Cm::Response();
  aircraftMessage.cmLogonRequest = Cm::CMLogonRequest();
  aircraftMessage.cmLogonRequest->aircraftFlightIdentification = "AC767";
  aircraftMessage.cmLogonRequest->airportDeparture.value() = "CZVR";

  Cm::ATCUplinkMessageData messageData;
  *(messageData.elementIds["1"].uM6Level->singleLevel) = "L14";
  messageData.elementIds["5"].uM0NULL = Asn1::Asn1NULL();
//  (*messageData.constrainedData.element->routeClearanceData.element)[1].y = 6;
  messageData.constrainedData->routeClearanceData()[0].airportDeparture() = "CYUL";

  std::cout << std::string() << std::endl;
  std::string x = messageData.elementIds["1"].uM6Level->getType() == Asn1::Asn1Type::Null ? std::string("OK") : std::string("fail");
  std::string y = messageData.elementIds["5"].uM0NULL->getType() == Asn1::Asn1Type::Null ? std::string("OK") : std::string("fail");
  std::cout << x << "  " << y << std::endl;
  std::cout << aircraftMessage.cmLogonRequest->aircraftFlightIdentification.str() << std::endl;
  std::cout << aircraftMessage.cmLogonRequest->airportDeparture.value().str() << std::endl;
  std::cout << messageData.constrainedData->routeClearanceData.value()[0].airportDeparture.value().str() << std::endl;
  //std::cout << (*messageData.constrainedData.element->routeClearanceData.element)[1].y << std::endl;

  //Cm::AEQualifierVersionAddress aeQVA;

  //aircraftMessage[Cm::CMAircraftMessage::cmLogonRequest()].groundInitiatedApplications.get()[5] = aeQVA;

  Asn1::Serializers::PerSerializer s;
  s << aircraftMessage.cmLogonRequest->aircraftFlightIdentification;
  s << aircraftMessage.cmLogonRequest->airportDeparture.value();

#ifdef SERIALIZER
  Asn1::Serializer s;
  s << aircraftMessage;
#endif
}