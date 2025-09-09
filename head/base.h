#pragma once

#include <Windows.h>
#include <graphics.h>
#include <iostream>

#include "Camera.h"

extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;

void putImage(int x, int y, IMAGE* img);
void putImage(const Camera& camera, int x, int y, IMAGE* img);
void putImage(int x,int y,int w,int h,IMAGE* img,int src_x,int src_y);
void line(const Camera& camera, int x1, int y1, int x2, int y2);
void flip_image(IMAGE* src, IMAGE* dest);
void sketch_image(IMAGE* src,IMAGE* dst);