#include "level_detector.h"
#include <math.h>

LevelDetector::LevelDetector(int fs)
{
	setTimeAttack(0.01f, fs);
	setTimeRelease(0.14f, fs);
	state01 = state02 = 0;
}

void LevelDetector::setTimeAttack(float attack, int fs)
{
	alphaAttack = (uint64_t)llround((1<<Q_factor)*exp(-1.0 / (fs * (double)attack))); //aA = e^(-1/TA*fs)
}

void LevelDetector::setTimeRelease(float release, int fs)
{
	alphaRelease = (uint64_t)llround((1<<Q_factor)*exp(-1.0 / (fs * (double)release))); //aR = e^(-1/TR*fs)
}

//Smooth branched peak detector
uint32_t LevelDetector::processPeakBranched(uint32_t in)
{
	uint64_t tmp = ((uint64_t)in) << Q_factor;
	if (tmp < state01)
		state01 = ((alphaAttack * state01)>>Q_factor) + ((1<<Q_factor) - alphaAttack) * in;
	else
		state01 = ((alphaRelease * state01)>>Q_factor) + ((1<<Q_factor) - alphaRelease) * in;

	return (uint32_t)(state01>>Q_factor);
}

uint32_t LevelDetector::processPeakDecoupled(uint32_t in)
{
	state02 = ((alphaRelease * state02)>>Q_factor) + ((1<<Q_factor) - alphaRelease) * in;
	uint64_t tmp = ((uint64_t)in) << Q_factor;
#if 0
	max64(state02, tmp);
#else
	state02 = state02 ^ ((state02^tmp) & -(state02<tmp)); //работает быстрее
#endif

	state01 = ((alphaAttack * state01) + ( ((1<<Q_factor) - alphaAttack) * state02)) >> Q_factor;
	return (uint32_t)(state01 >> Q_factor);
}
