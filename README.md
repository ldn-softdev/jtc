# jtc
JSON test console 

Simple offline cli tool to manipulate JSON data format. jtc features following:
  - simple user interface allowing applying bulk changes in one command
  - featured simple walk interface let extracting any combination of data from source JSON
  - extracted data is representable either as it found, or as a complete JSON format
  - support Regular Expressions while searching source JSON
  - fast and efficient processing very large JSON files
  - written in C++14
  - extensively debuggable
  - conforms JSON specification (http://json.org/index.html)


Compile instructions:
Linux and MacOS Binaries are available for download from this project

For compiling, c++14 is requied (or later):
  - to compile under MacOS, use cli:
  c++ -o jtc -Wall -std=c++14  jtc.cpp -Ofast
  - To compile under Linux, use cli:
  c++ -o jtc -Wall -std=gnu++14 -static jtc.cpp -Ofast
  
