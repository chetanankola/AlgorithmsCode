#include "my570list.hpp"
#include "sort.hpp"
#include "display.hpp"
#include "main.hpp"


int main (int argc, char *argv[]) {

	if(argc<2) {
		GUsage();
	}
	///< DO SORT TRANSACTION FROM TFILE
	if(	!strcmp(argv[1],"sort") ) {
		#ifdef DEBUG1
			fprintf(stdout,"----------SORTING-----------------------\n");
		#endif
		Sort(argc,argv);
		return 1;
	}
	///< DO DISPLAY TOP DIR
	if(!strcmp(argv[1],"display") ) {
		#ifdef DEBUG1
			fprintf(stdout,"-------------------display-------------\n");
		#endif
		Display(argc,argv);
		return 1;
	}
	///< TEST THE LIST
	#ifdef DEBUG0
	if(!strcmp(argv[argc-1],"-LTest")) {
		int choice;
		cout<<"Enter some test case"<<endl;
		cin>>choice;
		switch(choice)
		{
			case 1: cout<<"Test:CreateList and print"<<endl;test1(); break;//Just a createlist and print
			case 2: cout<<"Test:Randomshuffle"<<endl;RandomShuffle(); break;
			case 3: cout<<"test2_linkunlink()"<<endl;test2_linkunlink(); break;
			case 4: cout<<"test2_unlinkAll()"<<endl;test2_unlinkAll(); break;
			case 5: cout<<"test_prepend()"<<endl;test_prepend(); break;
			case 6: cout<<"find_obj()"<<endl;test_FindObj(); break;
			case 7: cout<<"test_InsertBefore()"<<endl;test_InsertBefore(); break; 
			case 8: cout<<"test_InsertAfter()"<<endl;test_InsertAfter(); break;
			default:cout<<"please enter a valid testcase number"<<endl;break;
		}
		return 1;
	}
	#endif
	fprintf(stderr, "(malformed command)\n");
	GUsage();
	return 1;
}


void GUsage() {
	fprintf(stderr,"Usage:\n");
	fprintf(stderr,"\thw2 sort [tfile] \n");
	fprintf(stderr,"\thw2 display topdir\n");
	exit(-1);
}








/******************************************************TESTING LISTS***********************************************************************/
#ifdef DEBUG0
int test_FindObj() {
    int i=0;
	int searchitem;
	int num_items = 64;

	My570List *pList = new My570List();
    for (i=0; i < num_items; i++) {
        (void)pList->Prepend((void*)i);
    }
	PrintTestList(pList,num_items);
	while(1) {
		cout<<"enter search node; Press -1 to exit\n";
		cin>>searchitem;
		if(searchitem == -1) break;
		if( pList->Find((void*)searchitem)) {
			fprintf(stdout,"Found\n");
		} else {
			fprintf(stdout,"NOT Found\n");
		}
	}
	return 1;
}

int test_prepend() {

    int i=0;
	int num_items = 64;

	My570List *pList = new My570List();
    for (i=0; i < num_items; i++) {
        (void)pList->Prepend((void*)i);
    }
	PrintTestList(pList,num_items);
	return 1;
}

int RandomIndex(int num_values)
    /* return a random value >= 0 and <= num_values-1 */
{
    double dval=(drand48()*((double)num_values))-((double)0.5);
    int idx=0;

    if (dval < ((double)(-0.5))) dval = (double)(-0.5);
    idx = round(dval);
    if (idx < 0) idx = 0;
    if (idx >= num_values) idx = num_values-1;

    return idx;
}

int  RandomShuffle() {
	int num_items = 64;
	int i=0;
	My570List *pList = new My570List();
	
	for (i=0; i < num_items; i++) {
        (void)pList->Append((void*)i);
    }
	
    My570List *list2=new My570List();
    My570ListElem *elem=NULL;

    for (i=0; i < num_items; i++) {
        int j=0, idx=0, num_in_list=num_items-i;
        void *obj=NULL;

        idx = RandomIndex(num_in_list);
        for (elem=pList->First(); elem != NULL; elem=pList->Next(elem)) {
            if (j == idx) {
                break;
            }
            j++;
        }
        if (elem == NULL) {
            fprintf(stderr, "Unrecoverable error in RandomShuffle().\n");
            exit(1);
        }
        obj = elem->Obj();
        pList->Unlink(elem);
        (void)list2->Append(obj);
    }
    if (!pList->Empty()) {
        fprintf(stderr, "List not empty in RandomShuffle().\n");
        exit(1);
    }
    for (elem=list2->First(); elem != NULL; elem=list2->Next(elem)) {
        (void)pList->Append(elem->Obj());
    }	
return 1;
}
int test1() {
    int i=0;
	int num_items = 64;

	My570List *pList = new My570List();
    for (i=0; i < num_items; i++) {
        (void)pList->Append((void*)i);
    }
	PrintTestList(pList,num_items);
	return 1;
}

void PrintTestList(My570List *pList, int num_items)
{
    My570ListElem *elem=NULL;

    if (pList->Length() != num_items) {
        fprintf(stderr, "List length is not %1d in PrintTestList() listlength = %d.\n", num_items,pList->Length() );
        exit(1);
    }
    for (elem=pList->First(); elem != NULL; elem=pList->Next(elem)) {
        int ival=(int)(elem->Obj());
        fprintf(stdout, "%1d ", ival);
    }
    fprintf(stdout, "\n");
}

int test2_linkunlink() {
    int i=0;
	int num_items = 10;
	My570ListElem *cur=NULL;
	My570List *pList = new My570List();
    for (i=0; i < num_items; i++) {
        (void)pList->Append((void*)i);
    }
	cur = pList->Last();
	cur = pList->Prev(cur);
	fprintf(stdout,"before unlinking\n");
	PrintTestList(pList,num_items);
	pList->Unlink(cur);
	fprintf(stdout,"after unlinking\n");
	PrintTestList(pList,--num_items);
	return 1;
}

int test2_unlinkAll() {
    int i=0;
	int num_items = 10;
	My570List *pList = new My570List();
    for (i=0; i < num_items; i++) {
        (void)pList->Append((void*)i);
    }
	fprintf(stdout,"before unlinking all\n");
	PrintTestList(pList,num_items);
	pList->UnlinkAll();
	fprintf(stdout,"after unlinking all\n");
	PrintTestList(pList,num_items);
	return 1;
}

int test_InsertAfter() {

	int searchitem;
	int myitem = 0;
	int num_items = 0;

	My570List *pList = new My570List();
   /* for (i=0; i < num_items; i++) {
        (void)pList->Append((void*)i);
    }*/
	PrintTestList(pList,num_items);
	while(1) {
		cout<<"Insert after which node\n";
		cin>>searchitem;
		cout<<"enter your node item\n";
		cin>>myitem;
		if(searchitem == -1) break;
		if(pList->InsertAfter((void*)myitem, pList->Find((void*)searchitem)) ) {
			num_items++;
		}
		PrintTestList(pList,num_items);
	}

return 1;
}

int test_InsertBefore() {

	int searchitem;
	int myitem = 0;
	int num_items = 0;

	My570List *pList = new My570List();
   /* for (i=0; i < num_items; i++) {
        (void)pList->Append((void*)i);
    }*/
	PrintTestList(pList,num_items);
	while(1) {
		cout<<"Insert before which node\n";
		cin>>searchitem;
		cout<<"enter your node item\n";
		cin>>myitem;
		if(searchitem == -1) break;
		if(pList->InsertBefore((void*)myitem, pList->Find((void*)searchitem)) ) {
			num_items++;
		}
		PrintTestList(pList,num_items);
	}

	return 1;
}
#endif
