/***************************************************************************************/
//  SoftKinetic iisu SDK
//  Project Name      : iisu SDK
//  Revision          : 3.0.2
//  Date              : 2012/01/31
//  Module Name       : Easii
//  Description       : Provides a simplified mechanism to stop and
//                      start recording movies.
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

#include "Recorder.h"
#include <Iisu/Devices/DataExtractor.h>
#include <Foundation/Services/Commands/Command.h>

namespace SK {
namespace Easii {
struct Recorder::Impl
{
	SK::CommandHandle<SK::Result(const SK::String&)> m_start;
	SK::CommandHandle<SK::Result(void)> m_stop;

	SK::ParameterHandle<bool> m_isRecording;

	SK::String m_path;
};

Recorder::Recorder()
	: m_pimpl(new Impl)
{
}

Recorder::~Recorder()
{
	delete m_pimpl;
}

SK::Result Recorder::init(SK::Device &device)
{
	m_pimpl->m_start = device.getCommandManager().registerCommandHandle<SK::Result (const SK::String&)>("SOURCE.RECORDING.Start");
	m_pimpl->m_stop = device.getCommandManager().registerCommandHandle<SK::Result (void)>("SOURCE.RECORDING.Stop");

	m_pimpl->m_isRecording = device.registerParameterHandle<bool>("SOURCE.RECORDING.IsRecording");

	return SK::Result::NoError;
}

SK::Result Recorder::start()
{
	return m_pimpl->m_start(m_pimpl->m_path.ptr());
}

SK::Result Recorder::start(const SK::String &fileName)
{
	return m_pimpl->m_start(fileName.ptr());
}

SK::Result Recorder::stop()
{
	return m_pimpl->m_stop();
}

void Recorder::setPath(const SK::String &path)
{
	m_pimpl->m_path = path;
}

bool Recorder::isRecording() const
{
	return m_pimpl->m_isRecording.get();
}
} // namespace Easii
} // namespace SK
