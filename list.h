//--------------------------------------------------------------------
#ifndef C_LIST_H
#define C_LIST_H
//--------------------------------------------------------------------
class C_List
{
    public:
	C_List();
	~C_List();
    //property    
	unsigned long* items;
	int count;
    //methods    
	void clear(void);
	int add(void* AItem);
	void insert(int AIndex, void* AItem);
	void* first(void);
	void* last(void);
	void delete_item(int AIndex);
	void exchange(int AIndex1, int AIndex2);
	int index_of(void* AItem);
	int remove(void* AItem);
    private:
	int PageCount;    
};

class TList
{
    public:
	TList();
	~TList();
    //property    
	unsigned long* Items;
	int Count;
    //methods    
	void Clear(void);
	int Add(void* AItem);
	void Insert(int AIndex, void* AItem);
	void* First(void);
	void* Last(void);
	void Delete(int AIndex);
	void Exchange(int AIndex1, int AIndex2);
	int IndexOf(void* AItem);
	int Remove(void* AItem);
    private:
	int PageCount;    
};
//--------------------------------------------------------------------
#endif
