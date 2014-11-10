//--------------------------------------------------------------------------
// Author: Sidorov E.N.
// Date: 18.02.2011.
// Модуль, в котором описаны классы для работы со спектрами.
//--------------------------------------------------------------------------
#ifndef TSPECTR_H
#define TSPECTR_H
//--------------------------------------------------------------------------
#include <stdlib.h>
#include <memory.h>
#include "DSPEmulDefines.h"
#include "list.h"
//--------------------------------------------------------------------------
// Типы классов спектров
#define TSC_NONE		0
#define TSC_SIMPLE		1
#define TSC_COMPLEX		2

#define TSC_COUNT		2
//--------------------------------------------------------------------------
class TCustomSpectr;
class TSimpleSpectr;
class TComplexSpectr;
//--------------------------------------------------------------------------
// Функция амплитудной и частотной поправки на эффект частокола при применении весовой функции Ханнинга
extern void FrequencyCorrection(//входные параметры:
                              TDSPE_Float deltaDB, //разность наибольших дискретных
                                              //значений, которые принимают
                                              //спектральные линии непосредственно
                                              //снизу и сверху соответсвующего
                                              //учитываемой составляющей пика
                              TDSPE_Float Step,    //разрешающая способность спектра
                              //выходные параметры:
                              TDSPE_Float &deltaL, //амплитудная поправка
                              TDSPE_Float &deltaFc);//частотная поправка
//--------------------------------------------------------------------------
// Функция поиска пика в спектре
extern bool PeakSearch(TDSPE_Float freq,      //базовая частота для поиска пика
                     TDSPE_Float dfreq,            //коридор поиска
                     TCustomSpectr *Spectr,     //указатель на спектр
                     /*TSpectLine* AverangeLine,//указатель на массив уровней фона*/
                     TDSPE_Float &F,               //частота найденного пика
                     TDSPE_Float &L,               //уровень найденного пика
                     int &index);              //индекс пика в массиве спектра
//--------------------------------------------------------------------------
// Функция поиска пика в спектре
extern bool PeakSearch(TDSPE_Float freq,      //базовая частота для поиска пика
				TDSPE_Float freqmin,            //минимум коридора поиска
				TDSPE_Float freqmax,            //максимум коридора поиска
				TCustomSpectr *Spectr,     //указатель на спектр
				/*TSpectLine* AverangeLine,//указатель на массив уровней фона*/
				TDSPE_Float &F,               //частота найденного пика
				TDSPE_Float &L,               //уровень найденного пика
				TDSPE_Float &deltaFc,		//частотная поправка
				TDSPE_Float &deltaL,			//амплитудная поправка
				int &index);    //индекс пика в массиве спектра

//--------------------------------------------------------------------------
// Структура описания гармонической составляющей после разбора спектра
struct spline_t                       
{                                    
	TDSPE_Float F;					//частота гармоники, в Гц
	TDSPE_Float L;					//амплитуда обнаруженного пика (от уровня фона), в dB
	TDSPE_Float Module;				//амплитуда пика от уровня фона Base, в dB
	TDSPE_Float Base;                 //уровень фона, в dB
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
// Класс для работы со списком гармонических составляющих спектра.
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
// Функция, возвращающая результат сравнения параметров съема спектров.
bool SpectrsParamsAreEqual(TCustomSpectr* ASpectr1, TCustomSpectr* ASpectr2);
//--------------------------------------------------------------------------
// Базовый класс для всех классов спектров. Абстрактный.
class TCustomSpectr
{
public:
	TCustomSpectr();
	virtual ~TCustomSpectr();
	// Код класса спектра (перекрывается в потомках)
	virtual int GetClassCode() = 0;
	// Тип спектра (ФНЧ, огибающей)
	unsigned short GetType(){return FType;};
	void SetType(unsigned short AType);
	// ФНЧ спектра
	TLPFilter* GetLPFilter(){return &FLPFilter;};
	// ПФ спектра (если у него тип спектра огибающей)
	TBPFilter* GetBPFilter(){return &FBPFilter;};
	// Статистика оборотов при получении спектра
	TStatRot* GetStatRot(){return &FStatRot;};
	// Количество усреднений 
	unsigned int GetAvgCount(){return FAvgCount;};
	void SetAvgCount(unsigned int AAvgCount){FAvgCount = AAvgCount;};
	// Начальная частота
	TDSPE_Float GetBegFreq(){return FBegFreq;};
	void SetBegFreq(TDSPE_Float ABegFreq){FBegFreq = ABegFreq;};
	// Шаг по частоте
	TDSPE_Float GetFreqStep(){return FFreqStep;};
	void SetFreqStep(TDSPE_Float AFreqStep){FFreqStep = AFreqStep;};
	// Заполняет параметры спектра из структуры задания для его получения
	bool FillParams(spectrtaskparams_t ASTP);
	// Заполняет параметры спектра из параметров другого спектра
	bool FillParams(TCustomSpectr* ASpectr);
	// Количество линий
	unsigned int GetLinesCnt(){return FLinesCnt;};
	// Массив амплитуд в единицах измерения GetAmpUnit(). Если
	// GetAmpUnit() == TU_AMP_A_M_S2, то GetAmp() == GetSIAmp().
	TDSPE_Float* GetAmp(){return FAmp;};
	// Массив амплитуд в единицах измерения ускорения СИ TU_AMP_A_M_S2
	TDSPE_Float* GetSIAmp(){return FSIAmp;};
	// Единица измерения амплитуды
	unsigned short GetAmpUnit(){return FAmpUnit;};
	void SetAmpUnit(unsigned short AAmpUnit);
	// Устанавливает массив амплитуд (если ALinesCnt != FLinesCnt, то сначала очищаются
	// все массивы данных амплитуд и фаз)
	void SetAmp(const TDSPE_Float* AAmp, const unsigned int ALinesCnt, const unsigned short AAmpUnit = TU_AMP_A_DBA);
	// Очищает данные спектра (т.е. параметры, относящиеся к массивам
	// амплитуд и фаз (если они есть))
	virtual void ClearData();
	// Очищает объект полностью (сбрасывая все фильтры, тип и пр.)
	virtual void Clear();
	// Возвращает то, нормально ли заполнен спектр или нет
	virtual bool IsValid();
	// Возвращает список найденных гармоник. 
	// AMinAmp - минимальная амплитуда гармоники;
	// ABase - базовая линия, массив длиной FLinesCnt. При условии,
	// что ABase == NULL, предполагается, что базовая линия == 0.
	TSpLineList GetSpLineList(TDSPE_Float AMinAmp = 3.0, TDSPE_Float* ABase = NULL);
	// Возвращает список найденных гармоник относительно посчитанной по 
	// алгоритму бегущего среднего базовой линии.
	// AMinAmp - минимальная амплитуда гармоники;
	// AAvgWindowLength - длина окна усреднения.
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
// Класс для простых спектров, содержащий только амплитудную характеристику
class TSimpleSpectr : public TCustomSpectr
{
public:
	TSimpleSpectr();
	virtual ~TSimpleSpectr();
	// Код класса спектра
	virtual int GetClassCode(){return TSC_SIMPLE;};

	void operator = (TSimpleSpectr &b);
private:
};
//--------------------------------------------------------------------------
// Класс для комплексных спектров, содержащий амплитудную и фазовую 
// характеристику, а также имеющий возможность
// вычислять гармоники частоты вращения.
class TComplexSpectr : public TCustomSpectr
{
public:
	TComplexSpectr();
	virtual ~TComplexSpectr();
	// Код класса спектра
	virtual int GetClassCode(){return TSC_COMPLEX;};
	// Массив фаз
	TDSPE_Float* GetPhase(){return FPhase;};
	// Устанавливает массив фаз (если ALinesCnt != FLinesCnt, то сначала очищаются
	// все массивы данных амплитуд и фаз)
	void SetPhase(const TDSPE_Float* APhase, const unsigned int ALinesCnt, const unsigned short APhaseUnit = TU_ANGLE_GRAD);
	// Единица измерения фазы
	unsigned short GetPhaseUnit(){return FPhaseUnit;};
	void SetPhaseUnit(unsigned short APhaseUnit);
	// Очищает данные спектра (т.е. параметры, относящиеся к массивам
	// амплитуд и фаз (если они есть))
	virtual void ClearData();
	// Очищает объект полностью (сбрасывая все фильтры, тип и пр.)
	virtual void Clear();
	// Вычисляет вектор гармоники частоты вращения с номером AHarmonicNumber.
	// AHarmonicNumber == 1 соответствует непосредственно частоте вращения.
	// возвращает 
	//	true - все нормально, AVectorHarmonic - результат вычисления.
	//	false - ошибка.
	bool CalcHarmonicsFrot(unsigned short AHarmonicNumber, vharmonic_t& AVectorHarmonic);
	// Возвращает то, нормально ли заполнен спектр или нет
	virtual bool IsValid();

	void operator = (TComplexSpectr &b);
private:
	TDSPE_Float* FPhase;
	unsigned short FPhaseUnit;
};
//--------------------------------------------------------------------------
#endif