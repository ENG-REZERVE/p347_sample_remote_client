//--------------------------------------------------------------------------
// Author: Sidorov E.N.
// Date: 18.02.2011.
// ������ � ����������� �������� ����������
//--------------------------------------------------------------------------
#ifndef DFILTER_H
#define DFILTER_H
//--------------------------------------------------------------------------
#include <stdlib.h>
#include "DSPEmulDefines.h"
#ifdef __NEW_FILTER_FUNCTION__
#include "TMemoryCircleBuffer.h"
#endif // #ifndef __NEW_FILTER_FUNCTION__
//--------------------------------------------------------------------------
#define ENV_SINCOS_VALUES  4096
//--------------------------------------------------------------------------
// ���� ���� �������� (������� ��������)
#define TF_NONE								0		// 
#define TF_SLIDING_LOWPASS_FIR				1		// ���������� ��� ��� (� ������ ����������� ����������)
#define TF_SLIDING_LOWPASS_HALFBAND_FIR		2		// ���������� ��� ��� (� ������ ����������� ����������) ������������, � ������� �������� �������� ���.���-�� ==0
#define TF_SLIDING_RESAMPLE_FIR				3		// ���������� ��� ������ ��������� (� ������ ����������� ����������)
#define TF_SLIDING_BANDPASS_FIR				4		// ���������� ��� ������ ��������� (� ������ ����������� ����������)
#define TF_SLIDING_SPECPASS_FIR				5		// ���������� ����������� (SP) ��� ������ (� ������ ����������� ����������) 
#define TF_SLIDING_LOWPASS_IIR				6		// ���������� ��� ��� (� ������ ����������� ����������)
#define TF_SLIDING_BANDPASS_IIR				7		// ���������� ��� ������ ��������� (� ������ ����������� ����������)
#define TF_SLIDING_HIGHPASS_IIR				8		// ���������� ��� ��� (� ������ ����������� ����������)
#define TF_SLIDING_HIGHPASS_IIR_FAST		9		// ���������� ��� ��� (� ������ ����������� ����������) � ������� ���������� (������, �������������� ��� ������� �� �������� ��������� Alfa)
#define TF_SLIDING_LOWPASS_IIRBQ			10		// ���������� ������������ (BQ) ��� ��� (� ������ ����������� ����������) TODO
#define TF_SLIDING_BANDPASS_IIRBQ			11		// ���������� ������������ (BQ) ��� ������ ��������� (� ������ ����������� ����������) TODO
#define TF_SLIDING_HIGHPASS_IIRBQ			12		// ���������� ������������ (BQ) ��� ��� (� ������ ����������� ����������)
#define TF_SLIDING_SPECPASS_IIRBQ			13      // ���������� ������������ (BQ) ��� ����������� (SP) ������.
#define TF_SLIDING_INTG_TRP                 14      // ���������� ������������� ������� �������� ������.
#define TF_HEAP_LOWPASS_FIR					15		// �������������� �� ��� ����� ������ ��� ��� (��� ����� ����������� ����������)
#define TF_HEAP_BANDPASS_FIR				16		// �������������� �� ��� ����� ������ ��� ������ ��������� (��� ����� ����������� ����������)
#define TF_HEAP_LOWPASS_IIR					17		// �������������� �� ��� ����� ������ ��� ��� (��� ����� ����������� ����������)
#define TF_HEAP_BANDPASS_IIR				18		// �������������� �� ��� ����� ������ ��� ������ ��������� (��� ����� ����������� ����������)
#define TF_HEAP_LOWPASS_FIR_ZP				19		// �������������� �� ��� ����� ������ ��� ��� � ����������� ��������� ���� ������� (��� ����� ����������� ����������)
#define TF_HEAP_BANDPASS_FIR_ZP				20		// �������������� �� ��� ����� ������ ��� ������ ��������� � ����������� ��������� ���� ������� (��� ����� ����������� ����������)
#define TF_HEAP_LOWPASS_IIR_ZP				21		// �������������� �� ��� ����� ������ ��� ��� � ����������� ��������� ���� ������� (��� ����� ����������� ����������)
#define TF_HEAP_BANDPASS_IIR_ZP				22		// �������������� �� ��� ����� ������ ��� ������ ��������� � ����������� ��������� ���� ������� (��� ����� ����������� ����������)
//--------------------------------------------------------------------------
// ���� ��������
#define TFZP_NONE		0
#define TFZP_LOWPASS	1
#define TFZP_BANDPASS	2
//--------------------------------------------------------------------------
// ������ ���� ��������� ������������ � ����������� ��������� ���� �������
// ������� �����. �����������.
//--------------------------------------------------------------------------
class TFilterZP
{
protected:
	bool FOwnsNextCascade;
	TFilterZP* FNextCascade;
	TFilterZP* FPrevCascade;
	unsigned int FType;
    unsigned int FDecim;
	unsigned int FSizeBuffer;
	unsigned int FOrder;
	TDSPE_LongDouble* FSigBuffer;
	unsigned int FSigBufferLength;
//    procedure Set_Resultat (Value : Extended); virtual;
//    function Get_Resultat : Extended;
    unsigned int GetTotalDecim(unsigned int APrevTotalDecim);
	int GetTotalDecimSigShift(int APrevTotalDecimSigShift, unsigned int APrevTotalDecim);
	unsigned int GetTotalCascadsCount(int APrevTotalCascadsCount);
	bool GetTotalOwnsNextCascade(bool APrevTotalOwnsNextCascade);
public:
	// �����������. AOwnsNextCascade == true ���������
	// �� ��, ��� �� ��������� ������ �������� ���� ������
	// (��������� ������ ����� ������ ������ �������� ��������������)
	TFilterZP(bool AOwnsNextCascade = true);
	// �����������. AOwnsNextCascade == true ����������
	// �� ��, ��� �� ��������� ������ �������� ���� ������
	// (��������� ������ ����� ������ ������ �������� ��������������)
	// APrevCascade - ��������� �� ���������� ������ ����������.
	TFilterZP(TFilterZP* APrevCascade, bool AOwnsNextCascade);
	~TFilterZP();
    // ���� �������� ��������� �������� �������
	bool GetOwnsNextCascade(){return FOwnsNextCascade;};
	void SetOwnsNextCascade(bool AOwnsNextCascade){FOwnsNextCascade = AOwnsNextCascade;};
    // ��������� �� ��������� ������ �������
	TFilterZP* GetNextCascade(){return FNextCascade;};
	void SetNextCascade(TFilterZP* ANextCascade){FNextCascade = ANextCascade;};
	// ��������� �� ���������� ������ �������
	TFilterZP* GetPrevCascade(){return FPrevCascade;};
	void SetPrevCascade(TFilterZP* APrevCascade){FPrevCascade = APrevCascade;};
	// ������� ������� � ��� ���������� �������
	virtual void Clear();
    // ��� ������� TFZP_XXX
    virtual unsigned int GetType();
    // ������ ������
    virtual unsigned int GetSizeBuffer();
    // ������� �������
    virtual unsigned int GetOrder();
    // �������� ��������� �� ������ �������
    virtual unsigned int GetDecim();
	// ������� ���������� � ��������� ������� AInOutputSig ������ AInOutputSigLength.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool FilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength) = 0;
	// ������� ���������� � ��������� ������� AInputSig ������ AInputSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim) = 0;
	// ������� ��������� ������� AInOutputSig ������ AInOutputSigLength.
	// ���� ��������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ���������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	bool DecimateSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength);

	// ���������� �������� (����� ������� ��������)
	// ����������� ���������
	unsigned int GetTotalDecim();
	// ���������� �������� �������� ������� � ��������, �������
	// ����������� ����������. �� ������ �������� ���������� �����������
	// ������� ������ ����� ��������, �������� ������������� ������ ���������
	// ����� ��������.
	int GetTotalDecimSigShift();
	// ���������� ���������� �������� ���������� �������
	unsigned int GetTotalCascadsCount();
	// ���������� ���� ����, ��� ��� ������� ����� ������ ���������
	bool GetTotalOwnsNextCascade();
    // ����� �������
//    property Resultat : Extended read Get_Resultat;
};
//--------------------------------------------------------------------------
// ������ ���� ��������� � ����������� ��������� ���� �������
// ������������ �� 2.
//--------------------------------------------------------------------------
class TLowPassFilterZP : public TFilterZP
{
private:
	TDSPE_LongDouble* GetBArr();
	TDSPE_LongDouble* GetAArr();
	TDSPE_LongDouble* GetZArr();
	// ������������� �������� ����������� ��������� ��� ������������ 
	// ������� ��������. ������ �������� ������ ���� ��������
	// ������. 
	void SetTotalDecimCurrentCascade(unsigned int ATotalDecim);
public:
	// �����������. AOwnsNextCascade == true ���������
	// �� ��, ��� �� ��������� ������ �������� ���� ������
	// (��������� ������ ����� ������ ������ �������� ��������������)
	// ATotalDecim - ��������� ��������� ������� == ������� ������. ����
	// ATotalDecim > 32, �� ��������� ������ ��������, ����� ��������
	// ��������� ���������. ���� ATotalDecim �� ������� ������, �� ���������
	// ��������� �� ����� ���������� � ����� ����� 0.
	TLowPassFilterZP(unsigned int ATotalDecim, bool AOwnsNextCascade = true);
	// �����������. AOwnsNextCascade == true ����������
	// �� ��, ��� �� ��������� ������ �������� ���� ������
	// (��������� ������ ����� ������ ������ �������� ��������������)
	// APrevCascade - ��������� �� ���������� ������ ����������.
	// ATotalDecim - ��������� ��������� ������� == ������� ������. ����
	// ATotalDecim > 32, �� ��������� ������ ��������, ����� ��������
	// ��������� ���������. ���� ATotalDecim �� ������� ������, �� ���������
	// ��������� �� ����� ���������� � ����� ����� 0.
	TLowPassFilterZP(unsigned int ATotalDecim, TFilterZP* APrevCascade = NULL, bool AOwnsNextCascade = true);
	~TLowPassFilterZP();
    // ������ ������
    virtual unsigned int GetSizeBuffer();
    // ������� �������
    virtual unsigned int GetOrder();
	// ������� ���������� � ��������� ������� AInOutputSig ������ AInOutputSigLength.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool FilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength);
	// ������� ���������� � ��������� ������� AInputSig ������ AInputSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim);
	// ������������� �������� (����� ������� ��������)
	// ����������� ���������. ������ �������� ������ ���� ��������
	// ������. �������� ������ ��� ������ ��������
	// �������, �.�. � �������� PrevCascade == NULL � ��� �������,
	// ��� GetTotalOwnsNextCascade() == true.
	void SetTotalDecim(unsigned int ATotalDecim);

};
//--------------------------------------------------------------------------

/*
//--------------------------------------------------------------------------
// ���� �������� ������� �������������
#define TFS_NONE		0
#define TFS_LOWPASS		1
#define TFS_BANDPASS	2
//--------------------------------------------------------------------------
// ������ ������� ������������� ������������.
// ������� �����. �����������.
//--------------------------------------------------------------------------
class TFilterS
{
protected:
	bool FOwnsNextCascade;
	TFilterS* FNextCascade;
	TFilterS* FPrevCascade;
	unsigned int FType;
    unsigned int FDecim;
	TDSPE_LongDouble* FBuffer;
	unsigned int FSizeBuffer;
	unsigned int FOrder;

    TDSPE_LongDouble FResult;
	unsigned int FNumDec;
    bool FUnreadFlag;
    unsigned int FFirstItem;
    unsigned int FCountEmpty;

	void SetSizeBuffer(unsigned int ASizeBuffer);

    unsigned int GetTotalDecim(unsigned int APrevTotalDecim);
	int GetTotalDecimSigShift(int APrevTotalDecimSigShift, unsigned int APrevTotalDecim);
	unsigned int GetTotalCascadsCount(int APrevTotalCascadsCount);
	bool GetTotalOwnsNextCascade(bool APrevTotalOwnsNextCascade);
public:
	// �����������. AOwnsNextCascade == true ���������
	// �� ��, ��� �� ��������� ������ �������� ���� ������
	// (��������� ������ ����� ������ ������ �������� ��������������)
	TFilterS(bool AOwnsNextCascade = true);
	// �����������. AOwnsNextCascade == true ����������
	// �� ��, ��� �� ��������� ������ �������� ���� ������
	// (��������� ������ ����� ������ ������ �������� ��������������)
	// APrevCascade - ��������� �� ���������� ������ ����������.
	TFilterS(TFilterS* APrevCascade, bool AOwnsNextCascade);
	~TFilterS();
    // ���� �������� ��������� �������� �������
	bool GetOwnsNextCascade(){return FOwnsNextCascade;};
	void SetOwnsNextCascade(bool AOwnsNextCascade){FOwnsNextCascade = AOwnsNextCascade;};
    // ��������� �� ��������� ������ �������
	TFilterS* GetNextCascade(){return FNextCascade;};
	void SetNextCascade(TFilterS* ANextCascade){FNextCascade = ANextCascade;};
	// ��������� �� ���������� ������ �������
	TFilterS* GetPrevCascade(){return FPrevCascade;};
	void SetPrevCascade(TFilterS* APrevCascade){FPrevCascade = APrevCascade;};
	// ��������� �� ��������� ������ �������
	TFilterS* GetLastCascade();
	// ������� ������� � ��� ���������� �������
	virtual void Clear();
	// ���������� ������� ������� � ����� ������� ��� ��������� ��������
	virtual void ClearBuffer();
    // ��� ������� TFS_XXX
    virtual unsigned int GetType();
    // ������ ������
    virtual unsigned int GetSizeBuffer();
    // ������� �������
    virtual unsigned int GetOrder();
    // �������� ��������� �� ������ �������
    virtual unsigned int GetDecim();
	// ������������� ��������� ���������� �� ������ �������� �������
    virtual void SetResult(TDSPE_LongDouble Value);
	// ���������� ��������� ���������� �� ������ �������� �������,
	// ������������ ���� ����������� ����� ����, ��� ���������� ������
	// ����������.
    TDSPE_LongDouble GetResult();
	// ���������� ���������� ������������� ��������� ������
	virtual unsigned int GetCountEmptyItem();

	// ���������� �������� (����� ������� ��������)
	// ����������� ���������
	unsigned int GetTotalDecim();
	// ���������� �������� �������� ������� � ��������, �������
	// ����������� ����������. �� ������ �������� ���������� �����������
	// ������� ������ ����� ��������, �������� ������������� ������ ���������
	// ����� ��������.
	int GetTotalDecimSigShift();
	// ���������� ���������� �������� ���������� �������
	unsigned int GetTotalCascadsCount();
	// ���������� ���� ����, ��� ��� ������� ����� ������ ���������
	bool GetTotalOwnsNextCascade();
	// ��������� � ������ ����� ������ �������� �������.
	// ���� ADecimate ��������� �� ��, ���������� �� ���������������
	// ��������� �������. �� ��������� ��������� ����������. 
	// AFillBuffer - ���� ����, ��� ����� ������ ��� ���������� ��������
	// �������� �����. �� ��������� ������� ���������� �����.
	virtual void Push(TDSPE_LongDouble Value, bool AFillBuffer, bool ADecimate) = 0; 
};
//--------------------------------------------------------------------------
// ������ ��� ������� ������������� ������������.
//--------------------------------------------------------------------------
class TLowPassFilterS : public TFilterS
{
private:
	TDSPE_LongDouble* GetBArr();
	TDSPE_LongDouble* GetAArr();
	TDSPE_LongDouble* GetZArr();
	// ������������� �������� ����������� ��������� ��� ������������ 
	// ������� ��������. ������ �������� ������ ���� ��������
	// ������. 
	void SetTotalDecimCurrentCascade(unsigned int ATotalDecim);
public:
	// �����������. AOwnsNextCascade == true ���������
	// �� ��, ��� �� ��������� ������ �������� ���� ������
	// (��������� ������ ����� ������ ������ �������� ��������������)
	// ATotalDecim - ��������� ��������� ������� == ������� ������. ����
	// ATotalDecim > 32, �� ��������� ������ ��������, ����� ��������
	// ��������� ���������. ���� ATotalDecim �� ������� ������, �� ���������
	// ��������� �� ����� ���������� � ����� ����� 0.
	TLowPassFilterS(unsigned int ATotalDecim, bool AOwnsNextCascade = true);
	// �����������. AOwnsNextCascade == true ����������
	// �� ��, ��� �� ��������� ������ �������� ���� ������
	// (��������� ������ ����� ������ ������ �������� ��������������)
	// APrevCascade - ��������� �� ���������� ������ ����������.
	// ATotalDecim - ��������� ��������� ������� == ������� ������. ����
	// ATotalDecim > 32, �� ��������� ������ ��������, ����� ��������
	// ��������� ���������. ���� ATotalDecim �� ������� ������, �� ���������
	// ��������� �� ����� ���������� � ����� ����� 0.
	TLowPassFilterS(unsigned int ATotalDecim, TFilterS* APrevCascade = NULL, bool AOwnsNextCascade = true);
	~TLowPassFilterS();
    // ������� �������
    virtual unsigned int GetOrder();
	// ������������� �������� (����� ������� ��������)
	// ����������� ���������. ������ �������� ������ ���� ��������
	// ������. �������� ������ ��� ������ ��������
	// �������, �.�. � �������� PrevCascade == NULL � ��� �������,
	// ��� GetTotalOwnsNextCascade() == true.
	void SetTotalDecim(unsigned int ATotalDecim);
	// ��������� � ������ ����� ������ �������� �������.
	// ���� ADecimate ��������� �� ��, ���������� �� ���������������
	// ��������� �������. �� ��������� ��������� ����������. 
	// AFillBuffer - ���� ����, ��� ����� ������ ��� ���������� ��������
	// �������� �����. �� ��������� ������� ���������� �����.
	virtual void Push(TDSPE_LongDouble Value, bool AFillBuffer = true, bool ADecimate = true); 
};
//--------------------------------------------------------------------------
*/

//--------------------------------------------------------------------------
// ���������� ���� �������
class TCustomFilter;
class TSlidingFilter;
class TSlidingLPFilterFIR;
class TSlidingLPHBFilterFIR;
class TSlidingLPFilterIIR;
class TSlidingBPFilterFIR;
class TSlidingBPFilterIIR;
//--------------------------------------------------------------------------
// ������� ��� ������ � ���������
//--------------------------------------------------------------------------
// ������� ��� �������� ��� ��������
extern TCustomFilter* CreateLPFilter(unsigned int AType, unsigned int ADecim, bool AOwnsNextCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
extern TCustomFilter* CreateLPFilter(unsigned int AType, unsigned int ATotalDecim, TCustomFilter* APrevCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE); // APrevCascade = NULL

// ������� ��� �������� ��������� �������� ���������
extern TCustomFilter* CreateBPFilter(unsigned int AType, unsigned char AEnvType, bool AOwnsNextCascade, TDSPE_LongDouble AShiftCoefCorrection, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
extern TCustomFilter* CreateBPFilter(unsigned int AType, unsigned char AEnvType, unsigned int ATotalDecim, TCustomFilter* APrevCascade, TDSPE_LongDouble AShiftCoefCorrection, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE); //APrevCascade = NULL
extern TCustomFilter* CreateBPFilter(unsigned int AType, unsigned char AEnvType, unsigned int ATotalDecim, unsigned int APreDecim, TDSPE_LongDouble AShiftCoefCorrection, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);

// ������� ��� �������� �������� �����������������
extern TCustomFilter* CreateRSMPFilter(unsigned int AType, unsigned char AResampleType, bool AOwnsNextCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);

// ������� ��� �������� �� ��������
extern TCustomFilter* CreateHPFilter(unsigned int AType, unsigned char AHPFType, void* AParam1, bool AOwnsNextCascade, unsigned char ATSDTC, unsigned char ATDE);

// ������� ��� �������� ����������� (SP) ��������
extern TCustomFilter* CreateSPFilter(unsigned int AType, unsigned char AHPFType, bool AOwnsNextCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);

// ������� ��� �������� ������������� ��������
extern TCustomFilter* CreateINTGFilter(unsigned int AType, unsigned char AINTGFType, bool AOwnsNextCascade, unsigned char ATSDTC, unsigned char ATDE);

// ������� �������� ����, ��� ������� ����� ���������� ������� (�������� TSlidingLPFilterFIR � TSlidingBPFilterFIR)
bool GetFiltersEqualSubTypes(unsigned int AType1, unsigned int AType2);
//--------------------------------------------------------------------------
// ������� ����� ��� ���� ��������. �����������.
//--------------------------------------------------------------------------
class TCustomFilter
{
protected:
	unsigned char FTDE;			 // ��� ����������, ��� �������� ������������ ������
	unsigned char FTSDTC;		 // ��� ���� ������, � ������� �������� ��������� ������.
	unsigned int FSampleSize;	 // ������ ����������, ���������� ������ ���������� ������� (��. FTSDTC)
	bool FOwnsNextCascade;
	TCustomFilter* FNextCascade;
	TCustomFilter* FPrevCascade;
    unsigned int FDecim; // ����������� ���������
	unsigned int FInterp; // ����������� ������������
	unsigned char FCode;
	char* FSigBuffer;
	unsigned int FSigBufferLength;
	unsigned int FSigBufferRemain;	// ���-�� ���������� �������� �� ���������� ������� (��������� ������ ��� SlidingFilter)
    unsigned int FCoefsCount;		// ���-�� �������������
    unsigned int FRealCoefsCount;		// ���-�� �������������
	unsigned int FAddShift;			
	bool FUsualWidth;
    unsigned int GetTotalDecim(unsigned int APrevTotalDecim);
	unsigned int GetTotalInterp(unsigned int APrevTotalInterp);
	// ���������� ������ �������� ������� ��� ����� ������������ �������,
	// ������������ ��-�� ���������. 
	// ����� ���������, ���� ��������� �������� ��������������� ������ �
	// ������������� ������� ������� (�������).
	int32_t GetTotalDecimSigShift(int APrevTotalDecimSigShift, unsigned int APrevTotalDecim, unsigned int APrevTotalInterp);
	// ���������� ������ �������� ��������� �������� ������� ��� ����� ������������ �������,
	// ������������ ��-�� ����������. 
	// ����� ���������, ���� ��������� �������� ��������������� ������ �
	// ������������� ������� ������� (�������).
	int32_t GetTotalGroupDelaySigShift(int APrevTotalGroupDelaySigShift, unsigned int APrevTotalDecim, unsigned int APrevTotalInterp);
	// ���������� ������ �������� ������� ��� ����� ������������ �������,
	// ������������ ��-�� ���������� ������� ��������. 
	// ����� ���������, ���� ��������� �������� ��������������� ������ �
	// ������������� ������� ������� (�������).
	int32_t GetTotalBufferingSigShift(int APrevTotalBufferingSigShift, unsigned int APrevTotalDecim, unsigned int APrevTotalInterp);
	// ���������� ������ ���������� �������, ����������� �� ������� ����������\
	// ������� �������� ����� ������������ �������. ��������� ������
	// ��� SlidingFilters. ��� HeapFilters ������ = 0.
	// ����� ���������, ���� ��������� �������� ��������������� ������ �
	// ������������� ������� �������� (�������).
	int32_t GetTotalBufferingSigRemain(int APrevTotalBufferingSigRemain, unsigned int APrevTotalDecim, unsigned int APrevTotalInterp);

	unsigned int GetTotalCascadsCount(int APrevTotalCascadsCount);
	bool GetTotalOwnsNextCascade(bool APrevTotalOwnsNextCascade);
	// ���������� �������� ����������� ��������� ��� ������������ 
	// ������� ��������. 
	unsigned int GetTotalDecimCurrentCascade();
	// ���������� �������� ����������� ������������ ��� ������������ 
	// ������� ��������. 
	unsigned int GetTotalInterpCurrentCascade();
	// ������������� �������� ����������� ��������� ��� ������������ 
	// ������� ��������. ������ �������� ������ ���� ��������
	// ������. �������� ������ ���  �������
	// ������������ �������� (���). ��� �������� ��������� ������
	// ���� ���������.
	virtual bool SetTotalDecimCurrentCascade(unsigned int ATotalDecim);
	// ������������� ����������� ��������� ��� �������� �������.
	// ������ �������� ������ ���� ��������
	// ������. �������� ������ ���  �������
	// ������������ �������� (���). ��� �������� ��������� �� ��������.
	virtual bool SetDecim(unsigned int ADecim);
	// ������������� �������� (����� ������� ��������)
	// ����������� ���������. ������ �������� ������ ���� ��������
	// ������. �������� ������ ��� ������ ��������
	// �������, �.�. � �������� PrevCascade == NULL � ��� �������,
	// ��� GetTotalOwnsNextCascade() == true � ����� ������ ��� �������
	// ������������ �������� (���). ��� �������� ��������� �� ��������.
	virtual bool SetTotalDecim(unsigned int ATotalDecim);
	// �������, ������������ ��������� �� ������ B-������������� �������
	virtual void* GetBArrCoefs() = 0;
	// �������, ������������ ��������� �� ������ A-������������� �������
	virtual void* GetAArrCoefs() = 0;
	// �������, ������������ ��������� �� ������ Z-������������� �������
	virtual void* GetZArrCoefs() = 0;
	// �������, ������������ ������ ���������� B-������������� �������
	// ��� ���������� ����������� == GetOrder() - 1, � ����� ���� ������
	// ��������� ���������� GetRealCoefsCount(). ������ �������� ��������
	// �� ������� ����������� (� ������� TSlidingLPHBFilterFIR ��������
	// ������������� ����), ��� ������������ ��������� ������� (��������,
	// � ���������� �������������������� ������� ��� ������������ �������
	// �� ���������� ���������� �����).
	virtual unsigned int GetCoefsCount();
	// �������, ������������ �������� ���������� B-������������� �������.
	virtual unsigned int GetRealCoefsCount() = 0;
	// �������, ������������ ���������� �������� �������� �������, �����������
	// ��� ������������ ������� �� ������.
	// � ������ FIR �������� ��� �� �������. � ������ IIR �������� - ����������������
	// ��������������� �������� ��� ������� �������.
	virtual unsigned int GetStabSigLength();

	// ������������� ������ ������� �������
	virtual void SetSigBufferLength(unsigned int ASigBufferLength);
public:
	// �����������. AOwnsNextCascade == true ���������
	// �� ��, ��� �� ��������� ������ �������� ���� ������
	// (��������� ������ ����� ������ ������ �������� ��������������)
	// AUsualWidth - Is Usual Width - ���� ����, ��� ���������� ������ W ������� �������� ������� (������������),
	//                  �.�. Fstop == 0,5*Fs (Fs - ������� �������������). � ��������� ������ ��� == 0.609375*Fs/Decim.
	TCustomFilter(bool AOwnsNextCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	// �����������. AOwnsNextCascade == true ����������
	// �� ��, ��� �� ��������� ������ �������� ���� ������
	// (��������� ������ ����� ������ ������ �������� ��������������)
	// APrevCascade - ��������� �� ���������� ������ ����������.\
	// AUsualWidth - Is Usual Width - ���� ����, ��� ���������� ������ W ������� �������� ������� (������������),
	//                  �.�. Fstop == 0,5*Fs (Fs - ������� �������������). � ��������� ������ ��� == 0.609375*Fs/Decim.
	TCustomFilter(TCustomFilter* APrevCascade, bool AOwnsNextCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	virtual ~TCustomFilter();
    // ���� �������� ��������� �������� �������
	bool GetOwnsNextCascade(){return FOwnsNextCascade;};
	void SetOwnsNextCascade(bool AOwnsNextCascade){FOwnsNextCascade = AOwnsNextCascade;};
    // ��������� �� ��������� ������ �������
	TCustomFilter* GetNextCascade(){return FNextCascade;};
	void SetNextCascade(TCustomFilter* ANextCascade){FNextCascade = ANextCascade;};
	// ��������� �� ���������� ������ �������
	TCustomFilter* GetPrevCascade(){return FPrevCascade;};
	void SetPrevCascade(TCustomFilter* APrevCascade){FPrevCascade = APrevCascade;};
	// ��������� �� ����� ������ ������ �������
	TCustomFilter* GetFirstCascade();
	// ��������� �� ����� ��������� ������ �������
	TCustomFilter* GetLastCascade();
	// ������� ������� � ��� ���������� �������
	virtual void Clear();
	// ��������� ������������� ������� (����� ������ ��� ���, ���
	// ������� TSlidingFilter)
	virtual void Init();
    // ��� ������� TFZP_XXX
    virtual unsigned int GetType();
    // ����� ������
    virtual unsigned int GetBufferLength();
    // ������� �������
    virtual unsigned int GetOrder();
    // �������� ��������� �� ������ �������
    virtual unsigned int GetDecim();
	// �������� ������������ �� ������ �������
	virtual unsigned int GetInterp();
	// ��� �������
	virtual unsigned int GetCode();
	// �������� ��������������� �������� ����������� �������� ������
	// ��� ������ � ������������� ������ ��� ���������� ������������
	// �������� ������� � 1.
	virtual unsigned int GetAddShift();
	// ��������� �� ��, ��� ��� Decim == 1 ������ ��������,
	// ��� ��������� � ���, SP � �.�.
	virtual bool GetDecimOneMakesChanges(){return false;};
	// ��� ����������, ��� �������� ������������ ������
	unsigned char GetTDE(){return FTDE;};		 
	// ��� ���� ������, � ������� �������� ��������� ������.
	unsigned char GetTSDTC(){return FTSDTC;};		 

	// �������� ��������� �������� ������� � �������� (��������� ������ ��� 
	// ���������� ������������ FIR ��������, ��� ��� ������ � ��� ����� ���� �������� ���,
	// � ������� ��������� �������� � ������ ����������� ��������� � � ��������
	// ����� ((N-1)/2), ��� N - ���������� �������������.
	virtual unsigned int GetGroupDelay(){return 0;};
    // ������������ �������� ��������� ������� ������� ������� ����
    virtual unsigned int GetMaxSingleDecim() = 0;
	// ����, ��������� � ���, ��� ��� ���������� ��������� ����
	// ����� ������. 
	// ��� SlidingFIlter ����� ����������� �� ������, � ��� ��� HeapFilter
	// ����� ���������� ���� ����� ������.
	virtual bool GetNeadAllData() = 0;
	// ����, ��������� � ���, ��� ������ ����� ���� ���������� ����� ��� ����������,
	// �.�. ��� �� ������ ����� ������ ����������� ������� ��� ����, ����� ��������
	// ����� ��������������� �������� �� ������.
	// ���� true - �����.
	// ���� false - �� �����, � ������� ����� ���������� ����� �� ������� �� 
	// ���������� ����������� �������.
	virtual bool GetIsBufferingFilter() = 0;
	// ������� ���������� � ��������� ������� AInOutputSig ������ AInOutputSigLength.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// AUsedSigLength - ����� �������, ������� ���� ������������ ��� ����������.
	// ��� ���� Sliding �������� ���� �������� ����� ����� ���������� ������� �� �����
	// ��������� AInOutputSigLength, ��� ��� ���� �� ������� �������� �������, ������� �� 
	// ������������ ��� ���-�� �� ������, ����� � ������ ������� � ���� ����������� ������.
	// ��� Heap �������� ����������� �����������, � �� �� �����, ����� ������ ��������������
	// ����� ����� ����������. ������� AUsedSigLength ����� ������ �� ���������� ��������,
	// ������� �� ������������ ������ �� ������. ��� ������� ������� ������ � ����������.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool FilterSignal(void* &AInOutputSig, unsigned int &AInOutputSigLength, unsigned int &AUsedSigLength, unsigned char ATSDTC);
	// ������� ���������� � ��������� ������� AInSig ������ AInSigLength.
	// ���� ���������� ��������� �������, �� � AOutSig ������������ ��������
	// ��������������� ������ ������ AOutSigLength. 
	// ���� AOutSig == AInSig, ��� ��������, ��� ��� ���������� �� ����������
	// �������������� �������� �����. � ��������� ������, ����� ��� �������� ������ 
	// AOutSig ������ AOutSigLength ��������� � ���� ������� � ������ ������������� ���������� ��������.
	// ����� ���������� � AOutSigLength ������������ ������ ����������� ������� � 
	// ������ AOutSig.
	// AUsedSigLength - ����� �������, ������� ���� ������������ ��� ����������.
	// ��� ���� Sliding �������� ���� �������� ����� ����� ���������� ������� �� �����
	// ��������� AInSigLength, ��� ��� ���� �� ������� �������� �������, ������� �� 
	// ������������ ��� ���-�� �� ������, ����� � ������ ������� � ���� ����������� ������.
	// ��� Heap �������� ����������� �����������, � �� �� �����, ����� ������ ��������������
	// ����� ����� ����������. ������� AUsedSigLength ����� ������ �� ���������� ��������,
	// ������� �� ������������ ������ �� ������. ��� ������� ������� ������ � ����������.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool FilterSignal(void* AInSig, unsigned int AInSigLength, void* &AOutSig, unsigned int &AOutSigLength, unsigned int &AUsedSigLength, unsigned char ATSDTC);
	// ������� ���������� � ��������� ������� AInputSig ������ AInputSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(void* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC) = 0;

	// ������� ���������� � ��������� ������� AInSig ������ AInSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ���� AOutSig == AInSig, ��� ��������, ��� ��� ���������� �� ����������
	// �������������� �������� �����. � ��������� ������, ����� ��� �������� ������ 
	// AOutSig ������ AOutSigLength ��������� � ���� ������� � ������ ������������� ���������� ��������.
	// ����� ���������� � AOutSigLength ������������ ������ ����������� ������� � 
	// ������ AOutSig.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(void* AInSig, unsigned int AInSigLength, bool ANeedDecim, void* &AOutSig, unsigned int &AOutSigLength, unsigned char ATSDTC) = 0;

	// ������� ��������� ������� AInOutputSig ������ AInOutputSigLength.
	// ���� ��������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ���������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	bool DecimateSignal(void* &AInOutputSig, unsigned int &AInOutputSigLength, unsigned char ATSDTC);

	// ���������� �������� (����� ������� ��������)
	// ����������� ���������
	unsigned int GetTotalDecim();
	// ���������� �������� (����� ������� ��������)
	// ����������� ������������
	unsigned int GetTotalInterp();
	// ���������� �������� �������� ������� � ��������, �������
	// ����������� ����������. �� ������ �������� ���������� �����������
	// ������� ������ ����� ��������, �������� ������������� ������ ���������
	// ����� ��������.
	int32_t GetTotalDecimSigShift();
	// ���������� �������� ������ �������� ��������� �������� ������� � ��������,
	// ������� ����������� �����������. 
	// ����� ���������, ���� ��������� �������� ��������������� ������ �
	// ������������� ������� ������� (�������).
	int32_t GetTotalGroupDelaySigShift();
	// ���������� ������ �������� ������� ��� ����� �������,
	// ������������ ��-�� ���������� ������� ��������. 
	// ����� ���������, ���� ��������� �������� ��������������� ������ �
	// ������������� ������� �������� (�������).
	int32_t GetTotalBufferingSigShift();
	// ���������� ������ ���������� �������, ����������� �� ������� ����������\
	// ������� �������� ����� �������. ��������� ������
	// ��� SlidingFilters. ��� HeapFilters ������ = 0.
	// ����� ���������, ���� ��������� �������� ��������������� ������ �
	// ������������� ������� �������� (�������).
	int32_t GetTotalBufferingSigRemain();
	// ���������� ���������� �������� ���������� �������
	unsigned int GetTotalCascadsCount();
	// ���������� ���� ����, ��� ��� ������� ����� ������ ���������
	bool GetTotalOwnsNextCascade();
};
//--------------------------------------------------------------------------
// ������� ����� ��� ���� ���������� ������������ �������� (� ������ ����������� ����������)
//---------------------------------------------------------------------------
class TSlidingFilter : public TCustomFilter
{
private:
protected:
#ifdef __NEW_FILTER_FUNCTION__
	TMemoryCircleBuffer* FSigBufferMC;
#endif // #ifdef __NEW_FILTER_FUNCTION__

	int	FSigBufferIndex;			// ������� ��������� � ������
	void* FBArrCoefs;		// ������������ �������
    unsigned short FDecimPhase;		// ���� ���������
	// ������������� ����������� ��������� ��� �������� �������.
	// ������ �������� ������ ���� ��������
	// ������. �������� ������ ���  �������
	// ������������ �������� (���). ��� �������� ��������� �� ��������.
	virtual bool SetDecim(unsigned int ADecim);

#ifdef __NEW_FILTER_FUNCTION__
	// ������������� ������ ������� �������
	virtual void SetSigBufferLength(unsigned int ASigBufferLength);
#endif // #ifdef __NEW_FILTER_FUNCTION__
public:
	// ����, ��������� � ���, ��� ��� ���������� ��������� ����
	// ����� ������. 
	// ��� SlidingFIlter ����� ����������� �� ������, � ��� ��� HeapFilter
	// ����� ���������� ���� ����� ������.
	virtual bool GetNeadAllData(){return false;};
	// ����, ��������� � ���, ��� ������ ����� ���� ���������� ����� ��� ����������,
	// �.�. ��� �� ������ ����� ������ ����������� ������� ��� ����, ����� ��������
	// ����� ��������������� �������� �� ������.
	// ���� true - �����.
	// ���� false - �� �����, � ������� ����� ���������� ����� �� ������� �� 
	// ���������� ����������� �������.
	virtual bool GetIsBufferingFilter(){return true;};
	// �����������. AOwnsNextCascade == true ���������
	// �� ��, ��� �� ��������� ������ �������� ���� ������
	// (��������� ������ ����� ������ ������ �������� ��������������)
	// ADecim - ��������� ��������� ������� ������� == ������� ������. ����
	// ADecim > GetMaxSingleDecim(), �� Decim == GetMaxSingleDecim().
	// ���� ATotalDecim �� ������� ������, �� ���������
	// ��������� �� ����� ���������� � ����� ����� 0.
	TSlidingFilter(unsigned int ADecim, bool AOwnsNextCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	// �����������. � ��� �� ��������� ����������� F�OwnsNextCascade == true,
	// ��� ��������� �� ��, ��� �� ��������� ������ �������� ���� ������
	// (��������� ������ ����� ������ ������ �������� ��������������)
	// ATotalDecim - ��������� ��������� ������� == ������� ������. ����
	// ATotalDecim > 32, �� ��������� ������ ��������, ����� ��������
	// ��������� ���������. ���� ATotalDecim �� ������� ������, �� ���������
	// ��������� �� ����� ���������� � ����� ����� 0.
	TSlidingFilter(unsigned int ATotalDecim, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	// �����������. ��� �� ��������� ����������� F�OwnsNextCascade == true,
	// ��� ��������� �� ��, ��� �� ��������� ������ �������� ���� ������
	// (��������� ������ ����� ������ ������ �������� ��������������)
	// APrevCascade - ��������� �� ���������� ������ ����������.
	// ATotalDecim - ��������� ��������� ������� == ������� ������. ����
	// ATotalDecim > 32, �� ��������� ������ ��������, ����� ��������
	// ��������� ���������. ���� ATotalDecim �� ������� ������, �� ���������
	// ��������� �� ����� ���������� � ����� ����� 0.
	TSlidingFilter(unsigned int ATotalDecim, TSlidingFilter* APrevCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	virtual ~TSlidingFilter();


/*	// ������� ������� � ��� ���������� �������
	virtual void Clear();
    // ��� ������� TFZP_XXX
    virtual unsigned int GetType();
    // ������ ������
    virtual unsigned int GetSizeBuffer();
    // ����� ������
    virtual unsigned int GetBufferLength();
    // ������� �������
    virtual unsigned int GetOrder();
    // �������� ��������� �� ������ �������
    virtual unsigned int GetDecim();*/
	// ������� ���������� � ��������� ������� AInOutputSig ������ AInOutputSigLength.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	// virtual bool FilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength) = 0;
	// ������� ���������� � ��������� ������� AInputSig ������ AInputSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	// virtual bool SingleFilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim) = 0;
	virtual void Init();
    virtual uint32_t GetInitLenght();
};
//---------------------------------------------------------------------------
// ���������� ������������ ��� ��� (� ������ ����������� ����������)
// TF_SLIDING_LOWPASS_FIR
//---------------------------------------------------------------------------
class TSlidingLPFilterFIR : public TSlidingFilter
{
private:
protected:
	unsigned int FW;
	bool GetOptimalDecim(unsigned int ATotalDecim, unsigned int &ADecim, unsigned int &AW);
	//unsigned int GetOrderByParams(unsigned char ATSDTC, unsigned char ATDE, unsigned char ADecim, unsigned int AW);
	unsigned int GetMaxWByParams(unsigned char ADecim);
	virtual void* GetBArrCoefs();
	virtual void* GetAArrCoefs();
	virtual void* GetZArrCoefs();
	// �������, ������������ �������� ���������� B-������������� �������.
	virtual unsigned int GetRealCoefsCount();
	// ������������� �������� ����������� ��������� ��� ������������ 
	// ������� ��������. ������ �������� ������ ���� ��������
	// ������. �������� ������ ���  �������
	// ������������ �������� (���). ��� �������� ��������� ������
	// ���� ���������.
	virtual bool SetTotalDecimCurrentCascade(unsigned int ATotalDecim);
public:
	// �����������. AOwnsNextCascade == true ���������
	// �� ��, ��� �� ��������� ������ �������� ���� ������
	// (��������� ������ ����� ������ ������ �������� ��������������)
	// ADecim - ��������� ��������� ������� ������� == ������� ������. ����
	// ADecim > GetMaxSingleDecim(), �� Decim == GetMaxSingleDecim().
	// ���� ATotalDecim �� ������� ������, �� ���������
	// ��������� �� ����� ���������� � ����� ����� 0.
	TSlidingLPFilterFIR(unsigned int ADecim, bool AOwnsNextCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	// �����������. � ��� �� ��������� ����������� F�OwnsNextCascade == true,
	// ��� ��������� �� ��, ��� �� ��������� ������ �������� ���� ������
	// (��������� ������ ����� ������ ������ �������� ��������������)
	// ATotalDecim - ��������� ��������� ������� == ������� ������. ����
	// ATotalDecim > 32, �� ��������� ������ ��������, ����� ��������
	// ��������� ���������. ���� ATotalDecim �� ������� ������, �� ���������
	// ��������� �� ����� ���������� � ����� ����� 0.
	TSlidingLPFilterFIR(unsigned int ATotalDecim, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	// �����������. ��� �� ��������� ����������� F�OwnsNextCascade == true,
	// ��� ��������� �� ��, ��� �� ��������� ������ �������� ���� ������
	// (��������� ������ ����� ������ ������ �������� ��������������)
	// APrevCascade - ��������� �� ���������� ������ ����������.
	// ATotalDecim - ��������� ��������� ������� == ������� ������. ����
	// ATotalDecim > 32, �� ��������� ������ ��������, ����� ��������
	// ��������� ���������. ���� ATotalDecim �� ������� ������, �� ���������
	// ��������� �� ����� ���������� � ����� ����� 0.
	TSlidingLPFilterFIR(unsigned int ATotalDecim, TSlidingFilter* APrevCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	virtual ~TSlidingLPFilterFIR(){};
    // ��� ������� TFZP_XXX
    virtual unsigned int GetType();
    // ������� �������
    virtual unsigned int GetOrder();
	// ������ ���������� ������
	unsigned int GetW();

/*	// ������� ������� � ��� ���������� �������
	virtual void Clear();
    // ��� ������� TFZP_XXX
    virtual unsigned int GetType();
    // ������ ������
    virtual unsigned int GetSizeBuffer();
    // ����� ������
    virtual unsigned int GetBufferLength();
    // ������� �������
    virtual unsigned int GetOrder();
    // �������� ��������� �� ������ �������
    virtual unsigned int GetDecim();*/
	// ��� �������
	virtual unsigned int GetCode();
	// �������� ��������� �������� ������� � �������� (��������� ������ ��� 
	// ���������� ������������ FIR ��������, ��� ��� ������ � ��� ����� ���� �������� ���,
	// � ������� ��������� �������� � ������ ����������� ��������� � � ��������
	// ����� ((N-1)/2), ��� N - ���������� �������������.
	virtual unsigned int GetGroupDelay();
    // ������������ �������� ��������� ������� ������� ������� ����
    virtual unsigned int GetMaxSingleDecim();
	// ������� ���������� � ��������� ������� AInOutputSig ������ AInOutputSigLength.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	//virtual bool FilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength);
	// ������� ���������� � ��������� ������� AInputSig ������ AInputSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(void* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC);

	// ������� ���������� � ��������� ������� AInSig ������ AInSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ���� AOutSig == AInSig, ��� ��������, ��� ��� ���������� �� ����������
	// �������������� �������� �����. � ��������� ������, ����� ��� �������� ������ 
	// AOutSig ������ AOutSigLength ��������� � ���� ������� � ������ ������������� ���������� ��������.
	// ����� ���������� � AOutSigLength ������������ ������ ����������� ������� � 
	// ������ AOutSig.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(void* AInSig, unsigned int AInSigLength, bool ANeedDecim, void* &AOutSig, unsigned int &AOutSigLength, unsigned char ATSDTC);

	// ������� ���������� � ��������� ������� AInputSig ������ AInputSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ��� �������-�������� ������ TDSPE_LongDouble.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim);
	// ������� ���������� � ��������� ������� AInputSig ������ AInputSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ��� �������-�������� ������ int32_t � ���������� ����� �������� ATSDTC.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(int32_t* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC);
};

//---------------------------------------------------------------------------
// ���������� ������������ ��� ��� (� ������ ����������� ����������) ������������, � ������� �������� �������� ���.���-�� ==0
// TF_SLIDING_LOWPASS_HALFBAND_FIR
//---------------------------------------------------------------------------
class TSlidingLPHBFilterFIR : public TSlidingFilter
{
private:
protected:
	virtual void* GetBArrCoefs();
	virtual void* GetAArrCoefs();
	virtual void* GetZArrCoefs();
	// �������, ������������ �������� ���������� B-������������� �������.
	virtual unsigned int GetRealCoefsCount();
	// ������������� �������� ����������� ��������� ��� ������������ 
	// ������� ��������. ������ �������� ������ ���� ��������
	// ������. �������� ������ ���  �������
	// ������������ �������� (���). ��� �������� ��������� ������
	// ���� ���������.
	virtual bool SetTotalDecimCurrentCascade(unsigned int ATotalDecim);
public:
	// �����������. AOwnsNextCascade == true ���������
	// �� ��, ��� �� ��������� ������ �������� ���� ������
	// (��������� ������ ����� ������ ������ �������� ��������������)
	// ADecim - ��������� ��������� ������� ������� == ������� ������. ����
	// ADecim > GetMaxSingleDecim(), �� Decim == GetMaxSingleDecim().
	// ���� ATotalDecim �� ������� ������, �� ���������
	// ��������� �� ����� ���������� � ����� ����� 0.
	TSlidingLPHBFilterFIR(unsigned int ADecim, bool AOwnsNextCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	// �����������. � ��� �� ��������� ����������� F�OwnsNextCascade == true,
	// ��� ��������� �� ��, ��� �� ��������� ������ �������� ���� ������
	// (��������� ������ ����� ������ ������ �������� ��������������)
	// ATotalDecim - ��������� ��������� ������� == ������� ������. ����
	// ATotalDecim > 32, �� ��������� ������ ��������, ����� ��������
	// ��������� ���������. ���� ATotalDecim �� ������� ������, �� ���������
	// ��������� �� ����� ���������� � ����� ����� 0.
	TSlidingLPHBFilterFIR(unsigned int ATotalDecim, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	// �����������. ��� �� ��������� ����������� F�OwnsNextCascade == true,
	// ��� ��������� �� ��, ��� �� ��������� ������ �������� ���� ������
	// (��������� ������ ����� ������ ������ �������� ��������������)
	// APrevCascade - ��������� �� ���������� ������ ����������.
	// ATotalDecim - ��������� ��������� ������� == ������� ������. ����
	// ATotalDecim > 32, �� ��������� ������ ��������, ����� ��������
	// ��������� ���������. ���� ATotalDecim �� ������� ������, �� ���������
	// ��������� �� ����� ���������� � ����� ����� 0.
	TSlidingLPHBFilterFIR(unsigned int ATotalDecim, TSlidingFilter* APrevCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	virtual ~TSlidingLPHBFilterFIR(){};
    // ��� ������� TFZP_XXX
    virtual unsigned int GetType();
    // ������� �������
    virtual unsigned int GetOrder();
	// ��� �������
	virtual unsigned int GetCode();

/*	// ������� ������� � ��� ���������� �������
	virtual void Clear();
    // ��� ������� TFZP_XXX
    virtual unsigned int GetType();
    // ������ ������
    virtual unsigned int GetSizeBuffer();
    // ����� ������
    virtual unsigned int GetBufferLength();
    // ������� �������
    virtual unsigned int GetOrder();
    // �������� ��������� �� ������ �������
    virtual unsigned int GetDecim();*/

	// �������� ��������� �������� ������� � �������� (��������� ������ ��� 
	// ���������� ������������ FIR ��������, ��� ��� ������ � ��� ����� ���� �������� ���,
	// � ������� ��������� �������� � ������ ����������� ��������� � � ��������
	// ����� ((N-1)/2), ��� N - ���������� �������������.
	virtual unsigned int GetGroupDelay();
    // ������������ �������� ��������� ������� ������� ������� ����
    virtual unsigned int GetMaxSingleDecim();
	// ������� ���������� � ��������� ������� AInOutputSig ������ AInOutputSigLength.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	//virtual bool FilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength);
	// ������� ���������� � ��������� ������� AInputSig ������ AInputSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(void* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC);
	// ������� ���������� � ��������� ������� AInSig ������ AInSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ���� AOutSig == AInSig, ��� ��������, ��� ��� ���������� �� ����������
	// �������������� �������� �����. � ��������� ������, ����� ��� �������� ������ 
	// AOutSig ������ AOutSigLength ��������� � ���� ������� � ������ ������������� ���������� ��������.
	// ����� ���������� � AOutSigLength ������������ ������ ����������� ������� � 
	// ������ AOutSig.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(void* AInSig, unsigned int AInSigLength, bool ANeedDecim, void* &AOutSig, unsigned int &AOutSigLength, unsigned char ATSDTC);

	// ������� ���������� � ��������� ������� AInputSig ������ AInputSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ��� �������-�������� ������ TDSPE_LongDouble.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim);
	// ������� ���������� � ��������� ������� AInputSig ������ AInputSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ��� �������-�������� ������ int32_t � ���������� ����� �������� ATSDTC.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(int32_t* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC);

};
//---------------------------------------------------------------------------
// ���������� ������������ ��� ��� (� ������ ����������� ����������)
// TF_SLIDING_LOWPASS_IIR
//---------------------------------------------------------------------------
class TSlidingLPFilterIIR : public TSlidingFilter
{
private:
	int FSigBufferIndexY; // ������� ��������� � ������ �������� ��������
	char* FSigBufferY;	// ����� IIR ������� ��� �������� �������� �������� ��������

protected:
	void* FAArrCoefs;		// ������������ �������
	void* FZArrCoefs;		// ������������ �������
	virtual void* GetBArrCoefs();
	virtual void* GetAArrCoefs();
	virtual void* GetZArrCoefs();
	// �������, ������������ �������� ���������� B-������������� �������.
	virtual unsigned int GetRealCoefsCount();
	// �������, ������������ ���������� �������� �������� �������, �����������
	// ��� ������������ ������� �� ������.
	// � ������ FIR �������� ��� �� �������. � ������ IIR �������� - ����������������
	// ��������������� �������� ��� ������� �������.
	virtual unsigned int GetStabSigLength();
	// ������������� �������� ����������� ��������� ��� ������������ 
	// ������� ��������. ������ �������� ������ ���� ��������
	// ������. �������� ������ ���  �������
	// ������������ �������� (���). ��� �������� ��������� ������
	// ���� ���������.
	virtual bool SetTotalDecimCurrentCascade(unsigned int ATotalDecim);
	// ������������� ����������� ��������� ��� �������� �������.
	// ������ �������� ������ ���� ��������
	// ������. �������� ������ ���  �������
	// ������������ �������� (���). ��� �������� ��������� �� ��������.
	virtual bool SetDecim(unsigned int ADecim);
	// ������������� ������ ������� �������
	virtual void SetSigBufferLength(unsigned int ASigBufferLength);

public:
	// �����������. AOwnsNextCascade == true ���������
	// �� ��, ��� �� ��������� ������ �������� ���� ������
	// (��������� ������ ����� ������ ������ �������� ��������������)
	// ADecim - ��������� ��������� ������� ������� == ������� ������. ����
	// ADecim > GetMaxSingleDecim(), �� Decim == GetMaxSingleDecim().
	// ���� ATotalDecim �� ������� ������, �� ���������
	// ��������� �� ����� ���������� � ����� ����� 0.
	TSlidingLPFilterIIR(unsigned int ADecim, bool AOwnsNextCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	// �����������. � ��� �� ��������� ����������� F�OwnsNextCascade == true,
	// ��� ��������� �� ��, ��� �� ��������� ������ �������� ���� ������
	// (��������� ������ ����� ������ ������ �������� ��������������)
	// ATotalDecim - ��������� ��������� ������� == ������� ������. ����
	// ATotalDecim > 32, �� ��������� ������ ��������, ����� ��������
	// ��������� ���������. ���� ATotalDecim �� ������� ������, �� ���������
	// ��������� �� ����� ���������� � ����� ����� 0.
	TSlidingLPFilterIIR(unsigned int ATotalDecim, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	// �����������. ��� �� ��������� ����������� F�OwnsNextCascade == true,
	// ��� ��������� �� ��, ��� �� ��������� ������ �������� ���� ������
	// (��������� ������ ����� ������ ������ �������� ��������������)
	// APrevCascade - ��������� �� ���������� ������ ����������.
	// ATotalDecim - ��������� ��������� ������� == ������� ������. ����
	// ATotalDecim > 32, �� ��������� ������ ��������, ����� ��������
	// ��������� ���������. ���� ATotalDecim �� ������� ������, �� ���������
	// ��������� �� ����� ���������� � ����� ����� 0.
	TSlidingLPFilterIIR(unsigned int ATotalDecim, TSlidingFilter* APrevCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	virtual ~TSlidingLPFilterIIR(){if (FSigBufferY != NULL){delete[] FSigBufferY; FSigBufferY = NULL;}};
    // ��� ������� TFZP_XXX
    virtual unsigned int GetType();
    // ������� �������
    virtual unsigned int GetOrder();
	// ��� �������
	virtual unsigned int GetCode();
/*	// ������� ������� � ��� ���������� �������
	virtual void Clear();
    // ��� ������� TFZP_XXX
    virtual unsigned int GetType();
    // ������ ������
    virtual unsigned int GetSizeBuffer();
    // ����� ������
    virtual unsigned int GetBufferLength();
    // ������� �������
    virtual unsigned int GetOrder();
    // �������� ��������� �� ������ �������
    virtual unsigned int GetDecim();*/
    // ������������ �������� ��������� ������� ������� ������� ����
    virtual unsigned int GetMaxSingleDecim();
	// ������� ���������� � ��������� ������� AInOutputSig ������ AInOutputSigLength.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	//virtual bool FilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength);
	// ������� ���������� � ��������� ������� AInputSig ������ AInputSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(void* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC);

	// ������� ���������� � ��������� ������� AInSig ������ AInSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ���� AOutSig == AInSig, ��� ��������, ��� ��� ���������� �� ����������
	// �������������� �������� �����. � ��������� ������, ����� ��� �������� ������ 
	// AOutSig ������ AOutSigLength ��������� � ���� ������� � ������ ������������� ���������� ��������.
	// ����� ���������� � AOutSigLength ������������ ������ ����������� ������� � 
	// ������ AOutSig.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(void* AInSig, unsigned int AInSigLength, bool ANeedDecim, void* &AOutSig, unsigned int &AOutSigLength, unsigned char ATSDTC);

	// ������� ���������� � ��������� ������� AInputSig ������ AInputSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ��� �������-�������� ������ TDSPE_LongDouble.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim);
	// ������� ���������� � ��������� ������� AInputSig ������ AInputSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ��� �������-�������� ������ int32_t � ���������� ����� �������� ATSDTC.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(int32_t* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC);
	virtual void Init();

};


//---------------------------------------------------------------------------
// �����, ����������� �������� �������� � ������ ����������� ������� 
// ������������ ��� �����������������.
//---------------------------------------------------------------------------
class TCustomSlidingResampleFilter
{
private:
protected:
	unsigned char FResampleType;
	//virtual void SetResampleType(unsigned char AResampleType) = 0;
public:
	TCustomSlidingResampleFilter(unsigned char AResampleType);
	virtual ~TCustomSlidingResampleFilter();
};

//---------------------------------------------------------------------------
// ���������� ������������ ��� ������ ��������� (� ������ ����������� ����������)
// TF_SLIDING_BANDPASS_FIR
//---------------------------------------------------------------------------
class TSlidingResampleFilterFIR : public TCustomSlidingResampleFilter, public TSlidingLPFilterFIR
{
private:
protected:
	unsigned short FInterpPhase;		// ���� ������������
	virtual void* GetBArrCoefs();
	virtual void* GetAArrCoefs();
	virtual void* GetZArrCoefs();
	// �������, ������������ �������� ���������� B-������������� �������.
	virtual unsigned int GetRealCoefsCount();
	// ������������� ������ ������� �������
	//virtual void SetSigBufferLength(unsigned int ASigBufferLength);
	// ������������� �������� ����������� ��������� ��� ������������ 
	// ������� ��������. ������ �������� ������ ���� ��������
	// ������. �������� ������ ���  �������
	// ������������ �������� (���). ��� �������� ��������� ������
	// ���� ���������.
	//virtual bool SetTotalDecimCurrentCascade(unsigned int ATotalDecim);
	// ������������� ����������� ��������� ��� �������� �������.
	// ������ �������� ������ ���� ��������
	// ������. �������� ������ ���  �������
	// ������������ �������� (���). ��� �������� ��������� �� ��������.
	virtual bool SetDecim(unsigned int ADecim);
	virtual void SetResampleType(unsigned char AResampleType);
public:
	// �����������. AOwnsNextCascade == true ���������
	// �� ��, ��� �� ��������� ������ �������� ���� ������
	// (��������� ������ ����� ������ ������ �������� ��������������)
	// AEnvType - ��� ������� ���������. ������ ����������� ������� ������
	// ��������� � ������������� ����������.
	TSlidingResampleFilterFIR(unsigned char AResampleType, bool AOwnsNextCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	virtual ~TSlidingResampleFilterFIR(){};
    // ��� ������� TFZP_XXX
    virtual unsigned int GetType();
    // ������� �������
    virtual unsigned int GetOrder();
	// ��� �������
	virtual unsigned int GetCode();
    // �������� ��������� �� ������ �������
    virtual unsigned int GetDecim();
	// �������� ������������ �� ������ �������
	virtual unsigned int GetInterp();
	// �������� ��������������� �������� ����������� �������� ������
	// ��� ������ � ������������� ������ ��� ���������� ������������
	// �������� ������� � 1.
	virtual unsigned int GetAddShift();
	// ��������� �� ��, ��� ��� Decim == 1 ������ ��������,
	// ��� ��������� � ���, SP � �.�.
	virtual bool GetDecimOneMakesChanges(){return true;};

/*	// ������� ������� � ��� ���������� �������
	virtual void Clear();
    // ��� ������� TFZP_XXX
    virtual unsigned int GetType();
    // ������ ������
    virtual unsigned int GetSizeBuffer();
    // ����� ������
    virtual unsigned int GetBufferLength();
    // ������� �������
    virtual unsigned int GetOrder();
    // �������� ��������� �� ������ �������
    virtual unsigned int GetDecim();*/
    // ������������ �������� ��������� ������� ������� ������� ����
    //virtual unsigned int GetMaxSingleDecim();
	// ������� ���������� � ��������� ������� AInOutputSig ������ AInOutputSigLength.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	// virtual bool FilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength) = 0;
	// ������� ���������� � ��������� ������� AInputSig ������ AInputSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(void* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC);
	// ������� ���������� � ��������� ������� AInSig ������ AInSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ���� AOutSig == AInSig, ��� ��������, ��� ��� ���������� �� ����������
	// �������������� �������� �����. � ��������� ������, ����� ��� �������� ������ 
	// AOutSig ������ AOutSigLength ��������� � ���� ������� � ������ ������������� ���������� ��������.
	// ����� ���������� � AOutSigLength ������������ ������ ����������� ������� � 
	// ������ AOutSig.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(void* AInSig, unsigned int AInSigLength, bool ANeedDecim, void* &AOutSig, unsigned int &AOutSigLength, unsigned char ATSDTC);

	// ������� ���������� � ��������� ������� AInputSig ������ AInputSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ��� �������-�������� ������ TDSPE_LongDouble.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(TDSPE_LongDouble* AInSig, unsigned int AInSigLength, bool ANeedDecim, TDSPE_LongDouble* &AOutSig, unsigned int &AOutSigLength);
	// ������� ���������� � ��������� ������� AInputSig ������ AInputSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ��� �������-�������� ������ int32_t � ���������� ����� �������� ATSDTC.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(int32_t* AInSig, unsigned int AInSigLength, bool ANeedDecim, int32_t* &AOutSig, unsigned int &AOutSigLength, unsigned char ATSDTC);

	virtual void Init();
    //virtual uint32_t GetInitLenght();
};
//---------------------------------------------------------------------------
// �����, ����������� �������� �������� � ������ ������� ���������
//---------------------------------------------------------------------------
class TCustomSlidingBPFilter
{
private:
protected:
	unsigned int FEnvType;
	char* FSigBufferImag;	// ����� ������� �������� ������ �����
    char* FSinValues;			// �������� ������
    char* FCosValues;		    // �������� ��������
    TDSPE_LongDouble  FShiftCoef;
	// ��������� �������� �������� FShiftCoef.
	// �� ���� �������� ���� ���������� FShiftCoef. ��� �����, ��������,
	// ��� ����, ����� ������, ��� �� ���� ������� ��������� �� �������
	// ������� ������, � ��� �� ����������������������.
	TDSPE_LongDouble  FShiftCoefCorrection; 
	int32_t  FShiftCoefInt32;
    int FSinCosIndex;      // ������� ������ � ������� �������� ������
	virtual void SetEnvType(unsigned int AEnvType) = 0;
	virtual TDSPE_LongDouble GetShiftCoef() = 0;
public:
	TCustomSlidingBPFilter(unsigned int AEnvType);
	virtual ~TCustomSlidingBPFilter();
	TDSPE_LongDouble GetShiftCoefCorrection(){return FShiftCoefCorrection;};
	// ������������� ����� �������� ������������ ��������� �������� �������� FShiftCoef
	virtual void SetShiftCoefCorrection(TDSPE_LongDouble AShiftCoefCorrection) = 0;
};
//---------------------------------------------------------------------------
// ���������� ������������ ��� ������ ��������� (� ������ ����������� ����������)
// TF_SLIDING_BANDPASS_FIR
//---------------------------------------------------------------------------
class TSlidingBPFilterFIR : public TCustomSlidingBPFilter, public TSlidingLPFilterFIR
{
private:
protected:
	virtual void* GetBArrCoefs();
	virtual void* GetAArrCoefs();
	virtual void* GetZArrCoefs();
	// �������, ������������ �������� ���������� B-������������� �������.
	virtual unsigned int GetRealCoefsCount();
	// ������������� ������ ������� �������
	virtual void SetSigBufferLength(unsigned int ASigBufferLength);
	// ������������� �������� ����������� ��������� ��� ������������ 
	// ������� ��������. ������ �������� ������ ���� ��������
	// ������. �������� ������ ���  �������
	// ������������ �������� (���). ��� �������� ��������� ������
	// ���� ���������.
	virtual bool SetTotalDecimCurrentCascade(unsigned int ATotalDecim);
	// ������������� ����������� ��������� ��� �������� �������.
	// ������ �������� ������ ���� ��������
	// ������. �������� ������ ���  �������
	// ������������ �������� (���). ��� �������� ��������� �� ��������.
	virtual bool SetDecim(unsigned int ADecim);
	virtual void SetEnvType(unsigned int AEnvType);
	virtual TDSPE_LongDouble GetShiftCoef();
public:
	// �����������. AOwnsNextCascade == true ���������
	// �� ��, ��� �� ��������� ������ �������� ���� ������
	// (��������� ������ ����� ������ ������ �������� ��������������)
	// AEnvType - ��� ������� ���������. ������ ����������� ������� ������
	// ��������� � ������������� ����������.
	TSlidingBPFilterFIR(unsigned char AEnvType, bool AOwnsNextCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	// �����������. AOwnsNextCascade == true ���������
	// �� ��, ��� �� ��������� ������ �������� ���� ������
	// (��������� ������ ����� ������ ������ �������� ��������������)
	// ATotalDecim - ��������� ��������� ������� == ������� ������. ������ 
	// ���� ATotalDecim > ������� ��������� ������������ ����� ������� ���������, �� ��������� ������ ��������, ����� ��������
	// ��������� ���������. ���� ATotalDecim �� ������� ������, �� ���������
	// ��������� �� ����� ���������� � ����� ����� 0.
	// AEnvType - ��� ������� ���������.
	TSlidingBPFilterFIR(unsigned char AEnvType, unsigned int ATotalDecim, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	// �����������. AOwnsNextCascade == true ���������
	// �� ��, ��� �� ��������� ������ �������� ���� ������
	// (��������� ������ ����� ������ ������ �������� ��������������)
	// ATotalDecim - ��������� ��������� ������� == ������� ������. ������ 
	// ���� ATotalDecim > ������� ��������� ������������ ����� ������� ���������, �� ��������� ������ ��������, ����� ��������
	// ��������� ���������. ���� ATotalDecim �� ������� ������, �� ���������
	// ��������� �� ����� ���������� � ����� ����� 0.
	// AEnvType - ��� ������� ���������.
	// APrevCascade - ��������� �� ���������� ������ ����������.
	TSlidingBPFilterFIR(unsigned char AEnvType, unsigned int ATotalDecim, TSlidingFilter* APrevCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	// �����������. AOwnsNextCascade == true ���������
	// �� ��, ��� �� ��������� ������ �������� ���� ������
	// (��������� ������ ����� ������ ������ �������� ��������������)
	// ATotalDecim - ��������� ��������� ������� == ������� ������. ������ 
	// ���� ATotalDecim > ������� ��������� ������������ ����� ������� ��������� ��������
	// �� APreDecim, �� ��������� ������ ��������, ����� ��������
	// ��������� ���������. ����� �������, ��� ������ ����������� ��� APreDecim > 1
	// ������� ��� ���� ��� ������ (������ ��������) � ����� ���������� == APreDecim. 
	// ���� ATotalDecim �� ������� ������, �� ���������
	// ��������� �� ����� ���������� � ����� ����� 0.
	// AEnvType - ��� ������� ���������.
	TSlidingBPFilterFIR(unsigned char AEnvType, unsigned int ATotalDecim, unsigned int APreDecim, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	virtual ~TSlidingBPFilterFIR(){};
    // ��� ������� TFZP_XXX
    virtual unsigned int GetType();
    // ������� �������
    virtual unsigned int GetOrder();
	// ��� �������
	virtual unsigned int GetCode();
    // �������� ��������� �� ������ �������
    virtual unsigned int GetDecim();
	// �������� ��������������� �������� ����������� �������� ������
	// ��� ������ � ������������� ������ ��� ���������� ������������
	// �������� ������� � 1.
	virtual unsigned int GetAddShift();
	// ������������� ����� �������� ������������ ��������� �������� �������� FShiftCoef
	virtual void SetShiftCoefCorrection(TDSPE_LongDouble AShiftCoefCorrection);
/*	// ������� ������� � ��� ���������� �������
	virtual void Clear();
    // ��� ������� TFZP_XXX
    virtual unsigned int GetType();
    // ������ ������
    virtual unsigned int GetSizeBuffer();
    // ����� ������
    virtual unsigned int GetBufferLength();
    // ������� �������
    virtual unsigned int GetOrder();
    // �������� ��������� �� ������ �������
    virtual unsigned int GetDecim();*/
    // ������������ �������� ��������� ������� ������� ������� ����
    virtual unsigned int GetMaxSingleDecim();
	// ������� ���������� � ��������� ������� AInOutputSig ������ AInOutputSigLength.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	// virtual bool FilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength) = 0;
	// ������� ���������� � ��������� ������� AInputSig ������ AInputSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(void* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC);
	// ������� ���������� � ��������� ������� AInSig ������ AInSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ���� AOutSig == AInSig, ��� ��������, ��� ��� ���������� �� ����������
	// �������������� �������� �����. � ��������� ������, ����� ��� �������� ������ 
	// AOutSig ������ AOutSigLength ��������� � ���� ������� � ������ ������������� ���������� ��������.
	// ����� ���������� � AOutSigLength ������������ ������ ����������� ������� � 
	// ������ AOutSig.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(void* AInSig, unsigned int AInSigLength, bool ANeedDecim, void* &AOutSig, unsigned int &AOutSigLength, unsigned char ATSDTC);

	// ������� ���������� � ��������� ������� AInputSig ������ AInputSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ��� �������-�������� ������ TDSPE_LongDouble.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim);
	// ������� ���������� � ��������� ������� AInputSig ������ AInputSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ��� �������-�������� ������ int32_t � ���������� ����� �������� ATSDTC.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(int32_t* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC);

	virtual void Init();
    //virtual uint32_t GetInitLenght();
};
//---------------------------------------------------------------------------
// ���������� ������������ ��� ������ ��������� (� ������ ����������� ����������)
// TF_SLIDING_BANDPASS_IIR
//---------------------------------------------------------------------------
class TSlidingBPFilterIIR : public TCustomSlidingBPFilter, public TSlidingLPFilterIIR
{
private:
protected:
	virtual void* GetBArrCoefs();
	virtual void* GetAArrCoefs();
	virtual void* GetZArrCoefs();
	// �������, ������������ �������� ���������� B-������������� �������.
	virtual unsigned int GetRealCoefsCount();
	// �������, ������������ ���������� �������� �������� �������, �����������
	// ��� ������������ ������� �� ������.
	// � ������ FIR �������� ��� �� �������. � ������ IIR �������� - ����������������
	// ��������������� �������� ��� ������� �������. 
	// TODO !!! - ���������� ������������ � ������ ������� �������
	virtual unsigned int GetStabSigLength(){return TCustomFilter::GetStabSigLength();};

	// ������������� ������ ������� �������
	virtual void SetSigBufferLength(unsigned int ASigBufferLength);
	// ������������� �������� ����������� ��������� ��� ������������ 
	// ������� ��������. ������ �������� ������ ���� ��������
	// ������. �������� ������ ���  �������
	// ������������ �������� (���). ��� �������� ��������� ������
	// ���� ���������.
	virtual bool SetTotalDecimCurrentCascade(unsigned int ATotalDecim);
	// ������������� ����������� ��������� ��� �������� �������.
	// ������ �������� ������ ���� ��������
	// ������. �������� ������ ���  �������
	// ������������ �������� (���). ��� �������� ��������� �� ��������.
	virtual bool SetDecim(unsigned int ADecim);
	virtual void SetEnvType(unsigned int AEnvType);
	virtual TDSPE_LongDouble GetShiftCoef();
public:
	// �����������. AOwnsNextCascade == true ���������
	// �� ��, ��� �� ��������� ������ �������� ���� ������
	// (��������� ������ ����� ������ ������ �������� ��������������)
	// AEnvType - ��� ������� ���������. ������ ����������� ������� ������
	// ��������� � ������������� ����������.
	TSlidingBPFilterIIR(unsigned char AEnvType, bool AOwnsNextCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	// �����������. AOwnsNextCascade == true ���������
	// �� ��, ��� �� ��������� ������ �������� ���� ������
	// (��������� ������ ����� ������ ������ �������� ��������������)
	// ATotalDecim - ��������� ��������� ������� == ������� ������. ������ 
	// ���� ATotalDecim > ������� ��������� ������������ ����� ������� ���������, �� ��������� ������ ��������, ����� ��������
	// ��������� ���������. ���� ATotalDecim �� ������� ������, �� ���������
	// ��������� �� ����� ���������� � ����� ����� 0.
	// AEnvType - ��� ������� ���������.
	TSlidingBPFilterIIR(unsigned char AEnvType, unsigned int ATotalDecim, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	// �����������. AOwnsNextCascade == true ���������
	// �� ��, ��� �� ��������� ������ �������� ���� ������
	// (��������� ������ ����� ������ ������ �������� ��������������)
	// ATotalDecim - ��������� ��������� ������� == ������� ������. ������ 
	// ���� ATotalDecim > ������� ��������� ������������ ����� ������� ���������, �� ��������� ������ ��������, ����� ��������
	// ��������� ���������. ���� ATotalDecim �� ������� ������, �� ���������
	// ��������� �� ����� ���������� � ����� ����� 0.
	// AEnvType - ��� ������� ���������.
	// APrevCascade - ��������� �� ���������� ������ ����������.
	TSlidingBPFilterIIR(unsigned char AEnvType, unsigned int ATotalDecim, TSlidingFilter* APrevCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	// �����������. AOwnsNextCascade == true ���������
	// �� ��, ��� �� ��������� ������ �������� ���� ������
	// (��������� ������ ����� ������ ������ �������� ��������������)
	// ATotalDecim - ��������� ��������� ������� == ������� ������. ������ 
	// ���� ATotalDecim > ������� ��������� ������������ ����� ������� ��������� ��������
	// �� APreDecim, �� ��������� ������ ��������, ����� ��������
	// ��������� ���������. ����� �������, ��� ������ ����������� ��� APreDecim > 1
	// ������� ��� ���� ��� ������ (������ ��������) � ����� ���������� == APreDecim. 
	// ���� ATotalDecim �� ������� ������, �� ���������
	// ��������� �� ����� ���������� � ����� ����� 0.
	// AEnvType - ��� ������� ���������.
	TSlidingBPFilterIIR(unsigned char AEnvType, unsigned int ATotalDecim, unsigned int APreDecim, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	virtual ~TSlidingBPFilterIIR(){};
    // ��� ������� TFZP_XXX
    virtual unsigned int GetType();
    // ������� �������
    virtual unsigned int GetOrder();
    // �������� ��������� �� ������ �������
    virtual unsigned int GetDecim();
	// ������������� ����� �������� ������������ ��������� �������� �������� FShiftCoef
	virtual void SetShiftCoefCorrection(TDSPE_LongDouble AShiftCoefCorrection);
/*	// ������� ������� � ��� ���������� �������
	virtual void Clear();
    // ��� ������� TFZP_XXX
    virtual unsigned int GetType();
    // ������ ������
    virtual unsigned int GetSizeBuffer();
    // ����� ������
    virtual unsigned int GetBufferLength();
    // ������� �������
    virtual unsigned int GetOrder();
    // �������� ��������� �� ������ �������
    virtual unsigned int GetDecim();*/
    // ������������ �������� ��������� ������� ������� ������� ����
    virtual unsigned int GetMaxSingleDecim();
	// ������� ���������� � ��������� ������� AInOutputSig ������ AInOutputSigLength.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	// virtual bool FilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength) = 0;
	// ������� ���������� � ��������� ������� AInputSig ������ AInputSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(void* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC);
	// ������� ���������� � ��������� ������� AInSig ������ AInSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ���� AOutSig == AInSig, ��� ��������, ��� ��� ���������� �� ����������
	// �������������� �������� �����. � ��������� ������, ����� ��� �������� ������ 
	// AOutSig ������ AOutSigLength ��������� � ���� ������� � ������ ������������� ���������� ��������.
	// ����� ���������� � AOutSigLength ������������ ������ ����������� ������� � 
	// ������ AOutSig.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(void* AInSig, unsigned int AInSigLength, bool ANeedDecim, void* &AOutSig, unsigned int &AOutSigLength, unsigned char ATSDTC);

	virtual void Init();
    //virtual uint32_t GetInitLenght();
};
//---------------------------------------------------------------------------
// �����, ����������� �������� �������� � ������ �� ������� 
//---------------------------------------------------------------------------
class TCustomSlidingHPFilter
{
private:
protected:
	unsigned char FHPFType;
	int FSigBufferIndexY; // ������� ��������� � ������ �������� ��������
	char* FSigBufferY;	// ����� IIR ������� ��� �������� �������� �������� ��������
	//virtual void SetResampleType(unsigned char AResampleType) = 0;
public:
	TCustomSlidingHPFilter(unsigned char AHPFType);
	virtual ~TCustomSlidingHPFilter();
};

//---------------------------------------------------------------------------
// ���������� ��� ��� (� ������ ����������� ����������)
// TF_SLIDING_HIGHPASS_IIR
//---------------------------------------------------------------------------
class TSlidingHPFilterIIR : public TCustomSlidingHPFilter, public TSlidingFilter
{
private:
protected:
	void* FAArrCoefs;		// ������������ �������
	void* FZArrCoefs;		// ������������ �������
	virtual void* GetBArrCoefs();
	virtual void* GetAArrCoefs();
	virtual void* GetZArrCoefs();
	// �������, ������������ �������� ���������� B-������������� �������.
	virtual unsigned int GetRealCoefsCount();
	// �������, ������������ ���������� �������� �������� �������, �����������
	// ��� ������������ ������� �� ������.
	// � ������ FIR �������� ��� �� �������. � ������ IIR �������� - ����������������
	// ��������������� �������� ��� ������� �������.
	virtual unsigned int GetStabSigLength();
	// ������������� ������ ������� �������
	virtual void SetSigBufferLength(unsigned int ASigBufferLength);
	// ������������� �������� ����������� ��������� ��� ������������ 
	// ������� ��������. ������ �������� ������ ���� ��������
	// ������. �������� ������ ���  �������
	// ������������ �������� (���). ��� �������� ��������� ������
	// ���� ���������.
	virtual bool SetTotalDecimCurrentCascade(unsigned int ATotalDecim);
	// ������������� ����������� ��������� ��� �������� �������.
	// ������ �������� ������ ���� ��������
	// ������. �������� ������ ���  �������
	// ������������ �������� (���). ��� �������� ��������� �� ��������.
	virtual bool SetDecim(unsigned int ADecim);
	// ������������� ��� �� �������
	virtual void SetHPFType(unsigned char AHPFType);

public:
	// �����������. 
	// AHPFType - ��� ������� �� �������. 
	TSlidingHPFilterIIR(unsigned char AHPFType, bool AOwnsNextCascade, unsigned char ATSDTC, unsigned char ATDE);
	virtual ~TSlidingHPFilterIIR(){};
    // ��� ������� TFZP_XXX
    virtual unsigned int GetType();
    // ������� �������
    virtual unsigned int GetOrder();
	// ��� �������
	virtual unsigned int GetCode();
	// �������� ��������� �� ������ �������
	virtual unsigned int GetDecim();
	// ��������� �� ��, ��� ��� Decim == 1 ������ ��������,
	// ��� ��������� � ���, SP � �.�.
	virtual bool GetDecimOneMakesChanges(){return true;};
/*	// ������� ������� � ��� ���������� �������
	virtual void Clear();
    // ��� ������� TFZP_XXX
    virtual unsigned int GetType();
    // ������ ������
    virtual unsigned int GetSizeBuffer();
    // ����� ������
    virtual unsigned int GetBufferLength();
    // ������� �������
    virtual unsigned int GetOrder();
    // �������� ��������� �� ������ �������
    virtual unsigned int GetDecim();*/
    // ������������ �������� ��������� ������� ������� ������� ����
    virtual unsigned int GetMaxSingleDecim();
	// ������� ���������� � ��������� ������� AInOutputSig ������ AInOutputSigLength.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	// virtual bool FilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength);
	// ������� ���������� � ��������� ������� AInputSig ������ AInputSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(void* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC);
	// ������� ���������� � ��������� ������� AInSig ������ AInSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ���� AOutSig == AInSig, ��� ��������, ��� ��� ���������� �� ����������
	// �������������� �������� �����. � ��������� ������, ����� ��� �������� ������ 
	// AOutSig ������ AOutSigLength ��������� � ���� ������� � ������ ������������� ���������� ��������.
	// ����� ���������� � AOutSigLength ������������ ������ ����������� ������� � 
	// ������ AOutSig.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(void* AInSig, unsigned int AInSigLength, bool ANeedDecim, void* &AOutSig, unsigned int &AOutSigLength, unsigned char ATSDTC);
	// ������� ���������� � ��������� ������� AInputSig ������ AInputSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ��� �������-�������� ������ TDSPE_LongDouble.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim);
	// ������� ���������� � ��������� ������� AInputSig ������ AInputSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ��� �������-�������� ������ int32_t � ���������� ����� �������� ATSDTC.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(int32_t* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC);
	virtual void Init();
};


//---------------------------------------------------------------------------
// ���������� ��� ��� (� ������ ����������� ����������)
// TF_SLIDING_HIGHPASS_IIRFast
//---------------------------------------------------------------------------
class TSlidingHPFilterIIRFast : public TCustomSlidingHPFilter, public TSlidingFilter
{
private:
	TDSPE_LongDouble FAlfa;
protected:
	virtual void* GetBArrCoefs();
	virtual void* GetAArrCoefs();
	virtual void* GetZArrCoefs();
	// �������, ������������ �������� ���������� B-������������� �������.
	virtual unsigned int GetRealCoefsCount();
	// �������, ������������ ���������� �������� �������� �������, �����������
	// ��� ������������ ������� �� ������.
	// � ������ FIR �������� ��� �� �������. � ������ IIR �������� - ����������������
	// ��������������� �������� ��� ������� �������.
	virtual unsigned int GetStabSigLength();
	// ������������� ������ ������� �������
	virtual void SetSigBufferLength(unsigned int ASigBufferLength);
	// ������������� �������� ����������� ��������� ��� ������������ 
	// ������� ��������. ������ �������� ������ ���� ��������
	// ������. �������� ������ ���  �������
	// ������������ �������� (���). ��� �������� ��������� ������
	// ���� ���������.
	virtual bool SetTotalDecimCurrentCascade(unsigned int ATotalDecim);
	// ������������� ����������� ��������� ��� �������� �������.
	// ������ �������� ������ ���� ��������
	// ������. �������� ������ ���  �������
	// ������������ �������� (���). ��� �������� ��������� �� ��������.
	virtual bool SetDecim(unsigned int ADecim);
	// ������������� ��� �� �������
	virtual void SetHPFType(unsigned char AHPFType);

public:
	// �����������. 
	// AAlfa - �������� ������� 
	TSlidingHPFilterIIRFast(TDSPE_LongDouble AAlfa, bool AOwnsNextCascade, unsigned char ATSDTC, unsigned char ATDE);
	virtual ~TSlidingHPFilterIIRFast(){};
    // ��� ������� TFZP_XXX
    virtual unsigned int GetType();
    // ������� �������
    virtual unsigned int GetOrder();
	// ��� �������
	virtual unsigned int GetCode();
	// �������� ��������� �� ������ �������
	virtual unsigned int GetDecim();
	// ��������� �� ��, ��� ��� Decim == 1 ������ ��������,
	// ��� ��������� � ���, SP � �.�.
	virtual bool GetDecimOneMakesChanges(){return true;};
/*	// ������� ������� � ��� ���������� �������
	virtual void Clear();
    // ��� ������� TFZP_XXX
    virtual unsigned int GetType();
    // ������ ������
    virtual unsigned int GetSizeBuffer();
    // ����� ������
    virtual unsigned int GetBufferLength();
    // ������� �������
    virtual unsigned int GetOrder();
    // �������� ��������� �� ������ �������
    virtual unsigned int GetDecim();*/
    // ������������ �������� ��������� ������� ������� ������� ����
    virtual unsigned int GetMaxSingleDecim();
	// ������� ���������� � ��������� ������� AInOutputSig ������ AInOutputSigLength.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	// virtual bool FilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength);
	// ������� ���������� � ��������� ������� AInputSig ������ AInputSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(void* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC);
	// ������� ���������� � ��������� ������� AInSig ������ AInSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ���� AOutSig == AInSig, ��� ��������, ��� ��� ���������� �� ����������
	// �������������� �������� �����. � ��������� ������, ����� ��� �������� ������ 
	// AOutSig ������ AOutSigLength ��������� � ���� ������� � ������ ������������� ���������� ��������.
	// ����� ���������� � AOutSigLength ������������ ������ ����������� ������� � 
	// ������ AOutSig.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(void* AInSig, unsigned int AInSigLength, bool ANeedDecim, void* &AOutSig, unsigned int &AOutSigLength, unsigned char ATSDTC);
	// ������� ���������� � ��������� ������� AInputSig ������ AInputSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ��� �������-�������� ������ TDSPE_LongDouble.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim);
	// ������� ���������� � ��������� ������� AInputSig ������ AInputSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ��� �������-�������� ������ int32_t � ���������� ����� �������� ATSDTC.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(int32_t* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC);
	virtual void Init();
};


//---------------------------------------------------------------------------
// �����, ����������� �������� �������� � ������ 
// c���������� ������������ �������.
//---------------------------------------------------------------------------
class TCustomSlidingSPFilter
{
private:
protected:
	unsigned char FSPFType;
public:
	TCustomSlidingSPFilter(unsigned char ASPFType);
	virtual ~TCustomSlidingSPFilter();
};

//---------------------------------------------------------------------------
// ���������� (� ������ ����������� ����������) ����������� (SP) ������������ (BQ) ��� ������.
// TF_SLIDING_SPECFILTER_IIRBQ
//---------------------------------------------------------------------------
class TSlidingSPFilterIIRBQ : public TCustomSlidingSPFilter, public TSlidingFilter
{
private:
protected:
	void* FAArrCoefs;		// ������������ �������
	void* FZArrCoefs;		// ������������ �������
	void* FGArrCoefs;		// ������������ �������
	unsigned int FSectionCnt;
	virtual void* GetBArrCoefs();
	virtual void* GetAArrCoefs();
	virtual void* GetZArrCoefs();
	virtual void* GetGArrCoefs();
	// �������, ������������ �������� ���������� B-������������� �������.
	virtual unsigned int GetRealCoefsCount();
	// �������, ������������ ���������� �������� �������� �������, �����������
	// ��� ������������ ������� �� ������.
	// � ������ FIR �������� ��� �� �������. � ������ IIR �������� - ����������������
	// ��������������� �������� ��� ������� �������.
	virtual unsigned int GetStabSigLength();
	// ������������� ������ ������� �������
	//virtual void SetSigBufferLength(unsigned int ASigBufferLength);
	// ������������� �������� ����������� ��������� ��� ������������ 
	// ������� ��������. ������ �������� ������ ���� ��������
	// ������. �������� ������ ���  �������
	// ������������ �������� (���). ��� �������� ��������� ������
	// ���� ���������.
	virtual bool SetTotalDecimCurrentCascade(unsigned int ATotalDecim);
	// ������������� ����������� ��������� ��� �������� �������.
	// ������ �������� ������ ���� ��������
	// ������. �������� ������ ���  �������
	// ������������ �������� (���). ��� �������� ��������� �� ��������.
	virtual bool SetDecim(unsigned int ADecim);
	// ������������� ��� SP �������
	virtual void SetSPFType(unsigned char ASPFType);

public:
	// �����������. 
	// ASPFType - ��� ������� SP �������. 
	TSlidingSPFilterIIRBQ(unsigned char ASPFType, bool AOwnsNextCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	virtual ~TSlidingSPFilterIIRBQ(){};
    // ��� ������� TFZP_XXX
    virtual unsigned int GetType();
    // ������� �������
    virtual unsigned int GetOrder();
	// ��� �������
	virtual unsigned int GetCode();
	// �������� ��������� �� ������ �������
	virtual unsigned int GetDecim();
	// ��������� �� ��, ��� ��� Decim == 1 ������ ��������,
	// ��� ��������� � ���, SP � �.�.
	virtual bool GetDecimOneMakesChanges(){return true;};

/*	// ������� ������� � ��� ���������� �������
	virtual void Clear();
    // ��� ������� TFZP_XXX
    virtual unsigned int GetType();
    // ������ ������
    virtual unsigned int GetSizeBuffer();
    // ����� ������
    virtual unsigned int GetBufferLength();
    // ������� �������
    virtual unsigned int GetOrder();
    // �������� ��������� �� ������ �������
    virtual unsigned int GetDecim();*/
    // ������������ �������� ��������� ������� ������� ������� ����
    virtual unsigned int GetMaxSingleDecim();
	// ������� ���������� � ��������� ������� AInOutputSig ������ AInOutputSigLength.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	// virtual bool FilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength);
	// ������� ���������� � ��������� ������� AInputSig ������ AInputSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(void* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC);
	// ������� ���������� � ��������� ������� AInSig ������ AInSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ���� AOutSig == AInSig, ��� ��������, ��� ��� ���������� �� ����������
	// �������������� �������� �����. � ��������� ������, ����� ��� �������� ������ 
	// AOutSig ������ AOutSigLength ��������� � ���� ������� � ������ ������������� ���������� ��������.
	// ����� ���������� � AOutSigLength ������������ ������ ����������� ������� � 
	// ������ AOutSig.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(void* AInSig, unsigned int AInSigLength, bool ANeedDecim, void* &AOutSig, unsigned int &AOutSigLength, unsigned char ATSDTC);
	// ������� ���������� � ��������� ������� AInputSig ������ AInputSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ��� �������-�������� ������ TDSPE_LongDouble.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim);
	// ������� ���������� � ��������� ������� AInputSig ������ AInputSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ��� �������-�������� ������ int32_t � ���������� ����� �������� ATSDTC.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(int32_t* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC);
};

//---------------------------------------------------------------------------
// ���������� (� ������ ����������� ����������) ����������� (SP) ������������ (BQ) ��� ������.
// TF_SLIDING_HIGHPASS_IIRBQ
//---------------------------------------------------------------------------
class TSlidingHPFilterIIRBQ : public TCustomSlidingHPFilter, public TSlidingFilter
{
private:
protected:
	void* FAArrCoefs;		// ������������ �������
	void* FZArrCoefs;		// ������������ �������
	void* FGArrCoefs;		// ������������ �������
	unsigned int FSectionCnt;
	virtual void* GetBArrCoefs();
	virtual void* GetAArrCoefs();
	virtual void* GetZArrCoefs();
	virtual void* GetGArrCoefs();
	// �������, ������������ �������� ���������� B-������������� �������.
	virtual unsigned int GetRealCoefsCount();
	// �������, ������������ ���������� �������� �������� �������, �����������
	// ��� ������������ ������� �� ������.
	// � ������ FIR �������� ��� �� �������. � ������ IIR �������� - ����������������
	// ��������������� �������� ��� ������� �������.
	virtual unsigned int GetStabSigLength();
	// ������������� ������ ������� �������
	//virtual void SetSigBufferLength(unsigned int ASigBufferLength);
	// ������������� �������� ����������� ��������� ��� ������������ 
	// ������� ��������. ������ �������� ������ ���� ��������
	// ������. �������� ������ ���  �������
	// ������������ �������� (���). ��� �������� ��������� ������
	// ���� ���������.
	virtual bool SetTotalDecimCurrentCascade(unsigned int ATotalDecim);
	// ������������� ����������� ��������� ��� �������� �������.
	// ������ �������� ������ ���� ��������
	// ������. �������� ������ ���  �������
	// ������������ �������� (���). ��� �������� ��������� �� ��������.
	virtual bool SetDecim(unsigned int ADecim);
	// ������������� ��� ���
	virtual void SetHPFType(unsigned char ASPFType);

public:
	// �����������. 
	// AHPFType - ��� ������� ���. 
	TSlidingHPFilterIIRBQ(unsigned char AHPFType, bool AOwnsNextCascade, unsigned char ATSDTC, unsigned char ATDE);
	virtual ~TSlidingHPFilterIIRBQ(){};
    // ��� ������� TFZP_XXX
    virtual unsigned int GetType();
    // ������� �������
    virtual unsigned int GetOrder();
	// ��� �������
	virtual unsigned int GetCode();
	// �������� ��������� �� ������ �������
	virtual unsigned int GetDecim();
	// ��������� �� ��, ��� ��� Decim == 1 ������ ��������,
	// ��� ��������� � ���, SP � �.�.
	virtual bool GetDecimOneMakesChanges(){return true;};

/*	// ������� ������� � ��� ���������� �������
	virtual void Clear();
    // ��� ������� TFZP_XXX
    virtual unsigned int GetType();
    // ������ ������
    virtual unsigned int GetSizeBuffer();
    // ����� ������
    virtual unsigned int GetBufferLength();
    // ������� �������
    virtual unsigned int GetOrder();
    // �������� ��������� �� ������ �������
    virtual unsigned int GetDecim();*/
    // ������������ �������� ��������� ������� ������� ������� ����
    virtual unsigned int GetMaxSingleDecim();
	// ������� ���������� � ��������� ������� AInOutputSig ������ AInOutputSigLength.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	// virtual bool FilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength);
	// ������� ���������� � ��������� ������� AInputSig ������ AInputSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(void* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC);
	// ������� ���������� � ��������� ������� AInSig ������ AInSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ���� AOutSig == AInSig, ��� ��������, ��� ��� ���������� �� ����������
	// �������������� �������� �����. � ��������� ������, ����� ��� �������� ������ 
	// AOutSig ������ AOutSigLength ��������� � ���� ������� � ������ ������������� ���������� ��������.
	// ����� ���������� � AOutSigLength ������������ ������ ����������� ������� � 
	// ������ AOutSig.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(void* AInSig, unsigned int AInSigLength, bool ANeedDecim, void* &AOutSig, unsigned int &AOutSigLength, unsigned char ATSDTC);
	// ������� ���������� � ��������� ������� AInputSig ������ AInputSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ��� �������-�������� ������ TDSPE_LongDouble.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim);
	// ������� ���������� � ��������� ������� AInputSig ������ AInputSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ��� �������-�������� ������ int32_t � ���������� ����� �������� ATSDTC.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(int32_t* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC);
};


//---------------------------------------------------------------------------
// ���������� (� ������ ����������� ����������) ����������� (SP) ������������ (BQ) ��� ������.
// TF_SLIDING_SPECFILTER_FIR
//---------------------------------------------------------------------------
class TSlidingSPFilterFIR : public TCustomSlidingSPFilter, public TSlidingFilter
{
private:
protected:
	virtual void* GetBArrCoefs();
	virtual void* GetAArrCoefs();
	virtual void* GetZArrCoefs();
	// �������, ������������ �������� ���������� B-������������� �������.
	virtual unsigned int GetRealCoefsCount();
	// �������, ������������ ���������� �������� �������� �������, �����������
	// ��� ������������ ������� �� ������.
	// � ������ FIR �������� ��� �� �������. � ������ IIR �������� - ����������������
	// ��������������� �������� ��� ������� �������.
	//virtual unsigned int GetStabSigLength();
	// ������������� ������ ������� �������
	//virtual void SetSigBufferLength(unsigned int ASigBufferLength);
	// ������������� �������� ����������� ��������� ��� ������������ 
	// ������� ��������. ������ �������� ������ ���� ��������
	// ������. �������� ������ ���  �������
	// ������������ �������� (���). ��� �������� ��������� ������
	// ���� ���������.
	virtual bool SetTotalDecimCurrentCascade(unsigned int ATotalDecim);
	// ������������� ����������� ��������� ��� �������� �������.
	// ������ �������� ������ ���� ��������
	// ������. �������� ������ ���  �������
	// ������������ �������� (���). ��� �������� ��������� �� ��������.
	virtual bool SetDecim(unsigned int ADecim);
	// ������������� ��� SP �������
	virtual void SetSPFType(unsigned char ASPFType);

public:
	// �����������. 
	// ASPFType - ��� ������� SP �������. 
	TSlidingSPFilterFIR(unsigned char ASPFType, bool AOwnsNextCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	virtual ~TSlidingSPFilterFIR(){};
    // ��� ������� TFZP_XXX
    virtual unsigned int GetType();
    // ������� �������
    virtual unsigned int GetOrder();
	// ��� �������
	virtual unsigned int GetCode();
	// �������� ��������� �� ������ �������
	virtual unsigned int GetDecim();
	// ��������� �� ��, ��� ��� Decim == 1 ������ ��������,
	// ��� ��������� � ���, SP � �.�.
	virtual bool GetDecimOneMakesChanges(){return true;};

	// �������� ��������������� �������� ����������� �������� ������
	// ��� ������ � ������������� ������ ��� ���������� ������������
	// �������� ������� � 1.
	//virtual unsigned int GetAddShift();

/*	// ������� ������� � ��� ���������� �������
	virtual void Clear();
    // ��� ������� TFZP_XXX
    virtual unsigned int GetType();
    // ������ ������
    virtual unsigned int GetSizeBuffer();
    // ����� ������
    virtual unsigned int GetBufferLength();
    // ������� �������
    virtual unsigned int GetOrder();
    // �������� ��������� �� ������ �������
    virtual unsigned int GetDecim();*/
	// �������� ��������������� �������� ����������� �������� ������
	// ��� ������ � ������������� ������ ��� ���������� ������������
	// �������� ������� � 1.
	virtual unsigned int GetAddShift();
    // ������������ �������� ��������� ������� ������� ������� ����
    virtual unsigned int GetMaxSingleDecim();
	// ������� ���������� � ��������� ������� AInOutputSig ������ AInOutputSigLength.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	// virtual bool FilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength);
	// ������� ���������� � ��������� ������� AInputSig ������ AInputSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(void* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC);
	// ������� ���������� � ��������� ������� AInSig ������ AInSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ���� AOutSig == AInSig, ��� ��������, ��� ��� ���������� �� ����������
	// �������������� �������� �����. � ��������� ������, ����� ��� �������� ������ 
	// AOutSig ������ AOutSigLength ��������� � ���� ������� � ������ ������������� ���������� ��������.
	// ����� ���������� � AOutSigLength ������������ ������ ����������� ������� � 
	// ������ AOutSig.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(void* AInSig, unsigned int AInSigLength, bool ANeedDecim, void* &AOutSig, unsigned int &AOutSigLength, unsigned char ATSDTC);
	// ������� ���������� � ��������� ������� AInputSig ������ AInputSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ��� �������-�������� ������ TDSPE_LongDouble.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim);
	// ������� ���������� � ��������� ������� AInputSig ������ AInputSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ��� �������-�������� ������ int32_t � ���������� ����� �������� ATSDTC.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(int32_t* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC);
};

//---------------------------------------------------------------------------
// �����, ����������� �������� �������� � ������ 
// c���������� �������������� �������.
//---------------------------------------------------------------------------
class TCustomSlidingINTGFilter
{
private:
protected:
	unsigned char FINTGFType;
public:
	TCustomSlidingINTGFilter(unsigned char AINTGFType);
	virtual ~TCustomSlidingINTGFilter();
};

//---------------------------------------------------------------------------
// ���������� ������������� ������ ������.
// TF_SLIDING_INTGFILTER_TRP
//---------------------------------------------------------------------------
class TSlidingINTGFilterTRP : public TCustomSlidingINTGFilter, public TSlidingFilter
{
private:
protected:
	virtual void* GetBArrCoefs();
	virtual void* GetAArrCoefs();
	virtual void* GetZArrCoefs();
	// �������, ������������ �������� ���������� B-������������� �������.
	virtual unsigned int GetRealCoefsCount();
	// �������, ������������ ���������� �������� �������� �������, �����������
	// ��� ������������ ������� �� ������.
	// � ������ FIR �������� ��� �� �������. � ������ IIR �������� - ����������������
	// ��������������� �������� ��� ������� �������.
	//virtual unsigned int GetStabSigLength();
	// ������������� ������ ������� �������
	//virtual void SetSigBufferLength(unsigned int ASigBufferLength);
	// ������������� �������� ����������� ��������� ��� ������������ 
	// ������� ��������. ������ �������� ������ ���� ��������
	// ������. �������� ������ ���  �������
	// ������������ �������� (���). ��� �������� ��������� ������
	// ���� ���������.
	virtual bool SetTotalDecimCurrentCascade(unsigned int ATotalDecim);
	// ������������� ����������� ��������� ��� �������� �������.
	// ������ �������� ������ ���� ��������
	// ������. �������� ������ ���  �������
	// ������������ �������� (���). ��� �������� ��������� �� ��������.
	virtual bool SetDecim(unsigned int ADecim);
	// ������������� ��� SP �������
	virtual void SetINTGFType(unsigned char AINTGFType);

public:
	// �����������. 
	// ASPFType - ��� ������� SP �������. 
	TSlidingINTGFilterTRP(unsigned char ASPFType, bool AOwnsNextCascade, unsigned char ATSDTC, unsigned char ATDE);
	virtual ~TSlidingINTGFilterTRP(){};
    // ��� ������� TFZP_XXX
    virtual unsigned int GetType();
    // ������� �������
    virtual unsigned int GetOrder();
	// ��� �������
	virtual unsigned int GetCode();
	// �������� ��������� �� ������ �������
	virtual unsigned int GetDecim();
	// �������� ��������������� �������� ����������� �������� ������
	// ��� ������ � ������������� ������ ��� ���������� ������������
	// �������� ������� � 1.
	//virtual unsigned int GetAddShift();
	// ��������� �� ��, ��� ��� Decim == 1 ������ ��������,
	// ��� ��������� � ���, SP � �.�.
	virtual bool GetDecimOneMakesChanges(){return true;};

/*	// ������� ������� � ��� ���������� �������
	virtual void Clear();
    // ��� ������� TFZP_XXX
    virtual unsigned int GetType();
    // ������ ������
    virtual unsigned int GetSizeBuffer();
    // ����� ������
    virtual unsigned int GetBufferLength();
    // ������� �������
    virtual unsigned int GetOrder();
    // �������� ��������� �� ������ �������
    virtual unsigned int GetDecim();*/
	// �������� ��������������� �������� ����������� �������� ������
	// ��� ������ � ������������� ������ ��� ���������� ������������
	// �������� ������� � 1.
	//virtual unsigned int GetAddShift();
    // ������������ �������� ��������� ������� ������� ������� ����
    virtual unsigned int GetMaxSingleDecim();
	// ������� ���������� � ��������� ������� AInOutputSig ������ AInOutputSigLength.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	// virtual bool FilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength);
	// ������� ���������� � ��������� ������� AInputSig ������ AInputSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(void* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC);
	// ������� ���������� � ��������� ������� AInSig ������ AInSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ���� AOutSig == AInSig, ��� ��������, ��� ��� ���������� �� ����������
	// �������������� �������� �����. � ��������� ������, ����� ��� �������� ������ 
	// AOutSig ������ AOutSigLength ��������� � ���� ������� � ������ ������������� ���������� ��������.
	// ����� ���������� � AOutSigLength ������������ ������ ����������� ������� � 
	// ������ AOutSig.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(void* AInSig, unsigned int AInSigLength, bool ANeedDecim, void* &AOutSig, unsigned int &AOutSigLength, unsigned char ATSDTC);
	// ������� ���������� � ��������� ������� AInputSig ������ AInputSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ��� �������-�������� ������ TDSPE_LongDouble.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim);
	// ������� ���������� � ��������� ������� AInputSig ������ AInputSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ��� �������-�������� ������ int32_t � ���������� ����� �������� ATSDTC.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(int32_t* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC);
};

//--------------------------------------------------------------------------
// ������� ����� ��� ���� ��������, �������������� �� ��� ����� ������ (��� ����� ����������� ����������)
//---------------------------------------------------------------------------
class THeapFilter : public TCustomFilter
{
private:
protected:

	void* FBArrCoefs;		// ������������ �������
	// ������������� ����������� ��������� ��� �������� �������.
	// ������ �������� ������ ���� ��������
	// ������. �������� ������ ���  �������
	// ������������ �������� (���). ��� �������� ��������� �� ��������.
	virtual bool SetDecim(unsigned int ADecim);
public:
	// ����, ��������� � ���, ��� ��� ���������� ��������� ����
	// ����� ������. 
	// ��� SlidingFIlter ����� ����������� �� ������, � ��� ��� HeapFilter
	// ����� ���������� ���� ����� ������.
	virtual bool GetNeadAllData(){return true;};
	// ����, ��������� � ���, ��� ������ ����� ���� ���������� ����� ��� ����������,
	// �.�. ��� �� ������ ����� ������ ����������� ������� ��� ����, ����� ��������
	// ����� ��������������� �������� �� ������.
	// ���� true - �����.
	// ���� false - �� �����, � ������� ����� ���������� ����� �� ������� �� 
	// ���������� ����������� �������.
	virtual bool GetIsBufferingFilter(){return false;};
	// �����������. AOwnsNextCascade == true ���������
	// �� ��, ��� �� ��������� ������ �������� ���� ������
	// (��������� ������ ����� ������ ������ �������� ��������������)
	// ADecim - ��������� ��������� ������� ������� == ������� ������. ����
	// ADecim > GetMaxSingleDecim(), �� Decim == GetMaxSingleDecim().
	// ���� ATotalDecim �� ������� ������, �� ���������
	// ��������� �� ����� ���������� � ����� ����� 0.
	THeapFilter(unsigned int ADecim, bool AOwnsNextCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	// �����������. � ��� �� ��������� ����������� F�OwnsNextCascade == true,
	// ��� ��������� �� ��, ��� �� ��������� ������ �������� ���� ������
	// (��������� ������ ����� ������ ������ �������� ��������������)
	// ATotalDecim - ��������� ��������� ������� == ������� ������. ����
	// ATotalDecim > 32, �� ��������� ������ ��������, ����� ��������
	// ��������� ���������. ���� ATotalDecim �� ������� ������, �� ���������
	// ��������� �� ����� ���������� � ����� ����� 0.
	THeapFilter(unsigned int ATotalDecim, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	// �����������. ��� �� ��������� ����������� F�OwnsNextCascade == true,
	// ��� ��������� �� ��, ��� �� ��������� ������ �������� ���� ������
	// (��������� ������ ����� ������ ������ �������� ��������������)
	// APrevCascade - ��������� �� ���������� ������ ����������.
	// ATotalDecim - ��������� ��������� ������� == ������� ������. ����
	// ATotalDecim > 32, �� ��������� ������ ��������, ����� ��������
	// ��������� ���������. ���� ATotalDecim �� ������� ������, �� ���������
	// ��������� �� ����� ���������� � ����� ����� 0.
	THeapFilter(unsigned int ATotalDecim, THeapFilter* APrevCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	virtual ~THeapFilter(){};


/*	// ������� ������� � ��� ���������� �������
	virtual void Clear();
    // ��� ������� TFZP_XXX
    virtual unsigned int GetType();
    // ������ ������
    virtual unsigned int GetSizeBuffer();
    // ����� ������
    virtual unsigned int GetBufferLength();
    // ������� �������
    virtual unsigned int GetOrder();
    // �������� ��������� �� ������ �������
    virtual unsigned int GetDecim();*/
	// ������� ���������� � ��������� ������� AInOutputSig ������ AInOutputSigLength.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	// virtual bool FilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength) = 0;
	// ������� ���������� � ��������� ������� AInputSig ������ AInputSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	// virtual bool SingleFilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim) = 0;
	virtual void Init();
	virtual uint32_t GetInitLenght();
};
//---------------------------------------------------------------------------
// �������������� �� ��� ����� ������ ��� ��� (��� ����� ����������� ����������)
// TF_HEAP_LOWPASS_FIR
//---------------------------------------------------------------------------
class THeapLPFilterFIR : public THeapFilter
{
private:
protected:
	virtual void* GetBArrCoefs();
	virtual void* GetAArrCoefs();
	virtual void* GetZArrCoefs();
	// �������, ������������ �������� ���������� B-������������� �������.
	virtual unsigned int GetRealCoefsCount();
	// ������������� �������� ����������� ��������� ��� ������������ 
	// ������� ��������. ������ �������� ������ ���� ��������
	// ������. �������� ������ ���  �������
	// ������������ �������� (���). ��� �������� ��������� ������
	// ���� ���������.
	virtual bool SetTotalDecimCurrentCascade(unsigned int ATotalDecim);
public:
	// �����������. AOwnsNextCascade == true ���������
	// �� ��, ��� �� ��������� ������ �������� ���� ������
	// (��������� ������ ����� ������ ������ �������� ��������������)
	// ADecim - ��������� ��������� ������� ������� == ������� ������. ����
	// ADecim > GetMaxSingleDecim(), �� Decim == GetMaxSingleDecim().
	// ���� ATotalDecim �� ������� ������, �� ���������
	// ��������� �� ����� ���������� � ����� ����� 0.
	THeapLPFilterFIR(unsigned int ADecim, bool AOwnsNextCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	// �����������. � ��� �� ��������� ����������� F�OwnsNextCascade == true,
	// ��� ��������� �� ��, ��� �� ��������� ������ �������� ���� ������
	// (��������� ������ ����� ������ ������ �������� ��������������)
	// ATotalDecim - ��������� ��������� ������� == ������� ������. ����
	// ATotalDecim > 32, �� ��������� ������ ��������, ����� ��������
	// ��������� ���������. ���� ATotalDecim �� ������� ������, �� ���������
	// ��������� �� ����� ���������� � ����� ����� 0.
	THeapLPFilterFIR(unsigned int ATotalDecim, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	// �����������. ��� �� ��������� ����������� F�OwnsNextCascade == true,
	// ��� ��������� �� ��, ��� �� ��������� ������ �������� ���� ������
	// (��������� ������ ����� ������ ������ �������� ��������������)
	// APrevCascade - ��������� �� ���������� ������ ����������.
	// ATotalDecim - ��������� ��������� ������� == ������� ������. ����
	// ATotalDecim > 32, �� ��������� ������ ��������, ����� ��������
	// ��������� ���������. ���� ATotalDecim �� ������� ������, �� ���������
	// ��������� �� ����� ���������� � ����� ����� 0.
	THeapLPFilterFIR(unsigned int ATotalDecim, THeapFilter* APrevCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	virtual ~THeapLPFilterFIR(){};
    // ��� ������� TFZP_XXX
    virtual unsigned int GetType();
    // ������� �������
    virtual unsigned int GetOrder();

/*	// ������� ������� � ��� ���������� �������
	virtual void Clear();
    // ��� ������� TFZP_XXX
    virtual unsigned int GetType();
    // ������ ������
    virtual unsigned int GetSizeBuffer();
    // ����� ������
    virtual unsigned int GetBufferLength();
    // ������� �������
    virtual unsigned int GetOrder();
    // �������� ��������� �� ������ �������
    virtual unsigned int GetDecim();*/
    // ������������ �������� ��������� ������� ������� ������� ����
    virtual unsigned int GetMaxSingleDecim();
	// ������� ���������� � ��������� ������� AInOutputSig ������ AInOutputSigLength.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	//virtual bool FilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength);
	// ������� ���������� � ��������� ������� AInputSig ������ AInputSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(void* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC);
	// ������� ���������� � ��������� ������� AInSig ������ AInSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ���� AOutSig == AInSig, ��� ��������, ��� ��� ���������� �� ����������
	// �������������� �������� �����. � ��������� ������, ����� ��� �������� ������ 
	// AOutSig ������ AOutSigLength ��������� � ���� ������� � ������ ������������� ���������� ��������.
	// ����� ���������� � AOutSigLength ������������ ������ ����������� ������� � 
	// ������ AOutSig.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(void* AInSig, unsigned int AInSigLength, bool ANeedDecim, void* &AOutSig, unsigned int &AOutSigLength, unsigned char ATSDTC);

};
//---------------------------------------------------------------------------
// �������������� �� ��� ����� ������ ��� ��� (��� ����� ����������� ����������)
// TF_HEAP_LOWPASS_IIR
//---------------------------------------------------------------------------
class THeapLPFilterIIR : public THeapFilter
{
private:
protected:
	void* FAArrCoefs;		// ������������ �������
	void* FZArrCoefs;		// ������������ �������
	virtual void* GetBArrCoefs();
	virtual void* GetAArrCoefs();
	virtual void* GetZArrCoefs();
	// �������, ������������ �������� ���������� B-������������� �������.
	virtual unsigned int GetRealCoefsCount();
	// ������������� �������� ����������� ��������� ��� ������������ 
	// ������� ��������. ������ �������� ������ ���� ��������
	// ������. �������� ������ ���  �������
	// ������������ �������� (���). ��� �������� ��������� ������
	// ���� ���������.
	virtual bool SetTotalDecimCurrentCascade(unsigned int ATotalDecim);
	// ������������� ����������� ��������� ��� �������� �������.
	// ������ �������� ������ ���� ��������
	// ������. �������� ������ ���  �������
	// ������������ �������� (���). ��� �������� ��������� �� ��������.
	virtual bool SetDecim(unsigned int ADecim);

public:
	// �����������. AOwnsNextCascade == true ���������
	// �� ��, ��� �� ��������� ������ �������� ���� ������
	// (��������� ������ ����� ������ ������ �������� ��������������)
	// ADecim - ��������� ��������� ������� ������� == ������� ������. ����
	// ADecim > GetMaxSingleDecim(), �� Decim == GetMaxSingleDecim().
	// ���� ATotalDecim �� ������� ������, �� ���������
	// ��������� �� ����� ���������� � ����� ����� 0.
	THeapLPFilterIIR(unsigned int ADecim, bool AOwnsNextCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	// �����������. � ��� �� ��������� ����������� F�OwnsNextCascade == true,
	// ��� ��������� �� ��, ��� �� ��������� ������ �������� ���� ������
	// (��������� ������ ����� ������ ������ �������� ��������������)
	// ATotalDecim - ��������� ��������� ������� == ������� ������. ����
	// ATotalDecim > 32, �� ��������� ������ ��������, ����� ��������
	// ��������� ���������. ���� ATotalDecim �� ������� ������, �� ���������
	// ��������� �� ����� ���������� � ����� ����� 0.
	THeapLPFilterIIR(unsigned int ATotalDecim, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	// �����������. ��� �� ��������� ����������� F�OwnsNextCascade == true,
	// ��� ��������� �� ��, ��� �� ��������� ������ �������� ���� ������
	// (��������� ������ ����� ������ ������ �������� ��������������)
	// APrevCascade - ��������� �� ���������� ������ ����������.
	// ATotalDecim - ��������� ��������� ������� == ������� ������. ����
	// ATotalDecim > 32, �� ��������� ������ ��������, ����� ��������
	// ��������� ���������. ���� ATotalDecim �� ������� ������, �� ���������
	// ��������� �� ����� ���������� � ����� ����� 0.
	THeapLPFilterIIR(unsigned int ATotalDecim, THeapFilter* APrevCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	virtual ~THeapLPFilterIIR(){};
    // ��� ������� TFZP_XXX
    virtual unsigned int GetType();
    // ������� �������
    virtual unsigned int GetOrder();

/*	// ������� ������� � ��� ���������� �������
	virtual void Clear();
    // ��� ������� TFZP_XXX
    virtual unsigned int GetType();
    // ������ ������
    virtual unsigned int GetSizeBuffer();
    // ����� ������
    virtual unsigned int GetBufferLength();
    // ������� �������
    virtual unsigned int GetOrder();
    // �������� ��������� �� ������ �������
    virtual unsigned int GetDecim();*/
    // ������������ �������� ��������� ������� ������� ������� ����
    virtual unsigned int GetMaxSingleDecim();
	// ������� ���������� � ��������� ������� AInOutputSig ������ AInOutputSigLength.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	// virtual bool FilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength);
	// ������� ���������� � ��������� ������� AInputSig ������ AInputSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(void* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC);
	// ������� ���������� � ��������� ������� AInSig ������ AInSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ���� AOutSig == AInSig, ��� ��������, ��� ��� ���������� �� ����������
	// �������������� �������� �����. � ��������� ������, ����� ��� �������� ������ 
	// AOutSig ������ AOutSigLength ��������� � ���� ������� � ������ ������������� ���������� ��������.
	// ����� ���������� � AOutSigLength ������������ ������ ����������� ������� � 
	// ������ AOutSig.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(void* AInSig, unsigned int AInSigLength, bool ANeedDecim, void* &AOutSig, unsigned int &AOutSigLength, unsigned char ATSDTC);

};
//---------------------------------------------------------------------------
// �������������� �� ��� ����� ������ ��� ��� � ����������� ��������� ���� ������� (��� ����� ����������� ����������)
// TF_HEAP_LOWPASS_FIR_ZP
//---------------------------------------------------------------------------
class THeapLPFilterFIR_ZP : public THeapFilter
{
private:
protected:
	virtual void* GetBArrCoefs();
	virtual void* GetAArrCoefs();
	virtual void* GetZArrCoefs();
	// �������, ������������ �������� ���������� B-������������� �������.
	virtual unsigned int GetRealCoefsCount();
	// ������������� �������� ����������� ��������� ��� ������������ 
	// ������� ��������. ������ �������� ������ ���� ��������
	// ������. �������� ������ ���  �������
	// ������������ �������� (���). ��� �������� ��������� ������
	// ���� ���������.
	virtual bool SetTotalDecimCurrentCascade(unsigned int ATotalDecim);
public:
	// �����������. AOwnsNextCascade == true ���������
	// �� ��, ��� �� ��������� ������ �������� ���� ������
	// (��������� ������ ����� ������ ������ �������� ��������������)
	// ADecim - ��������� ��������� ������� ������� == ������� ������. ����
	// ADecim > GetMaxSingleDecim(), �� Decim == GetMaxSingleDecim().
	// ���� ATotalDecim �� ������� ������, �� ���������
	// ��������� �� ����� ���������� � ����� ����� 0.
	THeapLPFilterFIR_ZP(unsigned int ADecim, bool AOwnsNextCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	// �����������. � ��� �� ��������� ����������� F�OwnsNextCascade == true,
	// ��� ��������� �� ��, ��� �� ��������� ������ �������� ���� ������
	// (��������� ������ ����� ������ ������ �������� ��������������)
	// ATotalDecim - ��������� ��������� ������� == ������� ������. ����
	// ATotalDecim > 32, �� ��������� ������ ��������, ����� ��������
	// ��������� ���������. ���� ATotalDecim �� ������� ������, �� ���������
	// ��������� �� ����� ���������� � ����� ����� 0.
	THeapLPFilterFIR_ZP(unsigned int ATotalDecim, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	// �����������. ��� �� ��������� ����������� F�OwnsNextCascade == true,
	// ��� ��������� �� ��, ��� �� ��������� ������ �������� ���� ������
	// (��������� ������ ����� ������ ������ �������� ��������������)
	// APrevCascade - ��������� �� ���������� ������ ����������.
	// ATotalDecim - ��������� ��������� ������� == ������� ������. ����
	// ATotalDecim > 32, �� ��������� ������ ��������, ����� ��������
	// ��������� ���������. ���� ATotalDecim �� ������� ������, �� ���������
	// ��������� �� ����� ���������� � ����� ����� 0.
	THeapLPFilterFIR_ZP(unsigned int ATotalDecim, THeapFilter* APrevCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	virtual ~THeapLPFilterFIR_ZP(){};
    // ��� ������� TFZP_XXX
    virtual unsigned int GetType();
    // ������� �������
    virtual unsigned int GetOrder();

/*	// ������� ������� � ��� ���������� �������
	virtual void Clear();
    // ��� ������� TFZP_XXX
    virtual unsigned int GetType();
    // ������ ������
    virtual unsigned int GetSizeBuffer();
    // ����� ������
    virtual unsigned int GetBufferLength();
    // ������� �������
    virtual unsigned int GetOrder();
    // �������� ��������� �� ������ �������
    virtual unsigned int GetDecim();*/
    // ������������ �������� ��������� ������� ������� ������� ����
    virtual unsigned int GetMaxSingleDecim();
	// ������� ���������� � ��������� ������� AInOutputSig ������ AInOutputSigLength.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	//virtual bool FilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength);
	// ������� ���������� � ��������� ������� AInputSig ������ AInputSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(void* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC);
	// ������� ���������� � ��������� ������� AInSig ������ AInSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ���� AOutSig == AInSig, ��� ��������, ��� ��� ���������� �� ����������
	// �������������� �������� �����. � ��������� ������, ����� ��� �������� ������ 
	// AOutSig ������ AOutSigLength ��������� � ���� ������� � ������ ������������� ���������� ��������.
	// ����� ���������� � AOutSigLength ������������ ������ ����������� ������� � 
	// ������ AOutSig.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(void* AInSig, unsigned int AInSigLength, bool ANeedDecim, void* &AOutSig, unsigned int &AOutSigLength, unsigned char ATSDTC);

};
//---------------------------------------------------------------------------
// �������������� �� ��� ����� ������ ��� ��� � ����������� ��������� ���� ������� (��� ����� ����������� ����������)
// TF_HEAP_LOWPASS_IIR_ZP
//---------------------------------------------------------------------------
class THeapLPFilterIIR_ZP : public THeapFilter
{
private:
protected:
	void* FAArrCoefs;		// ������������ �������
	void* FZArrCoefs;		// ������������ �������
	virtual void* GetBArrCoefs();
	virtual void* GetAArrCoefs();
	virtual void* GetZArrCoefs();
	// �������, ������������ �������� ���������� B-������������� �������.
	virtual unsigned int GetRealCoefsCount();
	// ������������� �������� ����������� ��������� ��� ������������ 
	// ������� ��������. ������ �������� ������ ���� ��������
	// ������. �������� ������ ���  �������
	// ������������ �������� (���). ��� �������� ��������� ������
	// ���� ���������.
	virtual bool SetTotalDecimCurrentCascade(unsigned int ATotalDecim);
	// ������������� ����������� ��������� ��� �������� �������.
	// ������ �������� ������ ���� ��������
	// ������. �������� ������ ���  �������
	// ������������ �������� (���). ��� �������� ��������� �� ��������.
	virtual bool SetDecim(unsigned int ADecim);

public:
	// �����������. AOwnsNextCascade == true ���������
	// �� ��, ��� �� ��������� ������ �������� ���� ������
	// (��������� ������ ����� ������ ������ �������� ��������������)
	// ADecim - ��������� ��������� ������� ������� == ������� ������. ����
	// ADecim > GetMaxSingleDecim(), �� Decim == GetMaxSingleDecim().
	// ���� ATotalDecim �� ������� ������, �� ���������
	// ��������� �� ����� ���������� � ����� ����� 0.
	THeapLPFilterIIR_ZP(unsigned int ADecim, bool AOwnsNextCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	// �����������. � ��� �� ��������� ����������� F�OwnsNextCascade == true,
	// ��� ��������� �� ��, ��� �� ��������� ������ �������� ���� ������
	// (��������� ������ ����� ������ ������ �������� ��������������)
	// ATotalDecim - ��������� ��������� ������� == ������� ������. ����
	// ATotalDecim > 32, �� ��������� ������ ��������, ����� ��������
	// ��������� ���������. ���� ATotalDecim �� ������� ������, �� ���������
	// ��������� �� ����� ���������� � ����� ����� 0.
	THeapLPFilterIIR_ZP(unsigned int ATotalDecim, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	// �����������. ��� �� ��������� ����������� F�OwnsNextCascade == true,
	// ��� ��������� �� ��, ��� �� ��������� ������ �������� ���� ������
	// (��������� ������ ����� ������ ������ �������� ��������������)
	// APrevCascade - ��������� �� ���������� ������ ����������.
	// ATotalDecim - ��������� ��������� ������� == ������� ������. ����
	// ATotalDecim > 32, �� ��������� ������ ��������, ����� ��������
	// ��������� ���������. ���� ATotalDecim �� ������� ������, �� ���������
	// ��������� �� ����� ���������� � ����� ����� 0.
	THeapLPFilterIIR_ZP(unsigned int ATotalDecim, THeapFilter* APrevCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	virtual ~THeapLPFilterIIR_ZP(){};
    // ��� ������� TFZP_XXX
    virtual unsigned int GetType();
    // ������� �������
    virtual unsigned int GetOrder();

/*	// ������� ������� � ��� ���������� �������
	virtual void Clear();
    // ��� ������� TFZP_XXX
    virtual unsigned int GetType();
    // ������ ������
    virtual unsigned int GetSizeBuffer();
    // ����� ������
    virtual unsigned int GetBufferLength();
    // ������� �������
    virtual unsigned int GetOrder();
    // �������� ��������� �� ������ �������
    virtual unsigned int GetDecim();*/
    // ������������ �������� ��������� ������� ������� ������� ����
    virtual unsigned int GetMaxSingleDecim();
	// ������� ���������� � ��������� ������� AInOutputSig ������ AInOutputSigLength.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	// virtual bool FilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength);
	// ������� ���������� � ��������� ������� AInputSig ������ AInputSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ���� ���������� ��������� �������, �� � AInOutputSig ������������ ��������
	// ��������������� ������ ������ AOutputSigLength.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(void* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC);
	// ������� ���������� � ��������� ������� AInSig ������ AInSigLength ������
	// ������ �������� ��� ������������� ��������� �������� � ��������� ����, ����� ��
	// � ����������� ��������� (������������ ��������) �������.
	// ���� AOutSig == AInSig, ��� ��������, ��� ��� ���������� �� ����������
	// �������������� �������� �����. � ��������� ������, ����� ��� �������� ������ 
	// AOutSig ������ AOutSigLength ��������� � ���� ������� � ������ ������������� ���������� ��������.
	// ����� ���������� � AOutSigLength ������������ ������ ����������� ������� � 
	// ������ AOutSig.
	// ��������� �� ������:
	// true  - ��������� ��������� ���������.
	// false - ��������� ���������.
	virtual bool SingleFilterSignal(void* AInSig, unsigned int AInSigLength, bool ANeedDecim, void* &AOutSig, unsigned int &AOutSigLength, unsigned char ATSDTC);

};

//---------------------------------------------------------------------------






// �������, ������� �������� �������� ������������ filtfilt(B,A,X)
extern void FiltFilt( const TDSPE_LongDouble *B, const int BLength, const TDSPE_LongDouble* A,
              const int ALength, TDSPE_LongDouble* InOutSignal, int InOutSignalLength);

// �������, ������� �������� �������� ������������ filtfilt(B,A,X)
extern bool FiltFilt_Z( const TDSPE_LongDouble* B, const int BLength,
                 const TDSPE_LongDouble* A, const int ALength,
                 const TDSPE_LongDouble* Z, const int ZLength,
                 TDSPE_LongDouble* InOutSignal, int InOutSignalLength);

#endif