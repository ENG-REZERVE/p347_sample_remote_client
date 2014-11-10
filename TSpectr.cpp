#include "TSpectr.h"

#include <memory.h>
#include <stdlib.h>



//--------------------------------------------------------------------------
/*амплитудная и частотная поправки на эффект частокола при применении весовой функции Ханнинга*/
void FrequencyCorrection(//входные параметры:
                              TDSPE_Float deltaDB, //разность наибольших дискретных
                                              //значений, которые принимают
                                              //спектральные линии непосредственно
                                              //снизу и сверху соответсвующего
                                              //учитываемой составляющей пика
                              TDSPE_Float Step,    //разрешающая способность спектра
                              //выходные параметры:
                              TDSPE_Float &deltaL, //амплитудная поправка
                              TDSPE_Float &deltaFc)//частотная поправка
{
 TDSPE_Float c_deltaL = 25.26315789473684;
 TDSPE_Float c_k = -0.08333333333333;
 TDSPE_Float c_q = 0.5;
 if((deltaDB < 0)&&(deltaDB > 6))
 {//вне допустимого диапазона
  deltaL =0;
  deltaFc = 0;
  return;
 }
 TDSPE_Float t01 = (6-deltaDB);
 deltaL = t01*t01/c_deltaL;
 deltaFc= (c_k*deltaDB + c_q)*Step;
 if (deltaL < 1e-99) deltaL = 0;
 if (deltaFc < 1e-99) deltaFc = 0;
}

//--------------------------------------------------------------------------
/*-Поиск пика-*/
bool PeakSearch(TDSPE_Float freq,      //базовая частота для поиска пика
                     TDSPE_Float dfreq,            //коридор поиска
                     TCustomSpectr *Spectr,     //указатель на спектр
                     /*TSpectLine* AverangeLine,//указатель на массив уровней фона*/
                     TDSPE_Float &F,               //частота найденного пика
                     TDSPE_Float &L,               //уровень найденного пика
					 TDSPE_Float &deltaFc,		//частотная поправка
					 TDSPE_Float &deltaL,			//амплитудная поправка
                     int &index)              //индекс пика в массиве спектра
{
 F = 0;
 L = 0;
 deltaFc = 0;
 deltaL = 0;
 index = -1;
	if (!Spectr->IsValid()) return false;
 int i, i1, ind1, ind2;
 TDSPE_Float s1, s2, s3, max, tr;
 TDSPE_Float dL, dFn; /*амплитудная и частотная поправки*/
 //dL = 0; L = 0; dFn = 0; F = 0; i1 = 0;
 TDSPE_Float TmpFreqStep = Spectr->GetFreqStep();
 TDSPE_Float* TmpAmp = Spectr->GetAmp();
 ind1 = (int)((freq - dfreq)/TmpFreqStep);
 ind2 = (int)((freq + dfreq)/TmpFreqStep)+1;
 if (ind1 < 0) 
	 ind1 = 0;
 if (ind2 >= Spectr->GetLinesCnt() - 1) 
	 ind2 = Spectr->GetLinesCnt() - 1;
 if ((ind1 >= Spectr->GetLinesCnt() - 1) ||
	 ind1 > ind2 || (ind2 - ind1) < 2)
	 return false;
 if (ind1 == ind2) i1= ind1;
 else
 {
  max = -1.0;
  i = ind1;
  do
  {
	  if(TmpAmp[i] > max)
   {
    max = TmpAmp[i];
    i1 = i;
   }
  }while(++i < ind2);
 }
 /*проверяем, является ли <max> точкой перегиба*/
 s1 = TmpAmp[i1]; /*ценральная*/
 s2 = TmpAmp[i1-1]; /*левая*/
 s3 = TmpAmp[i1+1]; /*правая*/
/* if(s1 - 4 - AverangeLine[i1] <= 0)
 {
  //найденная максимальная величина меньше уровня средней линии + trend
  return false;
 }*/
 dL = 0; dFn = 0;
 if ((s1==s2)&&(s1>s3))
 {
  /*равны центральная и левая и правая меньше их*/
  if (TmpAmp[i1-2] < s1)
  {
   /*составляющая левее левой меньше ее, т.е. пик есть*/
   dFn = - TmpFreqStep / 2; /*частотная поправка, половинка шага*/
   dL = 1.5; /*амплитудная поправка*/
   return true;
  }
  else
  {
   /*иначе это не пик*/
   return false;
  }
 }
 else
 {
  if((s1==s3)&&(s1>s2))
  {
   /*равны центральная и правая и левая меньше их*/
   if(TmpAmp[i1+2] < s1)
   {
    /*составляющая правее правой меньше ее, т.е. пик есть*/
    dFn = TmpFreqStep / 2; /*половинка шага, частотная поправка*/
    dL = 1.5; /*амплитудная поправка*/
    return true;
   }
   else
   {
    /*иначе это не пик*/
    return false;
   }
  }
  else
  {
   if((s1>s2)&&(s1>s3))
   {
    /*левая и правая меньше центральной*/
    if (s2==s3)
    {
     /*левая и правая равны, т.е. поправок не надо -> рез-т центральная*/
     dFn = 0; dL = 0;
    }
    else
    {
     if((s1-s2) < (s1-s3))
     {
      FrequencyCorrection(s1-s2,TmpFreqStep,dL,dFn);
      dFn = -dFn;
     }
     else
      FrequencyCorrection(s1-s3,TmpFreqStep,dL,dFn);
    }
   }
  }
 }
 deltaFc = dFn;
 deltaL = dL;
 F = i1 * TmpFreqStep + dFn;
 L = s1 + dL;
 index = i1;
 return true;
}

//--------------------------------------------------------------------------
/*-Поиск пика-*/
bool PeakSearch(TDSPE_Float freq,      //базовая частота для поиска пика
                TDSPE_Float freqmin,            //минимум коридора поиска
				TDSPE_Float freqmax,            //максимум коридора поиска
                TCustomSpectr *Spectr,     //указатель на спектр
                /*TSpectLine* AverangeLine,//указатель на массив уровней фона*/
                TDSPE_Float &F,               //частота найденного пика
                TDSPE_Float &L,               //уровень найденного пика
				TDSPE_Float &deltaFc,		//частотная поправка
				TDSPE_Float &deltaL,			//амплитудная поправка
                int &index)    //индекс пика в массиве спектра
{
 F = 0;
 L = 0;
 deltaFc = 0;
 deltaL = 0;
 index = -1;
	if (!Spectr->IsValid()) return false;
 int i, i1, ind1, ind2;
 TDSPE_Float s1, s2, s3, max, tr;
 TDSPE_Float dL, dFn; /*амплитудная и частотная поправки*/
 //dL = 0; L = 0; dFn = 0; F = 0; i1 = 0;
 TDSPE_Float TmpFreqStep = Spectr->GetFreqStep();
 TDSPE_Float* TmpAmp = Spectr->GetAmp();
 ind1 = (int)(freqmin/TmpFreqStep);
 ind2 = (int)(freqmax/TmpFreqStep)+1;
 if (ind1 < 0) 
	 ind1 = 0;
 if (ind2 >= Spectr->GetLinesCnt() - 1) 
	 ind2 = Spectr->GetLinesCnt() - 1;
 if (ind1 > ind2)
	 ind1 = ind2;
 
 /*if ((ind1 >= Spectr->GetLinesCnt() - 1) ||
	 ind1 > ind2 || (ind2 - ind1) < 2)
	 return false;*/
 if (ind1 == ind2)
 {
	 i1= ind1;
 }
 else
 {
	max = -1.0;
	i = ind1;
	do
	{
		if(TmpAmp[i] > max || i == ind1)
		{
			max = TmpAmp[i];
			i1 = i;
		}
	}while(++i < ind2);
 }
 /*проверяем, является ли <max> точкой перегиба*/
 s1 = TmpAmp[i1]; /*ценральная*/
 s2 = TmpAmp[i1-1]; /*левая*/
 s3 = TmpAmp[i1+1]; /*правая*/
/* if(s1 - 4 - AverangeLine[i1] <= 0)
 {
  //найденная максимальная величина меньше уровня средней линии + trend
  return false;
 }*/
 dL = 0; dFn = 0;
 if ((s1==s2)&&(s1>s3))
 {
  /*равны центральная и левая и правая меньше их*/
  if (TmpAmp[i1-2] < s1)
  {
   /*составляющая левее левой меньше ее, т.е. пик есть*/
   dFn = - TmpFreqStep / 2; /*частотная поправка, половинка шага*/
   dL = 1.5; /*амплитудная поправка*/
   return true;
  }
  else
  {
   /*иначе это не пик*/
   return false;
  }
 }
 else
 {
  if((s1==s3)&&(s1>s2))
  {
   /*равны центральная и правая и левая меньше их*/
   if(TmpAmp[i1+2] < s1)
   {
    /*составляющая правее правой меньше ее, т.е. пик есть*/
    dFn = TmpFreqStep / 2; /*половинка шага, частотная поправка*/
    dL = 1.5; /*амплитудная поправка*/
    return true;
   }
   else
   {
    /*иначе это не пик*/
    return false;
   }
  }
  else
  {
   if((s1>s2)&&(s1>s3))
   {
    /*левая и правая меньше центральной*/
    if (s2==s3)
    {
     /*левая и правая равны, т.е. поправок не надо -> рез-т центральная*/
     dFn = 0; dL = 0;
    }
    else
    {
     if((s1-s2) < (s1-s3))
     {
      FrequencyCorrection(s1-s2,TmpFreqStep,dL,dFn);
      dFn = -dFn;
     }
     else
      FrequencyCorrection(s1-s3,TmpFreqStep,dL,dFn);
    }
   }
  }
 }
 deltaFc = dFn;
 deltaL = dL;
 F = i1 * TmpFreqStep + dFn;
 L = s1 + dL;
 index = i1;
 return true;
}

//--------------------------------------------------------------------------
// Функция, возвращающая результат сравнения параметров съема спектров.
bool SpectrsParamsAreEqual(TCustomSpectr* ASpectr1, TCustomSpectr* ASpectr2)
{
	if (ASpectr1 == NULL || ASpectr2 == NULL) return false;
	bool res =  ASpectr1->GetClassCode() == ASpectr2->GetClassCode() &&
				ASpectr1->GetType() == ASpectr2->GetType() &&
				*ASpectr1->GetLPFilter() == *ASpectr2->GetLPFilter();
	if (ASpectr1->GetType() == TS_ENVELOPE)
	{
		res = res && *ASpectr1->GetBPFilter() == *ASpectr2->GetBPFilter();
	}
	return res;
}
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// TSpLineList
//--------------------------------------------------------------------------
TSpLineList::TSpLineList()
{
	FList = new TList();
}
//--------------------------------------------------------------------------
TSpLineList::TSpLineList(const TSpLineList& b)
{
	FList = new TList();
	for (int i = 0; i < ((TSpLineList&)b).Count(); i++)
	{
		spline_t* TmpB = ((TSpLineList&)b).GetSpLine(i);
		spline_t* TmpNew = new spline_t;
		*TmpNew = *TmpB;
		this->Add(TmpNew);
	}
}
//--------------------------------------------------------------------------
TSpLineList::~TSpLineList()
{
	Clear();
	delete FList;
	FList = NULL;
}
//--------------------------------------------------------------------------
void TSpLineList::Clear(void)
{
	while (FList->Count > 0)
		Delete(0);
	FList->Clear();
}
//--------------------------------------------------------------------------
int TSpLineList::Add(void* AItem)
{
	return FList->Add(AItem);
}
//--------------------------------------------------------------------------
void TSpLineList::Insert(int AIndex, void* AItem)
{
	FList->Insert(AIndex, AItem);
}
//--------------------------------------------------------------------------
void TSpLineList::Delete(int AIndex)
{
	if (AIndex >= 0 && AIndex < FList->Count)
	{
		spline_t* TmpSP = (spline_t*)FList->Items[AIndex];
		if (TmpSP != NULL)
		{
			delete TmpSP;
			TmpSP = NULL;
		}
		FList->Delete(AIndex);
	}
}
//--------------------------------------------------------------------------
spline_t* TSpLineList::GetSpLine(int AIndex)
{
	spline_t* res = NULL;
	if (AIndex >= 0 && AIndex < FList->Count)
	{
		res = (spline_t*)FList->Items[AIndex];
	}
	return res;
}
//--------------------------------------------------------------------------
int TSpLineList::Count()
{
	return FList->Count;
}
//--------------------------------------------------------------------------
int TSpLineList::GetSpLineIndexByAmpMax(TDSPE_Float AMaxModule, TDSPE_Float AFMin, TDSPE_Float AFMax)
{
	int res = -1;
	TDSPE_Float TmpAmpMax = 0;
	bool TmpFst = true;
	for (int i = 0; i < Count(); i++)
	{
		spline_t* TmpSPL = GetSpLine(i);
		if (TmpSPL != NULL)
		{
			if (TmpSPL->Module >= AMaxModule && 
				TmpSPL->F >= AFMin && 
				TmpSPL->F <= AFMax &&
				(TmpFst || TmpSPL->L > TmpAmpMax))
			{
				TmpFst = false;
				TmpAmpMax = TmpSPL->L; 
				res = i;
			}
		}
	}
	return res;
}
//--------------------------------------------------------------------------
// void TSpLineList::operator = (TSpLineList &b)
// {
// 	this->Clear();
// 	for (int i = 0; i < b.Count(); i++)
// 	{
// 		spline_t* TmpB = b.GetSpLine(i);
// 		spline_t* TmpNew = new spline_t;
// 		*TmpNew = *TmpB;
// 		this->Add(TmpNew);
// 	}
// }
//--------------------------------------------------------------------------
TSpLineList& TSpLineList::operator =(const TSpLineList &b)
{
	this->Clear();
	for (int i = 0; i < ((TSpLineList&)b).Count(); i++)
	{
		spline_t* TmpB = ((TSpLineList&)b).GetSpLine(i);
		spline_t* TmpNew = new spline_t;
		*TmpNew = *TmpB;
		this->Add(TmpNew);
	}
	return *this;
}
//--------------------------------------------------------------------------
// ~TSpLineList
//--------------------------------------------------------------------------




//--------------------------------------------------------------------------
// Базовый класс для всех классов спектров. Абстрактный.
TCustomSpectr::TCustomSpectr()
{
	FAmp = NULL;
	FSIAmp = NULL;
	TCustomSpectr::Clear();
}
//--------------------------------------------------------------------------
TCustomSpectr::~TCustomSpectr()
{
	Clear();	
}
//--------------------------------------------------------------------------
void TCustomSpectr::SetType(unsigned short AType)
{
	if (AType <= TS_ENVELOPE)
		FType = AType;
}
//--------------------------------------------------------------------------
bool TCustomSpectr::FillParams(spectrtaskparams_t ASTP)
{
	bool res = false;
	if (ASTP.SpectrClassCode == GetClassCode())
	{
		SetType(ASTP.SpectrType);
		if (!GetLPFilterIndexValid(ASTP.LPFilterIndex))
			return res;
		FLPFilter = LPFilters[ASTP.LPFilterIndex];
		if (FType == TS_ENVELOPE)
		{
			if (!GetBPFilterIndexValid(ASTP.BPFilterIndex))
				return res;
			FBPFilter = BPFilters[ASTP.BPFilterIndex];
		}
		if (!GetSpectLinesArIndexValid(ASTP.SpectLinesArrIndex))
			return res;
		FBegFreq = 0;
		FFreqStep = FLPFilter.MFreq / SpectLinesAr[ASTP.SpectLinesArrIndex];
                res = true;
	}
	return res;
}
//--------------------------------------------------------------------------
bool TCustomSpectr::FillParams(TCustomSpectr* ASpectr)
{
	bool res = false;
	if (GetClassCode() == ASpectr->GetClassCode())
	{
		SetType(ASpectr->GetType());
		*GetLPFilter() = *ASpectr->GetLPFilter();
		*GetBPFilter() = *ASpectr->GetBPFilter();
		SetBegFreq(ASpectr->GetBegFreq());
		SetFreqStep(ASpectr->GetFreqStep());
        res = true;
	}
	return res;
}
//--------------------------------------------------------------------------
void TCustomSpectr::SetAmp(const TDSPE_Float* AAmp, const unsigned int ALinesCnt, const unsigned short AAmpUnit)
{
	if (!GetAmpUnitValid(AAmpUnit)) return;
	if ((AAmp != NULL && ALinesCnt == 0) ||
		(AAmp == NULL && ALinesCnt != 0))
		return;
	if (FLinesCnt != ALinesCnt)
	{
		ClearData();
		FLinesCnt = ALinesCnt;
		if (FLinesCnt > 0)
		{
			FSIAmp = new TDSPE_Float[FLinesCnt];
			memset(&FSIAmp[0], 0x00, FLinesCnt * sizeof(TDSPE_Float));
		}
	}
	else if (FLinesCnt > 0 && FSIAmp == NULL)
	{
		FSIAmp = new TDSPE_Float[FLinesCnt];
		memset(&FSIAmp[0], 0x00, FLinesCnt * sizeof(TDSPE_Float));
	}
	if (FSIAmp != NULL && AAmp != NULL && FLinesCnt > 0)
	{
		if (AAmpUnit == TU_AMP_A_M_S2)
		{
			memcpy(&FSIAmp[0], &AAmp[0], FLinesCnt * sizeof(TDSPE_Float));
		}
		else
		{
			for (int i = 0; i < FLinesCnt; i++)
			{
				ConvertOtherUnitAmpInSIAmp(AAmp[i], FBegFreq + i * FFreqStep, AAmpUnit, FSIAmp[i]);
			}
		}
		FAmpUnit = TU_AMP_A_M_S2;
	}
	SetAmpUnit(AAmpUnit);
}
//--------------------------------------------------------------------------
void TCustomSpectr::SetAmpUnit(unsigned short AAmpUnit)
{
	if (!GetAmpUnitValid(AAmpUnit)) return;
	if (FSIAmp == NULL) return;
	if (AAmpUnit == TU_AMP_A_M_S2)
	{
		if (FAmp != NULL && FAmp != FSIAmp)
		{
			delete[] FAmp;
		}
		FAmp = FSIAmp;
	}
	else
	{
		if (FAmp == NULL || FAmp == FSIAmp)
		{
			FAmp = new TDSPE_Float[FLinesCnt];
		}
		memset(&FAmp[0], 0x00, FLinesCnt * sizeof(TDSPE_Float));
		for (int i = 0; i < FLinesCnt; i++)
		{
			ConvertSIAmpInOtherUnitAmp(FSIAmp[i], FBegFreq + i * FFreqStep, AAmpUnit, FAmp[i]);
		}
	}
	FAmpUnit = AAmpUnit;
}
//--------------------------------------------------------------------------
void TCustomSpectr::ClearData()
{
	if (FAmp != NULL && FAmp != FSIAmp)
	{
		delete[] FAmp;
	}
	FAmp = NULL;
	if (FSIAmp != NULL)
	{
		delete[] FSIAmp;
		FSIAmp = NULL;
	}
	FLinesCnt = 0;
	FAmpUnit = TU_AMP_A_M_S2;
}
//--------------------------------------------------------------------------
void TCustomSpectr::Clear()
{
	FType = TS_NONE;
	memset(&FLPFilter, 0x00, sizeof(TLPFilter));
	memset(&FBPFilter, 0x00, sizeof(TBPFilter));
	memset(&FStatRot, 0x00, sizeof(TStatRot));
	FAvgCount = 1;
	FBegFreq = 0;
	FFreqStep = 0;
	FLinesCnt = 0;
	TCustomSpectr::ClearData();
}
//--------------------------------------------------------------------------
bool TCustomSpectr::IsValid()
{
	bool res = false;
	if (GetClassCode() == 0)
		return false;

	if (FLPFilter.MFreq == 0)
		return false;

	if (FType == TS_NONE)
		return false;

	if (FType == TS_ENVELOPE)
	{
		if (FBPFilter.CFreq == 0 ||
			(FBPFilter.Oct == 0 && FBPFilter.Width == 0))
			return false;
	}

	if (FFreqStep == 0)
		return false;

// 	if (FStatRot.Avg == 0)
// 		return false;

	if (FLinesCnt == 0 || FAmp == NULL)
		return false;

	res = true;
	return res;
}
//--------------------------------------------------------------------------
TSpLineList TCustomSpectr::GetSpLineList(TDSPE_Float AMinAmp, TDSPE_Float* ABase)
{
	TSpLineList res;
	if (IsValid())
	{
		TDSPE_Float freq = 0;
		TDSPE_Float freqmin = 0;
		TDSPE_Float freqmax = 0;
		TDSPE_Float TmpBaseLine = 0;
		for (int i = 1; i < FLinesCnt; i++)
		{
			freq = FFreqStep * i;
			freqmin = freq - FFreqStep*2;
			freqmax = freq + FFreqStep*2;
			TDSPE_Float F, L, deltaFc, deltaL;
			F = L = deltaFc = deltaL = 0;
			int index = 0;	
			TmpBaseLine = 0;
			if (ABase != NULL)
			{
				TmpBaseLine = ABase[i];
			}
			bool rs = PeakSearch(freq, freqmin, freqmax, this, F, L, deltaFc, deltaL, index);    
			if (rs && index == i && (L - TmpBaseLine) > AMinAmp)
			{
				spline_t* TmpNewSpLine = new spline_t();
				TmpNewSpLine->Clear();
				TmpNewSpLine->F = F;
				TmpNewSpLine->L = L;
				TmpNewSpLine->Module = L - TmpBaseLine;
				TmpNewSpLine->Base = TmpBaseLine;
				res.Add(TmpNewSpLine);
			}
		}
	}
	return res;
}
//--------------------------------------------------------------------------
TSpLineList TCustomSpectr::GetSpLineListBaseAvg(TDSPE_Float AMinAmp, unsigned int AAvgWindowLength)
{
	TSpLineList res;
	return res;
}
//--------------------------------------------------------------------------
void TCustomSpectr::operator = (TCustomSpectr &b)
{
	this->Clear();
	FType = b.FType;
	FLPFilter = b.FLPFilter;
	FBPFilter = b.FBPFilter;
	FStatRot = b.FStatRot;
	FAvgCount = b.FAvgCount;
	FBegFreq = b.FBegFreq;
	FFreqStep = b.FFreqStep;
	FLinesCnt = b.FLinesCnt;
	FAmpUnit = b.FAmpUnit;
	if (b.FSIAmp != NULL)
	{
		FSIAmp = new TDSPE_Float[b.FLinesCnt];
		memcpy(&FSIAmp[0], &b.FSIAmp[0], sizeof(TDSPE_Float) * b.FLinesCnt);
	}
	if (b.FAmp != NULL)
	{
		if (b.FAmp != b.FSIAmp)
		{
			FAmp = new TDSPE_Float[b.FLinesCnt];
			memcpy(&FAmp[0], &b.FAmp[0], sizeof(TDSPE_Float) * b.FLinesCnt);
		}
		else
		{
			FAmp = FSIAmp;
		}
	}
}
//--------------------------------------------------------------------------
// Класс для простых спектров, содержащий только амплитудную характеристику
//--------------------------------------------------------------------------
TSimpleSpectr::TSimpleSpectr()
{
}
//--------------------------------------------------------------------------
TSimpleSpectr::~TSimpleSpectr()
{
}
//--------------------------------------------------------------------------
void TSimpleSpectr::operator = (TSimpleSpectr &b)
{
	*((TCustomSpectr*)this) = *((TCustomSpectr*)&b);
}
//--------------------------------------------------------------------------
// Класс для комплексных спектров, содержащий амплитудную и фазовую 
// характеристику, а также имеющий возможность
// вычислять гармоники частоты вращения.
//--------------------------------------------------------------------------
TComplexSpectr::TComplexSpectr()
{
	FPhase = NULL;
}
//--------------------------------------------------------------------------
TComplexSpectr::~TComplexSpectr()
{
}
//--------------------------------------------------------------------------

void TComplexSpectr::SetPhase(const TDSPE_Float* APhase, const unsigned int ALinesCnt, const unsigned short APhaseUnit)
{
	if (!GetAngleUnitValid(APhaseUnit)) return;
	if ((APhase != NULL && ALinesCnt == 0) ||
		(APhase == NULL && ALinesCnt != 0))
		return;
	if (FLinesCnt != ALinesCnt)
	{
		ClearData();
		FLinesCnt = ALinesCnt;
		if (FLinesCnt > 0)
		{
			FPhase = new TDSPE_Float[FLinesCnt];
			memset(&FPhase[0], 0x00, FLinesCnt * sizeof(TDSPE_Float));
		}
	}
	else if (FLinesCnt > 0 && FPhase == NULL)
	{
		FPhase = new TDSPE_Float[FLinesCnt];
		memset(&FPhase[0], 0x00, FLinesCnt * sizeof(TDSPE_Float));
	}
	if (FPhase != NULL && APhase != NULL && FLinesCnt > 0)
	{
		memcpy(&FPhase[0], &APhase[0], FLinesCnt * sizeof(TDSPE_Float));
	}
	FPhaseUnit = APhaseUnit;
}
//--------------------------------------------------------------------------
void TComplexSpectr::SetPhaseUnit(unsigned short APhaseUnit)
{
	if (!GetAngleUnitValid(APhaseUnit)) return;
	// Тут нужно попробовать переконвертить и если получится
	// изменить текущие единицы измерения
	//FPhaseUnit = APhaseUnit;
}
//--------------------------------------------------------------------------
void TComplexSpectr::ClearData()
{
	TCustomSpectr::ClearData();
	if (FPhase != NULL)
	{
		delete[] FPhase;
		FPhase = NULL;
	}
	FPhaseUnit = TU_ANGLE_GRAD;
}
//--------------------------------------------------------------------------
void TComplexSpectr::Clear()
{
	TCustomSpectr::Clear();
    TComplexSpectr::ClearData();
}
//--------------------------------------------------------------------------
bool TComplexSpectr::CalcHarmonicsFrot(unsigned short AHarmonicNumber, vharmonic_t& AVectorHarmonic)
{
	bool res = false;
	memset(&AVectorHarmonic, 0x00, sizeof(vharmonic_t));
	if (!IsValid()) return false;

	// Для нормального поиска линии частоты вращения нам необходимо
	// использовать коррекцию по частоте и амплитуде.
	// Это возможно только в единицах измерения амплитуды TU_AMP_A_DBA.
	// Если у нас такие единицы измерения, то просто работаем с текущим спектром,
	// в противном случае, создадим его копию и сделаем у нее единицу измерения
	// амплитуды TU_AMP_A_DBA.
	TComplexSpectr* TmpSpectr = this;
	if (this->GetAmpUnit() != TU_AMP_A_DBA)
	{
		TmpSpectr = new TComplexSpectr();
		*TmpSpectr = *this;
		TmpSpectr->SetAmpUnit(TU_AMP_A_DBA);
	}

	TDSPE_Float Harm_Freq = FStatRot.Avg * AHarmonicNumber;
	int F_Harm_index = Harm_Freq / (FFreqStep - FBegFreq);
	if (F_Harm_index == 0)
	{
		// Это означает, что мы собираемся мерить параметры параметры частоты вращения
		// которая заведомо меньше шага по частоте спектра. Это означает, что колебания
		// с нашей частотой вращения не законченные (нет ни одного полного периода) или
		// правильнее сказать, что информация об этих колебаниях отсутствует.
		return false;
	}

	/*TDSPE_Float dfreq1 = FStatRot.Avg - FStatRot.Min;
	TDSPE_Float dfreq2 = FStatRot.Max - FStatRot.Avg;
	TDSPE_Float dfreq = 0;
	if (dfreq1 > dfreq2)
		dfreq = 0*/
	TDSPE_Float TmpF = 0;
	TDSPE_Float TmpL = 0;
	TDSPE_Float deltaFc = 0;
	TDSPE_Float deltaL = 0;
	int TmpIndex = -1;

	// Это правильный вариант, но только не устраивает коррекция по частоте
	if (PeakSearch(Harm_Freq, Harm_Freq*0.05, (TCustomSpectr*)TmpSpectr, TmpF, TmpL, deltaFc, deltaL, TmpIndex))
	{
		// Применим коррекцию
		F_Harm_index = TmpIndex;
	}
	else
	{
		// Это без коррекции
		deltaL = 0;
		deltaFc = Harm_Freq - FBegFreq - F_Harm_index * FFreqStep;
	}

/*	// Это неправильный вариант с отключенной коррекцией по частоте
	if (PeakSearch(Harm_Freq, Harm_Freq*0.05, (TCustomSpectr*)TmpSpectr, TmpF, TmpL, deltaFc, deltaL, TmpIndex))
	{
		// Применим коррекцию
		//F_Harm_index = TmpIndex;

		deltaFc = Harm_Freq - FBegFreq - F_Harm_index * FFreqStep;
	}
	else
	{
		// Это без коррекции
		deltaL = 0;
		deltaFc = Harm_Freq - FBegFreq - F_Harm_index * FFreqStep;
	}*/

	// Теперь проверим, не вылезли ли мы за диапазон индексов массива
	if (F_Harm_index < 0 || F_Harm_index >= GetLinesCnt() || GetLinesCnt() == 0)
		return false;

	AVectorHarmonic.amp = TmpSpectr->GetAmp()[F_Harm_index] + deltaL;
	AVectorHarmonic.freq = FBegFreq + F_Harm_index * FFreqStep + deltaFc;
	// TO DO сделать пересчет фазы в зависимости от единицы измерения
	
	TDSPE_Float Phase_Tmp = FPhase[F_Harm_index];
	if (FPhaseUnit == TU_ANGLE_GRAD)
		Phase_Tmp *= M_PI / 180;
	if (Phase_Tmp < 0)
		Phase_Tmp = Phase_Tmp + 2 * M_PI;
	AVectorHarmonic.ph = (Phase_Tmp - M_PI * (deltaFc) / FFreqStep) * 180 / M_PI;
	if (AVectorHarmonic.ph < 0)
	{
		while (AVectorHarmonic.ph < 0)
			AVectorHarmonic.ph += 360;
	}
	else
	{
		while (AVectorHarmonic.ph > 360)
			AVectorHarmonic.ph -= 360;
	}
	if (TmpSpectr != this) 
		delete TmpSpectr;
	res = true;
	return res;
}
//--------------------------------------------------------------------------
bool TComplexSpectr::IsValid()
{
	bool res = (TCustomSpectr::IsValid() && (FPhase != NULL));
	return res;
}
//--------------------------------------------------------------------------
void TComplexSpectr::operator = (TComplexSpectr &b)
{
	*((TCustomSpectr*)this) = *((TCustomSpectr*)&b);
	this->FPhaseUnit = b.FPhaseUnit;
	if (b.FPhase != NULL)
	{
		FPhase = new TDSPE_Float[b.FLinesCnt];
		memcpy(&FPhase[0], &b.FPhase[0], sizeof(b.FLinesCnt));
	}
}
//--------------------------------------------------------------------------



