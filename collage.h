#pragma once
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <vector>

using namespace cv;
using namespace std;

class collage
{
public:
	collage();
	~collage();

	Mat			original;
	Mat			output;
	vector<Mat> partial_images_s;
	vector<Mat> partial_images_b;

	int size;
	int numberOfChunks;

	void kolazuj();
private:

	int evaluateDifference(vector<int> v);
	int compareChunks(Mat &a, Mat &b, int x = 0, int y = 0);



	void reSample(Mat &img, Size siz)
	{
		//resize(img, img, siz, 0, 0, INTER_NEAREST);
		//resize(img, img, siz, 0, 0, INTER_AREA);
		//resize(img, img, siz, 0, 0, INTER_LINEAR);
		resize(img, img, siz, 0, 0, INTER_CUBIC);
		//resize(img, img, siz, 0, 0, INTER_LANCZOS4);
	}


};

