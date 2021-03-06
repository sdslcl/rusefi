/*
 * @file mazda_miata_1_6.cpp
 *
 *
 * https://github.com/rusefi/rusefi/wiki/Frankenso_MazdaMiataNA6_pnp
 *
 *
 * Frankenso board
 * set engine_type 41
 *
 * @date Apr 16, 2016
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "custom_engine.h"
#include "fsio_impl.h"
#include "thermistors.h"
#include "mazda_miata_1_6.h"
#include "mazda_miata_base_maps.h"

EXTERN_CONFIG;

static const float hardCodedmafDecodingBins[256] = {0.099999905, 0.19999981, 0.29999924, 0.39999962, 0.5, 0.5999985, 0.70000076, 0.79999924, 0.9000015, 1.0, 1.0999985, 1.199997, 1.300003, 1.4000015, 1.5, 1.5999985, 1.699997, 1.800003, 1.9000015, 2.0, 2.100006, 2.2000122, 2.2999878, 2.399994, 2.5, 2.600006, 2.7000122, 2.7999878, 2.899994, 3.0, 3.100006, 3.2000122, 3.2999878, 3.399994, 3.5, 3.600006, 3.7000122, 3.7999878, 3.899994, 4.0, 4.0999756, 4.200012, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988, 4.299988};

static const float hardCodedmafDecoding[256] = {570.0, 437.0, 351.5, 293.55078, 237.0, 186.0, 145.0, 116.0, 96.0, 78.84961, 65.549805, 56.049805, 48.0, 40.0, 33.25, 29.449951, 25.649902, 22.800049, 20.899902, 19.0, 17.100098, 16.149902, 13.775024, 12.824951, 11.400024, 10.449951, 9.5, 8.550049, 8.454956, 8.359985, 8.300049, 8.199951, 7.5999756, 7.125, 7.125, 7.125, 6.6500244, 6.6500244, 6.6500244, 6.6500244, 6.6500244, 6.6500244, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

/* Generated by TS2C on Fri Jul 31 14:02:18 EDT 2020*/
static void setMafDecodingBins(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	MEMCPY(config->mafDecodingBins, hardCodedmafDecodingBins);
	MEMCPY(config->mafDecoding, hardCodedmafDecoding);
}

static const float hardCodedveLoadBins[16] = {20.0, 30.0, 34.0, 38.0, 43.0, 48.0, 54.0, 60.0, 67.0, 74.0, 82.0, 90.0, 99.0, 108.0, 118.0, 128.0};

static const float hardCodedveRpmBins[16] = {650.0, 950.0, 1250.0, 1600.0, 1950.0, 2350.0, 2750.0, 3200.0, 3700.0, 4200.0, 4700.0, 5200.0, 5800.0, 6400.0, 7100.0, 7800.0};

static const float hardCodedveTable[16][16] = {
/* Generated by TS2C on Sun Aug 02 10:02:25 EDT 2020*/
{/* 0 20.000	*//* 0 650.0*/40.592,	/* 1 950.0*/42.715,	/* 2 1250.0*/44.205,	/* 3 1600.0*/44.998,	/* 4 1950.0*/45.586,	/* 5 2350.0*/45.141,	/* 6 2750.0*/42.739,	/* 7 3200.0*/41.176,	/* 8 3700.0*/39.921,	/* 9 4200.0*/41.272,	/* 10 4700.0*/43.726,	/* 11 5200.0*/47.937,	/* 12 5800.0*/52.229,	/* 13 6400.0*/55.654,	/* 14 7100.0*/58.140,	/* 15 7800.0*/58.763,	},
{/* 1 30.000	*//* 0 650.0*/46.506,	/* 1 950.0*/49.126,	/* 2 1250.0*/49.847,	/* 3 1600.0*/49.706,	/* 4 1950.0*/49.866,	/* 5 2350.0*/50.828,	/* 6 2750.0*/51.849,	/* 7 3200.0*/50.064,	/* 8 3700.0*/48.359,	/* 9 4200.0*/49.920,	/* 10 4700.0*/54.116,	/* 11 5200.0*/58.954,	/* 12 5800.0*/64.546,	/* 13 6400.0*/66.713,	/* 14 7100.0*/67.738,	/* 15 7800.0*/68.457,	},
{/* 2 34.000	*//* 0 650.0*/51.408,	/* 1 950.0*/55.035,	/* 2 1250.0*/56.457,	/* 3 1600.0*/55.807,	/* 4 1950.0*/56.117,	/* 5 2350.0*/57.554,	/* 6 2750.0*/59.953,	/* 7 3200.0*/58.324,	/* 8 3700.0*/55.696,	/* 9 4200.0*/56.141,	/* 10 4700.0*/59.908,	/* 11 5200.0*/65.525,	/* 12 5800.0*/70.589,	/* 13 6400.0*/71.985,	/* 14 7100.0*/72.969,	/* 15 7800.0*/73.576,	},
{/* 3 38.000	*//* 0 650.0*/55.131,	/* 1 950.0*/58.797,	/* 2 1250.0*/60.825,	/* 3 1600.0*/62.831,	/* 4 1950.0*/62.511,	/* 5 2350.0*/63.555,	/* 6 2750.0*/66.933,	/* 7 3200.0*/66.365,	/* 8 3700.0*/63.211,	/* 9 4200.0*/63.721,	/* 10 4700.0*/67.063,	/* 11 5200.0*/72.315,	/* 12 5800.0*/76.385,	/* 13 6400.0*/78.133,	/* 14 7100.0*/78.436,	/* 15 7800.0*/78.577,	},
{/* 4 43.000	*//* 0 650.0*/62.427,	/* 1 950.0*/64.929,	/* 2 1250.0*/68.336,	/* 3 1600.0*/70.803,	/* 4 1950.0*/70.206,	/* 5 2350.0*/71.101,	/* 6 2750.0*/74.762,	/* 7 3200.0*/74.345,	/* 8 3700.0*/71.209,	/* 9 4200.0*/70.365,	/* 10 4700.0*/73.419,	/* 11 5200.0*/78.221,	/* 12 5800.0*/82.753,	/* 13 6400.0*/83.884,	/* 14 7100.0*/84.646,	/* 15 7800.0*/84.760,	},
{/* 5 48.000	*//* 0 650.0*/69.850,	/* 1 950.0*/72.581,	/* 2 1250.0*/75.672,	/* 3 1600.0*/78.355,	/* 4 1950.0*/78.085,	/* 5 2350.0*/80.211,	/* 6 2750.0*/82.045,	/* 7 3200.0*/81.545,	/* 8 3700.0*/78.744,	/* 9 4200.0*/78.467,	/* 10 4700.0*/78.896,	/* 11 5200.0*/83.297,	/* 12 5800.0*/87.946,	/* 13 6400.0*/90.166,	/* 14 7100.0*/91.174,	/* 15 7800.0*/91.355,	},
{/* 6 54.000	*//* 0 650.0*/75.722,	/* 1 950.0*/79.103,	/* 2 1250.0*/82.361,	/* 3 1600.0*/84.630,	/* 4 1950.0*/84.046,	/* 5 2350.0*/85.438,	/* 6 2750.0*/86.868,	/* 7 3200.0*/86.240,	/* 8 3700.0*/84.198,	/* 9 4200.0*/83.970,	/* 10 4700.0*/85.578,	/* 11 5200.0*/89.898,	/* 12 5800.0*/93.576,	/* 13 6400.0*/96.423,	/* 14 7100.0*/97.712,	/* 15 7800.0*/98.025,	},
{/* 7 60.000	*//* 0 650.0*/80.931,	/* 1 950.0*/83.518,	/* 2 1250.0*/86.618,	/* 3 1600.0*/90.079,	/* 4 1950.0*/88.666,	/* 5 2350.0*/88.837,	/* 6 2750.0*/90.304,	/* 7 3200.0*/89.797,	/* 8 3700.0*/89.359,	/* 9 4200.0*/89.215,	/* 10 4700.0*/91.475,	/* 11 5200.0*/95.601,	/* 12 5800.0*/98.813,	/* 13 6400.0*/101.662,	/* 14 7100.0*/103.927,	/* 15 7800.0*/104.533,	},
{/* 8 67.000	*//* 0 650.0*/85.475,	/* 1 950.0*/87.373,	/* 2 1250.0*/90.576,	/* 3 1600.0*/94.341,	/* 4 1950.0*/93.254,	/* 5 2350.0*/92.835,	/* 6 2750.0*/94.631,	/* 7 3200.0*/93.803,	/* 8 3700.0*/93.489,	/* 9 4200.0*/95.245,	/* 10 4700.0*/97.738,	/* 11 5200.0*/101.731,	/* 12 5800.0*/105.341,	/* 13 6400.0*/107.920,	/* 14 7100.0*/109.300,	/* 15 7800.0*/110.264,	},
{/* 9 74.000	*//* 0 650.0*/90.588,	/* 1 950.0*/92.696,	/* 2 1250.0*/94.371,	/* 3 1600.0*/99.087,	/* 4 1950.0*/98.227,	/* 5 2350.0*/98.105,	/* 6 2750.0*/99.515,	/* 7 3200.0*/98.095,	/* 8 3700.0*/99.186,	/* 9 4200.0*/101.554,	/* 10 4700.0*/105.278,	/* 11 5200.0*/108.206,	/* 12 5800.0*/111.131,	/* 13 6400.0*/112.939,	/* 14 7100.0*/114.120,	/* 15 7800.0*/114.274,	},
{/* 10 82.000	*//* 0 650.0*/96.035,	/* 1 950.0*/96.933,	/* 2 1250.0*/99.341,	/* 3 1600.0*/102.952,	/* 4 1950.0*/103.774,	/* 5 2350.0*/103.973,	/* 6 2750.0*/103.334,	/* 7 3200.0*/105.005,	/* 8 3700.0*/106.212,	/* 9 4200.0*/109.098,	/* 10 4700.0*/111.624,	/* 11 5200.0*/114.729,	/* 12 5800.0*/116.929,	/* 13 6400.0*/118.501,	/* 14 7100.0*/118.165,	/* 15 7800.0*/118.979,	},
{/* 11 90.000	*//* 0 650.0*/100.834,	/* 1 950.0*/101.388,	/* 2 1250.0*/103.619,	/* 3 1600.0*/107.806,	/* 4 1950.0*/107.164,	/* 5 2350.0*/106.776,	/* 6 2750.0*/106.612,	/* 7 3200.0*/108.891,	/* 8 3700.0*/112.385,	/* 9 4200.0*/114.962,	/* 10 4700.0*/117.627,	/* 11 5200.0*/120.118,	/* 12 5800.0*/121.564,	/* 13 6400.0*/122.254,	/* 14 7100.0*/122.201,	/* 15 7800.0*/121.964,	},
{/* 12 99.000	*//* 0 650.0*/104.415,	/* 1 950.0*/105.773,	/* 2 1250.0*/107.849,	/* 3 1600.0*/109.719,	/* 4 1950.0*/109.161,	/* 5 2350.0*/108.677,	/* 6 2750.0*/108.340,	/* 7 3200.0*/110.866,	/* 8 3700.0*/114.415,	/* 9 4200.0*/117.876,	/* 10 4700.0*/120.450,	/* 11 5200.0*/123.946,	/* 12 5800.0*/125.295,	/* 13 6400.0*/124.855,	/* 14 7100.0*/125.070,	/* 15 7800.0*/124.014,	},
{/* 13 108.000	*//* 0 650.0*/105.599,	/* 1 950.0*/106.125,	/* 2 1250.0*/107.606,	/* 3 1600.0*/108.824,	/* 4 1950.0*/109.716,	/* 5 2350.0*/108.998,	/* 6 2750.0*/109.269,	/* 7 3200.0*/110.926,	/* 8 3700.0*/114.086,	/* 9 4200.0*/117.390,	/* 10 4700.0*/119.910,	/* 11 5200.0*/123.648,	/* 12 5800.0*/124.843,	/* 13 6400.0*/126.550,	/* 14 7100.0*/126.680,	/* 15 7800.0*/125.881,	},
{/* 14 118.000	*//* 0 650.0*/105.855,	/* 1 950.0*/106.603,	/* 2 1250.0*/107.652,	/* 3 1600.0*/109.393,	/* 4 1950.0*/110.349,	/* 5 2350.0*/109.481,	/* 6 2750.0*/109.790,	/* 7 3200.0*/111.735,	/* 8 3700.0*/114.132,	/* 9 4200.0*/116.603,	/* 10 4700.0*/119.263,	/* 11 5200.0*/121.682,	/* 12 5800.0*/123.221,	/* 13 6400.0*/125.171,	/* 14 7100.0*/128.184,	/* 15 7800.0*/128.348,	},
{/* 15 128.000	*//* 0 650.0*/104.430,	/* 1 950.0*/105.297,	/* 2 1250.0*/107.271,	/* 3 1600.0*/108.832,	/* 4 1950.0*/108.953,	/* 5 2350.0*/110.116,	/* 6 2750.0*/111.153,	/* 7 3200.0*/112.316,	/* 8 3700.0*/114.148,	/* 9 4200.0*/116.354,	/* 10 4700.0*/118.107,	/* 11 5200.0*/119.835,	/* 12 5800.0*/121.332,	/* 13 6400.0*/124.078,	/* 14 7100.0*/127.961,	/* 15 7800.0*/130.209,	}
};

/* Generated by TS2C on Sun Aug 02 10:02:25 EDT 2020*/
static void setVeTable(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	MEMCPY(config->veLoadBins, hardCodedveLoadBins);
	MEMCPY(config->veRpmBins, hardCodedveRpmBins);
	MEMCPY(config->veTable, hardCodedveTable);
}


#if IGN_LOAD_COUNT == DEFAULT_IGN_LOAD_COUNT
static const ignition_table_t mapBased16IgnitionTable = {
		/* Generated by TS2C on Wed Jun 15 21:22:11 EDT 2016*/
		{/* 0 18.000	*//* 0 650.0*/10.000,	/* 1 1100.0*/11.432,	/* 2 1550.0*/12.863,	/* 3 2000.0*/14.295,	/* 4 2450.0*/15.726,	/* 5 2900.0*/17.157,	/* 6 3350.0*/18.589,	/* 7 3800.0*/20.021,	/* 8 4250.0*/21.452,	/* 9 4700.0*/22.884,	/* 10 5150.0*/24.315,	/* 11 5650.0*/25.906,	/* 12 6100.0*/27.337,	/* 13 6550.0*/28.769,	/* 14 7000.0*/30.200,	/* 15 7450.0*/30.200,	},
		{/* 1 24.000	*//* 0 650.0*/10.222,	/* 1 1100.0*/11.575,	/* 2 1550.0*/12.927,	/* 3 2000.0*/14.279,	/* 4 2450.0*/15.632,	/* 5 2900.0*/16.984,	/* 6 3350.0*/18.337,	/* 7 3800.0*/19.689,	/* 8 4250.0*/21.055,	/* 9 4700.0*/22.437,	/* 10 5150.0*/23.819,	/* 11 5650.0*/25.354,	/* 12 6100.0*/26.736,	/* 13 6550.0*/28.118,	/* 14 7000.0*/29.500,	/* 15 7450.0*/29.500,	},
		{/* 2 33.000	*//* 0 650.0*/10.556,	/* 1 1100.0*/11.789,	/* 2 1550.0*/13.023,	/* 3 2000.0*/14.257,	/* 4 2450.0*/15.490,	/* 5 2900.0*/16.724,	/* 6 3350.0*/17.958,	/* 7 3800.0*/19.192,	/* 8 4250.0*/20.426,	/* 9 4700.0*/21.659,	/* 10 5150.0*/22.893,	/* 11 5650.0*/24.264,	/* 12 6100.0*/25.497,	/* 13 6550.0*/26.731,	/* 14 7000.0*/27.965,	/* 15 7450.0*/27.965,	},
		{/* 3 39.000	*//* 0 650.0*/10.778,	/* 1 1100.0*/11.932,	/* 2 1550.0*/13.087,	/* 3 2000.0*/14.242,	/* 4 2450.0*/15.396,	/* 5 2900.0*/16.551,	/* 6 3350.0*/17.706,	/* 7 3800.0*/18.860,	/* 8 4250.0*/20.015,	/* 9 4700.0*/21.169,	/* 10 5150.0*/22.324,	/* 11 5650.0*/23.607,	/* 12 6100.0*/24.761,	/* 13 6550.0*/25.916,	/* 14 7000.0*/27.071,	/* 15 7450.0*/27.071,	},
		{/* 4 45.000	*//* 0 650.0*/11.000,	/* 1 1100.0*/12.076,	/* 2 1550.0*/13.151,	/* 3 2000.0*/14.227,	/* 4 2450.0*/15.302,	/* 5 2900.0*/16.378,	/* 6 3350.0*/17.453,	/* 7 3800.0*/18.529,	/* 8 4250.0*/19.604,	/* 9 4700.0*/20.680,	/* 10 5150.0*/21.755,	/* 11 5650.0*/22.950,	/* 12 6100.0*/24.026,	/* 13 6550.0*/25.101,	/* 14 7000.0*/26.177,	/* 15 7450.0*/26.177,	},
		{/* 5 51.000	*//* 0 650.0*/11.000,	/* 1 1100.0*/11.572,	/* 2 1550.0*/12.624,	/* 3 2000.0*/13.676,	/* 4 2450.0*/14.727,	/* 5 2900.0*/15.779,	/* 6 3350.0*/16.831,	/* 7 3800.0*/17.883,	/* 8 4250.0*/18.929,	/* 9 4700.0*/19.969,	/* 10 5150.0*/21.009,	/* 11 5650.0*/22.164,	/* 12 6100.0*/23.203,	/* 13 6550.0*/24.243,	/* 14 7000.0*/25.283,	/* 15 7450.0*/25.283,	},
		{/* 6 60.000	*//* 0 650.0*/9.800,	/* 1 1100.0*/10.816,	/* 2 1550.0*/11.833,	/* 3 2000.0*/12.849,	/* 4 2450.0*/13.865,	/* 5 2900.0*/14.882,	/* 6 3350.0*/15.898,	/* 7 3800.0*/16.914,	/* 8 4250.0*/17.917,	/* 9 4700.0*/18.903,	/* 10 5150.0*/19.889,	/* 11 5650.0*/20.984,	/* 12 6100.0*/21.970,	/* 13 6550.0*/22.956,	/* 14 7000.0*/23.941,	/* 15 7450.0*/23.941,	},
		{/* 7 66.000	*//* 0 650.0*/9.320,	/* 1 1100.0*/10.313,	/* 2 1550.0*/11.305,	/* 3 2000.0*/12.298,	/* 4 2450.0*/13.291,	/* 5 2900.0*/14.283,	/* 6 3350.0*/15.276,	/* 7 3800.0*/16.269,	/* 8 4250.0*/17.242,	/* 9 4700.0*/18.192,	/* 10 5150.0*/19.142,	/* 11 5650.0*/20.198,	/* 12 6100.0*/21.147,	/* 13 6550.0*/22.097,	/* 14 7000.0*/23.047,	/* 15 7450.0*/23.047,	},
		{/* 8 72.000	*//* 0 650.0*/8.840,	/* 1 1100.0*/9.809,	/* 2 1550.0*/10.778,	/* 3 2000.0*/11.747,	/* 4 2450.0*/12.716,	/* 5 2900.0*/13.685,	/* 6 3350.0*/14.654,	/* 7 3800.0*/15.623,	/* 8 4250.0*/16.567,	/* 9 4700.0*/17.481,	/* 10 5150.0*/18.395,	/* 11 5650.0*/19.411,	/* 12 6100.0*/20.325,	/* 13 6550.0*/21.239,	/* 14 7000.0*/22.153,	/* 15 7450.0*/22.153,	},
		{/* 9 78.000	*//* 0 650.0*/8.360,	/* 1 1100.0*/9.305,	/* 2 1550.0*/10.251,	/* 3 2000.0*/11.196,	/* 4 2450.0*/12.141,	/* 5 2900.0*/13.086,	/* 6 3350.0*/14.032,	/* 7 3800.0*/14.977,	/* 8 4250.0*/15.892,	/* 9 4700.0*/16.771,	/* 10 5150.0*/17.649,	/* 11 5650.0*/18.624,	/* 12 6100.0*/19.502,	/* 13 6550.0*/20.381,	/* 14 7000.0*/21.259,	/* 15 7450.0*/21.259,	},
		{/* 10 84.000	*//* 0 650.0*/7.880,	/* 1 1100.0*/8.802,	/* 2 1550.0*/9.723,	/* 3 2000.0*/10.645,	/* 4 2450.0*/11.566,	/* 5 2900.0*/12.488,	/* 6 3350.0*/13.410,	/* 7 3800.0*/14.331,	/* 8 4250.0*/15.253,	/* 9 4700.0*/16.175,	/* 10 5150.0*/17.096,	/* 11 5650.0*/18.120,	/* 12 6100.0*/19.042,	/* 13 6550.0*/19.963,	/* 14 7000.0*/20.885,	/* 15 7450.0*/20.885,	},
		{/* 11 93.000	*//* 0 650.0*/7.160,	/* 1 1100.0*/8.046,	/* 2 1550.0*/8.932,	/* 3 2000.0*/9.818,	/* 4 2450.0*/10.704,	/* 5 2900.0*/11.590,	/* 6 3350.0*/12.477,	/* 7 3800.0*/13.363,	/* 8 4250.0*/14.249,	/* 9 4700.0*/15.135,	/* 10 5150.0*/16.021,	/* 11 5650.0*/17.005,	/* 12 6100.0*/17.891,	/* 13 6550.0*/18.778,	/* 14 7000.0*/19.664,	/* 15 7450.0*/19.664,	},
		{/* 12 99.000	*//* 0 650.0*/6.680,	/* 1 1100.0*/6.967,	/* 2 1550.0*/8.405,	/* 3 2000.0*/9.000,	/* 4 2450.0*/9.937,	/* 5 2900.0*/10.992,	/* 6 3350.0*/11.854,	/* 7 3800.0*/12.717,	/* 8 4250.0*/13.579,	/* 9 4700.0*/14.442,	/* 10 5150.0*/15.304,	/* 11 5650.0*/16.262,	/* 12 6100.0*/17.125,	/* 13 6550.0*/17.987,	/* 14 7000.0*/18.850,	/* 15 7450.0*/18.850,	},
		{/* 13 105.000	*//* 0 650.0*/6.200,	/* 1 1100.0*/7.039,	/* 2 1550.0*/7.877,	/* 3 2000.0*/8.716,	/* 4 2450.0*/9.555,	/* 5 2900.0*/10.394,	/* 6 3350.0*/11.232,	/* 7 3800.0*/12.071,	/* 8 4250.0*/12.910,	/* 9 4700.0*/13.749,	/* 10 5150.0*/14.587,	/* 11 5650.0*/15.519,	/* 12 6100.0*/16.358,	/* 13 6550.0*/17.197,	/* 14 7000.0*/18.035,	/* 15 7450.0*/18.035,	},
		{/* 14 111.000	*//* 0 650.0*/5.720,	/* 1 1100.0*/6.535,	/* 2 1550.0*/7.350,	/* 3 2000.0*/8.165,	/* 4 2450.0*/8.980,	/* 5 2900.0*/9.795,	/* 6 3350.0*/10.610,	/* 7 3800.0*/11.425,	/* 8 4250.0*/12.240,	/* 9 4700.0*/13.055,	/* 10 5150.0*/13.870,	/* 11 5650.0*/14.776,	/* 12 6100.0*/15.591,	/* 13 6550.0*/16.406,	/* 14 7000.0*/17.221,	/* 15 7450.0*/17.221,	},
		{/* 15 120.000	*//* 0 650.0*/5.000,	/* 1 1100.0*/5.780,	/* 2 1550.0*/6.559,	/* 3 2000.0*/7.339,	/* 4 2450.0*/8.118,	/* 5 2900.0*/8.898,	/* 6 3350.0*/9.677,	/* 7 3800.0*/10.457,	/* 8 4250.0*/11.236,	/* 9 4700.0*/12.016,	/* 10 5150.0*/12.795,	/* 11 5650.0*/13.661,	/* 12 6100.0*/14.441,	/* 13 6550.0*/15.220,	/* 14 7000.0*/16.000,	/* 15 7450.0*/16.000,	},
};
#endif


static void miataNAcommonEngineSettings(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	engineConfiguration->trigger.type = TT_MAZDA_MIATA_NA;
	engineConfiguration->useOnlyRisingEdgeForTrigger = false;
	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->specs.firingOrder = FO_1_3_4_2;
	engineConfiguration->compressionRatio = 9.1;

	engineConfiguration->debugMode = DBG_TRIGGER_COUNTERS;

	setCommonNTCSensor(&engineConfiguration->clt, 2700);
	setCommonNTCSensor(&engineConfiguration->iat, 2700);

#if IGN_LOAD_COUNT == DEFAULT_IGN_LOAD_COUNT
	MEMCPY(config->ignitionTable, mapBased16IgnitionTable);
#endif

	engineConfiguration->silentTriggerError = false;

	engineConfiguration->manIdlePosition = 34;

	miataNA_setCrankingCycleBins(PASS_CONFIG_PARAMETER_SIGNATURE);
	miataNA_setCrankingFuelBins(PASS_CONFIG_PARAMETER_SIGNATURE);

	miataNA_setCltIdleCorrBins(PASS_CONFIG_PARAMETER_SIGNATURE);
	miataNA_setCltIdleRpmBins(PASS_CONFIG_PARAMETER_SIGNATURE);
	miataNA_setIacCoastingBins(PASS_CONFIG_PARAMETER_SIGNATURE);
	setMafDecodingBins(PASS_CONFIG_PARAMETER_SIGNATURE);
	miataNA_setIgnitionTable(PASS_CONFIG_PARAMETER_SIGNATURE);

	engineConfiguration->idle.solenoidFrequency = 160;
	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
}

void miataNAcommon(DECLARE_CONFIG_PARAMETER_SIGNATURE) {

	miataNAcommonEngineSettings(PASS_CONFIG_PARAMETER_SIGNATURE);

	engineConfiguration->idle.solenoidPin = GPIOB_9; // this W61 <> W61 jumper, pin 3W

	engineConfiguration->ignitionPins[0] = GPIOE_14; // Frankenso high side - pin 1G
	engineConfiguration->ignitionPins[1] = GPIO_UNASSIGNED;
	engineConfiguration->ignitionPins[2] = GPIOC_7; // Frankenso high side - pin 1H
	engineConfiguration->ignitionPins[3] = GPIO_UNASSIGNED;
}

static void setMiataNA6_settings(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	engineConfiguration->isFasterEngineSpinUpEnabled = true;


	setVeTable(PASS_CONFIG_PARAMETER_SIGNATURE);

	engineConfiguration->idleMode = IM_AUTO;
	// below 20% this valve seems to be opening for fail-safe idle air
	engineConfiguration->idleRpmPid.minValue = 20;
	engineConfiguration->idleRpmPid.pFactor = 0.01;
	engineConfiguration->idleRpmPid.iFactor = 0.00001;
	engineConfiguration->idleRpmPid.dFactor = 0.0001;
	engineConfiguration->idleRpmPid.periodMs = 100;

	/**
	 * http://miataturbo.wikidot.com/fuel-injectors
	 * 90-93 (Blue) - #195500-1970
	 */
	engineConfiguration->injector.flow = 230;

	// set cranking_timing_angle 10
	engineConfiguration->crankingTimingAngle = 10;

	engineConfiguration->map.sensor.type = MT_GM_3_BAR;

	// chartsize 200
	engineConfiguration->engineChartSize = 200;

	// maybe adjust CLT correction?
	// set cranking_fuel 8
	engineConfiguration->cranking.baseFuel = 1;
	config->crankingFuelCoef[0] = 28; // base cranking fuel adjustment coefficient
	config->crankingFuelBins[0] = -20; // temperature in C
	config->crankingFuelCoef[1] = 22;
	config->crankingFuelBins[1] = -10;
	config->crankingFuelCoef[2] = 18;
	config->crankingFuelBins[2] = 5;
	config->crankingFuelCoef[3] = 15;
	config->crankingFuelBins[3] = 30;

	config->crankingFuelCoef[4] = 10;
	config->crankingFuelBins[4] = 35;
	config->crankingFuelCoef[5] = 10;
	config->crankingFuelBins[5] = 50;
	config->crankingFuelCoef[6] = 10;
	config->crankingFuelBins[6] = 65;
	config->crankingFuelCoef[7] = 10;
	config->crankingFuelBins[7] = 90;

	engineConfiguration->specs.displacement = 1.6;
	strcpy(CONFIG(engineMake), ENGINE_MAKE_MAZDA);
	strcpy(CONFIG(engineCode), "NA6");

	// my car was originally a manual so proper TPS
	engineConfiguration->tpsMin = 93; // convert 12to10 bit (ADC/4)
	engineConfiguration->tpsMax = 656; // convert 12to10 bit (ADC/4)

	engineConfiguration->injectionMode = IM_BATCH;
}

/**
 * MIATA_NA6_MAP
 */
void setMiataNA6_MAP_Frankenso(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	setDefaultFrankensoConfiguration(PASS_CONFIG_PARAMETER_SIGNATURE);

	setFrankensoConfiguration(PASS_CONFIG_PARAMETER_SIGNATURE);

	engineConfiguration->isHip9011Enabled = false;
	CONFIG(isSdCardEnabled) = false;

	setMiataNA6_settings(PASS_CONFIG_PARAMETER_SIGNATURE);

	// Frankenso middle plug 2J, W32 top <> W47 bottom "#5 Green" jumper, not OEM
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_4;

	engineConfiguration->mafAdcChannel = EFI_ADC_0;

	// Wide band oxygen (from middle plug) to W52
	engineConfiguration->afr.hwChannel = EFI_ADC_13; // PA3

	engineConfiguration->vbattDividerCoeff = 9.75;// ((float) (8.2 + 33)) / 8.2 * 2;

	engineConfiguration->isSdCardEnabled = true;

//	/**
//	 * oil pressure line
//	 * adc4/pa4/W47
//	 */
//	engineConfiguration->fsioAdc[0] = EFI_ADC_NONE;


	// warning light
	/**
	 * to test
	 * set_fsio_setting 1 1800
	 * set_fsio_setting 2 95
	 * set_fsio_setting 4 14
	 *
	 * set_rpn_expression 1 "rpm 0 fsio_setting > coolant 1 fsio_setting > | vbatt 2 fsio_setting < |"
	 * rpn_eval "rpm 1 fsio_setting > coolant 2 fsio_setting > | vbatt 4 fsio_setting < |"
	 */
	/*
//WARNING:	these indeces are off
	engineConfiguration->fsio_setting[0] = 6400; // RPM threshold
	engineConfiguration->fsio_setting[1] = 100; // CLT threshold, fsio_setting #2
	engineConfiguration->fsio_setting[2] = 13.0; // voltage threshold, fsio_setting #3

	//	set_fsio_setting 4 3000
	engineConfiguration->fsio_setting[3] = 3000; // oil pressure RPM, fsio_setting #4
	// set_fsio_setting 5 0.52
	engineConfiguration->fsio_setting[4] = 0.52; // oil pressure threshold, fsio_setting #5
*/

//	 * set_rpn_expression 1 "rpm 3 fsio_setting >"
	// rpn_eval "rpm 1 fsio_setting >"
	// rpn_eval "fsio_analog_input"

	// set_fsio_expression 0 "((rpm > fsio_setting(4) & (fsio_analog_input < fsio_setting(5)) | rpm > fsio_setting(1) | (coolant > fsio_setting(2)  > | (vbatt < fsio_setting(3)"

#if EFI_FSIO
	// todo: convert
	setFsio(0, GPIOC_13, COMBINED_WARNING_LIGHT PASS_CONFIG_PARAMETER_SUFFIX);
#endif /* EFI_FSIO */

	engineConfiguration->injectionPins[0] = GPIOD_3; // #1&3 pin 3U
	engineConfiguration->injectionPins[1] = GPIOE_2; // #2&4 pin 3V
	engineConfiguration->injectionPins[2] = GPIO_UNASSIGNED;
	engineConfiguration->injectionPins[3] = GPIO_UNASSIGNED;

	// white wire from 1E - TOP of W4 to BOTTOM W62
	engineConfiguration->malfunctionIndicatorPin = GPIOD_5;

	// yellow wire from 1V/W22 to bottom of W48
	engineConfiguration->clutchDownPin = GPIOA_3;
	engineConfiguration->clutchDownPinMode = PI_PULLUP;


	// 110mm red wire from 1N/W14 to bottom of W45
	engineConfiguration->throttlePedalUpPin = GPIOA_7;

	// green wire from 1Q/W17 to bottom of W46
	engineConfiguration->acSwitchAdc = EFI_ADC_6; // PA6

#if ! EFI_UNIT_TEST
	// W57 PE3 A/C compressor relay out
	engineConfiguration->acRelayPin = GPIOE_3;
	// W58 PE4 A/C fan relay out
#endif /* EFI_UNIT_TEST */

	miataNAcommon(PASS_CONFIG_PARAMETER_SIGNATURE);
}

void setMiataNA6_VAF_Frankenso(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	setMiataNA6_MAP_Frankenso(PASS_CONFIG_PARAMETER_SIGNATURE);

	/**
	 * Stage 0 we only have OEM TPS switch
	 * See throttlePedalUpPin
	 */
	engineConfiguration->tps1_1AdcChannel = EFI_ADC_NONE;
	// todo: MAF-based ignition table?
}

/**
 * set engine_type 12
 */
void setMiataNA6_VAF_MRE(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	setMiataNA6_settings(PASS_CONFIG_PARAMETER_SIGNATURE);
	miataNAcommonEngineSettings(PASS_CONFIG_PARAMETER_SIGNATURE);

#if (BOARD_TLE8888_COUNT > 0)
	// idle.solenoidPin output is inherited from boards/microrusefi/board_configuration.cpp
	// CLT: "18 - AN temp 1"
	// IAT: "23 - AN temp 2"

	// MAF/VAF: "19 - AN volt 4"
	engineConfiguration->mafAdcChannel = EFI_ADC_12;

	//engineConfiguration->triggerInputPins[0] = GPIOC_6;
	engineConfiguration->triggerInputPins[1] = GPIOA_5;
	engineConfiguration->camInputs[0] = GPIO_UNASSIGNED;

	engineConfiguration->ignitionPins[0] = GPIOD_7;
	engineConfiguration->ignitionPins[1] = GPIO_UNASSIGNED;
	engineConfiguration->ignitionPins[2] = GPIOD_6;
	engineConfiguration->ignitionPins[3] = GPIO_UNASSIGNED;

	engineConfiguration->injectionPins[2] = GPIO_UNASSIGNED;
	engineConfiguration->injectionPins[3] = GPIO_UNASSIGNED;

	// tps = "20 - AN volt 5"
	//engineConfiguration->tps1_1AdcChannel = EFI_ADC_13;
	engineConfiguration->tps1_1AdcChannel = EFI_ADC_NONE;



	// TLE8888_PIN_24: "43 - GP Out 4"
	// MIL check engine
	// NA6 check engine light is just a normal low side driver
	engineConfiguration->malfunctionIndicatorPin = TLE8888_PIN_24;

	// IAC: GPIOE_9:  "7 - Lowside 1"


	// 0.2#3 has wrong R139
	// 56k high side/10k low side multiplied by above analogInputDividerCoefficient = 11
//	engineConfiguration->vbattDividerCoeff = (66.0f / 10.0f) * engineConfiguration->analogInputDividerCoefficient;


	engineConfiguration->isHip9011Enabled = false;
	engineConfiguration->isSdCardEnabled = true;
#endif /* BOARD_TLE8888_COUNT */
}

void setMiataNA6_MAP_MRE(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	setMiataNA6_settings(PASS_CONFIG_PARAMETER_SIGNATURE);
	miataNAcommonEngineSettings(PASS_CONFIG_PARAMETER_SIGNATURE);

	engineConfiguration->triggerInputPins[0] = GPIOA_5;
	engineConfiguration->triggerInputPins[1] = GPIOC_6;
	engineConfiguration->camInputs[0] = GPIO_UNASSIGNED;

	engineConfiguration->fuelPumpPin = GPIO_UNASSIGNED;


	engineConfiguration->useIacTableForCoasting = true;
	engineConfiguration->idlePidDeactivationTpsThreshold = 90;

	engineConfiguration->isVerboseIAC = true;

	engineConfiguration->idleRpmPid.pFactor = 0.01;
	engineConfiguration->idleRpmPid.iFactor = 0.02;
	engineConfiguration->idle_antiwindupFreq = 0.1;
	engineConfiguration->idle_derivativeFilterLoss = 0.1;
	engineConfiguration->idleRpmPid.dFactor = 0.002;
	engineConfiguration->idleRpmPid.offset = 37;
	engineConfiguration->acIdleExtraOffset = 14;
	engineConfiguration->idleRpmPid.minValue = 30;
	engineConfiguration->acIdleExtraMin = 14;
	engineConfiguration->idleRpmPid.maxValue = 70;
	engineConfiguration->idleRpmPid.periodMs = 40;
	engineConfiguration->idlerpmpid_iTermMin = -6;
	engineConfiguration->idlerpmpid_iTermMax = 30;
	engineConfiguration->pidExtraForLowRpm = 25;
	engineConfiguration->idlePidRpmDeadZone = 25;
	engineConfiguration->idlePidRpmUpperLimit = 1000;


	engineConfiguration->useFSIO12ForIdleOffset = true;
    setFsioExpression(QUOTE(MAGIC_OFFSET_FOR_IDLE_OFFSET), "ac_on_switch 0 cfg_acIdleExtraOffset if" PASS_CONFIG_PARAMETER_SUFFIX);

	engineConfiguration->useFSIO13ForIdleMinValue = true;
	setFsioExpression(QUOTE(MAGIC_OFFSET_FOR_IDLE_MIN_VALUE), "ac_on_switch 0 cfg_acIdleExtraMin if" PASS_CONFIG_PARAMETER_SUFFIX);


	engineConfiguration->useIdleTimingPidControl = true;
	engineConfiguration->idleTimingPid.pFactor = 0.05;
	engineConfiguration->idleTimingPid.iFactor = 0.0;
	engineConfiguration->idleTimingPid.dFactor = 0.0;
	engineConfiguration->idleTimingPid.minValue = -13;
	engineConfiguration->idleTimingPid.maxValue = 13;
	engineConfiguration->idleTimingPid.periodMs = 8;
	engineConfiguration->idleTimingPidWorkZone = 150;
	engineConfiguration->idlePidFalloffDeltaRpm = 50;
	engineConfiguration->idleTimingPidDeadZone = 10;


	// EFI_ADC_3: "22 - AN temp 4"
	engineConfiguration->acSwitchAdc = EFI_ADC_3;

	engineConfiguration->warningLedPin = GPIOD_13;
	engineConfiguration->triggerErrorPin = GPIOE_1;

	// todo: ask Stefan to clarify this
	engineConfiguration->tps1_1AdcChannel = EFI_ADC_6; // "26 - AN volt 2"
	engineConfiguration->tpsMin = 0;
	engineConfiguration->tpsMax = 982;

	engineConfiguration->map.sensor.hwChannel = EFI_ADC_13; // "20 - AN volt 5"

	engineConfiguration->mafAdcChannel = EFI_ADC_10;// "27 - AN volt 1"

#if (BOARD_TLE8888_COUNT > 0)
	// GPIOG_1: "Clutch Switch"
	engineConfiguration->clutchDownPin = GPIOG_1;

	engineConfiguration->fanPin = GPIO_UNASSIGNED;


	// TLE8888_PIN_23: "33 - GP Out 3"
	engineConfiguration->malfunctionIndicatorPin = TLE8888_PIN_23;

	// GPIOA_15: "AUX J2 PA15"
	engineConfiguration->fsioOutputPins[0] = GPIOA_15;
	// TLE8888_PIN_24: "43 - GP Out 4"
	engineConfiguration->fsioOutputPins[1] = TLE8888_PIN_24;

#endif /* BOARD_TLE8888_COUNT */
}
