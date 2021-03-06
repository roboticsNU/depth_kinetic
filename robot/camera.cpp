#include "camera.h"
#include <iostream> 
#include "EasiiSDK/Iisu.h" 

#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/highgui/highgui_c.h" 
#include "opencv2/video/video.hpp"
#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/videoio.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2\core\core.hpp>
#include<time.h>   
#include <stdio.h>
#include <math.h>
#include <queue>
#include <Windows.h>

using namespace SK::Easii;  
extern bool volatile canstart;
extern bool volatile finished;
extern bool volatile onlySerial;
extern int volatile iterationNumber;

std::queue<cv::Mat> frames;
std::queue<cv::Mat> framesConf;

int camera() {  

	if (onlySerial) {
		return -2;
	}

	SK::Result result;
	SK::Easii::Iisu &iisu = SK::Easii::Iisu::instance();

	// First, we initialize iisu using the default config file.
	const char *env = std::getenv("IISU_SDK_DIR");
	if (!env)
	{
		std::cerr << "IISU_SDK_DIR environment variable was not set." << std::endl;
		return EXIT_FAILURE;
	}

	SK::String iisuBinDir = SK::String(env) + "\\bin";
	result = iisu.init(iisuBinDir.ptr(), "iisu_config.xml");
	if (!result.succeeded())
	{
		std::cerr << result.getDescription().ptr() << std::endl;
		return result.getErrorCode();
	}

	// Once iisu is successfully initialized, we can start it asynchronously.
	result = iisu.start();
	if (!result.succeeded())
	{
		std::cerr << result.getDescription().ptr() << std::endl;
		iisu.close();
		return result.getErrorCode();
	}
	
	CvVideoWriter* vidWriterColored = 0;
	// Now we're ready to do stuff!  But this is for a later tutorial. 
	 
	clock_t t1, t2;

	t1 = clock();

	float thresholdTime = 10 * 60;
	float diff = 0;
	canstart = true;
	printf("CAMERA: camera starting\n");
	while (!finished) {
		/*diff = (float)clock() - (float)t1;
		diff /= CLOCKS_PER_SEC;
		int key = cvWaitKey(10);
		if (key == 27) {
			break;
		}*/

		/*
		SK::Image DepthImg = source.getDepthImage();
		uint32_t height = DepthImg.getImageinfos().height;
		uint32_t width = DepthImg.getImageInfos().width;
		*/ 
		
		if (iisu.update())
		{
			iisu.acquire();
			SK::Easii::Scene &scene = iisu.getScene();
			scene.getSource().enableDepthFilter(true);
			SK::Image DepthImg = scene.getSource().getDepthImage();
			SK::Image colored = scene.getSource().getColorImage();
			SK::Image confidenceIm = scene.getSource().getConfidenceImage();

			uint32_t height = DepthImg.getImageInfos().height;
			uint32_t width = DepthImg.getImageInfos().width;
			//uint8_t *rawData = DepthImg.getRAW();
			uint8_t *coloredRawData = colored.getRAW();
			//uint16_t *confImRaw= (uint16_t *)confidenceIm.getRAW();

			SK::ImageInfos rawImInfo = DepthImg.getImageInfos();
			SK::ImageInfos colImInfo = colored.getImageInfos();
			SK::ImageInfos confidenceInfo = confidenceIm.getImageInfos();

			/*IplImage *rawImg1 = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
			IplImage *rawImg2 = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);*/
			IplImage *rawImg3 = cvCreateImage(cvSize(colImInfo.width, colImInfo.height), IPL_DEPTH_8U, 3);
			IplImage *confIm = cvCreateImage(cvSize(confidenceInfo.width, confidenceInfo.height), IPL_DEPTH_16S, 1);
			/*cvZero(rawImg1);
			cvZero(rawImg2);*/
			//cvZero(rawImg3);
			//cvZero(confIm);

			if (vidWriterColored == 0) {
				//vidWriterDepth = cvCreateVideoWriter("C:\\depth_vid.avi", -1, 30 , cvSize(rawImg1->width, rawImg1->height), 0);
				vidWriterColored = cvCreateVideoWriter("data\\colored_vid.avi", CV_FOURCC('M', 'J', 'P', 'G'), 30 , cvSize(rawImg3->width, rawImg3->height), 1);
				//vidWriterConfidence = cvCreateVideoWriter("confidence_vid.avi", CV_FOURCC('M', 'J', 'P', 'G'), 30 , cvSize(confIm->width, confIm->height), 0);
			}  
			 
			int rawWidthStep = 2 * width;
			int coloredWidthStep = 4 * colImInfo.width;

			for (int i = 0; i < colImInfo.height; ++i) {				
				uchar *coloredPtr = (uchar*)(rawImg3->imageData + i * rawImg3->widthStep);
				
				for (int j = 0; j < colImInfo.width; ++j) {
					coloredPtr[3 * j + 0] = coloredRawData[(i) * coloredWidthStep + (4 * j + 0)];
					coloredPtr[3 * j + 1] = coloredRawData[(i) * coloredWidthStep + (4 * j + 1)];
					coloredPtr[3 * j + 2] = coloredRawData[(i) * coloredWidthStep + (4 * j + 2)];
					
				}
			} 
			

			/*for (int i = 0; i < height; ++i) {
				uchar *rawImgPtr1 = (uchar*)(rawImg1->imageData + i * rawImg1->widthStep);
				uchar *rawImgPtr2 = (uchar*)(rawImg2->imageData + i * rawImg2->widthStep);
				for (int j = 0; j < width; ++j) {
					int rawVal1 = rawData[i * rawWidthStep + (2 * j + 0)];
					int rawVal2 = rawData[i * rawWidthStep + (2 * j + 1)];
					rawImgPtr1[j] = rawVal1;
					rawImgPtr2[j] = rawVal2;			
				}
			}*/
			cv::Mat depth (DepthImg.getImageInfos().height, DepthImg.getImageInfos().width, CV_16U, (void*)(const uint16_t*)DepthImg.getRAW());
			frames.push(depth.clone());
			/*
			for (int i = 0; i < confidenceInfo.height; ++i) { 
				short *confImPtr = (short *)(confIm->imageData + i * confIm->widthStep);
				for (int j = 0; j < confidenceInfo.width; ++j) { 
					confImPtr[j] = confImRaw[i * confidenceInfo.width + j];
				}
			}*/
			 /*
			cvSmooth(rawImg1, rawImg1);
			cvSmooth(rawImg2, rawImg2);
			*/
			cvSmooth(rawImg3, rawImg3);
			 
			//cvShowImage("Depth image", rawImg1);
			//cvShowImage("Confidence image", confIm);
			//cvShowImage("Colored", rawImg3);
			cvWaitKey();
			//cvWriteFrame(vidWriterDepth, rawImg1);
			//cvWriteFrame(vidWriterConfidence, confIm);
			int stat = cvWriteFrame(vidWriterColored, rawImg3);

			cv::Mat conf (confidenceIm.getImageInfos().height, confidenceIm.getImageInfos().width, CV_16U, (void*)(const uint16_t*)confidenceIm.getRAW());
			framesConf.push(conf.clone());

			/*cvReleaseImage(&rawImg1);	
			cvReleaseImage(&rawImg2);*/
			cvReleaseImage(&rawImg3);	
			//cvReleaseImage(&confIm);	
			// tell iisu we finished using data.
			iisu.release(); 
		}
	} 
	// Stop iisu once we're finished.
	iisu.stop();

	// Clean up.
	iisu.close(); 
	int num = 0;
	char depthFileName[256];
	while (!frames.empty()) {
		cv::Mat fr = frames.front();
		sprintf(depthFileName, "data/depthsense%d/depth/depth%05d.png", iterationNumber, num++);
		cv::imwrite(depthFileName, fr);
		frames.pop();
	} 
		
	num = 0;
	while (!framesConf.empty()) {
		cv::Mat fr = framesConf.front();
		sprintf(depthFileName, "data/depthsense%d/conf/conf%05d.png", iterationNumber, num++);
		cv::imwrite(depthFileName, fr);
		framesConf.pop();
	} 

	/*cvReleaseVideoWriter(&vidWriterDepth);
	cvReleaseVideoWriter(&vidWriterConfidence);*/
	cvReleaseVideoWriter(&vidWriterColored);
	
	printf("CAMERA FINISHED\n");
	return 0; 
}
