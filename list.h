#ifndef LIST_H
#define LIST_H

typedef double elem_t;

const double POISON = NAN;
const double RESIZECOEFF = 2.0;
const double EPSILON = 1e-10;

#define DEBUG

#ifndef DEBUG
#define DBG if(false)
#else
#define DBG if(true)
#endif

struct ElemList
{
    elem_t elem;
    int next;
    int prev;
};

struct List
{
    struct ElemList* data;
    int head;
    int tail;
    int free;
    size_t size;
    size_t capacity;
};

int ListCtor(struct List* list, size_t capacity);

int TailInsert(struct List* list, elem_t val);

int HeadInsert(struct List* list, elem_t val);

int InsertAfterIndex(struct List* list, elem_t val, int index);

int InsertBeforeIndex(struct List* list, elem_t val, int index);

int ListEnlarge(struct List* list, int mode);

int FillWPoison(struct List* list, int start, int end);

int DeleteElement(struct List* list, int index);

int DeleteHead(struct List* list);

int DeleteTail(struct List* list);

int DeleteLastElement(struct List* list);

int ListExterminatus(struct List* list);

int FindElemByVal(struct List* list, elem_t val);

int FindElemByLogicIndex(struct List* list, int logicindex);

int compare(const elem_t a, const elem_t b);

int ListVerr(struct List* list);

int ListTextDump(struct List* list, int errors, int line, const char* func, const char* file);

int ListDetor(struct List* list);

enum SysErrors
{
    NOERR       = 0,
    MEMERR      = 1,
};

enum ListErrors
{
    ElemErr = 1 << 1,
    TailErr = 1 << 2,
    HeadErr = 1 << 3,
    SizeErr = 1 << 4,
    FreeErr = 1 << 5,
    DataErr = 1 << 6,
};

enum RESIZEMODES
{
    ENLARGE = 1,
    SHRINK = 2,
};

#endif //LIST_H
