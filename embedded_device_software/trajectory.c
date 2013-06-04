#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/timer.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/interrupt.h"
#include "trajectory.h"
#include "PID.h"

#define NUM_POINTS 1000
#define DEFAULT_FREQ 1

extern PID* Controller;
unsigned long count = 0;
unsigned long trajectorySource = 0x00;

//long trajectoryD[NUM_POINTS] =
//{
//	2192,2262,2331,2399,2467,2534,2599,2663,2725,2785,2842,
//	2896,2948,2997,3043,3085,3123,3158,3189,3216,3239,3257,
//	3271,3281,3287,3288,3285,3277,3265,3248,3228,3203,3174,
//	3141,3104,3064,3020,2973,2923,2870,2813,2755,2694,2631,
//	2567,2501,2433,2365,2296,2227,2157,2088,2019,1951,1883,
//	1817,1753,1690,1629,1571,1514,1461,1411,1364,1320,1280,
//	1243,1210,1181,1156,1136,1119,1107,1099,1096,1097,1103,
//	1113,1127,1145,1168,1195,1226,1261,1299,1341,1387,1436,
//	1488,1542,1599,1659,1721,1785,1850,1917,1985,2053,2122,
//	2192
//};

long trajectoryD[NUM_POINTS] =
{
//		0,127,253,379,502,624,743,860,972,1081,1186,1286,1380,
//		1469,1552,1629,1699,1763,1819,1868,1910,1944,1970,1988,
//		1998,2000,1994,1980,1958,1928,1890,1845,1792,1732,1665,
//		1592,1511,1425,1334,1236,1134,1027,916,802,684,563,441,
//		316,190,63,-63,-190,-316,-441,-563,-684,-802,-916,-1027,
//		-1134,-1236,-1334,-1425,-1511,-1592,-1665,-1732,-1792,
//		-1845,-1890,-1928,-1958,-1980,-1994,-2000,-1998,-1988,
//		-1970,-1944,-1910,-1868,-1819,-1763,-1699,-1629,-1552,
//		-1469,-1380,-1286,-1186,-1081,-972,-860,-743,-624,-502,
//		-379,-253,-127,0
		//-500,-483,-466,-448,-431,-414,-397,-380,-363,-346,-328,-311,-294,-277,-260,-243,-226,-209,-192,-175,-158,-141,-124,-107,-90,-74,-57,-40,-23,-7,10,27,43,60,76,93,109,126,142,158,174,191,207,223,239,255,271,287,303,318,334,350,365,381,396,412,427,442,458,473,488,503,518,533,547,562,577,591,606,620,635,649,663,677,691,705,719,733,746,760,774,787,800,814,827,840,853,865,878,891,903,916,928,941,953,965,977,989,1000,1012,1023,1035,1046,1057,1069,1080,1090,1101,1112,1122,1133,1143,1153,1163,1173,1183,1193,1203,1212,1221,1231,1240,1249,1258,1266,1275,1284,1292,1300,1308,1316,1324,1332,1340,1347,1354,1362,1369,1376,1383,1389,1396,1402,1409,1415,1421,1427,1432,1438,1443,1449,1454,1459,1464,1469,1474,1478,1482,1487,1491,1495,1499,1502,1506,1509,1512,1516,1519,1521,1524,1527,1529,1531,1533,1535,1537,1539,1540,1542,1543,1544,1545,1546,1547,1547,1548,1548,1548,1548,1548,1547,1547,1546,1546,1545,1544,1542,1541,1540,1538,1536,1534,1532,1530,1528,1525,1523,1520,1517,1514,1511,1507,1504,1500,1497,1493,1489,1485,1480,1476,1471,1466,1462,1457,1451,1446,1441,1435,1430,1424,1418,1412,1405,1399,1393,1386,1379,1372,1365,1358,1351,1343,1336,1328,1320,1312,1304,1296,1288,1279,1271,1262,1253,1244,1235,1226,1217,1207,1198,1188,1178,1168,1158,1148,1138,1128,1117,1107,1096,1085,1074,1063,1052,1041,1029,1018,1006,994,983,971,959,947,934,922,910,897,885,872,859,846,833,820,807,794,780,767,753,740,726,712,698,684,670,656,642,628,613,599,584,570,555,540,525,510,495,480,465,450,435,419,404,389,373,358,342,326,310,295,279,263,247,231,215,199,182,166,150,134,117,101,84,68,51,35,18,2,-15,-32,-48,-65,-82,-99,-116,-133,-150,-166,-183,-200,-217,-234,-252,-269,-286,-303,-320,-337,-354,-371,-388,-406,-423,-440,-457,-474,-491,-509,-526,-543,-560,-577,-594,-612,-629,-646,-663,-680,-697,-714,-731,-748,-766,-783,-800,-817,-834,-850,-867,-884,-901,-918,-935,-952,-968,-985,-1002,-1018,-1035,-1051,-1068,-1084,-1101,-1117,-1134,-1150,-1166,-1182,-1199,-1215,-1231,-1247,-1263,-1279,-1295,-1310,-1326,-1342,-1358,-1373,-1389,-1404,-1419,-1435,-1450,-1465,-1480,-1495,-1510,-1525,-1540,-1555,-1570,-1584,-1599,-1613,-1628,-1642,-1656,-1670,-1684,-1698,-1712,-1726,-1740,-1753,-1767,-1780,-1794,-1807,-1820,-1833,-1846,-1859,-1872,-1885,-1897,-1910,-1922,-1934,-1947,-1959,-1971,-1983,-1994,-2006,-2018,-2029,-2041,-2052,-2063,-2074,-2085,-2096,-2107,-2117,-2128,-2138,-2148,-2158,-2168,-2178,-2188,-2198,-2207,-2217,-2226,-2235,-2244,-2253,-2262,-2271,-2279,-2288,-2296,-2304,-2312,-2320,-2328,-2336,-2343,-2351,-2358,-2365,-2372,-2379,-2386,-2393,-2399,-2405,-2412,-2418,-2424,-2430,-2435,-2441,-2446,-2451,-2457,-2462,-2466,-2471,-2476,-2480,-2485,-2489,-2493,-2497,-2500,-2504,-2507,-2511,-2514,-2517,-2520,-2523,-2525,-2528,-2530,-2532,-2534,-2536,-2538,-2540,-2541,-2542,-2544,-2545,-2546,-2546,-2547,-2547,-2548,-2548,-2548,-2548,-2548,-2547,-2547,-2546,-2545,-2544,-2543,-2542,-2540,-2539,-2537,-2535,-2533,-2531,-2529,-2527,-2524,-2521,-2519,-2516,-2512,-2509,-2506,-2502,-2499,-2495,-2491,-2487,-2482,-2478,-2474,-2469,-2464,-2459,-2454,-2449,-2443,-2438,-2432,-2427,-2421,-2415,-2409,-2402,-2396,-2389,-2383,-2376,-2369,-2362,-2354,-2347,-2340,-2332,-2324,-2316,-2308,-2300,-2292,-2284,-2275,-2266,-2258,-2249,-2240,-2231,-2221,-2212,-2203,-2193,-2183,-2173,-2163,-2153,-2143,-2133,-2122,-2112,-2101,-2090,-2080,-2069,-2057,-2046,-2035,-2023,-2012,-2000,-1989,-1977,-1965,-1953,-1941,-1928,-1916,-1903,-1891,-1878,-1865,-1853,-1840,-1827,-1814,-1800,-1787,-1774,-1760,-1746,-1733,-1719,-1705,-1691,-1677,-1663,-1649,-1635,-1620,-1606,-1591,-1577,-1562,-1547,-1533,-1518,-1503,-1488,-1473,-1458,-1442,-1427,-1412,-1396,-1381,-1365,-1350,-1334,-1318,-1303,-1287,-1271,-1255,-1239,-1223,-1207,-1191,-1174,-1158,-1142,-1126,-1109,-1093,-1076,-1060,-1043,-1027,-1010,-993,-977,-960,-943,-926,-910,-893,-876,-859,-842,-825,-808,-791,-774,-757,-740,-723,-706,-689,-672,-654,-637,-620,-603,-586,-569,-552,-534,-517,-500
		0,-8,-16,-25,-33,-41,-49,-57,-66,-74,-82,-90,-98,-107,-115,-123,-131,-140,-148,-156,-164,-172,-181,-189,-197,-205,-213,-222,-230,-238,-246,-254,-263,-271,-279,-287,-295,-304,-312,-320,-328,-336,-345,-353,-361,-369,-377,-386,-394,-402,-410,-419,-427,-435,-443,-451,-460,-468,-476,-484,-492,-501,-509,-517,-525,-533,-542,-550,-558,-566,-574,-583,-591,-599,-607,-615,-624,-632,-640,-648,-657,-665,-673,-681,-689,-698,-706,-714,-722,-730,-739,-747,-755,-763,-771,-780,-788,-796,-804,-812,-821,-829,-837,-845,-853,-862,-870,-878,-886,-894,-903,-911,-919,-927,-936,-944,-952,-960,-968,-977,-985,-993,-1001,-1009,-1018,-1026,-1034,-1042,-1050,-1059,-1067,-1075,-1083,-1091,-1100,-1108,-1116,-1124,-1132,-1141,-1149,-1157,-1165,-1174,-1182,-1190,-1198,-1206,-1215,-1223,-1231,-1239,-1247,-1256,-1264,-1272,-1280,-1288,-1297,-1305,-1313,-1321,-1329,-1338,-1346,-1354,-1362,-1370,-1379,-1387,-1395,-1403,-1412,-1420,-1428,-1436,-1444,-1453,-1461,-1469,-1477,-1485,-1494,-1502,-1510,-1518,-1526,-1535,-1543,-1551,-1559,-1567,-1576,-1584,-1592,-1600,-1608,-1617,-1625,-1633,-1641,-1649,-1658,-1666,-1674,-1682,-1691,-1699,-1707,-1715,-1723,-1732,-1740,-1748,-1756,-1764,-1773,-1781,-1789,-1797,-1805,-1814,-1822,-1830,-1838,-1846,-1855,-1863,-1871,-1879,-1887,-1896,-1904,-1912,-1920,-1929,-1937,-1945,-1953,-1961,-1970,-1978,-1986,-1994,-2002,-2011,-2019,-2027,-2035,-2043,-2052,-2060,-2068,-2076,-2084,-2093,-2101,-2109,-2117,-2125,-2134,-2142,-2150,-2158,-2166,-2175,-2183,-2191,-2199,-2208,-2216,-2224,-2232,-2240,-2249,-2257,-2265,-2273,-2281,-2290,-2298,-2306,-2314,-2322,-2331,-2339,-2347,-2355,-2363,-2372,-2380,-2388,-2396,-2404,-2413,-2421,-2429,-2437,-2446,-2454,-2462,-2470,-2478,-2487,-2495,-2503,-2511,-2519,-2528,-2536,-2544,-2552,-2560,-2569,-2577,-2585,-2593,-2601,-2610,-2618,-2626,-2634,-2642,-2651,-2659,-2667,-2675,-2683,-2692,-2700,-2708,-2716,-2725,-2733,-2741,-2749,-2757,-2766,-2774,-2782,-2790,-2798,-2807,-2815,-2823,-2831,-2839,-2848,-2856,-2864,-2872,-2880,-2889,-2897,-2905,-2913,-2921,-2930,-2938,-2946,-2954,-2963,-2971,-2979,-2987,-2995,-3004,-3012,-3020,-3028,-3036,-3045,-3053,-3061,-3069,-3077,-3086,-3094,-3102,-3110,-3118,-3127,-3135,-3143,-3151,-3159,-3168,-3176,-3184,-3192,-3201,-3209,-3217,-3225,-3233,-3242,-3250,-3258,-3266,-3274,-3283,-3291,-3299,-3307,-3315,-3324,-3332,-3340,-3348,-3356,-3365,-3373,-3381,-3389,-3397,-3406,-3414,-3422,-3430,-3438,-3447,-3455,-3463,-3471,-3480,-3488,-3496,-3504,-3512,-3521,-3529,-3537,-3545,-3553,-3562,-3570,-3578,-3586,-3594,-3603,-3611,-3619,-3627,-3635,-3644,-3652,-3660,-3668,-3676,-3685,-3693,-3701,-3709,-3718,-3726,-3734,-3742,-3750,-3759,-3767,-3775,-3783,-3791,-3800,-3808,-3816,-3824,-3832,-3841,-3849,-3857,-3865,-3873,-3882,-3890,-3898,-3906,-3914,-3923,-3931,-3939,-3947,-3955,-3964,-3972,-3980,-3988,-3997,-4005,-4013,-4021,-4029,-4038,-4046,-4054,-4062,-4070,-4079,-4087,-4095,-4095,-4087,-4079,-4070,-4062,-4054,-4046,-4038,-4029,-4021,-4013,-4005,-3997,-3988,-3980,-3972,-3964,-3955,-3947,-3939,-3931,-3923,-3914,-3906,-3898,-3890,-3882,-3873,-3865,-3857,-3849,-3841,-3832,-3824,-3816,-3808,-3800,-3791,-3783,-3775,-3767,-3759,-3750,-3742,-3734,-3726,-3718,-3709,-3701,-3693,-3685,-3676,-3668,-3660,-3652,-3644,-3635,-3627,-3619,-3611,-3603,-3594,-3586,-3578,-3570,-3562,-3553,-3545,-3537,-3529,-3521,-3512,-3504,-3496,-3488,-3480,-3471,-3463,-3455,-3447,-3438,-3430,-3422,-3414,-3406,-3397,-3389,-3381,-3373,-3365,-3356,-3348,-3340,-3332,-3324,-3315,-3307,-3299,-3291,-3283,-3274,-3266,-3258,-3250,-3242,-3233,-3225,-3217,-3209,-3201,-3192,-3184,-3176,-3168,-3159,-3151,-3143,-3135,-3127,-3118,-3110,-3102,-3094,-3086,-3077,-3069,-3061,-3053,-3045,-3036,-3028,-3020,-3012,-3004,-2995,-2987,-2979,-2971,-2963,-2954,-2946,-2938,-2930,-2921,-2913,-2905,-2897,-2889,-2880,-2872,-2864,-2856,-2848,-2839,-2831,-2823,-2815,-2807,-2798,-2790,-2782,-2774,-2766,-2757,-2749,-2741,-2733,-2725,-2716,-2708,-2700,-2692,-2683,-2675,-2667,-2659,-2651,-2642,-2634,-2626,-2618,-2610,-2601,-2593,-2585,-2577,-2569,-2560,-2552,-2544,-2536,-2528,-2519,-2511,-2503,-2495,-2487,-2478,-2470,-2462,-2454,-2446,-2437,-2429,-2421,-2413,-2404,-2396,-2388,-2380,-2372,-2363,-2355,-2347,-2339,-2331,-2322,-2314,-2306,-2298,-2290,-2281,-2273,-2265,-2257,-2249,-2240,-2232,-2224,-2216,-2208,-2199,-2191,-2183,-2175,-2166,-2158,-2150,-2142,-2134,-2125,-2117,-2109,-2101,-2093,-2084,-2076,-2068,-2060,-2052,-2043,-2035,-2027,-2019,-2011,-2002,-1994,-1986,-1978,-1970,-1961,-1953,-1945,-1937,-1929,-1920,-1912,-1904,-1896,-1887,-1879,-1871,-1863,-1855,-1846,-1838,-1830,-1822,-1814,-1805,-1797,-1789,-1781,-1773,-1764,-1756,-1748,-1740,-1732,-1723,-1715,-1707,-1699,-1691,-1682,-1674,-1666,-1658,-1649,-1641,-1633,-1625,-1617,-1608,-1600,-1592,-1584,-1576,-1567,-1559,-1551,-1543,-1535,-1526,-1518,-1510,-1502,-1494,-1485,-1477,-1469,-1461,-1453,-1444,-1436,-1428,-1420,-1412,-1403,-1395,-1387,-1379,-1370,-1362,-1354,-1346,-1338,-1329,-1321,-1313,-1305,-1297,-1288,-1280,-1272,-1264,-1256,-1247,-1239,-1231,-1223,-1215,-1206,-1198,-1190,-1182,-1174,-1165,-1157,-1149,-1141,-1132,-1124,-1116,-1108,-1100,-1091,-1083,-1075,-1067,-1059,-1050,-1042,-1034,-1026,-1018,-1009,-1001,-993,-985,-977,-968,-960,-952,-944,-936,-927,-919,-911,-903,-894,-886,-878,-870,-862,-853,-845,-837,-829,-821,-812,-804,-796,-788,-780,-771,-763,-755,-747,-739,-730,-722,-714,-706,-698,-689,-681,-673,-665,-657,-648,-640,-632,-624,-615,-607,-599,-591,-583,-574,-566,-558,-550,-542,-533,-525,-517,-509,-501,-492,-484,-476,-468,-460,-451,-443,-435,-427,-419,-410,-402,-394,-386,-377,-369,-361,-353,-345,-336,-328,-320,-312,-304,-295,-287,-279,-271,-263,-254,-246,-238,-230,-222,-213,-205,-197,-189,-181,-172,-164,-156,-148,-140,-131,-123,-115,-107,-98,-90,-82,-74,-66,-57,-49,-41,-33,-25,-16,-8,0


};

long trajectory1 = 0;

void initTrajectoryTimerInt(void) {
	unsigned long ulPeriod = 0;
	//Enable Timer 3
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3);
	//Initialize timer period to default setting
	ulPeriod = SysCtlClockGet()/(NUM_POINTS*DEFAULT_FREQ);
	TimerConfigure(TIMER3_BASE, TIMER_CFG_PERIODIC);
	TimerLoadSet(TIMER3_BASE, TIMER_A, ulPeriod);
	//Enable interrupt for timer 3
	IntEnable(INT_TIMER3A);
	//Configure Timer3A interrupt
	TimerIntEnable(TIMER3_BASE, TIMER_TIMA_TIMEOUT);
	//Enable Timer3A
	//TimerEnable(TIMER3_BASE, TIMER_A);
	//IntMasterEnable();
}

void setTrajectoryHandler(void) {
	TimerIntClear(TIMER3_BASE, TIMER_TIMA_TIMEOUT);
	Controller->x = (trajectoryD + count);
	count = (count + 1) % NUM_POINTS;
}

void setTrajectorySource(unsigned long src){
	trajectorySource = src;
}

void setTrajectoryTimer(unsigned long newFreq) {
	unsigned long ulPeriod = 0;
	//Calculate desired period of the timer
	ulPeriod = SysCtlClockGet()/(NUM_POINTS*newFreq);
	//Set timer to the desired period
	TimerLoadSet(TIMER3_BASE, TIMER_A, ulPeriod);
}

void trajectoryEnable(void) {
	//Enable Timer3A
	if(trajectorySource == 0){
		TimerEnable(TIMER3_BASE, TIMER_A);
	}else{
		Controller->x = &trajectory1;
	}
}

void setTrajectoryElement(signed long pt){
	trajectory1 = pt;
}

void trajectoryDisable(void) {
	//Disable Timer3A
	TimerDisable(TIMER3_BASE, TIMER_A);
}
