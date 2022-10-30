#ifndef LIST_H
#define LIST_H

typedef double elem_t;

static int Piccounter = 1;

#define HEAD list->fictelem->next
#define TAIL list->fictelem->prev

#define DEBUG

#ifndef DEBUG
#define DBG if(false)
#else
#define DBG if(true)
#endif

struct Node
{
    elem_t val;
    struct Node* next;
    struct Node* prev;
};

typedef struct Node Node;

struct List
{
    int size;
    Node* fictelem;
    char* graphlog;
};

int ListCtor(struct List* list, char* graphlog);

int ListGraphDump(struct List* list, char reason[], int line);

Node* InsertElementAfterIndex(struct List* list, Node* index, elem_t val);

Node* InsertElementBeforeIndex(struct List* list, Node* index, elem_t val);

Node* InsertFirstElement(struct List* list, elem_t val);

Node* DeleteElement(struct List* list, Node* index);

Node* FindElemByLogicIndex(struct List* list, int logicindex);

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

#endif //LIST_H
