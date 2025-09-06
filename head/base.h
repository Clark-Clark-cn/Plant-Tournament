#pragma once

#include <Windows.h>
#include <graphics.h>
#include <iostream>

#include "Camera.h"

const int WINDOW_WIDTH=1280;
const int WINDOW_HEIGHT=720;

void putImage(int x, int y, IMAGE* img){
#ifdef _DEBUG
    if (!img)std::cout << "img=null" << std::endl;
#else
    if (!img)return;
#endif
    int w = img->getwidth();
    int h = img->getheight();
    AlphaBlend(GetImageHDC(NULL), x, y, w, h,
               GetImageHDC(img), 0, 0, w, h, {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA});
}
void putImage(const Camera& camera, int x, int y, IMAGE* img)
{
    int w = img->getwidth();
    int h = img->getheight();
    const Vector2& pos_cam = camera.get_position();
    AlphaBlend(GetImageHDC(NULL), x + pos_cam.x, y + pos_cam.y, w, h,
               GetImageHDC(img), 0, 0, w, h, {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA});
}
void putImage(int x,int y,int w,int h,IMAGE* img,int src_x,int src_y)
{
    int width=w>0?w:img->getwidth();
    int height=h>0?h:img->getheight();
    AlphaBlend(GetImageHDC(NULL), x, y, width, height,
               GetImageHDC(img), src_x, src_y, width, height, {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA});
}

void line(const Camera& camera, int x1, int y1, int x2, int y2)
{
    const Vector2& pos_cam = camera.get_position();
    ::line(x1 + pos_cam.x, y1 + pos_cam.y, x2 + pos_cam.x, y2 + pos_cam.y);
}

void flip_image(IMAGE* src, IMAGE* dest)
{
    int w = src->getwidth();
    int h = src->getheight();
    Resize(dest, w, h);
    DWORD* src_buffer=GetImageBuffer(src);
    DWORD* dest_buffer=GetImageBuffer(dest);
    for(int y=0;y<h;y++)
    {
        for(int x=0;x<w;x++)
        {
            int idx_src=y*w+x;
            int idx_dst=y*w+(w-1-x);
            dest_buffer[idx_dst]=src_buffer[idx_src];
        }
    }
}
void sketch_image(IMAGE* src,IMAGE* dst){
    int w = src->getwidth();
    int h = src->getheight();
    Resize(dst, w, h);
    DWORD* src_buffer=GetImageBuffer(src);
    DWORD* dest_buffer=GetImageBuffer(dst);
    for(int y=0;y<h;y++)
    {
        for(int x=0;x<w;x++)
        {
            int idx_src=y*w+x;
            dest_buffer[idx_src]=BGR(RGB(255,255,255))|(src_buffer[idx_src]&0xFF000000);
        }
    }
}