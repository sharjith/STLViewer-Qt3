######################################################################
# Automatically generated by qmake (1.07a) Sun Jun 25 15:25:55 2006
######################################################################

TEMPLATE = app
CONFIG	+= qt warn_on debug opengl
DEFINES	+= 'VERSION="5.0"' QT_CLEAN_NAMESPACE
INCLUDEPATH += . /home/sharjith/include
LIBS +=		 -L/home/sharjith/lib64 \
		 -lVKGeom -lVKGraphic -lVKCollection -lglut

# Input
HEADERS += GLDisplayContext.h \
           ListIteratorOfListOfCTriFacet.h \
           ListNodeOfCTriFacet.h \
           ListOfCTriFacet.h \
           MaterialDlg.ui.h \
           MDIWindow.h \
           QGLView.h \
           StLObject.h \
           StLReader.h \
           StLViewer.h \
           StLViewerDoc.h \
           StLViewerView.h \
           TriFacet.h
INTERFACES += MaterialDlg.ui
SOURCES += GLDisplayContext.cpp \
           ListIteratorOfListOfCTriFacet.cpp \
           ListNodeOfCTriFacet.cpp \
           ListOfCTriFacet.cpp \
           main.cpp \
           MDIWindow.cpp \
           QGLView.cpp \
           StLObject.cpp \
           StLReader.cpp \
           StLViewer.cpp \
           StLViewerDoc.cpp \
           StLViewerView.cpp
