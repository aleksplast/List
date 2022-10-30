#include <stdio.h>
#include <math.h>

#include "list.h"

int main()
{
    FILE* logs = fopen("logs.txt", "w");
    fclose(logs);

    char* graphlog = "graphlog.htm";

    struct List list = {};

    ListCtor(&list, 10, graphlog);

    int pos2 = InsertAfterIndex(&list, 40, 0);

    printf("HERE");

    ListGraphDump(&list, "insert after index", __LINE__);

    int pos3 = InsertBeforeIndex(&list, 80, pos2);

    ListGraphDump(&list, "insert before index", __LINE__);

    int pos4 = InsertAfterIndex(&list, 35, pos3);

    ListGraphDump(&list, "insert after index", __LINE__);

    int pos5 = InsertBeforeIndex(&list, 25, pos4);

    ListGraphDump(&list, "insert before index", __LINE__);

    DeleteElement(&list, pos4);

    ListGraphDump(&list, "deleting element", __LINE__);

    DeleteElement(&list, pos2);

    ListGraphDump(&list, "deleting element", __LINE__);

    ListSortByLogicIndex(&list);

    ListGraphDump(&list, "sorting", __LINE__);

    ListExterminatus(&list);

    ListGraphDump(&list, "exterminatus", __LINE__);

        InsertAfterIndex(&list, 30, 0);

    ListGraphDump(&list, "insert after index", __LINE__);

    ListDetor(&list);

    return 0;
}
