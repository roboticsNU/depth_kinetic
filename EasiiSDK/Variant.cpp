/***************************************************************************************/
//  SoftKinetic iisu SDK
//  Project Name      : iisu SDK
//  Revision          : 3.0.2
//  Date              : 2012/02/29
//  Module Name       : Easii
//  Description       : A type safe variant.
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

#include "Variant.h"
#include <algorithm>

namespace SK {
namespace Easii {
Variant::Variant()
	: m_valueHolder(new Details::NoneHolder)
{
}

Variant::Variant(const char *value)
	: m_valueHolder(new Details::Holder<SK::String>(SK::String(value)))
{
}

Variant::Variant(const Variant &other)
	: m_valueHolder(other.m_valueHolder->clone())
{
}

Variant &Variant::operator = (const Variant &other)
{
	Variant(other).swap(*this);
	return *this;
}

Variant::~Variant()
{
	delete m_valueHolder;
}

void Variant::swap(Variant &other)
{
	std::swap(m_valueHolder, other.m_valueHolder);
}

bool Variant::isValid() const
{
	return m_valueHolder->rawData().isValid();
}

const SK::TypeInfo &Variant::getType() const
{
	return m_valueHolder->rawData().getType();
}
} // namespace Easii
} // namespace SK
