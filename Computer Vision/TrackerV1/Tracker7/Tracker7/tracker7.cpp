//Tracker7.cpp

// via: youtube.com/watch?v=2i2bt-YSlYQ

#include<opencv/cvaux.h>
#include<opencv/highgui.h>
#include<opencv/cxcore.h>

#include<stdio.h>
#include<stdlib.h>

//////////////////////////////////////////
int main(int argc, char* argv[]) {

	CvSize size640x480 = cvSize(640,480);  // use a 640x480 size for all window, also make sure your webcam is set to 640x480 settings

	CvCapture* p_capwebcam; //we sill assign our web cam video stream to this later...

	IplImage* p_imgOriginal;		// pointer to an image structure, this will be the input image from the webcam
	IplImage* p_imgProcessed;		// pointer to an image structure. This will be the processed image from the webcam
									/* IPL short for Intel Image Processing Library, this is the standard structure used in OpenCV1 to work with images */
	CvMemStorage* p_strStorage;		// necessary storagte vairable to pass into cvHoughCircles()

	CvSeq* p_seqCircles;			// pointer to an OpenCV sequence, will be returned by cvHoughCircles() and will contain all circles
									// Call cvGetSeqE len(p_seqCircles, i) will return a 3 element of the ith circle (see next variable)

	float* p_fltXYRadius;			//pointer to a 3 element array of floats
									//[0] => x position of detected object
									//[1] => y position of detected object
									//[2] => radius of detected object

	int i;							// loop counter
	char charCheckForEscKey;		// char for checking key press (Esc exits program)

	p_capWebcam = cvCapturesFromCAM(0); //index indicates the number of webcams installed. 0 should refer to built in  cam

	if(p_capWebcam == NULL){			// ifcapture was not successful...
		printf("error: capture is NULL. No camera detected \n"); //error message to standard out...
		getchar();						// getchar() to pause for user see message...
		return(-1);						// exit  program
	}

										// declare 2 windows
	cvNamedwindow("Original", CV_WINDOW_AUTOSIZE); //Original image from webcam
	cvNamedwidow("Processed", CV_WINDOW_AUTOSIZE); //the processed image we will use for detecting the surface

	p_imgProcesse cvCreateImage(size640x480, // 640 x 480 pixels (CvSize struct from earlier)
								IPL_DEPTH_8U,	// 8 - bit color depth
								1);				// 1 channel (grayscale). if this was a color image, then we would use 3.

	while(1) {									// for each frame ...
		p_imgOriginal = cvQueryFrame(p_capWebcam);		// get frame from webcam

		if(p_imgOriginal == NULL) { //if frame was not captured successfully....
			printf("error: frame is NULL \n"); // error messate to std out
			getchar();
			break;
		}

		cvInRangeS(p_imgOriginal, //function input
					CV_RGB(175, 0, 0), //min filtering RGB values adjustment for future colors
					CV_RGB(256,100,100),	//max filtering
					p_imgProcessed);		//function output
		p_strStorage = cvCreateMemStorate(0);		// allocate necessary memory storage variable to pass to cv HoughCirles

									// smooth the  processed image, this will make it easier for the next function to pick out the circles
		cvSmooth(p_imgProcessed,		// function input
				p_imgProcessed,			// function  output
				CV_GAUSSIAN,			// use_Gaussian filter (average nearby pixels, with the closest pixels weighted more
				9,						// smoothing filter window width
				9);					// smoothing filter window height

										//fill sequential structure with all circles in processed image
		p_seqCircles = cvHoughCircles(p_imgProcessed(p_imgProcessed,	// input image,note that this has to be grayscale (no color)
										p_strStorage,					// provide function with memorystorage, make the function return a  pointer to a CvSeq
										CV_HOUGH_GRADIENT,				//two-pass algorithm for detecting circles, only choice available for this parameter
										2,								// size of the image divided by this value gives accumulator resolution = size of the image divided by 2
										p_imgProcessed -> height / 4	// minimum distance in pixels between the centers of the detected circles, prevents multiple detections of one object
										100,							// height threshold of canny edge detector, called by cvHoughCircles 
										50,								// low threshold of canny edge detector --> change this value for more less circular detections
										10,								// min circle radius, (in pixels)
										400);							// max circle radius, (in pixels)
		
		for(i=0; i < p_seqCircles->total; i++) { //for each element in sequential circles structures (i.e. for each object detected)
			p_fltXYRadius = (float*)cvGetSeqElem(p_seqCircle, i);		// from the sequential structures, read the ith value into a pointer from the float
			printf("ball position x= %f, y= %f, r= %f \n", p_fltXYRadius[0], //x position of the center point of the circle
															p_fltXYRadius[1],	// y position of the center point of the circle
															p_fltXYRadius[2]);	//radius of the circle

							// draw a small green circle at center of detected object
			cvCircle(p_imgOriginal,			// draw on the original image
					cvPoint(cvRound(p_fltXYRadius[0]), cvRound(p_fltXYRadius[1])),		// center point of circle
					3,																	// 3 pixel radius of circle
					CV_RGB(0,225,0),													// draw pure green
					CV_FILLED);															// thickness, fill in the circle
																						// draw a red circle around the detected object
			cvCircle(p_imgOriginal,														// draw on the original image
					cvPoint(cvRound(p_fltXYRadius[0]), cvRound(p_fltXYRadius[1])),		// copy and paste from above
					cvRound(p_fltXYRadius[2]),											// radius of circle in pixels
					CV_RGB(255,0,0),													// draw pure red
					3);																	// thickness of circle in pixels
		} // end for

		cvShowImage("Original", p_imgOriginal);				// original image with the detected ball overlay
		cvShowImage("Processed", p_imgProcessed);	// image after processing

		cvReleaseMeamStorage(&p_strStorage);		// deallocate necessary storage variable to pass into CV HoughCircles

		charCheckForEscKey = cvwaitKey(10);			// delay (in ms) , and get key press, if any

		if(charCheckForEscKey == 27) break;			// if the Esckey (ASCII 27) was pressed, jump out of while loop
	}	//end while

	cvReleaseCapture(&p_capWebcam);				//release memory as  applicable

	cvDestroyWindow("Original");
	cvDestorywindow("Processed");

	return(0);
	}