#include <cv.h>
#include <highgui.h>

int main( int argc, char** argv )
{
        //display the original image
        IplImage* img = cvLoadImage(argv[1]);
        cvNamedWindow("MyWindow");
        cvShowImage("MyWindow", img);

        //dilate and display the dilated image
        cvDilate(img, img, 0, 2);
        cvNamedWindow("Dilated");
        cvShowImage("Dilated", img);

        cvWaitKey(0);
       
        //cleaning up
        cvDestroyWindow("MyWindow");
        cvDestroyWindow("Dilated");
        cvReleaseImage(&img);
       
        return 0;
}
