/***************************************************************************************/
//  SoftKinetic iisu SDK
//  Project Name      : iisu SDK
//  Revision          : 3.0.2
//  Date              : 2012/01/31
//  Module Name       : Easii
//  Description       : Encapsulates information about the camera in
//                      use.
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

#include "CameraInfo.h"

namespace SK {
namespace Easii {
struct CameraInfo::Impl
{
	ParameterHandle<SK::Matrix4> m_cameraMatrix;
	ParameterHandle<SK::String> m_cameraName;
	ParameterHandle<float> m_fovH;
	ParameterHandle<float> m_fovV;

	Impl();
};

CameraInfo::Impl::Impl()
{
}

CameraInfo::CameraInfo()
	: m_pimpl(new Impl)
{
}

CameraInfo::~CameraInfo()
{
	delete m_pimpl;
}

SK::Result CameraInfo::init(SK::Device &device)
{
	m_pimpl->m_cameraMatrix = device.registerParameterHandle<SK::Matrix4>("SCENE.CameraToWorld");
	m_pimpl->m_cameraName = device.registerParameterHandle<SK::String>("SOURCE.CAMERA.Name");
	m_pimpl->m_fovH = device.registerParameterHandle<float>("SOURCE.CAMERA.DEPTH.HFOV");
	m_pimpl->m_fovV = device.registerParameterHandle<float>("SOURCE.CAMERA.DEPTH.VFOV");
	return SK::Result::NoError;
}

SK::String CameraInfo::getName() const
{
	return m_pimpl->m_cameraName.get();
}

SK::Vector3 CameraInfo::getUp() const
{
	SK::Matrix4 m = m_pimpl->m_cameraMatrix.get();
	return m.multiplyByVector(SK::Vector3::UNIT_Z);
}

SK::Vector3 CameraInfo::getFront() const
{
	SK::Matrix4 m = m_pimpl->m_cameraMatrix.get();
	return m.multiplyByVector(SK::Vector3::UNIT_Y);
}

SK::Vector3 CameraInfo::getLeft() const
{
	SK::Matrix4 m = m_pimpl->m_cameraMatrix.get();
	return m.multiplyByVector(SK::Vector3::UNIT_X);
}

SK::Vector3 CameraInfo::getPosition() const
{
	SK::Matrix4 m=m_pimpl->m_cameraMatrix.get();
	return m.getTranslation();
}

float CameraInfo::getHorizontalFOV() const
{
	return m_pimpl->m_fovH.get();
}

float CameraInfo::getVerticalFOV() const
{
	return m_pimpl->m_fovV.get();
}
} // namespace Easii
} // namespace SK
