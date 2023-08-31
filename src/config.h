//
// Created by Yixiang Tu on 2019/6/1.
//

#ifndef LAB2_CONFIG_H
#define LAB2_CONFIG_H

/*
 * May switch to basic phone rendering to get a fast result
 * (without shades, of course)
 */

#define SQUARE
//#ifndef __APPLE__
//#define SPECIFY
//constexpr int Width = 1280;
//constexpr int Height = 1280;
//#else
//#endif


#ifdef __APPLE__
//#define USE_PHONG
constexpr int PM_BATCH_SIZE = 1e7;
constexpr int threads = 8;
constexpr int save_interval = 10;
//#define LARGE
#define SMALL
#else
constexpr int PM_BATCH_SIZE = 1e8;
constexpr int threads = 24;
constexpr int save_interval = 240;
#define LARGE
#endif

#ifdef USE_PHONG
#define SPECIFY
constexpr int Width = 1280;
constexpr int Height = 1280;
#endif

#if defined(SPECIFY)
#elif defined(XLARGE)
#ifdef SQUARE
constexpr int Width = 3072;
constexpr int Height = 3072;
#else
constexpr int Width = 4096;
constexpr int Height = 2560;
#endif
#elif defined(LARGE)
#ifdef SQUARE
constexpr int Width = 2048;
constexpr int Height = 2048;
#else
constexpr int Width = 2880;
constexpr int Height = 1800;
#endif
#elif defined(SMALL)
#ifdef SQUARE
constexpr int Width = 640;
constexpr int Height = 640;
#else
constexpr int Width = 800;
constexpr int Height = 500;
#endif
#elif defined(SQUARE)
#ifdef SQUARE
constexpr int Width = 1280;
constexpr int Height = 1280;
#else
constexpr int Width = 1600;
constexpr int Height = 1000;
#endif
#endif


#endif //LAB2_CONFIG_H
