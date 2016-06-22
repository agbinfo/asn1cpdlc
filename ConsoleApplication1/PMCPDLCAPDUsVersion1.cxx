// PMCPDLCAPDUsVersion1.cxx
//

#include "PMCPDLCAPDUsVersion1.h"

# include "PMCPDLCMessageSetVersion1.h"

namespace PMCPDLCAPDUsVersion1 {
ProtectedGroundPDUs::Option<ProtectedGroundPDUs::abortUser_idx, PMCPDLCUserAbortReason>::type ProtectedGroundPDUs::abortUser()
{
  return Option<abortUser_idx, PMCPDLCUserAbortReason>::getElement(*this);
}
ProtectedGroundPDUs::Option<ProtectedGroundPDUs::abortProvider_idx, PMCPDLCProviderAbortReason>::type ProtectedGroundPDUs::abortProvider()
{
  return Option<abortProvider_idx, PMCPDLCProviderAbortReason>::getElement(*this);
}
ProtectedGroundPDUs::Option<ProtectedGroundPDUs::startup_idx, ProtectedUplinkMessage>::type ProtectedGroundPDUs::startup()
{
  return Option<startup_idx, ProtectedUplinkMessage>::getElement(*this);
}
ProtectedGroundPDUs::Option<ProtectedGroundPDUs::send_idx, ProtectedUplinkMessage>::type ProtectedGroundPDUs::send()
{
  return Option<send_idx, ProtectedUplinkMessage>::getElement(*this);
}
ProtectedGroundPDUs::Option<ProtectedGroundPDUs::forward_idx, ATCForwardMessage>::type ProtectedGroundPDUs::forward()
{
  return Option<forward_idx, ATCForwardMessage>::getElement(*this);
}
ProtectedGroundPDUs::Option<ProtectedGroundPDUs::forwardresponse_idx, ATCForwardResponse>::type ProtectedGroundPDUs::forwardresponse()
{
  return Option<forwardresponse_idx, ATCForwardResponse>::getElement(*this);
}
ForwardMessage::Option<ForwardMessage::upElementIDs_idx, Asn1::BitString>::type ForwardMessage::upElementIDs()
{
  return Option<upElementIDs_idx, Asn1::BitString>::getElement(*this);
}
ForwardMessage::Option<ForwardMessage::downElementIDs_idx, Asn1::BitString>::type ForwardMessage::downElementIDs()
{
  return Option<downElementIDs_idx, Asn1::BitString>::getElement(*this);
}
ProtectedAircraftPDUs::Option<ProtectedAircraftPDUs::abortUser_idx, PMCPDLCUserAbortReason>::type ProtectedAircraftPDUs::abortUser()
{
  return Option<abortUser_idx, PMCPDLCUserAbortReason>::getElement(*this);
}
ProtectedAircraftPDUs::Option<ProtectedAircraftPDUs::abortProvider_idx, PMCPDLCProviderAbortReason>::type ProtectedAircraftPDUs::abortProvider()
{
  return Option<abortProvider_idx, PMCPDLCProviderAbortReason>::getElement(*this);
}
ProtectedAircraftPDUs::Option<ProtectedAircraftPDUs::startdown_idx, ProtectedStartDownMessage>::type ProtectedAircraftPDUs::startdown()
{
  return Option<startdown_idx, ProtectedStartDownMessage>::getElement(*this);
}
ProtectedAircraftPDUs::Option<ProtectedAircraftPDUs::send_idx, ProtectedDownlinkMessage>::type ProtectedAircraftPDUs::send()
{
  return Option<send_idx, ProtectedDownlinkMessage>::getElement(*this);
}
}
