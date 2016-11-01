#include "LeftistHeap.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

template<typename QueueType>
void testSearch(QueueType &a_heap, const string &queue_file, const string &del_file)
{  cout << "\n\n||     Running Test Delete    ||\n";
   a_heap.makeEmpty();
   /* Builds the priority queue from inserting elements from file */
    //create a file reading object
    ifstream Numfile(queue_file);
    int line;
    //while loop until reaches end of file
    while (Numfile>>line){
        a_heap.insert(line);
    }
    Numfile.close();

   /* Prints out whether certain elements has been successfully inserted and the rightmost path length */
   cout << "\nSuccess inserting " << a_heap.getCurrentSize() << " into the queue. The minimum element is " << a_heap.findMin() <<endl;
   cout<< ". The length of the rightmost path is " << a_heap.getRPL() << " (according to the theory N >= 2^R - 1)\n\n"<<endl;

   /* Prints our whether certain elements has been found */
    ifstream Deletefile(del_file);
    int linet;
    //while loop until reaches end of file
    while (Deletefile>>linet){
        while(a_heap.remove(linet)) {
        cout << linet << " deleted from queue." << endl;
        }
    // Check if deletion was successful
    if (a_heap.find(linet)) {
    cout << "Serious problem with deletion routine... Need to debug...";
    cout << endl; } else {
        cout << "Deletion successful" << endl; }
    }
    Deletefile.close();
}

int main(int argc, char **argv)
{

  if (argc != 3)
  {  cout << "\nUsage: " << argv[0] << " <input_file_to_create_queue> <input_file_to_check_delete>" << endl;
     return 0;
  }
  string file_to_queue(argv[1]);
  string file_to_delete(argv[2]);
  cout << "\nRunning " << argv[0] << " with documents " << file_to_queue << " & " << file_to_delete << endl;

  LeftistHeap<int> a_priority_queue;
  testSearch(a_priority_queue, file_to_queue, file_to_delete);

  cout << endl << endl;
  return 0;
}






