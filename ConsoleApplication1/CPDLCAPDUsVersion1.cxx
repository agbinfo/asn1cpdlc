// CPDLCAPDUsVersion1.cxx
//

#include "CPDLCAPDUsVersion1.h"

# include "PMCPDLCMessageSetVersion1.h"
# include "PMCPDLCAPDUsVersion1.h"

namespace CPDLCAPDUsVersion1 {
GroundPDUs::Option<GroundPDUs::abortUser_idx, CPDLCUserAbortReason>::type GroundPDUs::abortUser()
{
  return Option<abortUser_idx, CPDLCUserAbortReason>::getElement(*this);
}
GroundPDUs::Option<GroundPDUs::abortProvider_idx, CPDLCProviderAbortReason>::type GroundPDUs::abortProvider()
{
  return Option<abortProvider_idx, CPDLCProviderAbortReason>::getElement(*this);
}
GroundPDUs::Option<GroundPDUs::startup_idx, UplinkMessage>::type GroundPDUs::startup()
{
  return Option<startup_idx, UplinkMessage>::getElement(*this);
}
GroundPDUs::Option<GroundPDUs::send_idx, PMCPDLCMessageSetVersion1::ATCUplinkMessage>::type GroundPDUs::send()
{
  return Option<send_idx, PMCPDLCMessageSetVersion1::ATCUplinkMessage>::getElement(*this);
}
GroundPDUs::Option<GroundPDUs::forward_idx, PMCPDLCAPDUsVersion1::ATCForwardMessage>::type GroundPDUs::forward()
{
  return Option<forward_idx, PMCPDLCAPDUsVersion1::ATCForwardMessage>::getElement(*this);
}
GroundPDUs::Option<GroundPDUs::forwardresponse_idx, PMCPDLCAPDUsVersion1::ATCForwardResponse>::type GroundPDUs::forwardresponse()
{
  return Option<forwardresponse_idx, PMCPDLCAPDUsVersion1::ATCForwardResponse>::getElement(*this);
}
UplinkMessage::Option<UplinkMessage::noMessage_idx, Asn1::Asn1NULL>::type UplinkMessage::noMessage()
{
  return Option<noMessage_idx, Asn1::Asn1NULL>::getElement(*this);
}
UplinkMessage::Option<UplinkMessage::aTCUplinkMessage_idx, PMCPDLCMessageSetVersion1::ATCUplinkMessage>::type UplinkMessage::aTCUplinkMessage()
{
  return Option<aTCUplinkMessage_idx, PMCPDLCMessageSetVersion1::ATCUplinkMessage>::getElement(*this);
}
AircraftPDUs::Option<AircraftPDUs::abortUser_idx, CPDLCUserAbortReason>::type AircraftPDUs::abortUser()
{
  return Option<abortUser_idx, CPDLCUserAbortReason>::getElement(*this);
}
AircraftPDUs::Option<AircraftPDUs::abortProvider_idx, CPDLCProviderAbortReason>::type AircraftPDUs::abortProvider()
{
  return Option<abortProvider_idx, CPDLCProviderAbortReason>::getElement(*this);
}
AircraftPDUs::Option<AircraftPDUs::startdown_idx, StartDownMessage>::type AircraftPDUs::startdown()
{
  return Option<startdown_idx, StartDownMessage>::getElement(*this);
}
AircraftPDUs::Option<AircraftPDUs::send_idx, PMCPDLCMessageSetVersion1::ATCDownlinkMessage>::type AircraftPDUs::send()
{
  return Option<send_idx, PMCPDLCMessageSetVersion1::ATCDownlinkMessage>::getElement(*this);
}
DownlinkMessage::Option<DownlinkMessage::noMessage_idx, Asn1::Asn1NULL>::type DownlinkMessage::noMessage()
{
  return Option<noMessage_idx, Asn1::Asn1NULL>::getElement(*this);
}
DownlinkMessage::Option<DownlinkMessage::aTCDownlinkMessage_idx, PMCPDLCMessageSetVersion1::ATCDownlinkMessage>::type DownlinkMessage::aTCDownlinkMessage()
{
  return Option<aTCDownlinkMessage_idx, PMCPDLCMessageSetVersion1::ATCDownlinkMessage>::getElement(*this);
}
}
