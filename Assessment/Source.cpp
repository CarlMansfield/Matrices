#include "Matrix.h"
#include "BinaryImage.h"

#include <iostream>
#include <fstream>
#include <istream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2\opencv.hpp>

double* readTXT(char* fileName, int sizeR, int sizeC);

void shuffleImage()
{
	//bounds for shuffled and noisy image
	int const p = 512;
	int const o = 512;
	//file names
	char* pathShuffled = "logo_shuffled.txt";
	char* pathNoise = "logo_with_noise.txt";
	char* output = "logo_restored.pgm";
	//read in data from text files, stored in double of pointers
	double* shuffleData = readTXT(pathShuffled, p, o);
	double* noiseData = readTXT(pathNoise, p, o);


	//create an empty binary image to store unshuffled data
	BinaryImage<double> logoUnshuffled(p, o);
	//store the data read in from txt files in 2 binary images with threshold value of 150
	BinaryImage<double> shuffled(p, o, shuffleData, 150);
	BinaryImage<double> noise(p, o, noiseData, 150);
	//delete the data read in, as it is now stored in a bin Matrix
	delete[] shuffleData;
	delete[] noiseData;

	double SSD = 0;
	double bestSSD = 0;
	bool blockFound[16][16];
	bool matchFound = false;

	//set all values in boolean 2d array as false
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			blockFound[i][j] = false;
		}
	}

	//goes through each block from noisy image 16*16 of 32*32 pixel blocks
	for (int x = 0; x < 16; x++)
	{
		for (int y = 0; y < 16; y++)
		{
			Matrix<double> subWithNoise = noise.getBlock(x * 32, y * 32, (x * 32) + 32, (y * 32) + 32);
			std::cout << "Finding best match for block x: " << x << " y: " << y << std::endl;
			bestSSD = 1024 * 1024 * 255;
			int bestMatchX = 0;
			int bestMatchY = 0;
			//finds best suited shuffled block to the noise block
			for (int xx = 0; xx < 16; xx++)
			{
				for (int yy = 0; yy < 16; yy++)
				{
					//checks if the block has already been used before
					if (!blockFound[xx][yy])
					{
						//gets the shuffled block and calculates sum of squared differences
						Matrix<double> subShuffled = shuffled.getBlock(xx * 32, yy * 32, (xx * 32) + 32, (yy * 32) + 32);
						Matrix<double> difference = subWithNoise - subShuffled;
						Matrix<double> product = difference * difference;
						SSD = product.sum();

						if (SSD < bestSSD)
						{
							bestSSD = SSD;
							bestMatchX = xx;
							bestMatchY = yy;
						}
					}
				}
			}
			//enters the best match found from all 16*16 searches
			for (int i = 0; i < 32; i++)
			{
				for (int j = 0; j < 32; j++)
				{
					double temp = shuffled.get(bestMatchX * 32 + i, bestMatchY * 32 + j);
					logoUnshuffled.set((x * 32) + i, (y * 32) + j, temp);

				}
			}
			//set the block used as true in boolean array
			blockFound[bestMatchX][bestMatchY] = true;
			std::cout << "Best match has been found at x: " << bestMatchX << " y: " << bestMatchY << std::endl;
			//write updated unshuffled image to PGM
			logoUnshuffled.writePGM(output, 10);
			//logowritePGM(output, logoUnshuffled, 10);
			//display image using OpenCV
			cv::Mat image;
			image = cv::imread("logo_restored.pgm", CV_LOAD_IMAGE_GRAYSCALE);
			cv::Mat binaryMat(image.size(), image.type());
			cv::threshold(image, binaryMat, 0, 255, cv::THRESH_BINARY);
			imshow("test", binaryMat);
			cv::waitKey(1);
		}
	}
	
}

void findWally()
{
	//file names of text images
	char* wallyPath = "Wally_grey.txt";
	char* scenePath = "Cluttered_scene.txt";
	//bounds for the template image
	int const templateHeight = 49;
	int const templateWidth = 36;
	//read in data of wally and scene as double of pointers
	double* wallyData = readTXT(wallyPath, templateHeight, templateWidth);
	double* sceneData = readTXT(scenePath, 768, 1024);
	//create Matrices for wally and scene using double pointers
	Matrix<double> wally(templateHeight, templateWidth, wallyData);
	Matrix<double> scene(768, 1024, sceneData);
	//calculate mean of wally
	double wallyMean = wally.sum() / (templateWidth*templateHeight);
	//take away mean of wally from each pixel
	for (int i = 0; i < templateHeight; i++)
	{
		for (int j = 0; j < templateWidth; j++)
		{
			double temp = wally.get(i, j) - wallyMean;
			wally.set(i, j, temp);
		}
	}


	/*lower than the lowest possible correlation so the
	first correlation measure is always going to be better than -2*/
	double bestNC = -2;
	//values stored to find best match of wally
	int bestX = 0;
	int bestY = 0;

	std::cout << "Attempting to find wally, please wait..." << std::endl;
	//minus template so it does not go out of bounds when getting template near edges of scene image
	for (int x = 0; x < 768 - templateHeight; x++)
	{
		for (int y = 0; y < 1024 - templateWidth; y++)
		{
			//create matrix for scene block
			Matrix<double> sceneBlock(templateHeight, templateWidth);

			//get data for scene block
			for (int i = 0; i < templateHeight; i++)
			{
				for (int j = 0; j < templateWidth; j++)
				{
					double temp = scene.get(x + i, y + j);
					sceneBlock.set(i, j, temp);
				}
			}

			//calculate mean for the scene block
			double sceneMean = sceneBlock.sum() / (templateHeight * templateWidth);

			//take mean values away from each element in scene block
			for (int i = 0; i < templateHeight; i++)
			{
				for (int j = 0; j < templateWidth; j++)
				{
					double temp = sceneBlock.get(i, j) - sceneMean;
					sceneBlock.set(i, j, temp);
				}
			}

			double sumWally = 0;
			double sumScene = 0;
			double crossCorr = 0;

			//compute normalised correlation
			for (int i = 0; i < templateHeight; i++)
			{
				for (int j = 0; j < templateWidth; j++)
				{
					//adds each pixel from template and block as part of NC formula
					double wallyPixel = wally.get(i, j);
					double scenePixel = sceneBlock.get(i, j);
					crossCorr += wallyPixel * scenePixel;
					sumWally += wallyPixel * wallyPixel;
					sumScene += scenePixel * scenePixel;
				}
			}

			//finalise correlation
			double NC = crossCorr / (sqrt(sumWally) * sqrt(sumScene));

			/* if the correlation is better, set the best co-ordinates
			no direct match may be possible so NC should not be == 1 */
			if (NC > bestNC)
			{
				bestNC = NC;
				bestX = x;
				bestY = y;
				std::cout << "New best possible match has been found with correlation of: " << NC << std::endl;
				std::cout << "Still searching for new best matches, please wait..." << std::endl;
			}

		}
	}

	//set values of best match to 0, showing where wally is
	for (int i = 0; i < templateHeight; i++)
	{
		for (int j = 0; j < templateWidth; j++)
		{
			scene.set((bestX)+i, (bestY)+j, 0);
		}
	}

	//write the updated scene to PGM file
	char* wallyFound = "wally_found.pgm";
	//writePGM(wallyFound, scene, 255);
	scene.writePGM(wallyFound, 255);

}

int main()
{
	int userInput;
	char userResponse;
	bool running = true;
	while (running == true)
	{
		std::cout << "Choose which operation you would like to conduct:" << std::endl;
		std::cout << "Enter 1 - Shuffle a scrambled image back to its normal form" << std::endl;
		std::cout << "Enter 2 - Find an image within a crowded image (Where's Wally)" << std::endl;
		std::cin >> userInput;
		switch (userInput)
		{
		case 1:
			std::cout << "Attempting to shuffle image into correct form...." << std::endl;
			shuffleImage();
			std::cout << "Image has been shuffled into its original form!" << std::endl;
			break;
		case 2:
			std::cout << "Attempting to find Wally...." << std::endl;
			findWally();
			std::cout << "Wally has been located!" << std::endl;
			break;
		}

		std::cout << "To exit the program please enter Y" << std::endl;
		std::cin >> userResponse;
		if (userResponse == 'Y')
		{
			running = false;
		}
		else if (userResponse != 'Y')
		{
			running = true;
		}
	}
	return 0;
}

//read from a txt file, add each element to data array
double* readTXT(char* fileName, int sizeR, int sizeC)
{
	using namespace std;
	double* data = new double[sizeR*sizeC];
	int i = 0;
	ifstream myfile(fileName);
	if (myfile.is_open())
	{
		while (myfile.good())
		{
			if (i>sizeR*sizeC - 1) break;
			myfile >> *(data + i);
			i++;
		}
		std::cout << "text file has been read and returned as an array" << std::endl;
		myfile.close();
	}

	else std::cout << "unable to open file" << std::endl;

	return data;
}