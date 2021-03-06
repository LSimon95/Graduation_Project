#include "stdafx.h"
#include "stdio.h"
#include "opencv.h"

#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

void Menu(void);
unsigned char ExitFlag = 0;
unsigned char item;

int main()
{
	printf("|****************************|\r\n");
	printf("|***********  XB ************|\r\n");
	printf("|*****Handwriting System*****|\r\n");
	printf("|****************************|\r\n");

	while (!ExitFlag)
		Menu();

	_tsystem(_T("pause"));
	return NULL;

}

void Menu(void)
{
	printf("Please select function\r\n");
	printf("1.Complete SVM-FPGA Training\r\n");
	printf("2.Character lib imag(png) to 2-Bit lattice diagram(Matlab)\r\n");
	printf("0.Exit\r\n");
	scanf("%d", (int *)&item);
	switch (item)
	{
	case 1:
		pic_handle();
		break;
	case 0:
		ExitFlag = 1;
		break;
	default:
		break;
	}
}