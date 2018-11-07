// Kolaz.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <stdlib.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <iostream>
#include <filesystem>
#include <string>

#include "collage.h"


using namespace cv;
using namespace std;

void reSample(Mat &img, Size siz)
{
	//resize(img, img, siz, 0, 0, INTER_NEAREST);
	//resize(img, img, siz, 0, 0, INTER_AREA);
	//resize(img, img, siz, 0, 0, INTER_LINEAR);
	resize(img, img, siz, 0, 0, INTER_CUBIC);
	//resize(img, img, siz, 0, 0, INTER_LANCZOS4);
}

void downSample(Mat &img) 
{
	resize(img, img, Size(img.cols / 2, img.rows / 2), 0, 0, INTER_NEAREST);
}


void upSample(Mat &img)
{
	resize(img, img, Size(img.cols * 2, img.rows * 2), 0, 0, INTER_NEAREST);
}

int evaluateDifference(vector<int> &v) 
{
	long int sum=0;
	for (int a : v) 
	{
		sum += a;
	}
	return sum;
}

int differential(Mat &a, Mat &b, int x=0, int y=0) 
{
	vector<int> v(a.channels());
	for (int i = 0; i < a.rows; ++i)
	{
		for (int j = 0; j < a.cols; ++j)
		{
			for (int c = 0; c < a.channels(); ++c)
			{
				v[c] += abs(a.at<cv::Vec3b>(i, j)[c] - b.at<cv::Vec3b>((i+x), (j+y))[c]);
			}
		}
	}	
	//int tmp = (*max_element(v.begin(), v.end()) + *min_element(v.begin(), v.end()));	
	return evaluateDifference(v);
	//return tmp;
}



int sizer = 64;
int fragment_size = 16;

int main(int argc, char** argv)
{


	//Mat original = imread("pomoc.jpg", IMREAD_ANYCOLOR);	
	//Mat mod=original;


	std::string path = std::experimental::filesystem::current_path().string()+"/DATASET4";
	//vector<Mat> wektor_mniejsze;
	//vector<Mat> wektor_wieksze;

	collage kolaz;
	kolaz.original = imread("ja.jpg", IMREAD_ANYCOLOR);
	kolaz.size = sizer;
	kolaz.numberOfChunks = fragment_size;

	for (auto & p : std::experimental::filesystem::directory_iterator (path)) 
	{
		string tmp = p.path().string();
		cout << tmp << endl;
		std::replace(tmp.begin(), tmp.end(), '\\', '/');
		Mat  img = imread(tmp, IMREAD_ANYCOLOR);
		reSample(img, Size(fragment_size, fragment_size));
		kolaz.partial_images_b.push_back(img);
		reSample(img, Size(1,1));
		kolaz.partial_images_s.push_back(img);
	}

	kolaz.kolazuj();


	//cout << endl << endl << "POROWNYWANIE" << endl;


	//reSample(mod, cv::Size(sizer, sizer));
	//vector<int> wynik(sizer*sizer);
	//vector<int> wyjsciowe(sizer*sizer);
	//std::fill(wynik.begin(), wynik.end(), INT_MAX);
	//for (int i = 0; i < sizer; i++) 
	//{
	//	for (int j = 0; j < sizer; j++) 
	//	{
	//		for(int x=0; x<wektor_mniejsze.size();x++)
	//		{
	//			int tmp = differential(wektor_mniejsze[x] , mod, i, j);
	//			if (tmp < wynik[i*sizer+j])
	//			{
	//				wynik[i*sizer+j] = tmp;
	//				wyjsciowe[i*sizer+j] = x;
	//			}
	//		}
	//		cout << i << " " << j << endl;

	//	}
	//}

	//cout << endl << endl << "TWORZENIE" << endl;

	//Mat ret(Size(sizer*fragment_size, sizer*fragment_size),CV_8UC3);

	//int s = wektor_wieksze[0].rows;
	//int j = 0;
	//int i = 0;
	//for (int a=0; a<wynik.size();a++)
	//{
	//	if (i > sizer-1) { j++; i = 0; }
	//	/*reSample(wektor_mniejsze[wyjsciowe[i + j * sizer]], Size(fragment_size, fragment_size));
	//	wektor_mniejsze[wyjsciowe[i+j*sizer]].copyTo(ret(Rect(i*s, j*s, fragment_size,fragment_size)));*/
	//	wektor_wieksze[wyjsciowe[i + j*sizer]].copyTo(ret(Rect(i*s, j*s, fragment_size, fragment_size)));
	//	
	//	i++;
	//}
	//reSample(mod, Size(200, 200));
	//imshow("bra",ret);
	//imshow("tak", mod);

	////////////////////////////////////////////////////

	//Mat img(1, 1, CV_8UC3);
	//int counter = 65;
	//int downlimit = 0;
	//int uplimit = 256;
	//string sciezka = "D:/@PROJEKTY@/Programowanie/2018/Kolaz/Kolaz/Kolaz/folder/";
	//for(int b= downlimit; b<=uplimit;b+=counter)
	//{
	//	for (int g = downlimit; g<=uplimit; g += counter)
	//	{
	//		for (int r = downlimit; r<=uplimit; r += counter)
	//		{
	//			img.at<cv::Vec3b>(0, 0)[0] = b;
	//			img.at<cv::Vec3b>(0, 0)[1] = r;
	//			img.at<cv::Vec3b>(0, 0)[2] = g;
	//			cout << r << " " << g << " " << b << endl;
	//			//string t = to_string(r + g + b);			
	//			string tt = to_string(r) + to_string(g) + to_string(b);
	//			imwrite("kolors" + tt + ".jpg", img);
	//		}
	//	}
	//}


	waitKey(0); // Wait for a keystroke in the window
	return 0;
}