#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "list.h"

#define LISTCHECK int errors = ListVerr(list);                                                      \
                    DBG ListTextDump(list, errors, __LINE__, __func__, __FILE__);

#define SIZECHECK   if (list->size == list->capacity - 1)                                           \
                        ListResize(list, ENLARGE);                                                  \
                    else if (list->size < list->capacity * SHRINKCOEFF && list->status == SORTED)   \
                        ListResize(list, SHRINK);

int ListCtor(struct List* list, size_t capacity)
{
    DBG assert(list != NULL);

    if ((list->data = (struct ElemList*) calloc(capacity + 1, sizeof(struct ElemList))) == NULL)
        return MEMERR;

    list->data[0].elem = POISON;
    list->data[0].next = 0;
    list->data[0].prev = 0;
    list->data[capacity].next = 0;

    FillWPoison(list, 1, capacity + 1);

    list->free = 1;
    list->head = 1;
    list->tail = 1;
    list->size = 0;
    list->capacity = capacity;
    list->status = SORTED;

    list->graphlog = fopen("graphlog.htm", "w");
    fclose(list->graphlog);

    LISTCHECK

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

int ListVerr(struct List* list)
{
    DBG assert(list != NULL);

    int errors = 0;

    if (list->free <= 0)
        errors |= FREEERR;
    if (list->data == NULL)
        errors |= DATAERR;
    if (list->head < 0)
        errors |= HEADERR;
    if (list->size < 0 || list->size > list->capacity)
        errors |= SIZEERR;
    if (list->tail < 0)
        errors |= TAILERR;

    int index = list->head;

    if (list->size > 1)
    {
        if (list->data[list->head].prev != -1 || list->data[list->data[list->head].next].prev != list->head)
            errors |= HEADERR;

        if (list->data[list->tail].next != -1 || list->data[list->data[list->tail].prev].next != list->tail)
            errors |= TAILERR;
        while (index != list->data[list->tail].prev)
        {
            index = list->data[index].next;
            if (list->data[list->data[index].next].prev != index || list->data[list->data[index].prev].next != index)
            {
                errors |= ELEMERR;
            }
        }
    }
    else if (list->size == 1)
    {
        if (list->data[list->head].prev != -1 || list->data[list->tail].next != -1)
            errors |= ELEMERR;
    }

    index = list->free;
    while (index <= list->capacity)
    {
        if (list->data[index].next <= 0)
        {
            errors |= FREEERR;
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
    if (list->status == SORTED)
        fprintf(logs, "status = SORTED\n");
    else
        fprintf(logs, "status = UNSORTED\n");

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

int ListGraphDump(struct List* list, char reason[], int line)
{
    DBG assert(list != NULL);

    char picture[15] = "picture";
    sprintf(picture, "%d.dot", Piccounter);
    char picturesrc[15] = "picture";
    sprintf(picturesrc, "%d.png", Piccounter);

    FILE* pic = fopen(picture, "w");

    fprintf(pic, "DiGraph List {\n");
    fprintf(pic, "\trankdir = TB\n");

    for (int i = 0; i < list->capacity + 1; i++)
    {
        if (isnan(list->data[i].elem))
            fprintf(pic, "\t\"box%d\" [shape = \"record\", style = \"filled\", fillcolor = \"yellow\", label = \"{index = %d|PSN|prev = %d|next = %d}\"]\n", i, i, list->data[i].prev, list->data[i].next);
        else
            fprintf(pic, "\t\"box%d\" [shape = \"record\", style = \"filled\", fillcolor = \"green\", label = \"{index = %d|val = %.0lf|prev = %d|next = %d}\"]\n", i, i, list->data[i].elem, list->data[i].prev, list->data[i].next);
    }
    fprintf(pic, "\t\"Tail\" [shape = \"record\", style = \"filled\", fillcolor = \"cyan\", label = \"Tail\"]\n");
    fprintf(pic, "\t\"Head\" [shape = \"record\", style = \"filled\", fillcolor = \"cyan\", label = \"Head\"]\n");
    fprintf(pic, "\t\"Free\" [shape = \"record\", style = \"filled\", fillcolor = \"cyan\", label = \"Free\"]\n");

    fprintf(pic, "\t{rank = same;");
    for (int i = 0; i < list->capacity + 1; i++)
        fprintf(pic, "box%d; ", i);
    fprintf(pic, "}\n");

    for (int i = 1; i < list->capacity; i++)
    {
        fprintf(pic, "\t\"box%d\" -> \"box%d\"[style = \"invis\"];\n", i, i + 1);
    }

    for (int i = 1; i < list->capacity + 1; i++)
    {
        if (list->data[i].next != -1 && list->data[i].next < list->capacity + 1)
            fprintf(pic, "\t\"box%d\" -> \"box%d\";\n", i, list->data[i].next);
        if (list->data[i].prev != -1)
            fprintf(pic, "\t\"box%d\" -> \"box%d\";\n", i, list->data[i].prev);
    }

    fprintf(pic, "\t\"Tail\" -> \"box%d\";\n", list->tail);
    fprintf(pic, "\t\"Head\" -> \"box%d\";\n", list->head);
    fprintf(pic, "\t\"Free\" -> \"box%d\";\n", list->free);

    fprintf(pic, "}");

    fclose(pic);
    Piccounter += 1;

    char src[80] = "";
    sprintf(src, "dot -Tpng %s -o %s", picture, picturesrc);

    system(src);

    list->graphlog = fopen("graphlog.htm", "a");

    fprintf(list->graphlog, "<pre>\n");

    fprintf(list->graphlog, "<h2>ListDump after %s on %d line</h2>\n", reason, line);
    fprintf(list->graphlog, "<img src = \"%s\">\n", picturesrc);

    fprintf(list->graphlog, "<hr>\n");

    fclose(list->graphlog);

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

    SIZECHECK
    LISTCHECK

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
    list->status = UNSORTED;

    SIZECHECK
    LISTCHECK

    return newhead;
}

int InsertAfterIndex(struct List* list, elem_t val, int index)
{
    DBG assert(list != NULL);

    if (list->size == 0)
        return InsertFirstElem(list, val);

    int newelem = list->free;
    list->free = list->data[newelem].next;

    list->data[newelem].elem = val;
    list->data[newelem].next = list->data[index].next;
    list->data[newelem].prev = index;

    list->data[list->data[index].next].prev = newelem;
    list->data[index].next = newelem;

    list->tail = newelem;

    list->size++;
    list->status = UNSORTED;

    SIZECHECK
    LISTCHECK

    return newelem;
}

int InsertBeforeIndex(struct List* list, elem_t val, int index)
{
    DBG assert(list != NULL);

    if (list->size == 0)
        return InsertFirstElem(list, val);

    int newelem = list->free;
    list->free = list->data[newelem].next;

    list->data[newelem].elem = val;
    list->data[newelem].prev = list->data[index].prev;
    list->data[newelem].next = index;

    list->data[list->data[index].prev].next = newelem;
    list->data[index].prev = newelem;

    list->head = newelem;

    list->size++;
    list->status = UNSORTED;

    SIZECHECK
    LISTCHECK

    return newelem;
}

int InsertFirstElem(struct List* list, elem_t val)
{
    int firstelem = list->free;
    list->free = list->data[firstelem].next;

    list->data[firstelem].elem = val;
    list->data[firstelem].next = firstelem;
    list->data[firstelem].prev = firstelem;

    list->size++;
    list->status = UNSORTED;

    return firstelem;
}

int ListResize(struct List* list, int mode)
{
    DBG assert(list != NULL);
    if (mode == ENLARGE)
        list->capacity *= RESIZECOEFF;
    else if (mode == SHRINK && list->status == SORTED && list->size * 2 < list->capacity)
        list->capacity /= RESIZECOEFF;
    else
        return RESIZERR;

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

int DeleteElement(struct List* list, int index)
{
    DBG assert(list != NULL);

    if (list->size == 1)
        return DeleteLastElement(list);

    list->data[index].elem = 0;

    list->data[list->data[index].next].prev = list->data[index].prev;
    list->data[list->data[index].prev].next = list->data[index].next;

    list->head = list->data[index].prev;
    list->tail = list->data[index].next;

    list->data[index].elem = POISON;
    list->data[index].next = list->free;
    list->data[index].prev = -1;

    list->free = index;
    list->size -= 1;
    list->status = UNSORTED;

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
    list->status = UNSORTED;

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
    list->status = UNSORTED;

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
    list->data[lastelem].prev = -1;

    list->free = lastelem;
    list->size -= 1;
    list->status = UNSORTED;

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

    if (list->status == SORTED && logicindex <= list->size)
        return logicindex;

    else if (logicindex > list->size)
        return SIZEERR;

    int counter = 0;
    int index = list->head;

    while (counter < logicindex)
    {
        index = list->data[index].next;
        counter++;
    }

    return index;
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

int ListSortByLogicIndex(struct List* list)
{
    struct ElemList* newdata = (struct ElemList*) calloc(list->capacity + 1, sizeof(struct ElemList));

    if (newdata == NULL)
        return MEMERR;

    newdata[0].elem = POISON;
    newdata[0].next = 0;
    newdata[0].prev = 0;

    int index = list->head;
    int counter = 1;

    while (counter <= list->size)
    {
        newdata[counter].elem = list->data[index].elem;
        newdata[counter].next = counter + 1;
        newdata[counter].prev = counter - 1;

        counter++;
        index = list->data[index].next;
    }

    newdata[1].prev = counter - 1;
    newdata[counter - 1].next = 1;

    free(list->data);
    list->data = newdata;
    list->tail = counter - 1;
    list->head = 1;
    list->free = counter;
    list->status = SORTED;

    FillWPoison(list, list->free, list->capacity + 1);

    SIZECHECK
    LISTCHECK

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
