#include "auto_estimation.h"

#include <math.h>
#include <algorithm>

AutoEstimation::AutoEstimation():
	avgAttackTime(0.01f),
	avgReleaseTime(0.14f),
	maxAttackTime(0.08f),
	maxReleaseTime(1.0f),
	alpha(0.9999f),
	peakState(0),
	rmsState(0)
{

}

void AutoEstimation::prepare(int fs, int16_t avg_signal, float release_time)
{
	//Calculate alpha for release time of 200ms, same release time for peak & rms detector
	alpha = expf(-1.0f / (fs * release_time));

	//Init accumulators
    peakState = abs(avg_signal);
    rmsState = abs(avg_signal);
}

void AutoEstimation::process(float src)
{
	const float s = src * src;

	//Update peak state
	peakState = std::max(s, alpha * peakState + (1-alpha) * s);

	//Update rms state
	rmsState = alpha * rmsState + (1-alpha) * s;

	//calculate squared crest factor
	const float cFactor = peakState / rmsState;

	//calculate ballistics
	if (cFactor > 0.0f)
	{
		avgAttackTime = 2 * (maxAttackTime / cFactor);
		avgReleaseTime = 2 * (maxReleaseTime / cFactor) - avgAttackTime;
	}
}

void AutoEstimation::process(const float* src, int numSamples)
{
	//Reset avg attack/release
	avgAttackTime = 0.0;
	avgReleaseTime = 0.0;

	//Calculate averages of auto - attack/release times for a single buffer
	for (int i = 0; i < numSamples; ++i)
	{
		//Square of input signal
		const float s = (src[i])*(src[i]);

		//Update peak state
		peakState = std::max(s, alpha * peakState + (1-alpha) * s);

		//Update rms state
		rmsState = alpha * rmsState + (1-alpha) * s;

		//calculate squared crest factor
		float cFactor = peakState / rmsState;

		//calculate ballistics
		if (cFactor > 0.0f)
		{
			float attackTimeInSeconds = 2 * (maxAttackTime / cFactor);
			float releaseTimeInSeconds = 2 * (maxReleaseTime / cFactor) - attackTimeInSeconds;

			//Update avg ballistics
			avgAttackTime += attackTimeInSeconds;
			avgReleaseTime += releaseTimeInSeconds;
		}
	}

	// Calculate average ballistics & crest factor
	avgAttackTime /= numSamples;
	avgReleaseTime /= numSamples;
}
