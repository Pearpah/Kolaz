#include "stdafx.h"
#include "collage.h"
#include "iostream"



collage::collage()
{
}


collage::~collage()
{
}

void collage::kolazuj()
{
	Mat output(Size(size*numberOfChunks, size*numberOfChunks), CV_8UC3);
	reSample(original, cv::Size(size, size));
	vector<int> wyjsciowe(size*size);
	vector<int> wynik(size*size);
	std::fill(wynik.begin(), wynik.end(), INT_MAX);
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			for (int x = 0; x<partial_images_s.size(); x++)
			{
				int tmp = compareChunks(partial_images_s[x], original, i, j);
				if (tmp < wynik[i*size + j])
				{
					wynik[i*size + j] = tmp;
					wyjsciowe[i*size + j] = x;
				}
			}
			cout << "\r" << i+1 << " " << j+1 << "\t - "<<i*size+j+1 << "\t " << size*size;
		}
	}

	int s = partial_images_b[0].rows;
	int j = 0;
	int i = 0;
	for (int a = 0; a<wynik.size(); a++)
	{
		if (i > size - 1) { j++; i = 0; }
		/*reSample(wektor_mniejsze[wyjsciowe[i + j * sizer]], Size(fragment_size, fragment_size));
		wektor_mniejsze[wyjsciowe[i+j*sizer]].copyTo(ret(Rect(i*s, j*s, fragment_size,fragment_size)));*/
		partial_images_b[wyjsciowe[i + j*size]].copyTo(output(Rect(i*s, j*s, numberOfChunks, numberOfChunks)));
		i++;
	}
	imshow("kupa",original);
	imshow("aet", output);
}

int collage::evaluateDifference(vector<int> v)
{
	long int sum = 0;
	for (int a : v)
	{
		sum += a;
	}
	return sum;
}

int collage::compareChunks(Mat & a, Mat & b, int x, int y)
{
		vector<int> v(a.channels());
		for (int i = 0; i < a.rows; ++i)
		{
			for (int j = 0; j < a.cols; ++j)
			{
				for (int c = 0; c < a.channels(); ++c)
				{
					v[c] += abs(a.at<cv::Vec3b>(i, j)[c] - b.at<cv::Vec3b>((i + x), (j + y))[c]);
				}
			}
		}
		//int tmp = (*max_element(v.begin(), v.end()) + *min_element(v.begin(), v.end()));	
		return evaluateDifference(v);
		//return tmp;
}
