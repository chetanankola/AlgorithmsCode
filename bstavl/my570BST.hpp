
/*
 * Author:      William Chia-Wei Cheng (bill.cheng@acm.org)
 * Modified by Chetan Ankola
 *
 * @(#)$Id: My570BST.hpp,v 1.1 2011/08/11 04:56:07 william Exp $
 */

#ifndef _MY570BST_H_
#define _MY570BST_H_

#include "cs570.h"

#define MAXBUFSIZE 5096 
#define LEFT 0
#define RIGHT 1
enum  Enum_Balfactor { L = 'L', R = 'R', E = 'E' };
class My570BSTElem;

class My570BST { 
public:
    My570BST();
    virtual ~My570BST();

    int Length() { return num_members; }
    int Empty() { return num_members<=0; }
    void Unlink(My570BSTElem*);
    void UnlinkAll();
    My570BSTElem *Root();
    My570BSTElem *Left(My570BSTElem *cur);
    My570BSTElem *Right(My570BSTElem *cur);
	My570BSTElem *Parent(My570BSTElem *cur);
	bool InsertNode(void *obj);
	bool DeleteNode(void *obj);
	void Up_Del_BothChildNull(My570BSTElem* Node);
	void Up_Del_OneChildNull(My570BSTElem* Node);
	My570BSTElem* getInorderSuccessor(My570BSTElem* Node);
	void SwapNode(My570BSTElem* Node,My570BSTElem* InorderSuc);
	My570BSTElem *Find(void *obj); 
	void DisplayTree();
	void display_recursively(My570BSTElem *root,char* formatstr, int childtype);
	char balancetype(int balfac);
	bool AVLInsertNode(void *obj);
	int RightSubTreeHeight(My570BSTElem* Node );
	int LeftSubTreeHeight(My570BSTElem* Node ) ;
	int TreeHeight(My570BSTElem* Node ) ;
	
	void UpdateBalanceFactorofAllNodes();
	void UpdateBalanceFactor(My570BSTElem* Node);
	int getBalanceFactor(My570BSTElem* Node);
	My570BSTElem* getFirstParentWithNoEqualBalance( My570BSTElem* Node );
	void LeftRotate(My570BSTElem* Node);
	void RightRotate(My570BSTElem* Node);
	void updateBFofOneNode(My570BSTElem* Node) ;
	void BalanceTree(My570BSTElem* NewNode);
	
	
	
	
private:
    friend class My570BSTElem;

    int num_members;
    My570BSTElem *anchor;
};

class My570BSTElem {
public:
    My570BSTElem(void *object=NULL);
    virtual ~My570BSTElem();

    void *Obj() { return obj; }
    void Set(void *object) 	{ 	obj = object; 		}
	My570BSTElem* Right() 	{ 	return this->right;	}
	My570BSTElem* Left()	{  	return this->left; 	}
	My570BSTElem* Parent()	{ 	return this->parent;}
	int BalanceFactor() 	{   return balfactor; 	}
private:
    friend class My570BST;

    void *obj;
    My570BSTElem *left;
    My570BSTElem *right;
	My570BSTElem *parent;
	int balfactor;
};

#endif /*_My570BST_H_*/

