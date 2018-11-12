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
	static void kolazuj(Mat original, int number_of_chunks, int frag_size, vector<pair<Mat, string>> mapa);
	static void reSample(Mat &img, Size siz)
	{
		//resize(img, img, siz, 0, 0, INTER_NEAREST);
		//resize(img, img, siz, 0, 0, INTER_AREA);
		//resize(img, img, siz, 0, 0, INTER_LINEAR);
		resize(img, img, siz, 0, 0, INTER_CUBIC);
		//resize(img, img, siz, 0, 0, INTER_LANCZOS4);
	}
private:
	static void make_final_image(std::vector<int> &score, std::vector<string> &return_array, cv::Mat &output, int frag_size, int number_of_chunks);
	static int	evaluateDifference(vector<int> v);
	static int	compareChunks(Mat &a, Mat &b, int x = 0, int y = 0);
	static void create_configuration(int size, vector<pair<Mat, string>> mapa, cv::Mat &original, std::vector<int> &score, std::vector<string> &return_array);
	   


};

