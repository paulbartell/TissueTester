#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/timer.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "trajectory.h"
#include "PID.h"

extern PID* Controller;

static const unsigned float trajectory[] = 
{
	2192,2262,2331,2399,2467,2534,2599,2663,2725,2785,2842,
	2896,2948,2997,3043,3085,3123,3158,3189,3216,3239,3257,
	3271,3281,3287,3288,3285,3277,3265,3248,3228,3203,3174,
	3141,3104,3064,3020,2973,2923,2870,2813,2755,2694,2631,
	2567,2501,2433,2365,2296,2227,2157,2088,2019,1951,1883,
	1817,1753,1690,1629,1571,1514,1461,1411,1364,1320,1280,
	1243,1210,1181,1156,1136,1119,1107,1099,1096,1097,1103,
	1113,1127,1145,1168,1195,1226,1261,1299,1341,1387,1436,
	1488,1542,1599,1659,1721,1785,1850,1917,1985,2053,2122,
	2192
};

void initTrajectoryTimer(void) {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3);


}

void setTrajectory(unsigned long newFreq) {
	
}

void setTimer(unsigned long newFreq) {

}

