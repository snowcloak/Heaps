#ifndef BINARY_HEAP_H_
#define BINARY_HEAP_H_

#include <cmath>
#include <iostream>
#include "QuadraticProbing.h"
#include "dsexceptions.h"

// Leftist heap class
//
// CONSTRUCTION: with no parameters
//
// ******************PUBLIC OPERATIONS*********************
// void insert( x )       --> Insert x
// deleteMin( minItem )   --> Remove (and optionally return) smallest item
// Comparable findMin( )  --> Return smallest item
// bool isEmpty( )        --> Return true if empty; else false
// bool isFull( )         --> Return true if full; else false
// void makeEmpty( )      --> Remove all items
// void merge( rhs )      --> Absorb rhs into this heap
// ******************ERRORS********************************
// Throws Underflow and Overflow as warranted


// Node and forward declaration because g++ does
// not understand nested classes.
template <class Comparable>
class LeftistHeap;

template <class Comparable>
class LeftistNode
{
    Comparable   element;
    LeftistNode *parent;
    LeftistNode *left;
    LeftistNode *right;
    int          npl;

    LeftistNode( const Comparable & theElement,LeftistNode *p = NULL, LeftistNode *lt = NULL,
                LeftistNode *rt = NULL, int np = 0 )
    : element( theElement ), parent( p ), left( lt ), right( rt ), npl( np ) { }
    friend class LeftistHeap<Comparable>;
};

template <class Comparable>
class LeftistHeap
{
public:
    LeftistHeap( )
    {
        root = NULL;
    }

    LeftistHeap( const LeftistHeap & rhs )
    {
        root = NULL;
        *this = rhs;
    }


    ~LeftistHeap( )
    {
        makeEmpty( );
    }

    bool isEmpty( ) const
    {
        return root == NULL;
    }


    bool isFull( ) const
    {
        return false;
    }

    const Comparable & findMin( ) const
    {
        if( isEmpty( ) )
            throw Underflow( );
        return root->element;
    }

    void insert( const Comparable & x )
    {
        LeftistNode<Comparable>* ptr = new LeftistNode<Comparable>( x ); //pointer to new node
        root = merge( ptr, root );
        HeapTable.insert(x, ptr); //insert into the Hash Table
    }

   /**
     * Inserts x; duplicates allowed.
     */
    void insert( Comparable && x )
    {
        Comparable copy = std::move( x );
        LeftistNode<Comparable>* ptr = new LeftistNode<Comparable>{ copy };
        root = merge( root, ptr );
        HeapTable.insert( copy, ptr );
    }

    void deleteMin( )
    {
        if( isEmpty( ) )
            throw Underflow( );

        else
        {   LeftistNode<Comparable> *oldRoot = root;
            root->left->parent = nullptr;
            root->right->parent = nullptr;
            root = merge( root->left, root->right );
            HeapTable.remove(oldRoot->element);
            delete oldRoot;
        }
    }

    void deleteMin( Comparable & minItem )
    {
        minItem = findMin( );
        deleteMin( );
    }

    void makeEmpty( )
    {
        reclaimMemory( root );
        root = NULL;
        HeapTable.makeEmpty();
    }

    void merge( LeftistHeap & rhs )
    {
        if( this == &rhs )    // Avoid aliasing problems
            return;

        root = merge( root, rhs.root );
        rhs.root = NULL;
    }


    const LeftistHeap & operator=( const LeftistHeap & rhs )
    {
        if( this != &rhs )
        {
            makeEmpty( );
            root = clone( rhs.root );
        }
        return *this;
    }

    bool remove( const Comparable & x)
    {
        if( isEmpty( ) )
            throw Underflow( );

        //modified deletion
        else if ( find( x ) )
            //pointer indexing from table
        {   LeftistNode<Comparable> *toDelete = HeapTable.find( x );
            while (toDelete->parent != NULL)
            {   /* Percolate up the element to be deleted */
                //three-way swap
                Comparable temp = (toDelete->parent)->element;  // saves upper node's element to temp
                (toDelete->parent)->element = toDelete->element;  // swap: upper node gets lower node's element, delete element is here
                toDelete->element = temp;  // swap: lower node gets upper node's element (saved in temp)
                HeapTable.update( toDelete->element, toDelete ); // update the table
                toDelete = toDelete->parent;  // update deletion pointer: move up to the parent pointer
            }
            deleteMin( );
            HeapTable.remove( x ); // Update the table: remove from hash
            return true;

        }
        else
            return false;
    }


    //new functions not otherwise specified

    //look through the hash table and reveal its position
    bool find(const Comparable & x ){
        if (HeapTable.find(x) == nullptr)
            return false;
        else if ( x == HeapTable.find(x)->element )
            return true;
        else
            return false;
    }

     //Return tree/queue size
    int getCurrentSize( ) const
    {
        return HeapTable.getSize( );
    }


     //Return length of external right path length
    double getRPL( ) const
    {
        return log2(getCurrentSize()+1)+0.5;
    }

private:
    LeftistNode<Comparable> *root;
    HashTable<Comparable, LeftistNode<Comparable>*> HeapTable; //the hash table

    LeftistNode<Comparable> * merge( LeftistNode<Comparable> *h1, LeftistNode<Comparable> *h2 ) const
    {
        if( h1 == nullptr )     // Single node
        {   h2->parent = h1;    // Other fields in h2 already accurate
 	    return h2;	 }
        if( h2 == nullptr )     // Single node
	{   h1->parent = h2;        // Other fields in h1 already accurate
            return h1;	 }
        if( h1->element < h2->element )
            return merge1( h1, h2 );
        else
            return merge1( h2, h1 );
    }

    LeftistNode<Comparable> * merge1( LeftistNode<Comparable> *h1, LeftistNode<Comparable> *h2 ) const
    {
        if( h1->left == nullptr )   // Single node
	{   h1->left = h2;       // Other fields in h1 already accurate
	}
        else
        {
            h1->right = merge( h1->right, h2 );
            if( h1->left->npl < h1->right->npl )
                swapChildren( h1 );
            h1->npl = h1->right->npl + 1;
        }
        return h1;
    }

    void swapChildren( LeftistNode<Comparable> * t ) const
    {
        LeftistNode<Comparable> *tmp = t->left;
        t->left = t->right;
        t->right = tmp;
    }

    void reclaimMemory( LeftistNode<Comparable> * t ) const
    {
        if( t != NULL )
        {
            reclaimMemory( t->left );
            reclaimMemory( t->right );
            delete t;
        }
    }



    LeftistNode<Comparable> * clone( LeftistNode<Comparable> *t ) const
    {
        if( t == NULL )
            return NULL;
        else
            return new LeftistNode<Comparable>( t->element, clone( t->left ),
                                               clone( t->right ), t->npl );
    }
};

#endif
