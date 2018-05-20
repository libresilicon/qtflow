# qtflow
A free and open source VLSI development EDA based on Qt5 and C++

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
For OpenSUSE just install the Qt5 development pattern:

	sudo zypper in patterns-kde-devel_qt5 

### Ubuntu 18.04 LTS
Tested, works on Ubuntu 18.04 LTS out of the box. On Ubuntu 16.04 LTS the package Gt is too old.

Install packages:

	sudo apt-get cmake 
		git \ 
		install \
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
		libkf5texteditor-dev \
                libqt5svg5-dev \
                gettext \
                qttools5-dev \
                libqt5webkit5-dev \
                qtmultimedia5-dev \
                libqt5xmlpatterns5-dev

## Windows
TBD
However... Will require a whole bunch of the KTextEditor (KDE5) depencies to be drawn in...

## Clone
Clone submodules as well:

	git clone --recursive https://github.com/libresilicon/qtflow.git

Change into repo:

	cd qtflow

## Configure
Create build dir:

	mkdir build

Change into it:

	cd build

In CMake you can choose to build the full EDA (BUILD_FULL_EDA) , only the separate stand alone tools (BUILD_STAND_ALONE_BINARIES) or both:

In order to build everything you can run:

	cmake -DBUILD_FULL_EDA=ON BUILD_STAND_ALONE_BINARIES=ON ..

For only the tools:

	cmake -DBUILD_FULL_EDA=OFF BUILD_STAND_ALONE_BINARIES=ON ..

Or only the EDA:

	cmake -DBUILD_FULL_EDA=ON BUILD_STAND_ALONE_BINARIES=OFF ..

## Build
Compile with threads:

	make -j 

