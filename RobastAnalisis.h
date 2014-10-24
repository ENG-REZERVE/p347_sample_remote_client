//--------------------------------------------------------------------------
// Author: Sidorov E.N.
// Date: 03.07.2013.
// ������, � ������� ������� ������, ��������� ����������� 
// ��������� ������ ����������� ���������.
//--------------------------------------------------------------------------
#ifndef ROBASTANALISIS_H
#define ROBASTANALISIS_H
//--------------------------------------------------------------------------
#include <stdlib.h>
#include <memory.h>
#include "list.h"

//--------------------------------------------------------------------------
// ���� ������ � ��������� ������, ������������ � ��������� �������.
//typedef long double TRB_LongDouble;
typedef float TRB_LongDouble;
//--------------------------------------------------------------------------
// ������� ������ �������� ��� ���������� �������
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
// ������ �������� ��� ���������� �������
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
	TRBItemList*	FRBItemList;			// ������ ���������� �� ���������� ���������
	TRBItemList*	FRBItemListSorted;		// ������������� ������ ���������� �� ���������� ���������
	bool			FGenValueX;				// ����, ��������� � ���, ��� ���������� � �������
											// �������������� ������������ �������� ValueX ��� ���������� 
											// ������ ��������. ��� �������� ����� �������� ������� ��������
											// � ������ GetRBItemList().
	//bool	FStabilized;					// ���� ������������ ����������
	bool FGroupOperation;					// ���� ����, ��� ������������ �������� �������� (��������, ����������), ���
											// ����, ����� �� �������� ������ ��� �������� ����������.

	int		FNeededCount;					// ��������� ���������� ����������� ���������.
											// ���, ��� ������ 1 ���������� �������������.
											// � ��������� ������ ����������� � ������ ������ FNeededCount
											// ���������� ���������, ��� ��������� ����������.

	rbitem_t	FMedianValue;				// ����������� �������� �������
	int	FMedianIndexL;						// ������ ��������, � �������� ��������� ������� (�����)
	int	FMedianIndexR;						// ������ ��������, � �������� ��������� ������� (������)
	int	FMedianIndexSorted;					// ������ ��������, � �������� ��������� ������� � ��������������� ������

	// ������� ��������� �������
	void ClearResult();
public:
	// �����������.
	// AGenValueX - ����, ��������� � ���, ��� ���������� � �������
	// �������������� ������������ �������� ValueX ��� ���������� 
	// ������ ��������. ��� �������� ����� �������� ������� ��������
	// � ������ GetRBItemList().
	TRBMedianQueueStat(bool AGenValueX);
	~TRBMedianQueueStat();
	// ���������� ��������� �� ������ �������� 
	TRBItemList* GetRBItemList(){return FRBItemList;}; 
	// ���������� ��������� �� ������������� ������ �������� 
	TRBItemList* GetRBItemListSorted(){return FRBItemListSorted;}; 
	// ���������� �������� ���������� �� ��������
	//TStatRot GetResStatRot(){return FResStatRot;}; 
	// ���������� ���� ������������ ���������� (�������, ��������� � ��.)
	//bool GetStabilized(){return FStabilized;};           
	// ���������� ���������� ����������
	int GetCount(){return FRBItemList->Count();};
	// ���������� ��������� ���������� ����������� ���������.
	// ���, ��� <=0 ���������� �������������.
	// � ��������� ������ ����������� � ������ ������ FNeededCount
	// ���������� ���������, ��� ��������� ���������� ��� ���������� ������
	// ���������� ���������, ���� ��� ��������� ������� �������� ������������� ���������.
	int GetNeededCount(){return FNeededCount;};
	// ������������� ��������� ���������� ����������� ���������.
	void SetNeededCount(int ANeededCount);
	// �������, ���������� ������� ���� ���������� 
	void Clear();
	// �������, ���������� �������� �����������
	void CalcResult();
	// ��������� ������� ������ �������� ��� ���������� ������� ARBItem 
	// � ������ ���� ��������� FRBItemList. ���������� ������ � ������. 
	int AddRBItem(rbitem_t* ARBItem);
	// ������� �� ������ ������ � �������� AIndex
	void DeleteItem(int AIndex);

	// ���������� �������� �������.
	rbitem_t* GetMedianValue(){return &FMedianValue;};
	// ���������� ������ ��������, � �������� ��������� ������� (�����).
	int	GetMedianIndexL(){return FMedianIndexL;};
	// ���������� ������ ��������, � �������� ��������� ������� (������).
	int	GetMedianIndexR(){return FMedianIndexR;};
	// ���������� ������ ��������, � �������� ��������� ������� � ��������������� ������
	int	GetMedianIndexSorted(){return FMedianIndexSorted;};

	//void operator = (TRBMedianQueueStat &b);
};
//--------------------------------------------------------------------------
// �����, ������������ ��������� ������ ���������� �������� �� �� 
// �������� � ����������� �������� �� ���������. ��������� ������
// ������������ ������� �������.
//--------------------------------------------------------------------------
// �������� ����������� �������� ���������� 
// ����� ����� ����������������� �����������
#define RB_VALIDSIGMA		3.0
// �������� ����������� �������� ���������� 
// ����� ����� ����������������� ����������� (��� ������ ������)
#define RB_VALIDSIGMAROUGH	6.0
class TRBAccelQueueStatFilter 
{
private:
	TRBMedianQueueStat* FRBQueueStatVal;
	TRBMedianQueueStat* FRBQueueStatAccelVal;
	bool				FGenValueX;			// ����, ��������� � ���, ��� ���������� � �������
											// �������������� ������������ �������� ValueX ��� ���������� 
											// ������ ��������. ��� �������� ����� �������� ������� ��������
											// � ������.
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
	// �����������.
	// ANeededCount - ��������� ���������� ����������� ���������. 
	// ��� ANeededCount <= 0 - ��� �������������, 
	// ��� ANeededCount > 0 - ����� ������ �������� ���������� ��� ����������
	// ���������� ANeededCount.
	// AGenValueX - ����, ��������� � ���, ��� ���������� � �������
	// �������������� ������������ �������� ValueX ��� ���������� 
	// ������ ��������. ��� �������� ����� �������� ������� ��������
	// � ������.
	TRBAccelQueueStatFilter(int ANeededCount, bool AGenValueX);
	~TRBAccelQueueStatFilter();
	// �������, ����������� ����� ������� ARBItem � ������� ���������� ��������.
	// AControlQueueStat - ������� ������� ���������� ��������, �� ������� ����������
	//					��������������� ��� ���������� �������� ARBItem.
	// AControlAccelMedianKoeff - ����������� ���������� �������� ����������� ��������, 
	//					������������� ���������� ���������� ������� ���������� ��������.
	//					��� �������� �������� � ����������� �� �������� ARBItem->ValueY.
	// AMaxAbsAccel - ������������ �������� ���������. 
	// AMinMedianValueY - ����������� �������� �������, ���� �������� ��� �������� ����������
	//						�����������.
	// ���������� ������ �������� � ������ TRBQueueStatVal.
	int AddVal(rbitem_t* ARBItem, TRB_LongDouble AMinMedianValueY = 0, TRB_LongDouble AMaxAbsAccel = 0, 
		TRBAccelQueueStatFilter* AExternQueueStat = NULL, 
		TRB_LongDouble AExternAccelMedianKoeff = 2);

	// �������, ����������� ����� ������� �� ��������� AValueY � ������� ���������� ��������.
	// AControlQueueStat - ������� ������� ���������� ��������, �� ������� ����������
	//					��������������� ��� ���������� �������� ARBItem.
	// AControlAccelMedianKoeff - ����������� ���������� �������� ����������� ��������, 
	//					������������� ���������� ���������� ������� ���������� ��������.
	//					��� �������� �������� � ����������� �� �������� ARBItem->ValueY.
	// AMaxAbsAccel - ������������ �������� ���������. 
	// AMinMedianValueY - ����������� �������� �������, ���� �������� ��� �������� ����������
	//						�����������.
	// ���������� ������ �������� � ������ TRBQueueStatVal.
	int AddVal(TRB_LongDouble AValueY, TRB_LongDouble AMinMedianValueY = 0, TRB_LongDouble AMaxAbsAccel = 0, 
		TRBAccelQueueStatFilter* AExternQueueStat = NULL, 
		TRB_LongDouble AExternAccelMedianKoeff = 2);

	// �������, ������������ ������ ���������� ��������.
	// AControlQueueStat - ������� ������� ���������� ��������, �� ������� ����������
	//					��������������� ��� ������ �������.
	// AControlAccelMedianKoeff - ����������� ���������� �������� ����������� ��������, 
	//					������������� ���������� ���������� ������� ���������� ��������.
	// AMaxAbsAccel - ������������ �������� ���������. 
	// AMinMedianValueY - ����������� �������� �������, ���� �������� ��� �������� ����������
	//						�����������.
	void CalcResult(TRB_LongDouble AMinMedianValueY = 0, TRB_LongDouble AMaxAbsAccel = 0, 
		TRBAccelQueueStatFilter* AExternQueueStat = NULL, 
		TRB_LongDouble AExternAccelMedianKoeff = 2);


	TRBMedianQueueStat* GetRBQueueStatVal(){return FRBQueueStatVal;};
	TRBMedianQueueStat* GetRBQueueStatAccelVal(){return FRBQueueStatAccelVal;};
	// ���������� ��������� ���������� ����������� ���������.
	// ���, ��� <=0 ���������� �������������.
	// � ��������� ������ ����������� � ������ ������ FNeededCount
	// ���������� ���������, ��� ��������� ���������� ��� ���������� ������
	// ���������� ���������, ���� ��� ��������� ������� �������� ������������� ���������.
	int GetNeededCount(){return FRBQueueStatVal->GetNeededCount();};
	// ������������� ��������� ���������� ����������� ���������. ����� ������
	// ������ ��������� ���������� ������� CalcResult(...) � ������������ �����������.
	void SetNeededCount(int ANeededCount);
	// �������, ���������� ������� ���� ���������� 
	void Clear();
	// �������, ���������� ���� ����, ��� �������� (��� �������� �������)
	bool GetAllNotValid(){return FAllNotValid;};
	// �������, ���������� ���� ����, ��� �������� (��� �������� �������)
	bool GetAllNotValidRough(){return FAllNotValidRough;};
	// �������, ���������� ���� ����, ��� �������� (��� �������� �������)
	bool GetHaveNotValid(){return FHaveNotValid;};
	// �������, ���������� ���� ����, ��� ������ �������� (��� �������� �������) 
	bool GetHaveNotValidRough(){return FHaveNotValidRough;};
	// ������ ������� ��������� �������� 
	int GetFirstGoodIndex(){return FFirstGoodIndex;};
	// ������ ������� ����� ��������� �������� 
	int GetFirstGoodRoughIndex(){return FFirstGoodRoughIndex;};
	// ������ ���������� ��������� �������� 
	int GetLastGoodIndex(){return FLastGoodIndex;};
	// ������ ���������� ����� ��������� �������� 
	int GetLastGoodRoughIndex(){return FLastGoodRoughIndex;};

	//void operator = (TRBAccelQueueStatFilter &b);

};
//--------------------------------------------------------------------------

#endif
