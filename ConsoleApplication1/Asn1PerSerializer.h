#pragma once

#include <iterator>
#include <string>
#include <sstream>
#include <vector>

#include "Asn1.h"

namespace PerSerializer {
  class Serializer {
  public:
    virtual void serialize(const Asn1::Asn1Object&) = 0;
    virtual void deserialize(Asn1::Asn1Object&) = 0;
  };

  // b7 | b6 | b5
  class BitStream {
  private:
    std::vector<uint8_t> data;

    uint8_t readpending = 0;
    unsigned nb_pending_bits = 0;
    unsigned read_byteindex = 0;
  public:
    uint64_t pop_front(unsigned bit_count) {
      if (bit_count <= 0 || bit_count > 64) return 0;
      uint64_t val = 0;
      int bits = bit_count;
      if (nb_pending_bits == 0) {
        readpending = data[read_byteindex];  // todo: check for overflow
        ++read_byteindex;
        nb_pending_bits = 8;
      }
      while (bits > 0)
      {
        val <<= 8;
        val |= readpending;
        bits -= nb_pending_bits;
      }
      if (bits < 0) { // read more than needed... give it back
        readpending = (val & 0xFF) >> (bits + 8);
        nb_pending_bits = -bits;
      }
      return val & ((1 << bit_count) - 1);
    }

  private:
    unsigned overflow = 0;
    unsigned overflow_count = 0;
  public:
    void push_back(uint64_t bits, unsigned bit_count) {
      if (bit_count > 0 && bit_count <= 64) {
        uint64_t mask = 1i64 << (bit_count - 1);
        while (mask) {
          *this << ((bits & mask) != 0);
          mask >>= 1;
        }
      }
    }
    void operator<<(uint8_t bits) {
      if (!overflow_count) {
        data.push_back(bits);
      }
      else {
        uint8_t to_store = ((bits & 0x7F) >> (7 - overflow_count));
        data.push_back(static_cast<unsigned char>(overflow | to_store));
        overflow = bits << (7 - overflow_count);
      }
    }
    void operator<<(bool bit) {
      if (overflow_count == 7) {
        data.push_back(bit ? static_cast<unsigned char>(overflow | 1) : static_cast<unsigned char>(overflow));
        overflow_count = 0;
      }
      else {
        if (bit) {
          overflow |= bit << (7 - overflow_count);
        }
        ++overflow_count;
      }
    }
  };

  // 18 - Encoding the sequence type
  /*
  18         Encoding the sequence type 
    NOTE – (Tutorial) A sequence type begins with a preamble which is a bit-map. If the sequence type has no extension marker, then
    the bit-map merely records the presence or absence of default and optional components in the type, encoded as a fixed length 
    bit-field. If the sequence type does have an extension marker, then the bit-map is preceded by a single bit that says whether 
    values of extension additions are actually present in the encoding. The preamble is encoded without any length determinant 
    provided it is less than 64K bits long, otherwise a length determinant is encoded to obtain fragmentation. The preamble is 
    followed by the fields that encode each of the components, taken in turn. If there are extension additions, then immediately 
    before the first one is encoded there is the encoding (as a normally small length) of a count of the number of extension 
    additions in the type being encoded, followed by a bit-map equal in length to this count which records the presence or absence
    of values of each extension addition. This is followed by the encodings of the extension additions as if each one was the value
    of an open type field. 

  18.1       If the sequence type has an extension marker, then a single bit shall first be added to the field-list in a bit-field 
  of length one. The bit shall be one if values of extension additions are present in this encoding, and zero otherwise. (This bit
  is called the "extension bit" in the following text.) If there is no extension marker, there shall be no extension bit added. 

  18.2       If the sequence type has "n" components in the extension root that are marked OPTIONAL or DEFAULT, then a single 
  bit-field with "n" bits shall be produced for addition to the field-list. The bits of the bit-field shall, taken in order, encode
  the presence or absence of an encoding of each optional or default component in the sequence type. A bit value of 1 shall encode
  the presence of the encoding of the component, and a bit value of 0 shall encode the absence of the encoding of the component.
  The leading bit in the preamble shall encode the presence or absence of the first optional or default component, and the trailing
  bit shall encode the presence or absence of the last optional or default component. 

  18.3       If "n" is less than 64K, the bit-field shall be appended to the field-list. If "n" is greater than or equal to 64K, 
  then the procedures of 10.9 shall be invoked to add this bit-field of "n" bits to the field-list, preceded by a length determinant 
  equal to "n" bits as a constrained whole number with "ub" and "lb" both set to "n". 

      NOTE – In this case, "ub" and "lb" will be ignored by the length procedures. These procedures are invoked here in order to 
      provide fragmentation of a large preamble. The situation is expected to arise only rarely. 

  18.4       The preamble shall be followed by the field-lists of each of the components of the sequence value which are present, 
  taken in turn. 

  18.5       For CANONICAL-PER, encodings of components marked DEFAULT shall always be absent if the value to be encoded is the 
  default value. For BASIC-PER, encodings of components marked DEFAULT shall always be absent if the value to be encoded is the 
  default value of a simple type (see 3.6.25), otherwise it is a sender's option whether or not to encode it. 

  18.6       This completes the encoding if the extension bit is absent or is zero. If the extension bit is present and set to 
  one, then the following procedures apply.
  
  18.7       Let the number of extension additions in the type being encoded be "n", then a bit-field with "n" bits shall be 
  produced for addition to the field-list. The bits of the bit-field shall, taken in order, encode the presence or absence of an 
  encoding of each extension addition in the type being encoded. A bit value of 1 shall encode the presence of the encoding of the 
  extension addition, and a bit value of 0 shall encode the absence of the encoding of the extension addition. The leading bit in 
  the bit-field shall encode the presence or absence of the first extension addition, and the trailing bit shall encode the 
  presence or absence of the last extension addition. 
  
      NOTE – If conformance is claimed to a particular version of a specification, then the value "n" is always equal to the 
      number of extension additions in that version.

  18.8       The procedures of 10.9 shall be invoked to add this bit-field of "n" bits to the field-list, preceded by a length 
  determinant equal to "n" as a normally small length.

      NOTE – "n" cannot be zero, as this procedure is only invoked if there is at least one extension addition being encoded. 

  18.9       This shall be followed by field-lists containing the encodings of each extension addition that is present, taken in 
  turn. Each extension addition that is a "ComponentType" (i.e., not an "ExtensionAdditionGroup") shall be encoded as if it 
  were the value of an open type field as specified in 10.2.1.  Each extension addition that is an "ExtensionAdditionGroup" shall
  be encoded as a sequence type as specified in 18.2 to 18.6,  which is then encoded as if it were the value of an open type 
  field as specified in 10.2.1. If all components values of the "ExtensionAdditionGroup" are missing then, the 
  "ExtensionAdditionGroup" shall be encoded as a missing extension addition (i.e., the corresponding bit in the bit-field 
  described in 18.7 shall be set to 0). 

      NOTE 1 – If an "ExtensionAdditionGroup" contains components marked OPTIONAL or DEFAULT, then the "ExtensionAdditionGroup" is
      prefixed with a bit-map that indicates the presence/absence of values for each component marked OPTIONAL or DEFAULT. 

      NOTE 2 – "RootComponentTypeList" components that are defined after the extension marker pair are encoded as if they were 
      defined immediately before the extension marker pair. 
  */
  class SequenceSerializer : Serializer {
    BitStream bs;
    virtual void serialize(const Asn1::Asn1Object& obj)
    {
      const Asn1::Sequence& seq = dynamic_cast<const Asn1::Sequence&>(obj);
      if (seq.extensible()) {
        bs << seq.extended();
      }
    }
  };

  class PerR : Serializer {
    BitStream bs;
    unsigned bitsForRange(uint64_t range) const {
      unsigned bits = 0;
      unsigned max_range = 1;
      while (range > max_range) {
        max_range >>= 1;
        bits++;
      }
    }
    virtual void serialize(const Asn1::Asn1Object& obj)
    {
      if (obj.getType() == Asn1::Boolean)
      {
        const Asn1::Asn1Boolean& asn1Bool = dynamic_cast<const Asn1::Asn1Boolean&>(obj);
        bs << asn1Bool.value();
      }
      else if (obj.getType() == Asn1::Integer)
      {
        // 10.3 - Encoding as a non-negative-binary-integer
        const Asn1::Asn1Integer& asn1Int = dynamic_cast<const Asn1::Asn1Integer&>(obj);
        unsigned range = asn1Int.getMaximum() - asn1Int.getMinimum() + 1;
        uint64_t value = asn1Int.getValue() - asn1Int.getMinimum();
        bs.push_back(value, bitsForRange(range));
      }
    }
    virtual void deserialize(Asn1::Asn1Object&) = 0;
  };

  BitStream& operator<<(BitStream& s, Asn1::Sequence& m) {
    if (m.extensible()) { s << m.extended(); }
    Asn1::Sequence::ContainerType_ elements = m.elements();
    for (Asn1::Sequence::ContainerType_::const_iterator it = elements.begin(); it != elements.end(); ++it) {
      // if element is OPTIONAL then write presence bit
      // if ((*it)->isOptional()) { s << (*it)->isPresent(); }
    }
    return s;
  }

};

