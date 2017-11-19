#include "stdafx.h"
#include "stdio.h"
#include "opencv.h"
#include "math.h"

#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
//#include <iostream>  


using namespace cv;
using namespace std;

#define cvThreshould 

//Mat srcImage;
//Mat midImage;

vector<Vec2f> combine_lines(vector<Vec2f> Lines);
Rect2i cutwordarea(Mat SrcImage, vector<Vec2f> HoughLines);
Point getcrosspoint(float Line1Rho, float Line1Theta, float Line2Rho, float Line2Theta, Mat SrcImage);
int cutandsavewords(Rect WordAreaRect, Mat ReImgae);
Rect2i boundingrectofoneword(Mat oneWord);

int pic_handle(void)
{
	printf("handling pic...\n");

	Mat ReImage = imread("C:\\Users\\qanpa\\Google Drive\\Document\\School\\In_Progress\\Graduation_Project\\OpenCV\\TestR1.jpg"); // Must "\\"
	Mat GRAYImage;
	cvtColor(ReImage, GRAYImage, COLOR_BGRA2GRAY);
	//resize(GRAYImage, srcImage, Size(2550, 3300), 0, 0);
	Mat midImage1, midImage2;

	namedWindow("Input", WINDOW_NORMAL); //Load Image
	resizeWindow("Input", 1200, 2133);
	imshow("Input", GRAYImage);
	waitKey(500);

	threshold(GRAYImage, midImage1, 200, 255, THRESH_BINARY);
	midImage2 = 255 - midImage1;
	/*Canny(midImage1, midImage2, 50, 200, 3);*/
	cvtColor(midImage2, midImage1, COLOR_GRAY2BGR);//Binary Image
	imshow("Input", midImage2);
	waitKey(500);

	vector<Vec2f>  Lines, LinesAfterCombine; //HoughLines
	HoughLines(midImage2, Lines, 1, CV_PI / 180, 1250, 0, 0);
	printf("Hough Lines Size:%ld\n", (long)Lines.size());
	LinesAfterCombine = combine_lines(Lines);
	printf("Lines After Combine Size:%ld\n", (long)LinesAfterCombine.size());
	for (size_t i = 0; i < LinesAfterCombine.size(); i++)
	{
		float rho = LinesAfterCombine[i][0], theta = LinesAfterCombine[i][1];
		Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a*rho, y0 = b*rho;
		pt1.x = cvRound(x0 + 5000 * (-b));
		pt1.y = cvRound(y0 + 5000 * (a));
		pt2.x = cvRound(x0 - 5000 * (-b));
		pt2.y = cvRound(y0 - 5000 * (a));
		line(midImage1, pt1, pt2, Scalar(00, 00, 255), 1, CV_AA);
	}
	imshow("Input", midImage1);
	waitKey(500);

	Mat WordArea(Size(900, 1600), CV_8UC1);
	Rect2i WordAreaRect = cutwordarea(GRAYImage, LinesAfterCombine);
	
	cutandsavewords(WordAreaRect, ReImage);


	waitKey(-1); 
	//imshow("Src", srcImage);
	//imshow("Edge", midImage);
	//resize(srcImage, midImage, Size(100, 120), 0, 0);

	//Canny(srcImage, midImage, 1, 3, 3, false);

	return NULL;
}

vector<Vec2f> combine_lines(vector<Vec2f> Lines)
{
	vector<Vec2f> LinesAfterCombine;
	int CombineResult = 1; //0 - unlike 1 - like

	LinesAfterCombine.push_back(Lines[0]);
	for (size_t i = 0; i < Lines.size(); i++)
	{
		for (size_t j = 0; j < LinesAfterCombine.size(); j++)
		{
			if(fabs(LinesAfterCombine[j][0] - Lines[i][0]) > 30.0)
			{
				CombineResult = 0;
			}
		}
		if (CombineResult == 0)
		{
			LinesAfterCombine.push_back(Lines[i]);
			CombineResult = 1;
		}
	}
	return LinesAfterCombine;
}

Rect2i cutwordarea(Mat SrcImage, vector<Vec2f> HoughLines) //return 900 * 1600
{
	vector<Vec2f> AreaLines; //top bot lef rig
	Mat SrcImageBK;
	SrcImage.copyTo(SrcImageBK);
	AreaLines.resize(4);
	float Theta0MinRho = 5000, Theta0MaxRho = 0, Theta05piMinRho = 5000, Theta05piMaxRho = 0;
	for (size_t i = 0; i < HoughLines.size(); i++)
	{
		if (HoughLines[i][1] > 1.4 && HoughLines[i][1] < 1.8)
		{
			if (Theta05piMaxRho < HoughLines[i][0])
			{
				Theta05piMaxRho = HoughLines[i][0];
				AreaLines[1][0] = HoughLines[i][0];
				AreaLines[1][1] = HoughLines[i][1]; //Bot
			}

			if (Theta05piMinRho > HoughLines[i][0])
			{
				Theta05piMinRho = HoughLines[i][0];
				AreaLines[0][0] = HoughLines[i][0];
				AreaLines[0][1] = HoughLines[i][1];//Top
			}
		}

		if (HoughLines[i][1] < 0.3)
		{
			if (Theta0MaxRho < HoughLines[i][0])
			{
				Theta0MaxRho = HoughLines[i][0];
				AreaLines[3][0] = HoughLines[i][0];
				AreaLines[3][1] = HoughLines[i][1]; //Rig
			}

			if (Theta0MinRho > HoughLines[i][0])
			{
				Theta0MinRho = HoughLines[i][0];
				AreaLines[2][0] = HoughLines[i][0];
				AreaLines[2][1] = HoughLines[i][1]; ///Lef
			}
		}
	}

	for (size_t i = 0; i < AreaLines.size(); i++)
	{
		float rho = AreaLines[i][0], theta = AreaLines[i][1];
		Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a*rho, y0 = b*rho;
		pt1.x = cvRound(x0 + 5000 * (-b));
		pt1.y = cvRound(y0 + 5000 * (a));
		pt2.x = cvRound(x0 - 5000 * (-b));
		pt2.y = cvRound(y0 - 5000 * (a));
		line(SrcImage, pt1, pt2, Scalar(0, 0, 0), 1, CV_AA);
	}
	printf("Word Area Line Size:%ld\n", (long)AreaLines.size());

	printf("%f, %f, %f, %f, %f, %f, %f, %f\n", 
		AreaLines[0][0],
		AreaLines[0][1], 
		AreaLines[1][0], 
		AreaLines[1][1],
		AreaLines[2][0],
		AreaLines[2][1],
		AreaLines[3][0],
		AreaLines[3][1]);

	//vector<Vec2f> TestLines;
	//TestLines.resize(100);
	//for (int k = 0; k < 100; k++)
	//{
	//	TestLines[k][0] = 750;
	//	TestLines[k][1] = (CV_PI / 100) * k;
	//}
	//for (size_t i = 0; i < TestLines.size(); i++)
	//{
	//	float rho = TestLines[i][0], theta = TestLines[i][1];
	//	Point pt1, pt2;
	//	double a = cos(theta), b = sin(theta);
	//	double x0 = a*rho, y0 = b*rho;
	//	pt1.x = cvRound(x0 + 5000 * (-b));
	//	pt1.y = cvRound(y0 + 5000 * (a));
	//	pt2.x = cvRound(x0 - 5000 * (-b));
	//	pt2.y = cvRound(y0 - 5000 * (a));
	//	line(SrcImage, pt1, pt2, Scalar(0, 0, 0), 1, CV_AA);
	//	imshow("Input", SrcImage);
	//	waitKey(500);
	//}

	Point ptLT = getcrosspoint(AreaLines[0][0], AreaLines[0][1], AreaLines[2][0], AreaLines[2][1], SrcImage);
	Point ptLB = getcrosspoint(AreaLines[1][0], AreaLines[1][1], AreaLines[2][0], AreaLines[2][1], SrcImage);
	Point ptRT = getcrosspoint(AreaLines[0][0], AreaLines[0][1], AreaLines[3][0], AreaLines[3][1], SrcImage);
	Point ptRB = getcrosspoint(AreaLines[1][0], AreaLines[1][1], AreaLines[3][0], AreaLines[3][1], SrcImage);

	circle(SrcImage, ptLT, 25, Scalar(0, 0, 255), 5);
	circle(SrcImage, ptLB, 25, Scalar(0, 0, 255), 5);
	circle(SrcImage, ptRT, 25, Scalar(0, 0, 255), 5);
	circle(SrcImage, ptRB, 25, Scalar(0, 0, 255), 5);

	imshow("Input", SrcImage);
	waitKey(500);

	Rect2i WordArea(ptLT.x, ptLT.y, ptRT.x - ptLT.x, ptRB.y - ptRT.y);

	return WordArea;
}

Point getcrosspoint(float Line1Rho, float Line1Theta, float Line2Rho, float Line2Theta, Mat SrcImage)
{
	Point2f pt1, pt2, pt3, pt4;
	Point pt;

	pt1.x = Line1Rho * cos(Line1Theta); pt1.y = Line1Rho * sin(Line1Theta);
	pt2.x = (float)((Line1Rho / 0.70711) * cos(Line1Theta + (CV_PI / 4))); pt2.y = (float)((Line1Rho / 0.70711) * sin(Line1Theta + (CV_PI / 4)));
	pt3.x = Line2Rho * cos(Line2Theta); pt3.y = Line2Rho * sin(Line2Theta);
	pt4.x = (float)((Line2Rho / 0.70711) * cos(Line2Theta + (CV_PI / 4))); pt4.y = (float)((Line2Rho / 0.70711) * sin(Line2Theta + (CV_PI / 4)));

	//denominator
	float deMa[2][2];
	deMa[0][0] = pt1.x - pt2.x;
	deMa[0][1] = pt1.y - pt2.y;
	deMa[1][0] = pt3.x - pt4.x;
	deMa[1][1] = pt3.y - pt4.y;

	float de = (deMa[0][0] * deMa[1][1]) - (deMa[0][1] * deMa[1][0]);

	//numerator 
	float nuLine1 = (pt1.x * pt2.y) - (pt1.y * pt2.x);
	float nuLine2 = (pt3.x * pt4.y) - (pt3.y * pt4.x);
	float nux = (nuLine1 * (pt3.x - pt4.x)) - ((pt1.x - pt2.x) * nuLine2);
	float nuy = (nuLine1 * (pt3.y - pt4.y)) - ((pt1.y - pt2.y) * nuLine2);

	pt.x = (int)(nux / de);
	pt.y = (int)(nuy / de);
	return pt;
}

int cutandsavewords(Rect WordAreaRect, Mat ReImgae)
{
	Mat WordArea, WordAreaGray, WordArea2B, WordArea1152_2048, WordArea1152_2048Dis;

	ReImgae(WordAreaRect).copyTo(WordArea);
	
	imshow("Input", WordArea);
	waitKey(500);

	Mat WordAreaHSV;

	cvtColor(WordArea, WordAreaHSV, COLOR_BGR2HSV);
	imshow("Input", WordAreaHSV);
	waitKey(500);

	//Filter Red Frame
	printf("%d, %d\n", WordAreaHSV.rows, WordAreaHSV.cols);

	for (int i = 0; i < WordAreaHSV.rows; i++) //HSV remove red frame
	{
		for (int j = 0; j < WordAreaHSV.cols; j++)
		{
			uchar *Data = WordAreaHSV.ptr<uchar>(i);

			if (Data[j * 3] > 167 && Data[(j * 3)] < 190 && Data[(j * 3) + 1]  > 30 && Data[(j * 3) + 2]  > 70)
			{
				Data[j * 3] = Data[(j * 3) + 1] = 0;
				Data[(j * 3) + 2] = 255;
			}
		}
	}
	imshow("Input", WordAreaHSV);
	waitKey(500);

	cvtColor(WordAreaHSV, WordArea, COLOR_HSV2BGR);
	cvtColor(WordArea, WordAreaGray, COLOR_BGR2GRAY);
	Mat blured;
	blur(WordAreaGray, blured, Size(2, 2));

	threshold(blured, WordArea2B, 190, 255, THRESH_BINARY);

	blur(WordArea2B, blured, Size(2, 2));
	threshold(blured, WordArea2B, 190, 255, THRESH_BINARY);

	imshow("Input", WordArea2B);
	waitKey(500);

	resize(WordArea2B, WordArea1152_2048, Size(1152, 2048));
	imshow("Input", WordArea1152_2048);
	waitKey(500);
	WordArea1152_2048.copyTo(WordArea1152_2048Dis);

	//Mat WordArea576_1024IW = 255 - WordArea1152_2048;

	Rect2i RectoneWord(0, 0, 128, 128);
	Rect2i RectoneWordMin;
	Mat oneWord, oneWord6464, oneWord6464Th;
	//vector<vector<Point>> contours;
	//vector<Vec4i> hierarchy;

	//findContours(WordArea576_1024IW, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE, Point());

	//vector<Rect> MinRectofOneWord(contours.size());

	//for (int i = 0; i < contours.size(); i++)
	//{
	//	MinRectofOneWord[i] = boundingRect(Mat(contours[i]));
	//	rectangle(WordArea1152_2048, MinRectofOneWord[i], Scalar(0));
	//}
	//imshow("Input", WordArea1152_2048);
	//waitKey(500);
	for (int i = 0; i < 16; i++)
	{
		
		for (int j = 0; j < 9; j++)
		{
			
			RectoneWord.x = 128 * j;
			RectoneWord.y = 128 * i;
			
			WordArea1152_2048(RectoneWord).copyTo(oneWord);
			

			RectoneWordMin = boundingrectofoneword(oneWord);

			if (RectoneWordMin.x == 0 && RectoneWordMin.y == 0)
			{
				if (RectoneWordMin.width == 127 && RectoneWordMin.height == 127)
				{
					continue;
				}
			}

			char Num[4]; 
			sprintf_s(Num, "%d", i * 9 + j);
			String str(Num);
			putText(WordArea1152_2048Dis, str, Point(128 * j, 128 * i + 10), FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar(0)); //print Num

			RectoneWordMin.x += 128 * j;
			RectoneWordMin.y += 128 * i;

			rectangle(WordArea1152_2048Dis, RectoneWordMin, Scalar(0), 2);
			imshow("Input", WordArea1152_2048Dis);
			waitKey(100);

			
			WordArea1152_2048(RectoneWordMin).copyTo(oneWord);
			resize(oneWord, oneWord6464, Size(64, 64));
			threshold(oneWord6464, oneWord6464Th, 190, 255, THRESH_BINARY);
			String FileName("C:\\Users\\qanpa\\Google Drive\\Document\\School\\In_Progress\\Graduation_Project\\OpenCV\\Output\\");
			FileName = FileName + str + ".pbm";

			vector<int> compression_params;
			compression_params.push_back(IMWRITE_PXM_BINARY);
			compression_params.push_back(0);
			imwrite(FileName, oneWord6464Th, compression_params);

		}
	}
	return NULL;
}

Rect2i boundingrectofoneword(Mat oneWord)
{
	Mat oneWordCanny;
	
	Canny(oneWord, oneWordCanny, 50, 200, 3);

	int minx = 127, miny = 127, maxx = 0, maxy = 0;
	uchar *Data;
	for (int i = 0; i < 128; i++)
	{
		Data = oneWordCanny.ptr<uchar>(i);
		for (int j = 0; j < 128; j++)
		{
			if (Data[j] > 200)
			{
				if (j < minx)
					minx = j;
				if (j > maxx)
					maxx = j;
				if (i < miny)
					miny = i;
				if (i > maxy)
					maxy = i;
			}
		}
	}
	if (minx == 127 && miny == 127 && maxx == 0 && maxy == 0)
	{
		minx = 0;
		miny = 0;
		maxx = 127;
		maxy = 127;
	}

	if(minx > 0)
		minx -= 1;
	if(maxx < 127)
		maxx += 1;
	if(miny > 0)
		miny -= 1;
	if(maxy < 127)
		maxy += 1;
	Rect2i RectMin(Point(minx, miny), Size2i(maxx - minx, maxy - miny));
	return RectMin;
}