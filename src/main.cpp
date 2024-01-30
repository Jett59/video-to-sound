#include <portaudio.h>
#include <opencv4/opencv2/opencv.hpp>

#include <iostream>

int main(int argc, char** argv) {
  auto videoFile = argc < 2 ? "video.mp4" : argv[1];
  cv::VideoCapture videoCapture(videoFile);
  if (!videoCapture.isOpened()) {
    std::cout << "Cannot open video (maybe you need to create " << videoFile
              << ")" << std::endl;
    return 1;
  }
  while (true) {
    cv::Mat frame;
    videoCapture >> frame;
    if (frame.empty()) {
      std::cout << "No frame" << std::endl;
      break;
    }
  }
  return 0;
}
