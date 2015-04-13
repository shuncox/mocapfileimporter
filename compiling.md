# Build Instructions #

## Windows ##
### Environment ###
  * Visual Studio 2003 Toolkit (for Maya 7.5 and lower version)
  * Visual Studio 2008 (for Maya 8.0 and higher version)
  * Maya devkit for Windows
### Steps ###
_For Maya 7.5 and lower version_
  1. Set MAYA\_LOCATION to directory contains various Maya devkit
  1. Execute jam
_For Maya 8.0 and higher version_
  1. Open imocaputilz.sln with VS2008
  1. Change include and lib path in the project property to current Maya devkit directory if necessary
  1. Batch build

---

## Linux ##
### Environment ###
  * gcc/g++-3.3 (for Maya 7.5 and lower version)
  * gcc/g++-4.0 (for Maya 8.0 and higher version)
  * Maya devkit for Linux
### Steps ###
  1. Set MAYA\_LOCATION to directory contains various Maya devkit
  1. Execute ./lbuild

---

## Mac OS X ##
Not tested yet.