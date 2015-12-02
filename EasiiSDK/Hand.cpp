/***************************************************************************************/
//  SoftKinetic iisu SDK
//  Project Name      : iisu SDK
//  Revision          : 3.6
//  Date              : 2012/02/20
//  Module Name       : Easii
//  Description       : Encapsulates the data to model a hand in near mode.
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

#include "Hand.h"
#include "HandController.h"
#include <Iisu/Devices/DataExtractor.h>

namespace SK {
namespace Easii {
struct Hand::Impl
{
	int m_index;

	DataHandle<int32_t> m_status;
	DataHandle<bool> m_isGroup;
	DataHandle<bool> m_isOpen;
	DataHandle<int32_t> m_label;
	DataHandle<int32_t> m_handedness;
	DataHandle<int32_t> m_handGestureID;

	DataHandle<bool> m_isIntersectingUpperImageBoundary;
	DataHandle<bool> m_isIntersectingLowerImageBoundary;
	DataHandle<bool> m_isIntersectingLeftImageBoundary;
	DataHandle<bool> m_isIntersectingRightImageBoundary;

	DataHandle<Vector2> m_palmPosition2d;
	DataHandle<Vector3> m_palmPosition3d;
	DataHandle<Vector2> m_tipPosition2d;
	DataHandle<Vector3> m_tipPosition3d;
	DataHandle<Vector2> m_forearmPosition2d;
	DataHandle<Vector3> m_forearmPosition3d;
	DataHandle<Vector3> m_palmNormal3d;
	DataHandle<float> m_palmRadius;
	DataHandle<Vector3> m_upperGrabberPosition3d;
	DataHandle<Vector3> m_lowerGrabberPosition3d;
	DataHandle<Vector3> m_centerGrabberPosition3d;
	DataHandle<float> m_Openness;

	DataHandle<Array<Vector3> > m_centroids;
	DataHandle<Array<int32_t> > m_jumpStatuses;

	DataHandle<Array<int32_t> > m_fingerStatuses;
	DataHandle<Array<Vector2> > m_fingerTips2d;
	DataHandle<Array<Vector3> > m_fingerTips3d;

	DataHandle<Array<Vector2> > m_meshPoints2d;
	DataHandle<Array<Vector3> > m_meshPoints3d;
	DataHandle<Array<Vector3> > m_meshNormals;
	DataHandle<Array<float> > m_meshIntensities;
	DataHandle<Array<Vector3i> > m_meshTriangles;
	DataHandle<Array<int32_t> > m_meshContour;

	HandController m_controller;

};

Hand::Hand()
	: m_pimpl(new Impl)
{
}

Hand::~Hand()
{
	delete m_pimpl;
}

SK::Result Hand::init(int index, SK::Device &device)
{
	std::ostringstream os;
	os << "CI.HAND" << (index + 1) << ".";
	std::string prefix = os.str();

	os.str("");
	os << prefix << "Status";
	m_pimpl->m_status = device.registerDataHandle<int32_t>(os.str().c_str());

	os.str("");
	os << prefix << "IsOpen";
	m_pimpl->m_isOpen = device.registerDataHandle<bool>(os.str().c_str());

	os.str("");
	os << prefix << "Label";
	m_pimpl->m_label = device.registerDataHandle<int32_t>(os.str().c_str());

	os.str("");
	os << prefix << "IsIntersectingUpperImageBoundary";
	m_pimpl->m_isIntersectingUpperImageBoundary = device.registerDataHandle<bool>(os.str().c_str());

	os.str("");
	os << prefix << "IsIntersectingLowerImageBoundary";
	m_pimpl->m_isIntersectingLowerImageBoundary = device.registerDataHandle<bool>(os.str().c_str());

	os.str("");
	os << prefix << "IsIntersectingLeftImageBoundary";
	m_pimpl->m_isIntersectingLeftImageBoundary = device.registerDataHandle<bool>(os.str().c_str());

	os.str("");
	os << prefix << "IsIntersectingRightImageBoundary";
	m_pimpl->m_isIntersectingRightImageBoundary = device.registerDataHandle<bool>(os.str().c_str());

	os.str("");
	os << prefix << "PalmPosition2D";
	m_pimpl->m_palmPosition2d = device.registerDataHandle<SK::Vector2>(os.str().c_str());

	os.str("");
	os << prefix << "PalmPosition3D";
	m_pimpl->m_palmPosition3d = device.registerDataHandle<SK::Vector3>(os.str().c_str());

	os.str("");
	os << prefix << "UpperGrabber3D";
	m_pimpl->m_upperGrabberPosition3d = device.registerDataHandle<SK::Vector3>(os.str().c_str());

	os.str("");
	os << prefix << "LowerGrabber3D";
	m_pimpl->m_lowerGrabberPosition3d = device.registerDataHandle<SK::Vector3>(os.str().c_str());

	os.str("");
	os << prefix << "CentralGrabber3D";
	m_pimpl->m_centerGrabberPosition3d = device.registerDataHandle<SK::Vector3>(os.str().c_str());

	os.str("");
	os << prefix << "Openness";
	m_pimpl->m_Openness = device.registerDataHandle<float>(os.str().c_str());

	

	os.str("");
	os << prefix << "TipPosition2D";
	m_pimpl->m_tipPosition2d = device.registerDataHandle<SK::Vector2>(os.str().c_str());

	os.str("");
	os << prefix << "TipPosition3D";
	m_pimpl->m_tipPosition3d = device.registerDataHandle<SK::Vector3>(os.str().c_str());

	os.str("");
	os << prefix << "ForearmPosition2D";
	m_pimpl->m_forearmPosition2d = device.registerDataHandle<SK::Vector2>(os.str().c_str());

	os.str("");
	os << prefix << "ForearmPosition3D";
	m_pimpl->m_forearmPosition3d = device.registerDataHandle<SK::Vector3>(os.str().c_str());

	os.str("");
	os << prefix << "PalmNormal3D";
	m_pimpl->m_palmNormal3d = device.registerDataHandle<SK::Vector3>(os.str().c_str());

	os.str("");
	os << prefix << "PalmRadius3D";
	m_pimpl->m_palmRadius = device.registerDataHandle<float>(os.str().c_str());

	os.str("");
	os << prefix << "CENTROIDS.Positions";
	m_pimpl->m_centroids = device.registerDataHandle<SK::Array<Vector3> >(os.str().c_str());

	os.str("");
	os << prefix << "CENTROIDS.JumpStatus";
	m_pimpl->m_jumpStatuses = device.registerDataHandle<SK::Array<int32_t> >(os.str().c_str());

	os.str("");
	os << prefix << "FingerStatus";
	m_pimpl->m_fingerStatuses = device.registerDataHandle<SK::Array<int32_t> >(os.str().c_str());

	os.str("");
	os << prefix << "FingerTipPositions2D";
	m_pimpl->m_fingerTips2d = device.registerDataHandle<SK::Array<Vector2> >(os.str().c_str());

	os.str("");
	os << prefix << "FingerTipPositions3D";
	m_pimpl->m_fingerTips3d = device.registerDataHandle<SK::Array<Vector3> >(os.str().c_str());

	os.str("");
	os << prefix << "MESH.Points2D";
	m_pimpl->m_meshPoints2d = device.registerDataHandle<SK::Array<Vector2> >(os.str().c_str());

	os.str("");
	os << prefix << "MESH.Points3D";
	m_pimpl->m_meshPoints3d = device.registerDataHandle<SK::Array<Vector3> >(os.str().c_str());

	os.str("");
	os << prefix << "MESH.Normals";
	m_pimpl->m_meshNormals = device.registerDataHandle<SK::Array<Vector3> >(os.str().c_str());

	os.str("");
	os << prefix << "MESH.Intensities";
	m_pimpl->m_meshIntensities = device.registerDataHandle<SK::Array<float> >(os.str().c_str());

	os.str("");
	os << prefix << "MESH.Triangles";
	m_pimpl->m_meshTriangles = device.registerDataHandle<SK::Array<Vector3i> >(os.str().c_str());

	os.str("");
	os << prefix << "MESH.Contour";
	m_pimpl->m_meshContour = device.registerDataHandle<SK::Array<int32_t> >(os.str().c_str());

	os.str("");
	os << prefix << "Side";
	m_pimpl->m_handedness = device.registerDataHandle<int32_t>(os.str().c_str());

	os.str("");
	os << prefix << "PosingGestureId";
	m_pimpl->m_handGestureID = device.registerDataHandle<int32_t>(os.str().c_str());

	m_pimpl->m_controller.init(*this);

	return SK::Result();
}

int32_t Hand::getStatus() const
{
	return m_pimpl->m_status.get();
}

bool Hand::isGroup() const
{
	return m_pimpl->m_isGroup.get();
}

bool Hand::isOpen() const
{
	return m_pimpl->m_isOpen.get();
}

int32_t Hand::getLabelID() const
{
	return m_pimpl->m_label.get();
}

bool Hand::isIntersectingUpperImageBoundary() const
{
	return m_pimpl->m_isIntersectingUpperImageBoundary.get();
}

bool Hand::isIntersectingLowerImageBoundary() const
{
	return m_pimpl->m_isIntersectingLowerImageBoundary.get();
}

bool Hand::isIntersectingLeftImageBoundary() const
{
	return m_pimpl->m_isIntersectingLeftImageBoundary.get();
}

bool Hand::isIntersectingRightImageBoundary() const
{
	return m_pimpl->m_isIntersectingRightImageBoundary.get();
}
SK::Vector2 Hand::getPalmPosition2D() const
{
	return m_pimpl->m_palmPosition2d.get();
}

SK::Vector3 Hand::getPalmPosition3D() const
{
	return m_pimpl->m_palmPosition3d.get();
}

SK::Vector3 Hand::getUpperGrabberPosition3D() const
{
	return m_pimpl->m_upperGrabberPosition3d.get();
}

SK::Vector3 Hand::getLowerGrabberPosition3D() const
{
	return m_pimpl->m_lowerGrabberPosition3d.get();
}

SK::Vector3 Hand::getCenterGrabberPosition3D() const
{
	return m_pimpl->m_centerGrabberPosition3d.get();
}

float Hand::getOpenness() const
{
	return m_pimpl->m_Openness.get();
}

SK::Vector2 Hand::getTipPosition2D() const
{
	return m_pimpl->m_tipPosition2d.get();
}

SK::Vector3 Hand::getTipPosition3D() const
{
	return m_pimpl->m_tipPosition3d.get();
}

SK::Vector2 Hand::getForearmPosition2D() const
{
	return m_pimpl->m_forearmPosition2d.get();
}

SK::Vector3 Hand::getForearmPosition3D() const
{
	return m_pimpl->m_forearmPosition3d.get();
}

SK::Vector3 Hand::getPalmNormal3D() const
{
	return m_pimpl->m_palmNormal3d.get();
}

float Hand::getPalmRadius() const
{
	return m_pimpl->m_palmRadius.get();
}

const SK::Array<SK::Vector3> &Hand::getCentroids() const
{
	return m_pimpl->m_centroids.get();
}

const SK::Array<int32_t> &Hand::getJumpStatuses() const
{
	return m_pimpl->m_jumpStatuses.get();
}

int Hand::getFingerCount() const
{
	return m_pimpl->m_fingerStatuses.get().size();
}

int32_t Hand::getFingerStatus(int index) const
{
	return m_pimpl->m_fingerStatuses.get()[index];
}

const SK::Array<int32_t> &Hand::getFingerStatuses() const
{
	return m_pimpl->m_fingerStatuses.get();
}

SK::Vector2 Hand::getFingerTipPosition2D(int index) const
{
	return m_pimpl->m_fingerTips2d.get()[index];
}

const SK::Array<SK::Vector2> &Hand::getFingerTipPositions2D() const
{
	return m_pimpl->m_fingerTips2d.get();
}

SK::Vector3 Hand::getFingerTipPosition3D(int index) const
{
	return m_pimpl->m_fingerTips3d.get()[index];
}

const SK::Array<SK::Vector3> &Hand::getFingerTipPositions3D() const
{
	return m_pimpl->m_fingerTips3d.get();
}

const SK::Array<SK::Vector2> &Hand::getMeshPoints2D() const
{
	return m_pimpl->m_meshPoints2d.get();
}

const SK::Array<SK::Vector3> &Hand::getMeshPoints3D() const
{
	return m_pimpl->m_meshPoints3d.get();
}

const SK::Array<SK::Vector3> &Hand::getMeshNormals() const
{
	return m_pimpl->m_meshNormals.get();
}

const SK::Array<float> &Hand::getMeshIntensities() const
{
	return m_pimpl->m_meshIntensities.get();
}

const SK::Array<SK::Vector3i> &Hand::getMeshTriangles() const
{
	return m_pimpl->m_meshTriangles.get();
}

const SK::Array<int32_t> &Hand::getMeshContour() const
{
	return m_pimpl->m_meshContour.get();
}

HandController &Hand::getController()
{
	return m_pimpl->m_controller;
}

int32_t Hand::getHandedness() const
{
	return m_pimpl->m_handedness.get();
}

const int32_t & Hand::getPosingGestureID() const
{
	return m_pimpl->m_handGestureID.get();
}

} // namespace Easii
} // namespace SK
