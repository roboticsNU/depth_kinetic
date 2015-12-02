/***************************************************************************************/
//  SoftKinetic iisu SDK
//  Project Name      : iisu SDK
//  Revision          : 3.0.2
//  Date              : 2012/01/31
//  Module Name       : Easii
//  Description       : Encapsulates a user pointer (see skeleton).
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

#include "UserPointer.h"
#include "UserController.h"

namespace SK {
namespace Easii {
struct UserPointer::Impl
{
	SK::Vector3 m_position;
	UserController* m_controller;
	DataHandle<SK::Array<SK::Vector3> >* m_bodyPartArray;
};

UserPointer::UserPointer()
	: m_pimpl(new Impl)
{
}

UserPointer::~UserPointer()
{
	delete m_pimpl;
}

void UserPointer::init(UserController& controller, SK::DataHandle<SK::Array<SK::Vector3> >& bodyPartArray)
{
	m_pimpl->m_bodyPartArray = &bodyPartArray;
	m_pimpl->m_controller = &controller;
}

SK::Vector3 UserPointer::getPosition() const
{
	return m_pimpl->m_controller->normalizeCoordinates(
		m_pimpl->m_controller->worldToController(
			m_pimpl->m_bodyPartArray->get()[SK::SkeletonEnum::RIGHT_WRIST]));
}

int UserPointer::getStatus() const
{
	return 0;
}
} // namespace Easii
} // namespace SK
