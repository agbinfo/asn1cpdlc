// CMMessageSetVersion1.h : Cm.asn1
//
#pragma once

#include <iterator>
#include <vector>
#include "Asn1.h"
#include "Asn1PerSerializer.h"

#include "PMCPDLCMessageSetVersion1.h"
//namespace PMCPDLCMessageSetVersion1 { class ATCUplinkMessage; }
//namespace PMCPDLCMessageSetVersion1 { class ATCDownlinkMessage; }

namespace PMCPDLCAPDUsVersion1 { class ATCForwardMessage; }
namespace PMCPDLCAPDUsVersion1 { class ATCForwardResponse; }

namespace CMMessageSetVersion1 {
  typedef Asn1::Asn1Int<0, 23> Timehours;
  typedef Asn1::Asn1Int<0, 59> Timeminutes;
  typedef Asn1::Asn1Int<1, 31> Day;
  typedef Asn1::Asn1Int<1, 12> Month;
  typedef Asn1::Asn1Int<1996, 2095> Year;
  typedef Asn1::Asn1Int<1, 255> VersionNumber;
  typedef Asn1::Asn1Int<0, 255> AEQualifier;
  typedef Asn1::Asn1IA5String<4, 8> FacilityDesignation;
  typedef Asn1::Asn1IA5String<2, 8> AircraftFlightIdentification;
  typedef Asn1::Asn1IA5String<5> Airport;
  typedef Asn1::Asn1String<> ECPoint;

  class CMLogonRequest;
  class CMContactResponse;
  class CMAbortReason;
  class CMServerFacilityQueryRequest;

  class CMAircraftMessage : public Asn1::Choice {
  public:
    enum CMAircraftMessage_Choices {
      cmLogonRequest_idx = 1,
      cmContactResponse_idx,
      cmAbortReason_idx,
      // ...
      end_idx = cmAbortReason_idx,
      cmServerFacilityQueryRequest_idx
    };
    virtual bool extensible() const { return true; }
  public:
    Option<cmLogonRequest_idx, CMLogonRequest>::type cmLogonRequest() { return Option<cmLogonRequest_idx, CMLogonRequest>::getElement(*this); }
    Option<cmContactResponse_idx, CMContactResponse>::type cmContactResponse() { return Option<cmContactResponse_idx, CMContactResponse>::getElement(*this); }
    Option<cmAbortReason_idx, CMAbortReason>::type cmAbortReason() { return Option<cmAbortReason_idx, CMAbortReason>::getElement(*this); }
    Option<cmServerFacilityQueryRequest_idx, CMServerFacilityQueryRequest>::type cmServerFacilityQueryRequest() { return Option<cmServerFacilityQueryRequest_idx, CMServerFacilityQueryRequest>::getElement(*this); }
  public:
    CMAircraftMessage() : Choice(cmLogonRequest_idx, cmAbortReason_idx) {
    }
  };

  class CPDLCUserAbortReason;
  class CPDLCProviderAbortReason;

  class UplinkMessage;

  class GroundPDUs : public Asn1::Choice {
  public:
    typedef GroundPDUs PrimitiveType;
    enum GroundPDUs_Choices {
      abortUser_idx = 0,
      abortProvider_idx = 1,
      startup_idx = 2,
      send_idx =3 ,
      forward_idx = 4,
      forwardresponse_idx = 5,
      end_idx = forwardresponse_idx
    };
    virtual bool extensible() const { return true; }
    Option<abortUser_idx, CPDLCUserAbortReason>::type abortUser();
    Option<abortProvider_idx, CPDLCProviderAbortReason>::type abortProvider();
    Option<startup_idx, UplinkMessage>::type startup();
    Option<send_idx, PMCPDLCMessageSetVersion1::ATCUplinkMessage>::type send();
    Option<forward_idx, PMCPDLCAPDUsVersion1::ATCForwardMessage>::type forward();
    Option<forwardresponse_idx, PMCPDLCAPDUsVersion1::ATCForwardResponse>::type forwardresponse();

    GroundPDUs() : Asn1::Choice(abortUser_idx, forwardresponse_idx) { }
  };

  class UplinkMessage : public Asn1::Choice {
  public:
    typedef UplinkMessage PrimitiveType;
    enum UplinkMessage_Choices {
      noMessage_idx = 0,
      aTCUplinkMessage_idx = 1,
      end_idx = aTCUplinkMessage_idx
    };
    Option<noMessage_idx, Asn1::Asn1NULL>::type noMessage() { return Option<noMessage_idx, Asn1::Asn1NULL>::getElement(*this); }
    Option<aTCUplinkMessage_idx, PMCPDLCMessageSetVersion1::ATCUplinkMessage>::type aTCUplinkMessage() { return Option<aTCUplinkMessage_idx, PMCPDLCMessageSetVersion1::ATCUplinkMessage>::getElement(*this); }
    UplinkMessage() : Choice() {}
  };

  class StartDownMessage;

  class AircraftPDUs : public Asn1::Choice {
  public:
    typedef AircraftPDUs PrimitiveType;
    enum AircraftPDUs_Choices {
      abortUser_idx = 0,
      abortProvider_idx = 1,
      startdown_idx = 2,
      send_idx = 3,
      end_idx = send_idx
    };
    virtual bool extensible() const { return true; }
    Option<abortUser_idx, CPDLCUserAbortReason>::type abortUser() { return Option<abortUser_idx, CPDLCUserAbortReason>::getElement(*this); }
    Option<abortProvider_idx, CPDLCProviderAbortReason>::type abortProvider() { return Option<abortProvider_idx, CPDLCProviderAbortReason>::getElement(*this); }
    Option<startdown_idx, StartDownMessage>::type startdown() { return Option<startdown_idx, StartDownMessage>::getElement(*this); }
    Option<send_idx, PMCPDLCMessageSetVersion1::ATCDownlinkMessage>::type send() { return Option<send_idx, PMCPDLCMessageSetVersion1::ATCDownlinkMessage>::getElement(*this); }

    AircraftPDUs() : Choice() { }
  };

  class Mode : public Asn1::Enumerated<0,1> {
  public:
    typedef enum {
      cpdlc = 0,
      dsc = 1
      //...
    } EnumValues;
    Mode() : Enumerated(cpdlc) { }
    Mode(int value) : Enumerated(value) { }
  };

  class DownlinkMessage;

  class StartDownMessage : public Asn1::Sequence {
  public:
    Required<Mode>::type mode;  // DEFAULT cpdlc
    Required<DownlinkMessage>::type startDownlinkMessage;
    StartDownMessage() : Sequence(2) {
      Inserter it = back_inserter();
      mode = Mode::cpdlc;
      it << mode << startDownlinkMessage;
    }
  };

  class DownlinkMessage : public Asn1::Choice {
    typedef DownlinkMessage PrimitiveType;
    enum DownlinkMessage_Choices {
      noMessage_idx = 0,
      aTCDownlinkMessage_idx = 1,
      end_idx = aTCDownlinkMessage_idx
    };
    Option<noMessage_idx, Asn1::Asn1NULL>::type noMessage() { return Option<noMessage_idx, Asn1::Asn1NULL>::getElement(*this); }
    Option<aTCDownlinkMessage_idx, PMCPDLCMessageSetVersion1::ATCDownlinkMessage>::type aTCDownlinkMessage() {
      return Option<aTCDownlinkMessage_idx, PMCPDLCMessageSetVersion1::ATCDownlinkMessage>::getElement(*this);
    }
  };

  class CPDLCUserAbortReason : public Asn1::Enumerated<0,6> {
  public:
    virtual bool extensible() const { return true; }
    typedef enum {
      undefined = 0,
      no_message_identification_numbers_available = 1,
      duplicate_message_identification_numbers = 2,
      no_longer_next_data_authority = 3,
      current_data_authority_abort = 4,
      commanded_termination = 5,
      invalid_response = 6,
      //...
    } EnumValues;
    CPDLCUserAbortReason() : Enumerated(undefined) { }
    CPDLCUserAbortReason(int value) : Enumerated(value) { }
  };

  class CPDLCProviderAbortReason : public Asn1::Enumerated<0,7> {
  public:
    virtual bool extensible() const { return true; }
    typedef enum {
      timer_expired = 0,
      undefined_error = 1,		
      invalid_PDU = 2,
      protocol_error = 3,
      communication_service_error = 4,
      communication_service_failure = 5,
      invalid_QOS_parameter = 6,
      expected_PDU_missing = 7
      //...
    } EnumValues;
    CPDLCProviderAbortReason() : Enumerated(timer_expired) { }
    CPDLCProviderAbortReason(int value) : Enumerated(value) { }
  };

  class Date : public Asn1::Sequence {
  public:
    Required<Year>::type year;
    Required<Month>::type month;
    Required<Day>::type day;
  public:
    Date() : Asn1::Sequence(3) { //, hours(), minutes() {
      Asn1::Sequence::Inserter it = back_inserter();
      *it++ = year;
      *it++ = month;
      *it++ = day;
    }
    bool isInitialized() const { return false; }
  };

  class Time : public Asn1::Sequence {
  public:
    Required<Timehours>::type hours;
    Required<Timeminutes>::type minutes;
  public:
    Time() : Sequence(2) { //, hours(), minutes() {
      Sequence::Inserter it = back_inserter();
      *it++ = hours;
      *it++ = minutes;
    }
    bool isInitialized() const { return false; }
  };

  class DateTime : public Asn1::Sequence {
  public:
    Required<Date>::type date;
    Required<Time>::type time;
  public:
    DateTime() : Asn1::Sequence(2) { // date, time
      Asn1::Sequence::Inserter it = back_inserter();
      *it++ = date;
      *it++ = time;
    }
  };

  class Response : public Asn1::Enumerated<0,1> {
  public:
    typedef enum {
      contactSuccess = 0,
      contactNotSuccessful = 1
    } EnumValues;
    Response() : Enumerated(contactSuccess) { }
    Response(int value) : Enumerated(value) { }
  };
  typedef Response CMLogonResponse;

  class CMAbortReason : public Asn1::Enumerated<0,9> {
  public:
    virtual bool extensible() const { return true; }
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
    CMAbortReason() : Enumerated(timer__expired) { }
    CMAbortReason(int value) : Enumerated(value) { }
  };

  class DomainFlag : public Asn1::Enumerated<0,1> {
  public:
    virtual bool extensible() const { return true; }
    typedef enum {
      keySharedInADM = 0,
      keyNotSharedInADM = 1,
      //...
    } EnumValues;
    DomainFlag() : Enumerated(keySharedInADM) { }
    DomainFlag(int value) : Enumerated(value) { }
  };

  class ShortTsap : public Asn1::Sequence {
  public:
    // virtual bool extensible() const { return true; }
    Optional<Asn1::Asn1String<3> >::type aRS;
    // Extended
    Required<Asn1::Asn1String<10, 11> >::type locSysNselTsel;
  public:
    ShortTsap() : Sequence(2) { // aRS, locSysNselTsel
      Sequence::Inserter it = back_inserter();
      *it++ = aRS;
      *it++ = locSysNselTsel;
    }
  };

  //LongTsap :: = SEQUENCE
  //{
  //	rDP                   OCTET STRING(SIZE(5)),
  //	shortTsap             ShortTsap
  //}
  class LongTsap : public Asn1::Sequence {
  public:
    Required<Asn1::Asn1String<5>>::type rDP;
    Required<ShortTsap>::type shortTsap;
    LongTsap() : Sequence(0) {}
    // TODO: add definition
  };

  class APAddress : public Asn1::Choice {
  public:
    // typedef APAddress PrimitiveType;
    enum APAddress_Choices {
      longTsap_idx,
      shortTsap_idx,
      end_idx = shortTsap_idx
    };
    Option<longTsap_idx, LongTsap>::type longTsap() { return Option<longTsap_idx, LongTsap>::getElement(*this); }
    Option<shortTsap_idx, ShortTsap>::type shortTsap() { return Option<shortTsap_idx, ShortTsap>::getElement(*this); }
    APAddress() : Asn1::Choice() { }
  };


  class CMContactResponse : public Asn1::Sequence {
  public:
    Required<AircraftFlightIdentification>::type aircraftFlightIdentification;
    Required<LongTsap>::type cMLongTSAP;
    // TODO: Add optionals
    CMContactResponse() : Sequence(0) {
      Sequence::Inserter it = back_inserter();
      *it++ = aircraftFlightIdentification;
      *it++ = cMLongTSAP;
      // TODO: Add optionals
    }
  };

  class CMLogonRequest : public Asn1::Sequence {
  public:
    Required<AircraftFlightIdentification>::type aircraftFlightIdentification;
    Required<LongTsap>::type cMLongTSAP;
    // TODO: Add optionals
    CMLogonRequest() : Sequence(2) {
      Sequence::Inserter it = back_inserter();
      *it++ = aircraftFlightIdentification;
      *it++ = cMLongTSAP;
      // TODO: Add optionals
    }
  };

  class AEQualifierVersionAddress : public Asn1::Sequence {
  public:
    Required<AEQualifier>::type aeQualifier;
    Required<VersionNumber>::type apVersion;
    Required<APAddress>::type apAddress;
    AEQualifierVersionAddress() : Sequence(3) {
      Inserter it = back_inserter();
      *it++ = aeQualifier;
      *it++ = apVersion;
      *it++ = apAddress;
    }
    AEQualifierVersionAddress(const AEQualifierVersionAddress& rhs) : Sequence(3) {
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

  class SecAir : public Asn1::Sequence {
  public:
    Required<AEQualifierVersionAddress>::type applicationInformation;
    Optional<ECPoint>::type keyAgreementPublicKey;
    Optional<DomainFlag>::type domainFlag;
    SecAir() : Asn1::Sequence(0) {}
    // TODO: add definition
  };

  class SecGnd : public Asn1::Sequence {
    SecGnd() : Asn1::Sequence(0) {}
    // TODO: add definition
  };

  // some stuff

  class RequestedInfo : public Asn1::Sequence {
  public:
    Required<FacilityDesignation>::type facilityDesignation;
    Optional<LongTsap>::type cMLongTSAP;
    Optional <Asn1::SequenceOf< SecAir, 1, 256> >::type airInitiatedApplications;
    Optional <Asn1::SequenceOf< SecGnd, 1, 256> >::type groundOnlyInitiatedApplications;
  };

  class CMServerFacilityQueryRequest : public Asn1::Sequence {
  public:
    typedef Asn1::SequenceOf<AEQualifierVersionAddress, 1, 256> SequenceOf_AEQualifierVersionAddress;
    Required<AircraftFlightIdentification>::type aircraftFlightIdentification;
    Required<LongTsap>::type cMLongTSAP;
    Optional<SequenceOf_AEQualifierVersionAddress>::type groundInitiatedApplications;
    // Optional<SequenceOf<AEQualifierVersion,1,256> >::type groundInitiatedApplications;
    // Required<SequenceOf<FacilityDesignation,1,8> >::type requestedFacilities;
    Optional<Airport>::type airportDeparture;
    Optional<Airport>::type airportDestination;
    Optional<DateTime>::type dateTimeDepartureETD;
    // ...
    CMServerFacilityQueryRequest() : Asn1::Sequence(8) {
      Asn1::Sequence::Inserter it = back_inserter();
      *it++ = aircraftFlightIdentification;
      *it++ = cMLongTSAP;
    }
  };


}

