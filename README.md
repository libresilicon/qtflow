# qtflow
## Depencies
### Debian
$ sudo apt-get install qt5-qmake qtbase5-dev-tools qttools5-dev-tools qtscript-tools
$ sudo qtchooser -qt=qt5

## Clone
$ git clone --recursive https://github.com/leviathanch/qtflow.git
$ cd qtflow

## Configure
$ mkdir build
$ cd build
$ qmake ..

## Build
$ make -j6 

