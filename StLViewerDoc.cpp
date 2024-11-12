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

// include files for Qt
#include <qdir.h>
#include <qfileinfo.h>
#include <qwidget.h>
#include <qmsgbox.h>
#include <qfiledialog.h>


// application specific includes
#include "StLViewer.h"
#include "StLViewerDoc.h"
#include "StLViewerView.h"

StLViewerDoc::StLViewerDoc()
{
  pViewList = new QList<StLViewerView>;
  pViewList->setAutoDelete(false);
  dContext = new CGLDisplayContext(this);
  m_Reader = 0;
  m_stlObj = 0;
}

StLViewerDoc::~StLViewerDoc()
{
  delete pViewList;
  delete dContext;
}

void StLViewerDoc::addView(StLViewerView *view)
{
  pViewList->append(view);
	changedViewList();
}

void StLViewerDoc::removeView(StLViewerView *view)
{
	  pViewList->remove(view);
	  if(!pViewList->isEmpty())
			changedViewList();
		else
			deleteContents();
}

void StLViewerDoc::changedViewList(){	
	
	StLViewerView *w;
	if((int)pViewList->count() == 1){
  	w=pViewList->first();
  	w->setCaption(m_title);
	}
	else{
		int i;
    for( i=1,w=pViewList->first(); w!=0; i++, w=pViewList->next())
  		w->setCaption(QString(m_title+":%1").arg(i));	
	}
}

bool StLViewerDoc::isLastView() {
  return ((int) pViewList->count() == 1);
}


void StLViewerDoc::updateAllViews(StLViewerView *sender)
{
  StLViewerView *w;
  for(w=pViewList->first(); w!=0; w=pViewList->next())
  {
     w->update(sender);
  }

}

void StLViewerDoc::setPathName(const QString &name)
{
  m_filename=name;
	m_title=QFileInfo(name).fileName();
}

const QString& StLViewerDoc::pathName() const
{
  return m_filename;
}

void StLViewerDoc::setTitle(const QString &title)
{
  m_title=title;
}

const QString &StLViewerDoc::title() const
{
  return m_title;
}


void StLViewerDoc::closeDocument()
{
  StLViewerView *w;
  if(!isLastView())
  {
    for(w=pViewList->first(); w!=0; w=pViewList->next())
    {
   	 	if(!w->close())
 				break;
    }
	}
  if(isLastView())
  {
  	w=pViewList->first();
  	w->close();
  }
}

bool StLViewerDoc::newDocument()
{
  /////////////////////////////////////////////////
  // TODO: Add your document initialization code here
  /////////////////////////////////////////////////
  modified=false;
  return true;
}

bool StLViewerDoc::openDocument(const QString &filename, const char *format /*=0*/)
{

/*	QFile f( filename );
	if ( !f.open( IO_ReadOnly ) )
		return false;
  /////////////////////////////////////////////////
  // TODO: Add your document opening code here
  /////////////////////////////////////////////////
	f.close();*/
	
	ReadStLFile(filename);
	
/*  modified=false;
  m_filename=filename;
	m_title=QFileInfo(f).fileName();*/
  return true;
}

bool StLViewerDoc::saveDocument(const QString &filename, const char *format /*=0*/)
{
	QFile f( filename );
	if ( !f.open( IO_WriteOnly ) )
		return false;

  /////////////////////////////////////////////////
  // TODO: Add your document saving code here
  /////////////////////////////////////////////////

  f.close();

  modified=false;
  m_filename=filename;
	m_title=QFileInfo(f).fileName();
  return true;
}

void StLViewerDoc::deleteContents()
{
  /////////////////////////////////////////////////
  // TODO: Add implementation to delete the document contents
  /////////////////////////////////////////////////

}

bool StLViewerDoc::canCloseFrame(StLViewerView* pFrame)
{
	if(!isLastView())
		return true;
		
	bool ret=false;
  if(isModified())
  {
		QString saveName;
  	switch(QMessageBox::information(pFrame, title(), tr("The current file has been modified.\n"
                          "Do you want to save it?"),QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel ))
    {
			case QMessageBox::Yes:
				if(title().contains(tr("Untitled")))
				{
					saveName=QFileDialog::getSaveFileName(0, 0, pFrame);
                                          if(saveName.isEmpty())
          	                                return false;
				}
				else
					saveName=pathName();

				if(!saveDocument(saveName))
				{
 					switch(QMessageBox::critical(pFrame, tr("I/O Error !"), tr("Could not save the current document !\n"
																												"Close anyway ?"),QMessageBox::Yes ,QMessageBox::No))

 					{
 						case QMessageBox::Yes:
 							ret=true;
 						case QMessageBox::No:
 							ret=false;
 					}
				}
				else
					ret=true;
				break;
			case QMessageBox::No:
				ret=true;
				break;
			case QMessageBox::Cancel:
			default:
				ret=false;
				break;
		}
	}
	else
		ret=true;

	return ret;
}

void StLViewerDoc::ReadStLFile(const QString& str)
{
  m_Reader = new CStLReader(str);
  if(m_Reader)
  {
      m_stlObj = new CStLObject(m_Reader);
      if(m_stlObj)
      {  
	  dContext->Display(m_stlObj);
	  m_stlObj = 0;
      }
      m_Reader = 0;
  }
}
