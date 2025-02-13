#include "gain_detector.h"

#include "macro.h"
#include <math.h>


//(a*b)>>scale
#define MULT64_SHIFT32(a,b,scale) ((uint32_t) ( ( ( (uint64_t)(a) ) * ( (uint64_t)(b) )) >> (scale)  ))

//(a*b)/scale
#define MULT64_DIV32(a,b,scale) ((uint32_t) ( ( ( (uint64_t)(a) ) * ( (uint64_t)(b) )) / (scale)  ))

GainDetector::GainDetector() :
	gain(1),
	min_gain((int)(0.5f*(1<<factor))),
	max_gain(2*(1<<factor)),
	threshold(500),
	alpha((int)(0.995f*(1<<factor)))
{	
}

uint32_t GainDetector::get_gained(uint32_t s)
{
	return MULT64_SHIFT32(gain,s,factor);
}

void GainDetector::set_range(float min, float max)
{
	min_gain = lroundf((1<<factor)*min);
	max_gain = lroundf((1<<factor)*max);
	gain = (max_gain+min_gain) >> 1;
}

void GainDetector::set_threshold(int expected_lvl)
{
	threshold = expected_lvl;
}

void GainDetector::process(uint32_t rms)
{
	if(UNLIKELY(rms == 0))
	{
		return;
	}

	gain = (int)(MULT64_SHIFT32(gain,alpha,factor) + MULT64_DIV32( ((1<<factor)-alpha),threshold,rms));
	gain = gain ^ ((gain^min_gain) & -(gain<min_gain)); //max
	gain = gain ^ ((gain^max_gain) & -(gain>max_gain)); //min
}
