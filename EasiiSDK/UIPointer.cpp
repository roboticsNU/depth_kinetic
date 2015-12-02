/***************************************************************************************/
//  SoftKinetic iisu SDK
//  Project Name      : iisu SDK
//  Revision          : 3.0.2
//  Date              : 2012/01/31
//  Module Name       : Easii
//  Description       : Encapsulates a UI pointer.
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

#include "UIPointer.h"
#include "BufferedVector.h"
#include <Iisu/Devices/Device.h>
#include <Foundation/DataTypes/Geometry/Vector3.h>
#include <Iisu/Devices/DataExtractor.h>
#include <Foundation/Services/Events/GestureEvent.h>

namespace SK {
namespace Easii {
struct UIPointer::Impl
{
	DataHandle<SK::Vector3> m_position;
	DataHandle<int32_t> m_status;
};

UIPointer::UIPointer()
	: m_pimpl(new Impl)
{
}

UIPointer::~UIPointer()
{
	delete m_pimpl;
}

SK::Result UIPointer::init(int index, SK::Device &device)
{
	std::stringstream ss;
	ss << "UI.CONTROLLER" << (index + 1) << ".";
	std::string prefix = ss.str();

	ss.str("");
	ss << prefix << "POINTER.WorldCoordinates";
	m_pimpl->m_position = device.registerDataHandle<Vector3>(ss.str().c_str());

	ss.str("");
	ss << prefix << "POINTER.Status";
	m_pimpl->m_status = device.registerDataHandle<int32_t>(ss.str().c_str());

	return SK::Result::NoError;
}

SK::Vector3 UIPointer::getPosition() const
{
	return const_cast<SK::DataHandle<SK::Vector3> &>(m_pimpl->m_position).get();
}

int UIPointer::getStatus() const
{
	return const_cast<SK::DataHandle<int32_t> &>(m_pimpl->m_status).get();
}
} // namespace Easii
} // namespace SK
