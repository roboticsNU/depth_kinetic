/***************************************************************************************/
//  SoftKinetic iisu SDK
//  Project Name      : iisu SDK
//  Revision          : 3.0.2
//  Date              : 2012/01/31
//  Module Name       : Easii
//  Description       : Simple adapter around iisu's background
//                      learning data.
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

#include "Background.h"

namespace SK {
namespace Easii {
struct Background::Impl
{
	bool m_takingSnapshot;
	SK::CommandHandle<SK::Result(const SK::Enum&)> m_takeSnapshot;
	ParameterHandle<SK::Enum> m_backgroundRemovalMode;

	void onBackgroundSnapshotEvent(SK::BackgroundSnapshotResultEvent event);

	Impl();
};

Background::Impl::Impl()
	: m_takingSnapshot(false)
{
}

void Background::Impl::onBackgroundSnapshotEvent(SK::BackgroundSnapshotResultEvent)
{
	m_takingSnapshot = false;
}

Background::Background()
	: m_pimpl(new Impl)
{
}

Background::~Background()
{
	delete m_pimpl;
}

SK::Result Background::init(SK::Device &device)
{
	m_pimpl->m_takeSnapshot =  device.getCommandManager().registerCommandHandle<SK::Result (const SK::Enum&)>("SCENE.BACKGROUND.TakeSnapshot");
	device.getEventManager().registerEventListener("SCENE.BACKGROUND.SnapshotDone",*m_pimpl,&Background::Impl::onBackgroundSnapshotEvent).ignoreError();
	m_pimpl->m_backgroundRemovalMode = device.registerParameterHandle<SK::Enum>("SCENE.BACKGROUND.Mode");
	return SK::Result::NoError;
}

void Background::setEnabled(bool)
{
}

bool Background::isEnabled() const
{
	return false;
}

void Background::setRemovalMode(RemovalMode mode)
{
	m_pimpl->m_backgroundRemovalMode.set(mode).ignoreError();
}

Background::RemovalMode Background::getRemovalMode() const
{
	return static_cast<RemovalMode>(m_pimpl->m_backgroundRemovalMode.get().value);
}

SK::Result Background::takeSnapshot()
{
	SK::Result result = m_pimpl->m_takeSnapshot(SK::SnapshotModeEnum::FULL);
	m_pimpl->m_takingSnapshot = result.succeeded();
	return result;
}

bool Background::isTakingSnapshot() const
{
	return m_pimpl->m_takingSnapshot;
}
} // namespace Easii
} // namespace SK
