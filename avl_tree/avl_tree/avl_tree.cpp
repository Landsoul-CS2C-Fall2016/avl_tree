// avl_tree.cpp : Defines the entry point for the console application.
//

#include <algorithm>
#include <iostream>
#include <vector>
#include <string>

#include "avl_tree.h"
using namespace std;

int main() {
   // 2014 Total Pay for some Foothill CS faculty from
   // http://transparentcalifornia.com/export/2014-community-colleges.csv
   // and
   // http://www.foothill.edu/cs/schedule.php?act=1&dept=C%20S
   std::vector<std::pair<int, std::string> > professors = {
      {5089, "Rahul Agarwal" },
      { 15351, "Thomas Riordan" },
      { 36983, "Bita Mazloom" },
      { 38966, "David Harden" },
      
      { 46862, "Rula Khayrallah" },
      { 52952, "Anand Venkataraman" },
      { 20468, "Hadeel Ammari" },
      { 97262, "Ladawn Meade" },
      { 21617, "Viet Trinh" },
      { 130809, "Elaine Haight" },
      { 49732, "Jesse Cecil" },
      { 180789, "William Murphy" },
      { 144740, "Michael Loceff" },
   };
   AVLTree<int, std::string> tree;

   std::random_shuffle(professors.begin(), professors.end());
   for (const auto& pay_name_pair : professors) {
      std::cout << "insert(" << pay_name_pair.first << ", "
         << pay_name_pair.second << ")" << std::endl;
      tree.insert(pay_name_pair.first, pay_name_pair.second);
   }
   std::cout << std::endl;


   for (const auto& pay_name_pair : professors) {
      const auto& key = pay_name_pair.first;
      std::cout << "get(" << key << ") == " << tree.get(key) << std::endl;
   }
   cout << endl;

   cout << "Printing tree untouched:\n\n";
   tree.testPrint();
   cout << endl << endl;

   std::random_shuffle(professors.begin(), professors.end());
   for (const auto& pay_name_pair : professors) {
      std::cout << "Deleting(" << pay_name_pair.first << ", "
         << pay_name_pair.second << ")\n";
      tree.remove(pay_name_pair.first);
      tree.testPrint();
      cout << endl << endl;
   }
  // tree.testPrint();


   // TODO: Check that heights are correct ...

   return 0;
}