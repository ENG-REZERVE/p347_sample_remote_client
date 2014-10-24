//--------------------------------------------------------------------------
// Author: Sidorov E.N.
// Date: 03.07.2013.
// Модуль, в котором описаны классы, созданные производить 
// робастный анализ результатов измерений.
//--------------------------------------------------------------------------
#ifndef ROBASTANALISIS_H
#define ROBASTANALISIS_H
//--------------------------------------------------------------------------
#include <stdlib.h>
#include <memory.h>
#include "list.h"

//--------------------------------------------------------------------------
// Типы данных с плавающей точкой, используемые в робастном анализе.
//typedef long double TRB_LongDouble;
typedef float TRB_LongDouble;
//--------------------------------------------------------------------------
// Элемент списка значений для робастного анализа
//--------------------------------------------------------------------------
struct rbitem_t
{
	TRB_LongDouble ValueY;
	TRB_LongDouble ValueX;
	bool ValidExtern;
	bool Valid;
	bool ValidRough;
	void Clear()
	{
		memset(this, 0x00, sizeof(rbitem_t));
	}
	bool operator ==(const rbitem_t& b) const
	{
		return ValueY == b.ValueY && 
			ValueX == b.ValueX &&
			ValidExtern == b.ValidExtern &&
			Valid == b.Valid &&
			ValidRough == b.ValidRough;
	}
	bool operator !=(const rbitem_t& b) const
	{  return !(b == *this); }

	rbitem_t& operator =(rbitem_t& b)
	{
		this->ValueY = b.ValueY;
		this->ValueX = b.ValueX;
		this->ValidExtern = b.ValidExtern;
		this->Valid = b.Valid;
		this->ValidRough = b.ValidRough;
		return *this;
	}
};
//--------------------------------------------------------------------------
// Список значений для робастного анализа
//--------------------------------------------------------------------------
class TRBItemList
{
private:
	TList* FList;
	bool FOwner;
public:
	TRBItemList(bool AOwner);
	TRBItemList(const TRBItemList &b);
	~TRBItemList();
	void Clear(void);
	int Add(void* AItem);
	int Insert(int AIndex, void* AItem);
	int IndexOf(void* AItem);
	void Delete(int AIndex);
	int Count();
	rbitem_t* GetRBItem(int AIndex);
	int GetRBItemIndexByValueX(TRB_LongDouble AValueX);
	//void operator = (TRBItemList &b);
	TRBItemList& operator = (const TRBItemList &b);
};
//--------------------------------------------------------------------------
class TRBMedianQueueStat
{
private:
protected:
	TRBItemList*	FRBItemList;			// Массив наблюдений за измеряемой величиной
	TRBItemList*	FRBItemListSorted;		// Упорядоченный массив наблюдений за измеряемой величиной
	bool			FGenValueX;				// Флаг, говорящий о том, что необходимо в объекте
											// самостоятельно генерировать значения ValueX при добавлении 
											// нового элемента. Это значение будет индексом данного элемента
											// в списке GetRBItemList().
	//bool	FStabilized;					// флаг стабилизации результата
	bool FGroupOperation;					// флаг того, что производится груповая операция (удаления, добавления), для
											// того, чтобы не вызывать каждый раз пересчет результата.

	int		FNeededCount;					// Требуемое количество результатов измерений.
											// все, что меньше 1 обозначает бесконечность.
											// в противном случае принимается в расчет только FNeededCount
											// количество измерений, все остальное забывается.

	rbitem_t	FMedianValue;				// Расчитанное значение медианы
	int	FMedianIndexL;						// Индекс элемента, к которому привязана медиана (слева)
	int	FMedianIndexR;						// Индекс элемента, к которому привязана медиана (справа)
	int	FMedianIndexSorted;					// Индекс элемента, к которому привязана медиана в отсортированном списке

	// Очищает результат расчета
	void ClearResult();
public:
	// Конструктор.
	// AGenValueX - флаг, говорящий о том, что необходимо в объекте
	// самостоятельно генерировать значения ValueX при добавлении 
	// нового элемента. Это значение будет индексом данного элемента
	// в списке GetRBItemList().
	TRBMedianQueueStat(bool AGenValueX);
	~TRBMedianQueueStat();
	// Возвращает указатель на список значений 
	TRBItemList* GetRBItemList(){return FRBItemList;}; 
	// Возвращает указатель на упорядоченный список значений 
	TRBItemList* GetRBItemListSorted(){return FRBItemListSorted;}; 
	// Возвращает итоговую статистику по оборотам
	//TStatRot GetResStatRot(){return FResStatRot;}; 
	// Возвращает флаг стабилизации результата (спектра, куртозиза и пр.)
	//bool GetStabilized(){return FStabilized;};           
	// Возвращает количество усреднений
	int GetCount(){return FRBItemList->Count();};
	// Возвращает требуемое количество результатов измерений.
	// все, что <=0 обозначает бесконечность.
	// в противном случае принимается в расчет только FNeededCount
	// количество измерений, все остальное забывается при добавлении нового
	// результата измерения, либо при установке данного значения положительной величиной.
	int GetNeededCount(){return FNeededCount;};
	// Устанавливает требуемое количество результатов измерений.
	void SetNeededCount(int ANeededCount);
	// Функция, вызывающая очистку всей статистики 
	void Clear();
	// Функция, вызывающая пересчет результатов
	void CalcResult();
	// Добавляет элемент списка значений для робастного анализа ARBItem 
	// в список этих элементов FRBItemList. Возвращает индекс в списке. 
	int AddRBItem(rbitem_t* ARBItem);
	// Удаляет из списка запись с индексом AIndex
	void DeleteItem(int AIndex);

	// Возвращает значение медианы.
	rbitem_t* GetMedianValue(){return &FMedianValue;};
	// Возвращает индекс элемента, к которому привязана медиана (слева).
	int	GetMedianIndexL(){return FMedianIndexL;};
	// Возвращает индекс элемента, к которому привязана медиана (справа).
	int	GetMedianIndexR(){return FMedianIndexR;};
	// Возвращает индекс элемента, к которому привязана медиана в отсортированном списке
	int	GetMedianIndexSorted(){return FMedianIndexSorted;};

	//void operator = (TRBMedianQueueStat &b);
};
//--------------------------------------------------------------------------
// Класс, производящий робастный анализ измеряемой величины по ее 
// значению и абсолютному значению ее ускорения. Робастный анализ
// производится методом медианы.
//--------------------------------------------------------------------------
// Величина разрешенных значений отклонений 
// между двумя последовательными измерениями
#define RB_VALIDSIGMA		3.0
// Величина разрешенных значений отклонений 
// между двумя последовательными измерениями (для грубой оценки)
#define RB_VALIDSIGMAROUGH	6.0
class TRBAccelQueueStatFilter 
{
private:
	TRBMedianQueueStat* FRBQueueStatVal;
	TRBMedianQueueStat* FRBQueueStatAccelVal;
	bool				FGenValueX;			// Флаг, говорящий о том, что необходимо в объекте
											// самостоятельно генерировать значения ValueX при добавлении 
											// нового элемента. Это значение будет индексом данного элемента
											// в списке.
	bool FAllNotValid;
	bool FAllNotValidRough;
	bool FHaveNotValid;
	bool FHaveNotValidRough;
	int FFirstGoodIndex;
	int FFirstGoodRoughIndex;
	int FLastGoodIndex;
	int FLastGoodRoughIndex;
protected:
public:
	// Конструктор.
	// ANeededCount - требуемое количество результатов измерений. 
	// при ANeededCount <= 0 - это бесконечность, 
	// при ANeededCount > 0 - самые первые значения забываются при достижении
	// количества ANeededCount.
	// AGenValueX - флаг, говорящий о том, что необходимо в объекте
	// самостоятельно генерировать значения ValueX при добавлении 
	// нового элемента. Это значение будет индексом данного элемента
	// в списке.
	TRBAccelQueueStatFilter(int ANeededCount, bool AGenValueX);
	~TRBAccelQueueStatFilter();
	// Функция, добавляющая новый элемент ARBItem в историю измеряемой величины.
	// AControlQueueStat - история внешней измеряемой величины, на которую необходимо
	//					ориентироваться при добавлении элемента ARBItem.
	// AControlAccelMedianKoeff - коэффициент увеличения коридора разрешенных значений, 
	//					определяемого мекдианным ускорением внешней измеряемой величины.
	//					Это значение задается в зависимости от величины ARBItem->ValueY.
	// AMaxAbsAccel - максимальная величина ускорения. 
	// AMinMedianValueY - Минимальное значение медианы, ниже которого все значения признаются
	//						невалидными.
	// Возвращает индекс элемента в списке TRBQueueStatVal.
	int AddVal(rbitem_t* ARBItem, TRB_LongDouble AMinMedianValueY = 0, TRB_LongDouble AMaxAbsAccel = 0, 
		TRBAccelQueueStatFilter* AExternQueueStat = NULL, 
		TRB_LongDouble AExternAccelMedianKoeff = 2);

	// Функция, добавляющая новый элемент со значением AValueY в историю измеряемой величины.
	// AControlQueueStat - история внешней измеряемой величины, на которую необходимо
	//					ориентироваться при добавлении элемента ARBItem.
	// AControlAccelMedianKoeff - коэффициент увеличения коридора разрешенных значений, 
	//					определяемого мекдианным ускорением внешней измеряемой величины.
	//					Это значение задается в зависимости от величины ARBItem->ValueY.
	// AMaxAbsAccel - максимальная величина ускорения. 
	// AMinMedianValueY - Минимальное значение медианы, ниже которого все значения признаются
	//						невалидными.
	// Возвращает индекс элемента в списке TRBQueueStatVal.
	int AddVal(TRB_LongDouble AValueY, TRB_LongDouble AMinMedianValueY = 0, TRB_LongDouble AMaxAbsAccel = 0, 
		TRBAccelQueueStatFilter* AExternQueueStat = NULL, 
		TRB_LongDouble AExternAccelMedianKoeff = 2);

	// Функция, производящая расчет валидности значений.
	// AControlQueueStat - история внешней измеряемой величины, на которую необходимо
	//					ориентироваться при данном расчете.
	// AControlAccelMedianKoeff - коэффициент увеличения коридора разрешенных значений, 
	//					определяемого мекдианным ускорением внешней измеряемой величины.
	// AMaxAbsAccel - максимальная величина ускорения. 
	// AMinMedianValueY - Минимальное значение медианы, ниже которого все значения признаются
	//						невалидными.
	void CalcResult(TRB_LongDouble AMinMedianValueY = 0, TRB_LongDouble AMaxAbsAccel = 0, 
		TRBAccelQueueStatFilter* AExternQueueStat = NULL, 
		TRB_LongDouble AExternAccelMedianKoeff = 2);


	TRBMedianQueueStat* GetRBQueueStatVal(){return FRBQueueStatVal;};
	TRBMedianQueueStat* GetRBQueueStatAccelVal(){return FRBQueueStatAccelVal;};
	// Возвращает требуемое количество результатов измерений.
	// все, что <=0 обозначает бесконечность.
	// в противном случае принимается в расчет только FNeededCount
	// количество измерений, все остальное забывается при добавлении нового
	// результата измерения, либо при установке данного значения положительной величиной.
	int GetNeededCount(){return FRBQueueStatVal->GetNeededCount();};
	// Устанавливает требуемое количество результатов измерений. После вызова
	// данной процедуры необходимо вызвать CalcResult(...) с необходимыми параметрами.
	void SetNeededCount(int ANeededCount);
	// Функция, вызывающая очистку всей статистики 
	void Clear();
	// Функция, возвращает флаг того, нет выбросов (все значения валидны)
	bool GetAllNotValid(){return FAllNotValid;};
	// Функция, возвращает флаг того, нет выбросов (все значения валидны)
	bool GetAllNotValidRough(){return FAllNotValidRough;};
	// Функция, возвращает флаг того, нет выбросов (все значения валидны)
	bool GetHaveNotValid(){return FHaveNotValid;};
	// Функция, возвращает флаг того, нет грубых выбросов (все значения валидны) 
	bool GetHaveNotValidRough(){return FHaveNotValidRough;};
	// Индекс первого валидного значения 
	int GetFirstGoodIndex(){return FFirstGoodIndex;};
	// Индекс первого грубо валидного значения 
	int GetFirstGoodRoughIndex(){return FFirstGoodRoughIndex;};
	// Индекс последнего валидного значения 
	int GetLastGoodIndex(){return FLastGoodIndex;};
	// Индекс последнего грубо валидного значения 
	int GetLastGoodRoughIndex(){return FLastGoodRoughIndex;};

	//void operator = (TRBAccelQueueStatFilter &b);

};
//--------------------------------------------------------------------------

#endif
