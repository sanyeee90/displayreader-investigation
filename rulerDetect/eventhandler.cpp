#include "eventhandler.h"
#include <iostream>
#include <opencv2/highgui/highgui.hpp>

void EventHandler::operator()(int event, int x, int y, int flags, void *param)
{

	switch (event)
	{
	case CV_EVENT_LBUTTONDOWN:
	{
		std::cout << "Mouse Pressed" << std::endl;

		if (!this->roi_captured)
		{
			pt1.x = x;
			pt1.y = y;
			std::cout << pt1;
		}
		else
		{
			std::cout << "ROI Already Acquired" << std::endl;
		}
		break;
	}
	case CV_EVENT_LBUTTONUP:
	{
		if (!this->got_roi)
		{
			Mat cl;
			std::cout << "Mouse LBUTTON Released" << std::endl;
			cl = cap_img.clone();
			pt2.x = x;
			pt2.y = y;
			std::cout << "PT1 " << pt1.x << "," << pt1.y << std::endl;
			std::cout << "PT2 " << pt2.x << "," << pt2.y << std::endl;

			this->got_roi = true;
		}
		else
		{
			std::cout << "ROI Already Acquired" << std::endl;
		}
		break;
	}

	}

}