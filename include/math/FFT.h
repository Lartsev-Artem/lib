#pragma once

#include <stdint.h>

typedef double real_type;

#define FFT_SCALE_FACTOR 10 //для целочисленной арифметики

#ifndef M_PI
#define M_PI 3.141592653589793
#endif


static inline bool isPowerOfTwo(const uint32_t num)
{
	return num && (!(num & (num - 1)));
}

int sinTab(int num_elements, real_type*& Sin);
int sinTab(int num_elements, int16_t*& Sin);

bool FFT(real_type* data, const size_t num_elements);
bool FFT(int16_t* data, const size_t num_elements);

#if 0
#include <iostream>
//#define OUTPUT_LOG
static inline void FFT_REAL_TEST(int L)
{
	int FS = 48000;	
	int fc = 10000;

	real_type* data = new real_type[2 * L];

	for (size_t i = 0; i < L; i++)
	{
		data[2 * i] = 100 * sin(2 * M_PI * i / FS * fc);
		data[2 * i + 1] = 0;
	}
	
	if (!FFT(data, L))
	{
		std::cout << "Error size!\n";
		return;
	}	
#ifdef OUTPUT_LOG
	for (size_t i = 0; i < L; i++)
	{
		//std::cout << abs(data[2 * i]) << ", ";		
		std::cout << sqrt(data[2 * i]* data[2 * i] + data[2 * i+1]* data[2 * i+1]) << ", ";
	}
	std::cout << "\n";
#endif // OUTPUT_LOG

	delete[] data;
}

static inline void FFT_INT_TEST(int L)
{
	int FS = 48000;	
	int fc = 10000;

	int16_t* data = new int16_t[2 * L];

	for (size_t i = 0; i < L; i++)
	{
		data[2 * i] = 100 * sin(2 * M_PI * i / FS * fc);
		data[2 * i + 1] = 0;
	}
	
	if (!FFT(data, L))
	{
		std::cout << "Error size!\n";
		return;
	}	
#ifdef OUTPUT_LOG
	for (size_t i = 0; i < L; i++)
	{
		std::cout << sqrt(data[2 * i] * data[2 * i] + data[2 * i + 1] * data[2 * i + 1]) << ", ";
		//std::cout << abs(data[2 * i]) << ", ";
	}
	std::cout << "\n";
#endif // OUTPUT_LOG

	delete[] data;
}
#include <chrono>
static inline int run_main()
{
	auto t = std::chrono::system_clock::now();
	FFT_INT_TEST(1024 * 1024);
	std::cout << "Time INT: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - t) << "\n";

	t = std::chrono::system_clock::now();
	FFT_REAL_TEST(1024 * 1024);
	std::cout << "Time REAL: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - t) << "\n";

	system("pause");
	return 0;
}

#endif