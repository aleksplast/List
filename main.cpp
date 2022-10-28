#include <stdio.h>
#include <math.h>

#include "list.h"

int main()
{
    FILE* logs = fopen("logs.txt", "w");
    fclose(logs);

    struct List list = {};

    ListCtor(&list, 20);

    int pos2 = InsertAfterIndex(&list, 40, list.head);

    ListGraphDump(&list, "insert after index", __LINE__);

    int pos5 = InsertBeforeIndex(&list, 80, pos2);

    ListGraphDump(&list, "insert before index", __LINE__);

    DeleteElement(&list, pos2);

    ListGraphDump(&list, "deleting element", __LINE__);

    ListSortByLogicIndex(&list);

    ListGraphDump(&list, "sorting", __LINE__);

    ListExterminatus(&list);

    ListGraphDump(&list, "exterminatus", __LINE__);

//    printf("elem5 = %d\n", list.data[pos5]);

//    printf("elem1 = %d\n", list.data[pos2]);

//    printf("elem2 = %d\n", list.data[pos1]);

//    printf("elem3 = %d\n", list.data[pos4]);

//    printf("elem4 = %d\n", list.data[pos3]);

    ListDetor(&list);

    return 0;
}
