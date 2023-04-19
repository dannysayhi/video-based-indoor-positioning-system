/*

#include <cv.h>
#include <highgui.h>
#include <stdio.h>

int main()
{
    CvCapture *capture;
    IplImage *frame;

    capture=cvCreateCameraCapture(0);
    //cvNamedWindow("Webcam",0);

    CvVideoWriter *writer;
    char AviFileName[]="Output.avi";
    int AviForamt = -1;
    int FPS = 25;
    CvSize AviSize = cvSize(640,480);
    int AviColor = 1;
    writer=cvCreateVideoWriter(AviFileName,AviForamt,FPS,AviSize,AviColor);
	//printf("start!");
    int i=0;
    while(true)
    {
        frame = cvQueryFrame(capture);
        cvWriteFrame(writer,frame);

        cvShowImage("Webcam",frame);
        printf("%d\n",i);

        if(cvWaitKey(20)>0)     break;
        i++;
    }

    cvReleaseCapture(&capture);
    cvReleaseVideoWriter(&writer);
    cvDestroyWindow("Webcam");
}
*/