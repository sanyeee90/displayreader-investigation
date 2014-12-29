#include <opencv2/core/core.hpp>

using namespace cv;

class EventHandler {

private:
	bool roi_captured, got_roi;
	Point pt1, pt2;
	Mat cap_img, result_image;
public:
	EventHandler() { this->roi_captured = false; this->got_roi = false; }
	void operator()(int event, int x, int y, int flags, void *param);
	bool isGotRoi() { return this->got_roi; }
	Point getFirstPoint() { return this->pt1; }
	Point getLastPoint() { return this->pt2; }
	void setInputImage(Mat& img) { this->cap_img = img.clone(); }
	void (EventHandler::*pt2Member)(int, int, int, int, void *) = NULL;
};

