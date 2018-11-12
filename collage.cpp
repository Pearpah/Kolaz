#include "stdafx.h"
#include "collage.h"
#include "iostream"



//Repeating variables
//No need for the return_array and score to be two different entities
//It should be returning a Mat
void collage::kolazuj(Mat original, int number_of_chunks, int frag_size, vector<pair<Mat,string>> mapa)
{
	//Output image
	Mat output(Size(number_of_chunks*frag_size, number_of_chunks*frag_size), CV_8UC3);
	//Resizing original picture before chunking
	reSample(original, cv::Size(number_of_chunks, number_of_chunks));
	//Filenames in positions
	vector<string> return_array(number_of_chunks*number_of_chunks);
	//Scores of the best filename in position
	vector<int> score(number_of_chunks*number_of_chunks);
	//Smaller score = better
	std::fill(score.begin(), score.end(), INT_MAX);
	//Creates configuration of filenames (by best score)
	cout << "\n \tbegin configuration";
	create_configuration(number_of_chunks, mapa, original, score, return_array);
	//Creates final image
	cout << "\n \tbegin making the final image";
	make_final_image(score,return_array, output, frag_size, number_of_chunks);
	cout << endl;
	imshow("OriginalImage",original);
	imshow("FinalIMage", output);
}


//If the image is reccuring it should add it to some kind of memory 
/*
*	Perhaps it would be wiser to instead of iterating over pixels
*	iterate over the configuration
*		- no need to upload the same image twice
*/
void collage::make_final_image(std::vector<int> &score, std::vector<string> &return_array, cv::Mat &output, int frag_size, int number_of_chunks)
{
	//int s = partial_images_b[0].rows;
	int j = 0;
	int i = 0;
	int counter = 0;
	//Size of scores should be the same as the size of height*width of image that we want to construct
	//#pragma omp parallel for
	for (int a = 0; a<score.size(); a++)
	{
		//Iterates over the rows
		//If the column reaches the last one go to the next rows and 0 the col
		if (i > number_of_chunks - 1) { j++; i = 0; }
		//Reads the file
		Mat img = imread(return_array[i+j* number_of_chunks]);
		//Resamples it to wanted size
		reSample(img, Size(frag_size, frag_size));
		//Copies it
		img.copyTo(output(Rect(i*frag_size, j*frag_size, frag_size, frag_size)));
		//Next col!
		i++;
		counter++;
		cout << "\r" + to_string(counter * 100 / score.size()) + "\%";
	}

}

void collage::create_configuration(int size, vector<pair<Mat, string>> mapa, cv::Mat &original, std::vector<int> &score, std::vector<string> &return_array)
{
	int counter = 0;
	//#pragma omp parallel for
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			for (int x = 0; x<mapa.size(); x++)
			{
				//Compares miniature of an image to a rectangle in original
				int tmp = compareChunks(mapa[x].first, original, i, j);
				//If score is better place it in the scoreboard
				//Same with the return array
				if (tmp < score[i*size + j])
				{
					score[i*size + j] = tmp;
					return_array[i*size + j] = mapa[x].second;
				}
			}
		}
		counter++;
		cout << "\r" + to_string(counter * 100 / size) + "\%";
	}
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
