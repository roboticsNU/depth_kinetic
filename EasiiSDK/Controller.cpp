/***************************************************************************************/
//  SoftKinetic iisu SDK
//  Project Name      : iisu SDK
//  Revision          : 3.0.2
//  Date              : 2012/01/31
//  Module Name       : Easii
//  Description       : Encapsulates user or UI controller data.
//
// COPYRIGHT AND CONFIDENTIALITY NOTICE - SOFTKINETIC CONFIDENTIAL
// INFORMATION All rights reserved to SOFTKINETIC SOFTWARE SA (a
// company incorporated and existing under the laws of Belgium, with
// its principal place of business at Boulevard de la Plainelaan 15,
// 1050 Brussels (Belgium), registered with the Crossroads bank for
// enterprises under company number 0890 705 864 - "Softkinetic
// Software")
//
// The source code of the SoftKinetic iisu SDK is proprietary and
// confidential information of Softkinetic Software SA
// For any question about terms and conditions, please contact: info@softkinetic.com
// Copyright (c) 2002-2013 Softkinetic Software SA
/****************************************************************************************/

#include "Controller.h"

namespace SK {
namespace Easii {
Controller::~Controller()
{
}

const SK::Array<Gesture> &Controller::getGestureList() const
{
	return getGestures();
}
} // namespace Easii
} // namespace SK
