/***************************************************************************************/
//  SoftKinetic iisu SDK
//  Project Name      : iisu SDK
//  Revision          : 3.0.2
//  Date              : 2012/01/31
//  Module Name       : Easii
//  Description       : Encapsulates a UI controller.
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

#include "UIController.h"
#include "BufferedVector.h"
#include "BoundingBox.h"
#include <string>

// Include this only to make backward-compatible code work
#include "Iisu.h"

namespace SK {
namespace Easii {
struct UIController::Impl
{
	UIPointer m_pointer;

	DataHandle<int32_t> m_masterActive;
	DataHandle<SK::Vector3> m_position;
	ParameterHandle<float> m_controllerBoxWidth;
	ParameterHandle<float> m_controllerBoxHeight;
	ParameterHandle<float> m_controllerBoxDepth;
	int32_t m_index;
	Utils::BufferedVector<Gesture> m_gestures;

};

UIController::UIController()
	: m_pimpl(new Impl)
{
}

UIController::~UIController()
{
	delete m_pimpl;
}

SK::Result UIController::init(int index, SK::Device &device)
{
	std::stringstream ss;
	ss << "UI.CONTROLLER" << (index + 1) << ".";
	std::string prefix = ss.str();

	// Grab data handles
	ss.str("");
	ss << prefix << "Position";
	m_pimpl->m_position =  device.registerDataHandle<SK::Vector3>(ss.str().c_str());

	ss.str("");
	ss << prefix << "POINTER.Status";
	m_pimpl->m_masterActive =  device.registerDataHandle<int32_t>(ss.str().c_str());

	m_pimpl->m_controllerBoxWidth =  device.registerParameterHandle<float>("UI.CONTROLLERS.Width");
	m_pimpl->m_controllerBoxHeight =  device.registerParameterHandle<float>("UI.CONTROLLERS.Height");
	m_pimpl->m_controllerBoxDepth =  device.registerParameterHandle<float>("UI.CONTROLLERS.Depth");

	return m_pimpl->m_pointer.init(index, device);
}

//void UIController::beforeFrame()
//{
//	m_pimpl->m_gestures.flip();
//}

bool UIController::isActive() const
{
	return m_pimpl->m_pointer.getStatus() == 3;
}

UIPointer & UIController::getPointer()
{
	return m_pimpl->m_pointer;
}

const SK::Array<Gesture> &UIController::getGestures() const
{
	return m_pimpl->m_gestures.getReadArray();
}

BoundingBox UIController::getBoundingBox() const
{
	SK::Vector3 position = const_cast<SK::DataHandle<SK::Vector3> &>(m_pimpl->m_position).get();
	float width = const_cast<SK::ParameterHandle<float> &>(m_pimpl->m_controllerBoxWidth).get();
	float height = const_cast<SK::ParameterHandle<float> &>(m_pimpl->m_controllerBoxHeight).get();
	float depth = const_cast<SK::ParameterHandle<float> &>(m_pimpl->m_controllerBoxDepth).get();
	SK::Vector3 mod((width / 2), (depth / 2), (height / 2));
	return BoundingBox((position - mod), (position + mod));
}

SK::Vector3 UIController::getPosition() const
{
	return const_cast<SK::DataHandle<SK::Vector3> &>(m_pimpl->m_position).get();
}

void UIController::addGesture(const Gesture &gesture)
{
	m_pimpl->m_gestures.pushBack(gesture);
}
} // namespace Easii
} // namespace SK
