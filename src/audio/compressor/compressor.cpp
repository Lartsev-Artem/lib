#include "compressor.h"
#include <math.h>

#define SIGN(a, res) ( ((a)<0) ? -(res):(res) )

void compressor::init_low_compression(compressor::settings_t* _params)
{
	_params->abs_limit = 0x7FFF;
	_params->threshold = 4000;
	_params->knee = 1500;

	_params->time_atack = 0.002f;
	_params->time_release = 0.05f;


	_params->ratio = 2;
	_params->threshold = 25000;
	_params->knee = 1500;
	_params->gain_threshold = (int)(1*_params->abs_limit);
	_params->gain_bound[0] = 0.25;
	_params->gain_bound[1] = 30;
}

void compressor::init_middle_compression(compressor::settings_t* _params)
{
	_params->abs_limit = 0x7FFF;
	_params->threshold = 4000;
	_params->knee = 1500;

	_params->time_atack = 0.002f;
	_params->time_release = 0.05f;


	_params->ratio = 4;
	_params->threshold = 22000;
	_params->knee = 1200;
	_params->gain_threshold = (int)(1.5*_params->abs_limit);
	_params->gain_bound[0] = 0.25;
	_params->gain_bound[1] = 64;
}

void compressor::init_high_compression(compressor::settings_t* _params)
{
	_params->abs_limit = 0x7FFF;
	_params->threshold = 4000;
	_params->knee = 1500;

	_params->time_atack = 0.002f;
	_params->time_release = 0.05f;

	_params->ratio = 16;
	_params->threshold = 27000;
	_params->knee = 3000;
	_params->gain_threshold = (int)(3*_params->abs_limit);
	_params->gain_bound[0] = 0.25;
	_params->gain_bound[1] = 90;
}

Compressor::Compressor():
	limiter(nullptr),
	rms(nullptr),
	tgain(nullptr),
	_gain(1),
	_threshold(2000),
	_ratio(1),
	_kneeHalf(1),
	_knee_coef{0,0}
{
}

Compressor::Compressor(const compressor::settings_t* _params, int fs)
{
	Compressor();

	setParam(_params->ratio,_params->threshold,1,_params->knee);

	LogLimiter* limit = new LogLimiter;
	limit->set_range((uint32_t)(0.9*_params->abs_limit), _params->abs_limit);
	limit->update_coef(this, (uint32_t)(_params->gain_bound[1]*0x7FFF));
	limiter = (VolumeLimiter*)limit;

	rms = new LevelDetector(fs);
	rms->setTimeAttack(_params->time_atack, fs);
	rms->setTimeRelease(_params->time_release, fs);

	tgain = new GainDetector();
	tgain->set_threshold(_params->gain_threshold);
	tgain->set_range(_params->gain_bound[0], _params->gain_bound[1]);
}

Compressor::~Compressor()
{
	if(limiter)
	{
		delete limiter;
	}

	if(rms)
	{
		delete rms;
	}

	if(tgain)
	{
		delete tgain;
	}
}


void Compressor::setParam(int ratio, int threshold, int gain, int knee)
{
	_gain = gain;
	_threshold = threshold;
	_kneeHalf = knee>>1;
	_ratio = ratio;

	double k = 0.5*(1.0/ratio+1.0);
	double m=((double )threshold - 0.5*knee)*(1-k);

	_knee_coef[0] = lround(k*(1<<Qfactor));
	_knee_coef[1] = lround(m*(1<<Qfactor));
}

uint32_t Compressor::process(uint32_t input) const
{
	int overshoot = (int)input - _threshold;

	if(overshoot < -_kneeHalf)
	{
		return input;
	}

	if(overshoot > _kneeHalf)
	{
		return (uint32_t)(_threshold + (overshoot/_ratio));

	}

	return (uint32_t)((_knee_coef[0] * (int)input + _knee_coef[1]) >> Qfactor);
}

void Compressor::Update(int16_t* output, const int16_t* input, uint32_t length)
{
	for(uint32_t i = 0; i<length; i++)
	{
		//uint32_t s = abs16(input[i]);
		uint32_t mask = (uint32_t)input[i] >> 15;
		uint32_t s = ((uint32_t)input[i]+mask) ^ mask;

		uint32_t lvl = rms->processPeakDecoupled(s);

		tgain->process(lvl);
		s = tgain->get_gained(s); //линейная величина

		s = this->process(s);

		s = limiter->process(s);

		output[i] = (int16_t)SIGN(input[i], s);
	}
}
