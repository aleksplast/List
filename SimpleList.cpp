#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "SimpleList.h"

int ListCtor(struct List* list, char* graphlog)
{
    list->size = 0;

    list->fictelem = (Node*) calloc(1, sizeof(Node));
    list->fictelem->next = list->fictelem;
    list->fictelem->prev = list->fictelem;

    list->graphlog = graphlog;
    FILE* graphlogf = fopen(graphlog, "w");
    fclose(graphlogf);

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

    Node* index = list->fictelem;
    int counter = 0;

    for (int i = 0; i < list->size + 1; i++)
    {
        if (index == list->fictelem)
            fprintf(pic, "\t\"box%d\" [shape = \"record\", style = \"filled\", fillcolor = \"yellow\", label = \"{index = %d|ptr = %p|val = %.0lf|prev = %p|next = %p}\"]\n",             \
            i, i, index, index->val, index->prev, index->next);
        else
            fprintf(pic, "\t\"box%d\" [shape = \"record\", style = \"filled\", fillcolor = \"green\", label = \"{index = %d|ptr = %p|val = %.0lf|prev = %p|next = %p}\"]\n",             \
            i, i, index, index->val, index->prev, index->next);
        index = index->next;
    }

    fprintf(pic, "\t\"Tail\" [shape = \"record\", style = \"filled\", fillcolor = \"cyan\", label = \"Tail\"]\n");
    fprintf(pic, "\t\"Head\" [shape = \"record\", style = \"filled\", fillcolor = \"cyan\", label = \"Head\"]\n");

    fprintf(pic, "\t{rank = same;");
    for (int i = 0; i < list->size + 1; i++)
        fprintf(pic, "box%d; ", i);
    fprintf(pic, "}\n");

    for (int i = 0; i < list->size; i++)
    {
        fprintf(pic, "\t\"box%d\" -> \"box%d\"[style = \"invis\"];\n", i, i + 1);
    }

    index = list->fictelem;
    counter = 0;

    while (counter < list->size + 1)
    {
        fprintf(pic, "\t\"box%d\" -> \"box%d\";\n", counter, (counter + 1) % (list->size + 1));
        if (counter - 1 < 0)
            fprintf(pic, "\t\"box%d\" -> \"box%d\";\n", counter, list->size);
        else
            fprintf(pic, "\t\"box%d\" -> \"box%d\";\n", counter, counter - 1);

        if (index == HEAD)
        {
            fprintf(pic, "\t\"Head\" -> \"box%d\";\n", counter);
        }
        if (index == TAIL)
            fprintf(pic, "\t\"Tail\" -> \"box%d\";\n", counter);
        counter++;
        index = index->next;
    }

    fprintf(pic, "}");

    fclose(pic);
    Piccounter += 1;

    char src[80] = "";
    sprintf(src, "dot -Tpng %s -o %s", picture, picturesrc);

    system(src);

    FILE* graphlog = fopen(list->graphlog, "a");

    fprintf(graphlog, "<pre>\n");

    fprintf(graphlog, "<h2>ListDump after %s on %d line</h2>\n", reason, line);
    fprintf(graphlog, "<img src = \"%s\">\n", picturesrc);

    fprintf(graphlog, "<hr>\n");

    fclose(graphlog);

    return NOERR;
}

Node* InsertElementAfterIndex(struct List* list, Node* index, elem_t val)
{
    Node* newelem = (Node*) calloc(1, sizeof(Node));

    printf("index = %p, next = %p, prev = %p\n", index, index->next, index->prev);
    newelem->val = val;
    newelem->next = index->next;
    newelem->prev = index;
    printf("next = %p, prev = %p\n", newelem->next, newelem->prev);
    index->next = newelem;

    if (index == TAIL)
    {
        TAIL = newelem;
        newelem->next = list->fictelem;
    }

    printf("next = %p, prev = %p", newelem->next, newelem->prev);

    list->size++;

    return newelem;
}

Node* InsertElementBeforeIndex(struct List* list, Node* index, elem_t val)
{
    Node* newelem = (Node*) calloc(1, sizeof(Node));

    newelem->val = val;
    newelem->prev = index->prev;
    newelem->next = index;
    index->prev = newelem;

    if (index == HEAD)
    {
        HEAD = newelem;
        newelem->prev = list->fictelem;
    }

    list->size++;

    return newelem;
}

Node* InsertFirstElement(struct List* list, elem_t val)
{
    Node* firstelem = (Node*) calloc(1, sizeof(Node));

    HEAD = firstelem;
    TAIL = firstelem;

    firstelem->val = val;
    firstelem->next = firstelem;
    firstelem->prev = firstelem;

    list->size++;

    return firstelem;
}

Node* DeleteElement(struct List* list, Node* index)
{
    index->next->prev = index->prev;
    index->prev->next = index->next;

    free(index);

    list->size--;

    return index;
}

Node* FindElemByLogicIndex(struct List* list, int logicindex)
{
    int counter = 0;
    Node* index = HEAD;

    while (counter < logicindex)
    {
        index = index->next;
        counter++;
    }

    return index;
}

int ListDetor(struct List* list)
{
    free(list->fictelem);

    list->size = -1;

    return NOERR;
}
