// CMMessageSetVersion1.cpp
//

#include "PMCPDLCMessageSetVersion1.h"
#include "PMCPDLCAPDUsVersion1.h"
#include "CMMessageSetVersion1.h"

#include <vector>
#include "Asn1.h"
#include "Asn1PerSerializer.h"

namespace CMMessageSetVersion1 {

  GroundPDUs::Option<GroundPDUs::abortUser_idx, CPDLCUserAbortReason>::type GroundPDUs::abortUser() {
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


}

