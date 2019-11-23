# MPS_ToolKit
Useful tools related to image processing, color science, imaging science, etc

These files are designed to become a libary, but is in early development.

# Academic Disclaimer and Lincense
This library if free to use (and or modify) for academic and commercial use. However, it should not be considered a replacemnt for academic work assigned by course instructors. Primarily when the goal of the assignment is to recreate any or all parts of this libary. If you have questitions ask your course instructor.

# Building
Files can be compiled using cmake. Example:
  ```
  $ mkdir build
  $ cd build
  $ cmake ..
  $ make
  ```
In this build directory, there will be two subdirectories, apps and libs. The apps directory contains command line interface (CLI) applications, leveraging many of the library functions and classes. The libs directory contains the library files that can be linked to other projects.
