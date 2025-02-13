#ifndef GAIN_DETECTOR_H
#define GAIN_DETECTOR_H

#include <stdint.h>
#include "macro.h"


class GainDetector
{	
public:
    GainDetector();

    void set_threshold(int expected_lvl);
    void set_range(float min, float max);

    void process(uint32_t rms);
    uint32_t get_gained(uint32_t s);

private:
    static constexpr int factor = 20;

    int gain;
    int min_gain;
    int max_gain;
    int threshold;
    int alpha;
};

#undef MULT64_SHIFT32
#undef MULT64_DIV32

#endif // ! GAIN_DETECTOR_H

