#include <stdio.h>

#include "SimpleList.h"

int main()
{
    struct List list = {};

    ListCtor(&list, "SimpleGraphLog.htm");

    Node* pos1 = InsertElementAfterIndex(&list, list.fictelem, 10);

    ListGraphDump(&list, "inserting first element", __LINE__);

    Node* pos2 = InsertElementBeforeIndex(&list, pos1, 15);

    ListGraphDump(&list, "inserting before index", __LINE__);

    Node* pos3 = InsertElementAfterIndex(&list, pos1, 20);

    ListGraphDump(&list, "inserting after index", __LINE__);

    DeleteElement(&list, pos1);

    ListGraphDump(&list, "deleting element", __LINE__);

    ListDetor(&list);
}
