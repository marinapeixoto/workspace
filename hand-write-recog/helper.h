#pragma once
#ifndef TK_HELPER_H
#define TK_HELPER_H

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\ml\ml.hpp>

#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>

//#define __NONII__ 
#define __TKORAYS__
#include "pathdef.h"
#pragma comment(lib, __OPENCV_LIB_CORE__)
#pragma comment(lib, __OPENCV_LIB_HIGHGUI__)
#pragma comment(lib, __OPENCV_LIB_ML__)

using namespace std;
using namespace cv;

#endif