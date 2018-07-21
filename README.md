# jtc
JSON test console 


#### Simple offline cli tool to manipulate JSON data format

jtc features following:
  - simple user interface allowing applying bulk changes in one command
  - featured walk interface let extracting any combination of data from source JSON
  - extracted data is representable either as it found, or as a complete JSON format
  - support Regular Expressions when searching source JSON
  - fast and efficient processing very large JSON files
  - written entirely in C++, no dependencies
  - extensively debuggable
  - conforms JSON specification ([json.org](http://json.org/index.html))


#### Linux and MacOS precompiled binaries are available for download

For compiling c++14 (or later) is required:
  - to compile under MacOS, use cli: `c++ -o jtc -Wall -std=c++14 -Ofast jtc.cpp`
  - To compile under Linux, use cli: `c++ -o jtc -Wall -std=gnu++14 -static -Ofast jtc.cpp`

*pass `-DNDEBUG` flag if you like to compile w/o debugs, however it's unadvisable -
there's no performance gain from doing so*


#### Compile and install instructions:

download `jtc-master.zip`, unzip it, descend into unzipped folder, compile using
an appropriate command, move compiled file into an install location.

here's the example steps:
  - say, `jtc-master.zip` has been downloaded to a folder and the terminal app is open in that folder:
  - `unzip jtc-master.zip`
  - `cd jtc-master`
  - `c++ -o jtc -Wall -std=c++14 -Ofast jtc.cpp`
  - `sudo mv ./jtc /usr/local/bin/`


#### Quick usage guide:
*run `jtc -g` for walk path explanations and additional usage examples*

Consider following JSON (a mockup of a bookmark storage):
>
>
>





