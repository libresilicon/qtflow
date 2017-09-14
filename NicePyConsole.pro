TARGET   = NicePyConsole
TEMPLATE = app

DESTDIR  = ../../lib

contains(QT_MAJOR_VERSION, 5) {
  QT += widgets
}

mac:CONFIG-= app_bundle

include ( ../../build/common.prf )  
include ( ../../build/PythonQt.prf )  
include ( ../../build/PythonQt_QtAll.prf )


HEADERS +=                  \
  SimpleConsole.h           \
  NicePyConsole.h           \
  PygmentsHighlighter.h     \
  PythonCompleter.h         \
  PythonCompleterPopup.h
  
SOURCES +=                  \
  SimpleConsole.cpp         \
  NicePyConsole.cpp         \  
  main.cpp                  \
  PygmentsHighlighter.cpp   \
  PythonCompleter.cpp       \
  PythonCompleterPopup.cpp

OTHER_FILES +=              \
  PygmentsHighlighter.py    \
  PythonCompleter.py        \
  module_completion.py

RESOURCES += \
    NPC.qrc
