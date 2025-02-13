#ifndef VOLUME_LIMITER_H
#define VOLUME_LIMITER_H

#include <stdint.h>

class Compressor;

class VolumeLimiter
{
public:
	virtual ~VolumeLimiter() {}
	virtual uint32_t process(uint32_t in) const = 0;
};

class SimpleLimiter : public VolumeLimiter
{	
public:	  
	SimpleLimiter();
	~SimpleLimiter() override {}

	void set_max_lvl(uint32_t a);
	uint32_t process(uint32_t in) const override;

private:
	uint32_t max_level;
};

class LogLimiter : public VolumeLimiter
{
	static constexpr int64_t Qfactor = 10;
public:
	LogLimiter();
	~LogLimiter() override {}

	void set_range(uint32_t min, uint32_t max);
	void update_coef(const Compressor* compressor, uint32_t abs_max_level);
	uint32_t process(uint32_t in) const override;

private:	
	uint32_t max_level; ///< максимальный уровень выходного сигнала
	uint32_t min_level; ///< уровень с которого начинается лимитирование
	int64_t coefs[2];	 ///< коэффициенты лимитирующей A*log(x)+B
};

#endif // ! VOLUME_LIMITER_H
