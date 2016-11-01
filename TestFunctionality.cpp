//
//General Code
//  main.cpp
//  Heaps
//
//  Created by Miguel Zeng on 11/13/15.
//  Copyright © 2015 Miguel Zeng. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <cstdlib>
#include "LeftistHeap.h"
#include "QuadraticProbing.h"

void Parser(LeftistHeap<int> & a_heap, string numbers_filename){
    //create a file reading object
    ifstream Numfile(numbers_filename);
    int line;
    //while loop until reaches end of file

    while (Numfile>>line){
        a_heap.insert(line);
    }
    Numfile.close();
}

int main(int argc, const char * argv[]) {
    LeftistHeap<int> a_priority_queue;
    Parser(a_priority_queue, "5_numbers.txt");
    // Insert a few elements into the queue... // Search for an element.
    const int a_value = 12;
    if (a_priority_queue.find(a_value))
        cout << "Found " << a_value << endl;
    else cout << "Not found " << a_value << endl; // Delete an element.

    const int b_value = 12;
    while (a_priority_queue.remove(b_value)) {
        // This code deletes all b_value items.
        // Remove() returns true if item deleted and false otherwise // (i.e. item not in the queue).
        cout << b_value << " deleted from queue." << endl;
    }
    // Check if deletion was successful
    if (a_priority_queue.find(b_value)) {
    cout << "Serious problem with deletion routine... Need to debug...";
    cout << endl; } else {
        cout << "Deletion successful" << endl; }

    return 0;
}
