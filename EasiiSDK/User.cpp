/***************************************************************************************/
//  SoftKinetic iisu SDK
//  Project Name      : iisu SDK
//  Revision          : 3.0.2
//  Date              : 2012/01/31
//  Module Name       : Easii
//  Description       : Encapsulates user data.
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

#include "User.h"
#include <sstream>


namespace SK {
namespace Easii {
struct User::Impl
{
	DataHandle<bool> m_active;
	DataHandle<SK::Array<SK::Vector3> > m_centroids;
	DataHandle<float> m_height;
	DataHandle<SK::Vector3> m_massCenter;
	DataHandle<SK::Vector3> m_mainDirection;
	DataHandle<SK::Vector3> m_boundingBoxMin;
	DataHandle<SK::Vector3> m_boundingBoxMax;
	DataHandle<int32_t> m_sceneObjectID;
	DataHandle<SK::Array<SK::Vertex> > m_vertexArray;
	DataHandle<SK::Array<SK::Vector3> > m_bodyPartArray;
	DataHandle<SK::Array<float> > m_bodyPartConfidenceArray;
	DataHandle<int32_t> m_skeletonStatus;

	SK::Array<SK::Easii::BodyPart> m_bodyParts;

	UserController m_controller;

	Impl();
};

User::Impl::Impl()
{
}

User::User()
	: m_pimpl(new Impl)
{
}

User::~User()
{
	delete m_pimpl;
}

SK::Result User::init(int index, SK::Device &device)
{
	std::stringstream ss;
	ss << "USER"<< (index + 1) <<".";
	std::string prefix = ss.str();

	ss.str("");
	ss << prefix << "IsActive";
	m_pimpl->m_active = device.registerDataHandle<bool>(ss.str().c_str());

	ss.str("");
	ss << prefix << "SHAPE.CENTROIDS.Positions";
	m_pimpl->m_centroids = device.registerDataHandle<SK::Array<SK::Vector3> >(ss.str().c_str());

	ss.str("");
	ss << prefix << "MassCenter";
	m_pimpl->m_massCenter = device.registerDataHandle<SK::Vector3>(ss.str().c_str());

	ss.str("");
	ss << prefix << "Height";
	m_pimpl->m_height = device.registerDataHandle<float>(ss.str().c_str());

	ss.str("");
	ss << prefix << "MainDirection";
	m_pimpl->m_mainDirection = device.registerDataHandle<SK::Vector3>(ss.str().c_str());

	ss.str("");
	ss << prefix << "MaxBoundingBox";
	m_pimpl->m_boundingBoxMax = device.registerDataHandle<SK::Vector3>(ss.str().c_str());

	ss.str("");
	ss << prefix << "MinBoundingBox";
	m_pimpl->m_boundingBoxMin = device.registerDataHandle<SK::Vector3>(ss.str().c_str());

	ss.str("");
	ss << prefix << "SceneObjectID";
	m_pimpl->m_sceneObjectID = device.registerDataHandle<int32_t>(ss.str().c_str());

	ss.str("");
	ss << prefix << "VertexArray";
	m_pimpl->m_vertexArray = device.registerDataHandle<SK::Array<SK::Vertex> >(ss.str().c_str());

	ss.str("");
	ss << prefix << "SKELETON.Status";
	m_pimpl->m_skeletonStatus = device.registerDataHandle<int32_t >(ss.str().c_str());

	ss.str("");
	ss << prefix << "SKELETON.KeyPoints";
	m_pimpl->m_bodyPartArray = device.registerDataHandle<SK::Array<SK::Vector3> >(ss.str().c_str());

	ss.str("");
	ss << prefix << "SKELETON.KeyPointsConfidence";
	m_pimpl->m_bodyPartConfidenceArray = device.registerDataHandle<SK::Array<float> >(ss.str().c_str());

	SK::Result result = m_pimpl->m_controller.init(index, device, m_pimpl->m_bodyPartArray);
	if (!result.succeeded())
	{
		return SK::Result("Failed to initialise user controller.");
	}

	m_pimpl->m_bodyParts.resize(BodyPart::_COUNT);
	for(size_t i = 0; i < BodyPart::_COUNT; ++i)
	{
		m_pimpl->m_bodyParts[i].init(device, i,
									 m_pimpl->m_bodyPartArray,
									 m_pimpl->m_bodyPartConfidenceArray);
	}

	return SK::Result::NoError;
}

bool User::isActive() const
{
	return m_pimpl->m_active.get();
}

const SK::Array<SK::Vector3> &User::getCentroids() const
{
	return m_pimpl->m_centroids.get();
}

float User::getHeight() const
{
	return m_pimpl->m_height.get();
}

SK::Vector3 User::getMassCenter() const
{
	return m_pimpl->m_massCenter.get();
}

SK::Vector3 User::getMainDirection() const
{
	return m_pimpl->m_mainDirection.get();
}

SK::Easii::BoundingBox User::getBoundingBox() const
{
	return BoundingBox(m_pimpl->m_boundingBoxMin.get(), m_pimpl->m_boundingBoxMax.get());
}


int32_t User::getLabelID() const
{
	return m_pimpl->m_sceneObjectID.get();
}

const SK::Array<SK::Vertex> &User::getVertices() const
{
	return m_pimpl->m_vertexArray.get();
}

BodyPart &User::getBodyPart(BodyPartEnum::KeyPoints part)
{
	return m_pimpl->m_bodyParts[part];
}

BodyPart &User::getBodyPart(BodyPart::KeyPoint part)
{
	return m_pimpl->m_bodyParts[part];
}

bool User::hasSkeleton() const
{
	return (m_pimpl->m_skeletonStatus.get() == 1);
}

UserController& User::getController()
{
	return m_pimpl->m_controller;
}
} // namespace Easii
} // namespace SK
