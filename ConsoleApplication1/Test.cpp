// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include <iterator>
#include <vector>
#include <iostream>
#include "Asn1.h"
#include "PMCPDLCMessageSetVersion1.h"
#include "PMCPDLCAPDUsVersion1.h"
#include "PMCPDLCMessageSetVersion1Serializer.h"
// #include "CMMessageSetVersion1.h"

//namespace Test {
//  class CmLogonRequest;
//  class B;
//  typedef Asn1::Asn1Int<0, 23> Timehours;
//
//  template <typename T> class ChoiceElement {};
//
//
//  class CMAircraftMessage : public Asn1::Choice { // <CMAircraftMessage_Choices> {
//  public:
//    //Option<1, Timehours>::type timehours() { Option<1, Timehours> opt; return opt.getElement(*this); }
//    Option<1, CmLogonRequest>::type cmLogonRequest() { return Option<1, CmLogonRequest>::getElement(*this); }
//    const Option<1, CmLogonRequest>::type cmLogonRequest() const { return Option<1, CmLogonRequest>::getElement(*this); }
//    Option<2, B>::type b() { return Option<2, B>::getElement(*this); }
//    Option<3, B>::type b2() { return Option<3, B>::getElement(*this); }
//  public:
//    CMAircraftMessage() : Choice()
//    {
//    }
//    //		const Asn1Object& getElement() const { return cmLogonRequest(); }
//    // Asn1Object& getElement() { return cmLogonRequest(); }
//    // CMLogonRequest;
//    // CMContactResponse
//    // CMAbortReason
//    // ...
//    // CMServerFacilityQueryRequest
//  };
//
//  class CmLogonRequest : public Asn1::Sequence {
//  public:
//    Optional<B>::type b;
//    CmLogonRequest() : Asn1::Sequence(1) {
//      Asn1::Sequence::Inserter it = back_inserter();
//      *it++ = b;
//    }
//  };
//
//  class B;
//  class LogicalAck;
//
//  class B : public Asn1::Sequence {
//  public:
//    // Required<CmLogonRequest>::type a;
//    Required<Timehours>::type a;
//    Required<LogicalAck>::type ack;
//    
//    B() : Asn1::Sequence(1) {
//      Asn1::Sequence::Inserter it = back_inserter();
//      *it++ = a;
//    }
//  };
//
//  class LogicalAck : public Asn1::Enumerated<0,1> {
//  public:
//    typedef enum {
//      required = 0,
//      notRequired = 1
//    } EnumValues;
//    LogicalAck() : Enumerated(required) {  }
//    LogicalAck(int value) : Enumerated(value) {  }
//  };
//
//}
//
//using namespace Test;

//int main()
//{
//  CMAircraftMessage cmAircraftMessage;
//  cmAircraftMessage.cmLogonRequest().b().a() = 22;
//  cmAircraftMessage.cmLogonRequest().b().ack() = LogicalAck::notRequired;
//  CmLogonRequest cmLogonRequest;
//  cmLogonRequest.b() = cmAircraftMessage.cmLogonRequest().b();
//  // std::cout << "size of cmAircraftMessage is " << sizeof(cmAircraftMessage) << std::endl;
//  // cmAircraftMessage.cmLogonRequest();
//  // cmAircraftMessage.b();
//  B b;
//  return 0;
//}


using namespace PMCPDLCMessageSetVersion1;
using namespace PMCPDLCAPDUsVersion1;

int main()
{
  //ProtectedStartDownMessage startDownMessage;
  //startDownMessage.mode = ProtectedMode::cpdlc;
  //startDownMessage.startDownlinkMessage = 

  ATCUplinkMessage atcUplinkMessage;
  ATCMessageHeader atcMessageHeader;
  DateTimeGroup dateTimeGroup;
  Date date;

  date.day = 1;
  date.month = 9;
  date.year = 2015;
  Timehhmmss timeHhmmss;
  Time time;
  time.hours = 22;
  time.minutes = 19;
  timeHhmmss.hoursminutes = time;
  timeHhmmss.seconds = 12;

  dateTimeGroup.date = date;
  dateTimeGroup.timehhmmss = timeHhmmss;

  atcMessageHeader.messageIdNumber = 0;
  // OPTIONAL atcMessageHeader.messageRefNumber
  atcMessageHeader.dateTime = dateTimeGroup;
  atcMessageHeader.logicalAck = LogicalAck::notRequired;

  ATCUplinkMessageData atcUplinkMessageData;
  ATCUplinkMsgElementId atcUplinkMsgElementId;
  atcUplinkMsgElementId.uM107NULL();  // MAINTAIN PRESENT SPEED

  atcUplinkMessageData.elementIds()["1"] = atcUplinkMsgElementId;
  // OPTIONAL atcUplinkMessageData.constrainedData

  atcUplinkMessage.header = atcMessageHeader;
  atcUplinkMessage.messageData = atcUplinkMessageData;

  PMCPDLCMessageSetVersion1::Serializer ser;
  ser << atcUplinkMessage;

  return 0;
}
