/*
 *  preprocessing.h
 *  
 *
 *  Created by damiles on 18/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */
#ifdef _CH_
#pragma package <opencv>
#endif

#ifndef _EiC
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <ctype.h>
#endif

IplImage preprocessing(IplImage* imgSrc,int new_width, int new_height);
