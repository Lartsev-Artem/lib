#include "volume_limiter.h"
#include "compressor.h"
#include "amp_db_converter.h"

#include <math.h>

SimpleLimiter::SimpleLimiter() :max_level(1000)
{

}
void SimpleLimiter::set_max_lvl(uint32_t a)
{
	max_level= a;
}
uint32_t SimpleLimiter::process(uint32_t in) const
{
	if(in > max_level)
	{
		return max_level;
	}

	return in;
}


LogLimiter::LogLimiter():
	max_level(1000),
	min_level(900),
	coefs{0,900}
{		
}

void LogLimiter::set_range(uint32_t min, uint32_t max)
{
	min_level = min;
	max_level = max;	
}

void LogLimiter::update_coef(const Compressor* compressor, uint32_t abs_max_level)
{	
	double k_max_sig = compressor->process(abs_max_level);

	double c0 =((double)(max_level-min_level))/(log10(k_max_sig)-log10(min_level));
	double c1 = min_level-c0*log10(min_level);

	coefs[0]=llround(c0*(1<<Qfactor)/20); //20 - множитель в дЅ
	coefs[1]=llround(c1*(1<<Qfactor));
}

uint32_t LogLimiter::process(uint32_t in) const
{
	if(in > min_level)
	{
#if 0
		/// \note: если усиленный сигнал выйдет за пределы предполагаемого максимума,
		/// то значение логарифма выйдет за пределы установленного максимума, поэтому дополнительно введена проверка на минимум
		/// другой вариант вызывать update_coef(comp, 0x7FFF*max_gain), т.е. согласованно с усилением
		uint32_t lg = (uint32_t)((((coefs[0]*(int64_t)compressor::to_db(in)) >> Compressor::Qfactor) + coefs[1]) >> Qfactor);
		return  max_level ^ ((max_level^lg) & -(max_level>lg));
#else
		return (uint32_t)((((coefs[0]*(int64_t)compressor::to_db(in)) >> Compressor::Qfactor) + coefs[1]) >> Qfactor);
#endif
	}

	return in;
}
