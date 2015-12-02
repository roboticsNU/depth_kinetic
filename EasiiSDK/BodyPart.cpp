/***************************************************************************************/
//  SoftKinetic iisu SDK
//  Project Name      : iisu SDK
//  Revision          : 3.0.2
//  Date              : 2012/01/31
//  Module Name       : Easii
//  Description       : Encapsulates a human body part (c.f. user
//                      skeleton), including it's position and it's
//                      confidence.
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

#include "BodyPart.h"
#include <Foundation/DataTypes/Skeleton/KeyPoints.h>

namespace SK {
namespace Easii {
struct BodyPart::Impl
{
	uint32_t m_index;
	DataHandle<SK::Array<SK::Vector3> >* m_bodyPartArray;
	DataHandle<SK::Array<float> >* m_bodyPartConfidenceArray;

	Impl();
};

BodyPart::Impl::Impl()
	: m_index(UINT32_MAX)
	, m_bodyPartArray(0)
	, m_bodyPartConfidenceArray(0)
{
}

BodyPart::BodyPart()
	: m_pimpl(new Impl)
{
}

BodyPart::~BodyPart()
{
	delete m_pimpl;
}

void BodyPart::init(Device& device,
					uint32_t index,
					DataHandle<SK::Array<SK::Vector3> >& bodyPartArray,
					DataHandle<SK::Array<float> >& bodyPartConfidenceArray)
{
	m_pimpl->m_bodyPartArray = &bodyPartArray;
	m_pimpl->m_bodyPartConfidenceArray = &bodyPartConfidenceArray;
	m_pimpl->m_index = index;
}

SK::Vector3 BodyPart::getPosition() const
{
	SK::Vector3 position;
	assert(m_pimpl->m_bodyPartArray != NULL);
	if(m_pimpl->m_bodyPartArray != NULL)
	{
		const SK::Array<SK::Vector3>& partValue = m_pimpl->m_bodyPartArray->get();
		if (m_pimpl->m_index < partValue.size()) {
			position = partValue[m_pimpl->m_index];
		}
	}
	return position;
}

float BodyPart::getConfidence() const
{
	float confidence = 0.0f;
	assert(m_pimpl->m_bodyPartConfidenceArray != NULL);
	if(m_pimpl->m_bodyPartConfidenceArray != NULL)
	{
		const SK::Array<float>& partConfidence = m_pimpl->m_bodyPartConfidenceArray->get();

		if (m_pimpl->m_index < partConfidence.size())
		{
			confidence = partConfidence[m_pimpl->m_index];
		}
	}
	return confidence;
}
} // namespace Easii
} // namespace SK
