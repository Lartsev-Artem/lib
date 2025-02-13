#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <stdint.h>

#include "volume_limiter.h"
#include "level_detector.h"
#include "gain_detector.h"
namespace compressor
{
    struct settings_t
    {
        int ratio;		///< коэффициент сжати€ (в линейной части)
        int knee;		///< ширина области "м€гкого сжати€"
        int threshold;	///< амплитуда начала компрессии

        int gain_threshold;	 ///< номинал усилени€ (на этот уровень выт€гиваем сигнал (до сжати€))
        float gain_bound[2]; ///< границы коэффициента усилени€ (min,max)

        uint32_t abs_limit;	 ///< абсолютный предел сигнала (дл€ лимитера)

        float time_atack;	///< врем€ реакции на усиление сигнала (в сек.)
        float time_release; ///< врем€ реакции на ослабление сигнала (в сек.)
    };
    void init_low_compression(compressor::settings_t* _params);
    void init_middle_compression(compressor::settings_t* _params);
    void init_high_compression(compressor::settings_t* _params);
}

class Compressor
{
public:
    enum e_mode_compression_t
    {
        e_compression_no = -1,
        e_compression_low,
        e_compression_middle,
        e_compression_high,
        e_compression_count
    };

    Compressor();
    Compressor(const compressor::settings_t* _params, int fs);
    ~Compressor();
    void setParam(int ratio, int threshold, int gain, int knee);
    void Update(int16_t* output, const int16_t* input, uint32_t length);
    uint32_t process(uint32_t input) const;

public:
    VolumeLimiter* limiter;
    LevelDetector* rms;
    GainDetector* tgain;

private:
    friend class LogLimiter;
    static constexpr int64_t Qfactor = 10;

    int _gain;
    int _threshold;
    int _ratio;
    int _kneeHalf;
    int _knee_coef[2];


};


#endif // ! COMPRESSOR_H

