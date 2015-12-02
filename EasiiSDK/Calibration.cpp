/***************************************************************************************/
//  SoftKinetic iisu SDK
//  Project Name      : iisu SDK
//  Revision          : 3.0.2
//  Date              : 2012/01/31
//  Module Name       : Easii
//  Description       : Encapsulates iisu's scene calibration.
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

#include "Calibration.h"
#include <Iisu/Devices/DataExtractor.h>

namespace SK {
namespace Easii {
struct Calibration::Impl
{
	ParameterHandle<SK::Matrix4> m_cam2worldMatrix;
};

Calibration::Calibration()
	: m_pimpl(new Impl)
{
}

Calibration::~Calibration()
{
	delete m_pimpl;
}

SK::Result Calibration::init(SK::Device &device)
{
	m_pimpl->m_cam2worldMatrix = device.registerParameterHandle<SK::Matrix4>("SCENE.CameraToWorld");
	return SK::Result::NoError;
}

SK::Matrix4 Calibration::getCam2WorldMatrix() const
{
	return getCameraToWorldMatrix();
}

SK::Matrix4 Calibration::getCameraToWorldMatrix() const
{
	return const_cast<SK::ParameterHandle<SK::Matrix4> &>(m_pimpl->m_cam2worldMatrix).get();
}

SK::Vector3 Calibration::cam2World(const SK::Vector3 &v) const
{
	return cameraToWorld(v);
}

SK::Vector3 Calibration::cameraToWorld(const SK::Vector3 &v) const
{
	return
		const_cast<SK::ParameterHandle<SK::Matrix4> &>(m_pimpl->m_cam2worldMatrix)
		.get().multiplyByPoint(v);
}

SK::Vector3 Calibration::world2Cam(const SK::Vector3 &v) const
{
	return worldToCamera(v);
}

SK::Vector3 Calibration::worldToCamera(const SK::Vector3 &v) const
{
	return
		const_cast<SK::ParameterHandle<SK::Matrix4> &>(m_pimpl->m_cam2worldMatrix)
		.get().getInverseAffine().multiplyByPoint(v);
}
} // namespace Easii
} // namespace SK
