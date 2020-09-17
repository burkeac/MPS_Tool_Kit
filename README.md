# MPS_ToolKit
Useful tools related to image processing, color science, imaging science, etc

These files are designed to become a libary, but is in early development.

# Documentation
Doxygen docs can be found here:  https://burkeac.github.io/MPS_Toolkit/

# Academic Disclaimer and Lincense
This library has been released under the MIT license. See the license file for more details. 

Remember, library thirdparty implementations should not be considered a replacemnt for academic work assigned by course instructors. Primarily when the goal of the assignment is to recreate any or all parts of this libary. If you have questitions ask your course instructor.

# Building
Files can be compiled using cmake. Example:
  ```
  $ mkdir build
  $ cd build
  $ cmake ..
  $ make
  ```
In this build directory, there will be two subdirectories, apps and libs. The apps directory contains command line interface (CLI) applications, leveraging many of the library functions and classes. The libs directory contains the library files that can be linked to other projects.
