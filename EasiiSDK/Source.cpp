/***************************************************************************************/
//  SoftKinetic iisu SDK
//  Project Name      : iisu SDK
//  Revision          : 3.0.2
//  Date              : 2012/01/31
//  Module Name       : Easii
//  Description       : Encapsulates the iisu source (either a camera
//                      or movie).
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

#include "Source.h"
#include <Iisu/Devices/Device.h>
#include <Foundation/DataTypes/Geometry/Vector3.h>
#include <Foundation/DataTypes/Geometry/Vector2.h>
#include <Foundation/DataTypes/Image/Image.h>
#include <Foundation/DataTypes/String/String.h>
#include <Iisu/Devices/DataExtractor.h>
#include <Foundation/Services/Events/BackgroundLearningEvent.h>


namespace SK {
namespace Easii {
struct Source::Impl
{
	void onOcclusion(SK::CameraOcclusionEvent event);
	void onDeocclusion(SK::CameraDeocclusionEvent event);

	SK::Device *m_device;

	DataHandle<SK::Image> m_depthMap;
	DataHandle<SK::Image> m_confidenceMap;
	DataHandle<SK::Image> m_color;

	ParameterHandle<int32_t> m_frameCount;
	ParameterHandle<int32_t> m_currentFrame;

	ParameterHandle<bool> m_confidenceThresholdFilterEnabled;
	ParameterHandle<int32_t> m_minConfidence;

	ParameterHandle<bool> m_reconstructionFilterEnabled;
	ParameterHandle<float> m_maxReconstruction;

	ParameterHandle<bool> m_depthFilterEnabled;
	ParameterHandle<float> m_minDepth;
	ParameterHandle<float> m_maxDepth;
	ParameterHandle<bool> m_isMovie;

	CameraInfo m_cameraInfo;
	MovieInfo m_movieInfo;

	bool m_occluded;

	Impl();
};

Source::Impl::Impl()
	: m_device(0)
	, m_occluded(false)
{
}

void Source::Impl::onOcclusion(SK::CameraOcclusionEvent event)
{
	m_occluded = true;
}

void Source::Impl::onDeocclusion(SK::CameraDeocclusionEvent event)
{
	m_occluded = false;
}

Source::Source()
	: m_pimpl(new Impl)
{
}

Source::~Source()
{
	delete m_pimpl;
}

SK::Result Source::init(SK::Device &device)
{
	m_pimpl->m_device = &device;
	m_pimpl->m_occluded = false;

	m_pimpl->m_isMovie = device.registerParameterHandle<bool>("SOURCE.IsMovie");

	m_pimpl->m_frameCount = device.registerParameterHandle<int32_t>("SOURCE.MOVIE.FrameCount");
    m_pimpl->m_currentFrame = device.registerParameterHandle<int32_t>("SOURCE.MOVIE.CurrentFrame");

	m_pimpl->m_confidenceThresholdFilterEnabled = device.registerParameterHandle<bool>("SOURCE.FILTER.CONFIDENCE.Enabled");
	m_pimpl->m_minConfidence = device.registerParameterHandle<int32_t>("SOURCE.FILTER.CONFIDENCE.Threshold");

	m_pimpl->m_reconstructionFilterEnabled = device.registerParameterHandle<bool>("SOURCE.FILTER.RECONSTRUCTION.Enabled");
	m_pimpl->m_maxReconstruction = device.registerParameterHandle<float>("SOURCE.FILTER.RECONSTRUCTION.Threshold");

	m_pimpl->m_depthFilterEnabled = device.registerParameterHandle<bool>("SOURCE.FILTER.DEPTH.Enabled");
	m_pimpl->m_minDepth = device.registerParameterHandle<float>("SOURCE.FILTER.MinDepth");
	m_pimpl->m_maxDepth = device.registerParameterHandle<float>("SOURCE.FILTER.MaxDepth");

	m_pimpl->m_depthMap = device.registerDataHandle<SK::Image>("SOURCE.CAMERA.DEPTH.Image");

	m_pimpl->m_confidenceMap = device.registerDataHandle<SK::Image>("SOURCE.CAMERA.CONFIDENCE.Image");

	m_pimpl->m_color = device.registerDataHandle<SK::Image>("SOURCE.CAMERA.COLOR.Image");

	SK::Result result =device.getEventManager().registerEventListener("SCENE.CAMERA.Occluded",*m_pimpl,&Source::Impl::onOcclusion);
	result =device.getEventManager().registerEventListener("SCENE.CAMERA.Deoccluded",*m_pimpl,&Source::Impl::onDeocclusion);

	result = m_pimpl->m_cameraInfo.init(device);
	if (!result.succeeded())
	{
		return result;
	}

	result = m_pimpl->m_movieInfo.init(device);
	return result;
}

bool Source::isMovie() const
{
	return
		const_cast<SK::ParameterHandle<bool> &>(m_pimpl->m_isMovie).get();
}

bool Source::isOccluded() const
{
	return m_pimpl->m_occluded;
}

CameraInfo &Source::getCameraInfo()
{
	return m_pimpl->m_cameraInfo;
}

MovieInfo &Source::getMovieInfo()
{
	return m_pimpl->m_movieInfo;
}

const SK::Image &Source::getDepthImage() const
{
	return const_cast<SK::DataHandle<SK::Image> &>(m_pimpl->m_depthMap).get();
}

bool Source::hasDepthImage() const
{
	return const_cast<SK::DataHandle<SK::Image> &>(m_pimpl->m_depthMap).isValid();
}

const SK::Image &Source::getConfidenceImage() const
{
	return const_cast<SK::DataHandle<SK::Image> &>(m_pimpl->m_confidenceMap).get();
}

bool Source::hasConfidenceImage() const
{
	return const_cast<SK::DataHandle<SK::Image> &>(m_pimpl->m_confidenceMap).isValid();
}

const SK::Image &Source::getColorImage() const
{
	return const_cast<SK::DataHandle<SK::Image> &>(m_pimpl->m_color).get();
}

bool Source::hasColorImage() const
{
	return const_cast<SK::DataHandle<SK::Image> &>(m_pimpl->m_color).isValid();
}

const SK::Image &Source::getRGBImage() const
{
	return getColorImage();
}

bool Source::hasRGBImage() const
{
	return hasColorImage();
}

int32_t Source::getFrame() const
{
	int32_t actualFrame;
	if (isMovie())
	{
		actualFrame = const_cast<SK::ParameterHandle<int32_t> &>(m_pimpl->m_currentFrame).get();
	}
	else
	{
		actualFrame = const_cast<SK::Device *>(m_pimpl->m_device)->getFrameId();
	}
	return actualFrame;
}

int32_t Source::getCurrentFrameIndex() const
{
	return getFrame();
}

void Source::enableConfidenceThresholdFilter(bool enabled)
{
	m_pimpl->m_confidenceThresholdFilterEnabled.set(enabled).ignoreError();
}

bool Source::isConfidenceThresholdFilterEnabled() const
{
	return m_pimpl->m_confidenceThresholdFilterEnabled.get();
}

void Source::setMinConfidenceThreshold(int32_t threshold)
{
	setMinimumConfidenceThreshold(threshold);
}

void Source::setMinimumConfidenceThreshold(int32_t threshold)
{
	m_pimpl->m_minConfidence.set(threshold);
}

int32_t Source::getMinimumConfidenceThreshold() const
{
	return m_pimpl->m_minConfidence.get();
}

void Source::enableReconstructionFilter(bool enabled)
{
	m_pimpl->m_reconstructionFilterEnabled.set(enabled).ignoreError();
}

bool Source::isReconstructionFilterEnabled() const
{
	return m_pimpl->m_reconstructionFilterEnabled.get();
}

void Source::setMaxReconstructionThreshold(float threshold)
{
	setMaximumReconstructionThreshold(threshold);
}

void Source::setMaximumReconstructionThreshold(float threshold)
{
	m_pimpl->m_maxReconstruction.set(threshold);
}

float Source::getMaximumReconstructionThreshold() const
{
	return m_pimpl->m_maxReconstruction.get();
}

void Source::enableDepthFilter(bool enabled)
{
	m_pimpl->m_depthFilterEnabled.set(enabled).ignoreError();
}

bool Source::isDepthFilterEnabled() const
{
	return m_pimpl->m_depthFilterEnabled.get();
}

void Source::setMinDepth(float depth)
{
	setMinimumDepth(depth);
}

void Source::setMinimumDepth(float depth)
{
	m_pimpl->m_minDepth.set(depth);
}

float Source::getMinimumDepth() const
{
	return m_pimpl->m_minDepth.get();
}

void Source::setMaxDepth(float depth)
{
	setMaximumDepth(depth);
}

void Source::setMaximumDepth(float depth)
{
	m_pimpl->m_maxDepth.set(depth);
}

float Source::getMaximumDepth() const
{
	return m_pimpl->m_maxDepth.get();
}
} // namespace Easii
} // namespace SK
