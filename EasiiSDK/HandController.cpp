/***************************************************************************************/
//  SoftKinetic iisu SDK
//  Project Name      : iisu SDK
//  Revision          : 3.6
//  Date              : 2012/02/20
//  Module Name       : Easii
//  Description       : Encapsulates the data to model a hand controller.
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

#include "HandController.h"
#include "HandPointer.h"
#include "Hand.h"
#include "BufferedVector.h"
#include "BoundingBox.h"

namespace SK {
namespace Easii {
struct HandController::Impl
{
	Hand *m_hand;
	HandPointer m_pointer;
	Utils::BufferedVector<Gesture> m_gestures;

	Impl();
};

HandController::Impl::Impl()
	: m_hand(static_cast<Hand *>(0))
{
}

HandController::HandController()
	: m_pimpl(new Impl)
{
}

HandController::~HandController()
{
	delete m_pimpl;
}

void HandController::init(Hand &hand)
{
	m_pimpl->m_hand = &hand;
	m_pimpl->m_pointer.init(hand);
}

bool HandController::isActive() const
{
	assert(m_pimpl->m_hand);
	return m_pimpl->m_hand->getStatus() != 0;
}

HandPointer &HandController::getPointer()
{
	return m_pimpl->m_pointer;
}

const SK::Array<Gesture> &HandController::getGestures() const
{
	return m_pimpl->m_gestures.getReadArray();
}

BoundingBox HandController::getBoundingBox() const
{
	return BoundingBox(Vector3(), Vector3());
}

SK::Vector3 HandController::getPosition() const
{
	assert(m_pimpl->m_hand);
	return m_pimpl->m_hand->getPalmPosition3D();
}

void HandController::addGesture(const Gesture &)
{
}
} // namespace Easii
} // namespace SK
