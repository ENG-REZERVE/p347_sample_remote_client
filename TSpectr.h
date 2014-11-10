//--------------------------------------------------------------------------
// Author: Sidorov E.N.
// Date: 18.02.2011.
// ������, � ������� ������� ������ ��� ������ �� ���������.
//--------------------------------------------------------------------------
#ifndef TSPECTR_H
#define TSPECTR_H
//--------------------------------------------------------------------------
#include <stdlib.h>
#include <memory.h>
#include "DSPEmulDefines.h"
#include "list.h"
//--------------------------------------------------------------------------
// ���� ������� ��������
#define TSC_NONE		0
#define TSC_SIMPLE		1
#define TSC_COMPLEX		2

#define TSC_COUNT		2
//--------------------------------------------------------------------------
class TCustomSpectr;
class TSimpleSpectr;
class TComplexSpectr;
//--------------------------------------------------------------------------
// ������� ����������� � ��������� �������� �� ������ ��������� ��� ���������� ������� ������� ��������
extern void FrequencyCorrection(//������� ���������:
                              TDSPE_Float deltaDB, //�������� ���������� ����������
                                              //��������, ������� ���������
                                              //������������ ����� ���������������
                                              //����� � ������ ���������������
                                              //����������� ������������ ����
                              TDSPE_Float Step,    //����������� ����������� �������
                              //�������� ���������:
                              TDSPE_Float &deltaL, //����������� ��������
                              TDSPE_Float &deltaFc);//��������� ��������
//--------------------------------------------------------------------------
// ������� ������ ���� � �������
extern bool PeakSearch(TDSPE_Float freq,      //������� ������� ��� ������ ����
                     TDSPE_Float dfreq,            //������� ������
                     TCustomSpectr *Spectr,     //��������� �� ������
                     /*TSpectLine* AverangeLine,//��������� �� ������ ������� ����*/
                     TDSPE_Float &F,               //������� ���������� ����
                     TDSPE_Float &L,               //������� ���������� ����
                     int &index);              //������ ���� � ������� �������
//--------------------------------------------------------------------------
// ������� ������ ���� � �������
extern bool PeakSearch(TDSPE_Float freq,      //������� ������� ��� ������ ����
				TDSPE_Float freqmin,            //������� �������� ������
				TDSPE_Float freqmax,            //�������� �������� ������
				TCustomSpectr *Spectr,     //��������� �� ������
				/*TSpectLine* AverangeLine,//��������� �� ������ ������� ����*/
				TDSPE_Float &F,               //������� ���������� ����
				TDSPE_Float &L,               //������� ���������� ����
				TDSPE_Float &deltaFc,		//��������� ��������
				TDSPE_Float &deltaL,			//����������� ��������
				int &index);    //������ ���� � ������� �������

//--------------------------------------------------------------------------
// ��������� �������� ������������� ������������ ����� ������� �������
struct spline_t                       
{                                    
	TDSPE_Float F;					//������� ���������, � ��
	TDSPE_Float L;					//��������� ������������� ���� (�� ������ ����), � dB
	TDSPE_Float Module;				//��������� ���� �� ������ ���� Base, � dB
	TDSPE_Float Base;                 //������� ����, � dB
	//int	Index;
	
	void Clear()
	{
		memset(this, 0x00, sizeof(spline_t));
	}

	bool operator ==(const spline_t& b) const
	{
		return F == b.F && 
			L == b.L &&
			Module == b.Module &&
			Base == b.Base;
	}
	bool operator !=(const spline_t& b) const
	{  return !(b == *this); }
};

//--------------------------------------------------------------------------
// ����� ��� ������ �� ������� ������������� ������������ �������.
//--------------------------------------------------------------------------
class TSpLineList
{
private:
	TList* FList;
public:
	TSpLineList();
	TSpLineList(const TSpLineList& b);
	~TSpLineList();
	void Clear(void);
	int Add(void* AItem);
	void Insert(int AIndex, void* AItem);
	void Delete(int AIndex);
	int Count();
	spline_t* GetSpLine(int AIndex);
	int GetSpLineIndexByAmpMax(TDSPE_Float AMaxModule, TDSPE_Float AFMin, TDSPE_Float AFMax);
	//void operator = (TSpLineList &b);
	TSpLineList& operator = (const TSpLineList &b);
};
//--------------------------------------------------------------------------
// �������, ������������ ��������� ��������� ���������� ����� ��������.
bool SpectrsParamsAreEqual(TCustomSpectr* ASpectr1, TCustomSpectr* ASpectr2);
//--------------------------------------------------------------------------
// ������� ����� ��� ���� ������� ��������. �����������.
class TCustomSpectr
{
public:
	TCustomSpectr();
	virtual ~TCustomSpectr();
	// ��� ������ ������� (������������� � ��������)
	virtual int GetClassCode() = 0;
	// ��� ������� (���, ���������)
	unsigned short GetType(){return FType;};
	void SetType(unsigned short AType);
	// ��� �������
	TLPFilter* GetLPFilter(){return &FLPFilter;};
	// �� ������� (���� � ���� ��� ������� ���������)
	TBPFilter* GetBPFilter(){return &FBPFilter;};
	// ���������� �������� ��� ��������� �������
	TStatRot* GetStatRot(){return &FStatRot;};
	// ���������� ���������� 
	unsigned int GetAvgCount(){return FAvgCount;};
	void SetAvgCount(unsigned int AAvgCount){FAvgCount = AAvgCount;};
	// ��������� �������
	TDSPE_Float GetBegFreq(){return FBegFreq;};
	void SetBegFreq(TDSPE_Float ABegFreq){FBegFreq = ABegFreq;};
	// ��� �� �������
	TDSPE_Float GetFreqStep(){return FFreqStep;};
	void SetFreqStep(TDSPE_Float AFreqStep){FFreqStep = AFreqStep;};
	// ��������� ��������� ������� �� ��������� ������� ��� ��� ���������
	bool FillParams(spectrtaskparams_t ASTP);
	// ��������� ��������� ������� �� ���������� ������� �������
	bool FillParams(TCustomSpectr* ASpectr);
	// ���������� �����
	unsigned int GetLinesCnt(){return FLinesCnt;};
	// ������ �������� � �������� ��������� GetAmpUnit(). ����
	// GetAmpUnit() == TU_AMP_A_M_S2, �� GetAmp() == GetSIAmp().
	TDSPE_Float* GetAmp(){return FAmp;};
	// ������ �������� � �������� ��������� ��������� �� TU_AMP_A_M_S2
	TDSPE_Float* GetSIAmp(){return FSIAmp;};
	// ������� ��������� ���������
	unsigned short GetAmpUnit(){return FAmpUnit;};
	void SetAmpUnit(unsigned short AAmpUnit);
	// ������������� ������ �������� (���� ALinesCnt != FLinesCnt, �� ������� ���������
	// ��� ������� ������ �������� � ���)
	void SetAmp(const TDSPE_Float* AAmp, const unsigned int ALinesCnt, const unsigned short AAmpUnit = TU_AMP_A_DBA);
	// ������� ������ ������� (�.�. ���������, ����������� � ��������
	// �������� � ��� (���� ��� ����))
	virtual void ClearData();
	// ������� ������ ��������� (��������� ��� �������, ��� � ��.)
	virtual void Clear();
	// ���������� ��, ��������� �� �������� ������ ��� ���
	virtual bool IsValid();
	// ���������� ������ ��������� ��������. 
	// AMinAmp - ����������� ��������� ���������;
	// ABase - ������� �����, ������ ������ FLinesCnt. ��� �������,
	// ��� ABase == NULL, ��������������, ��� ������� ����� == 0.
	TSpLineList GetSpLineList(TDSPE_Float AMinAmp = 3.0, TDSPE_Float* ABase = NULL);
	// ���������� ������ ��������� �������� ������������ ����������� �� 
	// ��������� �������� �������� ������� �����.
	// AMinAmp - ����������� ��������� ���������;
	// AAvgWindowLength - ����� ���� ����������.
	TSpLineList GetSpLineListBaseAvg(TDSPE_Float AMinAmp = 3.0, unsigned int AAvgWindowLength = 10);

	void operator = (TCustomSpectr &b);
protected:
	unsigned short FType;           
	TLPFilter FLPFilter;
	TBPFilter FBPFilter;
	TStatRot FStatRot;
	unsigned int FAvgCount;
	TDSPE_Float FBegFreq;
	TDSPE_Float FFreqStep;
	unsigned int FLinesCnt;
	unsigned short FAmpUnit;
	TDSPE_Float* FAmp;
	TDSPE_Float* FSIAmp;
};
//--------------------------------------------------------------------------
// ����� ��� ������� ��������, ���������� ������ ����������� ��������������
class TSimpleSpectr : public TCustomSpectr
{
public:
	TSimpleSpectr();
	virtual ~TSimpleSpectr();
	// ��� ������ �������
	virtual int GetClassCode(){return TSC_SIMPLE;};

	void operator = (TSimpleSpectr &b);
private:
};
//--------------------------------------------------------------------------
// ����� ��� ����������� ��������, ���������� ����������� � ������� 
// ��������������, � ����� ������� �����������
// ��������� ��������� ������� ��������.
class TComplexSpectr : public TCustomSpectr
{
public:
	TComplexSpectr();
	virtual ~TComplexSpectr();
	// ��� ������ �������
	virtual int GetClassCode(){return TSC_COMPLEX;};
	// ������ ���
	TDSPE_Float* GetPhase(){return FPhase;};
	// ������������� ������ ��� (���� ALinesCnt != FLinesCnt, �� ������� ���������
	// ��� ������� ������ �������� � ���)
	void SetPhase(const TDSPE_Float* APhase, const unsigned int ALinesCnt, const unsigned short APhaseUnit = TU_ANGLE_GRAD);
	// ������� ��������� ����
	unsigned short GetPhaseUnit(){return FPhaseUnit;};
	void SetPhaseUnit(unsigned short APhaseUnit);
	// ������� ������ ������� (�.�. ���������, ����������� � ��������
	// �������� � ��� (���� ��� ����))
	virtual void ClearData();
	// ������� ������ ��������� (��������� ��� �������, ��� � ��.)
	virtual void Clear();
	// ��������� ������ ��������� ������� �������� � ������� AHarmonicNumber.
	// AHarmonicNumber == 1 ������������� ��������������� ������� ��������.
	// ���������� 
	//	true - ��� ���������, AVectorHarmonic - ��������� ����������.
	//	false - ������.
	bool CalcHarmonicsFrot(unsigned short AHarmonicNumber, vharmonic_t& AVectorHarmonic);
	// ���������� ��, ��������� �� �������� ������ ��� ���
	virtual bool IsValid();

	void operator = (TComplexSpectr &b);
private:
	TDSPE_Float* FPhase;
	unsigned short FPhaseUnit;
};
//--------------------------------------------------------------------------
#endif