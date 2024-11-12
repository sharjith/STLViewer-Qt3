//############################################################################

//  Author:	Sharjith Nair
//  Email:	sharjith_ssn@hotmail.com

// Program:	OpenGL, C++ Qt StL Data File Viewer Application

//							Important Notice:

//	This Idea and the Application is Copyright(c) Sharjith Nair 2002, 2003.
//	You can freely use it as long as you credit me for it.

//	No guarantee/warranty is given on this app and I will not be responsible
//	for any damage to you, your property or any other person from using it.
//	USE IT ON YOUR OWN RISK.

//	Thankyou
//	Sharjith Nair.

//#############################################################################

#include <qapplication.h>
#include <qfont.h>
#include <qstring.h>
#include <qtextcodec.h>
#include <qtranslator.h>
#include <qstylefactory.h>
#include <qdesktopwidget.h>

#include "StLViewer.h"

int main(int argc, char *argv[])
{
  //QApplication::setStyle(QStyleFactory::create("Motif"));
  //QApplication::setPalette(QPalette(QColor(127, 127, 127)), true);
  QApplication a(argc, argv);
  QTranslator tor( 0 );
  // set the location where your .qm files are in load() below as the last parameter instead of "."
  // for development, use "/" to use the english original as
  // .qm files are stored in the base project directory.
  tor.load( QString("stlviewer.") + QTextCodec::locale(), "." );
  a.installTranslator( &tor );

  StLViewerApp *stlviewer=new StLViewerApp();
  a.setMainWidget(stlviewer);

  stlviewer->show();

  if(argc>1)
    stlviewer->openDocumentFile(argv[1]);
	else
	  stlviewer->openDocumentFile();
	
  return a.exec();
}
