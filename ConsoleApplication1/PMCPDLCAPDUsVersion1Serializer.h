// PMCPDLCAPDUsVersion1Serializer.h
//
#pragma once

#include <iterator>
#include <vector>
#include "Asn1.h"

#include "PMCPDLCAPDUsVersion1.h"
#include "Asn1PerSerializer.h"

namespace Asn1 {
  namespace Serializer {

    //std::ostream& operator<<(std::ostream& s, Asn1::Sequence& m) {
    //  return s;
    //}

    //std::ostream& operator<<(std::ostream& s, ProtectedStartDownMessage& m) {
    //  s << static_cast<Asn1::Sequence&>(m);
    //  return s;
    //}
    //PerSerializer& operator<<(PerSerializer& s, Asn1::SequenceElementBase & m) {
    //  if (m.isOptional())
    //  {
    //    s << m.isInitialized();
    //  }
    //  return s;
    //}

    //PerSerializer& operator<<(PerSerializer& s, Asn1::Sequence m) {
    //  s.stream.push_back(1, 1);
    //  for (auto e = m.elements().begin(); e != m.elements().end(); ++e)
    //  {
    //    s << **e;
    //  }
    //}

    //PerSerializer::Serializer& operator<<(PerSerializer::Serializer& s, ATCUplinkMessage& m) {
    //  s << static_cast<Asn1::Sequence&>(m);
    //  s << m.mode().value();
    //}
    PerSerializer& operator<<(PerSerializer& s, PMCPDLCAPDUsVersion1::ProtectedDownlinkMessage& m) {
      // s << static_cast<Asn1::Sequence&>(m);
      s.stream.push_back(m.algorithmIdentifier.isInitialized(), 1);
      s.stream.push_back(m.protectedMessage.isInitialized(), 1);      
      if (m.algorithmIdentifier.isInitialized()) {
        //s << m.algorithmIdentifier();
      }
      if (m.protectedMessage().isInitialized()) {
        //s << m.protectedMessage();
      }
      return s;
    }

    PerSerializer& operator<<(PerSerializer& s, PMCPDLCAPDUsVersion1::ProtectedStartDownMessage& m) {
      // s << static_cast<Asn1::Sequence&>(m);
      if (m.elements()[0]);
      s.stream.push_back(m.mode().value());
      s << m.startDownlinkMessage();
      return s;
    }
  }
}