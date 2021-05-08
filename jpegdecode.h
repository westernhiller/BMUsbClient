#ifndef JPEGDECODE_H
#define JPEGDECODE_H

#include "turbojpeg.h"
#include <tuple>
#include <vector>
#include <fstream>

#include "global.h"

std::tuple<bool,std::vector<uint8_t>,uint64_t,uint64_t,uint64_t> decodeJpeg2X(uint8_t* pJpegData,uint64_t JpegdataSize,const char* convrt_flag);

std::tuple<bool,std::vector<uint8_t>,uint64_t,uint64_t,uint64_t> decodeJpeg2X(std::string filename,const char* convrt_flag);

QImage Jpeg2QImage(uint8_t *jpegData, uint64_t jpegSize);

cv::Mat Jpeg2Mat(uint8_t *jpegData, uint64_t jpegSize);

cv::Mat Jpeg2Mat(std::string filename);

#endif // JPEGDECODE_H
