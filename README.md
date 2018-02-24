# qtflow
## Depencies
 * Qt5Widgets
 * Qt5Network (required version >= 5.10.0)
 * Qt5Qml
 * Qt5PrintSupport
 * Qt5Xml
 * Qt5XmlPatterns
 * KF5Archive (required version >= 5.43.0)
 * KF5Config (required version >= 5.43.0)
 * KF5GuiAddons (required version >= 5.43.0)
 * Gettext
 * PythonInterp
 * KF5I18n (required version >= 5.43.0)
 * KF5KIO (required version >= 5.43.0)
 * KF5Parts (required version >= 5.43.0)
 * KF5Sonnet (required version >= 5.43.0)
 * KF5IconThemes (required version >= 5.43.0)
 * Qt5Core
 * Qt5Gui
 * KF5SyntaxHighlighting (required version >= 5.43.0)
 * Qt5Test
 * Qt5 (required version >= 5.7.0)
 * LibGit2
 * EditorConfig
 * KTextEditor

### OpenSUSE
 sudo zypper in patterns-kde-devel_qt5 

### Debian
Install packages:

 sudo apt-get install \
	qtbase5-dev \
	qtbase5-dev-tools \
	qttools5-dev-tools \
	qtscript-tools \
	python-all-dev \
	build-essential \
	qt5-default \
	bison \
	flex \
	qtbase5-private-dev \
	qtconnectivity5-dev \
	qtdeclarative5-dev \
	qtdeclarative5-dev-tools \
	qtconnectivity5-dev \
	qt5keychain-dev \
	qt5-style-plugins \
	libkf5texteditor-dev

## Windows
TBD
However... Will require a whole bunch of the KTextEditor (KDE5) depencies to be drawn in...

## Clone
Clone submodules as well:

 git clone --recursive https://github.com/leviathanch/qtflow.git

Change into repo:

 cd qtflow

## Configure
Create build dir:

 mkdir build

Change into it:

 cd build

Run CMake:

 cmake ..

## Build
Compile with threads:

 make -j6 

