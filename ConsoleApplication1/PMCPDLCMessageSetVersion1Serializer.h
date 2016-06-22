// PMCPDLCMessageSetVersion1Serializer.h
//
#pragma once

#include <iterator>
#include <vector>
#include "Asn1.h"

#include "PMCPDLCMessageSetVersion1.h"
#include "Asn1PerSerializer.h"

namespace PMCPDLCMessageSetVersion1 {
  class Serializer : public PerSerializer::Serializer {
  public:
    PerSerializer::BitStream stream;
  public:
    virtual void serialize(const Asn1::Asn1Object& m) {
      if (m.getType() == Asn1::Asn1Type::EnumeratedType) {
        // nothing
      }
    }
    virtual void deserialize(Asn1::Asn1Object&) {

    }
  };


  // Move to Asn1PerSerializer.h
  Serializer& serializeSequence(Serializer& s, const Asn1::Sequence m) {
    // m.elements
    s.serialize(m);
    return s;
  }

  Serializer& operator<<(Serializer& s, const LogicalAck& m) {
    s.stream.push_back(m.value(), 1);
    return s;
  }

  Serializer& operator>>(Serializer& s, LogicalAck& m) {
    m = static_cast<int>(s.stream.pop_front(1));
    return s;
  }

  Serializer& operator<<(Serializer& s, const MsgIdentificationNumber& m) {
    s.stream.push_back(m.getValue(), 6);
    return s;
  }

  Serializer& operator>>(Serializer& s, MsgIdentificationNumber& m) {
    m.setValue(static_cast<Asn1::Asn1Integer::PrimitiveType>(s.stream.pop_front(6)));
    return s;
  }

  Serializer& operator<<(Serializer& s, const DateTimeGroup& /*m*/) {
    return s;
  }

  Serializer& operator>>(Serializer& s, DateTimeGroup& /*m*/) {
    return s;
  }

  Serializer& operator<<(Serializer& s, const ATCMessageHeader& m) {
    // Serialize Sequence header : no_marker, optionals[messageRefNumber], defaults[logicalAck=notRequired]
    s.stream.push_back(m.messageRefNumber.isInitialized() ? 1 : 0, 1);
    s.stream.push_back(m.logicalAck.isInitialized() ? 1 : 0, 1);   // Write a 1 if not using default.
    // Serialize Element
    s << m.messageIdNumber();
    if (m.messageRefNumber.isInitialized()) s << m.messageRefNumber();
    s << m.dateTime();
    if (m.logicalAck.isInitialized()) s << m.logicalAck();
    return s;
  }

  Serializer& operator >> (Serializer& s, ATCMessageHeader& m) {
    // no extension marker
    // optional/default
    // serializeSequence(s, no_marker, optionals[messageRefNumber], defaults[logicalAck=notRequired]);
    bool has_messageRefNumber = (s.stream.pop_front(1) == 1);  // OPTIONAL; 0 = not used, 1 = present
    bool has_logicalAck = (s.stream.pop_front(1) == 1);  // DEFAULT; 0 = default, 1 = present
    s >> m.messageIdNumber();
    if (has_messageRefNumber) s >> m.messageRefNumber();
    s >> m.dateTime();
    if (has_logicalAck) s >> m.logicalAck();
    return s;
  }

  Serializer& operator<<(Serializer& s, const ATCUplinkMessage& m) {
    s << m.header();
    return s;
  }

  Serializer& operator>>(Serializer& s, ATCUplinkMessage& m) {
    s >> m.header();
    return s;
  }

 
}
