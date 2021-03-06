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
#include <Windows.h>

#include "collage.h"


using namespace cv;
using namespace std;


inline long long PerformanceCounter()
{
	LARGE_INTEGER result;
	::QueryPerformanceCounter(&result);
	return result.QuadPart;
}

inline long long PerformanceFrequency()
{
	LARGE_INTEGER result;
	::QueryPerformanceFrequency(&result);
	return result.QuadPart;
}

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


int number_of_chunks = 256;
int out_frag_siz = 4;

int main(int argc, char** argv)
{
	std::string path = std::experimental::filesystem::current_path().string()+"/DATASET";

	Mat original = imread("ja.jpg", IMREAD_ANYCOLOR);
	vector<pair<Mat, string>> mapa;
	for (auto & p : std::experimental::filesystem::directory_iterator (path)) 
	{
		string tmp = p.path().string();
		cout << tmp << endl;
		std::replace(tmp.begin(), tmp.end(), '\\', '/');
		Mat  img = imread(tmp, IMREAD_ANYCOLOR);		
		collage::reSample(img, Size(1,1));
		mapa.push_back(make_pair(img, tmp));
	}


	auto cyclesNow = PerformanceCounter();
	collage::kolazuj(original, number_of_chunks, out_frag_siz, mapa);
	auto cyclesDiff = PerformanceCounter() - cyclesNow;
	auto durationSeconds = cyclesDiff * 1.f / PerformanceFrequency();
	cout <<"\n"<< durationSeconds << " seconds" << endl;

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