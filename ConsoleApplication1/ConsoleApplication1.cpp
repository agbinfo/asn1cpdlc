// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include <iterator>
#include <vector>
#include "Asn1.h"
#include "Asn1PerSerializer.h"
#include "PMCPDLCMessageSetVersion1.h"
//#include "PMCPDLCAPDUsVersion1.h"

//#include "CMMessageSetVersion1.h"
//#include "CPDLCAPDUsVersion1.h"

namespace Cpdlc2 {
  using namespace Asn1;

  typedef Asn1Int<0, 23> Timehours;
  typedef Asn1Int<0, 59> Timeminutes;
  typedef Asn1Int<1, 31> Day;
  typedef Asn1Int<1, 12> Month;
  typedef Asn1Int<1996, 2095> Year;
  typedef Asn1Int<1, 255> VersionNumber;
  typedef Asn1Int<0, 255> AEQualifier;
  typedef Asn1IA5String<4, 8> FacilityDesignation;
  typedef Asn1IA5String<2, 8> AircraftFlightIdentification;
  typedef Asn1IA5String<5> Airport;
  typedef Asn1String<> ECPoint;

  class Date : public Asn1Sequence {
  public:
    Required<Year>::type year;
    Required<Month>::type month;
    Required<Day>::type day;
  public:
    Date() : Asn1Sequence(3) { //, hours(), minutes() {
      Asn1Sequence::Inserter it = back_inserter();
      *it++ = year;
      *it++ = month;
      *it++ = day;
    }
    // Asn1Type getType() const { return Sequence; }
    bool isInitialized() const { return false; }
  };

  class Time : public Asn1Sequence {
  public:
    Required<Timehours>::type hours;
    Required<Timeminutes>::type minutes;
  public:
    Time() : Asn1Sequence(2) { //, hours(), minutes() {
      Asn1Sequence::Inserter it = back_inserter();
      *it++ = hours;
      *it++ = minutes;
    }
    // Asn1Type getType() const { return Sequence; }
    bool isInitialized() const { return false; }
  };

  class DateTime : public Asn1Sequence {
  public:
    Required<Date>::type date;
    Required<Time>::type time;
  public:
    DateTime() : Asn1Sequence(2) { // date, time
      Asn1Sequence::Inserter it = back_inserter();
      *it++ = date;
      *it++ = time;
    }
  };

  class Response : public Asn1Enumerated {
  public:
    typedef enum {
      contactSuccess = 0,
      contactNotSuccessful = 1
    } EnumValues;
    Response() {
      enum_values.push_back(contactSuccess);
      enum_values.push_back(contactNotSuccessful);
    }
  };
  typedef Response CMLogonResponse;

  class CMAbortReason : public Asn1Enumerated {
  public:
    typedef enum {
      timer__expired = 0,
      undefined__error = 1,
      invalid__PDU = 2,
      protocol__error = 3,
      dialogue__acceptance__not__permitted = 4,
      dialogue__end__not__accepted = 5,
      communication__service__error = 6,
      communication__service__failure = 7,
      invalid__QOS__parameter = 8,
      expected__PDU__missing = 9
      // ...
    } EnumValues;
    CMAbortReason() {
      *this << timer__expired << undefined__error << invalid__PDU << protocol__error << dialogue__acceptance__not__permitted;
      *this << dialogue__end__not__accepted << communication__service__error << communication__service__failure;
      *this << invalid__QOS__parameter << expected__PDU__missing;
    }
    virtual bool extensible() const { return true; }
  };

  class DomainFlag : public Asn1Enumerated {
  public:
    typedef enum {
      keySharedInADM = 0,
      keyNotSharedInADM = 1,
      //...
    } EnumValues;
    DomainFlag() {
      *this << keySharedInADM << keyNotSharedInADM;
    }
    virtual bool extensible() const { return true; }
  };

  class ShortTsap : public Asn1Sequence {
  public:
    Optional<Asn1String<3> >::type aRS;
    // Extended
    Required<Asn1String<10, 11> >::type locSysNselTsel;
  public:
    ShortTsap() : Asn1Sequence(2) { // aRS, locSysNselTsel
      Asn1Sequence::Inserter it = back_inserter();
      *it++ = aRS;
      *it++ = locSysNselTsel;
    }
  };

  //LongTsap :: = SEQUENCE
  //{
  //	rDP                   OCTET STRING(SIZE(5)),
  //	shortTsap             ShortTsap
  //}
  class LongTsap : public Asn1Sequence {
  public:
    Required<Asn1String<5>>::type rDP;
    Required<ShortTsap>::type shortTsap;
    LongTsap() : Asn1Sequence(0) {}
    // TODO: add definition
  };

  class APAddress : public Asn1Choice {
  public:
    // typedef APAddress PrimitiveType;
    enum APAddress_Choices {
      longTsap_idx,
      shortTsap_idx,
      end_idx = shortTsap_idx
    };
    Choice<longTsap_idx, LongTsap>::type longTsap;
    Choice<shortTsap_idx, ShortTsap>::type shortTsap;
    APAddress() : Asn1Choice() {
      Inserter it = back_inserter();
      *it++ = longTsap;
      *it++ = shortTsap;
    }
    //Asn1Object& getElement() { return longTsap(); }
  };


  class CMContactResponse : public Asn1Sequence {
  public:
    Required<AircraftFlightIdentification>::type aircraftFlightIdentification;
    Required<LongTsap>::type cMLongTSAP;
    // TODO: Add optionals
    CMContactResponse() : Asn1Sequence(0) {
      Asn1Sequence::Inserter it = back_inserter();
      *it++ = aircraftFlightIdentification;
      *it++ = cMLongTSAP;
      // TODO: Add optionals
    }
  };

  class CMLogonRequest : public Asn1Sequence {
  public:
    Required<AircraftFlightIdentification>::type aircraftFlightIdentification;
    Required<LongTsap>::type cMLongTSAP;
    // TODO: Add optionals
    CMLogonRequest() : Asn1Sequence(2) {
      Asn1Sequence::Inserter it = back_inserter();
      *it++ = aircraftFlightIdentification;
      *it++ = cMLongTSAP;
      // TODO: Add optionals
    }
  };

  class AEQualifierVersionAddress : public Asn1Sequence {
  public:
    Required<AEQualifier>::type aeQualifier;
    Required<VersionNumber>::type apVersion;
    Required<APAddress>::type apAddress;
    AEQualifierVersionAddress() : Asn1Sequence(3) {
      Inserter it = back_inserter();
      *it++ = aeQualifier;
      *it++ = apVersion;
      *it++ = apAddress;
    }
    AEQualifierVersionAddress(const AEQualifierVersionAddress& rhs) : Asn1Sequence(3) {
      Inserter it = back_inserter();
      *it++ = aeQualifier;
      *it++ = apVersion;
      *it++ = apAddress;
      if (rhs.aeQualifier.isInitialized())
      { 
        aeQualifier = rhs.aeQualifier();
      }
      if (rhs.apVersion.isInitialized()) { apVersion = rhs.apVersion(); }
      // apAddress = rhs.apAddress;
    }
    void operator=(const AEQualifierVersionAddress& rhs) {
      if (rhs.aeQualifier.isInitialized())
      {
        aeQualifier = rhs.aeQualifier();
      }
      else {
        aeQualifier.destroy();
      }
      if (rhs.apVersion.isInitialized()) {
        apVersion = rhs.apVersion();
      }
      else {
        apVersion.destroy();
      }
      // apAddress = rhs.apAddress;
    }
  };

  class SecAir : public Asn1Sequence {
  public:
    Required<AEQualifierVersionAddress>::type applicationInformation;
    Optional<ECPoint>::type keyAgreementPublicKey;
    Optional<DomainFlag>::type domainFlag;
    SecAir() : Asn1Sequence(0) {}
    // TODO: add definition
  };

  class SecGnd : public Asn1Sequence {
    SecGnd() : Asn1Sequence(0) {}
    // TODO: add definition
  };

  class RequestedInfo : public Asn1Sequence {
  public:
    Required<FacilityDesignation>::type facilityDesignation;
    Optional<LongTsap>::type cMLongTSAP;
    Optional <SequenceOf< SecAir, 1, 256> >::type airInitiatedApplications;
    Optional <SequenceOf< SecGnd, 1, 256> >::type groundOnlyInitiatedApplications;
  };

  class CMServerFacilityQueryRequest : public Asn1Sequence {
  public:
    typedef SequenceOf<AEQualifierVersionAddress, 1, 256> SequenceOf_AEQualifierVersionAddress;
    Required<AircraftFlightIdentification>::type aircraftFlightIdentification;
    Required<LongTsap>::type cMLongTSAP;
    Optional<SequenceOf_AEQualifierVersionAddress>::type groundInitiatedApplications;
    // Optional<SequenceOf<AEQualifierVersion,1,256> >::type groundInitiatedApplications;
    // Required<SequenceOf<FacilityDesignation,1,8> >::type requestedFacilities;
    Optional<Airport>::type airportDeparture;
    Optional<Airport>::type airportDestination;
    Optional<DateTime>::type dateTimeDepartureETD;
    // ...
    CMServerFacilityQueryRequest() : Asn1Sequence(8) {
      Asn1Sequence::Inserter it = back_inserter();
      *it++ = aircraftFlightIdentification;
      *it++ = cMLongTSAP;
    }
  };

  class CMAircraftMessage : public Asn1Choice { // <CMAircraftMessage_Choices> {
  public:
    enum CMAircraftMessage_Choices {
      cmLogonRequest_idx,
      cmContactResponse_idx,
      cmAbortReason_idx,
      // ...
      end_idx = cmAbortReason_idx,
      cmServerFacilityQueryRequest_idx
    };
  public:
    Choice<cmLogonRequest_idx, CMLogonRequest>::type cmLogonRequest;
    Choice<cmContactResponse_idx, CMContactResponse>::type cmContactResponse;
    Choice<cmAbortReason_idx, CMAbortReason>::type cmAbortReason;
    Choice<cmServerFacilityQueryRequest_idx, CMServerFacilityQueryRequest>::type cmServerFacilityQueryRequest;
  public:
    CMAircraftMessage() : Asn1Choice() {
      Inserter it = back_inserter();
      *it++ = cmLogonRequest;
      *it++ = cmContactResponse;
      *it++ = cmAbortReason;
    }
    //		const Asn1Object& getElement() const { return cmLogonRequest(); }
    // Asn1Object& getElement() { return cmLogonRequest(); }
    // CMLogonRequest;
    // CMContactResponse
    // CMAbortReason
    // ...
    // CMServerFacilityQueryRequest
  };

  class B;

  class A : public Asn1Sequence {
  public:
    Optional<B>::type b;
    A() : Asn1Sequence(1) {
      Asn1Sequence::Inserter it = back_inserter();
      *it++ = b;
    }
  };

  class B : public Asn1Sequence {
  public:
    Required<A>::type a;
    B() : Asn1Sequence(1) {
      Asn1Sequence::Inserter it = back_inserter();
      *it++ = a;
    }
  };
}



int main2()
{
  Cpdlc2::CMAircraftMessage aircraftMessage;
  aircraftMessage.cmContactResponse;
  // Asn1String<5> rDP;
  aircraftMessage.cmLogonRequest().cMLongTSAP().rDP() = std::string("12345");
  aircraftMessage.cmLogonRequest().cMLongTSAP().rDP() = std::string("54321");
  Asn1::ChoiceElementBase* cb = &aircraftMessage.cmLogonRequest;
  Asn1::Asn1Object* a = cb->element();

  int maxSize = aircraftMessage.cmContactResponse().aircraftFlightIdentification().getMaximumSize();
  aircraftMessage.cmContactResponse().aircraftFlightIdentification() = std::string("AC344");
  std::string flight_id = aircraftMessage.cmContactResponse().aircraftFlightIdentification();
  Cpdlc2::Time time;
  Cpdlc2::ShortTsap tsap;
  time.hours = 1;
  time.minutes = 12;

  Cpdlc2::CMServerFacilityQueryRequest request;
  Cpdlc2::CMServerFacilityQueryRequest::SequenceOf_AEQualifierVersionAddress qualifiers;
  qualifiers.push_back(Cpdlc2::AEQualifierVersionAddress());
  qualifiers.push_back(Cpdlc2::AEQualifierVersionAddress());
  qualifiers["2"].aeQualifier() = 233;
  qualifiers[0].aeQualifier() = 234;
  qualifiers["2"] = qualifiers[0];
  request.groundInitiatedApplications = qualifiers;
  return 0;
}

