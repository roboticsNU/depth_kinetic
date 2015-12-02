/***************************************************************************************/
//  SoftKinetic iisu SDK
//  Project Name      : iisu SDK
//  Revision          : 3.6
//  Date              : 2012/02/20
//  Module Name       : Easii
//  Description       : Encapsulates the data to model a hand pointer.
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

#include "HandPointer.h"
#include "Hand.h"

namespace SK {
namespace Easii {
struct HandPointer::Impl
{
	Hand *m_hand;

	Impl();
};

HandPointer::Impl::Impl()
	: m_hand(static_cast<Hand *>(0))
{
}

HandPointer::HandPointer()
	: m_pimpl(new Impl)
{
}

HandPointer::~HandPointer()
{
	delete m_pimpl;
}

void HandPointer::init(Hand &hand)
{
	m_pimpl->m_hand = &hand;
}

SK::Vector3 HandPointer::getPosition() const
{
	assert(m_pimpl->m_hand);
	return m_pimpl->m_hand->getTipPosition3D();
}

int HandPointer::getStatus() const
{
	assert(m_pimpl->m_hand);
	return m_pimpl->m_hand->getStatus();
}
} // namespace Easii
} // namespace SK
