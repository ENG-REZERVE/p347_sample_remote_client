//--------------------------------------------------------------------
#include "list.h"
#include <assert.h>
//--------------------------------------------------------------------
#ifndef PAGE_SIZE
#define PAGE_SIZE	sizeof(unsigned long)
#endif
//--------------------------------------------------------------------
#ifndef NULL
#define NULL		0
#endif
//--------------------------------------------------------------------
C_List::C_List()
{
    count = 0;
    items = NULL;
    items = new unsigned long[PAGE_SIZE/sizeof(unsigned long)];
    PageCount = 1;
}
//--------------------------------------------------------------------
C_List::~C_List()
{
    if (items != NULL)
	delete[] items;
}
//--------------------------------------------------------------------
void C_List::clear(void)
{
    count = 0;
}
//--------------------------------------------------------------------
void* C_List::first(void)
{
    if ((items == NULL) || (count == 0))
	return NULL;
    return (void*)items[0];
}
//--------------------------------------------------------------------
void* C_List::last(void)
{
    if ((items == NULL) || (count == 0))
	return NULL;
    return (void*)items[count-1];
}
//--------------------------------------------------------------------
int C_List::add(void* AItem)
{
    if (count >= (PAGE_SIZE/sizeof(unsigned long))){
	PageCount++;
	unsigned long* temp = NULL;
	temp = new unsigned long[PageCount*(PAGE_SIZE/sizeof(unsigned long))];
	//assert(temp != NULL);
	for (int i = 0; i < count; i ++)
	    temp[i] = items[i];
	delete[] items;
	items = temp;
    }
    items[count] = (unsigned long)AItem;
    count++;
    return count-1;
}
//--------------------------------------------------------------------
void C_List::insert(int AIndex, void* AItem)
{
    add(AItem);
    if (AIndex < count){
	for (int i = count-1; i > AIndex; i--)
	    items[i] = items[i-1];
	items[AIndex] = (long unsigned int)AItem;    
    }
}
//--------------------------------------------------------------------
void C_List::delete_item(int AIndex)
{
    if ((AIndex < 0) || (count <= 0) || (AIndex >= count))
	return;
    for (int i = AIndex; i < count-1; i ++)
	items[i] = items[i+1];
    count--;	
}
//--------------------------------------------------------------------
void C_List::exchange(int AIndex1, int AIndex2)
{
    if ((count <= 0) || (AIndex1 < 0) 
	|| (AIndex2 < 0) || (AIndex1 == AIndex2) 
	|| (AIndex1 >= count) || (AIndex2 >= count))
	return;
    unsigned long temp = items[AIndex1];
    items[AIndex1] = items[AIndex2];
    items[AIndex2] = temp;
}
//--------------------------------------------------------------------
int C_List::index_of(void* AItem)
{
    int res = -1;
    for (int i = 0; i < count; i ++)
	if ((void*)items[i] == AItem)
	    return i;
    return res;
}
//--------------------------------------------------------------------
int C_List::remove(void* AItem)
{
    int idx = index_of(AItem);
    if (idx >= 0)
	delete_item(idx);
    return idx;	
}
//--------------------------------------------------------------------






//--------------------------------------------------------------------
TList::TList()
{
    Count = 0;
    Items = NULL;
    Items = new unsigned long[PAGE_SIZE/sizeof(unsigned long)];
    assert(Items != NULL);
    PageCount = 1;
}
//--------------------------------------------------------------------
TList::~TList()
{
    if (Items != NULL)
	delete[] Items;
}
//--------------------------------------------------------------------
void TList::Clear(void)
{
    Count = 0;	
}
//--------------------------------------------------------------------
void* TList::First(void)
{
    if ((Items == NULL) || (Count == 0))
	return NULL;
    return (void*)Items[0];
}
//--------------------------------------------------------------------
void* TList::Last(void)
{
    if ((Items == NULL) || (Count == 0))
	return NULL;
    return (void*)Items[Count-1];
}
//--------------------------------------------------------------------
int TList::Add(void* AItem)
{
    if (Count >= (PAGE_SIZE/sizeof(unsigned long)))
    {
	PageCount++;
	unsigned long* temp = NULL;
	temp = new unsigned long[PageCount*(PAGE_SIZE/sizeof(unsigned long))];
	assert(temp != NULL);
	for (int i = 0; i < Count; i ++)
	    temp[i] = Items[i];
	delete[] Items;
	Items = temp;
    }
    Items[Count] = (unsigned long)AItem;
    Count++;
    return Count-1;
}
//--------------------------------------------------------------------
void TList::Insert(int AIndex, void* AItem)
{
    Add(AItem);
    if (AIndex < Count)
    {
	for (int i = Count-1; i > AIndex; i--)
	    Items[i] = Items[i-1];
	Items[AIndex] = (long unsigned int)AItem;    
    }	
}
//--------------------------------------------------------------------
void TList::Delete(int AIndex)
{
    if ((AIndex < 0) || (Count <= 0) || (AIndex >= Count))
	return;
    for (int i = AIndex; i < Count-1; i ++)	
	Items[i] = Items[i+1];
    Count--;	
}
//--------------------------------------------------------------------
void TList::Exchange(int AIndex1, int AIndex2)
{
    if ((Count <= 0) || (AIndex1 < 0) 
	|| (AIndex2 < 0) || (AIndex1 == AIndex2) 
	|| (AIndex1 >= Count) || (AIndex2 >= Count))
	return;
    unsigned long temp = Items[AIndex1];	
    Items[AIndex1] = Items[AIndex2];
    Items[AIndex2] = temp;
}
//--------------------------------------------------------------------
int TList::IndexOf(void* AItem)
{
    int res = -1;
    for (int i = 0; i < Count; i ++)
	if ((void*)Items[i] == AItem)
	    return i;
    return res;
}
//--------------------------------------------------------------------
int TList::Remove(void* AItem)
{
    int idx = IndexOf(AItem);
    if (idx >= 0)
	Delete(idx);
    return idx;	
}
//--------------------------------------------------------------------
