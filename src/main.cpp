// This has to go at the top.
// Ref:
// https://stackoverflow.com/questions/6563810/m-pi-works-with-math-h-but-not-with-cmath-in-visual-studio
#define _USE_MATH_DEFINES
#include <cmath>

#include <portaudio.h>
#include <opencv2/opencv.hpp>

#include <iostream>

struct AudioOutputInfo {
  float progress;
};

static int audioCallback(const void* inputBuffer,
                         void* outputBuffer,
                         unsigned long samplesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void* userData) {
  AudioOutputInfo* info = (AudioOutputInfo*)userData;
  float* out = (float*)outputBuffer;

  for (unsigned long i = 0; i < samplesPerBuffer; i++) {
    // It is stereo, so we need to write to both channels.
    out[2 * i] = sin(2 * M_PI * info->progress) / 3;
    out[2 * i + 1] = -sin(2 * M_PI * info->progress) / 3;
    info->progress += 0.01;
  }
  return 0;
}

int main(int argc, char** argv) {
  cv::VideoCapture videoCapture;
  if (argc > 1) {
    videoCapture.open(argv[1]);
  } else {
    videoCapture.open(0);
  }
  if (!videoCapture.isOpened()) {
    std::cout << "Cannot open video" << std::endl;
    return 1;
  }

  auto error = Pa_Initialize();
  if (error != paNoError) {
    std::cout << "Cannot initialize PortAudio: " << Pa_GetErrorText(error)
              << std::endl;
    return 1;
  }

  AudioOutputInfo audioOutputInfo;
  audioOutputInfo.progress = 0.0f;

  PaStream* stream;
  error = Pa_OpenDefaultStream(&stream, 0, 2, paFloat32, 44100, 256,
                               audioCallback, &audioOutputInfo);
  if (error != paNoError) {
    std::cout << "Cannot open PortAudio stream: " << Pa_GetErrorText(error)
              << std::endl;
    return 1;
  }

  error = Pa_StartStream(stream);
  if (error != paNoError) {
    std::cout << "Cannot start PortAudio: " << Pa_GetErrorText(error)
              << std::endl;
    return 1;
  }

  std::cout << "Beginning..." << std::endl;
  while (true) {
    cv::Mat frame;
    videoCapture >> frame;
    if (frame.empty()) {
      std::cout << "No frame" << std::endl;
      break;
    }
  }

  error = Pa_StopStream(stream);
  if (error != paNoError) {
    std::cout << "Cannot stop PortAudio: " << Pa_GetErrorText(error)
              << std::endl;
    return 1;
  }

  error = Pa_CloseStream(stream);
  if (error != paNoError) {
    std::cout << "Cannot close PortAudio: " << Pa_GetErrorText(error)
              << std::endl;
    return 1;
  }

  error = Pa_Terminate();
  if (error != paNoError) {
    std::cout << "Cannot terminate PortAudio: " << Pa_GetErrorText(error)
              << std::endl;
    return 1;
  }
  return 0;
}
