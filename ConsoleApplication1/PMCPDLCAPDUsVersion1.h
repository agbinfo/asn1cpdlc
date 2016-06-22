// PMCPDLCAPDUsVersion1.h
//
#pragma once

#include <iterator>
#include <vector>
#include "Asn1.h"

namespace PMCPDLCMessageSetVersion1 { class DateTimeGroup; }
namespace PMCPDLCMessageSetVersion1 { class AircraftFlightIdentification; }
namespace PMCPDLCMessageSetVersion1 { class AircraftAddress; }

namespace PMCPDLCAPDUsVersion1 {
typedef Asn1::Oid AlgorithmIdentifier;
class CPDLCMessage : public Asn1::BitString {
public:
  CPDLCMessage() : BitString() { }
  CPDLCMessage(int min, int max) : BitString(min, max) { }
  CPDLCMessage(const CPDLCMessage&src) : BitString(src) { }
  const CPDLCMessage& operator= (const CPDLCMessage&src) {
    return dynamic_cast<const CPDLCMessage&>(BitString::operator=(src));
  }
  const CPDLCMessage& operator= (const PrimitiveType v) {
    return dynamic_cast<const CPDLCMessage&>(BitString::operator=(v));
  }
};
class ProtectedGroundPDUs;
class ProtectedUplinkMessage;
class ATCForwardMessage;
class ForwardHeader;
class ForwardMessage;
class ATCForwardResponse;
class ProtectedAircraftPDUs;
class ProtectedStartDownMessage;
class ProtectedMode;
class ProtectedDownlinkMessage;
class PMCPDLCUserAbortReason;
class PMCPDLCProviderAbortReason;

class ProtectedGroundPDUs : public Asn1::Choice {
public:
  typedef ProtectedGroundPDUs PrimitiveType;
  enum {
    abortUser_idx = 0,
    abortProvider_idx = 1,
    startup_idx = 2,
    send_idx = 3,
    forward_idx = 4,
    forwardresponse_idx = 5
  };
  virtual bool extensible() const { return true; }
  Option<abortUser_idx, PMCPDLCUserAbortReason>::type abortUser();
  Option<abortProvider_idx, PMCPDLCProviderAbortReason>::type abortProvider();
  Option<startup_idx, ProtectedUplinkMessage>::type startup();
  Option<send_idx, ProtectedUplinkMessage>::type send();
  Option<forward_idx, ATCForwardMessage>::type forward();
  Option<forwardresponse_idx, ATCForwardResponse>::type forwardresponse();
  ProtectedGroundPDUs() : Choice() { }
};

class ProtectedUplinkMessage : public Asn1::Sequence {
public:
  Optional<AlgorithmIdentifier>::type algorithmIdentifier;
  Optional<CPDLCMessage>::type protectedMessage;
  Required<Asn1::BitString>::type integrityCheck;
  ProtectedUplinkMessage() : Sequence(3) {
    Sequence::Inserter it = back_inserter();
    it << algorithmIdentifier << protectedMessage << integrityCheck;
  }
};

class ATCForwardMessage : public Asn1::Sequence {
public:
  Required<ForwardHeader>::type forwardHeader;
  Required<ForwardMessage>::type forwardMessage;
  ATCForwardMessage() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << forwardHeader << forwardMessage;
  }
};

class ForwardHeader : public Asn1::Sequence {
public:
  Required<PMCPDLCMessageSetVersion1::DateTimeGroup>::type dateTime;
  Required<PMCPDLCMessageSetVersion1::AircraftFlightIdentification>::type aircraftID;
  Required<PMCPDLCMessageSetVersion1::AircraftAddress>::type aircraftAddress;
  ForwardHeader() : Sequence(3) {
    Sequence::Inserter it = back_inserter();
    it << dateTime << aircraftID << aircraftAddress;
  }
};

class ForwardMessage : public Asn1::Choice {
public:
  typedef ForwardMessage PrimitiveType;
  enum {
    upElementIDs_idx = 0,
    downElementIDs_idx = 1
  };
  virtual bool extensible() const { return false; }
  Option<upElementIDs_idx, Asn1::BitString>::type upElementIDs();
  Option<downElementIDs_idx, Asn1::BitString>::type downElementIDs();
  ForwardMessage() : Choice() { }
};

class ATCForwardResponse : public Asn1::Enumerated<0,2> {
public:
  virtual bool extensible() const { return true; }
  typedef enum {
    success = 0,
    service_not_supported = 1,
    version_not_equal = 2
  } EnumValues;
  ATCForwardResponse() : Enumerated() { }
  ATCForwardResponse(int value) : Enumerated(value) { }
};

class ProtectedAircraftPDUs : public Asn1::Choice {
public:
  typedef ProtectedAircraftPDUs PrimitiveType;
  enum {
    abortUser_idx = 0,
    abortProvider_idx = 1,
    startdown_idx = 2,
    send_idx = 3
  };
  virtual bool extensible() const { return true; }
  Option<abortUser_idx, PMCPDLCUserAbortReason>::type abortUser();
  Option<abortProvider_idx, PMCPDLCProviderAbortReason>::type abortProvider();
  Option<startdown_idx, ProtectedStartDownMessage>::type startdown();
  Option<send_idx, ProtectedDownlinkMessage>::type send();
  ProtectedAircraftPDUs() : Choice() { }
};

class ProtectedStartDownMessage : public Asn1::Sequence {
public:
  Required<ProtectedMode>::type mode;
  Required<ProtectedDownlinkMessage>::type startDownlinkMessage;
  ProtectedStartDownMessage() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << mode << startDownlinkMessage;
  }
};

//std::ostream& operator<<(std::ostream& s, Asn1::Sequence& /*m*/) {
//  return s;
//}
//
//std::ostream& operator<<(std::ostream& s, ProtectedStartDownMessage& m) {
//  s << static_cast<Asn1::Sequence&>(m);
//  return s;
//}

class ProtectedMode : public Asn1::Enumerated<0,1> {
public:
  virtual bool extensible() const { return false; }
  typedef enum {
    cpdlc = 0,
    dsc = 1
  } EnumValues;
  ProtectedMode() : Enumerated() { }
  ProtectedMode(int value) : Enumerated(value) { }
};

class ProtectedDownlinkMessage : public Asn1::Sequence {
public:
  Optional<AlgorithmIdentifier>::type algorithmIdentifier;
  Optional<CPDLCMessage>::type protectedMessage;
  Required<Asn1::BitString>::type integrityCheck;
  ProtectedDownlinkMessage() : Sequence(3) {
    Sequence::Inserter it = back_inserter();
    it << algorithmIdentifier << protectedMessage << integrityCheck;
  }
};

class PMCPDLCUserAbortReason : public Asn1::Enumerated<0,12> {
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
    time_out_of_synchronisation = 7,
    unknown_integrity_check = 8,
    validation_failure = 9,
    unable_to_decode_message = 10,
    invalid_pdu = 11,
    invalid_CPDLC_message = 12
  } EnumValues;
  PMCPDLCUserAbortReason() : Enumerated() { }
  PMCPDLCUserAbortReason(int value) : Enumerated(value) { }
};

class PMCPDLCProviderAbortReason : public Asn1::Enumerated<0,7> {
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
  PMCPDLCProviderAbortReason() : Enumerated() { }
  PMCPDLCProviderAbortReason(int value) : Enumerated(value) { }
};

}
