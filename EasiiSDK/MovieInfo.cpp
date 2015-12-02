/***************************************************************************************/
//  SoftKinetic iisu SDK
//  Project Name      : iisu SDK
//  Revision          : 3.0.2
//  Date              : 2012/01/31
//  Module Name       : Easii
//  Description       : Encapsulates information about the current
//                      movie.
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

#include "MovieInfo.h"
#include <Iisu/Devices/DataExtractor.h>

namespace SK {
namespace Easii {
struct MovieInfo::Impl
{
	SK::ParameterHandle<SK::String> m_name;
	SK::ParameterHandle<int32_t> m_frameCount;
};

MovieInfo::MovieInfo()
	: m_pimpl(new Impl)
{
}

MovieInfo::~MovieInfo()
{
	delete m_pimpl;
}

SK::Result MovieInfo::init(SK::Device &device)
{
	m_pimpl->m_name = device.registerParameterHandle<SK::String>("SOURCE.MOVIE.Name");
	m_pimpl->m_frameCount = device.registerParameterHandle<int32_t>("SOURCE.MOVIE.FrameCount");
	return SK::Result::NoError;
}

SK::String MovieInfo::getName() const
{
	return m_pimpl->m_name.get();
}

int32_t MovieInfo::getFrameCount() const
{
	return m_pimpl->m_frameCount.get();
};
} // namespace Easii
} // namespace SK
