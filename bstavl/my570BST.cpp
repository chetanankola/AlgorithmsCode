/*!
 * USCID:1895488595
 * Algorithms CSCI-570
 * # homework2
 * 
 * 
 * Description: This program is for creating a Doubly circular linked Tree
 * Please look up the My570BST.hpp for usage of the library
 * 
 */
#include "my570BST.hpp"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <iostream>

extern bool DisplayFlag;
extern bool AVLFlag;

/*Functions related My570BST*/

/*
 * Constructor for the main tree: numofmembers, and anchor neeeds to be initialized
 */
My570BST::My570BST() {
	num_members = 0;
	anchor = new My570BSTElem();

	#ifdef DEBUG2
	fprintf(stdout,"My570BST::My570BST(): num_members= %d\n",num_members);
	#endif
}
/*
 * Destructor
 */
My570BST::~My570BST() {
	UnlinkAll();
	delete anchor;					///< recheck
}


/*
 * Function To unlink all nodes and make tree empty
 * Unlink and delete all elements from the tree and make the tree empty. 
 * Please do not delete the objects pointed to be the tree elements.
 */
void My570BST::UnlinkAll() {
	
	int i=0;
	int totalnumofelems = num_members;
	My570BSTElem *elem = NULL;

	///<Case1: if tree is empty
	if(Empty()) {
		num_members = 0;
		anchor->left = NULL;
		anchor->right = NULL;
		anchor->parent = NULL;
		return;
	}
	for (i =0;i<totalnumofelems;i++) {
		elem = Root();
		Unlink(elem); 					///< This will decrement num_members for every Unlink  
										///< Change:Unlink should adjust/update root node or any node around the deleted node if necessary
		
	}
	//num_members = totalnumofelems;
	anchor->left = NULL;
	anchor->right = NULL;	
	anchor->parent = NULL;
}


/* FUNCTION DEPRECATED!!! NOT USED ANYMORE : REFER: DeleteNode function
 * Function to unlink a Node for the Tree
 * Unlink and delete elem from the tree. Please do not delete the object pointed to by elem.
 **/
void My570BST::Unlink(My570BSTElem* cur) {
	//Cur can never be anchor!!! if so then flag ERROR and exit
	if(cur == anchor)  {
		fprintf(stderr,"My570BST::Unlink(My570BSTElem* cur):Trying to unlink anchor...ERROR!!!! EXITING!!!\n");
		exit(1);
	}
	///<Case1: tree is empty Should never happen // if tree is empty this case should be taken care before itself than in Unlink
	if( Empty() ) {
		fprintf(stderr,"My570BST::Unlink(My570BSTElem* cur):Trying to unlink an element from an empty Tree\n");
		exit(1);					///<recheck if need to exit or return NULL
	}

}

/* Function to insert node into a tree unbalanced manner
 * Object type can be anything generally a string
 * Returns true if successful else false if not successful.
 * As for assignment you need to print error message and quit if unsuccessful or if the key is already found
 */
bool My570BST::InsertNode(void *obj) {
	if( obj == NULL && (obj!=0) ) { // !Empty()) {	
		fprintf(stderr,"bool My570BST::InsertNode(void *obj) : obj is NULL! ListLength = %d\n",num_members);
		exit(-1);
	}
///REMEMBER TO INCREMENT THE num_members!!! EXTREMELY IMPORTANT and DECREMENT DURING DELETION
	///<case1: empty list: create the Node, Node->next = anchor, Node->prev = anchor; anchor->next = Node, anchor->prev = Node;
	if(Empty()) {
		#ifdef DEBUG1
		fprintf(stdout,"My570BST::InsertNode(void *%s): EmptyList\n",(char*)obj);				///< PRINT THE RAW BUFFER
		#endif 
		My570BSTElem* NewNode = new My570BSTElem( (void*) obj);
		anchor->right = NewNode;
		anchor->left = NewNode;
		anchor->parent = NULL;
		
		NewNode->left = NULL;
		NewNode->right= NULL;
		NewNode->parent = NULL; /// <every node except The root node will have parent!!

		num_members++;
		#ifdef DEBUG1
			fprintf(stdout,"bool My570BST::InsertNode(void *obj) : Inserted a Node to empty list;  numofmembers=%d\n",num_members);
		#endif
		return true;
	}

	if(Find((void*) obj)) { //meaning if the object is found!! then error!
		fprintf(stderr,"Insertion Failed:Duplicate Node Insertion: NodeKey:%s\n",(char*)obj);
		exit(-1);
	}
	///If not found then insert
	My570BSTElem* NewNode = new My570BSTElem( (void*) obj);
	My570BSTElem *elem=NULL;
	My570BSTElem *prev=NULL;
	elem=Root();
	prev = elem;
	bool isRight = false;
	while(elem!=NULL){
		char* nodekey = (char*) elem->obj;
		char* insertionkey = (char*) obj;
		prev = elem;
		if( strcmp(insertionkey,nodekey)==0 ) { ///< or should I do a strcomparison?
			fprintf(stderr,"Insersssdstion Failed:Duplicate Node Insertion: NodeKey:%s\n",(char*)obj);
			exit(-1);
		}
		if( strcmp(insertionkey,nodekey)>0) {
			elem = elem->right; ///< or this->Right(elem);
			isRight = true;
		}	
		if( strcmp(insertionkey,nodekey) <0) {
			elem = elem->left;  ///< or this->Left(elem);
			isRight = false;
		}
	}///<end of while
	
			
	if(prev==NULL) {
		fprintf(stderr,"Insertion Failed:Prev = null: NodeKey:%s\n",(char*)obj);
		exit(-1);
	}
	
	if(isRight) {
		prev->right = NewNode;
	} else  {
		prev->left = NewNode;
		
	}
	num_members++;
	NewNode->parent = prev;		
	return true;	
}
/*
 * Begin code I did not write.
 * This code is partly derived from http://stackoverflow.com/questions/2597637/finding-height-in-binary-search-tree
 * If the source code requires you to include copyright, put copyright here.
 */
int My570BST::TreeHeight(My570BSTElem* Node ) {
	
	if(Node==NULL) 
		return -1;
		
	int leftheight =  TreeHeight(Node->left );
	int rightheight = TreeHeight(Node->right);
	
	if(leftheight>rightheight) {
		return leftheight+1;
	} else {
		return rightheight+1;
	}
}
/*
 * End code I did not write.
 */	
 
 
int My570BST::RightSubTreeHeight(My570BSTElem* Node ) {
	if(Node->right==NULL) {
		return 0;
	}
	return (TreeHeight(Node->right)+1);
}
int My570BST::LeftSubTreeHeight(My570BSTElem* Node ) {

	if(Node->left==NULL) {
		return 0;
	}
	return (TreeHeight(Node->left)+1);
}

int My570BST::getBalanceFactor(My570BSTElem* Node) {
	return (LeftSubTreeHeight(Node ) - RightSubTreeHeight(Node ));
}


void My570BST::updateBFofOneNode(My570BSTElem* Node) {
	Node->balfactor =  (LeftSubTreeHeight(Node ) - RightSubTreeHeight(Node ));
}
void My570BST::UpdateBalanceFactor(My570BSTElem* Node) {

	if(Node==NULL) {
		return;
	}
	Node->balfactor = getBalanceFactor(Node);
	#ifdef DEBUG2
	fprintf(stdout,"%s[%d]\n",(char*)Node->obj,Node->balfactor);
	#endif
	UpdateBalanceFactor(Node->left);
	UpdateBalanceFactor(Node->right);
}
void My570BST::UpdateBalanceFactorofAllNodes() {
	UpdateBalanceFactor(Root());
}




/**
* function returns Null if tree is balanced already otherwise returns 
* the first Node with L or R as its balfactor
*/
My570BSTElem* My570BST::getFirstParentWithNoEqualBalance( My570BSTElem* Node ) {
	
	if(Node==NULL) {
		fprintf(stderr,"My570BST::getFirstParentWithNoEqualBalance(NULL)\n");
		exit(-1);
	}
	if(Node==Root()) { //if Root() then Node is balanced
		fprintf(stderr,"My570BST::getFirstParentWithNoEqualBalance(Root) should not be encountered Since insertion into empty list returns before hand itself\n");
		exit(-1);
	}
	
	My570BSTElem* temp = Node->parent;
	while(temp!=Root()) {
		//if(temp->balfactor!=0) //this can be done if we do "updateallnodesbalfactor which is heavy..
		if(getBalanceFactor(temp)>1 || getBalanceFactor(temp) <-1) {
			break;
		}
		temp=temp->parent;
	}
	
	if(getBalanceFactor(temp)>1 || getBalanceFactor(temp) <-1) {
	#ifdef DEBUG0
		fprintf(stdout,"Node is %s, and its first nonequal parent is %s[%d]\n",(char*) Node->obj,(char*)temp->obj,getBalanceFactor(temp) );
	#endif
		return temp;
	}	
	return NULL;	
}

////////////<AVL INSERTNODE/////////////////////////////////////////////////////////////////

/* Function to insert node into a tree in balanced manner
 * Object type can be anything generally a string
 * Returns true if successful else false if not successful.
 * As for assignment you need to print error message and quit if unsuccessful or if the key is already found
 */
bool My570BST::AVLInsertNode(void *obj) {
	if( obj == NULL && (obj!=0) ) { // !Empty()) {	
		fprintf(stderr,"bool My570BST::AVLInsertNode(void *obj) : obj is NULL! ListLength = %d\n",num_members);
		exit(-1);
	}
///REMEMBER TO INCREMENT THE num_members!!! EXTREMELY IMPORTANT and DECREMENT DURING DELETION
	///<case1: empty list: create the Node, Node->next = anchor, Node->prev = anchor; anchor->next = Node, anchor->prev = Node;
	if(Empty()) {
		#ifdef DEBUG1
		fprintf(stdout,"My570BST::InsertNode(void *%s): EmptyList\n",(char*)obj);				///< PRINT THE RAW BUFFER
		#endif 
		My570BSTElem* NewNode = new My570BSTElem( (void*) obj);
		anchor->right = NewNode;
		anchor->left = NewNode;
		anchor->parent = NULL;
		
		NewNode->left = NULL;
		NewNode->right= NULL;
		NewNode->parent = NULL; /// <every node except The root node will have parent!!

		num_members++;
		#ifdef DEBUG1
			fprintf(stdout,"bool My570BST::AVLInsertNode(void *obj) : Inserted a Node to empty list;  numofmembers=%d\n",num_members);
		#endif
		return true;
	}

	if(Find((void*) obj)) { //meaning if the object is found!! then error!
		fprintf(stderr,"Insertion Failed:Duplicate Node Insertion: NodeKey:%s\n",(char*)obj);
		exit(-1);
	}
	///If not found then insert
	My570BSTElem* NewNode = new My570BSTElem( (void*) obj);
	My570BSTElem *elem=NULL;
	My570BSTElem *prev=NULL;
	elem=Root();
	prev = elem;
	bool isRight = false;
	while(elem!=NULL){
		char* nodekey = (char*) elem->obj;
		char* insertionkey = (char*) obj;
		prev = elem;
		if( strcmp(insertionkey,nodekey)==0 ) { ///< or should I do a strcomparison?
			fprintf(stderr,"Insertion Failed:Duplicate Node Insertion: NodeKey:%s\n",(char*)obj);
			exit(-1);
		}
		if( strcmp(insertionkey,nodekey)>0) {
			elem = elem->right; ///< or this->Right(elem);
			isRight = true;
		}	
		if( strcmp(insertionkey,nodekey) <0) {
			elem = elem->left;  ///< or this->Left(elem);
			isRight = false;
		}
	}///<end of while
	
			
	if(prev==NULL) {
		fprintf(stderr,"Insertion Failed:Prev = null: NodeKey:%s\n",(char*)obj);
		exit(-1);
	}
	
	if(isRight) {
		prev->right = NewNode;
	} else  {
		prev->left = NewNode;
		
	}
	num_members++;
	NewNode->parent = prev;	

	#ifdef DEBUG0
	DisplayTree(); //DISPLAY TREE BEFORE ROTATION
	#endif
	
	//If this doesnt happen then it means the tree is balanced AVL already
	//UpdateBalanceFactorofAllNodes();	
	//	BalanceTree(NewNode);

	return true;	
}


void My570BST::BalanceTree(My570BSTElem* NewNode) {
	My570BSTElem* NoEParent  = getFirstParentWithNoEqualBalance(NewNode);
	if(NoEParent==NULL) {
		return;
	}
	
	if(getBalanceFactor(NoEParent)<-1) {//NEw node is in Right Subtree
		#ifdef DEBUG0
		fprintf(stdout,"Perform Left Rotation about %s\n",(char*) (NoEParent->right)->obj);
		#endif
		if( (getBalanceFactor(NoEParent->right))<0 ) {
			LeftRotate(NoEParent);
		} else {
			#ifdef DEBUG0
			fprintf(stdout,"DoubleRotation needed:[%s] is in Right subtree\n",(char*) (NoEParent->right)->obj);
			#endif 
			RightRotate(NoEParent->right);
			LeftRotate(NoEParent);
		}
	} else if(getBalanceFactor(NoEParent)>1) {//New node is in Left Subtree	
		//do a right rotation
		#ifdef DEBUG0
		fprintf(stdout,"Perform Right Rotation about %s\n",(char*) (NoEParent->left)->obj);
		#endif
		if( (getBalanceFactor(NoEParent->left))>0 ) {
			RightRotate(NoEParent);
		}else {
			#ifdef DEBUG0
			fprintf(stdout,"DoubleRotation needed:[%s] is in Right subtree\n",(char*) (NoEParent->right)->obj);
			#endif
			LeftRotate(NoEParent->left);
			RightRotate(NoEParent);
		}
		
	} else {
		fprintf(stderr,"BalanceTree():1st parent which is non-E has E!!!- fix getparent()\n");
		exit(-1);
	}
}
/*
a               
 \
  b
   \
    c
====================  c
					a   b
b becomes the new root.
a takes ownership of b's left child as its right child, or in this case, null.
b takes ownership of a as its left child.

*/
void My570BST::LeftRotate(My570BSTElem* a) {
 
  My570BSTElem* b = a->right;
// My570BSTElem* c = b->right; ///// this need not be case.. c might be b->left in which case do double rotat
  b->parent = a->parent;
  if(a==Root()) { //make b the Root
	anchor->left = anchor->right = b;
  } else if( a!=Root() ) {
	  int dif = (getBalanceFactor(a))-(getBalanceFactor(a->parent));
	  #ifdef DEBUG0
	  fprintf(stdout,"dif=%d\n",dif);
	  #endif
	  if(dif < -1){ 
		(a->parent)->left = b;
	  } else {
		(a->parent)->right = b;
	  }
  }
  a->right = b->left;  ///root ->left is untouched..
  if(b->left!=NULL){
	(b->left)->parent = a;
  }
  b->left = a;
  a->parent= b;
}
/*
		c        
       b                
      a 
	  
	  b
	 a  c
b becomes the new root.
c takes ownership of b's right child, as its left child. In this case, that value is null.
b takes ownership of c, as it's right child.

*/
void My570BST::RightRotate(My570BSTElem* c) {

	if(c==NULL) {
		fprintf(stderr,"RightRotate(NULL)!!!\n");
		exit(-1);
	}
	
	
	My570BSTElem* parentofc = c->parent;
	My570BSTElem* b = c->left;
	b->parent = parentofc;
	if(c==Root()) { //make b the Root
	anchor->left = anchor->right = b;
	} else if(c!=Root() ) {
		int dif = (getBalanceFactor(c))-(getBalanceFactor(parentofc));
		#ifdef DEBUG0
		fprintf(stdout,"dif=%d\n",dif);
		#endif
		if(dif > 1) {
			parentofc->right = b;
		} else {
			parentofc->left = b;
		}
	}
  c->left = b->right;  ///root ->left is untouched..
  if(b->right!=NULL){
	(b->right)->parent = c;
  }
  b->right = c;
 parentofc= b;

    
}







char My570BST::balancetype(int balfac) {

	if( balfac>1 || balfac<-1) {
		fprintf(stderr,"balance factor = %d gone haywire!! should be 1 , 0 or -1 \n",balfac);
		exit(-1);
	}
	if(balfac==0) {
		return 'E';		///< Left subtree = Right subtree
	}
	if(balfac==1) {
		return 'L'; 	///< Left subtree > Right subtree by one level
	}
	if(balfac==-1) {
		return 'R';		///< Right subtree > Left subtree by one level
	}
	
	return 'E';
}










/**
 This is Pre-order traversal. print root then left then right
*/
void My570BST::display_recursively(My570BSTElem *root,char* formatstr, int childtype) {
	if(Empty()) {
		fprintf(stdout,"(EMPTY)\n");
		return;
	}
	char stylestr[256];
	if(childtype==LEFT) {
		strcpy(stylestr,"+-- ");
	} else if(childtype==RIGHT) {
		strcpy(stylestr,"\\-- ");	
	}
	
	
	if(root==NULL) {
		fprintf(stdout," %s%sNULL\n",formatstr,stylestr);
		return;
	}
	
	if(root==Root()) {
		if(AVLFlag) {
			fprintf(stdout,"%s (%c)",(char*)root->obj,balancetype(root->balfactor));
			#ifdef DEBUG0
			fprintf(stdout,"%s (%d)",(char*)root->obj,getBalanceFactor(root));
			#endif
			#ifdef DEBUG1
			fprintf(stdout,"R=[%d] L=[%d]",RightSubTreeHeight(root),LeftSubTreeHeight(root));
			#endif  
			fprintf(stdout,"\n");
		} else {
			fprintf(stdout,"%s\n",(char*)root->obj);
		}
		char *emptystr = new char[MAXBUFSIZE];
		strcpy(emptystr,"");
		display_recursively(root->left,emptystr,LEFT);
		display_recursively(root->right,emptystr,RIGHT);		
		
	} else {
		if(AVLFlag) {
			fprintf(stdout," %s%s%s (%c)",formatstr,stylestr,(char*)root->obj,balancetype(root->balfactor));	///< VISIT ROOT ///PRINT ROOT
			#ifdef DEBUG0
			fprintf(stdout," %s%s%s (%d)",formatstr,stylestr,(char*)root->obj,getBalanceFactor(root));	///< VISIT ROOT ///PRINT ROOT
			#endif
			#ifdef DEBUG1
			fprintf(stdout,"R=[%d] L=[%d]",RightSubTreeHeight(root),LeftSubTreeHeight(root));
			#endif 			
			fprintf(stdout,"\n");
		} else {
			fprintf(stdout," %s%s%s\n",formatstr,stylestr,(char*)root->obj);						///< VISIT ROOT ///PRINT ROOT
		}
		if(childtype==LEFT){
			char* Leftformatstr= new char[MAXBUFSIZE];
			strcpy(Leftformatstr,formatstr);
			char fourspacepipe[256];
			strcpy(fourspacepipe,"|    ");
			strcat(Leftformatstr,fourspacepipe);
			display_recursively(root->left,Leftformatstr,LEFT);							///<RECURSE VISIT LEFT				
			display_recursively(root->right,Leftformatstr,RIGHT);						///<RECURSE VISIT RIGHT
		} 
		if(childtype==RIGHT) {
			char* Rightformatstr = new char[MAXBUFSIZE];
			strcpy(Rightformatstr,formatstr);
			char fivespace[256];
			strcpy(fivespace,"     ");
			strcat(Rightformatstr,fivespace);
			display_recursively(root->left,Rightformatstr,LEFT);	
			display_recursively(root->right,Rightformatstr,RIGHT);						///<RECURSE VISIT RIGHT				
		}
		
	}
}
void My570BST::DisplayTree() {
	My570BSTElem *temproot = Root();
	char formatstr[100];
	strcpy(formatstr,"");
	display_recursively(temproot,formatstr,-1);
}





	
	

/** DELETION AGORITHM in an unordered way
Deletion:  delete node P
if P has zero child, update parent’s link
if P has one child, update parent’s link
if P has two children
a) swap P with its in-order successor Q
b) Q must either have zero or one child, so update parent 
of Q’s link according to the above 2 rules
**/
/* Function to delete node into a tree unbalanced manner
 * Object type can be anything generally a string
 * Returns true if successful else false if not successful.
 * As for assignment you need to print error message and quit if unsuccessful or if the key is not found
 */
bool My570BST::DeleteNode(void *obj) {
	My570BSTElem *Node = NULL;
	My570BSTElem *Parent = NULL;
	///< REMEMBER TO DECREMENT for every successful deletion
	if( obj == NULL && (obj!=0) ) { // !Empty()) {	
		fprintf(stderr,"bool My570BST::DeleteNode(void *obj) : obj is NULL! ListLength = %d\n",num_members);
		exit(-1);
	}

	if(!(Node = Find((void*) obj))) { //meaning if the object is NOT found!! then error!
		fprintf(stderr,"NodeKey:%s NOT FOUND, Deletion In BST failed!!\n",(char*)obj);
		exit(-1);
	}
	if( Empty() ) {
		fprintf(stderr,"ErrorTrying to delete an element from an empty TREE\n");
		exit(-1);
	}
	#ifdef DEBUG1
	fprintf(stdout,"FOUND KEY [%s] to delete\n",(char*)Node->obj);
	#endif
	
	///< at this point deletion is possible
	/** DELETION AGORITHM in an unordered way
	Deletion:  delete node P
	if P has zero child, update parent’s link
	if P has one child, update parent’s link
	if P has two children
	a) swap P with its in-order successor Q
	b) Q must either have zero or one child, so update parent 
	of Q’s link according to the above 2 rules
	**/
		
	Parent = Node->parent;
	////////////////////////////////////////////////////////////////////////
	//CASE1: 1
	///< If NODE has 0 children then update parent and delete node. update root
	if(Node->left==NULL && Node->right==NULL) {
		Up_Del_BothChildNull(Node);
		return true;
	}//END OF CASE1
	
	///////////////////////////////////////////////////////////////////
	///< Case 2: if one child is null other is not just update parent
	if(Node->left==NULL || Node->right==NULL) {
		Up_Del_OneChildNull(Node);
		return true;
	}//END OF CASE2

	///CAse3: if neither child is null
	if(Node->left!=NULL && Node->right!=NULL) {
		My570BSTElem* InorderSuc = getInorderSuccessor( Node);
		#ifdef DEBUG1
		fprintf(stdout,"InorderSuccesor(%s)=%s",(char*) obj,(char*)InorderSuc->obj);
		#endif
		SwapNode(Node,InorderSuc);
		///< In order successor has either one or zero child so update accordingly.
		if(InorderSuc->left==NULL && InorderSuc->right==NULL) {
			Up_Del_BothChildNull(InorderSuc);
			return true;
		}//END OF CASE1
		
		///////////////////////////////////////////////////////////////////
		///< Case 2: if one child is null other is not just update parent
		if(InorderSuc->left==NULL || InorderSuc->right==NULL) {
			Up_Del_OneChildNull(InorderSuc);
			return true;
		}//END OF CASE2		
		
	}
	return true;	
}

void My570BST::SwapNode(My570BSTElem* Node,My570BSTElem* InorderSuc) {
	char temp[MAXBUFSIZE];
	strcpy(temp,(char*)Node->obj);
	strcpy((char*)Node->obj,(char*)InorderSuc->obj);
	strcpy((char*)InorderSuc->obj,temp);
}

///< simple!! go right and go extreme left before you hit null. thats your inorder successor
My570BSTElem* My570BST::getInorderSuccessor(My570BSTElem* Node) {
	My570BSTElem* temp = Node;
	My570BSTElem* prev = temp;

	if(temp==NULL) {
		fprintf(stderr,"getInorderSuccessor(NULL)::ERROR!!\n");
		exit(-1);
	}
	temp = temp->right; ///This cant be null since both children are non-null.
	while(temp!=NULL) {
		prev = temp;
		temp = temp->left;
	}
	
	return prev;

}
void My570BST::Up_Del_OneChildNull(My570BSTElem* Node) {
	My570BSTElem* Parent = Node->parent;
	
	#ifdef DEBUG1
	fprintf(stdout,"My570BST::Up_Del_OneChildNull(%s): Deleting a Node with one child null\n",(char*) Node->obj);
	#endif 
	if(Node ==Root()) { ///< if that node is root node update anchor
		if(Node->left==NULL && Node->right!=NULL) {
			anchor->left =  Node->right;
			anchor->right = Node->right;	
		} else if( Node->left!=NULL && Node->right==NULL) {
			anchor->left =  Node->left;
			anchor->right = Node->left;
		}
		delete Node;
		num_members--;
		return;
	}
	
	if(Node->left==NULL && Node->right!=NULL) {
		if(Parent->left == Node) {
			Parent->left = Node->right;
			(Node->right)->parent = Parent;
		}else if(Parent->right==Node) {
			Parent->right = Node->right;
			(Node->right)->parent = Parent;
		} else {
			fprintf(stderr,"My570BST::Up_Del_OneChildNull(%s): Parent Node wasnt updated properly !! ERROR!!!!\n",(char*) Node->obj);
			exit(-1);
		}
	}else if ( Node->left!=NULL && Node->right==NULL) { 
		if(Parent->left == Node) {
			Parent->left = Node->left;
			(Node->left)->parent = Parent;
		}else if(Parent->right==Node) {
			Parent->right = Node->left;
			(Node->left)->parent = Parent;
		} else {
			fprintf(stderr,"My570BST::Up_Del_OneChildNull(%s): Parent Node wasnt updated properly !! ERROR!!!!\n",(char*)Node->obj);
			exit(-1);
		}		
	}
	delete Node;
	num_members--;
}

void My570BST::Up_Del_BothChildNull(My570BSTElem* Node) {
	My570BSTElem* Parent = Node->parent;
	#ifdef DEBUG1
	fprintf(stdout,"My570BST::Up_Del_BothChildNull(%s): Deleting a Node with both child null\n",(char*) Node->obj);
	#endif 
	if(Node == Root() ) {  ///< test this case!!!
		anchor->left = NULL;
		anchor->right = NULL;
		delete Node;
		num_members--;
		return;
	}
	if(Parent->left == Node) {
		Parent->left = NULL;
	}else if(Parent->right==Node) {
		Parent->right = NULL;
	} else {
		fprintf(stderr,"My570BST::DeleteNode(%s): Parent Node wasnt updated properly !! ERROR!!!!\n",(char*) Node->obj);
		exit(-1);
	}
	delete Node;
	num_members--;
	#ifdef DEBUG1
	fprintf(stdout,"numofnodes left=%d\n",num_members);
	#endif 
}
/* 
 * Function to find the Node with Node->obj same as obj passed in function
 * Returns the tree element elem such that elem->Obj() == obj. 
 * Returns NULL if no such element can be found.
 */
My570BSTElem * My570BST::Find(void *obj) {	
	///< recheck if this is necessary or not since this kind of check is not done for append or prepend
	if(obj == NULL && (obj!=0) ) {
		#ifdef DEBUG0
			fprintf(stdout,"My570BST::Find(obj): A null obj passed .... \n");
		#endif
		return NULL; 	///< obj cant be null
	}
	
	if(Empty()) {
		return NULL;	///< Tree cant be empty
	}
	
	My570BSTElem *elem=NULL;
	elem=Root(); 
	while(elem!=NULL){
		char* nodekey = (char*) elem->obj;
		char* insertionkey = (char*) obj;
	
		#ifdef DEBUG1
		fprintf(stdout,"nodekey=%s  Insertionkey=%s\n",nodekey,insertionkey);
		#endif
		if( strcmp(insertionkey,nodekey)==0 ) { ///< or should I do a strcomparison?
			return elem;
		}
		if( strcmp(insertionkey,nodekey)>0) {
			elem = elem->right; ///< or this->Right(elem);
		}	
		if( strcmp(insertionkey,nodekey) <0) {
			elem = elem->left;  ///< or this->Left(elem);
		}

	}
	return NULL;	///<meaning object was not found
}

/*
 * Function Root()
 * return the first tree element
 */
My570BSTElem *	My570BST::Root()  {
	if(Empty()) {
		return NULL;
	}
	return anchor->left; // or return anchor->right  both neways point to root node of tree
	//this part will be decided by insert node if numofndes = 0 then anchor->right and anchor->left = root = first node.
}

/*
 * Function Right()
 * return the right child elem
 */
My570BSTElem * My570BST::Right(My570BSTElem *cur) {
	if(cur->right == NULL)
		return NULL;
		
	return cur->right;
}
/*
 * Function Left()
 * return the right child elem
 */
My570BSTElem * My570BST::Left(My570BSTElem *cur) {
	if(cur->left == NULL)
		return NULL;
		
	return cur->left;
}

/*
 * Function Parent()
 * return the right child elem
 */
My570BSTElem * My570BST::Parent(My570BSTElem *cur) {
	if(cur->parent == NULL)
		return NULL;
		
	return cur->parent;
}


/*
 * Constructor for the NODE of the TREE: Obj, left and right needs to be initialized.
 */
My570BSTElem::My570BSTElem( void *object) {
	Set((void*)object);
	left = NULL;	///< left = NULL; or left = this; recheck 
	right = NULL;	///< left = NULL; or right = this; recheck
	parent = NULL;
	balfactor = 0;	///< balance factor = [node's right subtree-depth - node's left subtree depth ]
	#ifdef DEBUG2
		fprintf(stdout,"My570BSTElem::My570BSTElem()\n");
		//Populate Object and see
	#endif
}

My570BSTElem::~My570BSTElem() 
{
	obj = NULL;		///<recheck
	left = NULL;	///<recheck
	right = NULL;	///<recheck
	parent = NULL;	///<recheck
	balfactor = 0;
}
