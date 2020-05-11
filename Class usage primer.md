## A simple example of class usage and its interface (c++14):

Say, we want to write a simple programm in c++ achieveing a following task:

1. read Address Book JSON from `<stdin>`
2. sort all records by **`Name`** (for simplicity, assume all records have that label)
3. output resulting Address Book JSON

Below is the code sample how that could be achieved using `Json.hpp` class and the source JSON - Address Book:
```c++
#include <iostream>
#include <fstream>
#include "lib/Json.hpp"

// compile with: c++ -o sort_ab -Wall -std=c++14 sorting_ab.cpp
using namespace std;


int main(int argc, char *argv[]) { 
 // read and parse json from cin:
 Json jin{ {istream_iterator<char>{cin>>noskipws}, istream_iterator<char>{}} };
 
 // get all the names into vector and sort them
 vector<string> names{jin.walk("<Name>l:"), jin.walk().end()};
 sort(names.begin(), names.end());

 // rebuild AB with sorted records:
 Json sorted = ARY{};
 for(const auto &name: names)
  sorted.push_back(move( *jin.walk("[Name]:<" + name + ">[-1]") ));

 // // put back into the original container and print using indentation 3 
 jin["AddressBook"].clear().push_back( move(sorted) ); 
 cout << jin.tab(3) << endl;
}
```

**Output result:**
```bash
bash $ c++ -o sort_ab -Wall -std=c++14 sorting_ab.cpp
bash $ 
bash $ <addressbook-sample.json sort_ab | jtc -tc      # using jtc here only for a compact view
{
   "AddressBook": [
      [
         {
            "Name": "Ivan",
            "address": { "city": "Seattle", "postal code": 98104, "state": "WA", "street address": "5423 Madison St" },
            "age": 31,
            "children": [],
            "phoneNumbers": [
               { "number": "573 923-6483", "type": "home" },
               { "number": "523 283-0372", "type": "mobile" }
            ],
            "spouse": null
         },
         {
            "Name": "Jane",
            "address": { "city": "Denver", "postal code": 80206, "state": "CO", "street address": "6213 E Colfax Ave" },
            "age": 25,
            "children": [ "Robert", "Lila" ],
            "phoneNumbers": [
               { "number": "358 303-0373", "type": "office" },
               { "number": "333 638-0238", "type": "home" }
            ],
            "spouse": "Chuck"
         },
         {
            "Name": "John",
            "address": { "city": "New York", "postal code": 10012, "state": "NY", "street address": "599 Lafayette St" },
            "age": 25,
            "children": [ "Olivia" ],
            "phoneNumbers": [
               { "number": "212 555-1234", "type": "mobile" },
               { "number": "213 123-2368", "type": "mobile" }
            ],
            "spouse": "Martha"
         }
      ]
   ]
}
bash $ 
```
for the complete description of Json class interface, refer to [Json.hpp](https://github.com/ldn-softdev/jtc/blob/master/lib/Json.hpp)

Btw, the same sorting is achievable using `<>g` lexeme in `jtc` walk:
```bash
bash $ <addressbook-sample.json jtc -tc -w'[0]' -pi'[Name]:<>g:[-1]'
{
   "AddressBook": [
      {
         "Name": "Ivan",
         "address": { "city": "Seattle", "postal code": 98104, "state": "WA", "street address": "5423 Madison St" },
         "age": 31,
         "children": [],
         "phoneNumbers": [
            { "number": "573 923-6483", "type": "home" },
            { "number": "523 283-0372", "type": "mobile" }
         ],
         "spouse": null
      },
      {
         "Name": "Jane",
         "address": { "city": "Denver", "postal code": 80206, "state": "CO", "street address": "6213 E Colfax Ave" },
         "age": 25,
         "children": [ "Robert", "Lila" ],
         "phoneNumbers": [
            { "number": "358 303-0373", "type": "office" },
            { "number": "333 638-0238", "type": "home" }
         ],
         "spouse": "Chuck"
      },
      {
         "Name": "John",
         "address": { "city": "New York", "postal code": 10012, "state": "NY", "street address": "599 Lafayette St" },
         "age": 25,
         "children": [ "Olivia" ],
         "phoneNumbers": [
            { "number": "212 555-1234", "type": "mobile" },
            { "number": "213 123-2368", "type": "mobile" }
         ],
         "spouse": "Martha"
      }
   ]
}
bash $
```
