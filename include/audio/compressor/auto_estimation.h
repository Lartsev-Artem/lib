#ifndef AUTO_ESTIMATION_H
#define AUTO_ESTIMATION_H

#include <stdint.h>

class AutoEstimation
{
public:    

	AutoEstimation();

	float avgAttackTime;	///< среднее время атаки (c)
	float avgReleaseTime;  ///< среднее время релаксации (c)

	float maxAttackTime;	///< максимальное время атаки (c)
	float maxReleaseTime;	///< максимальное время релаксации (c)

	float alpha;		///< коэффициент фильтра
	float peakState;	///< фильторованые скачки сигнала
	float rmsState;		///< фильторованые уровень сигнала


	void prepare(int fs, int16_t avg_signal, float release_time = 0.2f);
	void process(float src);
	void process(const float* src, int numSamples);

};

#endif // ! AUTO_ESTIMATION_H
