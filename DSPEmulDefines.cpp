#include "DSPEmulDefines.h"

#include <math.h>
//--------------------------------------------------------------------------
bool GetTypeDSPEmulValid(int ATDE)
{
	switch (ATDE)
	{
		case TDE_DSP03 :
		case TDE_P347ADC24 :
		case TDE_P347ADC18 :
		{
			return true;
		}
		default: return false;
	}
}
//--------------------------------------------------------------------------
#ifdef __CFGDE_P347ADC24__
// Массив поддерживаемых фильтров ФНЧ 
TLPFilter LPFilters[LPFiltersCount] = {
{0, 204800.,524288},
{1, 102400.,262144},
{2, 51200.,131072},
{3, 25600.,65536},
{4, 12800.,32768},
{5, 6400.,16384},
{6, 3200.,8192},
{7, 1600.,4096},
{8, 800.,2048},
{9, 400.,1024},
{10, 200.,512},
{11, 100.,256},
{12, 50.,128},
{13, 25.,64},
{14, 12.5,32},
{15, 6.25,16}};
//--------------------------------------------------------------------------
// Массив поддерживаемых полосовых фильтров
TBPFilter BPFilters[BPFiltersCount] = {
{0, 160000, 4352*8, 3, 8, 1, 8, ENVT_1_3_20000},
{1, 128000, 3488*8, 3, 8, 1, 8, ENVT_1_3_16000},
{2, 102400, 2784*8, 3, 8, 1, 8, ENVT_1_3_12800},
{3, 80000, 4352*4, 3, 16, 2, 8, ENVT_1_3_20000},
{4, 64000, 3488*4, 3, 16, 2, 8, ENVT_1_3_16000},
{5, 51200, 2784*4, 3, 16, 2, 8, ENVT_1_3_12800},
{6, 40000, 4352*2, 3, 32, 4, 8, ENVT_1_3_20000},
{7, 32000, 3488*2, 3, 32, 4, 8, ENVT_1_3_16000},
{8, 25600, 2784*2, 3, 32, 4, 8, ENVT_1_3_12800},
{9, 20000, 4352, 3, 64, 8, 8, ENVT_1_3_20000},
{10, 16000, 3488, 3, 64, 8, 8, ENVT_1_3_16000},
{11, 12800, 2784, 3, 64, 8, 8, ENVT_1_3_12800},
{12, 10000, 2176, 3, 128, 16, 8, ENVT_1_3_20000},
{13, 8000, 1744, 3, 128, 16, 8, ENVT_1_3_16000},
{14, 6400, 1392, 3, 128, 16, 8, ENVT_1_3_12800},
{15, 5000, 1088, 3, 256, 32, 8, ENVT_1_3_20000},
{16, 4000, 872, 3, 256, 32, 8, ENVT_1_3_16000},
{17, 3200, 696, 3, 256, 32, 8, ENVT_1_3_12800},
{18, 2500, 544, 3, 512, 64, 8, ENVT_1_3_20000},
{19, 2000, 436, 3, 512, 64, 8, ENVT_1_3_16000},
{20, 1600, 348, 3, 512, 64, 8, ENVT_1_3_12800},
{21, 1250, 272, 3, 1024, 128, 8, ENVT_1_3_20000},
{22, 1000, 218, 3, 1024, 128, 8, ENVT_1_3_16000},
{23, 800, 174, 3, 1024, 128, 8, ENVT_1_3_12800},

{24, 120000, 8800*8, 1, 4, 1, 4, ENVT_1_1_15000},
{25, 60000,  8800*4, 1, 8, 2, 4, ENVT_1_1_15000},
{26, 30000,  8800*2, 1, 16, 4, 4, ENVT_1_1_15000},
{27, 15000,  8800,   1, 32, 8, 4, ENVT_1_1_15000},
{28, 7500,   4400,   1, 64, 16, 4, ENVT_1_1_15000},
{29, 3750,   2200,   1, 128, 32, 4, ENVT_1_1_15000}
}; 

#else
// Массив поддерживаемых фильтров ФНЧ 
TLPFilter LPFilters[LPFiltersCount] = {
{0, 25600.,65536},
{1, 12800.,32768},
{2, 6400.,16384},
{3, 3200.,8192},
{4, 1600.,4096},
{5, 800.,2048},
{6, 400.,1024},
{7, 200.,512},
{8, 100.,256},
{9, 50.,128},
{10, 25.,64},
{11, 12.5,32},
{12, 6.25,16}};
//--------------------------------------------------------------------------
// Массив поддерживаемых полосовых фильтров
TBPFilter BPFilters[BPFiltersCount] = {
{0, 20000, 4352, 3, 8, 1, 8, ENVT_1_3_20000},
{1, 16000, 3488, 3, 8, 1, 8, ENVT_1_3_16000},
{2, 12800, 2784, 3, 8, 1, 8, ENVT_1_3_12800},
{3, 10000, 2176, 3, 16, 2, 8, ENVT_1_3_20000},
{4, 8000, 1744, 3, 16, 2, 8, ENVT_1_3_16000},
{5, 6400, 1392, 3, 16, 2, 8, ENVT_1_3_12800},
{6, 5000, 1088, 3, 32, 4, 8, ENVT_1_3_20000},
{7, 4000, 872, 3, 32, 4, 8, ENVT_1_3_16000},
{8, 3200, 696, 3, 32, 4, 8, ENVT_1_3_12800},
{9, 2500, 544, 3, 64, 8, 8, ENVT_1_3_20000},
{10, 2000, 436, 3, 64, 8, 8, ENVT_1_3_16000},
{11, 1600, 348, 3, 64, 8, 8, ENVT_1_3_12800},
{12, 1250, 272, 3, 128, 16, 8, ENVT_1_3_20000},
{13, 1000, 218, 3, 128, 16, 8, ENVT_1_3_16000},
{14, 800, 174, 3, 128, 16, 8, ENVT_1_3_12800},
{15, 15000, 8800, 1, 4, 1, 4, ENVT_1_1_15000},
{16, 7500, 4400, 1, 8, 2, 4, ENVT_1_1_15000},
{17, 3750, 2200, 1, 16, 4, 4, ENVT_1_1_15000}}; 
#endif
//--------------------------------------------------------------------------
// Массив поддерживаемых фильтров передискретизации
TRSMPFilter RSMPFilters[RSMPFiltersCount] = {
{0, 16, 15, 1.0666666666667, RSMPT_16_15},
{1, 4, 5, 0.8, RSMPT_4_5},
{2, 1024, 1125, 0.91022222222222, RSMPT_1024_1125},
{3, 16, 15, 1.0666666666667, RSMPT_16_15_Ex},
{4, 4, 5, 0.8, RSMPT_4_5_Ex},
{5, 1024, 1125, 0.91022222222222, RSMPT_1024_1125_Ex}};
//--------------------------------------------------------------------------
// Массив поддерживаемых фильтров ФВЧ 
THPFilter HPFilters[HPFiltersCount] = {
{0, 0.1, 1, 4096, HPFT_01_1_40_002_4096},
{1, 0.1, 1, 8192, HPFT_01_1_40_002_8192},
{2, 0.1, 10, 4096, HPFT_01_10_40_002_4096},
{3, 0.1, 10, 8192, HPFT_01_10_40_002_8192},
{4, 0.1, 20, 4096, HPFT_01_20_40_002_4096},
{5, 0.1, 20, 8192, HPFT_01_20_40_002_8192},
{6, 0.3, 2, 9000, HPFT_03_2_36_1_9000},
{7, 2.5, 10, 9000, HPFT_2_5_10_36_01_9000},
{8, 2.5, 20, 9000, HPFT_2_5_20_36_01_9000}};
//--------------------------------------------------------------------------
// Массив поддерживаемых SP фильтров
TSPFilter SPFilters[SPFiltersCount] = {
{0, 9000, SPFT_LP1000_4000_01_40_9000},
{1, 9000, SPFT_LP2000_4000_01_40_9000}};
//--------------------------------------------------------------------------
// Массив поддерживаемых INTG фильтров
TINTGFilter INTGFilters[INTGFiltersCount] = {
{1, INTGFT_TRP}};
//--------------------------------------------------------------------------
// Массив поддерживаемых размеров спектров
int SpectLinesAr[SpectLinesArCount] = {
50, 
100, 
200, 
400, 
800, 
1600, 
3200, 
6400,
25};
//--------------------------------------------------------------------------
bool GetLPFilterIndexValid(unsigned short ALPFilterIndex)
{
	return (ALPFilterIndex < LPFiltersCount);
}
//--------------------------------------------------------------------------
int GetLPFilterIndexByMFreq(TDSPE_Float AMFreq)
{
	int res = -1;
	for (int i = 0; i < LPFiltersCount; i++) {
		if (LPFilters[i].MFreq == AMFreq){
			res = LPFilters[i].Index;
			break;
		}
	}
	return res;
}
//--------------------------------------------------------------------------
int GetLPFilterIndexByRFreq(TDSPE_Float ARFreq)
{
	int res = -1;
	for (int i = 0; i < LPFiltersCount; i++) {
		if (LPFilters[i].RFreq == ARFreq){
			res = LPFilters[i].Index;
			break;
		}
	}
	return res;
}
//--------------------------------------------------------------------------
bool GetBPFilterIndexValid(unsigned short ABPFilterIndex)
{
	return (ABPFilterIndex < BPFiltersCount);
}
//--------------------------------------------------------------------------
int GetBPFilterIndexByCFreq(int ACFreq)
{
	int res = -1;
	for (int i = 0; i < BPFiltersCount; i++) {
		if (BPFilters[i].CFreq == ACFreq){
			res = BPFilters[i].Index;
			break;
		}
	}
	return res;
}
//--------------------------------------------------------------------------
int GetMinLPFilterIndexForBPFilter(unsigned short ABPFilterIndex)
{
	int res = -1;
	if (GetBPFilterIndexValid(ABPFilterIndex))
	{
		res = GetLPFilterIndexByRFreq(DEFAULT_SAMPLING_FREQ/BPFilters[ABPFilterIndex].Dec);
	}
	return res;
}
//--------------------------------------------------------------------------
bool GetRSMPFilterIndexValid(unsigned short ARSMPFilterIndex)
{
	return (ARSMPFilterIndex < RSMPFiltersCount);
}
//--------------------------------------------------------------------------
int GetRSMPFilterIndexByInterpDec(int AInterp, int ADec)
{
	int res = -1;
	for (int i = 0; i < RSMPFiltersCount; i++) {
		if (RSMPFilters[i].Interp == AInterp && RSMPFilters[i].Dec == ADec){
			res = RSMPFilters[i].Index;
			break;
		}
	}
	return res;
}
//--------------------------------------------------------------------------
bool GetHPFilterIndexValid(unsigned short AHPFilterIndex)
{
	return (AHPFilterIndex < HPFiltersCount);
}
//--------------------------------------------------------------------------
bool GetSPFilterIndexValid(unsigned short ASPFilterIndex)
{
	return (ASPFilterIndex < SPFiltersCount);
}
//--------------------------------------------------------------------------
bool GetINTGFilterIndexValid(unsigned short AINTGFilterIndex)
{
	return (AINTGFilterIndex < INTGFiltersCount);
}
//--------------------------------------------------------------------------
bool GetAmpUnitValid(unsigned short AAmpUnit)
{
	return (AAmpUnit < TU_AMP_COUNT);
};
//--------------------------------------------------------------------------
bool GetAngleUnitValid(unsigned short AAngleUnit)
{
	return (AAngleUnit < TU_ANGLE_COUNT);
};
//--------------------------------------------------------------------------
bool GetSpectLinesArIndexValid(unsigned short ASpectLinesArIndex)
{
	return (ASpectLinesArIndex < SpectLinesArCount);
}
//--------------------------------------------------------------------------
int GetSpectLinesArIndexByLinesCount(int ACount)
{
	int res = -1;
	for (int i = 0; i < SpectLinesArCount; i++)
	{
		if (SpectLinesAr[i] == ACount)
		{
			res = i;
		}
	}
	return res;
}
//--------------------------------------------------------------------------
int GetLinesCountBySpectLinesArIndex(int AIndex)
{
	int res = 0;
	if (AIndex >= 0 && AIndex < SpectLinesArCount)
	{
		res = SpectLinesAr[AIndex];
	}
	return res;
}
//--------------------------------------------------------------------------
#ifdef __CFGDE_P347ADC24__
int GetLPSocketByDSPEmulIndex(int ALPFDSPEmulIndex)
{
	int res = -1;
	switch (ALPFDSPEmulIndex)
	{
	case 13 : res = 1; break;//1 LPF_25
	case 12	: res = 2; break;//2 LPF_50
	case 11	: res = 3; break;//3 LPF_100
	case 10	: res = 4; break;//4 LPF_200
	case 9	: res = 5; break;//5 LPF_400
	case 8	: res = 6; break;//6 LPF_800
	case 7	: res = 7; break;//7 LPF_1600
	case 6	: res = 8; break;//8 LPF_3200
	case 5	: res = 9; break;//9 LPF_6400
	case 4	: res = 10; break;//10 LPF_12800
	case 3	: res = 11; break;//11 LPF_25600
	case 2	: res = 12; break;//11 LPF_51200
	case 1	: res = 13; break;//11 LPF_102400
	case 0	: res = 14; break;//11 LPF_204800
	}
	return res;
}
//--------------------------------------------------------------------------
int GetLPDSPEmulBySocketIndex(int ALPFSocketIndex)
{
	int res = -1;
	switch (ALPFSocketIndex)
	{
	case 1 : res = 13; break;//1 LPF_25
	case 2	: res = 12; break;//2 LPF_50
	case 3	: res = 11; break;//3 LPF_100
	case 4	: res = 10; break;//4 LPF_200
	case 5	: res = 9; break;//5 LPF_400
	case 6	: res = 8; break;//6 LPF_800
	case 7	: res = 7; break;//7 LPF_1600
	case 8	: res = 6; break;//8 LPF_3200
	case 9	: res = 5; break;//9 LPF_6400
	case 10	: res = 4; break;//10 LPF_12800
	case 12	: res = 3; break;//11 LPF_25600
	case 13	: res = 2; break;//11 LPF_51200
	case 14	: res = 1; break;//11 LPF_102400
	case 15	: res = 0; break;//11 LPF_204800
	}
	return res;
}
//--------------------------------------------------------------------------
int GetBPSocketByDSPEmulIndex(int ABPFDSPEmulIndex)
{
	int res = -1;
	switch (ABPFDSPEmulIndex)
	{
	case 23: res = 1; break; //BPF_1_3_800	1	//центральная 800 Гц, полоса 185Гц
	case 22: res = 2; break; //BPF_1_3_1000	2	//центральная 1000 Гц, полоса 232 Гц
	case 21: res = 3; break; //BPF_1_3_1250	3	//центральная 1230 Гц, полоса 289 Гц
	case 20: res = 4; break; //BPF_1_3_1600	4	//центральная 1600 Гц, полоса 371 Гц
	case 19: res = 5; break; //BPF_1_3_2000	5	//центральная 2000 Гц, полоса 463 Гц
	case 18: res = 6; break; //BPF_1_3_2500	6	//центральная 2500 Гц, полоса 579 Гц
	case 17: res = 7; break; //BPF_1_3_3200	7	//центральная 3200 Гц, полоса 741 Гц
	case 16: res = 8; break; //BPF_1_3_4000	8	//центральная 4000 Гц, полоса 926 Гц
	case 15: res = 9; break; //BPF_1_3_5000	9	//центральная 5000 Гц, полоса 1158 Гц
	case 14: res = 10; break; //BPF_1_3_6400	10	//центральная 6400 Гц, полоса 1482 Гц
	case 13: res = 11; break; //BPF_1_3_8000	11	//центральная 8000 Гц, полоса 1853 Гц
	case 12: res = 12; break; //BPF_1_3_10000	12	//центральная 10000 Гц, полоса 2316 Гц
	case 11: res = 13; break; //BPF_1_3_12800	13	//центральная 12800 Гц, полоса 2964 Гц
	case 10: res = 14; break; //BPF_1_3_16000	14	//центральная 16000 Гц, полоса 3705 Гц
	case 9: res = 15; break; //BPF_1_3_20000	15	//центральная 20000 Гц, полоса 4631 Гц
	case 29: res = 16; break; //BPF_1_1_3750	16	//центральная 3750 Гц, полоса 2652 Гц
	case 28: res = 17; break; //BPF_1_1_7500	17	//центральная 7500 Гц, полоса 5303 Гц
	case 27: res = 18; break; //BPF_1_1_15000	18	//центральная 15000 Гц, полоса 10606 Гц
	}
	return res;
}
//--------------------------------------------------------------------------
int GetBPDSPEmulBySocketIndex(int ABPFSocketIndex)
{
	int res = -1;
	switch (ABPFSocketIndex)
	{
	case 1: res = 23; break; //BPF_1_3_800	1	//центральная 800 Гц, полоса 185Гц
	case 2: res = 22; break; //BPF_1_3_1000	2	//центральная 1000 Гц, полоса 232 Гц
	case 3: res = 21; break; //BPF_1_3_1250	3	//центральная 1230 Гц, полоса 289 Гц
	case 4: res = 20; break; //BPF_1_3_1600	4	//центральная 1600 Гц, полоса 371 Гц
	case 5: res = 19; break; //BPF_1_3_2000	5	//центральная 2000 Гц, полоса 463 Гц
	case 6: res = 18; break; //BPF_1_3_2500	6	//центральная 2500 Гц, полоса 579 Гц
	case 7: res = 17; break; //BPF_1_3_3200	7	//центральная 3200 Гц, полоса 741 Гц
	case 8: res = 16; break; //BPF_1_3_4000	8	//центральная 4000 Гц, полоса 926 Гц
	case 9: res = 15; break; //BPF_1_3_5000	9	//центральная 5000 Гц, полоса 1158 Гц
	case 10: res = 14; break; //BPF_1_3_6400	10	//центральная 6400 Гц, полоса 1482 Гц
	case 11: res = 13; break; //BPF_1_3_8000	11	//центральная 8000 Гц, полоса 1853 Гц
	case 12: res = 12; break; //BPF_1_3_10000	12	//центральная 10000 Гц, полоса 2316 Гц
	case 13: res = 11; break; //BPF_1_3_12800	13	//центральная 12800 Гц, полоса 2964 Гц
	case 14: res = 10; break; //BPF_1_3_16000	14	//центральная 16000 Гц, полоса 3705 Гц
	case 15: res = 9; break; //BPF_1_3_20000	15	//центральная 20000 Гц, полоса 4631 Гц
	case 16: res = 29; break; //BPF_1_1_3750	16	//центральная 3750 Гц, полоса 2652 Гц
	case 17: res = 28; break; //BPF_1_1_7500	17	//центральная 7500 Гц, полоса 5303 Гц
	case 18: res = 27; break; //BPF_1_1_15000	18	//центральная 15000 Гц, полоса 10606 Гц
	}
	return res;
}
#else
int GetLPSocketByDSPEmulIndex(int ALPFDSPEmulIndex)
{
	int res = -1;
	switch (ALPFDSPEmulIndex)
	{
		case 10 : res = 1; break;//1 LPF_25
		case 9	: res = 2; break;//2 LPF_50
		case 8	: res = 3; break;//3 LPF_100
		case 7	: res = 4; break;//4 LPF_200
		case 6	: res = 5; break;//5 LPF_400
		case 5	: res = 6; break;//6 LPF_800
		case 4	: res = 7; break;//7 LPF_1600
		case 3	: res = 8; break;//8 LPF_3200
		case 2	: res = 9; break;//9 LPF_6400
		case 1	: res = 10; break;//10 LPF_12800
		case 0	: res = 11; break;//11 LPF_25600
	}
	return res;
}
//--------------------------------------------------------------------------
int GetLPDSPEmulBySocketIndex(int ALPFSocketIndex)
{
	int res = -1;
	switch (ALPFSocketIndex)
	{
		case 1 : res = 10; break;//1 LPF_25
		case 2	: res = 9; break;//2 LPF_50
		case 3	: res = 8; break;//3 LPF_100
		case 4	: res = 7; break;//4 LPF_200
		case 5	: res = 6; break;//5 LPF_400
		case 6	: res = 5; break;//6 LPF_800
		case 7	: res = 4; break;//7 LPF_1600
		case 8	: res = 3; break;//8 LPF_3200
		case 9	: res = 2; break;//9 LPF_6400
		case 10	: res = 1; break;//10 LPF_12800
		case 11	: res = 0; break;//11 LPF_25600
	}
	return res;
}
//--------------------------------------------------------------------------
int GetBPSocketByDSPEmulIndex(int ABPFDSPEmulIndex)
{
	int res = -1;
	switch (ABPFDSPEmulIndex)
	{
		case 14: res = 1; break; //BPF_1_3_800	1	//центральная 800 Гц, полоса 185Гц
		case 13: res = 2; break; //BPF_1_3_1000	2	//центральная 1000 Гц, полоса 232 Гц
		case 12: res = 3; break; //BPF_1_3_1250	3	//центральная 1230 Гц, полоса 289 Гц
		case 11: res = 4; break; //BPF_1_3_1600	4	//центральная 1600 Гц, полоса 371 Гц
		case 10: res = 5; break; //BPF_1_3_2000	5	//центральная 2000 Гц, полоса 463 Гц
		case 9: res = 6; break; //BPF_1_3_2500	6	//центральная 2500 Гц, полоса 579 Гц
		case 8: res = 7; break; //BPF_1_3_3200	7	//центральная 3200 Гц, полоса 741 Гц
		case 7: res = 8; break; //BPF_1_3_4000	8	//центральная 4000 Гц, полоса 926 Гц
		case 6: res = 9; break; //BPF_1_3_5000	9	//центральная 5000 Гц, полоса 1158 Гц
		case 5: res = 10; break; //BPF_1_3_6400	10	//центральная 6400 Гц, полоса 1482 Гц
		case 4: res = 11; break; //BPF_1_3_8000	11	//центральная 8000 Гц, полоса 1853 Гц
		case 3: res = 12; break; //BPF_1_3_10000	12	//центральная 10000 Гц, полоса 2316 Гц
		case 2: res = 13; break; //BPF_1_3_12800	13	//центральная 12800 Гц, полоса 2964 Гц
		case 1: res = 14; break; //BPF_1_3_16000	14	//центральная 16000 Гц, полоса 3705 Гц
		case 0: res = 15; break; //BPF_1_3_20000	15	//центральная 20000 Гц, полоса 4631 Гц
		case 17: res = 16; break; //BPF_1_1_3750	16	//центральная 3750 Гц, полоса 2652 Гц
		case 16: res = 17; break; //BPF_1_1_7500	17	//центральная 7500 Гц, полоса 5303 Гц
		case 15: res = 18; break; //BPF_1_1_15000	18	//центральная 15000 Гц, полоса 10606 Гц
	}
	return res;
}
//--------------------------------------------------------------------------
int GetBPDSPEmulBySocketIndex(int ABPFSocketIndex)
{
	int res = -1;
	switch (ABPFSocketIndex)
	{
		case 1: res = 14; break; //BPF_1_3_800	1	//центральная 800 Гц, полоса 185Гц
		case 2: res = 13; break; //BPF_1_3_1000	2	//центральная 1000 Гц, полоса 232 Гц
		case 3: res = 12; break; //BPF_1_3_1250	3	//центральная 1230 Гц, полоса 289 Гц
		case 4: res = 11; break; //BPF_1_3_1600	4	//центральная 1600 Гц, полоса 371 Гц
		case 5: res = 10; break; //BPF_1_3_2000	5	//центральная 2000 Гц, полоса 463 Гц
		case 6: res = 9; break; //BPF_1_3_2500	6	//центральная 2500 Гц, полоса 579 Гц
		case 7: res = 8; break; //BPF_1_3_3200	7	//центральная 3200 Гц, полоса 741 Гц
		case 8: res = 7; break; //BPF_1_3_4000	8	//центральная 4000 Гц, полоса 926 Гц
		case 9: res = 6; break; //BPF_1_3_5000	9	//центральная 5000 Гц, полоса 1158 Гц
		case 10: res = 5; break; //BPF_1_3_6400	10	//центральная 6400 Гц, полоса 1482 Гц
		case 11: res = 4; break; //BPF_1_3_8000	11	//центральная 8000 Гц, полоса 1853 Гц
		case 12: res = 3; break; //BPF_1_3_10000	12	//центральная 10000 Гц, полоса 2316 Гц
		case 13: res = 2; break; //BPF_1_3_12800	13	//центральная 12800 Гц, полоса 2964 Гц
		case 14: res = 1; break; //BPF_1_3_16000	14	//центральная 16000 Гц, полоса 3705 Гц
		case 15: res = 0; break; //BPF_1_3_20000	15	//центральная 20000 Гц, полоса 4631 Гц
		case 16: res = 17; break; //BPF_1_1_3750	16	//центральная 3750 Гц, полоса 2652 Гц
		case 17: res = 16; break; //BPF_1_1_7500	17	//центральная 7500 Гц, полоса 5303 Гц
		case 18: res = 15; break; //BPF_1_1_15000	18	//центральная 15000 Гц, полоса 10606 Гц
	}
	return res;
}
#endif
//--------------------------------------------------------------------------
int GetSpLinesSocketByDSPEmulIndex(int ASpLinesDSPEmulIndex)
{
	int res = -1;
	switch (ASpLinesDSPEmulIndex)
	{
		case 3: res = 1; break; //RES_400			1
		case 4: res = 2; break; //RES_800			2
		case 5: res = 3; break; //RES_1600		3
		case 6: res = 4; break; //RES_3200		4	//опытная прошивка
		case 7: res = 5; break; //RES_6400		5	//опытная прошивка
	}
	return res;
}
//--------------------------------------------------------------------------
int GetSpLinesDSPEmulBySocketIndex(int ASpLinesSocketIndex)
{
	int res = -1;
	switch (ASpLinesSocketIndex)
	{
		case 1: res = 3; break; //RES_400			1
		case 2: res = 4; break; //RES_800			2
		case 3: res = 5; break; //RES_1600		3
		case 4: res = 6; break; //RES_3200		4	//опытная прошивка
		case 5: res = 7; break; //RES_6400		5	//опытная прошивка
	}
	return res;
}
//--------------------------------------------------------------------------

// В материалах фирмы "DLI" "Вибродиагностика" (Смирнов В.А.)
// (см. папку балансировка (литература)) указано, что для определения
// логарифмических уровней за исходные значения в США
// принимают:
// a0 = 9.804e-6 м*с^-2 (== 1мкg) - для виброускорения,
// v0 = 10^-8 м*с - для виброскорости.

// В ГОСТе 30296-95 (см. папку балансировка (литература)) указано,
// что для определения логарифмических уровней за исходные значения
// принимают:
// a0 = 10^-6 м*с^-2 - для виброускорения,
// v0 = 5*10^-8 м*с - для виброскорости.

// Это значения коэффициентов пересчета вибрации со значеиями исходных
// уровней (для определения логарифмических уровней) для виброускорения
// и виброскорости, принятыми в США
// const TUnitsAmp UnitsAmp[TbPossCntAmp]            = {1e6, 1e3, 1000/2.54e-2, 1e-12/*8e-11*/, 1.0e3, 1, 1/2.54e-2, 1e-9, 1e3, 1, 1/9.804, 0.000009804*0.1};

// Это значения коэффициентов пересчета вибрации со значеиями исходных
// уровней (для определения логарифмических уровней) для виброускорения
// и виброскорости, принятыми в России (ГОСТах)
const TDSPE_Float UnitsAmp[TU_AMP_COUNT]            = {1e6, 1e3, 1000/2.54e-2, 1e-12/*8e-11*/, 1.0e3, 1, 1/2.54e-2, 5e-8, 1e3, 1, 1/10.0, 1e-6};
//--------------------------------------------------------------------------
int ConvertSIAmpInOtherUnitAmp(TDSPE_Float ASIAmp, TDSPE_Float AFreqHz, unsigned short ATypeUnitAmp, TDSPE_Float &AOtherUnitAmp)
{
	if (ATypeUnitAmp == TU_AMP_A_M_S2)
	{
		AOtherUnitAmp = ASIAmp;
		return CUEC_NONE;
	}
	AOtherUnitAmp = 0;
	int res = CUEC_NONE;
	if (!GetAmpUnitValid(ATypeUnitAmp))
		return CUEC_AMP_UNIT_NOT_EXIST;

	if (ASIAmp < 0)
		return CUEC_AMP_MUST_ZERO_OR_POSITIVE;

	if ((ATypeUnitAmp >= TU_AMP_S_MKM) && (ATypeUnitAmp <= TU_AMP_V_DBV) && !(AFreqHz > 0)) 
		return CUEC_FREQ_MUST_POSITIVE;

	if (((ATypeUnitAmp == TU_AMP_S_DBD) || 
		 (ATypeUnitAmp == TU_AMP_V_DBV) || 
		 (ATypeUnitAmp == TU_AMP_A_DBA)) && (ASIAmp <= 0)) 
		return CUEC_LOG_AMP_MUST_POSITIVE;

	switch (ATypeUnitAmp)
	{
		case TU_AMP_S_MKM:  AOtherUnitAmp = ASIAmp * UnitsAmp[ATypeUnitAmp] / (4.0 * M_PI * M_PI * AFreqHz * AFreqHz); break;
		case TU_AMP_S_MM:  AOtherUnitAmp = ASIAmp * UnitsAmp[ATypeUnitAmp] / (4.0 * M_PI * M_PI * AFreqHz * AFreqHz); break;
		case TU_AMP_S_MILS:  AOtherUnitAmp = ASIAmp * UnitsAmp[ATypeUnitAmp] / (4.0 * M_PI * M_PI * AFreqHz * AFreqHz); break;
		case TU_AMP_S_DBD:  AOtherUnitAmp = 20.0 * log10(ASIAmp / (UnitsAmp[ATypeUnitAmp] * 4.0 * M_PI * M_PI * AFreqHz * AFreqHz)); break;
		case TU_AMP_V_MM_S:  AOtherUnitAmp = ASIAmp * UnitsAmp[ATypeUnitAmp] / (2.0 * M_PI * AFreqHz); break;
		case TU_AMP_V_M_S:  AOtherUnitAmp = ASIAmp * UnitsAmp[ATypeUnitAmp] / (2.0 * M_PI * AFreqHz); break;
		case TU_AMP_V_IN_S:  AOtherUnitAmp = ASIAmp * UnitsAmp[ATypeUnitAmp] / (2.0 * M_PI * AFreqHz); break;
		case TU_AMP_V_DBV:  AOtherUnitAmp = 20.0 * log10(ASIAmp / (UnitsAmp[ATypeUnitAmp] * 2.0 * M_PI * AFreqHz)); break;
		case TU_AMP_A_MM_S2:  AOtherUnitAmp = ASIAmp * UnitsAmp[ATypeUnitAmp]; break;
		case TU_AMP_A_M_S2:  AOtherUnitAmp = ASIAmp * UnitsAmp[ATypeUnitAmp]; break;
		case TU_AMP_A_G: AOtherUnitAmp = ASIAmp * UnitsAmp[ATypeUnitAmp]; break;
		case TU_AMP_A_DBA: AOtherUnitAmp = 20.0 * log10(ASIAmp / UnitsAmp[ATypeUnitAmp]); break;
		//case TU_AMP_A_DBA: AOtherUnitAmp = 140 + 20.0 * log10(ASIAmp * 1000); break;
	}
	return res;
}
//--------------------------------------------------------------------------
int ConvertOtherUnitAmpInSIAmp(TDSPE_Float AOtherUnitAmp, TDSPE_Float AFreqHz, unsigned short ATypeUnitAmp, TDSPE_Float &ASIAmp)
{
	if (ATypeUnitAmp == TU_AMP_A_M_S2)
	{
		ASIAmp = AOtherUnitAmp;
		return CUEC_NONE;
	}
	ASIAmp = 0;
	int res = CUEC_NONE;
	if (!GetAmpUnitValid(ATypeUnitAmp))
		return CUEC_AMP_UNIT_NOT_EXIST;

	if (((ATypeUnitAmp != TU_AMP_S_DBD) && 
		(ATypeUnitAmp != TU_AMP_V_DBV) && 
		(ATypeUnitAmp != TU_AMP_A_DBA)) && (AOtherUnitAmp < 0)) 
		return CUEC_AMP_MUST_ZERO_OR_POSITIVE;
// 	if (AOtherUnitAmp < 0)
// 		return CUEC_AMP_MUST_ZERO_OR_POSITIVE;

/*	if ((ATypeUnitAmp >= TU_AMP_S_MKM) && (ATypeUnitAmp <= TU_AMP_V_DBV) && !(AFreqHz > 0)) 
		return CUEC_FREQ_MUST_POSITIVE;*/

// 	if (((ATypeUnitAmp == TU_AMP_S_DBD) || 
// 		 (ATypeUnitAmp == TU_AMP_V_DBV) || 
// 		 (ATypeUnitAmp == TU_AMP_A_DBA)) && !(AOtherUnitAmp > 0)) 
// 		return CUEC_LOG_AMP_MUST_POSITIVE;

	switch (ATypeUnitAmp)
	{
		case TU_AMP_S_MKM:  ASIAmp = 4.0 * M_PI * M_PI * AFreqHz * AFreqHz * AOtherUnitAmp / UnitsAmp[ATypeUnitAmp]; break;
		case TU_AMP_S_MM:  ASIAmp = 4.0 * M_PI * M_PI * AFreqHz * AFreqHz * AOtherUnitAmp / UnitsAmp[ATypeUnitAmp]; break;
		case TU_AMP_S_MILS:  ASIAmp = 4.0 * M_PI * M_PI * AFreqHz * AFreqHz * AOtherUnitAmp / UnitsAmp[ATypeUnitAmp]; break;
		case TU_AMP_S_DBD:  ASIAmp = 4.0 * M_PI * M_PI * AFreqHz * AFreqHz * UnitsAmp[ATypeUnitAmp] * pow(10.0, AOtherUnitAmp / 20.0); break;
		case TU_AMP_V_MM_S:  ASIAmp = 2.0 * M_PI * AFreqHz * AOtherUnitAmp / UnitsAmp[ATypeUnitAmp]; break;
		case TU_AMP_V_M_S:  ASIAmp = 2.0 * M_PI * AFreqHz * AOtherUnitAmp / UnitsAmp[ATypeUnitAmp]; break;
		case TU_AMP_V_IN_S:  ASIAmp = 2.0 * M_PI * AFreqHz * AOtherUnitAmp / UnitsAmp[ATypeUnitAmp]; break;
		case TU_AMP_V_DBV:  ASIAmp = 2.0 * M_PI * AFreqHz * UnitsAmp[ATypeUnitAmp] * pow(10.0, AOtherUnitAmp / 20.0); break;
		case TU_AMP_A_MM_S2:  ASIAmp = AOtherUnitAmp / UnitsAmp[ATypeUnitAmp]; break;
		case TU_AMP_A_M_S2:  ASIAmp = AOtherUnitAmp / UnitsAmp[ATypeUnitAmp]; break;
		case TU_AMP_A_G: ASIAmp = AOtherUnitAmp / UnitsAmp[ATypeUnitAmp]; break;
		//case TU_AMP_A_DBA: ASIAmp = pow(10.0, (AOtherUnitAmp - 140) / 20.0) * 0.001; break;
		case TU_AMP_A_DBA: ASIAmp = pow(10.0, AOtherUnitAmp / 20.0)*UnitsAmp[ATypeUnitAmp]; break;
	}
	return res;
}
//--------------------------------------------------------------------------
int ConvertFstUnitAmpInSndUnitAmp(TDSPE_Float AFstUnitAmp, TDSPE_Float AFreqHz, unsigned short AFstTypeUnitAmp, unsigned short ASndTypeUnitAmp, TDSPE_Float &ASndUnitAmp)
{
	ASndUnitAmp = 0;
	TDSPE_Float TmpSIAmp;
	int res = ConvertOtherUnitAmpInSIAmp(AFstUnitAmp, AFreqHz, AFstTypeUnitAmp, TmpSIAmp);
	if (res != CUEC_NONE)
		return res;
	res = ConvertSIAmpInOtherUnitAmp(TmpSIAmp, AFreqHz, ASndTypeUnitAmp, ASndUnitAmp);
	return res;
}
//--------------------------------------------------------------------------
bool AmpUnitsInOneDomain(unsigned short ATypeUnitAmp1, unsigned short ATypeUnitAmp2)
{
	switch (ATypeUnitAmp1)
	{
		case TU_AMP_S_MKM: 
		case TU_AMP_S_MM: 
		case TU_AMP_S_MILS: 
		case TU_AMP_S_DBD: return (ATypeUnitAmp2 >= TU_AMP_S_MKM && ATypeUnitAmp2 <= TU_AMP_S_DBD); break;
		case TU_AMP_V_MM_S: 
		case TU_AMP_V_M_S: 
		case TU_AMP_V_IN_S: 
		case TU_AMP_V_DBV: return (ATypeUnitAmp2 >= TU_AMP_V_MM_S && ATypeUnitAmp2 <= TU_AMP_V_DBV); break;
		case TU_AMP_A_MM_S2: 
		case TU_AMP_A_M_S2: 
		case TU_AMP_A_G: 
		case TU_AMP_A_DBA: return (ATypeUnitAmp2 >= TU_AMP_A_MM_S2 && ATypeUnitAmp2 <= TU_AMP_A_DBA); break;
		default : return false;
	}
}
//--------------------------------------------------------------------------
bool AmpUnitIsSIDomainAmpUnit(unsigned short ASITypeUnitAmp)
{
	switch (ASITypeUnitAmp)
	{
	//case TU_AMP_S_MKM: 
	case TU_AMP_S_MM: 
// 	case TU_AMP_S_MILS: 
// 	case TU_AMP_S_DBD: res = (ATypeUnitAmp2 >= TU_AMP_S_MKM && ATypeUnitAmp2 <= TU_AMP_S_DBD); break;
// 	case TU_AMP_V_MM_S: 
	case TU_AMP_V_M_S: 
// 	case TU_AMP_V_IN_S: 
// 	case TU_AMP_V_DBV: res = (ATypeUnitAmp2 >= TU_AMP_V_MM_S && ATypeUnitAmp2 <= TU_AMP_V_DBV); break;
// 	case TU_AMP_A_MM_S2: 
	case TU_AMP_A_M_S2: return true;
// 	case TU_AMP_A_G: 
// 	case TU_AMP_A_DBA: res = (ATypeUnitAmp2 >= TU_AMP_A_MM_S2 && ATypeUnitAmp2 <= TU_AMP_A_DBA); break;
	default : return false;
	}
}
//--------------------------------------------------------------------------
bool GetSIDomainAmpUnitForAmpUnit(unsigned short ATypeUnitAmp, unsigned short &ASITypeUnitAmp)
{
	switch (ATypeUnitAmp)
	{
	case TU_AMP_S_MKM: 
	case TU_AMP_S_MM: 
	case TU_AMP_S_MILS: 
	case TU_AMP_S_DBD: {ASITypeUnitAmp = TU_AMP_S_MM; return true;};
	case TU_AMP_V_MM_S: 
	case TU_AMP_V_M_S: 
	case TU_AMP_V_IN_S: 
	case TU_AMP_V_DBV: {ASITypeUnitAmp = TU_AMP_V_M_S; return true;};
	case TU_AMP_A_MM_S2: 
	case TU_AMP_A_M_S2: 
	case TU_AMP_A_G: 
	case TU_AMP_A_DBA: {ASITypeUnitAmp = TU_AMP_A_M_S2; return true;};
	default : return false;
	}
}
//--------------------------------------------------------------------------
bool AmpUnitIsLog(unsigned short ATypeUnitAmp)
{
	switch (ATypeUnitAmp)
	{
// 	case TU_AMP_S_MKM: 
// 	case TU_AMP_S_MM: 
// 	case TU_AMP_S_MILS: 
	case TU_AMP_S_DBD: 
// 	case TU_AMP_V_MM_S: 
// 	case TU_AMP_V_M_S: 
// 	case TU_AMP_V_IN_S: 
	case TU_AMP_V_DBV: 
// 	case TU_AMP_A_MM_S2: 
// 	case TU_AMP_A_M_S2: 
// 	case TU_AMP_A_G: 
	case TU_AMP_A_DBA: return true;
	default : return false;
	}
}
//--------------------------------------------------------------------------
// Преобразует SI-шное представление величины вибрации к другим единицам измерения
// находящимся в том же домене, т.е. виброускорение в виброускорение, скорость в скорость и 
// виброперемещение в виброперемещение. Для виброперемещения SI-шными единицами считаются TU_AMP_S_MM,
// для виброскорости - TU_AMP_V_M_S, для виброускорения - TU_AMP_A_M_S2.
int ConvertOneDomainSIAmpInOtherUnitAmp(TDSPE_Float ASIDomainAmp, unsigned short ASIDomainTypeUnitAmp, 
	unsigned short ATypeUnitAmp, TDSPE_Float &AOtherUnitAmp)
{
	AOtherUnitAmp = 0;
	int res = CUEC_NONE;
	if (!GetAmpUnitValid(ASIDomainTypeUnitAmp) || !GetAmpUnitValid(ATypeUnitAmp))
		return CUEC_AMP_UNIT_NOT_EXIST;

	if (!AmpUnitIsSIDomainAmpUnit(ASIDomainTypeUnitAmp))
		return CUEC_AMP_UNIT_NOT_SIDOMAIN_AMP_UNIT;

	if (ATypeUnitAmp == ASIDomainTypeUnitAmp)
	{
		AOtherUnitAmp = ASIDomainAmp;
		return CUEC_NONE;
	}

	if (!AmpUnitsInOneDomain(ASIDomainTypeUnitAmp, ATypeUnitAmp))
		return CUEC_AMP_UNITS_MUST_ONE_DOMAIN;

	if (ASIDomainAmp < 0)
		return CUEC_AMP_MUST_ZERO_OR_POSITIVE;

 	if (((ATypeUnitAmp == TU_AMP_S_DBD) || 
 		 (ATypeUnitAmp == TU_AMP_V_DBV) || 
 		 (ATypeUnitAmp == TU_AMP_A_DBA)) && (ASIDomainAmp <= 0)) 
 		return CUEC_LOG_AMP_MUST_POSITIVE;

	switch (ASIDomainTypeUnitAmp)
	{
		case TU_AMP_S_MM:
		{
			switch (ATypeUnitAmp)
			{
			case TU_AMP_S_MKM:  AOtherUnitAmp = ASIDomainAmp * 1000; break;
			case TU_AMP_S_MM:  AOtherUnitAmp = ASIDomainAmp; break;
			case TU_AMP_S_MILS:  AOtherUnitAmp = ASIDomainAmp *0.001 * UnitsAmp[ATypeUnitAmp]; break;
			case TU_AMP_S_DBD:  AOtherUnitAmp = 20.0 * log10(ASIDomainAmp * 0.001/ (UnitsAmp[ATypeUnitAmp])); break;
			}
		} break;
		case TU_AMP_V_M_S:
		{
			switch (ATypeUnitAmp)
			{
			case TU_AMP_V_MM_S:  AOtherUnitAmp = ASIDomainAmp * 1000; break;
			case TU_AMP_V_M_S:  AOtherUnitAmp = ASIDomainAmp; break;
			case TU_AMP_V_IN_S:  AOtherUnitAmp = ASIDomainAmp * UnitsAmp[ATypeUnitAmp]; break;
			case TU_AMP_V_DBV:  AOtherUnitAmp = 20.0 * log10(ASIDomainAmp / (UnitsAmp[ATypeUnitAmp])); break;
			}
		} break;
		case TU_AMP_A_M_S2:
		{
			switch (ATypeUnitAmp)
			{
			case TU_AMP_A_MM_S2:  AOtherUnitAmp = ASIDomainAmp * 1000; break;
			case TU_AMP_A_M_S2:  AOtherUnitAmp = ASIDomainAmp; break;
			case TU_AMP_A_G:  AOtherUnitAmp = ASIDomainAmp * UnitsAmp[ATypeUnitAmp]; break;
			case TU_AMP_A_DBA:  AOtherUnitAmp = 20.0 * log10(ASIDomainAmp / (UnitsAmp[ATypeUnitAmp])); break;
			}
		} break;
	}
	return res;
}
//--------------------------------------------------------------------------
// Преобразует некоторое представление величины вибрации к SI-шной единице измерения
// находящимся в том же домене, т.е. виброускорение в виброускорение, скорость в скорость и 
// виброперемещение в виброперемещение. Для виброперемещения SI-шными единицами считаются TU_AMP_S_MM,
// для виброскорости - TU_AMP_V_M_S, для виброускорения - TU_AMP_A_M_S2.
int ConvertOneDomainUnitAmpInSIAmp(TDSPE_Float AUnitAmp, unsigned short ATypeUnitAmp, 
	unsigned short ASIDomainTypeUnitAmp, TDSPE_Float &ASIDomainAmp)
{
	ASIDomainAmp = 0;
	int res = CUEC_NONE;
	if (!GetAmpUnitValid(ASIDomainTypeUnitAmp) || !GetAmpUnitValid(ATypeUnitAmp))
		return CUEC_AMP_UNIT_NOT_EXIST;

	if (!AmpUnitIsSIDomainAmpUnit(ASIDomainTypeUnitAmp))
		return CUEC_AMP_UNIT_NOT_SIDOMAIN_AMP_UNIT;

	if (ATypeUnitAmp == ASIDomainTypeUnitAmp)
	{
		ASIDomainAmp = AUnitAmp;
		return CUEC_NONE;
	}

	if (!AmpUnitsInOneDomain(ASIDomainTypeUnitAmp, ATypeUnitAmp))
		return CUEC_AMP_UNITS_MUST_ONE_DOMAIN;

	if (((ATypeUnitAmp != TU_AMP_S_DBD) && 
		(ATypeUnitAmp != TU_AMP_V_DBV) && 
		(ATypeUnitAmp != TU_AMP_A_DBA)) && (AUnitAmp < 0)) 
		return CUEC_AMP_MUST_ZERO_OR_POSITIVE;

	switch (ASIDomainTypeUnitAmp)
	{
		case TU_AMP_S_MM:
		{
			switch (ATypeUnitAmp)
			{
			case TU_AMP_S_MKM:  ASIDomainAmp = AUnitAmp * 0.001; break;
			case TU_AMP_S_MM:  ASIDomainAmp = AUnitAmp; break;
			case TU_AMP_S_MILS:  ASIDomainAmp = AUnitAmp * 1000 / UnitsAmp[ATypeUnitAmp]; break;
			case TU_AMP_S_DBD:  ASIDomainAmp = 1000 * UnitsAmp[ATypeUnitAmp] * pow(10.0, AUnitAmp / 20.0); break;
			}
		} break;
		case TU_AMP_V_M_S:
		{
			switch (ATypeUnitAmp)
			{
			case TU_AMP_V_MM_S:  ASIDomainAmp = AUnitAmp * 1000; break;
			case TU_AMP_V_M_S:  ASIDomainAmp = AUnitAmp; break;
			case TU_AMP_V_IN_S:  ASIDomainAmp = AUnitAmp * UnitsAmp[ATypeUnitAmp]; break;
			case TU_AMP_V_DBV:  ASIDomainAmp = UnitsAmp[ATypeUnitAmp] * pow(10.0, AUnitAmp / 20.0); break;
			}
		} break;
		case TU_AMP_A_M_S2:
		{
			switch (ATypeUnitAmp)
			{
			case TU_AMP_A_MM_S2:  ASIDomainAmp = AUnitAmp * 1000; break;
			case TU_AMP_A_M_S2:  ASIDomainAmp = AUnitAmp; break;
			case TU_AMP_A_G:  ASIDomainAmp = AUnitAmp * UnitsAmp[ATypeUnitAmp]; break;
			case TU_AMP_A_DBA:  ASIDomainAmp = UnitsAmp[ATypeUnitAmp] * pow(10.0, AUnitAmp / 20.0); break;
			}
		} break;
	}
	return res;
}
//--------------------------------------------------------------------------
int ConvertOneDomainFstUnitAmpInSndUnitAmp(TDSPE_Float AFstUnitAmp, unsigned short AFstTypeUnitAmp, unsigned short ASndTypeUnitAmp, TDSPE_Float &ASndUnitAmp)
{
	ASndUnitAmp = 0;
	TDSPE_Float TmpSIDomainAmp;
	unsigned short TmpSIDomainTypeUnitAmp;
	if (!GetSIDomainAmpUnitForAmpUnit(AFstTypeUnitAmp, TmpSIDomainTypeUnitAmp))
		return CUEC_AMP_UNIT_NOT_EXIST;
	int res = ConvertOneDomainUnitAmpInSIAmp(AFstUnitAmp, AFstTypeUnitAmp, 
		TmpSIDomainTypeUnitAmp, TmpSIDomainAmp);
	if (res != CUEC_NONE)
		return res;
	res = ConvertOneDomainSIAmpInOtherUnitAmp(TmpSIDomainAmp, TmpSIDomainTypeUnitAmp, 
		ASndTypeUnitAmp, ASndUnitAmp);
	return res;
}
//--------------------------------------------------------------------------
unsigned int GetTSDTCSize(unsigned char ATSDTC)
{
	unsigned int res = 0;
	switch (ATSDTC)
	{
	case TSDTC_SHORT : return sizeof(short);
	case TSDTC_FLOAT : return sizeof(TDSPE_Float);
	case TSDTC_DOUBLE : return sizeof(TDSPE_Double);
	case TSDTC_LONGDOUBLE : return sizeof(TDSPE_LongDouble);
	case TSDTC_24DATA_8STATUS : return sizeof(tssample24_8_t);
	case TSDTC_18DATA_8STATUS : return sizeof(tssample18_8_t);
	case TSDTC_24DATA : return sizeof(tssample24_t);
	case TSDTC_18DATA : return sizeof(tssample18_t);
	case TSDTC_32DATA : return sizeof(tssample32_t);

	}
	return res;
}
//--------------------------------------------------------------------------
unsigned char GetTSDTCBitSize(unsigned char ATSDTC)
{
	unsigned int res = 0;
	switch (ATSDTC)
	{
	case TSDTC_SHORT : return sizeof(short)*8;
	case TSDTC_FLOAT : return sizeof(TDSPE_Float)*8;
	case TSDTC_DOUBLE : return sizeof(TDSPE_Double)*8;
	case TSDTC_LONGDOUBLE : return sizeof(TDSPE_LongDouble)*8;
	case TSDTC_24DATA_8STATUS : return sizeof(tssample24_8_t)*8;
	case TSDTC_18DATA_8STATUS : return sizeof(tssample18_8_t)*8;
	case TSDTC_24DATA : return 24;
	case TSDTC_18DATA : return 18;
	case TSDTC_32DATA : return sizeof(tssample32_t)*8;

	}
	return res;
}
//--------------------------------------------------------------------------
bool GetTimeSigDataTypeCodeValid(int ATSDTC)
{
	bool res = false;
	switch (ATSDTC)
	{
	case TSDTC_SHORT : 
	case TSDTC_FLOAT : 
	case TSDTC_DOUBLE : 
	case TSDTC_LONGDOUBLE : 
	case TSDTC_24DATA_8STATUS : 
	case TSDTC_18DATA_8STATUS : 
	case TSDTC_24DATA : 
	case TSDTC_18DATA : 
	case TSDTC_32DATA : res = true;

	}
	return res;
}
//--------------------------------------------------------------------------
bool ConverSig(void* ABuffSrc, unsigned int ALength, unsigned char ATSDTCSrc, unsigned char ATSDTCDest, bool ACreateBuff, void* &ABuffDest)
{
	bool res = false;
	if (ABuffSrc != NULL && ((!ACreateBuff && ABuffDest != NULL) || ACreateBuff) && ALength > 0)
	{
		// Создадим выходной буфер, если это требуется.
		if (ACreateBuff && GetTSDTCSize(ATSDTCDest) > 0)
		{
			ABuffDest = (void*)new char[GetTSDTCSize(ATSDTCDest)*ALength];
			//#ifndef __linux__
			//			// Не будем лишний раз дратить ресурсы процессора
			//			memset(ABuffDest, 0x00, GetTSDTCSize(ATSDTCSrc)*ALength);
			//#endif
		}

		switch (ATSDTCDest)
		{
		case TSDTC_SHORT :
			{
				switch (ATSDTCSrc)
				{
				case TSDTC_SHORT :
					{
						memcpy(ABuffDest, ABuffSrc, GetTSDTCSize(ATSDTCSrc) * ALength);
						res = true;
					} break; 
				case TSDTC_FLOAT :
					{
						for (int i = 0; i < ALength; i++)
						{
							((tssample_t*)(ABuffDest))[i] = (tssample_t)(((TDSPE_Float*)(ABuffSrc))[i] * 32768);
						}
						res = true;
					} break; 

				case TSDTC_DOUBLE :
					{
						for (int i = 0; i < ALength; i++)
						{
							((tssample_t*)(ABuffDest))[i] = (tssample_t)(((TDSPE_Double*)(ABuffSrc))[i] * 32768);
						}
						res = true;
					} break; 

				case TSDTC_LONGDOUBLE :
					{
						for (int i = 0; i < ALength; i++)
						{
							((tssample_t*)(ABuffDest))[i] = (tssample_t)(((TDSPE_LongDouble*)(ABuffSrc))[i] * 32768);
						}
						res = true;
					} break; 

				case TSDTC_24DATA_8STATUS :
					{
						for (int i = 0; i < ALength; i++)
						{
							((tssample_t*)(ABuffDest))[i] = (tssample_t)((tssample24_8_ext_t*)(ABuffSrc))[i].H;
						}
						res = true;
					} break; 

				case TSDTC_24DATA :
					{
						for (int i = 0; i < ALength; i++)
						{
							((tssample_t*)(ABuffDest))[i] = (tssample_t)((((tssample24_8_ext_t*)(ABuffSrc))[i].Val >> 8));
						}
						res = true;
					} break; 

				case TSDTC_32DATA :
					{
						for (int i = 0; i < ALength; i++)
						{
							((tssample_t*)(ABuffDest))[i] = (tssample_t)((tssample24_8_ext_t*)(ABuffSrc))[i].H;
						}
						res = true;
					} break; 


				default : return false;
				}
			} break;


		case TSDTC_LONGDOUBLE :
			{
				switch (ATSDTCSrc)
				{
				case TSDTC_SHORT :
					{
						for (int i = 0; i < ALength; i++)
						{
							((TDSPE_LongDouble*)(ABuffDest))[i] = ((tssample_t*)(ABuffSrc))[i] / 32768.0;
						}
						res = true;
					} break; 
				case TSDTC_FLOAT :
					{
						for (int i = 0; i < ALength; i++)
						{
							((TDSPE_LongDouble*)(ABuffDest))[i] = (TDSPE_LongDouble)(((TDSPE_Float*)(ABuffSrc))[i]);
						}
						res = true;
					} break; 

				case TSDTC_DOUBLE :
					{
						for (int i = 0; i < ALength; i++)
						{
							((TDSPE_LongDouble*)(ABuffDest))[i] = (TDSPE_LongDouble)(((TDSPE_Double*)(ABuffSrc))[i]);
						}
						res = true;
					} break; 

				case TSDTC_LONGDOUBLE :
					{
						memcpy(ABuffDest, ABuffSrc, GetTSDTCSize(ATSDTCSrc) * ALength);
						res = true;
					} break; 

				case TSDTC_24DATA_8STATUS :
					{
						for (int i = 0; i < ALength; i++)
						{
							((TDSPE_LongDouble*)(ABuffDest))[i] = (TDSPE_LongDouble)((int32_t)(((tssample24_8_t*)(ABuffSrc))[i]&MASK_TSSAMPLE24_8_DATA) / 2147483648.0);
						}
						res = true;
					} break; 

				case TSDTC_24DATA :
					{
						for (int i = 0; i < ALength; i++)
						{
							((TDSPE_LongDouble*)(ABuffDest))[i] = (TDSPE_LongDouble)(((tssample24_t*)(ABuffSrc))[i] / 8388608.0);
						}
						res = true;
					} break; 

				case TSDTC_32DATA :
					{
						for (int i = 0; i < ALength; i++)
						{
							((TDSPE_LongDouble*)(ABuffDest))[i] = (TDSPE_LongDouble)(((tssample32_t*)(ABuffSrc))[i] / 2147483648.0);
						}
						res = true;
					} break; 

				default : return false;
				}
			} break;

		case TSDTC_24DATA_8STATUS :
			{
				switch (ATSDTCSrc)
				{
				case TSDTC_SHORT :
					{
						for (int i = 0; i < ALength; i++)
						{
							((tssample24_8_t*)(ABuffDest))[i] = (((tssample24_8_t)(((tssample_t*)(ABuffSrc))[i])) << 16);
						}
						res = true;
					} break; 
				case TSDTC_FLOAT :
					{
						for (int i = 0; i < ALength; i++)
						{
							((tssample24_8_t*)(ABuffDest))[i] = (tssample24_8_t)((tssample24_8_t)((((TDSPE_Float*)(ABuffSrc))[i]) * 2147483648)&MASK_TSSAMPLE24_8_DATA);
						}
						res = true;
					} break; 

				case TSDTC_DOUBLE :
					{
						for (int i = 0; i < ALength; i++)
						{
							((tssample24_8_t*)(ABuffDest))[i] = (tssample24_8_t)((tssample24_8_t)((((TDSPE_Double*)(ABuffSrc))[i]) * 2147483648)&MASK_TSSAMPLE24_8_DATA);
						}
						res = true;
					} break; 

				case TSDTC_LONGDOUBLE :
					{
						for (int i = 0; i < ALength; i++)
						{
							((tssample24_8_t*)(ABuffDest))[i] = (tssample24_8_t)((tssample24_8_t)((((TDSPE_LongDouble*)(ABuffSrc))[i]) * 2147483648)&MASK_TSSAMPLE24_8_DATA);
						}
						res = true;
					} break; 

				case TSDTC_24DATA_8STATUS :
					{
						memcpy(ABuffDest, ABuffSrc, GetTSDTCSize(ATSDTCSrc) * ALength);
						res = true;
					} break; 

				case TSDTC_24DATA :
					{
						tssample24_t* TmpBuffSrc = (tssample24_t*)ABuffSrc;
						tssample24_8_t* TmpBuffDest = (tssample24_8_t*)ABuffDest;
						for (int i = 0; i < ALength; i++)
						{
							TmpBuffDest[i] = (tssample24_8_t)(TmpBuffSrc[i] << 8);
						}
						res = true;
					} break; 

				case TSDTC_32DATA :
					{
						memcpy(ABuffDest, ABuffSrc, GetTSDTCSize(ATSDTCSrc) * ALength);
						res = true;
					} break; 

				default : return false;
				}
			} break;

		case TSDTC_24DATA :
			{
				switch (ATSDTCSrc)
				{
				case TSDTC_SHORT :
					{
						for (int i = 0; i < ALength; i++)
						{
							((tssample24_t*)(ABuffDest))[i] = (((tssample24_t)(((tssample_t*)(ABuffSrc))[i])) << 8);
						}
						res = true;
					} break; 
				case TSDTC_FLOAT :
					{
						for (int i = 0; i < ALength; i++)
						{
							((tssample24_t*)(ABuffDest))[i] = (tssample24_t)((((TDSPE_Float*)(ABuffSrc))[i]) * 8388608);
						}
						res = true;
					} break; 

				case TSDTC_DOUBLE :
					{
						for (int i = 0; i < ALength; i++)
						{
							((tssample24_t*)(ABuffDest))[i] = (tssample24_t)((((TDSPE_Double*)(ABuffSrc))[i]) * 8388608);
						}
						res = true;
					} break; 

				case TSDTC_LONGDOUBLE :
					{
						for (int i = 0; i < ALength; i++)
						{
							((tssample24_t*)(ABuffDest))[i] = (tssample24_t)((((TDSPE_LongDouble*)(ABuffSrc))[i]) * 8388608);
						}
						res = true;
					} break; 

				case TSDTC_24DATA_8STATUS :
					{
						tssample24_8_t* TmpBuffSrc = (tssample24_8_t*)ABuffSrc;
						tssample24_t* TmpBuffDest = (tssample24_t*)ABuffDest;
						for (int i = 0; i < ALength; i++)
						{
							TmpBuffDest[i] = (tssample24_t)(TmpBuffSrc[i] >> 8);
						}
						res = true;
					} break; 

				case TSDTC_24DATA :
					{
						memcpy(ABuffDest, ABuffSrc, GetTSDTCSize(ATSDTCSrc) * ALength);
						res = true;
					} break; 

				case TSDTC_32DATA :
					{
						tssample32_t* TmpBuffSrc = (tssample32_t*)ABuffSrc;
						tssample24_t* TmpBuffDest = (tssample24_t*)ABuffDest;
						for (int i = 0; i < ALength; i++)
						{
							TmpBuffDest[i] = (tssample24_t)(TmpBuffSrc[i] >> 8);
						}
						res = true;
					} break; 

				default : return false;
				}
			} break;

		case TSDTC_32DATA :
			{
				switch (ATSDTCSrc)
				{
				case TSDTC_SHORT :
					{
						for (int i = 0; i < ALength; i++)
						{
							((tssample32_t*)(ABuffDest))[i] = (((tssample32_t)(((tssample_t*)(ABuffSrc))[i])) << 16);
						}
						res = true;
					} break; 
				case TSDTC_FLOAT :
					{
						for (int i = 0; i < ALength; i++)
						{
							((tssample32_t*)(ABuffDest))[i] = (tssample32_t)((((TDSPE_Float*)(ABuffSrc))[i]) * 2147483648);
						}
						res = true;
					} break; 

				case TSDTC_DOUBLE :
					{
						for (int i = 0; i < ALength; i++)
						{
							((tssample32_t*)(ABuffDest))[i] = (tssample32_t)((((TDSPE_Double*)(ABuffSrc))[i]) * 2147483648);
						}
						res = true;
					} break; 

				case TSDTC_LONGDOUBLE :
					{
						for (int i = 0; i < ALength; i++)
						{
							((tssample32_t*)(ABuffDest))[i] = (tssample32_t)((((TDSPE_LongDouble*)(ABuffSrc))[i]) * 2147483648);
						}
						res = true;
					} break; 

				case TSDTC_24DATA_8STATUS :
					{
						tssample24_8_t* TmpBuffSrc = (tssample24_8_t*)ABuffSrc;
						tssample32_t* TmpBuffDest = (tssample32_t*)ABuffDest;
						for (int i = 0; i < ALength; i++)
						{
							TmpBuffDest[i] = (tssample32_t)(TmpBuffSrc[i] & MASK_TSSAMPLE24_8_DATA);
						}
						res = true;
					} break; 

				case TSDTC_24DATA :
					{
						tssample24_t* TmpBuffSrc = (tssample24_t*)ABuffSrc;
						tssample32_t* TmpBuffDest = (tssample32_t*)ABuffDest;
						for (int i = 0; i < ALength; i++)
						{
							TmpBuffDest[i] = (tssample32_t)(TmpBuffSrc[i] << 8);
						}
						res = true;
					} break; 

				case TSDTC_32DATA :
					{
						memcpy(ABuffDest, ABuffSrc, GetTSDTCSize(ATSDTCSrc) * ALength);
						res = true;
					} break; 

				default : return false;
				}
			} break;

		default : return false;
		}
	}
	return res;
}

//--------------------------------------------------------------------------
bool GetTimeSigSourceTypeValid(int ATimeSigSourceType)
{
	return (ATimeSigSourceType >= TSST_NONE && ATimeSigSourceType < TSST_COUNT);
}
//--------------------------------------------------------------------------
bool GetTaskTypeStateValid(char ATaskTypeState)
{
	return (ATaskTypeState >= TTS_STOP && ATaskTypeState < TTS_COUNT);
}
//--------------------------------------------------------------------------
bool GetFParamsTypeValid(int AFParamsType)
{
	return (AFParamsType >= FPARAMS_NONE && AFParamsType < FPARAMS_COUNT);
}
//--------------------------------------------------------------------------