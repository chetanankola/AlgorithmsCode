
/*
 * Author:      William Chia-Wei Cheng (bill.cheng@acm.org)
 *
 * @(#)$Id: my570list.hpp,v 1.1 2011/08/11 04:56:07 william Exp $
 */

#ifndef _MY570LIST_H_
#define _MY570LIST_H_

#include "cs570.h"

class My570ListElem;

class My570List {
public:
    My570List();
    virtual ~My570List();

    int Length() { return num_members; }
    int Empty() { return num_members<=0; }

    int  Append(void *obj);
    int  Prepend(void *obj);
    void Unlink(My570ListElem*);
    void UnlinkAll();
    int  InsertBefore(void *obj, My570ListElem *elem);
    int  InsertAfter(void *obj, My570ListElem *elem);

    My570ListElem *First();
    My570ListElem *Last();
    My570ListElem *Next(My570ListElem *cur);
    My570ListElem *Prev(My570ListElem *cur);

    My570ListElem *Find(void *obj);
private:
    friend class My570ListElem;

    int num_members;
    My570ListElem *anchor;
};

class My570ListElem {
public:
    My570ListElem(void *object=NULL);
    virtual ~My570ListElem();

    void *Obj() { return obj; }
    void Set(void *object) { obj = object; }
private:
    friend class My570List;

    void *obj;
    My570ListElem *next;
    My570ListElem *prev;
};

#endif /*_MY570LIST_H_*/

