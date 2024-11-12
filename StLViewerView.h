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

#ifndef STLVIEWERVIEW_H
#define STLVIEWERVIEW_H

#include "StLViewer.h"
#include "QGLView.h"

// include files for Qt
#include <qwidget.h>
#include <qgl.h>
#include <qlabel.h>
#include <qstring.h>
#include <qdatetime.h>
#include <qtimer.h>
#include <qgl.h>
#include <qvbox.h>

class StLViewerDoc;

/**
 * This class provides an incomplete base for your application view.
 * 	
 * @author Source Framework Automatically Generated by KDevelop, (c) The KDevelop Team.
 * @version KDevelop version 1.1 code generation
 */
class StLViewerView : public QGLWidget
{
  Q_OBJECT

  friend class StLViewerDoc;

  public:
    /** Constructor for the view
    	* @param pDoc  your document instance that the view represents. Create a document before calling the constructor
    	* or connect an already existing document to a new MDI child widget.*/
    StLViewerView(StLViewerDoc* pDoc, QWidget* parent, const char *name, int wflags);
    /** Destructor for the main view */
    virtual ~StLViewerView();
		/** returns a pointer to the document connected to the view*/
    StLViewerDoc *getDocument() const;
    /** gets called to redraw the document contents if it has been modified */
		void update(StLViewerView* pSender);
    /** contains the implementation for printing functionality and gets called by StLViewerApp::slotFilePrint() */
    void print(QPrinter *pPrinter);
		
  protected:
	virtual void closeEvent(QCloseEvent*);
	StLViewerDoc *doc;
	virtual void initializeGL();
	virtual void resizeGL(int w, int h);
	virtual void paintGL();

private:
	void Popup();
	void DrawRectangle (const int  MinX  ,
    					        const int  MinY  ,
                                const int  MaxX  ,
				    	        const int  MaxY  ,
					            const bool  Draw
                                );

public:
	bool winZoom;
	bool dShaded;
	bool antialiased;
	bool gridOn;
	QLabel *coordViewer;
private:
	bool ldown;
	bool mdown;
	bool rdown;
				
	QPoint  lPoint;
	QPoint  mPoint;
	QPoint  rPoint;	
		
	int     myXmin;
    int     myYmin;
    int     myXmax;
    int     myYmax;
	
protected:
	void mousePressEvent( QMouseEvent *);
  	void mouseReleaseEvent( QMouseEvent *);
  	void mouseMoveEvent( QMouseEvent *);
  	void resizeEvent( QResizeEvent *);
public:
	void SetDisplayMode(const GLDisplayMode& aMode);
	GLDisplayMode GetDisplayMode() const { return myView->GetDisplayMode(); }
	QGLView* GetView() const { return myView; }
private:
	QGLView* myView;
	
private:
	
};

#endif