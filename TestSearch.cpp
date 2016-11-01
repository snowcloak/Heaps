#include "LeftistHeap.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

template<typename QueueType>
void testSearch(QueueType &a_heap, const string &queue_file, const string &search_file)
{  cout << "\n\n||     Running Test Search     ||\n";
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
   cout << "\nSuccess inserting " << a_heap.getCurrentSize() << " into the queue. The minimum element is " << a_heap.findMin() << ". The length of the rightmost path is " << a_heap.getRPL() << " (according to the theory N >= 2^R - 1)\n\n";

   /* Prints our whether certain elements has been found */
    ifstream Searchfile(search_file);
    int linet;
    //while loop until reaches end of file
    while (Searchfile>>linet){
        if(a_heap.find(linet))
        {
            cout<<"found: "<< linet<<endl;
        }
        else
        {
            cout<<"not found: "<<linet<<endl;
        }
    }
    Searchfile.close();
}

int main(int argc, char **argv)
{
  if (argc != 3)
  {  cout << "\nUsage: " << argv[0] << " <input_file_to_create_queue> <input_file_to_check_search>" << endl;
     return 0;
  }
  string file_to_queue(argv[1]);
  string file_to_search(argv[2]);
  cout << "\nRunning " << argv[0] << " with documents " << file_to_queue << " & " << file_to_search << endl;

  LeftistHeap<int> queue_of_strings;
  testSearch(queue_of_strings, file_to_queue, file_to_search);

  cout << endl << endl;
  return 0;
}






