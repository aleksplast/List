#include <stdio.h>
#include <math.h>

#include "list.h"

int main()
{
    FILE* logs = fopen("logs.txt", "w");
    fclose(logs);

    struct List list = {};

    ListCtor(&list, 20);

    int pos1 = TailInsert(&list, 5);

    int pos2 = HeadInsert(&list, 10);

    int pos3 = TailInsert(&list, 20);

    int pos4 = InsertAfterIndex(&list, 40, pos2);

    int pos5 = InsertBeforeIndex(&list, 80, pos2);

    DeleteElement(&list, pos2);

    ListExterminatus(&list);

    for (int i = 1; i < 35; i++)
        TailInsert(&list, i);

//    printf("elem5 = %d\n", list.data[pos5]);

//    printf("elem1 = %d\n", list.data[pos2]);

//    printf("elem2 = %d\n", list.data[pos1]);

//    printf("elem3 = %d\n", list.data[pos4]);

//    printf("elem4 = %d\n", list.data[pos3]);

    ListDetor(&list);

    return 0;
}
