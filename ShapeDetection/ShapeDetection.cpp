#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;


void getContours(Mat Dilatedimage, Mat imageRes) {

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	

	findContours(Dilatedimage, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	//drawContours(imageRes, contours, -1, Scalar(255,0,255), 2);

	//filter for removing noise
	for (int i = 0; i < contours.size(); i++) {
		
		
		float area = contourArea(contours[i]);
		
		cout << i << "  " << area << endl;
		vector<vector<Point>> contourPoly(contours.size());
		vector<Rect> boundRect(contours.size());
		String objType;

		if (area >= 1000) {
			double parameter = arcLength(contours[i], true);
			approxPolyDP(contours[i], contourPoly[i], 0.02 * parameter, true);
			drawContours(imageRes, contourPoly, i, Scalar(255, 0, 255), 2);
			cout << contourPoly[i].size() << endl;
			
			
			//Draw boxes around shapes
			boundRect[i] = boundingRect(contourPoly[i]);
			rectangle(imageRes, boundRect[i].tl(), boundRect[i].br(), Scalar(0,255,0), 2);

			//detect which object is what

			int cornerNum = (int)contourPoly[i].size();
			float aspectRatio;
			switch (cornerNum)
			{
			case 3:
				objType = "Triangle";
				break;
			case 4:

				aspectRatio = (float)boundRect[i].width / (float)boundRect[i].height;
				if (aspectRatio >= 0.95 && aspectRatio <= 1.05) {
					objType = "Square";
					break;
				}
				else {
					objType = "rectangle";
					break;
				}
				
				
			case 5:
				objType = "pentagon";
				break;
			default:
				objType = "circul";
				break;
			}

			putText(imageRes, objType, { boundRect[i].x, boundRect[i].y + 5 }, FONT_HERSHEY_PLAIN, 1, Scalar(0,0,0), 1);


		}
	}

}




int main()
{
	string path = "Resources/sHAPES.png";
	Mat image = imread(path);
	Mat GrayImage, BluredImage, CannyImage, DilationImage;


	//PreProcessing the image for detection


	cvtColor(image, GrayImage, COLOR_RGB2GRAY);
	GaussianBlur(GrayImage, BluredImage, Size(3, 3), 3, 0);
	Canny(BluredImage, CannyImage, 25, 75);

	Mat Kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	dilate(CannyImage, DilationImage, Kernel);

	getContours(DilationImage, image);

	imshow("image", image);
	/*imshow("gray image", GrayImage);
	imshow("blured image", BluredImage);
	imshow("contour image", CannyImage);
	imshow("dialted image", DilationImage);*/
	waitKey(0);
	return 0;
}