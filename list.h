#ifndef LIST_H
#define LIST_H

typedef double elem_t;

const double POISON = NAN;
const double RESIZECOEFF = 2.0;
const double SHRINKCOEFF = 0.375;
const double EPSILON = 1e-10;

#define HEAD list->data[0].next
#define TAIL list->data[0].prev

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

enum ListStatus
{
    UNSORTED    = 0,
    SORTED      = 1,
};

struct List
{
    struct ElemList* data;

    int free;

    size_t size;
    size_t capacity;

    char* graphlog;
    enum ListStatus status;
};

static int Piccounter = 1;

int ListCtor(struct List* list, size_t capacity, char* graphlog);

int TailInsert(struct List* list, elem_t val);

int HeadInsert(struct List* list, elem_t val);

int InsertAfterIndex(struct List* list, elem_t val, int index);

int InsertBeforeIndex(struct List* list, elem_t val, int index);

int InsertFirstElem(struct List* list, elem_t val);

int ListResize(struct List* list, int mode);

int FillWPoison(struct List* list, int start, int end);

int DeleteElement(struct List* list, int index);

int DeleteHead(struct List* list);

int DeleteTail(struct List* list);

int DeleteLastElement(struct List* list);

int ListExterminatus(struct List* list);

int FindElemByVal(struct List* list, elem_t val);

int FindElemByLogicIndex(struct List* list, int logicindex);

int compare(const elem_t a, const elem_t b);

int ListSortByLogicIndex(struct List* list);

int ListVerr(struct List* list);

int ListTextDump(struct List* list, int errors, int line, const char* func, const char* file);

int ListGraphDump(struct List* list, char reason[], int line);

int ListDetor(struct List* list);

enum SysErrors
{
    NOERR       = 0,
    MEMERR      = 1,
};

enum ListErrors
{
    ELEMERR     = 1 << 1,
    TAILERR     = 1 << 2,
    HEADERR     = 1 << 3,
    SIZEERR     = 1 << 4,
    FREEERR     = 1 << 5,
    DATAERR     = 1 << 6,
    RESIZERR    = 1 << 7,
};

enum ResizeModes
{
    ENLARGE     = 1,
    SHRINK      = 2,
};

#endif //LIST_H
