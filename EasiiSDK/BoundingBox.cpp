/***************************************************************************************/
//  SoftKinetic iisu SDK
//  Project Name      : iisu SDK
//  Revision          : 3.0.2
//  Date              : 2012/01/31
//  Module Name       : Easii
//  Description       : Defines the parameters of a bounding box in 3D
//                      space.
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

#include "BoundingBox.h"

namespace SK {
namespace Easii {
BoundingBox::BoundingBox(const SK::Vector3 &min,const SK::Vector3 &max)
	: m_min(min)
	, m_max(max)
{
}

BoundingBox::BoundingBox(const BoundingBox& other)
	: m_min(other.m_min)
	, m_max(other.m_max)
{
}

BoundingBox &BoundingBox::operator = (const BoundingBox &other)
{
	BoundingBox(other).swap(*this);
	return *this;
}

BoundingBox::~BoundingBox()
{
}

void BoundingBox::swap(BoundingBox &other)
{
	std::swap(m_min, other.m_min);
	std::swap(m_max, other.m_max);
}

SK::Vector3 BoundingBox::getMin() const
{
	return getMinimum();
}

SK::Vector3 BoundingBox::getMinimum() const
{
	return m_min;
}

SK::Vector3 BoundingBox::getMax() const
{
	return getMaximum();
}

SK::Vector3 BoundingBox::getMaximum() const
{
	return m_max;
}
} // namespace Easii
} // namespace SK
