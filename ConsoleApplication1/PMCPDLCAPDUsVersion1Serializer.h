// PMCPDLCAPDUsVersion1Serializer.h
//
#pragma once

#include <iterator>
#include <vector>
#include "Asn1.h"

#include "PMCPDLCAPDUsVersion1.h"
#include "Asn1PerSerializer.h"

namespace PMCPDLCAPDUsVersion1 {
  
  std::ostream& operator<<(std::ostream& s, Asn1::Sequence& m) {
    return s;
  }

  std::ostream& operator<<(std::ostream& s, ProtectedStartDownMessage& m) {
    s << static_cast<Asn1::Sequence&>(m);
    return s;
  }

  PerSerializer::Serializer& operator<<(PerSerializer::Serializer& s, Asn1::Sequence m) {
    s.serialize(m);
  }

  //PerSerializer::Serializer& operator<<(PerSerializer::Serializer& s, ATCUplinkMessage& m) {
  //  s << static_cast<Asn1::Sequence&>(m);
  //  s << m.mode().value();
  //}

  PerSerializer::Serializer& operator<<(PerSerializer::Serializer& s, ProtectedStartDownMessage& m) {
    s << static_cast<Asn1::Sequence&>(m);
    s << m.mode().value();
  }
}
