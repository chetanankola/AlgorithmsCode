/*!
 * USCID:1895488595
 * Algorithms CSCI-570
 * # homework2
 * 
 * 
 * Description: This program is for creating a Doubly circular linked List
 * Please look up the my570list.hpp for usage of the library
 * 
 */
#include "my570list.hpp"

#include "stdio.h"
#include "stdlib.h"
/*Functions related My570List*/

/*
 * Constructor for the main list: numofmembers, and anchor neeeds to be initialized
 */
My570List::My570List() {
	num_members = 0;
	anchor = new My570ListElem();

	#ifdef DEBUG1
	fprintf(stdout,"My570List::My570List(): num_members= %d\n",num_members);
	#endif
}
/*
 * Destructor
 */
My570List::~My570List() {
	UnlinkAll();
	delete anchor;					///< recheck
}



/*
 * Function to unlink a Node for the List
 * Unlink and delete elem from the list. Please do not delete the object pointed to by elem.
 **/
void My570List::Unlink(My570ListElem* cur) {
	//Cur can never be anchor!!! if so then flag ERROR and exit
	if(cur == anchor)  {
		fprintf(stderr,"My570List::Unlink(My570ListElem* cur):Trying to unlink anchor...ERROR!!!! EXITING!!!\n");
		exit(1);
	}
	///<Case1: list is empty Should never happen
	if( Empty() ) {
		fprintf(stderr,"My570List::Unlink(My570ListElem* cur):Trying to unlink an element from an empty List\n");
		exit(1);					///<recheck if need to exit or return NULL
	}
	
	///<Case2: List length is 1; the cur elem shod be same as First() or Last() else error
	if( Length() == 1 ) {
		if( cur == First()) {
			anchor->next = NULL; 	///<recheck this shod change to anchor->next = anchor if changed in constructor ...too
			anchor->prev = NULL;	///<recheck this shod change to anchor->next = anchor if changed in constructor ...too
			cur->next = NULL;
			cur->prev = NULL;
			delete cur;
			num_members--;
			return;
		} else {
			fprintf(stderr,"My570List::Unlink(My570ListElem* cur): Trying to unlink an element not in the list\n");
			exit(1);				///<recheck if need to exit or return NULL	
		}
	}
	
	///<Case3: regular case unlink it.

		(cur->prev)->next = cur->next;
		(cur->next)->prev = cur->prev;
		cur->next = NULL;
		cur->prev = NULL;
		delete cur;
		num_members--;
		return;
}
/*
 * Function To unlink all nodes and make list empty
 * Unlink and delete all elements from the list and make the list empty. 
 * Please do not delete the objects pointed to be the list elements.
 */
void My570List::UnlinkAll() {
	
	int i=0;
	int totalnumofelems = num_members;
	My570ListElem *elem = NULL;

	///<Case1: if list is empty
	if(Empty()) {
		num_members = 0;
		anchor->next = NULL;
		anchor->prev = NULL;
		return;
	}
	for (i =0;i<totalnumofelems;i++) {
		elem = First();
		Unlink(elem); 					///<This will decrement num_members for every Unlink
		
		#ifdef DEBUG0
		My570ListElem *temp = NULL;
		for (temp=First(); temp != NULL; temp=Next(temp)) {
			int ival=(int)(temp->Obj());
			fprintf(stdout, "%1d ", ival);
		}
		fprintf(stdout,"\n");
		#endif
	}
	//num_members = totalnumofelems;
	anchor->next = NULL;
	anchor->prev = NULL;	
}

/*
 * Function Prepend(void* obj)
 * Append an object before First;
 * Add obj before First(). This function returns TRUE if the operation is performed successfully and returns FALSE otherwise.
 */
 //v7.0 was stable before this was implemented.
int My570List::Prepend(void *obj) {

	///< To do Should there be a check for obj being null?
	///< if this case is not put here then what might happen is a NODE is created with object as empty..
	///< Now if u do this then u might get a hit on Find when u compare obj == NULL.. so You might want to be consistent
	///< and not do this anywhere
	///< recheck
	///< IF DONE HERE SHOULD ALSO BE DONE FOR PREPEND
	if( obj == NULL && (obj!=0) ) { // !Empty()) {	
		///< resulted in some error.. so lets not do this.. FOUND BUG: ERROR WAS BECAUSE obj == 0 is same as null so if value is 0 then u need to check that
		#ifdef DEBUG0	
		fprintf(stderr,"My570List::Append(obj) : obj is NULL! ListLength = %d\n",num_members);
		#endif
		return 0;
	}

	///<case1: empty list: create the Node, Node->next = anchor, Node->prev = anchor; anchor->next = Node, anchor->prev = Node;
	if(Empty()) {
		My570ListElem* NewNode = new My570ListElem( (void*) obj);
		NewNode->next = anchor;
		NewNode->prev = anchor;
		anchor->next  = NewNode;
		anchor->prev  = NewNode; 
		num_members++;
		#ifdef DEBUG1
			fprintf(stdout,"My570List::Prepend(obj) : Prepended a Node to empty list;  numofmembers=%d\n",num_members);
		#endif
		return 1;
	}
	///< case2: anchor - first/last       the new Node should become First.      LAst <===> ANCHOR <====>NewNode
	if(Length()==1) {
		My570ListElem* NewNode = new My570ListElem( (void*) obj);
		NewNode->next = Last(); 		///<or First() since its the same for length1 list
		NewNode->prev = anchor;		
		(Last())->prev = NewNode;		///< Last() should return anchor->next;
		anchor->next = NewNode;			///< will update the First to New Node
		//anchor->prev and Last()->next will always remain same..
		num_members++;
		#ifdef DEBUG1
			fprintf(stdout,"My570List::Prepend(obj) : Prepended a Node to length 1 List;  numofmembers=%d\n",num_members);
		#endif
		return 1;
	}
	///<case3: General Case: more than one Length'ed List
	My570ListElem* NewNode = new My570ListElem( (void*) obj);
	NewNode->next = First();
	NewNode->prev = anchor;
	(First())->prev = NewNode;
	//First->next will be same
	anchor->next = NewNode;				///<this will update the First();
	num_members++;
	#ifdef DEBUG1
		fprintf(stdout,"My570List::Prepend(obj) : Prepended a Node List with List.Length>1;  numofmembers=%d\n",num_members);
	#endif	
	return 1;
	
}


/*
 * Function Append(void* obj)
 * Append an object after Last;
 * Add obj after Last(). This function returns TRUE if the operation is performed successfully and returns FALSE otherwise.
 */
int My570List::Append(void *obj) {
	
	///< To do Should there be a check for obj being null?
	///< if this case is not put here then what might happen is a NODE is created with object as empty..
	///< Now if u do this then u might get a hit on Find when u compare obj == NULL.. so You might want to be consistent
	///< and not do this anywhere
	///< recheck
	///< IF DONE HERE SHOULD ALSO BE DONE FOR PREPEND
	if( obj == NULL && (obj!=0) ) { // !Empty()) {	
		///< resulted in some error.. so lets not do this.. FOUND BUG: ERROR WAS BECAUSE obj == 0 is same as null so if value is 0 then u need to check that
		#ifdef DEBUG0
		fprintf(stdout,"My570List::Append(obj) : obj is NULL! ListLength = %d\n",num_members);
		#endif
		return 0;
	}
	
	
	///<case1: empty list: create the Node, Node->next = anchor, Node->prev = anchor; anchor->next = Node, anchor->prev = Node;
	if(Empty()) {
		My570ListElem* NewNode = new My570ListElem( (void*) obj);
		NewNode->next = anchor;
		NewNode->prev = anchor;
		anchor->next  = NewNode;
		anchor->prev  = NewNode; 
		num_members++;
		#ifdef DEBUG1
			fprintf(stdout,"My570List::Append(obj) : Appended a Node to empty list;  numofmembers=%d\n",num_members);
		#endif
		return 1;
	}
	
	///< case2: anchor - first/last  the new Node should become Last.      NewNode <===> ANCHOR <====>FIRST
	if(Length()==1) {
		My570ListElem* NewNode = new My570ListElem( (void*) obj);
		NewNode->next = anchor;
		NewNode->prev = First();		///< First() should return anchor->next;
		(First())->next = NewNode;
		anchor->prev = NewNode;			///< will update the prev
		num_members++;
		#ifdef DEBUG1
			fprintf(stdout,"My570List::Append(obj) : Appended a Node to length 1 List;  numofmembers=%d\n",num_members);
		#endif
		return 1;
	}
	
	///<case3: General Case: more than one Length'ed List
	My570ListElem* NewNode = new My570ListElem( (void*) obj);
	NewNode->next = anchor;
	NewNode->prev = Last();
	(Last())->next = NewNode;
	anchor->prev = NewNode;
	num_members++;
	#ifdef DEBUG1
		fprintf(stdout,"My570List::Append(obj) : Appended a Node List with List.Length>1;  numofmembers=%d\n",num_members);
	#endif	
	return 1;
}

/*
 * Function to insert NewNode before the Node passed in the function.
 * Insert obj between elem and elem->prev. If elem is NULL, then this is the same as Prepend(). 
 * This function returns TRUE if the operation is performed successfully and returns FALSE otherwise.
 */
int My570List::InsertBefore(void *obj, My570ListElem *elem) {

	///<case1: elem = NULL so perform a prepend
	if(elem == NULL) {
		#ifdef DEBUG1
			fprintf(stdout,"My570List::InsertBefore(obj,NULL) Elem == NULL---calling prepend\n");
		#endif
		return Prepend((void*)obj);		///<recheck if typecast needed or not
	}
	
	///<case2: Empty List and elem not null
	if(Empty()) {
		fprintf(stderr,"My570List::InsertBefore(obj,elem) ERROR!! elem doesnt EXIST in the LIST\n");
		return 0;
	}
	
	///<case3: 1 element List
	if(Length()==1) {
		///< case where First() == Last() == elem
		///< a insert before will result in the NewNode to become First() and FirstNode to become LastNode
		///< this is the same case as prepend with listlenght as 1 so do a PREPEND
		#ifdef DEBUG1
			fprintf(stdout,"My570List::InsertBefore(obj,elem) listlength = 1 so call Prepend()\n");
		#endif
		return Prepend((void*)obj);		///< will incremenet num_members
	}
	
	///case4: regular case, works for insertion between 2 regular elems as well as between anchor and elem
	My570ListElem* NewNode = new My570ListElem( (void*) obj);
	NewNode->next = elem;
	NewNode->prev = elem->prev;
	(elem->prev)->next = NewNode;
	elem->prev = NewNode;
	num_members++;
	#ifdef DEBUG1
			fprintf(stdout,"My570List::InsertBefore(obj,elem) regular case,\n");
	#endif
	return 1;
}
/*
 * Function to insert NewNode after the Node passed in the function.
 * Insert obj between elem and elem->next. If elem is NULL, then this is the same as Append(). 
 * This function returns TRUE if the operation is performed successfully and returns FALSE otherwise.
 */
int My570List::InsertAfter(void *obj, My570ListElem *elem) {
	///<case1: elem = NULL so perform a prepend
	if(elem == NULL) {
		#ifdef DEBUG1
			fprintf(stdout,"My570List::InsertAfter(obj,NULL) Elem == NULL---calling Append\n");
		#endif
		return Append((void*)obj);		///<recheck if typecast needed or not
	}
	
	///<case2: Empty List and elem not null
	if(Empty()) {
		fprintf(stderr,"My570List::InsertAfter((obj,elem) ERROR!! elem doesnt EXIST in the LIST\n");
		return 0;
	}
	///<case3: 1 element List
	if(Length()==1) {
		///< case where First() == Last() == elem
		///< a insert before will result in the NewNode to become Last() and LastNode to become First
		///< this is the same case as append with listlenght as 1 so do a Append()
		#ifdef DEBUG1
			fprintf(stdout,"My570List::InsertAfter(obj,elem) listlength = 1 so call Append()\n");
		#endif
		return Append((void*)obj);		///< will increment num_members
	}	
	
	///case4: regular case, works for insertion between 2 regular elems as well as between elem and anchor
	My570ListElem* NewNode = new My570ListElem( (void*) obj);
	NewNode->next = elem->next;
	NewNode->prev = elem;
	(elem->next)->prev = NewNode;
	elem->next = NewNode;
	//elem->prev is unchanged
	//elem->next->next is unchanged
	num_members++;
	#ifdef DEBUG1
			fprintf(stdout,"My570List::InsertAfter(obj,elem) regular case,\n");
	#endif
	return 1;
}

/*
 * Function to find the Node with Node->obj same as obj passed in function
 * Returns the list element elem such that elem->Obj() == obj. 
 * Returns NULL if no such element can be found.
 */
My570ListElem * My570List::Find(void *obj) {
	
	///< recheck if this is necessary or not since this kind of check is not done for append or prepend
	if(obj == NULL && (obj!=0) ) {
		#ifdef DEBUG0
			fprintf(stdout,"My570List::Find(obj): A null obj passed .... \n");
		#endif
		return NULL; 	///< obj cant be null
	}
	
	if(Empty()) {
		return NULL;	///< List cant be emoty
	}
	My570ListElem *elem=NULL;
	for (elem=First(); elem != NULL; elem=Next(elem)) {
		if(elem->Obj() == obj ) {
			return elem;
		}
	}
	return NULL;	///<meaning object was not found
}



/*
 * Function First()
 * return the first list element
 */
My570ListElem *	My570List::First()  {
	if(Empty()) {
		return NULL;
	}
	return anchor->next;
}

/*
 * Function Last()
 * return the last list element
 */
My570ListElem * My570List::Last() {
	if(Empty()) {
		return NULL;
	}
	return anchor->prev;
}

/*
 * returns Node pointed by Current Nodes next pointer
 */
My570ListElem * My570List::Next(My570ListElem *cur) {
	if(cur->next == anchor)
		return NULL;
		
	return cur->next;
}

/*
 * returns Node pointed by Current Nodes prev pointer
 */
My570ListElem * My570List::Prev(My570ListElem *cur) {

	if(cur->prev == anchor)
		return NULL;		///<recheck 
	return cur->prev;
}


/*Functions related to My570ListElem********************************************************************************/

/*
 * Constructor for the NODE of the LIST: Obj, next and prev needs to be initialized.
 */
My570ListElem::My570ListElem( void *object) {
	Set((void*)object);
	next = NULL;	///< next = NULL; or next = this; recheck 
	prev = NULL;	///< next = NULL; or prev = this; recheck
	#ifdef DEBUG1
		fprintf(stdout,"My570ListElem::My570ListElem()\n");
		//Populate Object and see
	#endif
}


My570ListElem::~My570ListElem() 
{
	obj = NULL;		///<recheck
	next = NULL;	///<recheck
	prev = NULL;	///<recheck
}




/*******************MISC FUNCTIONS***********/

