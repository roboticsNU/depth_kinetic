/***************************************************************************************/
//  SoftKinetic iisu SDK
//  Project Name      : iisu SDK
//  Revision          : 3.0.2
//  Date              : 2012/01/31
//  Module Name       : Easii
//  Description       : Encapsulates a user controller.
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

#include "UserController.h"
#include "BufferedVector.h"
#include "BodyPart.h"

namespace SK {
namespace Easii {
struct UserController::Impl
{
	SK::Easii::UserPointer m_pointer;

	//DataHandle<int32_t> m_masterActive;
	ParameterHandle<float> m_controllerBoxWidth;
	ParameterHandle<float> m_controllerBoxHeight;
	ParameterHandle<float> m_controllerBoxDepth;

	SK::DataHandle<SK::Array<SK::Vector3> >* m_bodyPartArray;

	bool m_isActive;

	Utils::BufferedVector<Gesture> m_gestures;

	Impl();
};

UserController::Impl::Impl()
	: m_isActive(false)
{
}

UserController::UserController()
	: m_pimpl(new Impl)
{
}

UserController::~UserController()
{
	delete m_pimpl;
}

SK::Result UserController::init(int index, SK::Device &device, DataHandle<SK::Array<SK::Vector3> >& bodyPartArray)
{
	m_pimpl->m_bodyPartArray = &bodyPartArray;
	m_pimpl->m_controllerBoxWidth =  device.registerParameterHandle<float>("UI.CONTROLLERS.Width");
	m_pimpl->m_controllerBoxHeight =  device.registerParameterHandle<float>("UI.CONTROLLERS.Height");
	m_pimpl->m_controllerBoxDepth =  device.registerParameterHandle<float>("UI.CONTROLLERS.Depth");
	m_pimpl->m_pointer.init(*this, bodyPartArray);
	return Result::NoError;
};

SK::Vector3 UserController::normalizeCoordinates(const SK::Vector3 &coords) const
{
	float depth = m_pimpl->m_controllerBoxDepth.get();
	float height = m_pimpl->m_controllerBoxHeight.get();
	float width = m_pimpl->m_controllerBoxWidth.get();
	return SK::Vector3(-coords.x / width , coords.y / depth, coords.z / height);
}

SK::Vector3 UserController::worldToController(const SK::Vector3 &coords) const
{
	SK::Vector3 p = coords - getPosition();
	return SK::Vector3(p.dot(SK::Vector3::UNIT_X), p.dot(SK::Vector3::UNIT_Y), p.dot(SK::Vector3::UNIT_Z));
}

bool UserController::isActive() const
{
	return m_pimpl->m_isActive;
}

const SK::Array<Gesture> &UserController::getGestures() const
{
	return m_pimpl->m_gestures.getReadArray();
}

UserPointer & UserController::getPointer()
{
	return m_pimpl->m_pointer;
}

BoundingBox UserController::getBoundingBox() const
{
	SK::Vector3 position = getPosition();
	float width = m_pimpl->m_controllerBoxWidth.get();
	float height = m_pimpl->m_controllerBoxHeight.get();
	float depth = m_pimpl->m_controllerBoxDepth.get();
	SK::Vector3 mod(width / 2, depth / 2, height / 2);
	return BoundingBox(position - mod, position + mod);
};

SK::Vector3 UserController::getPosition() const
{
	SK::Vector3 position = m_pimpl->m_bodyPartArray->get()[SK::Easii::BodyPartEnum::COLLAR];
	position.y = position.y - 0.3f;
	return position;
}

void UserController::addGesture(const Gesture &)
{
}
} // namespace Easii
} // namespace Sk
