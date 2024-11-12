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
#include <qprinter.h>
#include <qpainter.h>


// application specific includes
#include "StLViewerView.h"
#include "StLViewerDoc.h"


StLViewerView::StLViewerView(StLViewerDoc* pDoc, QWidget *parent, const char* name, int wflags)
 : QGLWidget(parent, name)
{
    doc=pDoc;
    myView = new QGLView(this, doc->GetDisplayContext());
    	ldown = mdown = rdown = false;
    	winZoom = false;
	dShaded = false;
	antialiased = false;
	gridOn = false;
	setMouseTracking(TRUE);
}

StLViewerView::~StLViewerView()
{
	delete myView;
}

StLViewerDoc *StLViewerView::getDocument() const
{
	return doc;
}

void StLViewerView::update(StLViewerView* pSender){
	if(pSender != this)
		repaint();
}

void StLViewerView::print(QPrinter *pPrinter)
{
  if (pPrinter->setup(this))
  {
		QPainter p;
		p.begin(pPrinter);
		
		///////////////////////////////
		// TODO: add your printing code here
		///////////////////////////////
		
		p.end();
  }
}

void StLViewerView::closeEvent(QCloseEvent*)
{
  // LEAVE THIS EMPTY: THE EVENT FILTER IN THE StLViewerApp CLASS TAKES CARE FOR CLOSING
  // QWidget closeEvent must be prevented.
}


void StLViewerView::initializeGL()
{
	myView->InitGL();
}

void StLViewerView::resizeGL(int w, int h)
{
	myView->ReSize(w, h);
}

void StLViewerView::paintGL()
{
	myView->Draw();
}
	
void StLViewerView::resizeEvent( QResizeEvent* e)
{
	QRect aRect = geometry();
	resizeGL(aRect.width(), aRect.height());
	update(NULL);
}

void StLViewerView::mousePressEvent( QMouseEvent *e)
{
	if(e->button() & QMouseEvent::LeftButton)
	{
		// ask window to give us events even if outside the view
    		grabMouse(ArrowCursor);

		//  save the current mouse coordinate in min
		myXmin=e->x();  myYmin=e->y();
		myXmax=e->x();  myYmax=e->y();

		ldown = true;
		lPoint.setX(e->x());
		lPoint.setY(e->y());
		if(!(e->state() & ShiftButton) && !(e->state() & ControlButton) && !winZoom)
		{
			myView->Select(e->x(), e->y());
			
		}
		if ((e->state() & ShiftButton) && !(e->state() & ControlButton) && !winZoom)
		{
			myView->MultiSelect(e->x(), e->y());
			
		}
	}
	if(e->button() & QMouseEvent::MidButton)
	{
		mdown = true;		
		mPoint.setX(e->x());
		mPoint.setY(e->y());
	}
	if(e->button() & QMouseEvent::RightButton)
	{
		rdown = true;
		rPoint.setX(e->x());
		rPoint.setY(e->y());
		if(!(e->state() & ControlButton) && !(e->state() & ShiftButton))
			Popup();
	}
}

void StLViewerView::mouseReleaseEvent( QMouseEvent *e)
{
	if(e->button() & QMouseEvent::LeftButton)
	{
		ldown = false;
		myView->StopZooming();
		
		if(!(e->state() & ShiftButton) && !(e->state() & ControlButton) && !winZoom)
		{
			DrawRectangle(myXmin,myYmin,myXmax,myYmax,false);
           	myXmax=e->x();
           	myYmax=e->y();
			QPoint point(e->x(), e->y());
			QRect arect(lPoint, point);
			arect.normalize();
			if(arect.width() && arect.height())
				myView->SweepSelect(arect);
			
		}

		if(winZoom)
		{
			DrawRectangle(myXmin,myYmin,myXmax,myYmax,false);
           	myXmax=e->x();
           	myYmax=e->y();
			QRect myZoomRect(myXmin, myYmax, abs(myXmax-myXmin), abs(myYmax-myYmin));
			if ((abs(myXmin-myXmax)>1) || (abs(myYmin-myYmax)>1))
			// Test if the zoom window size is not null
			{
				myView->ZoomWindow(myZoomRect);
			}
			
		}

		winZoom = false;
		releaseMouse();
	}
	if(e->button() & QMouseEvent::MidButton)
	{
		mdown = false;
		myView->StopPanning();
	}
	
	if(e->button() & QMouseEvent::RightButton)
	{
		myView->StopRotation();
		rdown = false;
	}
}

void StLViewerView::mouseMoveEvent( QMouseEvent *e)
{
	QPoint point(e->x(), e->y());
		
	StLViewerApp *top = (StLViewerApp*)topLevelWidget()->topLevelWidget();
	QString str;
	CPoint3D C;
	myView->ScreenToPoint(e->x(), e->y(), C);
	str = str.sprintf("X  %0.3lf  Y  %0.3lf  Z  %0.3lf",C.GetX(), C.GetY(), C.GetZ());
	top->cViewer->setText(str);
	
	if(ldown && !(e->state() & ControlButton) && !(e->state() & ShiftButton))
	{
		myXmax = point.x(); myYmax = point.y();
		DrawRectangle(myXmin,myYmin,myXmax,myYmax,false);	
		DrawRectangle(myXmin,myYmin,myXmax,myYmax,true);
	}
	if(e->state() & ControlButton)
	{
		if(ldown)
		{
			myView->ZoomView(lPoint, point);		
		}
		else if(mdown)
		{
			myView->PanView(mPoint, point);		
		}
		else if(rdown)
		{
			myView->RotateView(rPoint, point);				
		}
	}
}

void StLViewerView::SetDisplayMode(const GLDisplayMode& aMode)
{
	myView->SetDisplayMode(aMode);
}

void StLViewerView::DrawRectangle (const int  MinX  ,
			        const int  MinY  ,
                    const int  MaxX  ,
	    	        const int  MaxY  ,
		            const bool  Draw  )
{
	QPainter thePainter(this);
	thePainter.setRasterOp(Qt::XorROP);
	thePainter.setPen(Qt::white);
	
    static		int StoredMinX, StoredMaxX, StoredMinY, StoredMaxY;
    static		bool m_IsVisible;

	QRect aRect;
	if ( m_IsVisible && !Draw) // move or up  : erase at the old position
    {
	 aRect.setRect( StoredMinX, StoredMinY, abs(StoredMaxX-StoredMinX), abs(StoredMaxY-StoredMinY));
	 thePainter.drawRect(aRect);
	 m_IsVisible = FALSE;
    }

    StoredMinX = (MinX < MaxX) ? MinX: MaxX ;
    StoredMinY = (MinY < MaxY) ? MinY: MaxY ;
    StoredMaxX = (MinX > MaxX) ? MinX: MaxX ;
    StoredMaxY = (MinY > MaxY) ? MinY: MaxY ;

    if (Draw) // move : draw
    {
	 aRect.setRect( StoredMinX, StoredMinY, abs(StoredMaxX-StoredMinX), abs(StoredMaxY-StoredMinY));
	 thePainter.drawRect(aRect);
     m_IsVisible = TRUE;
   }
}

void StLViewerView::Popup()
{
	StLViewerApp *top = (StLViewerApp*)topLevelWidget()->topLevelWidget();
	if(doc->GetDisplayContext()->HasSelected())
	{
		QPopupMenu *pPopup = new QPopupMenu();
		pPopup->insertTearOffHandle();
		//pPopup->insertItem(tr("OBJECT"));
		pPopup->insertSeparator();
		top->objColor->addTo(pPopup);
		top->objMaterial->addTo(pPopup);
		pPopup->insertSeparator();
		top->eraseObj->addTo(pPopup);
		top->deleteObj->addTo(pPopup);
		pPopup->popup(QCursor::pos());
	}
	else
	{
		QPopupMenu *pPopup = new QPopupMenu();
		pPopup->insertTearOffHandle();
		//pPopup->insertItem(tr("VIEWER"));
		pPopup->insertSeparator();
		top->displayAll->addTo(pPopup);
		pPopup->insertSeparator();
		top->bgColor->addTo(pPopup);
		pPopup->popup(QCursor::pos());
	}
}
