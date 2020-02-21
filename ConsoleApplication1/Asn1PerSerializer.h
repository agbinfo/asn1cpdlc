#pragma once

#include <iterator>
#include <string>
#include <sstream>
#include <vector>

#include "Asn1.h"

namespace Asn1 {

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
      if (bit) {
        overflow |= bit << (7 - overflow_count);
      }
      overflow_count = (overflow_count+1) & 8;
      if (overflow_count == 0) {
        data.push_back(bit ? static_cast<unsigned char>(overflow | 1) : static_cast<unsigned char>(overflow));
        overflow = 0;
      }
    }
  };

  class Serializer {
  public:
    enum class EncodingType
    {
      Default,
      Header,
      Options,
      IA5String
    };
    BitStream stream;
  };


  namespace Serializers {
    template <int Range>
    struct SizeField {
      static const int size = 1 + SizeField<Range/2>::size;
    };

    template <>
    struct SizeField<1> {
      static const int size = 1;
    };

    class PerSerializer : public Serializer {
    public:
      PerSerializer() : encoding_type_(EncodingType::Default) {}
      void setEncodingType(EncodingType type) { encoding_type_ = type; }
      EncodingType getEncodingType() const { return encoding_type_; }

    private:
      EncodingType encoding_type_;
    };

    typedef PerSerializer& Manip(PerSerializer&);

    PerSerializer& PerDefaultSerializer(PerSerializer& s)
    {
      s.setEncodingType(Serializer::EncodingType::Default);
      return s;
    };

    PerSerializer& PerOptionSerializer(PerSerializer& s)
    {
      s.setEncodingType(Serializer::EncodingType::Options);
      return s;
    };

    PerSerializer& IA5String(PerSerializer& s)
    {
      s.setEncodingType(Serializer::EncodingType::IA5String);
      return s;
    };

    PerSerializer& operator<<(PerSerializer& s, Manip& manip)
    {
      return manip(s);
    }

    /* 10.6 - Encoding of a normally small non-negative whole number
    */
    struct NormallySmallInt {
      NormallySmallInt(int v) : value_(v) {}
      int value() const { return value_; }
    private:
      int value_;
    };

    template <bool Extensible, int RootSize>
    struct OptionSelectionInfo {
      OptionSelectionInfo(int selected)
        : selected_(selected)
      {}
      int selected_;
    };

    template <int RootSize>
    OptionSelectionInfo<true, RootSize> ExtensibleOptionSelection(int selected) {
      return OptionSelectionInfo<true, RootSize>(selected);
    }

    Serializers::PerSerializer& operator<<(Serializers::PerSerializer& os, NormallySmallInt e) {
      if (e.value() < 64)
      {
        os.stream << false; // value less than 64
        os.stream.push_back(e.value(), 6);
      }
      else {
        // not supported - throw?
      }
      return os;
    }

    template <bool Extensible, int RootSize>
    Serializers::PerSerializer& operator<<(Serializers::PerSerializer& os, OptionSelectionInfo<Extensible,RootSize> e) {
      if (!Extensible) {
        os.stream.push_back(std::max<int>(0, e.selected_), Asn1::Serializers::SizeField<RootSize>::size);
        return os;
      }

      bool extended = e.selected_ >= RootSize;
      os.stream << extended;
      if (!extended) {
        os.stream.push_back(std::max<int>(0, e.selected_), Asn1::Serializers::SizeField<RootSize>::size);
      }
      else {
        // 22.8 encode normally-small-non-negative whole number of extension (10.6)
        os << Serializers::NormallySmallInt(e.selected_ - RootSize);
      }
      return os;
    }

    template <typename T>
    Serializers::PerSerializer& operator<<(Serializers::PerSerializer& os, const Asn1::Optional<T> & optional) {
      if (os.getEncodingType() == Serializer::EncodingType::Options) {
        os.stream << optional.hasElement();
      }
      else if (optional.hasElement()) {
        os << optional.value();
      }
      return os;
    }

    /*
    12 - Encoding the integer type
      NOTE 1 –(Tutorial ALIGNED variant) Ranges which allow the encoding of all values into one octet or less go into a minimum-sized
              bit-field with no length count. Ranges which allow encoding of all values into two octets go into two octets in an octet-aligned
              bit-field with no length count. Otherwise, the value is encoded into the minimum number of octets (using
              non-negative-binary-integer or 2's-complement-binary-integer encoding as appropriate) and a length determinant is added.
              In this case, if the integer value can be encoded in less than 127 octets (as an offset from any lower bound that might be
              determined), and there is no finite upper and lower bound, there is a one-octet length determinant, else the length is
              encoded in the fewest number of bits needed. Other cases are not of any practical interest, but are specified for
              completeness.
      NOTE 2 – (Tutorial UNALIGNED variant) Constrained integers are encoded in the fewest number of bits necessary to represent the
              range regardless of its size. Unconstrained integers are encoded as in Note 1.

    12.1 If an extension marker is present in the constraint specification of the integer type, then a single bit shall be added 
         to the field-list in a bit-field of length one. The bit shall be set to 1 if the value to be encoded is not within the range of
         the extension root, and zero otherwise. In the former case, the value shall be added to the field-list as an unconstrained
         integer value, as specified in 12.2.4 to 12.2.6, completing this procedure. In the latter case, the value shall be encoded
         as if the extension marker is not present.
    12.2 If an extension marker is not present in the constraint specification of the integer type, then the following applies.
      12.2.1 If PER-visible constraints restrict the integer value to a single value, then there shall be no addition to the field-list,
             completing these procedures.
      12.2.2 If PER-visible constraints restrict the integer value to be a constrained whole number, then it shall be converted to a
             field according to the procedures of 10.5 (encoding of a constrained whole number), and the procedures of 12.2.5 to 12.2.6
             shall then be applied.
      12.2.3 If PER-visible constraints restrict the integer value to be a semi-constrained whole number, then it shall be converted to
             a field according to the procedures of 10.7 (encoding of a semi-constrained whole number), and the procedures of 12.2.6 shall
             then be applied.
      12.2.4 If PER-visible constraints do not restrict the integer to be either a constrained or a semi-constrained whole number,
             then it shall be converted to a field according to the procedures of 10.8 (encoding of an unconstrained whole number),
             and the procedures of 12.2.6 shall then be applied.
      12.2.5 If the procedures invoked to encode the integer value into a field did not produce the indefinite length case (see 10.5.7.4
             and 10.8.2), then that field shall be appended to the field-list completing these procedures. 12.2.6Otherwise, (the
             indefinite length case) the procedures of 10.9 shall be invoked to append the field to the field-list preceded by one of
             the following:
         a) A constrained length determinant "len" (as determined by 10.5.7.4) if PER-visible constraints restrict the type with finite
            upper and lower bounds and, if the type is extensible, the value lies within the range of the extension root. The 
            lower bound "lb" used in the length determinant shall be 1, and the upper bound "ub" shall be the count of the number
            of octets required to hold the range of the integer value.
            NOTE – The encoding of the value "foo INTEGER (256..1234567) ::= 256" would thus be encoded as 00xxxxxx00000000, where
                   each 'x' represents a zero pad bit that may or may not be present depending on where within the octet the length
                   occurs (e.g., the encoding is 00 xxxxxx 00000000 if the length starts on an octet boundary, and 00 00000000 if it
                   starts with the two least signigicant bits (bits 2 and 1) of an octet).
         b) An unconstrained length determinant equal to "len" (as determined by 10.7 and 10.8) if PER-visible constraints do not
            restrict the type with finite upper and lower bounds, or if the type is extensible and the value does not lie within the
            range of the extension root.
    */

    template <int Min, int Max>
    PerSerializer& operator<<(PerSerializer& os, Asn1::Asn1Int<Min,Max> & asnInt) {
      // Unaligned Interger Encoding
      os.stream.push_back(asnInt.getValue() - Min, SizeField<Max - Min>::size);
      return os;
    }

    template <int Constant>
    PerSerializer& operator<<(PerSerializer& os, Asn1::Asn1Int<Constant, Constant> &) {
      return os;
    }

    /*
    16 - Encoding the octetstring type
      NOTE – Octet strings of fixed length less than or equal to two octets are not octet-aligned. All other octet strings are octet-aligned
      in the ALIGNED variant.  Fixed length octet strings encode with no length octets if they are shorter than 64K. For unconstrained
      octet strings the length is explicitly encoded (with fragmentation if necessary).

    16.1 PER-visible constraints can only constrain the length of the octetstring.
    16.2 Let the maximum number of octets in the octetstring (as determined by PER-visible constraints on the length)
         be "ub" and the minimum number of octets be "lb". If there is no finite maximum we say that "ub" is unset. If there is no
         constraint on the minimum then "lb" has the value zero. Let the length of the actual octetstring value to be encoded be
         "n" octets.
    16.3 If there is a PER-visible size constraint and an extension marker is present in it, a single bit shall be added to
         the field-list in a bit-field of length one. The bit shall be set to 1 if the length of this encoding is not within the range of
         the extension root, and zero otherwise. In the former case 16.8 shall be invoked to add the length as a semi-constrained
         whole number to the field-list, followed by the octetstring value. In the latter case the length and value shall be encoded
         as if the extension marker is not present.
    16.4 If an extension marker is not present in the constraint specification of the octetstring type, then 16.5 to 16.8 apply.
    16.5 If  the  octetstring  is  constrained  to  be  of  zero  length  ("ub"  equals  zero),  then  it  shall  not  be  encoded  (no
         additions to the field-list), completing the procedures of this clause.
    16.6 If all values of the octetstring are constrained to be of the same length ("ub" equals "lb") and that length is less
         than  or  equal  to  two  octets,  the  octetstring  shall  be  placed  in  a  bit-field  with  a  number  of  bits  equal  to
         the  constrained  length  "ub"  multiplied  by  eight  which  shall  be  appended  to  the  field-list  with  no  length
         determinant,  completing  the  procedures of this clause.
    16.7 If  all  values  of  the  octetstring  are  constrained  to  be  of  the  same  length  ("ub"  equals  "lb")  and  that  length
         is  greater  than  two  octets  but  less  than  64K,  then  the  octetstring  shall  be  placed  in  a  bit-field
         (octet-aligned  in  the ALIGNED  variant)  with  the  constrained  length  "ub"  octets  which  shall  be  appended  to  the
         field-list  with  no  length  determinant, completing the procedures of this clause.
    16.8 If  16.5  to  16.7  do  not  apply,  the  octetstring  shall  be  placed  in  a  bit-field  (octet-aligned  in  the
         ALIGNED  variant)  of  length  "n"  octets  and  the  procedures  of  10.9  shall  be  invoked  to  add  this  bit-field
         (octet-aligned  in  the  ALIGNED variant) of "n" octets to the field-list, preceded by a length determinant equal to "n" octets
         as a constrained whole number if "ub" is set, and as a semi-constrained whole number if "ub" is unset. "lb" is as determined
         above. NOTE – The fragmentation procedures may apply after 16K, 32K, 48K, or 64K octets.
    */
    PerSerializer& operator<<(PerSerializer& os, const std::string & str) {
      int byte_size = os.getEncodingType() == Serializer::EncodingType::IA5String ? 5 : 8;

      for (auto c : str)
      {
        os.stream.push_back(c, byte_size);
      }
      return os;
    };

    template <int MinSize = Unbounded, int MaxSize = MinSize>
    PerSerializer& operator<<(PerSerializer& os, const Asn1::Asn1IA5String<MinSize, MaxSize> & str) {
      // Assume not extensible. If extensible, should maybe create and use Asn1ExtensibleIA5String to identify
      os.stream.push_back(str.str().size(), SizeField<MaxSize-MinSize>::size);
      os << IA5String << str.str() << PerDefaultSerializer;
      return os;
    };

    template <int Size>
    PerSerializer& operator<<(PerSerializer& os, const Asn1::Asn1IA5String<Size, Size> & str) {
      // Assume not extensible. If extensible, should maybe create and use Asn1ExtensibleIA5String to identify
      // MinSize == MaxSize -> no size encoded
      os << IA5String << str.str() << PerDefaultSerializer;
      return os;
    };

    template <int MinSize = Unbounded, int MaxSize = MinSize>
    PerSerializer& operator<<(PerSerializer& os, const Asn1::Asn1String<MinSize, MaxSize> & str) {
      // Assume not extensible. If extensible, should maybe create and use Asn1ExtensibleString to identify
      return os;
    };

    template <int Size>
    PerSerializer& operator<<(PerSerializer& os, const Asn1::Asn1String<Size, Size> & s) {
      // Assume not extensible. If extensible, should maybe create and use Asn1ExtensibleString to identify
      return os;
    };

    // 17 - Encoding the null type 
    /* NOTE –(Tutorial) The null type is essentially a place holder, with practical meaning only in the case of a choice or an optional 
                        set or sequence component.Identification of the null in a choice, or its presence as an optional element,
                        is performed in these encoding rules without the need to have octets representing the null. Null values therefore
                        never contribute to the octets of an encoding.
       There shall be no addition to the field - list for a null value.
    */
    PerSerializer& operator<<(PerSerializer& os, const Asn1::Asn1NULL &) {
      return os;
    };

    /*
    18 Encoding the sequence type
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
    PerSerializer& operator<<(PerSerializer& os, Asn1::Sequence &) {
      return os;
    };


    /*
    19 - Encoding the sequence-of type 
    19.1 PER-visible constraints can constrain the number of components of the sequence-of type.
    19.2 Let the maximum number of components in the sequence-of (as determined by PER-visible constraints) be
         "ub" components and the minimum number of components be "lb". If there is no finite maximum or "ub" is greater than
         or equal to 64K we say that "ub" is unset. If there is no constraint on the minimum, then "lb" has the value zero. Let the
         number of components in the actual sequence-of value to be encoded be "n" components.
    19.3 The encoding of each component of the sequence-of will generate a number of fields to be appended to the
         field-list for the sequence-of type.
    19.4 If there is a PER-visible constraint and an extension marker is present in it, a single bit shall be added to the
         field-list in a bit-field of length one. The bit shall be set to 1 if the number of components in this encoding is not within
         the range of the extension root, and zero otherwise. In the former case 10.9 shall be invoked to add the length
         determinant as a semi-constrained whole number to the field-list, followed by the component values. In the latter case
         the length and value shall be encoded as if the extension marker is not present.
    19.5 If the number of components is fixed ("ub" equals "lb") and "ub" is less than 64K, then there shall be no length
         determinant for the sequence-of, and the fields of each component shall be appended in turn to the field-list of the
         sequence-of.
    19.6 Otherwise, the procedures of 10.9 shall be invoked to add the list of fields generated by the "n" components to
         the field-list, preceded by a length determinant equal to "n" components as a constrained whole number if "ub" is set,
         and as a semi-constrained whole number if "ub" is unset. "lb" is as determined above.
       NOTE 1 – The fragmentation procedures may apply after 16K, 32K, 48K, or 64K components.
       NOTE 2 – The break-points for fragmentation are between fields. The number of bits prior to a break-point are not necessarily a
                multiple of eight.
    */
    // Note: Extensible, unbounded and large (greater than 64K) SequenceOf types are not supported

    template <typename T, int LowerBound, int UpperBound>
    PerSerializer& operator<<(PerSerializer& os, const Asn1::SequenceOf<T, LowerBound,UpperBound> & seqof) {
      os.stream.push_back(seqof.size() - LowerBound, SizeField<UpperBound - LowerBound>::size);
      for (auto & e : seqof) {
        os << e;
      }
      return os;
    };

    template <typename T, int Size>
    PerSerializer& operator<<(PerSerializer& os, const Asn1::SequenceOf<T, Size> & seqof) {
      for (auto & e : seqof) {
        os << e;
      }
      return os;
    };

  }


}