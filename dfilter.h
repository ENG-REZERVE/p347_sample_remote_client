//--------------------------------------------------------------------------
// Author: Sidorov E.N.
// Date: 18.02.2011.
// Модуль с реализацией цифровой фильтрации
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
// Типы всех фильтров (классов фильтров)
#define TF_NONE								0		// 
#define TF_SLIDING_LOWPASS_FIR				1		// Скользящий КИХ ФНЧ (с учетом предистории фильтрации)
#define TF_SLIDING_LOWPASS_HALFBAND_FIR		2		// Скользящий КИХ ФНЧ (с учетом предистории фильтрации) полуполосной, в котором половина значений имп.хар-ки ==0
#define TF_SLIDING_RESAMPLE_FIR				3		// Скользящий КИХ фильтр огибающей (с учетом предистории фильтрации)
#define TF_SLIDING_BANDPASS_FIR				4		// Скользящий КИХ фильтр огибающей (с учетом предистории фильтрации)
#define TF_SLIDING_SPECPASS_FIR				5		// Скользящий специальный (SP) КИХ фильтр (с учетом предистории фильтрации) 
#define TF_SLIDING_LOWPASS_IIR				6		// Скользящий БИХ ФНЧ (с учетом предистории фильтрации)
#define TF_SLIDING_BANDPASS_IIR				7		// Скользящий БИХ фильтр огибающей (с учетом предистории фильтрации)
#define TF_SLIDING_HIGHPASS_IIR				8		// Скользящий БИХ ФВЧ (с учетом предистории фильтрации)
#define TF_SLIDING_HIGHPASS_IIR_FAST		9		// Скользящий БИХ ФВЧ (с учетом предистории фильтрации) с быстрым алгоритмом (однако, характеристики АЧХ зависят от входного параметра Alfa)
#define TF_SLIDING_LOWPASS_IIRBQ			10		// Скользящий биквадратный (BQ) БИХ ФНЧ (с учетом предистории фильтрации) TODO
#define TF_SLIDING_BANDPASS_IIRBQ			11		// Скользящий биквадратный (BQ) БИХ фильтр огибающей (с учетом предистории фильтрации) TODO
#define TF_SLIDING_HIGHPASS_IIRBQ			12		// Скользящий биквадратный (BQ) БИХ ФВЧ (с учетом предистории фильтрации)
#define TF_SLIDING_SPECPASS_IIRBQ			13      // Скользящий биквадратный (BQ) БИХ специальный (SP) фильтр.
#define TF_SLIDING_INTG_TRP                 14      // Скользящий интегрирующий методом трапеций фильтр.
#define TF_HEAP_LOWPASS_FIR					15		// Обрабатывающий за раз буфер данных КИХ ФНЧ (без учета предистории фильтрации)
#define TF_HEAP_BANDPASS_FIR				16		// Обрабатывающий за раз буфер данных КИХ фильтр огибающей (без учета предистории фильтрации)
#define TF_HEAP_LOWPASS_IIR					17		// Обрабатывающий за раз буфер данных БИХ ФНЧ (без учета предистории фильтрации)
#define TF_HEAP_BANDPASS_IIR				18		// Обрабатывающий за раз буфер данных БИХ фильтр огибающей (без учета предистории фильтрации)
#define TF_HEAP_LOWPASS_FIR_ZP				19		// Обрабатывающий за раз буфер данных КИХ ФНЧ с отсутствием искажения фазы сигнала (без учета предистории фильтрации)
#define TF_HEAP_BANDPASS_FIR_ZP				20		// Обрабатывающий за раз буфер данных КИХ фильтр огибающей с отсутствием искажения фазы сигнала (без учета предистории фильтрации)
#define TF_HEAP_LOWPASS_IIR_ZP				21		// Обрабатывающий за раз буфер данных БИХ ФНЧ с отсутствием искажения фазы сигнала (без учета предистории фильтрации)
#define TF_HEAP_BANDPASS_IIR_ZP				22		// Обрабатывающий за раз буфер данных БИХ фильтр огибающей с отсутствием искажения фазы сигнала (без учета предистории фильтрации)
//--------------------------------------------------------------------------
// Типы фильтров
#define TFZP_NONE		0
#define TFZP_LOWPASS	1
#define TFZP_BANDPASS	2
//--------------------------------------------------------------------------
// Фильтр двух проходный децимирующий с отсутствием искажения фазы сигнала
// Базовый класс. Абстрактный.
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
	// Конструктор. AOwnsNextCascade == true указывает
	// на то, что за следующий каскад отвечает этот фильтр
	// (следующий каскад будет удален данным фильтром самостоятельно)
	TFilterZP(bool AOwnsNextCascade = true);
	// Конструктор. AOwnsNextCascade == true куказывает
	// на то, что за следующий каскад отвечает этот фильтр
	// (следующий каскад будет удален данным фильтром самостоятельно)
	// APrevCascade - указывает на предыдущий каскад фильтрации.
	TFilterZP(TFilterZP* APrevCascade, bool AOwnsNextCascade);
	~TFilterZP();
    // Флаг владения следующим каскадом фильтра
	bool GetOwnsNextCascade(){return FOwnsNextCascade;};
	void SetOwnsNextCascade(bool AOwnsNextCascade){FOwnsNextCascade = AOwnsNextCascade;};
    // Указатель на следующий каскад фильтра
	TFilterZP* GetNextCascade(){return FNextCascade;};
	void SetNextCascade(TFilterZP* ANextCascade){FNextCascade = ANextCascade;};
	// Указатель на предыдущий каскад фильтра
	TFilterZP* GetPrevCascade(){return FPrevCascade;};
	void SetPrevCascade(TFilterZP* APrevCascade){FPrevCascade = APrevCascade;};
	// Очистка фильтра и его внутренних буферов
	virtual void Clear();
    // Тип фильтра TFZP_XXX
    virtual unsigned int GetType();
    // Размер буфера
    virtual unsigned int GetSizeBuffer();
    // Порядок фильтра
    virtual unsigned int GetOrder();
    // Величина децимации на выходе фильтра
    virtual unsigned int GetDecim();
	// Функция фильтрации и децимации сигнала AInOutputSig длиной AInOutputSigLength.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool FilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength) = 0;
	// Функция фильтрации и децимации сигнала AInputSig длиной AInputSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim) = 0;
	// Функция децимации сигнала AInOutputSig длиной AInOutputSigLength.
	// Если децимация проведена успешно, то в AInOutputSig возвращается выходной
	// продецимированны сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	bool DecimateSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength);

	// Возвращает итоговый (всего каскада фильтров)
	// коэффициент децимации
	unsigned int GetTotalDecim();
	// Возвращает итоговое смещение сигнала в отсчетах, которое
	// обусловлено децимацией. На данную величину необходимо увеличивать
	// входной сигнал перед отсчетом, которому соответствует нужная начальная
	// метка оборотов.
	int GetTotalDecimSigShift();
	// Возвращает количество каскадов составного фильтра
	unsigned int GetTotalCascadsCount();
	// Возвращает флаг того, что все каскады имеют своего владельца
	bool GetTotalOwnsNextCascade();
    // Выход фильтра
//    property Resultat : Extended read Get_Resultat;
};
//--------------------------------------------------------------------------
// Фильтр двух проходный с отсутствием искажения фазы сигнала
// Децимирующий на 2.
//--------------------------------------------------------------------------
class TLowPassFilterZP : public TFilterZP
{
private:
	TDSPE_LongDouble* GetBArr();
	TDSPE_LongDouble* GetAArr();
	TDSPE_LongDouble* GetZArr();
	// Устанавливает итоговый коэффициент децимации для нижележащего 
	// каскада фильтров. Данная величина должна быть степенью
	// двойки. 
	void SetTotalDecimCurrentCascade(unsigned int ATotalDecim);
public:
	// Конструктор. AOwnsNextCascade == true указывает
	// на то, что за следующий каскад отвечает этот фильтр
	// (следующий каскад будет удален данным фильтром самостоятельно)
	// ATotalDecim - дребуемая децимация фильтра == степень двойки. Если
	// ATotalDecim > 32, то создается каскад фильтров, чтобы добиться
	// требуемой децимации. Если ATotalDecim не степень двойки, то требуемая
	// децимация не будет достигнута и будет равна 0.
	TLowPassFilterZP(unsigned int ATotalDecim, bool AOwnsNextCascade = true);
	// Конструктор. AOwnsNextCascade == true куказывает
	// на то, что за следующий каскад отвечает этот фильтр
	// (следующий каскад будет удален данным фильтром самостоятельно)
	// APrevCascade - указывает на предыдущий каскад фильтрации.
	// ATotalDecim - дребуемая децимация фильтра == степень двойки. Если
	// ATotalDecim > 32, то создается каскад фильтров, чтобы добиться
	// требуемой децимации. Если ATotalDecim не степень двойки, то требуемая
	// децимация не будет достигнута и будет равна 0.
	TLowPassFilterZP(unsigned int ATotalDecim, TFilterZP* APrevCascade = NULL, bool AOwnsNextCascade = true);
	~TLowPassFilterZP();
    // Размер буфера
    virtual unsigned int GetSizeBuffer();
    // Порядок фильтра
    virtual unsigned int GetOrder();
	// Функция фильтрации и децимации сигнала AInOutputSig длиной AInOutputSigLength.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool FilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength);
	// Функция фильтрации и децимации сигнала AInputSig длиной AInputSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim);
	// Устанавливает итоговый (всего каскада фильтров)
	// коэффициент децимации. Данная величина должна быть степенью
	// двойки. Работает только для самого верхнего
	// каскада, т.е. у которого PrevCascade == NULL и при условии,
	// что GetTotalOwnsNextCascade() == true.
	void SetTotalDecim(unsigned int ATotalDecim);

};
//--------------------------------------------------------------------------

/*
//--------------------------------------------------------------------------
// Типы фильтров простых однопроходных
#define TFS_NONE		0
#define TFS_LOWPASS		1
#define TFS_BANDPASS	2
//--------------------------------------------------------------------------
// Фильтр простой однопроходный децимирующий.
// Базовый класс. Абстрактный.
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
	// Конструктор. AOwnsNextCascade == true указывает
	// на то, что за следующий каскад отвечает этот фильтр
	// (следующий каскад будет удален данным фильтром самостоятельно)
	TFilterS(bool AOwnsNextCascade = true);
	// Конструктор. AOwnsNextCascade == true куказывает
	// на то, что за следующий каскад отвечает этот фильтр
	// (следующий каскад будет удален данным фильтром самостоятельно)
	// APrevCascade - указывает на предыдущий каскад фильтрации.
	TFilterS(TFilterS* APrevCascade, bool AOwnsNextCascade);
	~TFilterS();
    // Флаг владения следующим каскадом фильтра
	bool GetOwnsNextCascade(){return FOwnsNextCascade;};
	void SetOwnsNextCascade(bool AOwnsNextCascade){FOwnsNextCascade = AOwnsNextCascade;};
    // Указатель на следующий каскад фильтра
	TFilterS* GetNextCascade(){return FNextCascade;};
	void SetNextCascade(TFilterS* ANextCascade){FNextCascade = ANextCascade;};
	// Указатель на предыдущий каскад фильтра
	TFilterS* GetPrevCascade(){return FPrevCascade;};
	void SetPrevCascade(TFilterS* APrevCascade){FPrevCascade = APrevCascade;};
	// Указатель на последний каскад фильтра
	TFilterS* GetLastCascade();
	// Очистка фильтра и его внутренних буферов
	virtual void Clear();
	// Внутренних буферов фильтра а также буферов его следующих каскадов
	virtual void ClearBuffer();
    // Тип фильтра TFS_XXX
    virtual unsigned int GetType();
    // Размер буфера
    virtual unsigned int GetSizeBuffer();
    // Порядок фильтра
    virtual unsigned int GetOrder();
    // Величина децимации на выходе фильтра
    virtual unsigned int GetDecim();
	// Устанавливает результат фильтрации на выходе текущего каскада
    virtual void SetResult(TDSPE_LongDouble Value);
	// Возвращает результат фильтрации на выходе текущего каскада,
	// одновременно идет сбрасывание флага того, что необходимо чтение
	// результата.
    TDSPE_LongDouble GetResult();
	// Возвращает количество незаполненных элементов буфера
	virtual unsigned int GetCountEmptyItem();

	// Возвращает итоговый (всего каскада фильтров)
	// коэффициент децимации
	unsigned int GetTotalDecim();
	// Возвращает итоговое смещение сигнала в отсчетах, которое
	// обусловлено децимацией. На данную величину необходимо увеличивать
	// входной сигнал перед отсчетом, которому соответствует нужная начальная
	// метка оборотов.
	int GetTotalDecimSigShift();
	// Возвращает количество каскадов составного фильтра
	unsigned int GetTotalCascadsCount();
	// Возвращает флаг того, что все каскады имеют своего владельца
	bool GetTotalOwnsNextCascade();
	// Впихывает в фильтр новый отсчет входного сигнала.
	// Флаг ADecimate указывает на то, необходимо ли продецимировать
	// результат расчета. По умолчанию децимация проводится. 
	// AFillBuffer - флаг того, что нужно прежде чем возвращать значения
	// запонить буфер. По умолчанию сначала запоняется буфер.
	virtual void Push(TDSPE_LongDouble Value, bool AFillBuffer, bool ADecimate) = 0; 
};
//--------------------------------------------------------------------------
// Фильтр ФНЧ простой однопроходный децимирующий.
//--------------------------------------------------------------------------
class TLowPassFilterS : public TFilterS
{
private:
	TDSPE_LongDouble* GetBArr();
	TDSPE_LongDouble* GetAArr();
	TDSPE_LongDouble* GetZArr();
	// Устанавливает итоговый коэффициент децимации для нижележащего 
	// каскада фильтров. Данная величина должна быть степенью
	// двойки. 
	void SetTotalDecimCurrentCascade(unsigned int ATotalDecim);
public:
	// Конструктор. AOwnsNextCascade == true указывает
	// на то, что за следующий каскад отвечает этот фильтр
	// (следующий каскад будет удален данным фильтром самостоятельно)
	// ATotalDecim - дребуемая децимация фильтра == степень двойки. Если
	// ATotalDecim > 32, то создается каскад фильтров, чтобы добиться
	// требуемой децимации. Если ATotalDecim не степень двойки, то требуемая
	// децимация не будет достигнута и будет равна 0.
	TLowPassFilterS(unsigned int ATotalDecim, bool AOwnsNextCascade = true);
	// Конструктор. AOwnsNextCascade == true куказывает
	// на то, что за следующий каскад отвечает этот фильтр
	// (следующий каскад будет удален данным фильтром самостоятельно)
	// APrevCascade - указывает на предыдущий каскад фильтрации.
	// ATotalDecim - дребуемая децимация фильтра == степень двойки. Если
	// ATotalDecim > 32, то создается каскад фильтров, чтобы добиться
	// требуемой децимации. Если ATotalDecim не степень двойки, то требуемая
	// децимация не будет достигнута и будет равна 0.
	TLowPassFilterS(unsigned int ATotalDecim, TFilterS* APrevCascade = NULL, bool AOwnsNextCascade = true);
	~TLowPassFilterS();
    // Порядок фильтра
    virtual unsigned int GetOrder();
	// Устанавливает итоговый (всего каскада фильтров)
	// коэффициент децимации. Данная величина должна быть степенью
	// двойки. Работает только для самого верхнего
	// каскада, т.е. у которого PrevCascade == NULL и при условии,
	// что GetTotalOwnsNextCascade() == true.
	void SetTotalDecim(unsigned int ATotalDecim);
	// Впихывает в фильтр новый отсчет входного сигнала.
	// Флаг ADecimate указывает на то, необходимо ли продецимировать
	// результат расчета. По умолчанию децимация проводится. 
	// AFillBuffer - флаг того, что нужно прежде чем возвращать значения
	// запонить буфер. По умолчанию сначала запоняется буфер.
	virtual void Push(TDSPE_LongDouble Value, bool AFillBuffer = true, bool ADecimate = true); 
};
//--------------------------------------------------------------------------
*/

//--------------------------------------------------------------------------
// Объявление всех классов
class TCustomFilter;
class TSlidingFilter;
class TSlidingLPFilterFIR;
class TSlidingLPHBFilterFIR;
class TSlidingLPFilterIIR;
class TSlidingBPFilterFIR;
class TSlidingBPFilterIIR;
//--------------------------------------------------------------------------
// Функции для работы с фильтрами
//--------------------------------------------------------------------------
// Функции для создания ФНЧ фильтров
extern TCustomFilter* CreateLPFilter(unsigned int AType, unsigned int ADecim, bool AOwnsNextCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
extern TCustomFilter* CreateLPFilter(unsigned int AType, unsigned int ATotalDecim, TCustomFilter* APrevCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE); // APrevCascade = NULL

// Функции для создания полосовых фильтров огибающей
extern TCustomFilter* CreateBPFilter(unsigned int AType, unsigned char AEnvType, bool AOwnsNextCascade, TDSPE_LongDouble AShiftCoefCorrection, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
extern TCustomFilter* CreateBPFilter(unsigned int AType, unsigned char AEnvType, unsigned int ATotalDecim, TCustomFilter* APrevCascade, TDSPE_LongDouble AShiftCoefCorrection, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE); //APrevCascade = NULL
extern TCustomFilter* CreateBPFilter(unsigned int AType, unsigned char AEnvType, unsigned int ATotalDecim, unsigned int APreDecim, TDSPE_LongDouble AShiftCoefCorrection, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);

// Функции для создания фильтров передискретизации
extern TCustomFilter* CreateRSMPFilter(unsigned int AType, unsigned char AResampleType, bool AOwnsNextCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);

// Функции для создания ВЧ фильтров
extern TCustomFilter* CreateHPFilter(unsigned int AType, unsigned char AHPFType, void* AParam1, bool AOwnsNextCascade, unsigned char ATSDTC, unsigned char ATDE);

// Функции для создания специальных (SP) фильтров
extern TCustomFilter* CreateSPFilter(unsigned int AType, unsigned char AHPFType, bool AOwnsNextCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);

// Функции для создания интегрирующих фильтров
extern TCustomFilter* CreateINTGFilter(unsigned int AType, unsigned char AINTGFType, bool AOwnsNextCascade, unsigned char ATSDTC, unsigned char ATDE);

// Функции проверки того, что фильтры имеют одинаковые подтипы (например TSlidingLPFilterFIR и TSlidingBPFilterFIR)
bool GetFiltersEqualSubTypes(unsigned int AType1, unsigned int AType2);
//--------------------------------------------------------------------------
// Базовый класс для всех Фильтров. Абстрактный.
//--------------------------------------------------------------------------
class TCustomFilter
{
protected:
	unsigned char FTDE;			 // Тип устройства, для которого используется фильтр
	unsigned char FTSDTC;		 // Код типа данных, в которых хранится временной сигнал.
	unsigned int FSampleSize;	 // Размер переменной, содержащей отсчет временного сигнала (см. FTSDTC)
	bool FOwnsNextCascade;
	TCustomFilter* FNextCascade;
	TCustomFilter* FPrevCascade;
    unsigned int FDecim; // Коэффициент децимации
	unsigned int FInterp; // Коэффициент интерполяции
	unsigned char FCode;
	char* FSigBuffer;
	unsigned int FSigBufferLength;
	unsigned int FSigBufferRemain;	// кол-во оставшихся отсчетов до заполнения фильтра (Актуально только для SlidingFilter)
    unsigned int FCoefsCount;		// кол-во коэффициентов
    unsigned int FRealCoefsCount;		// кол-во коэффициентов
	unsigned int FAddShift;			
	bool FUsualWidth;
    unsigned int GetTotalDecim(unsigned int APrevTotalDecim);
	unsigned int GetTotalInterp(unsigned int APrevTotalInterp);
	// Возвращает полное смещение сигнала для всего нижележащего каскада,
	// возникающего из-за децимации. 
	// Важно учитывать, если требуется получать отфильтрованный сигнал с
	// определенного момента времени (отсчета).
	int32_t GetTotalDecimSigShift(int APrevTotalDecimSigShift, unsigned int APrevTotalDecim, unsigned int APrevTotalInterp);
	// Возвращает полную величину групповой задержки сигнала для всего нижележащего каскада,
	// возникающего из-за фильтрации. 
	// Важно учитывать, если требуется получать отфильтрованный сигнал с
	// определенного момента времени (отсчета).
	int32_t GetTotalGroupDelaySigShift(int APrevTotalGroupDelaySigShift, unsigned int APrevTotalDecim, unsigned int APrevTotalInterp);
	// Возвращает полное смещение сигнала для всего нижележащего каскада,
	// возникающего из-за заполнения буферов фильтров. 
	// Важно учитывать, если требуется получать отфильтрованный сигнал с
	// определенного момента времени (отсчета).
	int32_t GetTotalBufferingSigShift(int APrevTotalBufferingSigShift, unsigned int APrevTotalDecim, unsigned int APrevTotalInterp);
	// Возвращает полное количество сигнала, оставшегося до полного заполнения\
	// буферов фильтров всего нижележащего каскада. Актуально только
	// для SlidingFilters. Для HeapFilters всегда = 0.
	// Важно учитывать, если требуется получать отфильтрованный сигнал с
	// определенного момента всремени (отсчета).
	int32_t GetTotalBufferingSigRemain(int APrevTotalBufferingSigRemain, unsigned int APrevTotalDecim, unsigned int APrevTotalInterp);

	unsigned int GetTotalCascadsCount(int APrevTotalCascadsCount);
	bool GetTotalOwnsNextCascade(bool APrevTotalOwnsNextCascade);
	// Возвращает итоговый коэффициент децимации для нижележащего 
	// каскада фильтров. 
	unsigned int GetTotalDecimCurrentCascade();
	// Возвращает итоговый коэффициент интерполяции для нижележащего 
	// каскада фильтров. 
	unsigned int GetTotalInterpCurrentCascade();
	// Устанавливает итоговый коэффициент децимации для нижележащего 
	// каскада фильтров. Данная величина должна быть степенью
	// двойки. Работает только для  обычных
	// децимирующих фильтров (ФНЧ). Для фильтров огибающей должна
	// быть перекрыта.
	virtual bool SetTotalDecimCurrentCascade(unsigned int ATotalDecim);
	// Устанавливает коэффициент децимации для текущего фильтра.
	// Данная величина должна быть степенью
	// двойки. Работает только для  обычных
	// децимирующих фильтров (ФНЧ). Для фильтров огибающей не работает.
	virtual bool SetDecim(unsigned int ADecim);
	// Устанавливает итоговый (всего каскада фильтров)
	// коэффициент децимации. Данная величина должна быть степенью
	// двойки. Работает только для самого верхнего
	// каскада, т.е. у которого PrevCascade == NULL и при условии,
	// что GetTotalOwnsNextCascade() == true а также только для обычных
	// децимирующих фильтров (ФНЧ). Для фильтров огибающей не работает.
	virtual bool SetTotalDecim(unsigned int ATotalDecim);
	// Функция, возвращающая указатель на массив B-коеффициентов фильтра
	virtual void* GetBArrCoefs() = 0;
	// Функция, возвращающая указатель на массив A-коеффициентов фильтра
	virtual void* GetAArrCoefs() = 0;
	// Функция, возвращающая указатель на массив Z-коеффициентов фильтра
	virtual void* GetZArrCoefs() = 0;
	// Функция, возвращающая полное количество B-коеффициентов фильтра
	// Это количество обязательно == GetOrder() - 1, и может быть больше
	// реального количества GetRealCoefsCount(). Данное различие возможно
	// по причине оптимизации (у фильтра TSlidingLPHBFilterFIR половина
	// коэффициентов нули), или уникальности структуры фильтра (например,
	// в полифазном передескретизирующем фильтре все коэффициенты разбиты
	// на подфильтры одинаковой длины).
	virtual unsigned int GetCoefsCount();
	// Функция, возвращающая реальное количество B-коеффициентов фильтра.
	virtual unsigned int GetRealCoefsCount() = 0;
	// Функция, возвращающая количество отсчетов входного сигнала, необходимых
	// для стабилизации сигнала на выходе.
	// В случае FIR фильтров это их порядок. В случае IIR фильтров - экспериментально
	// устанавливаемое значение для каждого фильтра.
	virtual unsigned int GetStabSigLength();

	// Устанавливает размер буферов фильтра
	virtual void SetSigBufferLength(unsigned int ASigBufferLength);
public:
	// Конструктор. AOwnsNextCascade == true указывает
	// на то, что за следующий каскад отвечает этот фильтр
	// (следующий каскад будет удален данным фильтром самостоятельно)
	// AUsualWidth - Is Usual Width - флаг того, что переходная полоса W фильтра является обычной (классической),
	//                  т.е. Fstop == 0,5*Fs (Fs - частота дискретизации). В противном случае она == 0.609375*Fs/Decim.
	TCustomFilter(bool AOwnsNextCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	// Конструктор. AOwnsNextCascade == true куказывает
	// на то, что за следующий каскад отвечает этот фильтр
	// (следующий каскад будет удален данным фильтром самостоятельно)
	// APrevCascade - указывает на предыдущий каскад фильтрации.\
	// AUsualWidth - Is Usual Width - флаг того, что переходная полоса W фильтра является обычной (классической),
	//                  т.е. Fstop == 0,5*Fs (Fs - частота дискретизации). В противном случае она == 0.609375*Fs/Decim.
	TCustomFilter(TCustomFilter* APrevCascade, bool AOwnsNextCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	virtual ~TCustomFilter();
    // Флаг владения следующим каскадом фильтра
	bool GetOwnsNextCascade(){return FOwnsNextCascade;};
	void SetOwnsNextCascade(bool AOwnsNextCascade){FOwnsNextCascade = AOwnsNextCascade;};
    // Указатель на следующий каскад фильтра
	TCustomFilter* GetNextCascade(){return FNextCascade;};
	void SetNextCascade(TCustomFilter* ANextCascade){FNextCascade = ANextCascade;};
	// Указатель на предыдущий каскад фильтра
	TCustomFilter* GetPrevCascade(){return FPrevCascade;};
	void SetPrevCascade(TCustomFilter* APrevCascade){FPrevCascade = APrevCascade;};
	// Указатель на самый первый каскад фильтра
	TCustomFilter* GetFirstCascade();
	// Указатель на самый последний каскад фильтра
	TCustomFilter* GetLastCascade();
	// Очистка фильтра и его внутренних буферов
	virtual void Clear();
	// Процедура инициализации фильтра (нужна только для тех, кто
	// подобен TSlidingFilter)
	virtual void Init();
    // Тип фильтра TFZP_XXX
    virtual unsigned int GetType();
    // Длина буфера
    virtual unsigned int GetBufferLength();
    // Порядок фильтра
    virtual unsigned int GetOrder();
    // Величина децимации на выходе фильтра
    virtual unsigned int GetDecim();
	// Величина интерполяции на выходе фильтра
	virtual unsigned int GetInterp();
	// Код фильтра
	virtual unsigned int GetCode();
	// Величина дополнительного смещения вычисленных значений вправо
	// при работе с фиксированной точкой для приведения коэффициента
	// усиления фильтра к 1.
	virtual unsigned int GetAddShift();
	// Указывает на то, что при Decim == 1 фильтр работает,
	// это относится к ФВЧ, SP и т.д.
	virtual bool GetDecimOneMakesChanges(){return false;};
	// Тип устройства, для которого используется фильтр
	unsigned char GetTDE(){return FTDE;};		 
	// Код типа данных, в которых хранится временной сигнал.
	unsigned char GetTSDTC(){return FTSDTC;};		 

	// Величина групповой задержки фильтра в отсчетах (актуальна только для 
	// скользящих симметричных FIR фильтров, так как только у них может быть линейная ФЧХ,
	// у которой групповая задержка в полосе пропускания константа и в отсчетах
	// равна ((N-1)/2), где N - количество коэффициентов.
	virtual unsigned int GetGroupDelay(){return 0;};
    // Максимальная величина децимации данного фильтра данного типа
    virtual unsigned int GetMaxSingleDecim() = 0;
	// Флаг, говорящий о том, что для фильтрации требуется весь
	// набор данных. 
	// Для SlidingFIlter можно фильтровать по частям, а вот для HeapFilter
	// лучше фильровать весь набор данных.
	virtual bool GetNeadAllData() = 0;
	// Флаг, говорящий о том, что фильтр имеет свой внутренний буфер для фильтрации,
	// т.е. для он помнит часть данных предыдущего сигнала для того, чтобы получать
	// новые отфильтрованные значения на выходе.
	// Если true - имеет.
	// Если false - не имеет, и поэтому новая фильтрация никак не зависит от 
	// фильтрации предыдущего сигнала.
	virtual bool GetIsBufferingFilter() = 0;
	// Функция фильтрации и децимации сигнала AInOutputSig длиной AInOutputSigLength.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// AUsedSigLength - длина сигнала, которая была использована при фильтрации.
	// Для всех Sliding фильтров этот параметр будет равен количеству сигнала на входе
	// процедуры AInOutputSigLength, так как даже те отсчеты входного сигнала, которые не 
	// сформировали еще что-то на выходе, лежат в буфере фильтра и ждут недостающих данных.
	// Для Heap фильтров буферизация отсутствует, и он не знает, какие данные обрабатывались
	// перед новым обращением. Поэтому AUsedSigLength будет меньше на количество отсчетов,
	// которые не сформировали ничего на выходе. Эти отсчеты связаны только с децимацией.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool FilterSignal(void* &AInOutputSig, unsigned int &AInOutputSigLength, unsigned int &AUsedSigLength, unsigned char ATSDTC);
	// Функция фильтрации и децимации сигнала AInSig длиной AInSigLength.
	// Если фильтрация проведена успешно, то в AOutSig возвращается выходной
	// отфильтрованный сигнал длиной AOutSigLength. 
	// Если AOutSig == AInSig, это означает, что при фильтрации не создавался
	// дополнительный выходной буфер. В противном случае, буфер под выходной сигнал 
	// AOutSig длиной AOutSigLength создается в теле функции и должен освобождаться вызывающей стороной.
	// После фильтрации в AOutSigLength возвращается полное ковличество сигнала в 
	// буфере AOutSig.
	// AUsedSigLength - длина сигнала, которая была использована при фильтрации.
	// Для всех Sliding фильтров этот параметр будет равен количеству сигнала на входе
	// процедуры AInSigLength, так как даже те отсчеты входного сигнала, которые не 
	// сформировали еще что-то на выходе, лежат в буфере фильтра и ждут недостающих данных.
	// Для Heap фильтров буферизация отсутствует, и он не знает, какие данные обрабатывались
	// перед новым обращением. Поэтому AUsedSigLength будет меньше на количество отсчетов,
	// которые не сформировали ничего на выходе. Эти отсчеты связаны только с децимацией.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool FilterSignal(void* AInSig, unsigned int AInSigLength, void* &AOutSig, unsigned int &AOutSigLength, unsigned int &AUsedSigLength, unsigned char ATSDTC);
	// Функция фильтрации и децимации сигнала AInputSig длиной AInputSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(void* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC) = 0;

	// Функция фильтрации и децимации сигнала AInSig длиной AInSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Если AOutSig == AInSig, это означает, что при фильтрации не создавался
	// дополнительный выходной буфер. В противном случае, буфер под выходной сигнал 
	// AOutSig длиной AOutSigLength создается в теле функции и должен освобождаться вызывающей стороной.
	// После фильтрации в AOutSigLength возвращается полное ковличество сигнала в 
	// буфере AOutSig.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(void* AInSig, unsigned int AInSigLength, bool ANeedDecim, void* &AOutSig, unsigned int &AOutSigLength, unsigned char ATSDTC) = 0;

	// Функция децимации сигнала AInOutputSig длиной AInOutputSigLength.
	// Если децимация проведена успешно, то в AInOutputSig возвращается выходной
	// продецимированны сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	bool DecimateSignal(void* &AInOutputSig, unsigned int &AInOutputSigLength, unsigned char ATSDTC);

	// Возвращает итоговый (всего каскада фильтров)
	// коэффициент децимации
	unsigned int GetTotalDecim();
	// Возвращает итоговый (всего каскада фильтров)
	// коэффициент интерполяции
	unsigned int GetTotalInterp();
	// Возвращает итоговое смещение сигнала в отсчетах, которое
	// обусловлено децимацией. На данную величину необходимо увеличивать
	// входной сигнал перед отсчетом, которому соответствует нужная начальная
	// метка оборотов.
	int32_t GetTotalDecimSigShift();
	// Возвращает итоговую полную величину групповой задержки сигнала в отсчетах,
	// которое обусловлено фильтрацией. 
	// Важно учитывать, если требуется получать отфильтрованный сигнал с
	// определенного момента времени (отсчета).
	int32_t GetTotalGroupDelaySigShift();
	// Возвращает полное смещение сигнала для всего каскада,
	// возникающего из-за заполнения буферов фильтров. 
	// Важно учитывать, если требуется получать отфильтрованный сигнал с
	// определенного момента всремени (отсчета).
	int32_t GetTotalBufferingSigShift();
	// Возвращает полное количество сигнала, оставшегося до полного заполнения\
	// буферов фильтров всего каскада. Актуально только
	// для SlidingFilters. Для HeapFilters всегда = 0.
	// Важно учитывать, если требуется получать отфильтрованный сигнал с
	// определенного момента всремени (отсчета).
	int32_t GetTotalBufferingSigRemain();
	// Возвращает количество каскадов составного фильтра
	unsigned int GetTotalCascadsCount();
	// Возвращает флаг того, что все каскады имеют своего владельца
	bool GetTotalOwnsNextCascade();
};
//--------------------------------------------------------------------------
// Базовый класс для всех скользящих децимирующих фильтров (с учетом предистории фильтрации)
//---------------------------------------------------------------------------
class TSlidingFilter : public TCustomFilter
{
private:
protected:
#ifdef __NEW_FILTER_FUNCTION__
	TMemoryCircleBuffer* FSigBufferMC;
#endif // #ifdef __NEW_FILTER_FUNCTION__

	int	FSigBufferIndex;			// Текущий указатель в буфере
	void* FBArrCoefs;		// Коэффициенты фильтра
    unsigned short FDecimPhase;		// фаза децимации
	// Устанавливает коэффициент децимации для текущего фильтра.
	// Данная величина должна быть степенью
	// двойки. Работает только для  обычных
	// децимирующих фильтров (ФНЧ). Для фильтров огибающей не работает.
	virtual bool SetDecim(unsigned int ADecim);

#ifdef __NEW_FILTER_FUNCTION__
	// Устанавливает размер буферов фильтра
	virtual void SetSigBufferLength(unsigned int ASigBufferLength);
#endif // #ifdef __NEW_FILTER_FUNCTION__
public:
	// Флаг, говорящий о том, что для фильтрации требуется весь
	// набор данных. 
	// Для SlidingFIlter можно фильтровать по частям, а вот для HeapFilter
	// лучше фильровать весь набор данных.
	virtual bool GetNeadAllData(){return false;};
	// Флаг, говорящий о том, что фильтр имеет свой внутренний буфер для фильтрации,
	// т.е. для он помнит часть данных предыдущего сигнала для того, чтобы получать
	// новые отфильтрованные значения на выходе.
	// Если true - имеет.
	// Если false - не имеет, и поэтому новая фильтрация никак не зависит от 
	// фильтрации предыдущего сигнала.
	virtual bool GetIsBufferingFilter(){return true;};
	// Конструктор. AOwnsNextCascade == true указывает
	// на то, что за следующий каскад отвечает этот фильтр
	// (следующий каскад будет удален данным фильтром самостоятельно)
	// ADecim - дребуемая децимация данного фильтра == степень двойки. Если
	// ADecim > GetMaxSingleDecim(), то Decim == GetMaxSingleDecim().
	// Если ATotalDecim не степень двойки, то требуемая
	// децимация не будет достигнута и будет равна 0.
	TSlidingFilter(unsigned int ADecim, bool AOwnsNextCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	// Конструктор. В нем по умолчанию принимается FАOwnsNextCascade == true,
	// что указывает на то, что за следующий каскад отвечает этот фильтр
	// (следующий каскад будет удален данным фильтром самостоятельно)
	// ATotalDecim - дребуемая децимация фильтра == степень двойки. Если
	// ATotalDecim > 32, то создается каскад фильтров, чтобы добиться
	// требуемой децимации. Если ATotalDecim не степень двойки, то требуемая
	// децимация не будет достигнута и будет равна 0.
	TSlidingFilter(unsigned int ATotalDecim, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	// Конструктор. нем по умолчанию принимается FАOwnsNextCascade == true,
	// что указывает на то, что за следующий каскад отвечает этот фильтр
	// (следующий каскад будет удален данным фильтром самостоятельно)
	// APrevCascade - указывает на предыдущий каскад фильтрации.
	// ATotalDecim - дребуемая децимация фильтра == степень двойки. Если
	// ATotalDecim > 32, то создается каскад фильтров, чтобы добиться
	// требуемой децимации. Если ATotalDecim не степень двойки, то требуемая
	// децимация не будет достигнута и будет равна 0.
	TSlidingFilter(unsigned int ATotalDecim, TSlidingFilter* APrevCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	virtual ~TSlidingFilter();


/*	// Очистка фильтра и его внутренних буферов
	virtual void Clear();
    // Тип фильтра TFZP_XXX
    virtual unsigned int GetType();
    // Размер буфера
    virtual unsigned int GetSizeBuffer();
    // Длина буфера
    virtual unsigned int GetBufferLength();
    // Порядок фильтра
    virtual unsigned int GetOrder();
    // Величина децимации на выходе фильтра
    virtual unsigned int GetDecim();*/
	// Функция фильтрации и децимации сигнала AInOutputSig длиной AInOutputSigLength.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	// virtual bool FilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength) = 0;
	// Функция фильтрации и децимации сигнала AInputSig длиной AInputSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	// virtual bool SingleFilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim) = 0;
	virtual void Init();
    virtual uint32_t GetInitLenght();
};
//---------------------------------------------------------------------------
// Скользящий децимирующий КИХ ФНЧ (с учетом предистории фильтрации)
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
	// Функция, возвращающая реальное количество B-коеффициентов фильтра.
	virtual unsigned int GetRealCoefsCount();
	// Устанавливает итоговый коэффициент децимации для нижележащего 
	// каскада фильтров. Данная величина должна быть степенью
	// двойки. Работает только для  обычных
	// децимирующих фильтров (ФНЧ). Для фильтров огибающей должна
	// быть перекрыта.
	virtual bool SetTotalDecimCurrentCascade(unsigned int ATotalDecim);
public:
	// Конструктор. AOwnsNextCascade == true указывает
	// на то, что за следующий каскад отвечает этот фильтр
	// (следующий каскад будет удален данным фильтром самостоятельно)
	// ADecim - дребуемая децимация данного фильтра == степень двойки. Если
	// ADecim > GetMaxSingleDecim(), то Decim == GetMaxSingleDecim().
	// Если ATotalDecim не степень двойки, то требуемая
	// децимация не будет достигнута и будет равна 0.
	TSlidingLPFilterFIR(unsigned int ADecim, bool AOwnsNextCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	// Конструктор. В нем по умолчанию принимается FАOwnsNextCascade == true,
	// что указывает на то, что за следующий каскад отвечает этот фильтр
	// (следующий каскад будет удален данным фильтром самостоятельно)
	// ATotalDecim - дребуемая децимация фильтра == степень двойки. Если
	// ATotalDecim > 32, то создается каскад фильтров, чтобы добиться
	// требуемой децимации. Если ATotalDecim не степень двойки, то требуемая
	// децимация не будет достигнута и будет равна 0.
	TSlidingLPFilterFIR(unsigned int ATotalDecim, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	// Конструктор. нем по умолчанию принимается FАOwnsNextCascade == true,
	// что указывает на то, что за следующий каскад отвечает этот фильтр
	// (следующий каскад будет удален данным фильтром самостоятельно)
	// APrevCascade - указывает на предыдущий каскад фильтрации.
	// ATotalDecim - дребуемая децимация фильтра == степень двойки. Если
	// ATotalDecim > 32, то создается каскад фильтров, чтобы добиться
	// требуемой децимации. Если ATotalDecim не степень двойки, то требуемая
	// децимация не будет достигнута и будет равна 0.
	TSlidingLPFilterFIR(unsigned int ATotalDecim, TSlidingFilter* APrevCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	virtual ~TSlidingLPFilterFIR(){};
    // Тип фильтра TFZP_XXX
    virtual unsigned int GetType();
    // Порядок фильтра
    virtual unsigned int GetOrder();
	// Ширина переходной полосы
	unsigned int GetW();

/*	// Очистка фильтра и его внутренних буферов
	virtual void Clear();
    // Тип фильтра TFZP_XXX
    virtual unsigned int GetType();
    // Размер буфера
    virtual unsigned int GetSizeBuffer();
    // Длина буфера
    virtual unsigned int GetBufferLength();
    // Порядок фильтра
    virtual unsigned int GetOrder();
    // Величина децимации на выходе фильтра
    virtual unsigned int GetDecim();*/
	// Код фильтра
	virtual unsigned int GetCode();
	// Величина групповой задержки фильтра в отсчетах (актуальна только для 
	// скользящих симметричных FIR фильтров, так как только у них может быть линейная ФЧХ,
	// у которой групповая задержка в полосе пропускания константа и в отсчетах
	// равна ((N-1)/2), где N - количество коэффициентов.
	virtual unsigned int GetGroupDelay();
    // Максимальная величина децимации данного фильтра данного типа
    virtual unsigned int GetMaxSingleDecim();
	// Функция фильтрации и децимации сигнала AInOutputSig длиной AInOutputSigLength.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	//virtual bool FilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength);
	// Функция фильтрации и децимации сигнала AInputSig длиной AInputSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(void* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC);

	// Функция фильтрации и децимации сигнала AInSig длиной AInSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Если AOutSig == AInSig, это означает, что при фильтрации не создавался
	// дополнительный выходной буфер. В противном случае, буфер под выходной сигнал 
	// AOutSig длиной AOutSigLength создается в теле функции и должен освобождаться вызывающей стороной.
	// После фильтрации в AOutSigLength возвращается полное ковличество сигнала в 
	// буфере AOutSig.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(void* AInSig, unsigned int AInSigLength, bool ANeedDecim, void* &AOutSig, unsigned int &AOutSigLength, unsigned char ATSDTC);

	// Функция фильтрации и децимации сигнала AInputSig длиной AInputSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Тип входных-выходных данных TDSPE_LongDouble.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim);
	// Функция фильтрации и децимации сигнала AInputSig длиной AInputSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Тип входных-выходных данных int32_t и уточняется через параметр ATSDTC.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(int32_t* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC);
};

//---------------------------------------------------------------------------
// Скользящий децимирующий КИХ ФНЧ (с учетом предистории фильтрации) полуполосной, в котором половина значений имп.хар-ки ==0
// TF_SLIDING_LOWPASS_HALFBAND_FIR
//---------------------------------------------------------------------------
class TSlidingLPHBFilterFIR : public TSlidingFilter
{
private:
protected:
	virtual void* GetBArrCoefs();
	virtual void* GetAArrCoefs();
	virtual void* GetZArrCoefs();
	// Функция, возвращающая реальное количество B-коеффициентов фильтра.
	virtual unsigned int GetRealCoefsCount();
	// Устанавливает итоговый коэффициент децимации для нижележащего 
	// каскада фильтров. Данная величина должна быть степенью
	// двойки. Работает только для  обычных
	// децимирующих фильтров (ФНЧ). Для фильтров огибающей должна
	// быть перекрыта.
	virtual bool SetTotalDecimCurrentCascade(unsigned int ATotalDecim);
public:
	// Конструктор. AOwnsNextCascade == true указывает
	// на то, что за следующий каскад отвечает этот фильтр
	// (следующий каскад будет удален данным фильтром самостоятельно)
	// ADecim - дребуемая децимация данного фильтра == степень двойки. Если
	// ADecim > GetMaxSingleDecim(), то Decim == GetMaxSingleDecim().
	// Если ATotalDecim не степень двойки, то требуемая
	// децимация не будет достигнута и будет равна 0.
	TSlidingLPHBFilterFIR(unsigned int ADecim, bool AOwnsNextCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	// Конструктор. В нем по умолчанию принимается FАOwnsNextCascade == true,
	// что указывает на то, что за следующий каскад отвечает этот фильтр
	// (следующий каскад будет удален данным фильтром самостоятельно)
	// ATotalDecim - дребуемая децимация фильтра == степень двойки. Если
	// ATotalDecim > 32, то создается каскад фильтров, чтобы добиться
	// требуемой децимации. Если ATotalDecim не степень двойки, то требуемая
	// децимация не будет достигнута и будет равна 0.
	TSlidingLPHBFilterFIR(unsigned int ATotalDecim, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	// Конструктор. нем по умолчанию принимается FАOwnsNextCascade == true,
	// что указывает на то, что за следующий каскад отвечает этот фильтр
	// (следующий каскад будет удален данным фильтром самостоятельно)
	// APrevCascade - указывает на предыдущий каскад фильтрации.
	// ATotalDecim - дребуемая децимация фильтра == степень двойки. Если
	// ATotalDecim > 32, то создается каскад фильтров, чтобы добиться
	// требуемой децимации. Если ATotalDecim не степень двойки, то требуемая
	// децимация не будет достигнута и будет равна 0.
	TSlidingLPHBFilterFIR(unsigned int ATotalDecim, TSlidingFilter* APrevCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	virtual ~TSlidingLPHBFilterFIR(){};
    // Тип фильтра TFZP_XXX
    virtual unsigned int GetType();
    // Порядок фильтра
    virtual unsigned int GetOrder();
	// Код фильтра
	virtual unsigned int GetCode();

/*	// Очистка фильтра и его внутренних буферов
	virtual void Clear();
    // Тип фильтра TFZP_XXX
    virtual unsigned int GetType();
    // Размер буфера
    virtual unsigned int GetSizeBuffer();
    // Длина буфера
    virtual unsigned int GetBufferLength();
    // Порядок фильтра
    virtual unsigned int GetOrder();
    // Величина децимации на выходе фильтра
    virtual unsigned int GetDecim();*/

	// Величина групповой задержки фильтра в отсчетах (актуальна только для 
	// скользящих симметричных FIR фильтров, так как только у них может быть линейная ФЧХ,
	// у которой групповая задержка в полосе пропускания константа и в отсчетах
	// равна ((N-1)/2), где N - количество коэффициентов.
	virtual unsigned int GetGroupDelay();
    // Максимальная величина децимации данного фильтра данного типа
    virtual unsigned int GetMaxSingleDecim();
	// Функция фильтрации и децимации сигнала AInOutputSig длиной AInOutputSigLength.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	//virtual bool FilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength);
	// Функция фильтрации и децимации сигнала AInputSig длиной AInputSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(void* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC);
	// Функция фильтрации и децимации сигнала AInSig длиной AInSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Если AOutSig == AInSig, это означает, что при фильтрации не создавался
	// дополнительный выходной буфер. В противном случае, буфер под выходной сигнал 
	// AOutSig длиной AOutSigLength создается в теле функции и должен освобождаться вызывающей стороной.
	// После фильтрации в AOutSigLength возвращается полное ковличество сигнала в 
	// буфере AOutSig.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(void* AInSig, unsigned int AInSigLength, bool ANeedDecim, void* &AOutSig, unsigned int &AOutSigLength, unsigned char ATSDTC);

	// Функция фильтрации и децимации сигнала AInputSig длиной AInputSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Тип входных-выходных данных TDSPE_LongDouble.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim);
	// Функция фильтрации и децимации сигнала AInputSig длиной AInputSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Тип входных-выходных данных int32_t и уточняется через параметр ATSDTC.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(int32_t* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC);

};
//---------------------------------------------------------------------------
// Скользящий децимирующий БИХ ФНЧ (с учетом предистории фильтрации)
// TF_SLIDING_LOWPASS_IIR
//---------------------------------------------------------------------------
class TSlidingLPFilterIIR : public TSlidingFilter
{
private:
	int FSigBufferIndexY; // Текущий указатель в буфере выходных значений
	char* FSigBufferY;	// буфер IIR фильтра для хранения отсчетов выходных значений

protected:
	void* FAArrCoefs;		// Коэффициенты фильтра
	void* FZArrCoefs;		// Коэффициенты фильтра
	virtual void* GetBArrCoefs();
	virtual void* GetAArrCoefs();
	virtual void* GetZArrCoefs();
	// Функция, возвращающая реальное количество B-коеффициентов фильтра.
	virtual unsigned int GetRealCoefsCount();
	// Функция, возвращающая количество отсчетов входного сигнала, необходимых
	// для стабилизации сигнала на выходе.
	// В случае FIR фильтров это их порядок. В случае IIR фильтров - экспериментально
	// устанавливаемое значение для каждого фильтра.
	virtual unsigned int GetStabSigLength();
	// Устанавливает итоговый коэффициент децимации для нижележащего 
	// каскада фильтров. Данная величина должна быть степенью
	// двойки. Работает только для  обычных
	// децимирующих фильтров (ФНЧ). Для фильтров огибающей должна
	// быть перекрыта.
	virtual bool SetTotalDecimCurrentCascade(unsigned int ATotalDecim);
	// Устанавливает коэффициент децимации для текущего фильтра.
	// Данная величина должна быть степенью
	// двойки. Работает только для  обычных
	// децимирующих фильтров (ФНЧ). Для фильтров огибающей не работает.
	virtual bool SetDecim(unsigned int ADecim);
	// Устанавливает размер буферов фильтра
	virtual void SetSigBufferLength(unsigned int ASigBufferLength);

public:
	// Конструктор. AOwnsNextCascade == true указывает
	// на то, что за следующий каскад отвечает этот фильтр
	// (следующий каскад будет удален данным фильтром самостоятельно)
	// ADecim - дребуемая децимация данного фильтра == степень двойки. Если
	// ADecim > GetMaxSingleDecim(), то Decim == GetMaxSingleDecim().
	// Если ATotalDecim не степень двойки, то требуемая
	// децимация не будет достигнута и будет равна 0.
	TSlidingLPFilterIIR(unsigned int ADecim, bool AOwnsNextCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	// Конструктор. В нем по умолчанию принимается FАOwnsNextCascade == true,
	// что указывает на то, что за следующий каскад отвечает этот фильтр
	// (следующий каскад будет удален данным фильтром самостоятельно)
	// ATotalDecim - дребуемая децимация фильтра == степень двойки. Если
	// ATotalDecim > 32, то создается каскад фильтров, чтобы добиться
	// требуемой децимации. Если ATotalDecim не степень двойки, то требуемая
	// децимация не будет достигнута и будет равна 0.
	TSlidingLPFilterIIR(unsigned int ATotalDecim, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	// Конструктор. нем по умолчанию принимается FАOwnsNextCascade == true,
	// что указывает на то, что за следующий каскад отвечает этот фильтр
	// (следующий каскад будет удален данным фильтром самостоятельно)
	// APrevCascade - указывает на предыдущий каскад фильтрации.
	// ATotalDecim - дребуемая децимация фильтра == степень двойки. Если
	// ATotalDecim > 32, то создается каскад фильтров, чтобы добиться
	// требуемой децимации. Если ATotalDecim не степень двойки, то требуемая
	// децимация не будет достигнута и будет равна 0.
	TSlidingLPFilterIIR(unsigned int ATotalDecim, TSlidingFilter* APrevCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	virtual ~TSlidingLPFilterIIR(){if (FSigBufferY != NULL){delete[] FSigBufferY; FSigBufferY = NULL;}};
    // Тип фильтра TFZP_XXX
    virtual unsigned int GetType();
    // Порядок фильтра
    virtual unsigned int GetOrder();
	// Код фильтра
	virtual unsigned int GetCode();
/*	// Очистка фильтра и его внутренних буферов
	virtual void Clear();
    // Тип фильтра TFZP_XXX
    virtual unsigned int GetType();
    // Размер буфера
    virtual unsigned int GetSizeBuffer();
    // Длина буфера
    virtual unsigned int GetBufferLength();
    // Порядок фильтра
    virtual unsigned int GetOrder();
    // Величина децимации на выходе фильтра
    virtual unsigned int GetDecim();*/
    // Максимальная величина децимации данного фильтра данного типа
    virtual unsigned int GetMaxSingleDecim();
	// Функция фильтрации и децимации сигнала AInOutputSig длиной AInOutputSigLength.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	//virtual bool FilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength);
	// Функция фильтрации и децимации сигнала AInputSig длиной AInputSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(void* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC);

	// Функция фильтрации и децимации сигнала AInSig длиной AInSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Если AOutSig == AInSig, это означает, что при фильтрации не создавался
	// дополнительный выходной буфер. В противном случае, буфер под выходной сигнал 
	// AOutSig длиной AOutSigLength создается в теле функции и должен освобождаться вызывающей стороной.
	// После фильтрации в AOutSigLength возвращается полное ковличество сигнала в 
	// буфере AOutSig.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(void* AInSig, unsigned int AInSigLength, bool ANeedDecim, void* &AOutSig, unsigned int &AOutSigLength, unsigned char ATSDTC);

	// Функция фильтрации и децимации сигнала AInputSig длиной AInputSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Тип входных-выходных данных TDSPE_LongDouble.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim);
	// Функция фильтрации и децимации сигнала AInputSig длиной AInputSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Тип входных-выходных данных int32_t и уточняется через параметр ATSDTC.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(int32_t* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC);
	virtual void Init();

};


//---------------------------------------------------------------------------
// Класс, объявляющий основные свойства и методы полифазного фильтра 
// требующегося для передескретизации.
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
// Скользящий децимирующий БИХ фильтр огибающей (с учетом предистории фильтрации)
// TF_SLIDING_BANDPASS_FIR
//---------------------------------------------------------------------------
class TSlidingResampleFilterFIR : public TCustomSlidingResampleFilter, public TSlidingLPFilterFIR
{
private:
protected:
	unsigned short FInterpPhase;		// фаза интерполяции
	virtual void* GetBArrCoefs();
	virtual void* GetAArrCoefs();
	virtual void* GetZArrCoefs();
	// Функция, возвращающая реальное количество B-коеффициентов фильтра.
	virtual unsigned int GetRealCoefsCount();
	// Устанавливает размер буферов фильтра
	//virtual void SetSigBufferLength(unsigned int ASigBufferLength);
	// Устанавливает итоговый коэффициент децимации для нижележащего 
	// каскада фильтров. Данная величина должна быть степенью
	// двойки. Работает только для  обычных
	// децимирующих фильтров (ФНЧ). Для фильтров огибающей должна
	// быть перекрыта.
	//virtual bool SetTotalDecimCurrentCascade(unsigned int ATotalDecim);
	// Устанавливает коэффициент децимации для текущего фильтра.
	// Данная величина должна быть степенью
	// двойки. Работает только для  обычных
	// децимирующих фильтров (ФНЧ). Для фильтров огибающей не работает.
	virtual bool SetDecim(unsigned int ADecim);
	virtual void SetResampleType(unsigned char AResampleType);
public:
	// Конструктор. AOwnsNextCascade == true указывает
	// на то, что за следующий каскад отвечает этот фильтр
	// (следующий каскад будет удален данным фильтром самостоятельно)
	// AEnvType - тип фильтра огибающей. Данный конструктор создает фильтр
	// огибающей с фиксированной децимацией.
	TSlidingResampleFilterFIR(unsigned char AResampleType, bool AOwnsNextCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	virtual ~TSlidingResampleFilterFIR(){};
    // Тип фильтра TFZP_XXX
    virtual unsigned int GetType();
    // Порядок фильтра
    virtual unsigned int GetOrder();
	// Код фильтра
	virtual unsigned int GetCode();
    // Величина децимации на выходе фильтра
    virtual unsigned int GetDecim();
	// Величина интерполяции на выходе фильтра
	virtual unsigned int GetInterp();
	// Величина дополнительного смещения вычисленных значений вправо
	// при работе с фиксированной точкой для приведения коэффициента
	// усиления фильтра к 1.
	virtual unsigned int GetAddShift();
	// Указывает на то, что при Decim == 1 фильтр работает,
	// это относится к ФВЧ, SP и т.д.
	virtual bool GetDecimOneMakesChanges(){return true;};

/*	// Очистка фильтра и его внутренних буферов
	virtual void Clear();
    // Тип фильтра TFZP_XXX
    virtual unsigned int GetType();
    // Размер буфера
    virtual unsigned int GetSizeBuffer();
    // Длина буфера
    virtual unsigned int GetBufferLength();
    // Порядок фильтра
    virtual unsigned int GetOrder();
    // Величина децимации на выходе фильтра
    virtual unsigned int GetDecim();*/
    // Максимальная величина децимации данного фильтра данного типа
    //virtual unsigned int GetMaxSingleDecim();
	// Функция фильтрации и децимации сигнала AInOutputSig длиной AInOutputSigLength.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	// virtual bool FilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength) = 0;
	// Функция фильтрации и децимации сигнала AInputSig длиной AInputSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(void* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC);
	// Функция фильтрации и децимации сигнала AInSig длиной AInSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Если AOutSig == AInSig, это означает, что при фильтрации не создавался
	// дополнительный выходной буфер. В противном случае, буфер под выходной сигнал 
	// AOutSig длиной AOutSigLength создается в теле функции и должен освобождаться вызывающей стороной.
	// После фильтрации в AOutSigLength возвращается полное ковличество сигнала в 
	// буфере AOutSig.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(void* AInSig, unsigned int AInSigLength, bool ANeedDecim, void* &AOutSig, unsigned int &AOutSigLength, unsigned char ATSDTC);

	// Функция фильтрации и децимации сигнала AInputSig длиной AInputSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Тип входных-выходных данных TDSPE_LongDouble.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(TDSPE_LongDouble* AInSig, unsigned int AInSigLength, bool ANeedDecim, TDSPE_LongDouble* &AOutSig, unsigned int &AOutSigLength);
	// Функция фильтрации и децимации сигнала AInputSig длиной AInputSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Тип входных-выходных данных int32_t и уточняется через параметр ATSDTC.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(int32_t* AInSig, unsigned int AInSigLength, bool ANeedDecim, int32_t* &AOutSig, unsigned int &AOutSigLength, unsigned char ATSDTC);

	virtual void Init();
    //virtual uint32_t GetInitLenght();
};
//---------------------------------------------------------------------------
// Класс, объявляющий основные свойства и методы фильтра огибающей
//---------------------------------------------------------------------------
class TCustomSlidingBPFilter
{
private:
protected:
	unsigned int FEnvType;
	char* FSigBufferImag;	// буфер фильтра хранения мнимой части
    char* FSinValues;			// значения синуса
    char* FCosValues;		    // значения косинуса
    TDSPE_LongDouble  FShiftCoef;
	// Коррекция смещения величины FShiftCoef.
	// На этот параметр идет домножение FShiftCoef. Это нужно, например,
	// для того, чтобы учесть, что на вход фильтра поступает не частота
	// степень двойки, а еще не передескретизированная.
	TDSPE_LongDouble  FShiftCoefCorrection; 
	int32_t  FShiftCoefInt32;
    int FSinCosIndex;      // текущий индекс в массиве значений синуса
	virtual void SetEnvType(unsigned int AEnvType) = 0;
	virtual TDSPE_LongDouble GetShiftCoef() = 0;
public:
	TCustomSlidingBPFilter(unsigned int AEnvType);
	virtual ~TCustomSlidingBPFilter();
	TDSPE_LongDouble GetShiftCoefCorrection(){return FShiftCoefCorrection;};
	// Устанавливает новое значение коэффициента коррекции смещения величины FShiftCoef
	virtual void SetShiftCoefCorrection(TDSPE_LongDouble AShiftCoefCorrection) = 0;
};
//---------------------------------------------------------------------------
// Скользящий децимирующий БИХ фильтр огибающей (с учетом предистории фильтрации)
// TF_SLIDING_BANDPASS_FIR
//---------------------------------------------------------------------------
class TSlidingBPFilterFIR : public TCustomSlidingBPFilter, public TSlidingLPFilterFIR
{
private:
protected:
	virtual void* GetBArrCoefs();
	virtual void* GetAArrCoefs();
	virtual void* GetZArrCoefs();
	// Функция, возвращающая реальное количество B-коеффициентов фильтра.
	virtual unsigned int GetRealCoefsCount();
	// Устанавливает размер буферов фильтра
	virtual void SetSigBufferLength(unsigned int ASigBufferLength);
	// Устанавливает итоговый коэффициент децимации для нижележащего 
	// каскада фильтров. Данная величина должна быть степенью
	// двойки. Работает только для  обычных
	// децимирующих фильтров (ФНЧ). Для фильтров огибающей должна
	// быть перекрыта.
	virtual bool SetTotalDecimCurrentCascade(unsigned int ATotalDecim);
	// Устанавливает коэффициент децимации для текущего фильтра.
	// Данная величина должна быть степенью
	// двойки. Работает только для  обычных
	// децимирующих фильтров (ФНЧ). Для фильтров огибающей не работает.
	virtual bool SetDecim(unsigned int ADecim);
	virtual void SetEnvType(unsigned int AEnvType);
	virtual TDSPE_LongDouble GetShiftCoef();
public:
	// Конструктор. AOwnsNextCascade == true указывает
	// на то, что за следующий каскад отвечает этот фильтр
	// (следующий каскад будет удален данным фильтром самостоятельно)
	// AEnvType - тип фильтра огибающей. Данный конструктор создает фильтр
	// огибающей с фиксированной децимацией.
	TSlidingBPFilterFIR(unsigned char AEnvType, bool AOwnsNextCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	// Конструктор. AOwnsNextCascade == true указывает
	// на то, что за следующий каскад отвечает этот фильтр
	// (следующий каскад будет удален данным фильтром самостоятельно)
	// ATotalDecim - дребуемая децимация фильтра == степень двойки. Причем 
	// если ATotalDecim > степени децимации определенной типом фильтра огибающей, то создается каскад фильтров, чтобы добиться
	// требуемой децимации. Если ATotalDecim не степень двойки, то требуемая
	// децимация не будет достигнута и будет равна 0.
	// AEnvType - тип фильтра огибающей.
	TSlidingBPFilterFIR(unsigned char AEnvType, unsigned int ATotalDecim, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	// Конструктор. AOwnsNextCascade == true указывает
	// на то, что за следующий каскад отвечает этот фильтр
	// (следующий каскад будет удален данным фильтром самостоятельно)
	// ATotalDecim - дребуемая децимация фильтра == степень двойки. Причем 
	// если ATotalDecim > степени децимации определенной типом фильтра огибающей, то создается каскад фильтров, чтобы добиться
	// требуемой децимации. Если ATotalDecim не степень двойки, то требуемая
	// децимация не будет достигнута и будет равна 0.
	// AEnvType - тип фильтра огибающей.
	// APrevCascade - указывает на предыдущий каскад фильтрации.
	TSlidingBPFilterFIR(unsigned char AEnvType, unsigned int ATotalDecim, TSlidingFilter* APrevCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	// Конструктор. AOwnsNextCascade == true указывает
	// на то, что за следующий каскад отвечает этот фильтр
	// (следующий каскад будет удален данным фильтром самостоятельно)
	// ATotalDecim - дребуемая децимация фильтра == степень двойки. Причем 
	// если ATotalDecim > степени децимации определенной типом фильтра огибающей умножить
	// на APreDecim, то создается каскад фильтров, чтобы добиться
	// требуемой децимации. Также понятно, что данный конструктор при APreDecim > 1
	// создает еще один ФНЧ фильтр (каскад фильтров) с общей децимацией == APreDecim. 
	// Если ATotalDecim не степень двойки, то требуемая
	// децимация не будет достигнута и будет равна 0.
	// AEnvType - тип фильтра огибающей.
	TSlidingBPFilterFIR(unsigned char AEnvType, unsigned int ATotalDecim, unsigned int APreDecim, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	virtual ~TSlidingBPFilterFIR(){};
    // Тип фильтра TFZP_XXX
    virtual unsigned int GetType();
    // Порядок фильтра
    virtual unsigned int GetOrder();
	// Код фильтра
	virtual unsigned int GetCode();
    // Величина децимации на выходе фильтра
    virtual unsigned int GetDecim();
	// Величина дополнительного смещения вычисленных значений вправо
	// при работе с фиксированной точкой для приведения коэффициента
	// усиления фильтра к 1.
	virtual unsigned int GetAddShift();
	// Устанавливает новое значение коэффициента коррекции смещения величины FShiftCoef
	virtual void SetShiftCoefCorrection(TDSPE_LongDouble AShiftCoefCorrection);
/*	// Очистка фильтра и его внутренних буферов
	virtual void Clear();
    // Тип фильтра TFZP_XXX
    virtual unsigned int GetType();
    // Размер буфера
    virtual unsigned int GetSizeBuffer();
    // Длина буфера
    virtual unsigned int GetBufferLength();
    // Порядок фильтра
    virtual unsigned int GetOrder();
    // Величина децимации на выходе фильтра
    virtual unsigned int GetDecim();*/
    // Максимальная величина децимации данного фильтра данного типа
    virtual unsigned int GetMaxSingleDecim();
	// Функция фильтрации и децимации сигнала AInOutputSig длиной AInOutputSigLength.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	// virtual bool FilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength) = 0;
	// Функция фильтрации и децимации сигнала AInputSig длиной AInputSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(void* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC);
	// Функция фильтрации и децимации сигнала AInSig длиной AInSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Если AOutSig == AInSig, это означает, что при фильтрации не создавался
	// дополнительный выходной буфер. В противном случае, буфер под выходной сигнал 
	// AOutSig длиной AOutSigLength создается в теле функции и должен освобождаться вызывающей стороной.
	// После фильтрации в AOutSigLength возвращается полное ковличество сигнала в 
	// буфере AOutSig.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(void* AInSig, unsigned int AInSigLength, bool ANeedDecim, void* &AOutSig, unsigned int &AOutSigLength, unsigned char ATSDTC);

	// Функция фильтрации и децимации сигнала AInputSig длиной AInputSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Тип входных-выходных данных TDSPE_LongDouble.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim);
	// Функция фильтрации и децимации сигнала AInputSig длиной AInputSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Тип входных-выходных данных int32_t и уточняется через параметр ATSDTC.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(int32_t* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC);

	virtual void Init();
    //virtual uint32_t GetInitLenght();
};
//---------------------------------------------------------------------------
// Скользящий децимирующий БИХ фильтр огибающей (с учетом предистории фильтрации)
// TF_SLIDING_BANDPASS_IIR
//---------------------------------------------------------------------------
class TSlidingBPFilterIIR : public TCustomSlidingBPFilter, public TSlidingLPFilterIIR
{
private:
protected:
	virtual void* GetBArrCoefs();
	virtual void* GetAArrCoefs();
	virtual void* GetZArrCoefs();
	// Функция, возвращающая реальное количество B-коеффициентов фильтра.
	virtual unsigned int GetRealCoefsCount();
	// Функция, возвращающая количество отсчетов входного сигнала, необходимых
	// для стабилизации сигнала на выходе.
	// В случае FIR фильтров это их порядок. В случае IIR фильтров - экспериментально
	// устанавливаемое значение для каждого фильтра. 
	// TODO !!! - необходимо определиться в случае каждого фильтра
	virtual unsigned int GetStabSigLength(){return TCustomFilter::GetStabSigLength();};

	// Устанавливает размер буферов фильтра
	virtual void SetSigBufferLength(unsigned int ASigBufferLength);
	// Устанавливает итоговый коэффициент децимации для нижележащего 
	// каскада фильтров. Данная величина должна быть степенью
	// двойки. Работает только для  обычных
	// децимирующих фильтров (ФНЧ). Для фильтров огибающей должна
	// быть перекрыта.
	virtual bool SetTotalDecimCurrentCascade(unsigned int ATotalDecim);
	// Устанавливает коэффициент децимации для текущего фильтра.
	// Данная величина должна быть степенью
	// двойки. Работает только для  обычных
	// децимирующих фильтров (ФНЧ). Для фильтров огибающей не работает.
	virtual bool SetDecim(unsigned int ADecim);
	virtual void SetEnvType(unsigned int AEnvType);
	virtual TDSPE_LongDouble GetShiftCoef();
public:
	// Конструктор. AOwnsNextCascade == true указывает
	// на то, что за следующий каскад отвечает этот фильтр
	// (следующий каскад будет удален данным фильтром самостоятельно)
	// AEnvType - тип фильтра огибающей. Данный конструктор создает фильтр
	// огибающей с фиксированной децимацией.
	TSlidingBPFilterIIR(unsigned char AEnvType, bool AOwnsNextCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	// Конструктор. AOwnsNextCascade == true указывает
	// на то, что за следующий каскад отвечает этот фильтр
	// (следующий каскад будет удален данным фильтром самостоятельно)
	// ATotalDecim - дребуемая децимация фильтра == степень двойки. Причем 
	// если ATotalDecim > степени децимации определенной типом фильтра огибающей, то создается каскад фильтров, чтобы добиться
	// требуемой децимации. Если ATotalDecim не степень двойки, то требуемая
	// децимация не будет достигнута и будет равна 0.
	// AEnvType - тип фильтра огибающей.
	TSlidingBPFilterIIR(unsigned char AEnvType, unsigned int ATotalDecim, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	// Конструктор. AOwnsNextCascade == true указывает
	// на то, что за следующий каскад отвечает этот фильтр
	// (следующий каскад будет удален данным фильтром самостоятельно)
	// ATotalDecim - дребуемая децимация фильтра == степень двойки. Причем 
	// если ATotalDecim > степени децимации определенной типом фильтра огибающей, то создается каскад фильтров, чтобы добиться
	// требуемой децимации. Если ATotalDecim не степень двойки, то требуемая
	// децимация не будет достигнута и будет равна 0.
	// AEnvType - тип фильтра огибающей.
	// APrevCascade - указывает на предыдущий каскад фильтрации.
	TSlidingBPFilterIIR(unsigned char AEnvType, unsigned int ATotalDecim, TSlidingFilter* APrevCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	// Конструктор. AOwnsNextCascade == true указывает
	// на то, что за следующий каскад отвечает этот фильтр
	// (следующий каскад будет удален данным фильтром самостоятельно)
	// ATotalDecim - дребуемая децимация фильтра == степень двойки. Причем 
	// если ATotalDecim > степени децимации определенной типом фильтра огибающей умножить
	// на APreDecim, то создается каскад фильтров, чтобы добиться
	// требуемой децимации. Также понятно, что данный конструктор при APreDecim > 1
	// создает еще один ФНЧ фильтр (каскад фильтров) с общей децимацией == APreDecim. 
	// Если ATotalDecim не степень двойки, то требуемая
	// децимация не будет достигнута и будет равна 0.
	// AEnvType - тип фильтра огибающей.
	TSlidingBPFilterIIR(unsigned char AEnvType, unsigned int ATotalDecim, unsigned int APreDecim, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	virtual ~TSlidingBPFilterIIR(){};
    // Тип фильтра TFZP_XXX
    virtual unsigned int GetType();
    // Порядок фильтра
    virtual unsigned int GetOrder();
    // Величина децимации на выходе фильтра
    virtual unsigned int GetDecim();
	// Устанавливает новое значение коэффициента коррекции смещения величины FShiftCoef
	virtual void SetShiftCoefCorrection(TDSPE_LongDouble AShiftCoefCorrection);
/*	// Очистка фильтра и его внутренних буферов
	virtual void Clear();
    // Тип фильтра TFZP_XXX
    virtual unsigned int GetType();
    // Размер буфера
    virtual unsigned int GetSizeBuffer();
    // Длина буфера
    virtual unsigned int GetBufferLength();
    // Порядок фильтра
    virtual unsigned int GetOrder();
    // Величина децимации на выходе фильтра
    virtual unsigned int GetDecim();*/
    // Максимальная величина децимации данного фильтра данного типа
    virtual unsigned int GetMaxSingleDecim();
	// Функция фильтрации и децимации сигнала AInOutputSig длиной AInOutputSigLength.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	// virtual bool FilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength) = 0;
	// Функция фильтрации и децимации сигнала AInputSig длиной AInputSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(void* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC);
	// Функция фильтрации и децимации сигнала AInSig длиной AInSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Если AOutSig == AInSig, это означает, что при фильтрации не создавался
	// дополнительный выходной буфер. В противном случае, буфер под выходной сигнал 
	// AOutSig длиной AOutSigLength создается в теле функции и должен освобождаться вызывающей стороной.
	// После фильтрации в AOutSigLength возвращается полное ковличество сигнала в 
	// буфере AOutSig.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(void* AInSig, unsigned int AInSigLength, bool ANeedDecim, void* &AOutSig, unsigned int &AOutSigLength, unsigned char ATSDTC);

	virtual void Init();
    //virtual uint32_t GetInitLenght();
};
//---------------------------------------------------------------------------
// Класс, объявляющий основные свойства и методы ВЧ фильтра 
//---------------------------------------------------------------------------
class TCustomSlidingHPFilter
{
private:
protected:
	unsigned char FHPFType;
	int FSigBufferIndexY; // Текущий указатель в буфере выходных значений
	char* FSigBufferY;	// буфер IIR фильтра для хранения отсчетов выходных значений
	//virtual void SetResampleType(unsigned char AResampleType) = 0;
public:
	TCustomSlidingHPFilter(unsigned char AHPFType);
	virtual ~TCustomSlidingHPFilter();
};

//---------------------------------------------------------------------------
// Скользящий БИХ ФВЧ (с учетом предистории фильтрации)
// TF_SLIDING_HIGHPASS_IIR
//---------------------------------------------------------------------------
class TSlidingHPFilterIIR : public TCustomSlidingHPFilter, public TSlidingFilter
{
private:
protected:
	void* FAArrCoefs;		// Коэффициенты фильтра
	void* FZArrCoefs;		// Коэффициенты фильтра
	virtual void* GetBArrCoefs();
	virtual void* GetAArrCoefs();
	virtual void* GetZArrCoefs();
	// Функция, возвращающая реальное количество B-коеффициентов фильтра.
	virtual unsigned int GetRealCoefsCount();
	// Функция, возвращающая количество отсчетов входного сигнала, необходимых
	// для стабилизации сигнала на выходе.
	// В случае FIR фильтров это их порядок. В случае IIR фильтров - экспериментально
	// устанавливаемое значение для каждого фильтра.
	virtual unsigned int GetStabSigLength();
	// Устанавливает размер буферов фильтра
	virtual void SetSigBufferLength(unsigned int ASigBufferLength);
	// Устанавливает итоговый коэффициент децимации для нижележащего 
	// каскада фильтров. Данная величина должна быть степенью
	// двойки. Работает только для  обычных
	// децимирующих фильтров (ФНЧ). Для фильтров огибающей должна
	// быть перекрыта.
	virtual bool SetTotalDecimCurrentCascade(unsigned int ATotalDecim);
	// Устанавливает коэффициент децимации для текущего фильтра.
	// Данная величина должна быть степенью
	// двойки. Работает только для  обычных
	// децимирующих фильтров (ФНЧ). Для фильтров огибающей не работает.
	virtual bool SetDecim(unsigned int ADecim);
	// Устанавливает тип ВЧ фильтра
	virtual void SetHPFType(unsigned char AHPFType);

public:
	// Конструктор. 
	// AHPFType - код данного ВЧ фильтра. 
	TSlidingHPFilterIIR(unsigned char AHPFType, bool AOwnsNextCascade, unsigned char ATSDTC, unsigned char ATDE);
	virtual ~TSlidingHPFilterIIR(){};
    // Тип фильтра TFZP_XXX
    virtual unsigned int GetType();
    // Порядок фильтра
    virtual unsigned int GetOrder();
	// Код фильтра
	virtual unsigned int GetCode();
	// Величина децимации на выходе фильтра
	virtual unsigned int GetDecim();
	// Указывает на то, что при Decim == 1 фильтр работает,
	// это относится к ФВЧ, SP и т.д.
	virtual bool GetDecimOneMakesChanges(){return true;};
/*	// Очистка фильтра и его внутренних буферов
	virtual void Clear();
    // Тип фильтра TFZP_XXX
    virtual unsigned int GetType();
    // Размер буфера
    virtual unsigned int GetSizeBuffer();
    // Длина буфера
    virtual unsigned int GetBufferLength();
    // Порядок фильтра
    virtual unsigned int GetOrder();
    // Величина децимации на выходе фильтра
    virtual unsigned int GetDecim();*/
    // Максимальная величина децимации данного фильтра данного типа
    virtual unsigned int GetMaxSingleDecim();
	// Функция фильтрации и децимации сигнала AInOutputSig длиной AInOutputSigLength.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	// virtual bool FilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength);
	// Функция фильтрации и децимации сигнала AInputSig длиной AInputSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(void* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC);
	// Функция фильтрации и децимации сигнала AInSig длиной AInSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Если AOutSig == AInSig, это означает, что при фильтрации не создавался
	// дополнительный выходной буфер. В противном случае, буфер под выходной сигнал 
	// AOutSig длиной AOutSigLength создается в теле функции и должен освобождаться вызывающей стороной.
	// После фильтрации в AOutSigLength возвращается полное ковличество сигнала в 
	// буфере AOutSig.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(void* AInSig, unsigned int AInSigLength, bool ANeedDecim, void* &AOutSig, unsigned int &AOutSigLength, unsigned char ATSDTC);
	// Функция фильтрации и децимации сигнала AInputSig длиной AInputSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Тип входных-выходных данных TDSPE_LongDouble.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim);
	// Функция фильтрации и децимации сигнала AInputSig длиной AInputSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Тип входных-выходных данных int32_t и уточняется через параметр ATSDTC.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(int32_t* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC);
	virtual void Init();
};


//---------------------------------------------------------------------------
// Скользящий БИХ ФВЧ (с учетом предистории фильтрации)
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
	// Функция, возвращающая реальное количество B-коеффициентов фильтра.
	virtual unsigned int GetRealCoefsCount();
	// Функция, возвращающая количество отсчетов входного сигнала, необходимых
	// для стабилизации сигнала на выходе.
	// В случае FIR фильтров это их порядок. В случае IIR фильтров - экспериментально
	// устанавливаемое значение для каждого фильтра.
	virtual unsigned int GetStabSigLength();
	// Устанавливает размер буферов фильтра
	virtual void SetSigBufferLength(unsigned int ASigBufferLength);
	// Устанавливает итоговый коэффициент децимации для нижележащего 
	// каскада фильтров. Данная величина должна быть степенью
	// двойки. Работает только для  обычных
	// децимирующих фильтров (ФНЧ). Для фильтров огибающей должна
	// быть перекрыта.
	virtual bool SetTotalDecimCurrentCascade(unsigned int ATotalDecim);
	// Устанавливает коэффициент децимации для текущего фильтра.
	// Данная величина должна быть степенью
	// двойки. Работает только для  обычных
	// децимирующих фильтров (ФНЧ). Для фильтров огибающей не работает.
	virtual bool SetDecim(unsigned int ADecim);
	// Устанавливает тип ВЧ фильтра
	virtual void SetHPFType(unsigned char AHPFType);

public:
	// Конструктор. 
	// AAlfa - параметр фильтра 
	TSlidingHPFilterIIRFast(TDSPE_LongDouble AAlfa, bool AOwnsNextCascade, unsigned char ATSDTC, unsigned char ATDE);
	virtual ~TSlidingHPFilterIIRFast(){};
    // Тип фильтра TFZP_XXX
    virtual unsigned int GetType();
    // Порядок фильтра
    virtual unsigned int GetOrder();
	// Код фильтра
	virtual unsigned int GetCode();
	// Величина децимации на выходе фильтра
	virtual unsigned int GetDecim();
	// Указывает на то, что при Decim == 1 фильтр работает,
	// это относится к ФВЧ, SP и т.д.
	virtual bool GetDecimOneMakesChanges(){return true;};
/*	// Очистка фильтра и его внутренних буферов
	virtual void Clear();
    // Тип фильтра TFZP_XXX
    virtual unsigned int GetType();
    // Размер буфера
    virtual unsigned int GetSizeBuffer();
    // Длина буфера
    virtual unsigned int GetBufferLength();
    // Порядок фильтра
    virtual unsigned int GetOrder();
    // Величина децимации на выходе фильтра
    virtual unsigned int GetDecim();*/
    // Максимальная величина децимации данного фильтра данного типа
    virtual unsigned int GetMaxSingleDecim();
	// Функция фильтрации и децимации сигнала AInOutputSig длиной AInOutputSigLength.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	// virtual bool FilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength);
	// Функция фильтрации и децимации сигнала AInputSig длиной AInputSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(void* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC);
	// Функция фильтрации и децимации сигнала AInSig длиной AInSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Если AOutSig == AInSig, это означает, что при фильтрации не создавался
	// дополнительный выходной буфер. В противном случае, буфер под выходной сигнал 
	// AOutSig длиной AOutSigLength создается в теле функции и должен освобождаться вызывающей стороной.
	// После фильтрации в AOutSigLength возвращается полное ковличество сигнала в 
	// буфере AOutSig.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(void* AInSig, unsigned int AInSigLength, bool ANeedDecim, void* &AOutSig, unsigned int &AOutSigLength, unsigned char ATSDTC);
	// Функция фильтрации и децимации сигнала AInputSig длиной AInputSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Тип входных-выходных данных TDSPE_LongDouble.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim);
	// Функция фильтрации и децимации сигнала AInputSig длиной AInputSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Тип входных-выходных данных int32_t и уточняется через параметр ATSDTC.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(int32_t* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC);
	virtual void Init();
};


//---------------------------------------------------------------------------
// Класс, объявляющий основные свойства и методы 
// cкользящего специального фильтра.
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
// Скользящий (с учетом предистории фильтрации) специальный (SP) биквадратный (BQ) БИХ фильтр.
// TF_SLIDING_SPECFILTER_IIRBQ
//---------------------------------------------------------------------------
class TSlidingSPFilterIIRBQ : public TCustomSlidingSPFilter, public TSlidingFilter
{
private:
protected:
	void* FAArrCoefs;		// Коэффициенты фильтра
	void* FZArrCoefs;		// Коэффициенты фильтра
	void* FGArrCoefs;		// Коэффициенты фильтра
	unsigned int FSectionCnt;
	virtual void* GetBArrCoefs();
	virtual void* GetAArrCoefs();
	virtual void* GetZArrCoefs();
	virtual void* GetGArrCoefs();
	// Функция, возвращающая реальное количество B-коеффициентов фильтра.
	virtual unsigned int GetRealCoefsCount();
	// Функция, возвращающая количество отсчетов входного сигнала, необходимых
	// для стабилизации сигнала на выходе.
	// В случае FIR фильтров это их порядок. В случае IIR фильтров - экспериментально
	// устанавливаемое значение для каждого фильтра.
	virtual unsigned int GetStabSigLength();
	// Устанавливает размер буферов фильтра
	//virtual void SetSigBufferLength(unsigned int ASigBufferLength);
	// Устанавливает итоговый коэффициент децимации для нижележащего 
	// каскада фильтров. Данная величина должна быть степенью
	// двойки. Работает только для  обычных
	// децимирующих фильтров (ФНЧ). Для фильтров огибающей должна
	// быть перекрыта.
	virtual bool SetTotalDecimCurrentCascade(unsigned int ATotalDecim);
	// Устанавливает коэффициент децимации для текущего фильтра.
	// Данная величина должна быть степенью
	// двойки. Работает только для  обычных
	// децимирующих фильтров (ФНЧ). Для фильтров огибающей не работает.
	virtual bool SetDecim(unsigned int ADecim);
	// Устанавливает тип SP фильтра
	virtual void SetSPFType(unsigned char ASPFType);

public:
	// Конструктор. 
	// ASPFType - код данного SP фильтра. 
	TSlidingSPFilterIIRBQ(unsigned char ASPFType, bool AOwnsNextCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	virtual ~TSlidingSPFilterIIRBQ(){};
    // Тип фильтра TFZP_XXX
    virtual unsigned int GetType();
    // Порядок фильтра
    virtual unsigned int GetOrder();
	// Код фильтра
	virtual unsigned int GetCode();
	// Величина децимации на выходе фильтра
	virtual unsigned int GetDecim();
	// Указывает на то, что при Decim == 1 фильтр работает,
	// это относится к ФВЧ, SP и т.д.
	virtual bool GetDecimOneMakesChanges(){return true;};

/*	// Очистка фильтра и его внутренних буферов
	virtual void Clear();
    // Тип фильтра TFZP_XXX
    virtual unsigned int GetType();
    // Размер буфера
    virtual unsigned int GetSizeBuffer();
    // Длина буфера
    virtual unsigned int GetBufferLength();
    // Порядок фильтра
    virtual unsigned int GetOrder();
    // Величина децимации на выходе фильтра
    virtual unsigned int GetDecim();*/
    // Максимальная величина децимации данного фильтра данного типа
    virtual unsigned int GetMaxSingleDecim();
	// Функция фильтрации и децимации сигнала AInOutputSig длиной AInOutputSigLength.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	// virtual bool FilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength);
	// Функция фильтрации и децимации сигнала AInputSig длиной AInputSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(void* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC);
	// Функция фильтрации и децимации сигнала AInSig длиной AInSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Если AOutSig == AInSig, это означает, что при фильтрации не создавался
	// дополнительный выходной буфер. В противном случае, буфер под выходной сигнал 
	// AOutSig длиной AOutSigLength создается в теле функции и должен освобождаться вызывающей стороной.
	// После фильтрации в AOutSigLength возвращается полное ковличество сигнала в 
	// буфере AOutSig.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(void* AInSig, unsigned int AInSigLength, bool ANeedDecim, void* &AOutSig, unsigned int &AOutSigLength, unsigned char ATSDTC);
	// Функция фильтрации и децимации сигнала AInputSig длиной AInputSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Тип входных-выходных данных TDSPE_LongDouble.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim);
	// Функция фильтрации и децимации сигнала AInputSig длиной AInputSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Тип входных-выходных данных int32_t и уточняется через параметр ATSDTC.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(int32_t* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC);
};

//---------------------------------------------------------------------------
// Скользящий (с учетом предистории фильтрации) специальный (SP) биквадратный (BQ) БИХ фильтр.
// TF_SLIDING_HIGHPASS_IIRBQ
//---------------------------------------------------------------------------
class TSlidingHPFilterIIRBQ : public TCustomSlidingHPFilter, public TSlidingFilter
{
private:
protected:
	void* FAArrCoefs;		// Коэффициенты фильтра
	void* FZArrCoefs;		// Коэффициенты фильтра
	void* FGArrCoefs;		// Коэффициенты фильтра
	unsigned int FSectionCnt;
	virtual void* GetBArrCoefs();
	virtual void* GetAArrCoefs();
	virtual void* GetZArrCoefs();
	virtual void* GetGArrCoefs();
	// Функция, возвращающая реальное количество B-коеффициентов фильтра.
	virtual unsigned int GetRealCoefsCount();
	// Функция, возвращающая количество отсчетов входного сигнала, необходимых
	// для стабилизации сигнала на выходе.
	// В случае FIR фильтров это их порядок. В случае IIR фильтров - экспериментально
	// устанавливаемое значение для каждого фильтра.
	virtual unsigned int GetStabSigLength();
	// Устанавливает размер буферов фильтра
	//virtual void SetSigBufferLength(unsigned int ASigBufferLength);
	// Устанавливает итоговый коэффициент децимации для нижележащего 
	// каскада фильтров. Данная величина должна быть степенью
	// двойки. Работает только для  обычных
	// децимирующих фильтров (ФНЧ). Для фильтров огибающей должна
	// быть перекрыта.
	virtual bool SetTotalDecimCurrentCascade(unsigned int ATotalDecim);
	// Устанавливает коэффициент децимации для текущего фильтра.
	// Данная величина должна быть степенью
	// двойки. Работает только для  обычных
	// децимирующих фильтров (ФНЧ). Для фильтров огибающей не работает.
	virtual bool SetDecim(unsigned int ADecim);
	// Устанавливает тип ФВЧ
	virtual void SetHPFType(unsigned char ASPFType);

public:
	// Конструктор. 
	// AHPFType - код данного ФВЧ. 
	TSlidingHPFilterIIRBQ(unsigned char AHPFType, bool AOwnsNextCascade, unsigned char ATSDTC, unsigned char ATDE);
	virtual ~TSlidingHPFilterIIRBQ(){};
    // Тип фильтра TFZP_XXX
    virtual unsigned int GetType();
    // Порядок фильтра
    virtual unsigned int GetOrder();
	// Код фильтра
	virtual unsigned int GetCode();
	// Величина децимации на выходе фильтра
	virtual unsigned int GetDecim();
	// Указывает на то, что при Decim == 1 фильтр работает,
	// это относится к ФВЧ, SP и т.д.
	virtual bool GetDecimOneMakesChanges(){return true;};

/*	// Очистка фильтра и его внутренних буферов
	virtual void Clear();
    // Тип фильтра TFZP_XXX
    virtual unsigned int GetType();
    // Размер буфера
    virtual unsigned int GetSizeBuffer();
    // Длина буфера
    virtual unsigned int GetBufferLength();
    // Порядок фильтра
    virtual unsigned int GetOrder();
    // Величина децимации на выходе фильтра
    virtual unsigned int GetDecim();*/
    // Максимальная величина децимации данного фильтра данного типа
    virtual unsigned int GetMaxSingleDecim();
	// Функция фильтрации и децимации сигнала AInOutputSig длиной AInOutputSigLength.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	// virtual bool FilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength);
	// Функция фильтрации и децимации сигнала AInputSig длиной AInputSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(void* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC);
	// Функция фильтрации и децимации сигнала AInSig длиной AInSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Если AOutSig == AInSig, это означает, что при фильтрации не создавался
	// дополнительный выходной буфер. В противном случае, буфер под выходной сигнал 
	// AOutSig длиной AOutSigLength создается в теле функции и должен освобождаться вызывающей стороной.
	// После фильтрации в AOutSigLength возвращается полное ковличество сигнала в 
	// буфере AOutSig.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(void* AInSig, unsigned int AInSigLength, bool ANeedDecim, void* &AOutSig, unsigned int &AOutSigLength, unsigned char ATSDTC);
	// Функция фильтрации и децимации сигнала AInputSig длиной AInputSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Тип входных-выходных данных TDSPE_LongDouble.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim);
	// Функция фильтрации и децимации сигнала AInputSig длиной AInputSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Тип входных-выходных данных int32_t и уточняется через параметр ATSDTC.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(int32_t* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC);
};


//---------------------------------------------------------------------------
// Скользящий (с учетом предистории фильтрации) специальный (SP) биквадратный (BQ) БИХ фильтр.
// TF_SLIDING_SPECFILTER_FIR
//---------------------------------------------------------------------------
class TSlidingSPFilterFIR : public TCustomSlidingSPFilter, public TSlidingFilter
{
private:
protected:
	virtual void* GetBArrCoefs();
	virtual void* GetAArrCoefs();
	virtual void* GetZArrCoefs();
	// Функция, возвращающая реальное количество B-коеффициентов фильтра.
	virtual unsigned int GetRealCoefsCount();
	// Функция, возвращающая количество отсчетов входного сигнала, необходимых
	// для стабилизации сигнала на выходе.
	// В случае FIR фильтров это их порядок. В случае IIR фильтров - экспериментально
	// устанавливаемое значение для каждого фильтра.
	//virtual unsigned int GetStabSigLength();
	// Устанавливает размер буферов фильтра
	//virtual void SetSigBufferLength(unsigned int ASigBufferLength);
	// Устанавливает итоговый коэффициент децимации для нижележащего 
	// каскада фильтров. Данная величина должна быть степенью
	// двойки. Работает только для  обычных
	// децимирующих фильтров (ФНЧ). Для фильтров огибающей должна
	// быть перекрыта.
	virtual bool SetTotalDecimCurrentCascade(unsigned int ATotalDecim);
	// Устанавливает коэффициент децимации для текущего фильтра.
	// Данная величина должна быть степенью
	// двойки. Работает только для  обычных
	// децимирующих фильтров (ФНЧ). Для фильтров огибающей не работает.
	virtual bool SetDecim(unsigned int ADecim);
	// Устанавливает тип SP фильтра
	virtual void SetSPFType(unsigned char ASPFType);

public:
	// Конструктор. 
	// ASPFType - код данного SP фильтра. 
	TSlidingSPFilterFIR(unsigned char ASPFType, bool AOwnsNextCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	virtual ~TSlidingSPFilterFIR(){};
    // Тип фильтра TFZP_XXX
    virtual unsigned int GetType();
    // Порядок фильтра
    virtual unsigned int GetOrder();
	// Код фильтра
	virtual unsigned int GetCode();
	// Величина децимации на выходе фильтра
	virtual unsigned int GetDecim();
	// Указывает на то, что при Decim == 1 фильтр работает,
	// это относится к ФВЧ, SP и т.д.
	virtual bool GetDecimOneMakesChanges(){return true;};

	// Величина дополнительного смещения вычисленных значений вправо
	// при работе с фиксированной точкой для приведения коэффициента
	// усиления фильтра к 1.
	//virtual unsigned int GetAddShift();

/*	// Очистка фильтра и его внутренних буферов
	virtual void Clear();
    // Тип фильтра TFZP_XXX
    virtual unsigned int GetType();
    // Размер буфера
    virtual unsigned int GetSizeBuffer();
    // Длина буфера
    virtual unsigned int GetBufferLength();
    // Порядок фильтра
    virtual unsigned int GetOrder();
    // Величина децимации на выходе фильтра
    virtual unsigned int GetDecim();*/
	// Величина дополнительного смещения вычисленных значений вправо
	// при работе с фиксированной точкой для приведения коэффициента
	// усиления фильтра к 1.
	virtual unsigned int GetAddShift();
    // Максимальная величина децимации данного фильтра данного типа
    virtual unsigned int GetMaxSingleDecim();
	// Функция фильтрации и децимации сигнала AInOutputSig длиной AInOutputSigLength.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	// virtual bool FilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength);
	// Функция фильтрации и децимации сигнала AInputSig длиной AInputSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(void* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC);
	// Функция фильтрации и децимации сигнала AInSig длиной AInSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Если AOutSig == AInSig, это означает, что при фильтрации не создавался
	// дополнительный выходной буфер. В противном случае, буфер под выходной сигнал 
	// AOutSig длиной AOutSigLength создается в теле функции и должен освобождаться вызывающей стороной.
	// После фильтрации в AOutSigLength возвращается полное ковличество сигнала в 
	// буфере AOutSig.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(void* AInSig, unsigned int AInSigLength, bool ANeedDecim, void* &AOutSig, unsigned int &AOutSigLength, unsigned char ATSDTC);
	// Функция фильтрации и децимации сигнала AInputSig длиной AInputSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Тип входных-выходных данных TDSPE_LongDouble.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim);
	// Функция фильтрации и децимации сигнала AInputSig длиной AInputSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Тип входных-выходных данных int32_t и уточняется через параметр ATSDTC.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(int32_t* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC);
};

//---------------------------------------------------------------------------
// Класс, объявляющий основные свойства и методы 
// cкользящего интегрирующего фильтра.
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
// Скользящий интегрирующий фильтр фильтр.
// TF_SLIDING_INTGFILTER_TRP
//---------------------------------------------------------------------------
class TSlidingINTGFilterTRP : public TCustomSlidingINTGFilter, public TSlidingFilter
{
private:
protected:
	virtual void* GetBArrCoefs();
	virtual void* GetAArrCoefs();
	virtual void* GetZArrCoefs();
	// Функция, возвращающая реальное количество B-коеффициентов фильтра.
	virtual unsigned int GetRealCoefsCount();
	// Функция, возвращающая количество отсчетов входного сигнала, необходимых
	// для стабилизации сигнала на выходе.
	// В случае FIR фильтров это их порядок. В случае IIR фильтров - экспериментально
	// устанавливаемое значение для каждого фильтра.
	//virtual unsigned int GetStabSigLength();
	// Устанавливает размер буферов фильтра
	//virtual void SetSigBufferLength(unsigned int ASigBufferLength);
	// Устанавливает итоговый коэффициент децимации для нижележащего 
	// каскада фильтров. Данная величина должна быть степенью
	// двойки. Работает только для  обычных
	// децимирующих фильтров (ФНЧ). Для фильтров огибающей должна
	// быть перекрыта.
	virtual bool SetTotalDecimCurrentCascade(unsigned int ATotalDecim);
	// Устанавливает коэффициент децимации для текущего фильтра.
	// Данная величина должна быть степенью
	// двойки. Работает только для  обычных
	// децимирующих фильтров (ФНЧ). Для фильтров огибающей не работает.
	virtual bool SetDecim(unsigned int ADecim);
	// Устанавливает тип SP фильтра
	virtual void SetINTGFType(unsigned char AINTGFType);

public:
	// Конструктор. 
	// ASPFType - код данного SP фильтра. 
	TSlidingINTGFilterTRP(unsigned char ASPFType, bool AOwnsNextCascade, unsigned char ATSDTC, unsigned char ATDE);
	virtual ~TSlidingINTGFilterTRP(){};
    // Тип фильтра TFZP_XXX
    virtual unsigned int GetType();
    // Порядок фильтра
    virtual unsigned int GetOrder();
	// Код фильтра
	virtual unsigned int GetCode();
	// Величина децимации на выходе фильтра
	virtual unsigned int GetDecim();
	// Величина дополнительного смещения вычисленных значений вправо
	// при работе с фиксированной точкой для приведения коэффициента
	// усиления фильтра к 1.
	//virtual unsigned int GetAddShift();
	// Указывает на то, что при Decim == 1 фильтр работает,
	// это относится к ФВЧ, SP и т.д.
	virtual bool GetDecimOneMakesChanges(){return true;};

/*	// Очистка фильтра и его внутренних буферов
	virtual void Clear();
    // Тип фильтра TFZP_XXX
    virtual unsigned int GetType();
    // Размер буфера
    virtual unsigned int GetSizeBuffer();
    // Длина буфера
    virtual unsigned int GetBufferLength();
    // Порядок фильтра
    virtual unsigned int GetOrder();
    // Величина децимации на выходе фильтра
    virtual unsigned int GetDecim();*/
	// Величина дополнительного смещения вычисленных значений вправо
	// при работе с фиксированной точкой для приведения коэффициента
	// усиления фильтра к 1.
	//virtual unsigned int GetAddShift();
    // Максимальная величина децимации данного фильтра данного типа
    virtual unsigned int GetMaxSingleDecim();
	// Функция фильтрации и децимации сигнала AInOutputSig длиной AInOutputSigLength.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	// virtual bool FilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength);
	// Функция фильтрации и децимации сигнала AInputSig длиной AInputSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(void* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC);
	// Функция фильтрации и децимации сигнала AInSig длиной AInSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Если AOutSig == AInSig, это означает, что при фильтрации не создавался
	// дополнительный выходной буфер. В противном случае, буфер под выходной сигнал 
	// AOutSig длиной AOutSigLength создается в теле функции и должен освобождаться вызывающей стороной.
	// После фильтрации в AOutSigLength возвращается полное ковличество сигнала в 
	// буфере AOutSig.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(void* AInSig, unsigned int AInSigLength, bool ANeedDecim, void* &AOutSig, unsigned int &AOutSigLength, unsigned char ATSDTC);
	// Функция фильтрации и децимации сигнала AInputSig длиной AInputSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Тип входных-выходных данных TDSPE_LongDouble.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim);
	// Функция фильтрации и децимации сигнала AInputSig длиной AInputSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Тип входных-выходных данных int32_t и уточняется через параметр ATSDTC.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(int32_t* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC);
};

//--------------------------------------------------------------------------
// Базовый класс для всех фильтров, обрабатывающих за раз буфер данных (без учета предистории фильтрации)
//---------------------------------------------------------------------------
class THeapFilter : public TCustomFilter
{
private:
protected:

	void* FBArrCoefs;		// Коэффициенты фильтра
	// Устанавливает коэффициент децимации для текущего фильтра.
	// Данная величина должна быть степенью
	// двойки. Работает только для  обычных
	// децимирующих фильтров (ФНЧ). Для фильтров огибающей не работает.
	virtual bool SetDecim(unsigned int ADecim);
public:
	// Флаг, говорящий о том, что для фильтрации требуется весь
	// набор данных. 
	// Для SlidingFIlter можно фильтровать по частям, а вот для HeapFilter
	// лучше фильровать весь набор данных.
	virtual bool GetNeadAllData(){return true;};
	// Флаг, говорящий о том, что фильтр имеет свой внутренний буфер для фильтрации,
	// т.е. для он помнит часть данных предыдущего сигнала для того, чтобы получать
	// новые отфильтрованные значения на выходе.
	// Если true - имеет.
	// Если false - не имеет, и поэтому новая фильтрация никак не зависит от 
	// фильтрации предыдущего сигнала.
	virtual bool GetIsBufferingFilter(){return false;};
	// Конструктор. AOwnsNextCascade == true указывает
	// на то, что за следующий каскад отвечает этот фильтр
	// (следующий каскад будет удален данным фильтром самостоятельно)
	// ADecim - дребуемая децимация данного фильтра == степень двойки. Если
	// ADecim > GetMaxSingleDecim(), то Decim == GetMaxSingleDecim().
	// Если ATotalDecim не степень двойки, то требуемая
	// децимация не будет достигнута и будет равна 0.
	THeapFilter(unsigned int ADecim, bool AOwnsNextCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	// Конструктор. В нем по умолчанию принимается FАOwnsNextCascade == true,
	// что указывает на то, что за следующий каскад отвечает этот фильтр
	// (следующий каскад будет удален данным фильтром самостоятельно)
	// ATotalDecim - дребуемая децимация фильтра == степень двойки. Если
	// ATotalDecim > 32, то создается каскад фильтров, чтобы добиться
	// требуемой децимации. Если ATotalDecim не степень двойки, то требуемая
	// децимация не будет достигнута и будет равна 0.
	THeapFilter(unsigned int ATotalDecim, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	// Конструктор. нем по умолчанию принимается FАOwnsNextCascade == true,
	// что указывает на то, что за следующий каскад отвечает этот фильтр
	// (следующий каскад будет удален данным фильтром самостоятельно)
	// APrevCascade - указывает на предыдущий каскад фильтрации.
	// ATotalDecim - дребуемая децимация фильтра == степень двойки. Если
	// ATotalDecim > 32, то создается каскад фильтров, чтобы добиться
	// требуемой децимации. Если ATotalDecim не степень двойки, то требуемая
	// децимация не будет достигнута и будет равна 0.
	THeapFilter(unsigned int ATotalDecim, THeapFilter* APrevCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	virtual ~THeapFilter(){};


/*	// Очистка фильтра и его внутренних буферов
	virtual void Clear();
    // Тип фильтра TFZP_XXX
    virtual unsigned int GetType();
    // Размер буфера
    virtual unsigned int GetSizeBuffer();
    // Длина буфера
    virtual unsigned int GetBufferLength();
    // Порядок фильтра
    virtual unsigned int GetOrder();
    // Величина децимации на выходе фильтра
    virtual unsigned int GetDecim();*/
	// Функция фильтрации и децимации сигнала AInOutputSig длиной AInOutputSigLength.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	// virtual bool FilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength) = 0;
	// Функция фильтрации и децимации сигнала AInputSig длиной AInputSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	// virtual bool SingleFilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim) = 0;
	virtual void Init();
	virtual uint32_t GetInitLenght();
};
//---------------------------------------------------------------------------
// Обрабатывающий за раз буфер данных КИХ ФНЧ (без учета предистории фильтрации)
// TF_HEAP_LOWPASS_FIR
//---------------------------------------------------------------------------
class THeapLPFilterFIR : public THeapFilter
{
private:
protected:
	virtual void* GetBArrCoefs();
	virtual void* GetAArrCoefs();
	virtual void* GetZArrCoefs();
	// Функция, возвращающая реальное количество B-коеффициентов фильтра.
	virtual unsigned int GetRealCoefsCount();
	// Устанавливает итоговый коэффициент децимации для нижележащего 
	// каскада фильтров. Данная величина должна быть степенью
	// двойки. Работает только для  обычных
	// децимирующих фильтров (ФНЧ). Для фильтров огибающей должна
	// быть перекрыта.
	virtual bool SetTotalDecimCurrentCascade(unsigned int ATotalDecim);
public:
	// Конструктор. AOwnsNextCascade == true указывает
	// на то, что за следующий каскад отвечает этот фильтр
	// (следующий каскад будет удален данным фильтром самостоятельно)
	// ADecim - дребуемая децимация данного фильтра == степень двойки. Если
	// ADecim > GetMaxSingleDecim(), то Decim == GetMaxSingleDecim().
	// Если ATotalDecim не степень двойки, то требуемая
	// децимация не будет достигнута и будет равна 0.
	THeapLPFilterFIR(unsigned int ADecim, bool AOwnsNextCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	// Конструктор. В нем по умолчанию принимается FАOwnsNextCascade == true,
	// что указывает на то, что за следующий каскад отвечает этот фильтр
	// (следующий каскад будет удален данным фильтром самостоятельно)
	// ATotalDecim - дребуемая децимация фильтра == степень двойки. Если
	// ATotalDecim > 32, то создается каскад фильтров, чтобы добиться
	// требуемой децимации. Если ATotalDecim не степень двойки, то требуемая
	// децимация не будет достигнута и будет равна 0.
	THeapLPFilterFIR(unsigned int ATotalDecim, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	// Конструктор. нем по умолчанию принимается FАOwnsNextCascade == true,
	// что указывает на то, что за следующий каскад отвечает этот фильтр
	// (следующий каскад будет удален данным фильтром самостоятельно)
	// APrevCascade - указывает на предыдущий каскад фильтрации.
	// ATotalDecim - дребуемая децимация фильтра == степень двойки. Если
	// ATotalDecim > 32, то создается каскад фильтров, чтобы добиться
	// требуемой децимации. Если ATotalDecim не степень двойки, то требуемая
	// децимация не будет достигнута и будет равна 0.
	THeapLPFilterFIR(unsigned int ATotalDecim, THeapFilter* APrevCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	virtual ~THeapLPFilterFIR(){};
    // Тип фильтра TFZP_XXX
    virtual unsigned int GetType();
    // Порядок фильтра
    virtual unsigned int GetOrder();

/*	// Очистка фильтра и его внутренних буферов
	virtual void Clear();
    // Тип фильтра TFZP_XXX
    virtual unsigned int GetType();
    // Размер буфера
    virtual unsigned int GetSizeBuffer();
    // Длина буфера
    virtual unsigned int GetBufferLength();
    // Порядок фильтра
    virtual unsigned int GetOrder();
    // Величина децимации на выходе фильтра
    virtual unsigned int GetDecim();*/
    // Максимальная величина децимации данного фильтра данного типа
    virtual unsigned int GetMaxSingleDecim();
	// Функция фильтрации и децимации сигнала AInOutputSig длиной AInOutputSigLength.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	//virtual bool FilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength);
	// Функция фильтрации и децимации сигнала AInputSig длиной AInputSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(void* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC);
	// Функция фильтрации и децимации сигнала AInSig длиной AInSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Если AOutSig == AInSig, это означает, что при фильтрации не создавался
	// дополнительный выходной буфер. В противном случае, буфер под выходной сигнал 
	// AOutSig длиной AOutSigLength создается в теле функции и должен освобождаться вызывающей стороной.
	// После фильтрации в AOutSigLength возвращается полное ковличество сигнала в 
	// буфере AOutSig.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(void* AInSig, unsigned int AInSigLength, bool ANeedDecim, void* &AOutSig, unsigned int &AOutSigLength, unsigned char ATSDTC);

};
//---------------------------------------------------------------------------
// Обрабатывающий за раз буфер данных БИХ ФНЧ (без учета предистории фильтрации)
// TF_HEAP_LOWPASS_IIR
//---------------------------------------------------------------------------
class THeapLPFilterIIR : public THeapFilter
{
private:
protected:
	void* FAArrCoefs;		// Коэффициенты фильтра
	void* FZArrCoefs;		// Коэффициенты фильтра
	virtual void* GetBArrCoefs();
	virtual void* GetAArrCoefs();
	virtual void* GetZArrCoefs();
	// Функция, возвращающая реальное количество B-коеффициентов фильтра.
	virtual unsigned int GetRealCoefsCount();
	// Устанавливает итоговый коэффициент децимации для нижележащего 
	// каскада фильтров. Данная величина должна быть степенью
	// двойки. Работает только для  обычных
	// децимирующих фильтров (ФНЧ). Для фильтров огибающей должна
	// быть перекрыта.
	virtual bool SetTotalDecimCurrentCascade(unsigned int ATotalDecim);
	// Устанавливает коэффициент децимации для текущего фильтра.
	// Данная величина должна быть степенью
	// двойки. Работает только для  обычных
	// децимирующих фильтров (ФНЧ). Для фильтров огибающей не работает.
	virtual bool SetDecim(unsigned int ADecim);

public:
	// Конструктор. AOwnsNextCascade == true указывает
	// на то, что за следующий каскад отвечает этот фильтр
	// (следующий каскад будет удален данным фильтром самостоятельно)
	// ADecim - дребуемая децимация данного фильтра == степень двойки. Если
	// ADecim > GetMaxSingleDecim(), то Decim == GetMaxSingleDecim().
	// Если ATotalDecim не степень двойки, то требуемая
	// децимация не будет достигнута и будет равна 0.
	THeapLPFilterIIR(unsigned int ADecim, bool AOwnsNextCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	// Конструктор. В нем по умолчанию принимается FАOwnsNextCascade == true,
	// что указывает на то, что за следующий каскад отвечает этот фильтр
	// (следующий каскад будет удален данным фильтром самостоятельно)
	// ATotalDecim - дребуемая децимация фильтра == степень двойки. Если
	// ATotalDecim > 32, то создается каскад фильтров, чтобы добиться
	// требуемой децимации. Если ATotalDecim не степень двойки, то требуемая
	// децимация не будет достигнута и будет равна 0.
	THeapLPFilterIIR(unsigned int ATotalDecim, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	// Конструктор. нем по умолчанию принимается FАOwnsNextCascade == true,
	// что указывает на то, что за следующий каскад отвечает этот фильтр
	// (следующий каскад будет удален данным фильтром самостоятельно)
	// APrevCascade - указывает на предыдущий каскад фильтрации.
	// ATotalDecim - дребуемая децимация фильтра == степень двойки. Если
	// ATotalDecim > 32, то создается каскад фильтров, чтобы добиться
	// требуемой децимации. Если ATotalDecim не степень двойки, то требуемая
	// децимация не будет достигнута и будет равна 0.
	THeapLPFilterIIR(unsigned int ATotalDecim, THeapFilter* APrevCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	virtual ~THeapLPFilterIIR(){};
    // Тип фильтра TFZP_XXX
    virtual unsigned int GetType();
    // Порядок фильтра
    virtual unsigned int GetOrder();

/*	// Очистка фильтра и его внутренних буферов
	virtual void Clear();
    // Тип фильтра TFZP_XXX
    virtual unsigned int GetType();
    // Размер буфера
    virtual unsigned int GetSizeBuffer();
    // Длина буфера
    virtual unsigned int GetBufferLength();
    // Порядок фильтра
    virtual unsigned int GetOrder();
    // Величина децимации на выходе фильтра
    virtual unsigned int GetDecim();*/
    // Максимальная величина децимации данного фильтра данного типа
    virtual unsigned int GetMaxSingleDecim();
	// Функция фильтрации и децимации сигнала AInOutputSig длиной AInOutputSigLength.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	// virtual bool FilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength);
	// Функция фильтрации и децимации сигнала AInputSig длиной AInputSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(void* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC);
	// Функция фильтрации и децимации сигнала AInSig длиной AInSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Если AOutSig == AInSig, это означает, что при фильтрации не создавался
	// дополнительный выходной буфер. В противном случае, буфер под выходной сигнал 
	// AOutSig длиной AOutSigLength создается в теле функции и должен освобождаться вызывающей стороной.
	// После фильтрации в AOutSigLength возвращается полное ковличество сигнала в 
	// буфере AOutSig.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(void* AInSig, unsigned int AInSigLength, bool ANeedDecim, void* &AOutSig, unsigned int &AOutSigLength, unsigned char ATSDTC);

};
//---------------------------------------------------------------------------
// Обрабатывающий за раз буфер данных КИХ ФНЧ с отсутствием искажения фазы сигнала (без учета предистории фильтрации)
// TF_HEAP_LOWPASS_FIR_ZP
//---------------------------------------------------------------------------
class THeapLPFilterFIR_ZP : public THeapFilter
{
private:
protected:
	virtual void* GetBArrCoefs();
	virtual void* GetAArrCoefs();
	virtual void* GetZArrCoefs();
	// Функция, возвращающая реальное количество B-коеффициентов фильтра.
	virtual unsigned int GetRealCoefsCount();
	// Устанавливает итоговый коэффициент децимации для нижележащего 
	// каскада фильтров. Данная величина должна быть степенью
	// двойки. Работает только для  обычных
	// децимирующих фильтров (ФНЧ). Для фильтров огибающей должна
	// быть перекрыта.
	virtual bool SetTotalDecimCurrentCascade(unsigned int ATotalDecim);
public:
	// Конструктор. AOwnsNextCascade == true указывает
	// на то, что за следующий каскад отвечает этот фильтр
	// (следующий каскад будет удален данным фильтром самостоятельно)
	// ADecim - дребуемая децимация данного фильтра == степень двойки. Если
	// ADecim > GetMaxSingleDecim(), то Decim == GetMaxSingleDecim().
	// Если ATotalDecim не степень двойки, то требуемая
	// децимация не будет достигнута и будет равна 0.
	THeapLPFilterFIR_ZP(unsigned int ADecim, bool AOwnsNextCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	// Конструктор. В нем по умолчанию принимается FАOwnsNextCascade == true,
	// что указывает на то, что за следующий каскад отвечает этот фильтр
	// (следующий каскад будет удален данным фильтром самостоятельно)
	// ATotalDecim - дребуемая децимация фильтра == степень двойки. Если
	// ATotalDecim > 32, то создается каскад фильтров, чтобы добиться
	// требуемой децимации. Если ATotalDecim не степень двойки, то требуемая
	// децимация не будет достигнута и будет равна 0.
	THeapLPFilterFIR_ZP(unsigned int ATotalDecim, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	// Конструктор. нем по умолчанию принимается FАOwnsNextCascade == true,
	// что указывает на то, что за следующий каскад отвечает этот фильтр
	// (следующий каскад будет удален данным фильтром самостоятельно)
	// APrevCascade - указывает на предыдущий каскад фильтрации.
	// ATotalDecim - дребуемая децимация фильтра == степень двойки. Если
	// ATotalDecim > 32, то создается каскад фильтров, чтобы добиться
	// требуемой децимации. Если ATotalDecim не степень двойки, то требуемая
	// децимация не будет достигнута и будет равна 0.
	THeapLPFilterFIR_ZP(unsigned int ATotalDecim, THeapFilter* APrevCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	virtual ~THeapLPFilterFIR_ZP(){};
    // Тип фильтра TFZP_XXX
    virtual unsigned int GetType();
    // Порядок фильтра
    virtual unsigned int GetOrder();

/*	// Очистка фильтра и его внутренних буферов
	virtual void Clear();
    // Тип фильтра TFZP_XXX
    virtual unsigned int GetType();
    // Размер буфера
    virtual unsigned int GetSizeBuffer();
    // Длина буфера
    virtual unsigned int GetBufferLength();
    // Порядок фильтра
    virtual unsigned int GetOrder();
    // Величина децимации на выходе фильтра
    virtual unsigned int GetDecim();*/
    // Максимальная величина децимации данного фильтра данного типа
    virtual unsigned int GetMaxSingleDecim();
	// Функция фильтрации и децимации сигнала AInOutputSig длиной AInOutputSigLength.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	//virtual bool FilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength);
	// Функция фильтрации и децимации сигнала AInputSig длиной AInputSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(void* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC);
	// Функция фильтрации и децимации сигнала AInSig длиной AInSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Если AOutSig == AInSig, это означает, что при фильтрации не создавался
	// дополнительный выходной буфер. В противном случае, буфер под выходной сигнал 
	// AOutSig длиной AOutSigLength создается в теле функции и должен освобождаться вызывающей стороной.
	// После фильтрации в AOutSigLength возвращается полное ковличество сигнала в 
	// буфере AOutSig.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(void* AInSig, unsigned int AInSigLength, bool ANeedDecim, void* &AOutSig, unsigned int &AOutSigLength, unsigned char ATSDTC);

};
//---------------------------------------------------------------------------
// Обрабатывающий за раз буфер данных БИХ ФНЧ с отсутствием искажения фазы сигнала (без учета предистории фильтрации)
// TF_HEAP_LOWPASS_IIR_ZP
//---------------------------------------------------------------------------
class THeapLPFilterIIR_ZP : public THeapFilter
{
private:
protected:
	void* FAArrCoefs;		// Коэффициенты фильтра
	void* FZArrCoefs;		// Коэффициенты фильтра
	virtual void* GetBArrCoefs();
	virtual void* GetAArrCoefs();
	virtual void* GetZArrCoefs();
	// Функция, возвращающая реальное количество B-коеффициентов фильтра.
	virtual unsigned int GetRealCoefsCount();
	// Устанавливает итоговый коэффициент децимации для нижележащего 
	// каскада фильтров. Данная величина должна быть степенью
	// двойки. Работает только для  обычных
	// децимирующих фильтров (ФНЧ). Для фильтров огибающей должна
	// быть перекрыта.
	virtual bool SetTotalDecimCurrentCascade(unsigned int ATotalDecim);
	// Устанавливает коэффициент децимации для текущего фильтра.
	// Данная величина должна быть степенью
	// двойки. Работает только для  обычных
	// децимирующих фильтров (ФНЧ). Для фильтров огибающей не работает.
	virtual bool SetDecim(unsigned int ADecim);

public:
	// Конструктор. AOwnsNextCascade == true указывает
	// на то, что за следующий каскад отвечает этот фильтр
	// (следующий каскад будет удален данным фильтром самостоятельно)
	// ADecim - дребуемая децимация данного фильтра == степень двойки. Если
	// ADecim > GetMaxSingleDecim(), то Decim == GetMaxSingleDecim().
	// Если ATotalDecim не степень двойки, то требуемая
	// децимация не будет достигнута и будет равна 0.
	THeapLPFilterIIR_ZP(unsigned int ADecim, bool AOwnsNextCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	// Конструктор. В нем по умолчанию принимается FАOwnsNextCascade == true,
	// что указывает на то, что за следующий каскад отвечает этот фильтр
	// (следующий каскад будет удален данным фильтром самостоятельно)
	// ATotalDecim - дребуемая децимация фильтра == степень двойки. Если
	// ATotalDecim > 32, то создается каскад фильтров, чтобы добиться
	// требуемой децимации. Если ATotalDecim не степень двойки, то требуемая
	// децимация не будет достигнута и будет равна 0.
	THeapLPFilterIIR_ZP(unsigned int ATotalDecim, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	// Конструктор. нем по умолчанию принимается FАOwnsNextCascade == true,
	// что указывает на то, что за следующий каскад отвечает этот фильтр
	// (следующий каскад будет удален данным фильтром самостоятельно)
	// APrevCascade - указывает на предыдущий каскад фильтрации.
	// ATotalDecim - дребуемая децимация фильтра == степень двойки. Если
	// ATotalDecim > 32, то создается каскад фильтров, чтобы добиться
	// требуемой децимации. Если ATotalDecim не степень двойки, то требуемая
	// децимация не будет достигнута и будет равна 0.
	THeapLPFilterIIR_ZP(unsigned int ATotalDecim, THeapFilter* APrevCascade, bool AUsualWidth, unsigned char ATSDTC, unsigned char ATDE);
	virtual ~THeapLPFilterIIR_ZP(){};
    // Тип фильтра TFZP_XXX
    virtual unsigned int GetType();
    // Порядок фильтра
    virtual unsigned int GetOrder();

/*	// Очистка фильтра и его внутренних буферов
	virtual void Clear();
    // Тип фильтра TFZP_XXX
    virtual unsigned int GetType();
    // Размер буфера
    virtual unsigned int GetSizeBuffer();
    // Длина буфера
    virtual unsigned int GetBufferLength();
    // Порядок фильтра
    virtual unsigned int GetOrder();
    // Величина децимации на выходе фильтра
    virtual unsigned int GetDecim();*/
    // Максимальная величина децимации данного фильтра данного типа
    virtual unsigned int GetMaxSingleDecim();
	// Функция фильтрации и децимации сигнала AInOutputSig длиной AInOutputSigLength.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	// virtual bool FilterSignal(TDSPE_LongDouble* &AInOutputSig, unsigned int &AInOutputSigLength);
	// Функция фильтрации и децимации сигнала AInputSig длиной AInputSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Если фильтрация проведена успешно, то в AInOutputSig возвращается выходной
	// отфильтрованный сигнал длиной AOutputSigLength.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(void* &AInOutputSig, unsigned int &AInOutputSigLength, bool ANeedDecim, unsigned char ATSDTC);
	// Функция фильтрации и децимации сигнала AInSig длиной AInSigLength только
	// данным фильтром без использования следующих каскадов с указанием того, нужна ли
	// в последствии децимация (прореживание отсчетов) сигнала.
	// Если AOutSig == AInSig, это означает, что при фильтрации не создавался
	// дополнительный выходной буфер. В противном случае, буфер под выходной сигнал 
	// AOutSig длиной AOutSigLength создается в теле функции и должен освобождаться вызывающей стороной.
	// После фильтрации в AOutSigLength возвращается полное ковличество сигнала в 
	// буфере AOutSig.
	// Результат на выходе:
	// true  - процедура выполнена нормально.
	// false - процедура провалена.
	virtual bool SingleFilterSignal(void* AInSig, unsigned int AInSigLength, bool ANeedDecim, void* &AOutSig, unsigned int &AOutSigLength, unsigned char ATSDTC);

};

//---------------------------------------------------------------------------






// Функция, которая является аналогом матлабовской filtfilt(B,A,X)
extern void FiltFilt( const TDSPE_LongDouble *B, const int BLength, const TDSPE_LongDouble* A,
              const int ALength, TDSPE_LongDouble* InOutSignal, int InOutSignalLength);

// Функция, которая является аналогом матлабовской filtfilt(B,A,X)
extern bool FiltFilt_Z( const TDSPE_LongDouble* B, const int BLength,
                 const TDSPE_LongDouble* A, const int ALength,
                 const TDSPE_LongDouble* Z, const int ZLength,
                 TDSPE_LongDouble* InOutSignal, int InOutSignalLength);

#endif