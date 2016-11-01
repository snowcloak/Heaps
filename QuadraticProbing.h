#ifndef QUADRATIC_PROBING_H
#define QUADRATIC_PROBING_H

#include <vector>
#include <algorithm>
#include <functional>
#include <string>
#include "QuadraticProbing.cpp"
using namespace std;

int nextPrime( int n );

// QuadraticProbing Hash table class
//
// CONSTRUCTION: an approximate initial size or default of 101
//
// ******************PUBLIC OPERATIONS*********************
// bool insert( x )       --> Insert x
// bool remove( x )       --> Remove x
// bool contains( x )     --> Return true if x is present
// void makeEmpty( )      --> Remove all items
// int hashCode( string str ) --> Global method to hash strings

template <typename HashedObj, typename Pair>
class HashTable
{
public:
    explicit HashTable( int size = 101 ) : array( nextPrime( size ) )
    { makeEmpty( ); }

    bool contains( const HashedObj & x ) const
    {
        return isActive( findPos( x ) );
    }

    void makeEmpty( )
    {
        currentSize = 0;
        for( auto & entry : array )
            entry.info = EMPTY;
    }

    bool insert( const HashedObj & x, const Pair p )
    {
        // Insert x as active
        int currentPos = findPos( x );
        if( isActive( currentPos ) )
            return false;

        array[ currentPos ].element = x;
        array[ currentPos ].position = p;
        array[ currentPos ].info = ACTIVE;

        // Rehash; see Section 5.5
        if( ++currentSize > array.size( ) / 2 )
            rehash( );

        return true;
    }

    bool insert( HashedObj && x, const Pair *p )
    {
        // Insert x as active
        int currentPos = findPos( x );
        if( isActive( currentPos ) )
            return false;

        array[ currentPos ] = std::move( x );
        array[ currentPos ].position = p;
        array[ currentPos ].info = ACTIVE;

        // Rehash; see Section 5.5
        if( ++currentSize > array.size( ) / 2 )
            rehash( );

        return true;
    }

    bool remove( const HashedObj & x )
    {
        int currentPos = findPos( x );
        if( !isActive( currentPos ) )
            return false;

        array[ currentPos ].info = DELETED;
        return true;
    }

    enum EntryType { ACTIVE, EMPTY, DELETED };

    //other functions
    //updating table
    void update(HashedObj & x, Pair p ){
        remove( x );    // remove the element from the hash
        insert( x, p ); // update the table by adding back the element and its new pair
    }

    //getter
    size_t getSize() const
    {
        return currentSize;
    }

    Pair find( const HashedObj & x ) const
    {
        if(contains(x)){
            return array[findPos(x)].position;}
        else return nullptr;
    }

private:
    struct HashEntry
    {
        HashedObj element;
        Pair position;
        EntryType info;

        HashEntry( const HashedObj & e = HashedObj{ }, EntryType i = EMPTY, const Pair &p = Pair{} )
        : element{ e }, info{ i } { }

        HashEntry( HashedObj && e, EntryType i = EMPTY, Pair &p = Pair{ } )
        : element{ std::move( e ) }, info{ i }, position{std::move(p)} { }
    };

    vector<HashEntry> array;
    int currentSize;

    bool isActive( int currentPos ) const
    { return array[ currentPos ].info == ACTIVE; }

    int findPos( const HashedObj & x ) const
    {
        int offset = 1;
        int currentPos = myhash( x );

        while( array[ currentPos ].info != EMPTY &&
              array[ currentPos ].element != x )
        {
            currentPos += offset;  // Compute ith probe
            offset += 2;
            if( currentPos >= array.size( ) )
                currentPos -= array.size( );
        }

        return currentPos;
    }

    void rehash( )
    {
        vector<HashEntry> oldArray = array;

        // Create new double-sized, empty table
        array.resize( nextPrime( 2 * oldArray.size( ) ) );
        for( auto & entry : array )
            entry.info = EMPTY;

        // Copy table over
        currentSize = 0;
        for( auto & entry : oldArray )
            if( entry.info == ACTIVE )
                insert( entry.element, entry.position );
    }

    size_t myhash( const HashedObj & x ) const
    {
        static hash<HashedObj> hf;
        return hf( x ) % array.size( );
    }
};

#endif
