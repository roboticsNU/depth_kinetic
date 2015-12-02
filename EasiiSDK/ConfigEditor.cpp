/***************************************************************************************/
//  SoftKinetic iisu SDK
//  Project Name      : iisu SDK
//  Revision          : 3.0.2
//  Date              : 2012/01/31
//  Module Name       : Easii
//  Description       : Adapter around iisu's configuration.
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

#include "ConfigEditor.h"


namespace SK {
namespace Easii {
struct ConfigEditor::Impl
{
	SK::IisuHandle *m_handle;

	Impl();
};

ConfigEditor::Impl::Impl()
	: m_handle(0)
{
}

ConfigEditor::ConfigEditor()
	: m_pimpl(new Impl)
{
}

ConfigEditor::~ConfigEditor()
{
	delete m_pimpl;
}

SK::Result ConfigEditor::init(SK::IisuHandle &handle)
{
	m_pimpl->m_handle = &handle;
	return SK::Result::NoError;
}

void ConfigEditor::setString(const char *path, const char *value, bool copyToConfigEditorFile)
{
	SK::Result result = m_pimpl->m_handle->setConfigString(path, value, copyToConfigEditorFile);
	result.ignoreError();
}

void ConfigEditor::setValue(const char *path, const char *value)
{
	SK::Result result = m_pimpl->m_handle->setConfigString(path, value, false);
	result.ignoreError();
}

void ConfigEditor::saveValue(const char *path, const char *value)
{
	SK::Result result = m_pimpl->m_handle->setConfigString(path, value, true);
	result.ignoreError();
}

SK::String ConfigEditor::getValue(const char *path) const
{
	SK::Return<SK::String> result = m_pimpl->m_handle->getConfigString(path);
	return result.succeeded()? result.get() : SK::String();
}

int32_t ConfigEditor::getPathCount() const
{
	SK::Return<int32_t> result = m_pimpl->m_handle->getConfigPathCount();
	return result.succeeded()? result.get() : 0;
}

SK::String ConfigEditor::getPath(int32_t index) const
{
	SK::Return<SK::String> result = m_pimpl->m_handle->getConfigPath(index);
	return result.succeeded()? result.get() : SK::String();
}

void ConfigEditor::saveConfigFile()
{
	SK::Result result = m_pimpl->m_handle->saveConfigFile();
	result.ignoreError();
}
} // namespace Easii
} // namespace SK
