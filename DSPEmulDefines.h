//--------------------------------------------------------------------------
// Author: Sidorov E.N.
// Date: 18.02.2011.
// ������, � ������� ������� ����������� ��� �������� � ����� ��������,
// ������� ����� ���� �������� ������������ DSP01-DSP03
//--------------------------------------------------------------------------
#ifndef DSPEMULDEFINES_H
#define DSPEMULDEFINES_H
//--------------------------------------------------------------------------
#include <math.h>
//--------------------------------------------------------------------------
#include "RobastAnalisis.h"
#include "FixedPoint.h"
//--------------------------------------------------------------------------
#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif

//#define __NEW_FILTER_FUNCTION__
//--------------------------------------------------------------------------
// ���� ������ � ��������� ������, ������������ � DSPEmul.
//--------------------------------------------------------------------------
#ifndef __linux__
typedef double TDSPE_Float;
typedef double TDSPE_Double;
typedef long double TDSPE_LongDouble;
#else //#ifndef __linux__
typedef float TDSPE_Float;
typedef double TDSPE_Double;
typedef double TDSPE_LongDouble;
#endif //#ifndef __linux__

//--------------------------------------------------------------------------
//+uki - boost overrides abs too
#include <ini_local_conf.h>
#ifndef USE_BOOST
#ifdef __linux__
inline float abs(float AX)
    {return (fabs(AX));}
inline double abs(double AX)
    {return (fabs(AX));}
inline long double abs(long double AX)
    {return (fabsl(AX));}
#endif
#endif

//--------------------------------------------------------------------------
// ���� ������ � ������������� ������, ������������ � DSPEmul.
//--------------------------------------------------------------------------
#ifndef __linux__

#ifndef int32_t
	typedef int int32_t;
#endif
#ifndef uint32_t
	typedef unsigned int uint32_t;
#endif
#ifndef int64_t
	typedef __int64 int64_t;
#endif
#ifndef uint64_t
	typedef unsigned __int64 uint64_t;
#endif

#else //#ifndef __linux__

#ifndef int32_t
typedef int int32_t;
#endif
#ifndef uint32_t
typedef unsigned int uint32_t;
#endif
#ifndef int64_t
typedef long long int int64_t;
#endif
#ifndef uint64_t
typedef unsigned long long int uint64_t;
#endif

#endif //#ifndef __linux__
//--------------------------------------------------------------------------
// ���� ���������.
// ���������� ��� �������� ��������������, ������������ ������� � �.�.
#define TDE_DSP03		0	// ���������� ���� DSP01-03
#define TDE_P347ADC24	1	// ���������� ���� P347 c ��� 24 ����
#define TDE_P347ADC18	2	// ���������� ���� P347 c ��� 18 ���

#define TDE_COUNT		3
extern bool GetTypeDSPEmulValid(int ATDE);
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
#define __CFGDE_P347ADC24__
#ifdef __CFGDE_P347ADC24__
// ������� ������������� ������� �� ��������� (������� ������)
#define DEFAULT_SAMPLING_FREQ 524288
// ������� ������������� ������� �� ��������� (�������� � ���)
#define DEFAULT_SAMPLING_FREQ_ADC 576000
#else
// ������� ������������� ������� �� ��������� (������� ������)
#define DEFAULT_SAMPLING_FREQ 65536
// ������� ������������� ������� �� ��������� (�������� � ���)
#define DEFAULT_SAMPLING_FREQ_ADC 65536
#endif
//--------------------------------------------------------------------------
// ��������� ���������� �������
#define GRAVITY_CONST_G		  10.0
//--------------------------------------------------------------------------
// ������� ���
//--------------------------------------------------------------------------
// ����������� ���� ��� �������
// Index - ������ � ������ ��������;
// MFreq - ������������ ������� ��� �������;
// RFreq - ������� ������������� ��� ��� �������;
struct TLPFilter
{
	//int MFreq1;
	unsigned char Index;
	TDSPE_Float MFreq;
	int RFreq;
	bool operator ==(const TLPFilter& lpf) const
	{
		return Index == lpf.Index && MFreq == lpf.MFreq && RFreq == lpf.RFreq;
	}
	bool operator !=(const TLPFilter& lpf) const
	{  return !(lpf == *this); }
};
// ���������� �������� ��
#ifdef __CFGDE_P347ADC24__
#define LPFiltersCount 16
#else
#define LPFiltersCount 13
#endif
// ������ �������������� �������� ��� 
// ����� ������� = log2 �� ��������� �������
extern TLPFilter LPFilters[LPFiltersCount];
extern bool GetLPFilterIndexValid(unsigned short ALPFilterIndex);
extern int GetLPFilterIndexByMFreq(TDSPE_Float AMFreq);
extern int GetLPFilterIndexByRFreq(TDSPE_Float ARFreq);

// ������� �������������� �������� �������� ���� � ���������� � ��������
extern int GetLPSocketByDSPEmulIndex(int ALPFDSPEmulIndex);
extern int GetLPDSPEmulBySocketIndex(int ALPFSocketIndex);

//--------------------------------------------------------------------------
// ������� ���������
//--------------------------------------------------------------------------
// ���� �������� ��������� (Envelop Type)
#define ENVT_NONE			0
#define ENVT_1_3_20000		1
#define ENVT_1_3_16000		2
#define ENVT_1_3_12800		3
#define ENVT_1_1_15000		4
//--------------------------------------------------------------------------
// ����������� ���� ���������� �������
// Index - ������ � ������ ��������;
// CFreq - ����������� �������;
// Width - ������ (������������, ����������� �� ����������);
// Oct   - ���������� �������;
// Dec   - ��������� ������� (���� ��������� * ��������������� ���, �.�. = PreDec * BPDec);
// PreDec - ��������� ������� ��������������� (��� �������� ��� ������� ������������� ������� DEFAULT_SAMPLING_FREQ ��);
// BPDec - ��������� ������ ���������� �������;
// EnvType - ��� ������� ���������

struct TBPFilter
{
	unsigned char Index;
	int CFreq;
	int Width;
	int Oct;
	int Dec;
	int PreDec;
	int BPDec;
	unsigned char EnvType;
	bool operator ==(const TBPFilter& bpf) const
	{
		return Index == bpf.Index && CFreq == bpf.CFreq && Width == bpf.Width &&
			   Oct == bpf.Oct && Dec == bpf.Dec &&
			   PreDec == bpf.PreDec && BPDec == bpf.BPDec && EnvType == bpf.EnvType;
	}
	bool operator !=(const TBPFilter& bpf) const
	{  return !(bpf == *this); }
};
// ���������� ��������� ��������
#ifdef __CFGDE_P347ADC24__
#define BPFiltersCount 30
#else
#define BPFiltersCount 18
#endif
// ������ �������������� ��������� ��������
extern TBPFilter BPFilters[BPFiltersCount]; 
extern bool GetBPFilterIndexValid(unsigned short ABPFilterIndex);
extern int GetBPFilterIndexByCFreq(int ACFreq);
extern int GetMinLPFilterIndexForBPFilter(unsigned short ABPFilterIndex);

// ������� �������������� �������� �������� ���� � ���������� � ��������
extern int GetBPSocketByDSPEmulIndex(int ABPFDSPEmulIndex);
extern int GetBPDSPEmulBySocketIndex(int ABPFSocketIndex);
//--------------------------------------------------------------------------
// ������� �����������������
//--------------------------------------------------------------------------
// ���� �������� ����������������� (Resample Type)
// ������ ����������� RSMPT_X_Y:
// X - ����������� ������������;
// Y - ����������� ���������.
#define RSMPT_NONE			0
#define RSMPT_16_15			1		 
#define RSMPT_4_5			2
#define RSMPT_1024_1125		3		 
#define RSMPT_16_15_Ex		4		 
#define RSMPT_4_5_Ex		5
#define RSMPT_1024_1125_Ex	6		 

//--------------------------------------------------------------------------
// ����������� ���� ������� �����������������
// Index - ������ � ������ ��������;
// Interp - ����������� ������������ �������;
// Dec   - ����������� ��������� ��������;
// Rel - ��������� Interp / Dec;
// ResampleType - ��� ������� �����������������
struct TRSMPFilter
{
	unsigned char Index;
	int Interp;
	int Dec;
	TDSPE_Float Rel;
	unsigned char ResampleType;
	bool operator ==(const TRSMPFilter& b) const
	{
		return Index == b.Index && Interp == b.Interp && Dec == b.Dec &&
			   Rel == b.Rel && ResampleType == b.ResampleType;
	}
	bool operator !=(const TRSMPFilter& b) const
	{  return !(b == *this); }
};
// ���������� �������� �����������������
#define RSMPFiltersCount 6
// ������ �������������� �������� �����������������
extern TRSMPFilter RSMPFilters[RSMPFiltersCount]; 
extern bool GetRSMPFilterIndexValid(unsigned short ARSMPFilterIndex);
extern int GetRSMPFilterIndexByInterpDec(int AInterp, int ADec);

//--------------------------------------------------------------------------
// �� ������� 
//--------------------------------------------------------------------------
// ���� �� �������� 
// ������ ����������� HPFT_Fstop_Fpass_Astop_Apass_Fs:
#define HPFT_NONE				0
#define HPFT_01_1_40_002_4096	1		 
#define HPFT_01_1_40_002_8192	2
#define HPFT_01_10_40_002_4096	3
#define HPFT_01_10_40_002_8192	4
#define HPFT_01_20_40_002_4096	5
#define HPFT_01_20_40_002_8192	6
#define HPFT_03_2_36_1_9000		7
#define HPFT_2_5_10_36_01_9000	8
#define HPFT_2_5_20_36_01_9000	9

//--------------------------------------------------------------------------
// ����������� ���� �� �������
// Index - ������ � ������ ��������;
// SFreq - (Stop freq)�������, ��� ������� ���������� �� ������� ��������� ��������� ��������;
// MFreq - ������� ����� �� �������;
// RFreq - ������� ������������� ��� ������� �� �������;
struct THPFilter
{
	//int MFreq1;
	unsigned char Index;
	TDSPE_Float SFreq;
	TDSPE_Float MFreq;
	int RFreq;
	unsigned char HPFType;
	bool operator ==(const THPFilter& hpf) const
	{
		return Index == hpf.Index && SFreq == hpf.SFreq && 
			MFreq == hpf.MFreq && RFreq == hpf.RFreq && HPFType == hpf.HPFType;
	}
	bool operator !=(const THPFilter& hpf) const
	{  return !(hpf == *this); }
};
// ���������� �������� ��
#ifdef __CFGDE_P347ADC24__
#define HPFiltersCount 9
#else
#define HPFiltersCount 9
#endif
// ������ �������������� �������� �� 
extern THPFilter HPFilters[HPFiltersCount];
extern bool GetHPFilterIndexValid(unsigned short AHPFilterIndex);
//extern int GetHPFilterIndexByMFreq(TDSPE_Float AMFreq, TDSPE_Float ARFreq);

//--------------------------------------------------------------------------
// ����������� (SP) ������� (Special pass filters == SP filters)
//--------------------------------------------------------------------------
// ���� SP �������� 
#define SPFT_NONE							0
// ��������� SP ������� SPFT_LP1000_4000_01_40_9000:
// Fpass = 1000 ��; Fstop = 4000 ��; 
// Apass = 0.1 ��; Astop = 40 ��; 
#define SPFT_LP1000_4000_01_40_9000         1
// ��������� SP ������� SPFT_LP2000_4000_01_40_9000:
// Fpass = 2000 ��; Fstop = 4000 ��; 
// Apass = 0.1 ��; Astop = 40 ��; 
#define SPFT_LP2000_4000_01_40_9000         2

//--------------------------------------------------------------------------
// ����������� ���� ������������ (SP) ������� 
// Index - ������ � ������ ��������;
// RFreq - ������� ������������� ��� ������� �� �������;
struct TSPFilter
{
	//int MFreq1;
	unsigned char Index;
	int RFreq;
	unsigned char SPFType;
	bool operator ==(const TSPFilter& spf) const
	{
		return Index == spf.Index && RFreq == spf.RFreq && SPFType == spf.SPFType;
	}
	bool operator !=(const TSPFilter& spf) const
	{  return !(spf == *this); }
};
// ���������� �������� ��
#ifdef __CFGDE_P347ADC24__
#define SPFiltersCount 2
#else
#define SPFiltersCount 2
#endif
// ������ �������������� ����������� (SP) ��������
extern TSPFilter SPFilters[SPFiltersCount];
extern bool GetSPFilterIndexValid(unsigned short ASPFilterIndex);
//--------------------------------------------------------------------------
// ������������� (INTG) ������� 
//--------------------------------------------------------------------------
// ���� INTG �������� 
#define INTGFT_NONE		0
// ��������� �������, ��������������� ������� �������� :
#define INTGFT_TRP      1
//--------------------------------------------------------------------------
// ����������� ���� ������������ (INTG) ������� 
// Index - ������ � ������ ��������;
struct TINTGFilter
{
	//int MFreq1;
	unsigned char Index;
	unsigned char INTGFType;
	bool operator ==(const TINTGFilter& intgf) const
	{
		return Index == intgf.Index && INTGFType == intgf.INTGFType;
	}
	bool operator !=(const TINTGFilter& intgf) const
	{  return !(intgf == *this); }
};
// ���������� �������� ��
#ifdef __CFGDE_P347ADC24__
#define INTGFiltersCount 1
#else
#define INTGFiltersCount 1
#endif
// ������ �������������� ������������� (INTG) ��������
extern TINTGFilter INTGFilters[INTGFiltersCount];
extern bool GetINTGFilterIndexValid(unsigned short AINTGFilterIndex);
//--------------------------------------------------------------------------
// ������� ��������, �������������� ������������
//--------------------------------------------------------------------------
// ���������� �������������� �������� ��������
#define SpectLinesArCount 9
// ������ �������������� �������� ��������
extern int SpectLinesAr[SpectLinesArCount];
extern bool GetSpectLinesArIndexValid(unsigned short ASpectLinesArIndex);

// ������� �������������� ���������� ����� � ������ 
extern int GetSpectLinesArIndexByLinesCount(int ACount);
// ������� �������������� ������� � ���������� ����� 
extern int GetLinesCountBySpectLinesArIndex(int AIndex);

// ������� �������������� �������� ���������� ����� ���� � ���������� � ��������
extern int GetSpLinesSocketByDSPEmulIndex(int ASpLinesDSPEmulIndex);
extern int GetSpLinesDSPEmulBySocketIndex(int ASpLinesSocketIndex);
//--------------------------------------------------------------------------
// ���� ���������� ��������
#define TS_NONE			0
#define TS_DIRECT		1
#define TS_ENVELOPE		2
//--------------------------------------------------------------------------
// ������� ��������� ���������
#define TU_AMP_S_MKM   0
#define TU_AMP_S_MM    1
#define TU_AMP_S_MILS  2
#define TU_AMP_S_DBD   3

#define TU_AMP_V_MM_S  4
#define TU_AMP_V_M_S   5
#define TU_AMP_V_IN_S  6
#define TU_AMP_V_DBV   7

#define TU_AMP_A_MM_S2 8
#define TU_AMP_A_M_S2  9
#define TU_AMP_A_G     10
#define TU_AMP_A_DBA   11

#define TU_AMP_COUNT   12	
//--------------------------------------------------------------------------
// ������� ��������� ����/����
#define TU_ANGLE_GRAD  0
#define TU_ANGLE_RAD   1

#define TU_ANGLE_COUNT 2	

extern bool GetAmpUnitValid(unsigned short AAmpUnit);
extern bool GetAngleUnitValid(unsigned short AAngleUnit);

// ������������ ��������� �� �� (������� �������� TU_AMP_V_M_S) � ��������� ������� ��������� ATypeUnitAmp
// ASIAmp - ��������� � ��
// AFreqHz - ������ � �� (��������� ��� �������� ����� �����������������, �������������� � ���������������)
// ATypeUnitAmp - ��� ��������� ������� ��������� TU_AMP_XXX
// AOtherUnitAmp - ��������� � ��������� ������� ���������
// ���������� ��� ������
int ConvertSIAmpInOtherUnitAmp(TDSPE_Float ASIAmp, TDSPE_Float AFreqHz, unsigned short ATypeUnitAmp, TDSPE_Float &AOtherUnitAmp);
// ������������ ��������� �� ������� ��������� ATypeUnitAmp � ��
// AOtherUnitAmp - ������� �������� ��������� � ������� ��������� ATypeUnitAmp
// AFreqHz - ������ � �� (��������� ��� �������� ����� �����������������, �������������� � ���������������)
// ATypeUnitAmp - ��� ������� ��������� TU_AMP_XXX �������� ��������� AOtherUnitAmp
// ASIAmp - ��������� � �� (�������� ��������)
// ���������� ��� ������
int ConvertOtherUnitAmpInSIAmp(TDSPE_Float AOtherUnitAmp, TDSPE_Float AFreqHz, unsigned short ATypeUnitAmp, TDSPE_Float &ASIAmp);
// ������������ ��������� �� ����� ������� ��������� AFstTypeUnitAmp � ������ ASndTypeUnitAmp.
// AFstUnitAmp - ��������� � ������� ��������� AFstTypeUnitAmp
// AFreqHz - ������ � �� (��������� ��� �������� ����� �����������������, �������������� � ���������������)
// AFstTypeUnitAmp - ��� ������� ��������� TU_AMP_XXX �������� ��������� AFstUnitAmp
// ASndTypeUnitAmp - ��� ������� ��������� TU_AMP_XXX ��������� ��������� ASndUnitAmp
// ASndUnitAmp - �������� �������� ��������� � ������� ��������� ASndTypeUnitAmp
// ���������� ��� ������
int ConvertFstUnitAmpInSndUnitAmp(TDSPE_Float AFstUnitAmp, TDSPE_Float AFreqHz, unsigned short AFstTypeUnitAmp, unsigned short ASndTypeUnitAmp, TDSPE_Float &ASndUnitAmp);
// ����������� SI-���� ������������� �������� �������� � ������ �������� ���������
// ����������� � ��� �� ������, �.�. �������������� � ��������������, �������� � �������� � 
// ���������������� � ����������������. ��� ���������������� SI-����� ��������� ��������� TU_AMP_S_MM,
// ��� ������������� - TU_AMP_V_M_S, ��� �������������� - TU_AMP_A_M_S2.
int ConvertOneDomainSIAmpInOtherUnitAmp(TDSPE_Float ASIDomainAmp, unsigned short ASIDomainTypeUnitAmp, 
										unsigned short ATypeUnitAmp, TDSPE_Float &AOtherUnitAmp);
// ����������� ��������� ������������� �������� �������� � SI-���� ������� ���������
// ����������� � ��� �� ������, �.�. �������������� � ��������������, �������� � �������� � 
// ���������������� � ����������������. ��� ���������������� SI-����� ��������� ��������� TU_AMP_S_MM,
// ��� ������������� - TU_AMP_V_M_S, ��� �������������� - TU_AMP_A_M_S2.
int ConvertOneDomainUnitAmpInSIAmp(TDSPE_Float AUnitAmp, unsigned short ATypeUnitAmp, 
								   unsigned short ASIDomainTypeUnitAmp, TDSPE_Float &ASIDomainAmp);
// ����������� ��������� ������������� �������� �������� � ������ ������� ���������
// ����������� � ��� �� ������, �.�. �������������� � ��������������, �������� � �������� � 
// ���������������� � ����������������. 
int ConvertOneDomainFstUnitAmpInSndUnitAmp(TDSPE_Float AFstUnitAmp, unsigned short AFstTypeUnitAmp, unsigned short ASndTypeUnitAmp, TDSPE_Float &ASndUnitAmp);


// ���� ������, ������������ ��������� ����������� ������ ���������
#define CUEC_NONE						0	//��� ������ ��������
#define CUEC_AMP_MUST_ZERO_OR_POSITIVE	-1	//��������� �. ���� �� ������ ����
#define CUEC_FREQ_MUST_POSITIVE			-2	//������� ��� ��������� ������ ��������� �.���� ������ ����
#define CUEC_LOG_AMP_MUST_POSITIVE		-3	//��������� ��� ��������������� ������ ��������� �.���� ������ ����
#define CUEC_AMP_UNIT_NOT_EXIST			-4	//�� ���������� ������ ��������� ��������� � ��������� ��������
#define CUEC_AMP_UNITS_MUST_ONE_DOMAIN	-5	//������� ��������� ������ ���� � ����� ������
#define CUEC_AMP_UNIT_NOT_SIDOMAIN_AMP_UNIT	-6	//������� ��������� �� �������� SI-���� � ������ ������
//--------------------------------------------------------------------------
// ���������, ����������� ���������� ��������
typedef struct
{
	int AvgCnt;
	TDSPE_Float Avg;
	TDSPE_Float Min;
	TDSPE_Float Max;
}TStatRot;
//--------------------------------------------------------------------------
// ���������, ����������� �������� �� ��������� �������
// Type - ��� ��������
// Value - �������� ��������
#define OFFST_SAMPLES	0	// �������� � �������� �������
#define OFFST_ROTMETKAS	1	// �������� � ��������� ������
struct TOffSet
{
	char Type;
	int64_t Value;
	bool operator ==(const TOffSet& ofs) const
	{
		return Type == ofs.Type && Value == ofs.Value;
	}
	bool operator !=(const TOffSet& ofs) const
	{  return !(ofs == *this); }
};
//--------------------------------------------------------------------------
// ���������, ���������� ���������� (�����������) � ������� ��������
struct TFrotLimits
{
	TDSPE_Float Frot_min; //����������� �������� �������� � ��
	TDSPE_Float Frot_max; //������������ �������� �������� � ��
	TDSPE_Float DeltaFrotOneMeas; //����������� ���������� ��������� ������� �������� �� ����������
	TDSPE_Float DeltaFrotAllMeas; //����������� ���������� ��������� ������� �������� �� ���������
	bool operator ==(const TFrotLimits& b) const
	{
		return Frot_min == b.Frot_min && 
			Frot_max == b.Frot_max &&
			DeltaFrotOneMeas == b.DeltaFrotOneMeas &&
			DeltaFrotAllMeas == b.DeltaFrotAllMeas;
	}
	bool operator !=(const TFrotLimits& b) const
	{  return !(b == *this); }
};
//-----------------------------------------------------------------------------
// ------ ���� ������ ��� ������ � �������� ��������� DSP01-03 � P347_v1. -----
//-----------------------------------------------------------------------------
// ��� ��� ������ ������� ������� � ��� DSP01-03 (16 ���)
typedef short tssample_t;
//--------------------------------------------------------------------------
// ��� ��� ������ ������� ������� � ��� P347_v1 (24 ��� data + 8 ��� info)
typedef int32_t tssample24_8_t;
// ��������������� ����������� ��� ������ ������� ������� � ��� P347_v1 (24 ��� data + 8 ��� info)
// ������ ��� ����� �������� ������� � ��������� ������ ������� �������.
typedef union 
{
	struct 
	{
#ifdef CPU_BIGENDIAN
		tssample_t L;
		tssample_t H;
#else
		tssample_t H;
		tssample_t L;
#endif
	};
	tssample24_8_t Val;
} tssample24_8_ext_t;
// �����, ���������� ������ ������� ������� � ��� P347_v1 (24 ��� data + 8 ��� info)
#define MASK_TSSAMPLE24_8_DATA		0xFFFFFF00
// �����, ���������� ��������� ����� �� ������� ������� � ��� P347_v1 (24 ��� data + 8 ��� info)
#define MASK_TSSAMPLE24_8_DO_METKA	0x00000001
//--------------------------------------------------------------------------
// ��� ��� ������ ������� ������� � ��� P347_v1 (18 ��� data + 8 ��� info)
typedef int32_t tssample18_8_t;
// ��������������� ����������� ��� ������ ������� ������� � ��� P347_v1 (18 ��� data + 8 ��� info)
// ������ ��� ����� �������� ������� � ��������� ������ ������� �������.
typedef union 
{
#ifdef CPU_BIGENDIAN
	tssample_t L;
	tssample_t H;
#else
	tssample_t H;
	tssample_t L;
#endif
	tssample18_8_t Val;
} tssample18_8_ext_t;
// �����, ���������� ������ ������� ������� � ��� P347_v1 (18 ��� data + 8 ��� info)
#define MASK_TSSAMPLE18_8_DATA		0xFFFFC000
// �����, ���������� ��������� ����� �� ������� ������� � ��� P347_v1 (18 ��� data + 8 ��� info)
#define MASK_TSSAMPLE18_8_DO_METKA	0x00000001
//--------------------------------------------------------------------------
// ��� ��� ������ ������� ������� (24 ��� data �������)
typedef int32_t tssample24_t;
//--------------------------------------------------------------------------
// ��� ��� ������ ������� ������� (18 ��� data �������)
typedef int32_t tssample18_t;
//--------------------------------------------------------------------------
// ��� ��� ������ ������� ������� (32 ���)
typedef int32_t tssample32_t;
//--------------------------------------------------------------------------
// ��� ��� ������ ����� �������� (32 ���)
typedef uint32_t tsrotmetka_t;
//--------------------------------------------------------------------------
// ���� ����� ������, � ������� �������� ��������� ������.
// � ������ ����� ������ � ��������� ������ ��������������,
// ��� � ������ ����������� �������� ������ �������� �� 32768,
// �.�. ����������� � ��������� -1..+1.
#define TSDTC_LONGDOUBLE	0		// TDSPE_LongDouble
#define TSDTC_DOUBLE		1		// TDSPE_Double
#define TSDTC_FLOAT			2		// TDSPE_Float
#define TSDTC_SHORT			3		// �������� short
#define TSDTC_24DATA_8STATUS 4		// 24 ���� ������ � ���, 8 ��� ��������� (��������� ��� � ������� ��������� �����)
#define TSDTC_18DATA_8STATUS 5		// 18 ���� ������ � ���, 8 ��� ��������� (��������� ��� � ������� ��������� �����)
#define TSDTC_24DATA		 6		// 24 ���� ������ � ���
#define TSDTC_18DATA		 7		// 18 ���� ������ � ���
#define TSDTC_32DATA		 8		// 32 ���� ������ 

#define TSDTC_COUNT			9		// ���������� ����� ����� ������ �������� ���������� �������

#define TSDTC_SRC			TSDTC_COUNT // ��� ���� �������, �����������, ��� ����� ������������ ��� ������� ���������

// ���������� ������ ���� ������ �� ��� ����
extern unsigned int GetTSDTCSize(unsigned char ATSDTC);
// ���������� ���������� ������� ��� ���� ������ �� ��� ����
extern unsigned char GetTSDTCBitSize(unsigned char ATSDTC);
// ���������� ������������ ���� ���� ������ ���������� �������
extern bool GetTimeSigDataTypeCodeValid(int ATSDTC);

// ��������������� ������� ����������� ������� ���� ATSDTCSrc �� ������ ABuffSrc 
// ������ ALength � ������ ���� ATSDTCDest � ����� ABuffDest.
// ��� ACreateBuff == false, ��� ������ ������ ���� ������� ����� ������� �������.
// ��� ACreateBuff == true, �������� ����� ABuffDest ��������� � ���� � ������ ����
// ������ ��������������. 
// ������� ����� � ������� �� ����������, �.�. ���� ���� �������������� �� ������� �� �����������
// ����� � ������, ������� ����� �����������, �� ����� �� �����������.
extern bool ConverSig(void* ABuffSrc, unsigned int ALength, unsigned char ATSDTCSrc, unsigned char ATSDTCDest, bool ACreateBuff, void* &ABuffDest);
//--------------------------------------------------------------------------
// ���� ���������� ���������� �������.
#define TSST_NONE		-1	// �������������� �������� ����. � ���� ������ ���������� �������� �� TimeSigID ������� � ��������� CMN.
#define TSST_CURRENT	0	// ������� �������� �������. (������ ���������� ��������� ������)
#define TSST_ORIGINAL	1	// ������ �������� �������� �������. (��������, ��� ������ ����������� ��� ��������� ����� ������ ��)
#define TSST_VIBEG		2	// ������ �������� ������� �� ������.

#define TSST_COUNT		3	// ���������� ����� ���������� �������.

extern bool GetTimeSigSourceTypeValid(int ATimeSigSourceType);
//--------------------------------------------------------------------------
// ���� ���������, � ������� ����� ���������� ������
#define TTS_STOP		0	// ������ �����������. �������� ����� �� ����������, �� ���� �� �������, �� ����������� �� BegOffSet.
#define TTS_PAUSE		1	// ������ �� �����. �������� ����� �� ���� �� �������, �� ����������� �� BegOffSet � �� ������� ������ >= BegOffSet.
#define TTS_PLAY		2	// ������ �������. �������� ����� �� ���� ������, ����������� �� BegOffSet.

#define TTS_COUNT		3	// ���������� ����� ��������� ������

extern bool GetTaskTypeStateValid(char ATaskTypeState);
//--------------------------------------------------------------------------
// ���� �������� ��� �������������� ������ ����� (Type OffSet Auto)
#define TOSA_FROM_END_TS	-1
#define TOSA_FROM_BEGIN_TS	-2
//--------------------------------------------------------------------------
// ���������, ���������� ��� ����������� ���������
// ��� ��������� � ��������� commontaskparams_t �� �������.
struct LPFParams_t{
	int LPFilterIndex;
	unsigned int LPFilterDecim;
	int LPFilterType;	
	bool LPFilterUsualW;
	bool operator ==(const LPFParams_t& b) const
	{
		return 
			LPFilterIndex == b.LPFilterIndex &&
			LPFilterDecim == b.LPFilterDecim &&
			LPFilterType == b.LPFilterType &&	
			LPFilterUsualW == b.LPFilterUsualW;
	}
	bool operator !=(const LPFParams_t& b) const
	{  return !(b == *this); }
};

// ���������, ���������� ��� ����������� ���������
// ��� ��������� � ��������� commontaskparams_t ���������� (���������) �������.
struct BPFParams_t{
	int BPFilterIndex;
	int BPFilterType;
	bool BPFilterUsualW;
	bool operator ==(const BPFParams_t& b) const
	{
		return 
			BPFilterIndex == b.BPFilterIndex &&
			BPFilterType == b.BPFilterType &&	
			BPFilterUsualW == b.BPFilterUsualW;
	}
	bool operator !=(const BPFParams_t& b) const
	{  return !(b == *this); }
};

// ���������, ���������� ��� ����������� ���������
// ��� ��������� � ��������� commontaskparams_t ������� ����������������� �������.
struct RSMPFParams_t{
	int RSMPFilterIndex;
	int RSMPFilterType;	
	bool RSMPFilterUsualW;	
	bool operator ==(const RSMPFParams_t& b) const
	{
		return 
			RSMPFilterIndex == b.RSMPFilterIndex &&
			RSMPFilterType == b.RSMPFilterType &&	
			RSMPFilterUsualW == b.RSMPFilterUsualW;
	}
	bool operator !=(const RSMPFParams_t& b) const
	{  return !(b == *this); }
};

// ���������, ���������� ��� ����������� ���������
// ��� ��������� � ��������� commontaskparams_t �� �������.
struct HPFParams_t{
	int HPFilterIndex;
	int HPFilterType;	
	TDSPE_LongDouble HPFilterParam1;	
	bool operator ==(const HPFParams_t& b) const
	{
		return 
			HPFilterIndex == b.HPFilterIndex &&
			HPFilterType == b.HPFilterType &&	
			HPFilterParam1 == b.HPFilterParam1;
	}
	bool operator !=(const HPFParams_t& b) const
	{  return !(b == *this); }

};

// ���������, ���������� ��� ����������� ���������
// ��� ��������� � ��������� commontaskparams_t ������������ (SP) �������.
struct SPFParams_t{
	int SPFilterIndex;
	int SPFilterType;
	bool SPFilterUsualW;
	bool operator ==(const SPFParams_t& b) const
	{
		return 
			SPFilterIndex == b.SPFilterIndex &&
			SPFilterType == b.SPFilterType &&	
			SPFilterUsualW == b.SPFilterUsualW;
	}
	bool operator !=(const SPFParams_t& b) const
	{  return !(b == *this); }
};

// ���������, ���������� ��� ����������� ���������
// ��� ��������� � ��������� commontaskparams_t �������������� (INTG) �������.
struct INTGFParams_t{
	int INTGFilterIndex;
	int INTGFilterType;
	bool operator ==(const INTGFParams_t& b) const
	{
		return 
			INTGFilterIndex == b.INTGFilterIndex &&
			INTGFilterType == b.INTGFilterType;
	}
	bool operator !=(const INTGFParams_t& b) const
	{  return !(b == *this); }
};


// ����, ��������������� ���������� �������� ���������� ��������
#define FPARAMS_NONE	0
#define FPARAMS_LP		1
#define FPARAMS_BP		2
#define FPARAMS_RSMP	3
#define FPARAMS_HP		4
#define FPARAMS_SP		5
#define FPARAMS_INTG	6

#define FPARAMS_COUNT	7

extern bool GetFParamsTypeValid(int AFParamsType);

// ���������, ���������� ��� ����������� ���������
// ��� ��������� � ��������� commontaskparams_t ������������ (SP) �������.
struct FParams{
	int Type;
	union {
		LPFParams_t		LPF;
		BPFParams_t		BPF;
		RSMPFParams_t	RSMPF;
		HPFParams_t		HPF;
		SPFParams_t		SPF;
		INTGFParams_t	INTGF;
	};
	bool operator ==(const FParams& b) const
	{
		bool res = false;
		if (Type == b.Type)
		{
			switch (Type)
			{
				case FPARAMS_NONE: return true;
				case FPARAMS_LP	 : return LPF == b.LPF;
				case FPARAMS_BP	 : return BPF == b.BPF;
				case FPARAMS_RSMP: return RSMPF == b.RSMPF;
				case FPARAMS_HP	 : return HPF == b.HPF;
				case FPARAMS_SP	 : return SPF == b.SPF;
				case FPARAMS_INTG : return INTGF == b.INTGF;
			}
		}
		return res;
	}
	bool operator !=(const FParams& b) const
	{  return !(b == *this); }
};

//--------------------------------------------------------------------------

// ���������, ���������� ����� ���������, ����������� ��� ��������� ����������� ���� �����
// OffSet - �������� �� ��������� �������, � �������� ���������� ��� ��������� (��������� ������� ����������).
//			���� OffSet.Value == TOSA_FROM_END_TS, ��� ��������, ��� ������ ���� ������ �����������.
//			� �������� �� OffSet ����� ������ ����� ����� �������, ������� �� ������ ������ ���� � ������.
//			���� OffSet.Value == TOSA_FROM_BEGIN_TS, ��� ��������, ��� ������ ���� ������ �����������.
//			� �������� �� OffSet ����� ������ ����� ������ �������, ������� �� ������ ������ ���� � ������.
// DeltaOffSet - ���������� �������� �� ��������� �������, � �������� ���������� ��������� ������� ������ ����������
//				(���� DeltaOffSet.Value == 0, �� ������� �� ���������� �� �����������.)
// Monitoring - ������� � ���, ��� ������ ������ �������� ��������������;
// TimeSigType - ��� ���������� �������, ���������� ����� ����������, �� ��������
//				 ����� ���������� ���������� ������, ��������� (TS_DIRECT, TS_ENVELOP ...).
// LPFilterIndex - ������ ��� � ������ �������� ���. (��� LPFilterDecim == 0).
// LPFilterDecim - ��������� ����������� ��������� ���. ������������ ��� ������� ���� �� ����� 0,
//					� ��������� ������ ������������ ������ LPFilterIndex.
// LPFilterType - ��� ��� ������� ��� ���������� (TF_SLIDING_XXX (��."dfilter.h"))
// LPFilterUsualW - Is Usual Width - ���� ����, ��� ���������� ������ W ������� �������� ������� (������������),
//                  �.�. Fstop == 0,5*Fs (Fs - ������� �������������). � ��������� ������ ��� == 0.609375*Fs/Decim.
// BPFilterIndex - ������ �� � ������ �������� ��.
// BPFilterType - ��� ���������� ������� ��� ���������� (TF_SLIDING_XXX (��."dfilter.h"))
// RSMPFilterIndex - ������ ������� ����������������� � ������ �������� ��.
// RSMPFilterType - ��� ������� ����������������� ��� ���������� (TF_SLIDING_XXX (��."dfilter.h"))
// RSMPFilterUsualW - Is Usual Width - ���� ����, ��� ���������� ������ W ������� �������� ������� (������������),
//                  �.�. Fstop == 0,5*Fs (Fs - ������� �������������). � ��������� ������ ��� == 0.609375*Fs*Interp/Decim.
// HPFilterIndex - ������ ��� � ������ �������� ���.
// HPFilterType - ��� ��� ������� ��� ���������� (TF_SLIDING_XXX (��."dfilter.h"))
// FParamsList - ������ ���������� �������� ��� �������������� ���������, ��� ��� ������ �������
//				 ������������ ����� ���� ����������� ������� ����� �������� (LP, BP, RSMP � �.�.).
//				 ��� ������� ��������� � ������������ � ������� ������������ � ������ FParamsList.
//				 �� ������������� ��������� ������� ������� � ���������, ���������� ����� 
//				 ��� ���������, �������� �����������, ������������� ��� �������, �.�.
//				 ����� �������, ��� ��� ����� �������������� ������ ��������� ����������.
//				 ���� �� ���� �� �������� �� �������� ������� �������, �� ������ ������, ����������
//				 ��� �������� ������ ���������� ������������ � ��������. ��������� ��������, 
//				 ����� ������ �����������, � ����� ������ ����������, ����� ��� ������ ���� �� �����-�� 
//				 ������ �� ����������� � �� ������������ ������.
// FParamsCount - ���������� ��������� � ������ FParamsList.
// SPFilterIndex - ������ ������������ (SP) ������� � ������ �������� ���.
// SPFilterType - ��� ������������ (SP) ������� (TF_SLIDING_XXX (��."dfilter.h"))
// AvgCount - ���������� ����������;
// FrotLimits - ���������, ���������� ���������� (�����������) � ������� ��������
// StabControl - �������� ������������.
// RotControl - �������� ������ ��������. ���� ������ ���� ����� false, �� ���������� �����
//				���������� ��� ���������� �� ��������, � ��������� ������ ���������� �����
//				���������� ������ ��� ������� ��������� �����, �� ������� ����� ��������� ����������.
// WaitPeriodForRotMetka - ������ ������� � �������� ��������� (��������) ������� ��� �������� ��������� 
//				�����. ��� RotControl == true, ������ �������� ������������, ��� ��� ���� �� ������ �����������
//				���������� ��������� ����� ��� ������� ���������� ���������� �� ������� ��������, ������ �� �������
//				��������� ���������. 
//				� ������ ��, ����� RotControl == false, ����� ������ �� ������������ ��������� �����
//				� �� ����������� ������ ���������� ��������� �����, ���������� �����, ������� ������� ������ 
//				����� ���������, ���� ��� ���������� ����� ���� ��������� ���������� ���������� �� ��������.
//				��� �������� ������ ����� ���������, ��������, ��� ������, ����������� ��������� �������. 
//				����������� �� ������ ����� ����� ��������� ������, ������� ����� ������������ ��� ������, 
//				� ������� RotControl==false, ��� � ������ � RotControl == true. � ��������� ������ ��� ������������� 
//				��������� WaitPeriodForRotMetka, ������ ����� �������� ������ �����, ����� � ������ ��������� 
//				�������� RotControl==true, �� ��� �� ������ ��������.
//				�������� ����� ��������� �� ��������� ��� �����, ����������� ������ ������� �������� ������,
//				������� ������������� ������� ����������� ������� ��������, ������� ���������� ������ �������,
//				��� ��� � ���� ������ � �� ������� ������� ������ ��������, � ����� ������ ����� ��� �������
//				���� �� ����� ������, ������� � ���������� �� ��������� �������� ������� (��. DEFAULT_WAITPERIODFORROTMETKA).
// TimeSigSrcType - ��� ��������� ���������� �������, ��� �������� ������������� ������ ������ (TSST_XXX).
//				����������, ����� ������ ����� ��� ���, ��������, �������� ��� ������ �� ������. ����� ����� 
//				�������, ��� �� �����, ����� ������ ����� ���������� (TimeSigSrcType == TSST_CURRENT).
//				���� TimeSigSrcType == TSST_NONE, �� ������������� �������� TimeSigID, ������� ����� 
//				������� ID ����������� ������� � �������� ���������.
// TimeSigID - ID ���������� �������, �� �������� ����� ���������� ���������� ������ ������. ���������
//				���� �����, ����� TimeSigSrcType == TSST_NONE.
// InitTaskState - ��������� ��������� ���������� ������ TTS_XXX;
// FinalTaskState - �������� ��������� ���������� ������ TTS_XXX. ���� ������ ��������������, �� ���� �������� �����
//				��������������, ���� �� ���� �� ������� ���������.
// TSDTC -		��� ������, � ������� �������� ������ �� ���������� ������� ��� ������. ����� ��� ���������
//				����� ��, � ����� ������� ����� ��������� ������ � ������� ��������� �������, ���� � 
//				��������� �����, ����� ��������� ������ ������ ��������.
// CalcTypeTSDTC - ��� ������, �� ������ �������� ����� ����������� �������������� ����������. ����� �������,
//				��� ��� �������� ����������, ����� �� �������������� ���������� � ��������� ��� �������������
//				������. ����� ��� ������������� ����� ����������� ���������� �������� ��������. ������ ��������
//				��������� ����� �� ������ ��������, � ������� �������� ��� ������, � ������ ������������� ���������.
// TDE			- ��� ����������.
#define DEFAULT_WAITPERIODFORROTMETKA	2.0	// ���

// ������������ ���������� ���������� �������� � ������ FParamsList
#define FPARAMSLIST_MAX_COUNT	10

struct commontaskparams_t
{
	TOffSet OffSet;		 
	TOffSet DeltaOffSet;
	bool Monitoring;
	int TimeSigType;
	int LPFilterIndex;
	unsigned int LPFilterDecim;
	int LPFilterType;	
	bool LPFilterUsualW;	
	int BPFilterIndex;
	int BPFilterType;
	bool BPFilterUsualW;
	int RSMPFilterIndex;
	int RSMPFilterType;	
	bool RSMPFilterUsualW;	
	int HPFilterIndex;
	int HPFilterType;	
	TDSPE_LongDouble HPFilterParam1;
	FParams FParamsList[FPARAMSLIST_MAX_COUNT];
	unsigned int FParamsCount;
	int AvgCount;
	TFrotLimits FrotLimits;
	bool FrotLimitsActive;
	bool StabControl;
	bool RotControl;
	TDSPE_Float WaitPeriodForRotMetka;
	int TimeSigSrcType;
	int TimeSigID;
	int InitTaskState;
	int FinalTaskState;
	unsigned char TSDTC;
	unsigned char CalcTypeTSDTC;
	unsigned char TDE;

	bool operator ==(const commontaskparams_t& b) const
	{
		bool res = OffSet == b.OffSet && 
			DeltaOffSet == b.DeltaOffSet &&
			Monitoring == b.Monitoring &&
			TimeSigType == b.TimeSigType &&
			LPFilterIndex == b.LPFilterIndex &&
			LPFilterDecim == b.LPFilterDecim &&
			LPFilterType == b.LPFilterType &&	
			LPFilterUsualW == b.LPFilterUsualW &&	
			BPFilterIndex == b.BPFilterIndex &&
			BPFilterType == b.BPFilterType &&	
			BPFilterUsualW == b.BPFilterUsualW &&
			RSMPFilterIndex == b.RSMPFilterIndex &&
			RSMPFilterType == b.RSMPFilterType &&	
			RSMPFilterUsualW == b.RSMPFilterUsualW &&	
			HPFilterIndex == b.HPFilterIndex &&
			HPFilterType == b.HPFilterType &&	
			HPFilterParam1 == b.HPFilterParam1 &&
			FParamsCount == b.FParamsCount;
			AvgCount == b.AvgCount &&
			FrotLimits == b.FrotLimits &&
			FrotLimitsActive == b.FrotLimitsActive &&
			StabControl == b.StabControl &&
			RotControl == b.RotControl &&
			TimeSigSrcType == b.TimeSigSrcType &&
			TimeSigID == b.TimeSigID &&
			InitTaskState == b.InitTaskState &&
			FinalTaskState == b.FinalTaskState &&
			TSDTC == b.TSDTC &&
			CalcTypeTSDTC == b.CalcTypeTSDTC &&
			TDE == b.TDE;
		if (res)
		{
			unsigned int Tmp = FParamsCount;
			if (Tmp > FPARAMSLIST_MAX_COUNT)
				Tmp = FPARAMSLIST_MAX_COUNT;
			for (int i = 0; i < Tmp; ++i)
			{
				if (FParamsList[i] != b.FParamsList[i]) 
					return false;
			}
		}
		return res;
	}
	bool operator !=(const commontaskparams_t& b) const
	{  return !(b == *this); }
};
//--------------------------------------------------------------------------

// ���������, ���������� ���������, ����������� ��� ��������� �������
// OffSet - �������� �� ��������� �������, � �������� ���������� ��� ��������� (��������� ������� �������)
// DeltaOffSet - ���������� �������� �� ��������� �������, � �������� ���������� ��������� ������� ������ �������
//				(���� DeltaOffSet.Value == 0, �� ������� �� ���������� �� �����������.)
// Monitoring - ������� � ���, ��� ������ ������ �������� ��������������;
// SpectrClassCode - ����� ������� (�������, �����������)(��. "TSpectr.h").
// SpectrType - ������, ��������� (TS_DIRECT, TS_ENVELOP ...).
// LPFilterIndex - ������ ��� � ������ �������� ���.
// LPFilterType - ��� ��� ������� ��� ���������� (SlidingFIR, SlidingIIR,  � ��. (��."dfilter.h"))
// BPFilterIndex - ������ �� � ������ �������� ��.
// BPFilterType - ��� ���������� ������� ��� ���������� (SlidingFIR, SlidingIIR,  � ��. (��."dfilter.h"))
// RSMPFilterIndex - ������ ������� ����������������� � ������ �������� ��.
// RSMPFilterType - ��� ������� ����������������� ��� ���������� (TF_SLIDING_XXX (��."dfilter.h"))
// HPFilterIndex - ������ ��� � ������ �������� ���.
// HPFilterType - ��� ��� ������� ��� ���������� (TF_SLIDING_XXX (��."dfilter.h"))
// SPFilterIndex - ������ ������������ (SP) ������� � ������ �������� ���.
// SPFilterType - ��� ������������ (SP) ������� (TF_SLIDING_XXX (��."dfilter.h"))
// SpectLinesArrIndex - ������ ���������� ����� � ������ ���������� �����.
// SpectAvgCount - ���������� ����������;
// FrotLimits - ���������, ���������� ���������� (�����������) � ������� ��������
// StabControl - �������� ������������ ��������.
// SpectAmpUnit - ������� ��������� ��������� �������� TU_AMP_XXX
// RemoveAvgLine - ����, ����������� �� ��, ��� ���������� ������� ���������� ������������ �������
//				���������� �������� ��������.
struct spectrtaskparams_t
{	
	union
	{
		commontaskparams_t CMN;
		struct
		{
			TOffSet OffSet;		 
			TOffSet DeltaOffSet;
			bool Monitoring;
			int SpectrType;
			int LPFilterIndex;
			unsigned int LPFilterDecim;
			int LPFilterType;	
			bool LPFilterGenW;
			int BPFilterIndex;
			int BPFilterType;
			bool BPFilterUsualW;
			int RSMPFilterIndex;
			int RSMPFilterType;
			bool RSMPFilterGenW;
			int HPFilterIndex;
			int HPFilterType;
			TDSPE_LongDouble HPFilterParam1;
			FParams FParamsList[FPARAMSLIST_MAX_COUNT];
			unsigned int FParamsCount;
			int SpectAvgCount;
			TFrotLimits FrotLimits;
			bool FrotLimitsActive;
			bool StabControl;
			bool RotControl;
			TDSPE_Float WaitPeriodForRotMetka;
			int TimeSigSrcType;
			int InitTaskState;
			int FinalTaskState;
			unsigned char TSDTC;
			unsigned char CalcTypeTSDTC;
			unsigned char TDE;
		};
	};
	int SpectrClassCode; 
	int SpectLinesArrIndex;
	unsigned short SpectAmpUnit;
	bool RemoveAvgLine;

	bool operator ==(const spectrtaskparams_t& b) const
	{
		return CMN == b.CMN && 
			SpectrClassCode == b.SpectrClassCode &&
			SpectLinesArrIndex == b.SpectLinesArrIndex &&
			SpectAmpUnit == b.SpectAmpUnit &&
			RemoveAvgLine == b.RemoveAvgLine;
	}
	bool operator !=(const spectrtaskparams_t& b) const
	{  return !(b == *this); }
};
//--------------------------------------------------------------------------
// ���������, ���������� ���������, ����������� ��� ��������� ������ �������� 
// ������� ��������.
// SpectrTaskParams - ��������� �������, �� �������� ����� �������� ���������.
// CMN			-	���������, ������������ ������������� �����������,
//					�� �������� ����� �������� ���������� ��
//                  ���������. 
// VHarmonicsCount - ���������� ��������� ��������.
// VHarmonicsAvgCount - ���������� ���������� ��� ��������� ��������.
// SpectLinesArrIndex - ������ ���������� ����� � ������ ���������� �����.

struct vharmonicstaskparams_t
{
	spectrtaskparams_t SpectrTaskParams; // ����� ����� �� ���� ���������
	commontaskparams_t CMN;
	int VHarmonicsCount;
	int VHarmonicsAvgCount;				// ����� ����� �� ���� ���������
	int SpectLinesArrIndex;

	bool operator ==(const vharmonicstaskparams_t& b) const
	{
		return CMN == b.CMN && 
			VHarmonicsCount == b.VHarmonicsCount &&
			SpectrTaskParams == b.SpectrTaskParams &&
			VHarmonicsAvgCount == b.VHarmonicsAvgCount &&
			SpectLinesArrIndex == b.SpectLinesArrIndex;
	}
	bool operator !=(const vharmonicstaskparams_t& b) const
	{  return !(b == *this); }
};
//--------------------------------------------------------------------------
//vharmonic_t - ����������� ���������
struct vharmonic_t
{
	TDSPE_Float amp;	//��������� [��/���2]
	TDSPE_Float ph;	//���� [����]
	TDSPE_Float freq; //������� [��]
	TStatRot StatRot;
	unsigned int AvgCount;
};
//--------------------------------------------------------------------------
// ���������, ���������� ���������, ����������� ��� ��������� ���������� ���������
// �� ���������� �������.
// CMN			-	���������, ������������ ������������� �����������,
//					�� �������� ����� �������� ���������� ��
//                  ���������. 
// OneMeasTimeSigLength - ���������� ��������, �� ������� ��������� ���� �������� ���������.
// MeasCount - ���������� ��������� ��������� ���������
// CoefCover - ����������� ���������� (0..1) ���� �������� �������� �������, �� ������� ������������� ���� ��������
struct statkurtosistaskparams_t
{
	commontaskparams_t CMN;
	unsigned int OneMeasTimeSigLength;
	unsigned int MeasCount;
	TDSPE_Float CoefCover;

	bool operator ==(const statkurtosistaskparams_t& b) const
	{
		return CMN == b.CMN && 
			OneMeasTimeSigLength == b.OneMeasTimeSigLength &&
			MeasCount == b.MeasCount &&
			CoefCover == b.CoefCover;
	}
	bool operator !=(const statkurtosistaskparams_t& b) const
	{  return !(b == *this); }
};
//--------------------------------------------------------------------------
// ���������, ���������� ���������� ��������� �� ���������� �������.
// min - ����. �������� �� ���������;
// max - ���. �������� �� ���������;
// mean - ������� �������� �� ���������;
// std - ��� �� ��������� �� ���������;
// kurtosis - �������� �� ���������;
// peak_factor - ��� ������ �� ���������;
typedef struct
{
	TDSPE_Float min;
	TDSPE_Float max;
	TDSPE_Float mean;
	TDSPE_Float std;
	TDSPE_Float kurtosis;
	TDSPE_Float peak_factor;
	TStatRot StatRot;
	unsigned int AvgCount;
}statkurtosis_t;
//--------------------------------------------------------------------------
// ���������, ���������� ���������, ����������� ��� ��������� ����������
// �� ���������� �������.
// CMN			 -	���������, ������������ ������������� �����������,
//					�� �������� ����� �������� ����������
// OneMeasTimeSigLength - ���������� ��������, �� ������� ��������� ��� ��������� ����������.
// AmpUnit - ������� ��������� ��������, � ������� ������������ ���������. (�� ������ ������ ��������
//			������������� ������ TU_AMP_A_MM_S2, TU_AMP_A_M_S2, TU_AMP_A_G, ��� ��� ���������, ��� ����
//			������ � ������� ��������������)
// IntegrateInTimeDomain - ����, ��������� �� ��, ��� ��� �������� �������������� ���������� �����������
//			�� ��������� �������, �.�. ������������� ��������������� ��������� ������.
struct stattimesigtaskparams_t
{
	commontaskparams_t CMN;
	unsigned int OneMeasTimeSigLength;
	unsigned short AmpUnit;
	bool IntegrateInTimeDomain;

	bool operator ==(const stattimesigtaskparams_t& b) const
	{
		return CMN == b.CMN && 
			OneMeasTimeSigLength == b.OneMeasTimeSigLength &&
			AmpUnit == b.AmpUnit &&
			IntegrateInTimeDomain == b.IntegrateInTimeDomain;
	}
	bool operator !=(const stattimesigtaskparams_t& b) const
	{  return !(b == *this); }
};
//--------------------------------------------------------------------------
// ���������, ���������� ��������� �������������� ������ ���������� �������
// min - ����. ��������.;
// max - ���. ��������;
// mean - ������� ��������;
// std - ���;
// kurtosis - ��������;
// peak_factor - ��� ������;
typedef struct {
	TDSPE_Float min;
	TDSPE_Float max;
	TDSPE_Float mean;
	TDSPE_Float std;
	TDSPE_Float kurtosis;
	TDSPE_Float peak_factor;
	TStatRot StatRot;
	unsigned int AvgCount;
}stattimesig_t;
//------------------------------------------------------------------
// ���������, ���������� ���������, ����������� ��� ��������� ������ ���������
// ���������� �������.
// CMN			 -	���������, ������������ ��������������� �����������
struct playertimesigtaskparams_t
{
	commontaskparams_t CMN;

	bool operator ==(const playertimesigtaskparams_t& b) const
	{
		return CMN == b.CMN;
	}
	bool operator !=(const playertimesigtaskparams_t& b) const
	{  return !(b == *this); }
};
//--------------------------------------------------------------------------
// ���������, ���������� ���������, ����������� ��� ���������� ������� �� ������ � ������� 
// �� �������� �������.
// CMN			-	���������, ������������ ������������� �����������,
//					�� �������� ����� ������� ������ �� �������� �������. 
// Frot_base	-	������� �������, � ������� ���������� �������� ������ �� ������
//					� ������� �� ������ ������� �������� Frot_base.
// FrotAvgMetkasCount - ���������� �����, �� ������� ������������ ������� �������
//					�������� �� ���������.

// ���������� ����� �� ��������� FrotAvgMetkasCount, �.�. ���� 
// �� ������� ��������� ������ FrotAvgMetkasCount == 0
#define DEFAULT_FROTAVGMETKASCOUNT	3
struct vibegtaskparams_t
{
	commontaskparams_t CMN;
	TDSPE_Float	Frot_base;
	unsigned int FrotAvgMetkasCount;

	bool operator ==(const vibegtaskparams_t& b) const
	{
		return CMN == b.CMN && 
			Frot_base == b.Frot_base &&
			FrotAvgMetkasCount == b.FrotAvgMetkasCount;
	}
	bool operator !=(const vibegtaskparams_t& b) const
	{  return !(b == *this); }
};
//--------------------------------------------------------------------------
// ���������, ���������� ���������, ����������� ��� ��������� ����������
// �������� ���������� ������� � ��������� �����.
// CMN			 -	���������, ������������ ������������� �����������,
//					�� �������� ����� �������� ����������
// OneMeasTimeSigLength - ���������� ��������, �� ������� ��������� ��� ��������� ����������.
// ��������� �������� ������� �������� � G_gravity
// AmpPeakToPeakMin - ����������� ��������� �������� PeakToPeak;
// AmpPeakToPeakMax - ������������ ��������� �������� PeakToPeak;
// AmpAvgMin - ����������� ��������� ������� ��������;
// AmpAvgMax - ������������ ��������� ������� ��������;
// AmpStdMin - ����������� ��������� �������� ��������������������� ����������;
// AmpStdMax - ������������ ��������� �������� ��������������������� ����������;
// DeltaFrot - 	������������ �������� ��������� ������� �������� �� OneMeasTimeSigLength ��������,
//				���������� ������� �������� ������ ���� ���� ����������, �.�. ���� ����������, ���� ������.
//				� ��������� ������, ���������, ��� ������� ���������.
// FrotMin - ���������� ����������� �������� ������� ��������, ����� �������
//				������ ������ ���������� ������ ���������� ������� �������� ������������.
struct qualitytimesigtaskparams_t
{
	commontaskparams_t CMN;
	unsigned int OneMeasTimeSigLength;
	// ��������� �������� ������� ��������
	// � G_gravity
	TDSPE_Float AmpPeakToPeakMin;
	TDSPE_Float AmpPeakToPeakMax;
	TDSPE_Float AmpStdMin;
	TDSPE_Float AmpStdMax;
/*	// ��������� �������� ������� � ������� ��������
	TDSPE_Float DeltaFrot;
	TDSPE_Float FrotMin;*/
	
	bool operator ==(const qualitytimesigtaskparams_t& b) const
	{
		return CMN == b.CMN && 
			OneMeasTimeSigLength == b.OneMeasTimeSigLength &&
			AmpPeakToPeakMin == b.AmpPeakToPeakMin &&
			AmpPeakToPeakMax == b.AmpPeakToPeakMax &&
			AmpStdMin == b.AmpStdMin &&
			AmpStdMax == b.AmpStdMax;
/*			DeltaFrot == b.DeltaFrot &&
			FrotMin == b.FrotMin;*/
	}
	bool operator !=(const qualitytimesigtaskparams_t& b) const
	{  return !(b == *this); }
};
//--------------------------------------------------------------------------
// ���������, ���������� ��������� �������������� ������ �������� ���������� ������� 
// � ��������� �����
// 
// SigOverFlow - ���� ������������ �������;
// SigTooLow - ���� ����, ��� ������ ������� ������;
// SigTooHigh - ���� ����, ��� ������ ������� �������;
// AmpPeakToPeakCalc - ����������� �������� PeakToPeak;
// AmpStdCalc - ����������� �������� ��������������������� ���������� � G_gravity;
// SigOverFlowCount - ���������� ������������ �������;
typedef struct {
	bool SigOverFlow;
	bool SigTooLow;
	bool SigTooHigh;
	TDSPE_Float AmpPeakToPeakCalc;
	TDSPE_Float AmpStdCalc;
	unsigned int SigOverFlowCount;
/*	bool FrotSignalOk;
	bool MultipleRotMetka;
	bool RotMetkaLost;
	bool IsAccelerating;
	bool IsBraking;
	TDSPE_Float DeltaFrotCalc;*/

	TStatRot StatRot;
	unsigned int AvgCount;
}qualitytimesig_t;
//------------------------------------------------------------------
// ���������, ���������� ���������, ����������� ��� ������ ���������� �������.
// CMN			 -	���������, ������������ ������������� �����������,
//					�� �������� ����� ����������� ������ ��� ������ 
// OnePartTimeSigLength - ����� ������ ������������ ����� ������� �� ���� (���������� ������� ��������� � �����).
// TotalTimeSigLength - ����� ����� ������������ ������� �� ����. ���� ������� 0, �� �����, ���� �� ��������� ����.
//					������ ���� ��� ����� ������� ��������� �  �������. �.�. ���� ������� ������������� 
//					����������, ������� ���������� ��������� � D ���, �� ������������ ������� ����� �� �����
//					� D ��� ������.
// SigLengthWithFsSrc - ����, ���������, � ���� ��������� ����� �������� OnePartTimeSigLength � TotalTimeSigLength.
//					��� SigLengthWithFsSrc == true - ���� ������� ������������� 
//					����������, ������� ���������� ��������� � D ���, �� ������������ ������� ����� �� �����
//					� D ��� ������.
//					��� SigLengthWithFsSrc == false - ������������ ������� ����� �� ����� TotalTimeSigLength, ��� ��� 
//					������������� ���������� � ���������� � D ��� ������� � D ��� �������� ���������� ��������� �������.
// TSDTC - ������ ������, (��. TSDTC_XXX)

// ������������ ����� ����� ����� � �����
#define MAX_FILENAMEFORSAVE_LENGTH		1024
// ����� ������ ������������ ����� ������� �� ���� �� ��������� (���������� ������� ��������� � �����)
#define DEFAULT_ONEPARTTIMESIGLENGTHSAVE	8192

struct savetimesigtaskparams_t
{
	commontaskparams_t CMN;
	char FileName[MAX_FILENAMEFORSAVE_LENGTH];
	unsigned int OnePartTimeSigLength;
	unsigned int TotalTimeSigLength;
	bool SigLengthWithFsSrc;
	unsigned char TSDTC;

	bool operator ==(const savetimesigtaskparams_t& b) const
	{
		for (int i = 0; i < MAX_FILENAMEFORSAVE_LENGTH; i++)
		{
			if (FileName[i] != b.FileName[i]) return false;
		}
		return CMN == b.CMN && 
			OnePartTimeSigLength == b.OnePartTimeSigLength &&
			TotalTimeSigLength == b.TotalTimeSigLength &&
			SigLengthWithFsSrc == b.SigLengthWithFsSrc &&
			TSDTC == b.TSDTC;
	}
	bool operator !=(const savetimesigtaskparams_t& b) const
	{  return !(b == *this); }
};
//--------------------------------------------------------------------------
// ���������, ���������� ��������� ��������� ������ ���������� �������
typedef struct {
	TStatRot StatRot;
	unsigned int AvgCount;
}savetimesig_t;
//--------------------------------------------------------------------------
// ���������, ���������� ���������, ����������� ��� ��������� ������� �������� 
// �� ���������� �������.
// CMN			-	���������, ������������ ������������� �����������,
//					�� �������� ����� �������� ���������� �� ��������. 
// DeltaOffSetInSec - ����� ����� ����� ����������������� ����������� ������� �������� (���)
// ScaleKoeffFFT - �����������, ����������� �� ��, � ����� ���������� ��� ���������� ���������
//					���������� ������� ������� (���, ��� ���� �����, ����� ���������� �������)
//					��� ������� fft. ������ ���������� ����������� ������� ����������� �������
//					���������� �����.
// FrotMin - ����������� ������� ��������, � ������� ����� �������� �������� ������ ������. ���� ���� �������
//					������ ������ �������� �� ������������ ������, �.�. ���������� ����.
// FrotMax - ������������ ������� ��������, ������� ����� ����������� ���������. ���� ���� �������
//					������ ������ �������� �� ������������ ������.
// FrotAccelMax - ������������ ��������� ������� ��������, ������� ����� ����������� ���������. ���� ���� ��������
//					������ ������ �������� �� ������������ ������.
// AmpModulMin - ����������� �������� ������ ���������, ������� ����������� ��� ������ ��������� ������� ��������
//				 (������ - ��� ���������� �������� ��������� ��������� ����� �������� ����).
// STDMin - ����������� �������� STD
// RBFilterLength - ����� ���������� �������, ������� ������������ �������� ������� �������� �� �������� ������,
//					���������� �� ��������.

#define DEFAULT_SCALEKOEFFFFT	16				// ���
#define DEFAULT_VSFRDELTAOFFSETINSEC	0.25	// ���
#define DEFAULT_VSFRACCELMAX	0.2;			// ��� == 0.8(��/���) / 4;
#define DEFAULT_VSFRAVGCOUNT	6				// ����
#define DEFAULT_VSRBFILTERLENGTH	12			// ����
#define DEFAULT_VSFRAMPMODULMIN	5				// � �������� ��������� ��������� � ������� 
#define DEFAULT_VSFRSTDMIN	0.005				// � G.
struct vsensorfrottaskparams_t
{
	union {
		commontaskparams_t CMN;
		spectrtaskparams_t STP;
	};
	TDSPE_Float DeltaOffSetInSec;
	unsigned int ScaleKoeffFFT;
	TDSPE_Float FrotMin;
	TDSPE_Float FrotMax;
	TDSPE_Float FrotAccelMax;
	TDSPE_Float AmpModulMin;
	TDSPE_Float STDMin;
	unsigned int RBFilterLength;

	bool operator ==(const vsensorfrottaskparams_t& b) const
	{
		return STP == b.STP && 
			DeltaOffSetInSec == b.DeltaOffSetInSec &&
			ScaleKoeffFFT == b.ScaleKoeffFFT &&
			FrotMin == b.FrotMin &&
			FrotMax == b.FrotMax &&
			FrotAccelMax == b.FrotAccelMax &&
			AmpModulMin == b.AmpModulMin &&
			STDMin == b.STDMin &&
			RBFilterLength == b.RBFilterLength;
	}
	bool operator !=(const vsensorfrottaskparams_t& b) const
	{  return !(b == *this); }
};
//--------------------------------------------------------------------------
struct rbitem_t;
// ���������, ���������� ��������� ��������� ������� ������ �������� ��
// ����� �������
typedef struct {
	rbitem_t Frot;
	TStatRot StatRot;
	unsigned int AvgCount;
}vsensorfrot_t;
//--------------------------------------------------------------------------
// ���������, ���������� ���������, ����������� ��� ��������� ���������� �������.
// CMN			 -	���������, ������������ ������������� �����������,
//					�� �������� ����� ����������� ������ ��� ������ 
// OnePartTimeSigLength - ����� ������ ��������������� ������� �������.
// ForgetFsSrc - ����, ���������, ����� �� ������ ��� �������� ������� �������������
//				��������� ���������� ������� � �������������� �������. ���� �����,
//				�� ��� ��������� ��������, �������� ��������� ����� ����� ��������
//				�� ����������� ���������. � ��������� ������ - ���.
//				true - �����,
//				false - ���.
// ����� ������ ��������������� ������� ������� �� ���������
#define DEFAULT_ONEPARTTIMESIGLENGTHDECIM	DEFAULT_SAMPLING_FREQ_ADC

struct decimtimesigtaskparams_t
{
	commontaskparams_t CMN;
	unsigned int OnePartTimeSigLength;
	bool ForgetFsSrc;
	bool operator ==(const decimtimesigtaskparams_t& b) const
	{
		return CMN == b.CMN && 
			OnePartTimeSigLength == b.OnePartTimeSigLength &&
			ForgetFsSrc == b.ForgetFsSrc;
	}
	bool operator !=(const decimtimesigtaskparams_t& b) const
	{  return !(b == *this); }
};
//--------------------------------------------------------------------------
// ���������, ���������� ��������� ��������� ��������� ���������� �������
typedef struct {
	TStatRot StatRot;
	unsigned int AvgCount;
}decimtimesig_t;
//--------------------------------------------------------------------------


#endif
