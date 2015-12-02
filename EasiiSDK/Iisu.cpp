/***************************************************************************************/
//  SoftKinetic iisu SDK
//  Project Name      : iisu SDK
//  Revision          : 3.0.2
//  Date              : 2012/01/31
//  Module Name       : Easii
//  Description       : The main controller object.
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

#include "Iisu.h"
#include <SDK/Context/Context.h>
#include <Iisu/Config/ApplicationConfigurator.h>
#include "BufferedVector.h"

namespace SK {
namespace Easii {
struct Iisu::Impl
{
	SK::Context &m_context;

	SK::Device* m_device;
	SK::IisuHandle* m_handle;

	bool m_isSlave;
	bool m_fatal;

	SK::Error m_fatalError;

	Scene m_scene;
	ConfigEditor m_configEditor;

	bool m_iisuRunningFlag;

	Utils::BufferedVector<SK::Error> m_errorList;

	Impl();

	SK::Result initDataStructure();
	void onError(SK::ErrorEvent evt);
	void onDeviceStatusChanged(SK::StatusEvent evt);
};

Iisu::Impl::Impl()
	: m_context(Context::Instance())
	, m_device(static_cast<SK::Device *>(0))
	, m_handle(static_cast<SK::IisuHandle *>(0))
	, m_isSlave(false)
	, m_iisuRunningFlag(true) // it's only after resetting we really want to re-init
	, m_fatal(false)
{
}

SK::Result Iisu::Impl::initDataStructure()
{
	SK::Result result = m_scene.init(*m_device);
	RETURN_MESSAGE_IF_FAILED(result, "Failed to initialise scene");

	result = m_configEditor.init(*m_handle);
	RETURN_MESSAGE_IF_FAILED(result, "Failed to initialise config");

	// Flip the list so we can get initialisation errors
	m_errorList.flip();
	return SK::Result::NoError;
}

void Iisu::Impl::onError(SK::ErrorEvent evt)
{
	if (evt.isFatal() && (!m_fatal))
	{
		m_fatal = true;
		m_fatalError = evt.getError();
	}
	else
	{
		m_errorList.pushBack(evt.getError());
	}
}

void Iisu::Impl::onDeviceStatusChanged(SK::StatusEvent evt)
{
	bool iisuRunningFlag = (evt.getDeviceStatus() & SK::DeviceStatus::PLAYING) != 0;
	if (iisuRunningFlag != m_iisuRunningFlag)
	{
		m_iisuRunningFlag = iisuRunningFlag;
		if (m_iisuRunningFlag)
		{
			initDataStructure().ignoreError();
		}
	}
}

Iisu::Iisu()
	: m_pimpl(new Impl)
{
}

Iisu::~Iisu()
{
	delete m_pimpl;
}

Iisu &Iisu::instance()
{
	static Iisu instance;
	return instance;
}

SK::Result Iisu::init()
{
	SK::String iisuBinDir;
	const char *iisuBinDirEnv = std::getenv("IISU_BIN_DIR");
	if (iisuBinDirEnv)
	{
		iisuBinDir = iisuBinDirEnv;
	}
	else
	{
		const char *iisuSdkDirEnv = std::getenv("IISU_SDK_DIR");
		if (iisuSdkDirEnv)
		{
			iisuBinDir = iisuSdkDirEnv;
			iisuBinDir += "\\bin";
		}
	}

	return init(iisuBinDir.ptr(), "iisu_config.xml");
}

SK::Result Iisu::init(const char *configFilePath)
{
	if (m_pimpl->m_handle)
	{
		return SK::Result("An iisu handle already exists.");
	}

	SK::Return<SK::ApplicationConfigurator> retconfig = SK::ApplicationConfigurator::create(configFilePath);
	RETURN_MESSAGE_IF_FAILED(retconfig,"Failed to get application configurator");

	SK::ApplicationConfigurator configurator=retconfig.get();

	SK::Return<SK::IisuHandle *> handle = m_pimpl->m_context.createHandle(configurator);
	RETURN_MESSAGE_IF_FAILED(handle, "Failed to get iisu handle");

	SK::Return<SK::Device *> device = handle.get()->initializeDevice(configurator);
	if (!device.succeeded())
	{
		m_pimpl->m_context.destroyHandle(*handle.get()).ignoreError();
		return SK::Result(device);
	}

	SK::Result result = initFromHandle(*handle.get());
	if (!result.succeeded())
	{
		m_pimpl->m_context.destroyHandle(*handle.get()).ignoreError();
	}
	return result;
}

SK::Result Iisu::init(const char *iisuBinDir, const char *iisuConfigFileName)
{
	if (m_pimpl->m_handle)
	{
		return SK::Result("An iisu handle already exists.");
	}

	SK::IisuHandle::Configuration config(iisuBinDir, iisuConfigFileName, "param_settings.xml",  "", false);
	config.isLocal = true;

	SK::Return<SK::IisuHandle *> handle = m_pimpl->m_context.createHandle(config);
	RETURN_MESSAGE_IF_FAILED(handle, "Failed to get iisu handle");

	SK::Result result = initFromHandle(*handle.get());
	if (!result.succeeded())
	{
		m_pimpl->m_context.destroyHandle(*handle.get()).ignoreError();
	}
	return result;
}

SK::Result Iisu::initFromHandle(SK::IisuHandle &handle, bool isSlave /*= false*/)
{
	if (m_pimpl->m_handle)
	{
		return SK::Result("An iisu handle already exists.");
	}

	RETURN_IF_FAILED(handle.getEventManager().registerEventListener("SYSTEM.Error", *m_pimpl, &Iisu::Impl::onError));

	// determine that this is a slave if the handle does not already
	// have a device.
	m_pimpl->m_isSlave = isSlave;
	if (!handle.isDeviceInitialized())
	{
		SK::Return<SK::Device *> device = handle.initializeDevice(SK::Device::Configuration());
		RETURN_MESSAGE_IF_FAILED(device, "Unable to initialize device");
	}

	SK::Return<SK::Device *> device = handle.getDevice();
	RETURN_MESSAGE_IF_FAILED(device, "Invalid handle");

	RETURN_IF_FAILED(handle.getEventManager().registerEventListener("DEVICE.Status", *m_pimpl, &Iisu::Impl::onDeviceStatusChanged));

	m_pimpl->m_handle = &handle;
	m_pimpl->m_context.registerHandle(handle);
	m_pimpl->m_device = device.get();

	return m_pimpl->initDataStructure();
}

void Iisu::close()
{
	shutdown();
}

void Iisu::shutdown()
{
	if (m_pimpl->m_handle)
	{
		if (!m_pimpl->m_isSlave)
		{
			SK::Result res = m_pimpl->m_context.destroyHandle(*m_pimpl->m_handle);
			if (res.failed())
			{
				m_pimpl->m_errorList.pushBack(res.getError());
			}
		}
		m_pimpl->m_device = static_cast<SK::Device *>(0);
		m_pimpl->m_handle = static_cast<SK::IisuHandle *>(0);
		m_pimpl->m_fatal = false;
		m_pimpl->m_iisuRunningFlag = true;
	}
}

SK::String Iisu::getVersion() const
{
	SK::Return<SK::String> version = m_pimpl->m_handle->getVersion();
	return version.succeeded()? version.get() : SK::String();
}

bool Iisu::isLayerEnabled(const char *layerName) const
{
	SK::String path = SK::String("/CONFIG/PROCESSING/") + layerName;
	return m_pimpl->m_configEditor.getValue(path.ptr()) == "1";
}

SK::Result Iisu::start()
{
	if (!m_pimpl->m_device)
	{
		return SK::Result("Device is not initialized.");
	}
	return m_pimpl->m_device->start();
}

void Iisu::stop()
{
	if ((m_pimpl->m_device != NULL) &&
		((m_pimpl->m_device->getStatus() & SK::Device::PLAYING) != 0))
	{
		SK::Result result = m_pimpl->m_device->stop();
		if (!result.succeeded())
		{
			m_pimpl->m_errorList.pushBack(result.getError());
		}
	}
}

bool Iisu::update()
{
	assert(m_pimpl->m_device != NULL);
	bool updated = false;
	if (!m_pimpl->m_fatal)
	{
		if ((m_pimpl->m_device->getStatus() & SK::Device::PLAYING) != 0)
		{
			SK::Result result = m_pimpl->m_device->updateFrame(true, 5);
			updated = result.succeeded();
			if (!updated)
			{
				m_pimpl->m_errorList.pushBack(result.getError());
			}
		}
		m_pimpl->m_errorList.flip();
	}
	return (!m_pimpl->m_fatal && updated);
}

bool Iisu::updateData()
{
	return updateAndAcquire();
}

bool Iisu::updateAndAcquire()
{
	assert(m_pimpl->m_device != NULL);
	bool updated = update();
	if (updated)
	{
		acquire();
	}
	return updated;
}

bool Iisu::acquire()
{
	assert(m_pimpl->m_device != NULL);
	m_pimpl->m_device->lockFrame();
	return true;
}

void Iisu::releaseData()
{
	release();
}

void Iisu::release()
{
	assert(m_pimpl->m_device != NULL);
	m_pimpl->m_device->releaseFrame();
}

const SK::Array<SK::Error> &Iisu::getErrors() const
{
	return m_pimpl->m_errorList.getReadArray();
}

const SK::Array<SK::Error> &Iisu::getErrorStack() const
{
	return getErrors();
}

Scene &Iisu::getScene()
{
	return m_pimpl->m_scene;
}

ConfigEditor &Iisu::getConfigEditor()
{
	return m_pimpl->m_configEditor;
}

Device *Iisu::getDevice()
{
	return m_pimpl->m_device;
}

const Device *Iisu::getDevice() const
{
	return m_pimpl->m_device;
}

IisuHandle *Iisu::getHandle()
{
	return m_pimpl->m_handle;
}

const IisuHandle *Iisu::getHandle() const
{
	return m_pimpl->m_handle;
}

bool Iisu::hasFatalError() const
{
	return m_pimpl->m_fatal;
}

SK::Error Iisu::getFatalError() const
{
	return m_pimpl->m_fatalError;
}
} // namespace Easii
} // namespace SK
