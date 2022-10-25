#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "list.h"

#define LISTCHECK int errors = ListVerr(list);                                          \
                    DBG ListTextDump(list, errors, __LINE__, __func__, __FILE__);

#define SIZECHECK   if (list->size == list->capacity - 1)                               \
                        ListEnlarge(list, ENLARGE);

int ListCtor(struct List* list, size_t capacity)
{
    DBG assert(list != NULL);

    if ((list->data = (struct ElemList*) calloc(capacity + 1, sizeof(struct ElemList))) == NULL)
        return MEMERR;

    list->data[0].elem = 0xDED32DED;
    list->data[0].next = 0;
    list->data[0].prev = 0;
    list->data[capacity].next = 0;

    FillWPoison(list, 1, capacity + 1);

    list->free = 1;
    list->head = 1;
    list->tail = 1;
    list->size = 0;
    list->capacity = capacity;

    LISTCHECK

    return NOERR;
}

int ListVerr(struct List* list)
{
    DBG assert(list != NULL);

    int errors = 0;

    if (list->free <= 0)
        errors |= FreeErr;
    if (list->data == NULL)
        errors |= DataErr;
    if (list->head < 0)
        errors |= HeadErr;
    if (list->size < 0 || list->size > list->capacity)
        errors |= SizeErr;
    if (list->tail < 0)
        errors |= TailErr;

    int index = list->head;

    if (list->size > 1)
    {
        if (list->data[list->head].prev != -1 || list->data[list->data[list->head].next].prev != list->head)
        {
            errors |= HeadErr;
            return errors;
        }

        if (list->data[list->tail].next != -1 || list->data[list->data[list->tail].prev].next != list->tail)
        {
            errors |= TailErr;
            return errors;
        }

        while (index != list->data[list->tail].prev)
        {
            index = list->data[index].next;

            if (list->data[list->data[index].next].prev != index || list->data[list->data[index].prev].next != index)
            {
                errors |= ElemErr;
                continue;
            }
        }
    }

    else if (list->size == 1)
    {
        if (list->data[list->head].prev != -1 || list->data[list->tail].next != -1)
        {
            errors |= ElemErr;
        }
    }

    index = list->free;
    while (index <= list->capacity)
    {
        if (list->data[index].next <= 0)
        {
            errors |= FreeErr;
            continue;
        }

        index = list->data[index].next;
    }

    return errors;
}

int ListTextDump(struct List* list, int errors, int line, const char* func, const char* file)
{
    DBG assert(list != NULL);

    FILE* logs = fopen("logs.txt", "a");

    fprintf(logs, "\n\n%s at ", func);
    fprintf(logs, "%s", file);
    fprintf(logs, "(%d)\n", line);
    fprintf(logs, "List[%p] ", list);
    if (errors == 0)
        fprintf(logs, "(OK)\n");
    else
        fprintf(logs, "(ERROR CODE = %d)\n", errors);

    fprintf(logs, "head = %d\n", list->head);
    fprintf(logs, "tail = %d\n", list->tail);
    fprintf(logs, "size = %d\n", list->size);
    fprintf(logs, "capacity = %d\n", list->capacity);
    fprintf(logs, "free = %d\n", list->free);

    fprintf(logs, "index:");
    for (int i = 1; i < list->capacity + 1; i++)
        fprintf(logs, "%2d ", i);
    fprintf(logs, "\n%6s", "val:");
    for (int i = 1; i < list->capacity + 1; i++)
        if (isnan(list->data[i].elem))
            fprintf(logs, "PS ");
        else
            fprintf(logs, "%2.0lf ", list->data[i].elem);
    fprintf(logs, "\n%6s", "next:");
    for (int i = 1; i < list->capacity + 1; i++)
        fprintf(logs, "%2d ", list->data[i].next);
    fprintf(logs, "\n%6s", "prev:");
    for (int i = 1; i < list->capacity + 1; i++)
        fprintf(logs, "%2d ", list->data[i].prev);

    fclose(logs);

    return NOERR;
}

int TailInsert(struct List* list, elem_t val)
{
    DBG assert(list != NULL);

    int newtail = list->free;
    list->free = list->data[list->free].next;

    list->data[newtail].elem = val;
    list->data[newtail].prev = list->tail;
    list->size++;

    if (list->tail == list->head)
        list->data[list->tail].prev = -1;

    list->data[list->tail].next = newtail;
    list->data[newtail].next = -1;
    list->tail = newtail;

    LISTCHECK
    SIZECHECK

    return newtail;
}

int HeadInsert(struct List* list, elem_t val)
{
    DBG assert(list != NULL);

    int newhead = list->free;
    list->free = list->data[newhead].next;

    list->data[newhead].elem = val;
    list->data[newhead].next = list->head;
    list->data[newhead].prev = -1;

    list->data[list->head].prev = newhead;
    list->head = newhead;

    list->size++;

    SIZECHECK
    LISTCHECK

    return newhead;
}

int InsertAfterIndex(struct List* list, elem_t val, int index)
{
    DBG assert(list != NULL);

    if (index == list->tail)
        return TailInsert(list, val);

    int newelem = list->free;
    list->free = list->data[newelem].next;

    list->data[newelem].elem = val;
    list->data[newelem].next = list->data[index].next;
    list->data[newelem].prev = index;

    list->data[list->data[index].next].prev = newelem;
    list->data[index].next = newelem;

    list->size++;

    SIZECHECK
    LISTCHECK

    return newelem;
}

int InsertBeforeIndex(struct List* list, elem_t val, int index)
{
    DBG assert(list != NULL);

    if (index == list->head)
        return HeadInsert(list, val);

    int newelem = list->free;
    list->free = list->data[newelem].next;

    list->data[newelem].elem = val;
    list->data[newelem].prev = list->data[index].prev;
    list->data[newelem].next = index;

    list->data[list->data[index].prev].next = newelem;
    list->data[index].prev = newelem;

    list->free = newelem;
    list->size++;

    SIZECHECK
    LISTCHECK

    return newelem;
}

int ListEnlarge(struct List* list, int mode)
{
    DBG assert(list != NULL);

    list->capacity *= RESIZECOEFF;

    printf("capacity = %d", list->capacity);

    struct ElemList* prev = list->data;

    struct ElemList* buffer = (struct ElemList*) realloc(list->data, (list->capacity + 1) * sizeof(struct ElemList));

    if (buffer == NULL)
        return MEMERR;

    list->data = buffer;

    FillWPoison(list, list->size + 1, list->capacity + 1);

    if (prev != list->data)
        free(prev);

    return NOERR;
}

int FillWPoison(struct List* list, int start, int end)
{
    DBG assert(list != NULL);

    int tempfree = list->size + 1;

    for (int i = start; i < end; i++)
    {
        list->data[i].elem = POISON;
        list->data[i].prev = -1;
        list->data[i].next = tempfree + 1;
        tempfree++;
    }

    return NOERR;
}

int DeleteElement(struct List* list, int index)
{
    DBG assert(list != NULL);

    if (list->head == list->tail)
    {
        return DeleteLastElement(list);
    }

    else if (index == list->head)
        return DeleteHead(list);

    else if (index == list->tail)
        return DeleteTail(list);

    list->data[index].elem = 0;

    list->data[list->data[index].next].prev = list->data[index].prev;
    list->data[list->data[index].prev].next = list->data[index].next;

    list->data[index].elem = POISON;
    list->data[index].next = list->free;
    list->data[index].prev = -1;

    list->free = index;
    list->size -= 1;

    LISTCHECK

    return index;
}

int DeleteHead(struct List* list)
{
    DBG assert(list != NULL);

    int newhead = list->data[list->head].next;
    int oldhead = list->head;

    list->data[oldhead].elem = POISON;
    list->data[oldhead].next = list->free;

    list->data[newhead].prev = -1;

    list->free = oldhead;
    list->head = newhead;
    list->size -= 1;

    LISTCHECK

    return oldhead;
}

int DeleteTail(struct List* list)
{
    DBG assert(list != NULL);

    int newtail = list->data[list->head].prev;
    int oldtail = list->tail;

    list->data[oldtail].elem = POISON;
    list->data[oldtail].next = list->free;

    list->data[newtail].next = -1;

    list->free = oldtail;
    list->tail = newtail;
    list->size -= 1;

    LISTCHECK

    return oldtail;
}

int DeleteLastElement(struct List* list)
{
    DBG assert(list != NULL);

    if (list->size == 0)
        return list->head;

    int lastelem = list->head;

    list->data[lastelem].elem = POISON;

    list->data[lastelem].next = list->free;

    list->free = lastelem;
    list->size -= 1;

    LISTCHECK

    return lastelem;
}

int ListExterminatus(struct List* list)
{
    DBG assert(list != NULL);

    while (list->size > 0)
        DeleteElement(list, list->head);

    LISTCHECK

    return NOERR;
}

int FindElemByVal(struct List* list, elem_t val)
{
    DBG assert(list != NULL);

    int index = list->head;
    while (index != list->tail)
    {
        if (compare(list->data[index].elem, val) == 0)
            return index;
        index = list->data[index].next;
    }

    return -1;
}

int FindElemByLogicIndex(struct List* list, int logicindex)
{
    DBG assert(list != NULL);

    int index = list->head;

    while (index != list->tail)
    {
        if (index == logicindex)
            return index;
        index = list->data[index].next;
    }

    return -1;
}

int compare(const elem_t a, const elem_t b)
{
    assert(isfinite(a));
    assert(isfinite(b));

    if (fabs(a-b) < EPSILON)
        return 0;
    if ((a-b) > EPSILON)
        return 1;
    if ((a-b) < EPSILON)
        return -1;

    return NOERR;
}

int ListDetor(struct List* list)
{
    DBG assert(list != NULL);

    LISTCHECK

    free(list->data);
    list->data = NULL;

    list->free = -1;
    list->head = -1;
    list->tail = -1;
    list->size = -1;

    return NOERR;
}
