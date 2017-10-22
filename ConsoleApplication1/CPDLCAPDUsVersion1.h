// CPDLCAPDUsVersion1.h
//
#pragma once

#include <iterator>
#include <vector>
#include "Asn1.h"

namespace PMCPDLCMessageSetVersion1 { class ATCUplinkMessage; }
namespace PMCPDLCMessageSetVersion1 { class ATCDownlinkMessage; }
namespace PMCPDLCAPDUsVersion1 { class ATCForwardMessage; }
namespace PMCPDLCAPDUsVersion1 { class ATCForwardResponse; }

namespace CPDLCAPDUsVersion1 {
class GroundPDUs;
class UplinkMessage;
class AircraftPDUs;
class StartDownMessage;
class Mode;
class DownlinkMessage;
class CPDLCUserAbortReason;
class CPDLCProviderAbortReason;

class GroundPDUs : public Asn1::Choice {
public:
  typedef GroundPDUs PrimitiveType;
  enum {
    abortUser_idx = 0,
    abortProvider_idx = 1,
    startup_idx = 2,
    send_idx = 3,
    forward_idx = 4,
    forwardresponse_idx = 5
  };
  virtual bool extensible() const { return true; }
  union
  {
    CPDLCProviderAbortReason* abortUser_;
    CPDLCProviderAbortReason* abortProvider_;
    UplinkMessage* startup_;
    PMCPDLCMessageSetVersion1::ATCUplinkMessage* send_;
    PMCPDLCAPDUsVersion1::ATCForwardMessage* forward_;
    PMCPDLCAPDUsVersion1::ATCForwardResponse* forwardresponse_;

  } options_;
  Option<abortUser_idx, CPDLCUserAbortReason>::type abortUser();
  Option<abortProvider_idx, CPDLCProviderAbortReason>::type abortProvider();
  Option<startup_idx, UplinkMessage>::type startup();
  Option<send_idx, PMCPDLCMessageSetVersion1::ATCUplinkMessage>::type send();
  Option<forward_idx, PMCPDLCAPDUsVersion1::ATCForwardMessage>::type forward();
  Option<forwardresponse_idx, PMCPDLCAPDUsVersion1::ATCForwardResponse>::type forwardresponse();
  GroundPDUs() : Choice() { }
};

class UplinkMessage : public Asn1::Choice {
public:
  typedef UplinkMessage PrimitiveType;
  enum {
    noMessage_idx = 0,
    aTCUplinkMessage_idx = 1
  };
  virtual bool extensible() const { return false; }
  Option<noMessage_idx, Asn1::Asn1NULL>::type noMessage();
  Option<aTCUplinkMessage_idx, PMCPDLCMessageSetVersion1::ATCUplinkMessage>::type aTCUplinkMessage();
  UplinkMessage() : Choice() { }
};

class AircraftPDUs : public Asn1::Choice {
public:
  typedef AircraftPDUs PrimitiveType;
  enum {
    abortUser_idx = 0,
    abortProvider_idx = 1,
    startdown_idx = 2,
    send_idx = 3
  };
  virtual bool extensible() const { return true; }
  Option<abortUser_idx, CPDLCUserAbortReason>::type abortUser();
  Option<abortProvider_idx, CPDLCProviderAbortReason>::type abortProvider();
  Option<startdown_idx, StartDownMessage>::type startdown();
  Option<send_idx, PMCPDLCMessageSetVersion1::ATCDownlinkMessage>::type send();
  AircraftPDUs() : Choice() { }
};

class StartDownMessage : public Asn1::Sequence {
public:
  Required<Mode>::type mode;
  Required<DownlinkMessage>::type startDownlinkMessage;
  StartDownMessage() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << mode << startDownlinkMessage;
  }
};

class Mode : public Asn1::Enumerated<0,1> {
public:
  virtual bool extensible() const { return false; }
  typedef enum {
    cpdlc = 0,
    dsc = 1
  } EnumValues;
  Mode() : Enumerated() { }
  Mode(int value) : Enumerated(value) { }
};

class DownlinkMessage : public Asn1::Choice {
public:
  typedef DownlinkMessage PrimitiveType;
  enum {
    noMessage_idx = 0,
    aTCDownlinkMessage_idx = 1
  };
  virtual bool extensible() const { return false; }
  Option<noMessage_idx, Asn1::Asn1NULL>::type noMessage();
  Option<aTCDownlinkMessage_idx, PMCPDLCMessageSetVersion1::ATCDownlinkMessage>::type aTCDownlinkMessage();
  DownlinkMessage() : Choice() { }
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
    invalid_response = 6
  } EnumValues;
  CPDLCUserAbortReason() : Enumerated() { }
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
  } EnumValues;
  CPDLCProviderAbortReason() : Enumerated() { }
  CPDLCProviderAbortReason(int value) : Enumerated(value) { }
};

}
