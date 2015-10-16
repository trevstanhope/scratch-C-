#include <opencv/cv.h>
#include <opencv2/core/core_c.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <algorithm>
#include <vector>

using namespace cv;
using namespace std;

//To determine the correct data type this function was written.

  string type2str(int type) {
    string r;

    uchar depth = type & CV_MAT_DEPTH_MASK;
    uchar chans = 1 + (type >> CV_CN_SHIFT);

    switch ( depth ) {
      case CV_8U:  r = "8U"; break;
      case CV_8S:  r = "8S"; break;
      case CV_16U: r = "16U"; break;
      case CV_16S: r = "16S"; break;
      case CV_32S: r = "32S"; break;
      case CV_32F: r = "32F"; break;
      case CV_64F: r = "64F"; break;
      default:     r = "User"; break;
    }

    r += "C";
    r += (chans+'0');

    return r;
  }

  // Function to find unique values of a vector.

  std::vector<float> unique(const cv::Mat& input, bool sort = false)
    {
      if (input.channels() > 1 || input.type() != CV_32F)
      {
          std::cerr << "Please enter a CV_32F 1-channel Mat" << std::endl;
          return std::vector<float>();
      }

      std::vector<float> out;
      for (int y = 0; y < input.rows; ++y)
      {
          const float* row_ptr = input.ptr<float>(y);
          for (int x = 0; x < input.cols; ++x)
          {
              float value = row_ptr[x];

              if ( std::find(out.begin(), out.end(), value) == out.end() )
                  out.push_back(value);
          }
      }

      if (sort)
          std::sort(out.begin(), out.end());

      return out;
    }


int main(int argc,char** argv)
{

    Mat left_image = imread(argv[1],0);
    Mat right_image = imread(argv[2],0);

    imshow("Source",left_image);
    waitKey(0);
    imshow("Target",right_image);
    waitKey(0);

    Mat output_image(left_image.rows,left_image.cols,CV_8UC1);
    Mat Level1(left_image.rows,left_image.cols,CV_8UC1);
    Mat Level2(left_image.rows,left_image.cols,CV_8UC1);
    Mat Level3(left_image.rows,left_image.cols,CV_8UC1);
    Mat Lv1(left_image.rows,left_image.cols,CV_8UC1);
    Mat Lv2(left_image.rows,left_image.cols,CV_8UC1);
    Mat Lv3(left_image.rows,left_image.cols,CV_8UC1);


    uchar Thresh = 12;
    uchar Range = 16;

    for(int i=0;i<left_image.rows;i++)
    {

        for(int j=0;j<left_image.cols;j++)
        {
            uchar dist= 255;
            int iMin = -1;
            for(int k=0;k<=Range-1;k++)
            {

                if(j - k < 0)
                    continue;
                uchar lPx = left_image.at<uchar>(i,j);
                uchar rPx = right_image.at<uchar>(i,j-k);

                uchar t = abs(lPx-rPx);
                t = (t > Thresh) ?  Thresh : t ;

                if(t < dist)
                {
                    dist = t;
                    iMin = k;
                }

            }

            output_image.at<uchar>(i,j) = (uchar) ( 255* ( ((float)iMin) /Range) );

        }
    }

    GaussianBlur(output_image,output_image,Size(3,3),3);
    imshow("Disparity",output_image);
    waitKey(0);

    //imwrite( "C:/Ceemple/user/OpenCV/stereo_vision/disparity.png", output_image );


    int threshold_value1 = 79;
    int max_value = 255;
    threshold(output_image, Lv1, threshold_value1, max_value, 0);

    int threshold_value2 = 126;
    threshold(output_image, Lv2, threshold_value2, max_value, 0);

    int threshold_value3 = 176;
    threshold(output_image, Lv3, threshold_value3, max_value, 0);

    /*imshow("3",Lv3);
    waitKey(0);*/

    Lv1 = Lv1 - Lv2;
    Lv2 = Lv2 - Lv3;

    /*imshow("2",Lv2);
    waitKey(0);
    imshow("level1",Lv1);
    waitKey(0);*/

    bitwise_and(right_image, Lv3, Level3);
    imshow("Nearest_objects",Level3);
    waitKey(0);

    bitwise_and(right_image, Lv2, Level2);
    imshow("Midground_objects",Level2);
    waitKey(0);

    bitwise_and(right_image, Lv1, Level1);
    imshow("Farthest_objects",Level1);
    waitKey(0);

    /*Block to project into a 3D co-ordinate space.
     *
     * output_image.convertTo(output_image, CV_32F);

    string ty =  type2str( output_image.type() );
    printf("Matrix: %s %dx%d \n", ty.c_str(), output_image.cols, output_image.rows );

    std::vector<float> grey_vals = unique(output_image, true);

    for (unsigned int i = 0; i < grey_vals.size(); i++){
        std::cout << grey_vals[i] << " ";
        std::cout << std::endl;
    }

   */

    /*double q[4][4] = {{ 1, 0, 0, -296.1 }, { 0, 1, 0, -233.7 }, { 0, 0, 0, 564.4 }, { 0, 0, -0.113, 4.165 }};
    Mat Q( 4, 4, CV_64F, q );
    Mat xyz;
    reprojectImageTo3D(output_image, xyz, Q, true);
    imshow("3D Image showing....",xyz);
    waitKey(0);*/

    return 0;

}




