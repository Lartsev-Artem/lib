#ifndef LEVEL_DETECTOR_H
#define LEVEL_DETECTOR_H

#include <stdint.h>

class LevelDetector
{	
public:

    LevelDetector(int fs);

    void setTimeAttack(float attack, int fs);
    void setTimeRelease(float release,int fs);

    uint32_t processPeakBranched(uint32_t in);
    uint32_t processPeakDecoupled(uint32_t in);

private:
    static constexpr uint32_t Q_factor = 20;
    uint64_t alphaAttack;
    uint64_t alphaRelease;
    uint64_t state01;
    uint64_t state02;
};


#endif // ! LEVEL_DETECTOR_H
