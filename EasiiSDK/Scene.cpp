/***************************************************************************************/
//  SoftKinetic iisu SDK
//  Project Name      : iisu SDK
//  Revision          : 3.0.2
//  Date              : 2012/01/31
//  Module Name       : Easii
//  Description       : Encapsulates the 3D scene.
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

#include "Scene.h"

namespace SK {
namespace Easii {
struct Scene::Impl
{
	SK::Array<User *> m_users;
	SK::Array<UIController *> m_controllers;
	SK::Array<Hand *> m_hands;

	Source m_source;
	Recorder m_recorder;
	Calibration m_calibration;
	Background m_background;

	int32_t m_maxUsers;
	int32_t m_maxControllers;
	int32_t m_maxHands;

	DataHandle<SK::Array<SK::Vertex> > m_vertexArrayCloud;
	DataHandle<SK::Image> m_labelImage;

	ParameterHandle<int> m_userCount;
	ParameterHandle<int> m_controllerCount;

	// activation gestures
	ParameterHandle<bool> m_circleEnabled, m_waveEnabled, m_pushEnabled;

	Impl();
	~Impl();

	void onClickDetected(SK::ClickGestureEvent e);
	void onSwipeDetected(SK::SwipeGestureEvent e);
	void onCircleDetected(SK::CircleGestureEvent e);
	void onWaveDetected(SK::WaveGestureEvent e);
	void onNumberDetected(SK::NumberGestureEvent e);
};

Scene::Impl::Impl()
	: m_maxUsers(4)
	, m_maxControllers(8)
	, m_maxHands(2)
{
}

Scene::Impl::~Impl()
{
	for (int32_t i = 0; i < m_users.size(); ++i)
	{
		delete m_users[i];
	}

	for (int32_t i = 0; i < m_controllers.size(); ++i)
	{
		delete m_controllers[i];
	}

	for (int32_t i = 0; i < m_hands.size(); ++i)
	{
		delete m_hands[i];
	}
}

void Scene::Impl::onClickDetected(SK::ClickGestureEvent e)
{
	int index = e.getRoiID() - 1;
	if ((index >= 0) && (index < m_controllers.size()))
	{
		m_controllers[index]->addGesture(Gesture(Gesture::CLICK, e.getPosition()));
	}
}

void Scene::Impl::onSwipeDetected(SK::SwipeGestureEvent e)
{
	int index = e.getRoiID() - 1;
	if ((index >= 0) && (index < m_controllers.size()))
	{
		m_controllers[index]->addGesture(Gesture(Gesture::SWIPE, e.getPosition()));
	}
}

void Scene::Impl::onCircleDetected(SK::CircleGestureEvent e)
{
	int index = e.getRoiID() - 1;
	if ((index >= 0) && (index < m_controllers.size()))
	{
		m_controllers[index]->addGesture(Gesture(Gesture::CIRCLE, e.getPosition()));
	}
}

void Scene::Impl::onNumberDetected(SK::NumberGestureEvent e)
{
	int index = e.getRoiID() - 1;
	if ((index >= 0) && (index < m_controllers.size()))
	{
		m_controllers[index]->addGesture(Gesture(Gesture::NUMBER, e.getPosition()));
	}
}

void Scene::Impl::onWaveDetected(SK::WaveGestureEvent e)
{
	int index = e.getRoiID() - 1;
	if ((index >= 0) && (index < m_controllers.size()))
	{
		m_controllers[index]->addGesture(Gesture(Gesture::WAVE, e.getPosition()));
	}
}

Scene::Scene()
	: m_pimpl(new Impl)
{
}

Scene::~Scene()
{
	delete m_pimpl;
}

namespace {
inline
int clamp(int x, int min, int max)
{
	return std::max(min, std::min(x, max));
}
} // namespace

SK::Result Scene::init(SK::Device &device)
{
	// Reset default config values
	m_pimpl->m_maxUsers = 4;
	m_pimpl->m_maxControllers = 8;
	m_pimpl->m_maxHands = 2;

	m_pimpl->m_vertexArrayCloud = device.registerDataHandle<SK::Array<SK::Vertex> >("SCENE.VertexArray");

	ParameterHandle<SK::String> ciEnabled = device.registerParameterHandle<SK::String>("CONFIG.PROCESSING.CI");
	if (ciEnabled.get() == "1")
	{
		m_pimpl->m_labelImage = device.registerDataHandle<SK::Image>("CI.SceneLabelImage");
	}
	else
	{
		m_pimpl->m_labelImage = device.registerDataHandle<SK::Image>("SCENE.LabelImage");
	}

	m_pimpl->m_userCount = device.registerParameterHandle<int>("UM.UserCount");
	m_pimpl->m_controllerCount = device.registerParameterHandle<int>("UI.ControllerCount");

	// Enable vertex generation
	ParameterHandle<bool> generateVertexArray = device.registerParameterHandle<bool>("SCENE.GenerateVertexArray");
	generateVertexArray.set(true).ignoreError();

	// Grab the maximum number of users from the config file
	ParameterHandle<SK::String> maxUsers = device.registerParameterHandle<SK::String>("CONFIG.OPTIONS.MAX_USERS");
	m_pimpl->m_maxUsers = clamp(atoi(maxUsers.get().ptr()), 1, m_pimpl->m_maxUsers);
	m_pimpl->m_users.clear();
	m_pimpl->m_users.resize(m_pimpl->m_maxUsers);
	for (uint32_t i = 0; i < m_pimpl->m_maxUsers; ++i)
	{
		User *user(new User);
		SK::Result result = user->init(i, device);
		if (!result.succeeded())
		{
			delete user;
			return result;
		}
		m_pimpl->m_users[i] = user;
	}

	ParameterHandle<SK::String> maxControllers = device.registerParameterHandle<SK::String>("CONFIG.OPTIONS.MAX_CONTROLLERS");
	m_pimpl->m_maxControllers = clamp(atoi(maxControllers.get().ptr()), 1, m_pimpl->m_maxControllers);
	m_pimpl->m_controllers.clear();
	m_pimpl->m_controllers.resize(m_pimpl->m_maxControllers);
	for (uint32_t i = 0; i < m_pimpl->m_maxControllers; ++i)
	{
		UIController *controller(new UIController);
		SK::Result result = controller->init(i, device);
		if (!result.succeeded())
		{
			delete controller;
			return result;
		}
		m_pimpl->m_controllers[i] = controller;
	}

	m_pimpl->m_hands.clear();
	m_pimpl->m_hands.resize(m_pimpl->m_maxHands);
	for (uint32_t i = 0; i < m_pimpl->m_maxHands; ++i)
	{
		Hand *hand(new Hand);
		SK::Result result = hand->init(i, device);
		if (!result.succeeded())
		{
			delete hand;
			return result;
		}
		m_pimpl->m_hands[i] = hand;
	}

	SK::Result result = m_pimpl->m_source.init(device);
	RETURN_MESSAGE_IF_FAILED(result,"Failed to initialize source");

	result = m_pimpl->m_recorder.init(device);
	RETURN_MESSAGE_IF_FAILED(result,"Failed to initialize recorder");

	result = m_pimpl->m_calibration.init(device);
	RETURN_MESSAGE_IF_FAILED(result,"Failed to initialize calibration");

	result = m_pimpl->m_background.init(device);
	RETURN_MESSAGE_IF_FAILED(result,"Failed to initialize background");

	// gestures
	m_pimpl->m_circleEnabled = device.registerParameterHandle<bool>("UI.ACTIVATIONGESTURE.CIRCLE.Enabled");
	m_pimpl->m_circleEnabled.set(true).ignoreError();
	m_pimpl->m_waveEnabled = device.registerParameterHandle<bool>("UI.ACTIVATIONGESTURE.WAVE.Enabled");
	m_pimpl->m_waveEnabled.set(false).ignoreError();
	m_pimpl->m_pushEnabled = device.registerParameterHandle<bool>("UI.ACTIVATIONGESTURE.PUSH.Enabled");
	m_pimpl->m_pushEnabled.set(false).ignoreError();

	device.getEventManager().registerEventListener("UI.CONTROLLERS.GESTURES.CLICK.Detected",*m_pimpl,&Scene::Impl::onClickDetected).ignoreError();
	device.getEventManager().registerEventListener("UI.CONTROLLERS.GESTURES.CIRCLE.Detected",*m_pimpl,&Scene::Impl::onCircleDetected).ignoreError();
	device.getEventManager().registerEventListener("UI.CONTROLLERS.GESTURES.SWIPE.Detected",*m_pimpl,&Scene::Impl::onSwipeDetected).ignoreError();
	device.getEventManager().registerEventListener("UI.CONTROLLERS.GESTURES.WAVE.Detected",*m_pimpl,&Scene::Impl::onWaveDetected).ignoreError();
	device.getEventManager().registerEventListener("UI.CONTROLLERS.GESTURES.NUMBER.Detected",*m_pimpl,&Scene::Impl::onNumberDetected).ignoreError();

	return Result::NoError;
}

int Scene::getMaxUserCount() const
{
	return getMaximumUserCount();
}

int Scene::getMaximumUserCount() const
{
	return m_pimpl->m_maxUsers;
}

void Scene::setUserCount(int userCount)
{
	m_pimpl->m_userCount.set(userCount).ignoreError();
}

int Scene::getUserCount() const
{
	return m_pimpl->m_userCount.get();
}

User &Scene::getUser(int index)
{
	return *m_pimpl->m_users[index];
}

int Scene::getMaxControllerCount() const
{
	return getMaximumControllerCount();
}

int Scene::getMaximumControllerCount() const
{
	return m_pimpl->m_maxControllers;
}

void Scene::setControllerCount(int controllerCount)
{
	m_pimpl->m_controllerCount.set(controllerCount).ignoreError();
}

int Scene::getControllerCount() const
{
	return m_pimpl->m_controllerCount.get();
}

Controller &Scene::getController(int index)
{
	return *m_pimpl->m_controllers[index];
}

int Scene::getMaximumHandCount() const
{
	return m_pimpl->m_maxHands;
}

int Scene::getHandCount() const
{
	return m_pimpl->m_maxHands;
}

Hand &Scene::getHand(int index)
{
	return *m_pimpl->m_hands[index];
}

Source &Scene::getSource()
{
	return m_pimpl->m_source;
}

Background &Scene::getBackground()
{
	return m_pimpl->m_background;
}

Calibration &Scene::getCalibration()
{
	return m_pimpl->m_calibration;
}

Recorder &Scene::getRecorder()
{
	return m_pimpl->m_recorder;
}

const SK::Array<SK::Vertex> &Scene::getCloud() const
{
	return m_pimpl->m_vertexArrayCloud.get();
}

const SK::Image &Scene::getLabelImage() const
{
	return m_pimpl->m_labelImage.get();
}
} // namespace Easii
} // namespace SK
