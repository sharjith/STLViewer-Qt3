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

// Qt includes
#include <qvbox.h>
#include <qaccel.h>
#include <qcolordialog.h>
#include <qdesktopwidget.h>

// application specific includes
#include "StLViewerView.h"
#include "StLViewerDoc.h"
#include "StLViewer.h"

#include "res/appicon.xpm"

#include "res/filenew.xpm"
#include "res/fileopen.xpm"
#include "res/filesave.xpm"

#include "res/top.xpm"
#include "res/bottom.xpm"
#include "res/front.xpm"
#include "res/back.xpm"
#include "res/left.xpm"
#include "res/right.xpm"
#include "res/axo.xpm"
#include "res/reset.xpm"
#include "res/fitall.xpm"
#include "res/zoomwin.xpm"
#include "res/dmode.xpm"
#include "res/hlr.xpm"
#include "res/antial.xpm"
#include "res/stl.xpm"

#ifndef VERSION
#define VERSION
#endif

StLViewerApp::StLViewerApp()
{
    setCaption(tr("StLViewer " VERSION ) );
    setIcon(QPixmap(appicon));
    printer = new QPrinter;
    untitledCount=0;
    pDocList = new QList<StLViewerDoc>();
    pDocList->setAutoDelete(true);

    ///////////////////////////////////////////////////////////////////
    // call inits to invoke all other construction parts
    initView();
    initActions();
    initMenuBar();
    initToolBar();
    initStatusBar();

    QDesktopWidget *d = QApplication::desktop();
    int w = d->width();     // returns desktop width
    int h = d->height();    // returns desktop height
    resize( w, h );

    hb = new QHBox(statusBar());
    hb->setSpacing(2);
    progBar = new QProgressBar(hb);
    cViewer = new QLabel(hb);
    statusBar()->addWidget(hb,0,TRUE);

    viewToolBar->setOn(true);
    viewStatusBar->setOn(true);
}

StLViewerApp::~StLViewerApp()
{
    delete printer;
}

void StLViewerApp::initActions()
{
    QPixmap openIcon, saveIcon, newIcon;
    newIcon = QPixmap(filenew);
    openIcon = QPixmap(fileopen);
    saveIcon = QPixmap(filesave);

    //viewer toolbar
    QPixmap topIcon, bottomIcon, frontIcon, backIcon, leftIcon, rightIcon, axoIcon,
	    resetIcon, fitallIcon, zoomwinIcon, dmodeIcon, hlrIcon, antialIcon;
    topIcon = QPixmap(top);
    bottomIcon = QPixmap(bottom);
    frontIcon = QPixmap(front);
    backIcon = QPixmap(back);
    leftIcon = QPixmap(left_view);
    rightIcon = QPixmap(right_view);
    axoIcon = QPixmap(axo);
    resetIcon = QPixmap(reset_xpm);
    fitallIcon = QPixmap(fitall);
    zoomwinIcon = QPixmap(zoomwin);
    dmodeIcon = QPixmap(dmode);
    hlrIcon = QPixmap(hlr);
    antialIcon = QPixmap(antial);

    fileNew = new QAction(tr("New File"), newIcon, tr("&New"), QAccel::stringToKey(tr("Ctrl+N")), this);
    fileNew->setStatusTip(tr("Creates a new document"));
    fileNew->setWhatsThis(tr("New File\n\nCreates a new document"));
    connect(fileNew, SIGNAL(activated()), this, SLOT(slotFileNew()));

    fileOpen = new QAction(tr("Open File"), openIcon, tr("&Open..."), 0, this);
    fileOpen->setStatusTip(tr("Opens an existing document"));
    fileOpen->setWhatsThis(tr("Open File\n\nOpens an existing document"));
    connect(fileOpen, SIGNAL(activated()), this, SLOT(slotFileOpen()));

    fileSave = new QAction(tr("Save File"), saveIcon, tr("&Save"), QAccel::stringToKey(tr("Ctrl+S")), this);
    fileSave->setStatusTip(tr("Saves the actual document"));
    fileSave->setWhatsThis(tr("Save File.\n\nSaves the actual document"));
    connect(fileSave, SIGNAL(activated()), this, SLOT(slotFileSave()));

    fileSaveAs = new QAction(tr("Save File As"), tr("Save &as..."), 0, this);
    fileSaveAs->setStatusTip(tr("Saves the actual document under a new filename"));
    fileSaveAs->setWhatsThis(tr("Save As\n\nSaves the actual document under a new filename"));
    connect(fileSaveAs, SIGNAL(activated()), this, SLOT(slotFileSave()));

    fileClose = new QAction(tr("Close File"), tr("&Close"), QAccel::stringToKey(tr("Ctrl+W")), this);
    fileClose->setStatusTip(tr("Closes the actual document"));
    fileClose->setWhatsThis(tr("Close File\n\nCloses the actual document"));
    connect(fileClose, SIGNAL(activated()), this, SLOT(slotFileClose()));

    filePrint = new QAction(tr("Print File"), tr("&Print"), QAccel::stringToKey(tr("Ctrl+P")), this);
    filePrint->setStatusTip(tr("Prints out the actual document"));
    filePrint->setWhatsThis(tr("Print File\n\nPrints out the actual document"));
    connect(filePrint, SIGNAL(activated()), this, SLOT(slotFilePrint()));

    fileQuit = new QAction(tr("Exit"), tr("E&xit"), QAccel::stringToKey(tr("Ctrl+Q")), this);
    fileQuit->setStatusTip(tr("Quits the application"));
    fileQuit->setWhatsThis(tr("Exit\n\nQuits the application"));
    connect(fileQuit, SIGNAL(activated()), this, SLOT(slotFileQuit()));

    editCut = new QAction(tr("Cut"), tr("Cu&t"), QAccel::stringToKey(tr("Ctrl+X")), this);
    editCut->setStatusTip(tr("Cuts the selected section and puts it to the clipboard"));
    editCut->setWhatsThis(tr("Cut\n\nCuts the selected section and puts it to the clipboard"));
    connect(editCut, SIGNAL(activated()), this, SLOT(slotEditCut()));

    editCopy = new QAction(tr("Copy"), tr("&Copy"), QAccel::stringToKey(tr("Ctrl+C")), this);
    editCopy->setStatusTip(tr("Copies the selected section to the clipboard"));
    editCopy->setWhatsThis(tr("Copy\n\nCopies the selected section to the clipboard"));
    connect(editCopy, SIGNAL(activated()), this, SLOT(slotEditCopy()));

    editUndo = new QAction(tr("Undo"), tr("&Undo"), QAccel::stringToKey(tr("Ctrl+Z")), this);
    editUndo->setStatusTip(tr("Reverts the last editing action"));
    editUndo->setWhatsThis(tr("Undo\n\nReverts the last editing action"));
    connect(editUndo, SIGNAL(activated()), this, SLOT(slotEditUndo()));

    editPaste = new QAction(tr("Paste"), tr("&Paste"), QAccel::stringToKey(tr("Ctrl+V")), this);
    editPaste->setStatusTip(tr("Pastes the clipboard contents to actual position"));
    editPaste->setWhatsThis(tr("Paste\n\nPastes the clipboard contents to actual position"));
    connect(editPaste, SIGNAL(activated()), this, SLOT(slotEditPaste()));

    viewToolBar = new QAction(tr("Toolbar"), tr("Tool&bar"), 0, this, 0, true);
    viewToolBar->setStatusTip(tr("Enables/disables the toolbar"));
    viewToolBar->setWhatsThis(tr("Toolbar\n\nEnables/disables the toolbar"));
    connect(viewToolBar, SIGNAL(toggled(bool)), this, SLOT(slotViewToolBar(bool)));

    viewStatusBar = new QAction(tr("Statusbar"), tr("&Statusbar"), 0, this, 0, true);
    viewStatusBar->setStatusTip(tr("Enables/disables the statusbar"));
    viewStatusBar->setWhatsThis(tr("Statusbar\n\nEnables/disables the statusbar"));
    connect(viewStatusBar, SIGNAL(toggled(bool)), this, SLOT(slotViewStatusBar(bool)));

    windowNewWindow = new QAction(tr("New Window"), tr("&New Window"), 0, this);
    windowNewWindow->setStatusTip(tr("Opens a new view for the current document"));
    windowNewWindow->setWhatsThis(tr("New Window\n\nOpens a new view for the current document"));
    connect(windowNewWindow, SIGNAL(activated()), this, SLOT(slotWindowNewWindow()));

    windowCascade = new QAction(tr("Cascade"), tr("&Cascade"), 0, this);
    windowCascade->setStatusTip(tr("Cascades all windows"));
    windowCascade->setWhatsThis(tr("Cascade\n\nCascades all windows"));
    connect(windowCascade, SIGNAL(activated()), pWorkspace, SLOT(cascade()));

    windowTile = new QAction(tr("Tile"), tr("&Tile"), 0, this);
    windowTile->setStatusTip(tr("Tiles all windows"));
    windowTile->setWhatsThis(tr("Tile\n\nTiles all windows"));
    connect(windowTile, SIGNAL(activated()), pWorkspace, SLOT(tile()));


    windowAction = new QActionGroup(this, 0, false);
    windowAction->insert(windowNewWindow);
    windowAction->insert(windowCascade);
    windowAction->insert(windowTile);

    helpAboutApp = new QAction(tr("About"), tr("&About..."), 0, this);
    helpAboutApp->setStatusTip(tr("About the application"));
    helpAboutApp->setWhatsThis(tr("About\n\nAbout the application"));
    connect(helpAboutApp, SIGNAL(activated()), this, SLOT(slotHelpAbout()));

    //viewer actions
    viewTop = new QAction(tr("Top View"), topIcon, tr("Top"), QAccel::stringToKey(tr("Ctrl+T")), this);
    viewTop->setStatusTip(tr("Displays The Top View"));
    viewTop->setWhatsThis(tr("Top View\n\nDisplays The Top View"));
    connect(viewTop, SIGNAL(activated()), this, SLOT(slotTopView()));

    viewBottom = new QAction(tr("Bottom View"), bottomIcon, tr("Bottom"), QAccel::stringToKey(tr("Ctrl+B")), this);
    viewBottom->setStatusTip(tr("Displays The Bottom View"));
    viewBottom->setWhatsThis(tr("Bottom View\n\nDisplays The Bottom View"));
    connect(viewBottom, SIGNAL(activated()), this, SLOT(slotBottomView()));

    viewFront = new QAction(tr("Front View"), frontIcon, tr("Front"), QAccel::stringToKey(tr("Ctrl+F")), this);
    viewFront->setStatusTip(tr("Displays The Front View"));
    viewFront->setWhatsThis(tr("Front View\n\nDisplays The Front View"));
    connect(viewFront, SIGNAL(activated()), this, SLOT(slotFrontView()));

    viewBack = new QAction(tr("Back View"), backIcon, tr("Back"), QAccel::stringToKey(tr("Shift+B")), this);
    viewBack->setStatusTip(tr("Displays The Back View"));
    viewBack->setWhatsThis(tr("Back View\n\nDisplays The Back View"));
    connect(viewBack, SIGNAL(activated()), this, SLOT(slotBackView()));

    viewLeft = new QAction(tr("Left View"), leftIcon, tr("Left"), QAccel::stringToKey(tr("Ctrl+L")), this);
    viewLeft->setStatusTip(tr("Displays The Left View"));
    viewLeft->setWhatsThis(tr("Left View\n\nDisplays The Left View"));
    connect(viewLeft, SIGNAL(activated()), this, SLOT(slotLeftView()));

    viewRight = new QAction(tr("Right View"), rightIcon, tr("Right"), QAccel::stringToKey(tr("Ctrl+R")), this);
    viewRight->setStatusTip(tr("Displays The Right View"));
    viewRight->setWhatsThis(tr("Right View\n\nDisplays The Right View"));
    connect(viewRight, SIGNAL(activated()), this, SLOT(slotRightView()));

    viewAxo = new QAction(tr("Axo View"), axoIcon, tr("Axo"), QAccel::stringToKey(tr("Ctrl+A")), this);
    viewAxo->setStatusTip(tr("Displays The Axonometric View"));
    viewAxo->setWhatsThis(tr("Axo View\n\nDisplays The Axonometric View"));
    connect(viewAxo, SIGNAL(activated()), this, SLOT(slotAxoView()));

    viewFitall = new QAction(tr("Fit All"), fitallIcon, tr("Fit All"), QAccel::stringToKey(tr("F")), this);
    viewFitall->setStatusTip(tr("Fits The View To Screen"));
    viewFitall->setWhatsThis(tr("Fit All\n\nFits The View To Screen"));
    connect(viewFitall, SIGNAL(activated()), this, SLOT(slotFitall()));

    viewZoomwin = new QAction(tr("Zoom Win"), zoomwinIcon, tr("Zoom Win"), QAccel::stringToKey(tr("Z")), this);
    viewZoomwin->setStatusTip(tr("Zooms The View Defined By A Window"));
    viewZoomwin->setWhatsThis(tr("Zoom Win\n\nZooms The View Defined By A Window"));
    connect(viewZoomwin, SIGNAL(activated()), this, SLOT(slotZoomwin()));

    viewReset = new QAction(tr("Reset View"), resetIcon, tr("Axo"), 0, this);
    viewReset->setStatusTip(tr("Resets The View"));
    viewReset->setWhatsThis(tr("Reset View\n\nResets The View"));
    connect(viewReset, SIGNAL(activated()), this, SLOT(slotReset()));

    viewDmode = new QAction(tr("Display Mode"), dmodeIcon, tr("Display Mode"), 0, this);
    viewDmode->setStatusTip(tr("Toggles The Display Mode To Shaded And Wireframe"));
    viewDmode->setWhatsThis(tr("Display Mode\n\nToggles The Display Mode To Shaded And Wireframe"));
    connect(viewDmode, SIGNAL(activated()), this, SLOT(slotDmode()));

    viewHLR = new QAction(tr("HLR View"), hlrIcon, tr("HLR"), 0, this);
    viewHLR->setStatusTip(tr("Hidden Line Removal"));
    viewHLR->setWhatsThis(tr("HLR\n\nDisplays Hidden Lines Removed"));
    connect(viewHLR, SIGNAL(activated()), this, SLOT(slotHLR()));

    viewAntial = new QAction(tr("Antialias View"), antialIcon, tr("HLR"), 0, this, 0, false);
    viewAntial->setStatusTip(tr("Toggle Antialias"));
    viewAntial->setWhatsThis(tr("Toggle Antialias\n\nToggles Antialiasing On And Off"));
    connect(viewAntial, SIGNAL(activated()), this, SLOT(slotAntial()));

    QPixmap stlIcon = QPixmap(stl);
    //stl action
    readStL = new QAction(tr("Read StL File"), stlIcon, tr("Read StL File"), 0, this, 0, false);
    readStL->setStatusTip(tr("Read StL Data File"));
    readStL->setWhatsThis(tr("Read StL Data File\n\nOpen And Read An StereoLithography Data File"));
    connect(readStL, SIGNAL(activated()), this, SLOT(slotReadStLFile()));

    //popup
    bgColor = new QAction(tr("Change Background Color"), tr("Change Background Color"), 0, this);
    bgColor->setStatusTip(tr("Change Background Color Of Viewer"));
    bgColor->setWhatsThis(tr("Change Background Color\n\nChange Background Color Of Viewer"));
    connect(bgColor, SIGNAL(activated()), this, SLOT(slotBackColor()));

    displayAll = new QAction(tr("Display All"), tr("Display All"), 0, this);
    displayAll->setStatusTip(tr("Display All"));
    displayAll->setWhatsThis(tr("Display All\n\nDisplay All Objects In Context"));
    connect(displayAll, SIGNAL(activated()), this, SLOT(slotDisplayAll()));

    objColor = new QAction(tr("Color"), tr("Color"), 0, this);
    objColor->setStatusTip(tr("Set Color"));
    objColor->setWhatsThis(tr("Set Color\n\nSet Color Of Selected Object"));
    connect(objColor, SIGNAL(activated()), this, SLOT(slotColor()));

    objMaterial = new QAction(tr("Material"), tr("Material"), 0, this);
    objMaterial->setStatusTip(tr("Set Material"));
    objMaterial->setWhatsThis(tr("Set Material\n\nSet Material Of Selected Object"));
    connect(objMaterial, SIGNAL(activated()), this, SLOT(slotMaterial()));

    eraseObj = new QAction(tr("Erase"), tr("Erase"), 0, this);
    eraseObj->setStatusTip(tr("Erase Selected"));
    eraseObj->setWhatsThis(tr("Erase Selected\n\nErase The Selected Object"));
    connect(eraseObj, SIGNAL(activated()), this, SLOT(slotErase()));

    deleteObj = new QAction(tr("Delete"), tr("Delete"), 0, this);
    deleteObj->setStatusTip(tr("Delete Selected"));
    deleteObj->setWhatsThis(tr("Delete Selected\n\nDelete The Selected Object"));
    connect(deleteObj, SIGNAL(activated()), this, SLOT(slotDelete()));

}

void StLViewerApp::initMenuBar()
{
    ///////////////////////////////////////////////////////////////////
    // MENUBAR

    ///////////////////////////////////////////////////////////////////
    // menuBar entry pFileMenu
    pFileMenu=new QPopupMenu();
    fileNew->addTo(pFileMenu);
    fileOpen->addTo(pFileMenu);
    fileClose->addTo(pFileMenu);
    pFileMenu->insertSeparator();
    fileSave->addTo(pFileMenu);
    fileSaveAs->addTo(pFileMenu);
    pFileMenu->insertSeparator();
    filePrint->addTo(pFileMenu);
    pFileMenu->insertSeparator();
    fileQuit->addTo(pFileMenu);

    ///////////////////////////////////////////////////////////////////
    // menuBar entry editMenu
    pEditMenu=new QPopupMenu();
    editUndo->addTo(pEditMenu);
    pEditMenu->insertSeparator();
    editCut->addTo(pEditMenu);
    editCopy->addTo(pEditMenu);
    editPaste->addTo(pEditMenu);

    ///////////////////////////////////////////////////////////////////
    // menuBar entry viewMenu
    pViewMenu=new QPopupMenu();
    pViewMenu->setCheckable(true);
    viewToolBar->addTo(pViewMenu);
    viewStatusBar->addTo(pViewMenu);
    ///////////////////////////////////////////////////////////////////
    // EDIT YOUR APPLICATION SPECIFIC MENUENTRIES HERE

    ///////////////////////////////////////////////////////////////////
    // menuBar entry windowMenu
    pWindowMenu = new QPopupMenu(this);
    pWindowMenu->setCheckable(true);
    connect(pWindowMenu, SIGNAL(aboutToShow()), this, SLOT(windowMenuAboutToShow()));

    ///////////////////////////////////////////////////////////////////
    // menuBar entry helpMenu
    pHelpMenu=new QPopupMenu();
    helpAboutApp->addTo(pHelpMenu);
    pHelpMenu->insertSeparator();
    pHelpMenu->insertItem(tr("What's &This"), this, SLOT(whatsThis()), SHIFT+Key_F1);


    menuBar()->insertItem(tr("&File"), pFileMenu);
    menuBar()->insertItem(tr("&Edit"), pEditMenu);
    menuBar()->insertItem(tr("&View"), pViewMenu);
    menuBar()->insertItem(tr("&Window"), pWindowMenu);
    menuBar()->insertSeparator();
    menuBar()->insertItem(tr("&Help"), pHelpMenu);

}

void StLViewerApp::initToolBar()
{
    ///////////////////////////////////////////////////////////////////
    // TOOLBAR
    fileToolbar = new QToolBar(this, "file operations");
    fileNew->addTo(fileToolbar);
    fileOpen->addTo(fileToolbar);
    readStL->addTo(fileToolbar);
    fileSave->addTo(fileToolbar);
    fileToolbar->addSeparator();
    QWhatsThis::whatsThisButton(fileToolbar);

}

void StLViewerApp::initStatusBar()
{
    ///////////////////////////////////////////////////////////////////
    //STATUSBAR
    statusBar()->message(tr("Ready."));
}

void StLViewerApp::initView()
{ 
    ////////////////////////////////////////////////////////////////////
    // set the main widget here
    QVBox* view_back = new QVBox( this );
    view_back->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
    pWorkspace = new QWorkspace( view_back );
    pWorkspace->setScrollBarsEnabled(true);
    setCentralWidget(view_back);
}


void StLViewerApp::createClient(StLViewerDoc* doc)
{
    QPixmap appIcon = QPixmap(appicon);
    MDIWindow* mw = new MDIWindow(pWorkspace, 0, 0);
    mw->setIcon(appIcon);
    StLViewerView* w = new StLViewerView(doc, mw,0,WDestructiveClose);
    mw->setCentralWidget(w);
    viewToolbar = new QToolBar(mw, " view Tool Bar");
    viewTop->addTo(viewToolbar);
    viewBottom->addTo(viewToolbar);
    viewFront->addTo(viewToolbar);
    viewBack->addTo(viewToolbar);
    viewLeft->addTo(viewToolbar);
    viewRight->addTo(viewToolbar);
    viewAxo->addTo(viewToolbar);
    viewToolbar->addSeparator();
    viewFitall->addTo(viewToolbar);
    viewZoomwin->addTo(viewToolbar);
    viewReset->addTo(viewToolbar);
    viewDmode->addTo(viewToolbar);
    viewHLR->addTo(viewToolbar);
    viewAntial->addTo(viewToolbar);
    w->installEventFilter(this);
    doc->addView(w);
    mw->showMaximized();
    cViewer->setFrameStyle( QFrame::StyledPanel | QFrame::Plain );
    cViewer->setMaximumSize(300,30);
    cViewer->setMinimumSize(300,30);
    //pWorkspace->tile();
}

void StLViewerApp::openDocumentFile(const char* file)
{
    statusBar()->message(tr("Opening file..."));
    StLViewerDoc* doc;
    // check, if document already open. If yes, set the focus to the first view
    for(doc=pDocList->first(); doc != 0; doc=pDocList->next())
    {
	if(doc->pathName()==file)
	{
	    StLViewerView* view=doc->firstView();	
	    view->setFocus();
	    return;
	}
    }
    doc = new StLViewerDoc();
    pDocList->append(doc);
    doc->newDocument();
    // Creates an untitled window if file is 0	
    if(!file)
    {
	untitledCount+=1;
	QString fileName=QString(tr("Untitled%1")).arg(untitledCount);
	doc->setPathName(fileName);
	doc->setTitle(fileName);
    }
    // Open the file
    else
    {
	if(!doc->openDocument(file))
	{
	    QMessageBox::critical(this, tr("Error !"),tr("Could not open document !"));
	    delete doc;
	    return;
	}
    }
    // create the window
    createClient(doc);

    statusBar()->message(tr("Ready."));
}

bool StLViewerApp::queryExit()
{
    int exit=QMessageBox::information(this, tr("Quit..."),
	    tr("Do your really want to quit?"),
	    QMessageBox::Ok, QMessageBox::Cancel);

    if (exit==1)
    {

    }
    else
    {

    };

    return (exit==1);
}

bool StLViewerApp::eventFilter(QObject* object, QEvent* event)
{
    if((event->type() == QEvent::Close)&&((StLViewerApp*)object!=this))
    {
	QCloseEvent* e=(QCloseEvent*)event;
	StLViewerView* pView=(StLViewerView*)object;
	StLViewerDoc* pDoc=pView->getDocument();
	if(pDoc->canCloseFrame(pView))
	{
	    pDoc->removeView(pView);
	    if(!pDoc->firstView())
		pDocList->remove(pDoc);

	    e->accept();
	}
	else
	    e->ignore();
    }
    return QWidget::eventFilter( object, event );    // standard event processing
}

/////////////////////////////////////////////////////////////////////
// SLOT IMPLEMENTATION
/////////////////////////////////////////////////////////////////////


void StLViewerApp::slotFileNew()
{
    statusBar()->message(tr("Creating new file..."));

    openDocumentFile();		

    statusBar()->message(tr("Ready."));
}

void StLViewerApp::slotFileOpen()
{
    statusBar()->message(tr("Opening file..."));

    QString fileName = QFileDialog::getOpenFileName(0,0,this);
    if (!fileName.isEmpty())
    {
	openDocumentFile(fileName);		
    }
    statusBar()->message(tr("Ready."));
}


void StLViewerApp::slotFileSave()
{
    statusBar()->message(tr("Saving file..."));

    StLViewerView* m = (StLViewerView*)pWorkspace->activeWindow();
    if( m )
    {
	StLViewerDoc* doc = m->getDocument();
	if(doc->title().contains(tr("Untitled")))
	    slotFileSaveAs();
	else
	    if(!doc->saveDocument(doc->pathName()))
		QMessageBox::critical (this, tr("I/O Error !"), tr("Could not save the current document !"));
    }

    statusBar()->message(tr("Ready."));
}

void StLViewerApp::slotFileSaveAs()
{
    statusBar()->message(tr("Saving file under new filename..."));
    QString fn = QFileDialog::getSaveFileName(0, 0, this);
    if (!fn.isEmpty())
    {
	StLViewerView* m = (StLViewerView*)pWorkspace->activeWindow();
	if( m )
	{
	    StLViewerDoc* doc = m->getDocument();
	    if(!doc->saveDocument(fn))
	    {
		QMessageBox::critical (this, tr("I/O Error !"), tr("Could not save the current document !"));
		return;
	    }
	    doc->changedViewList();
	}
    }
    statusBar()->message(tr("Ready."));
}

void StLViewerApp::slotFileClose()
{
    statusBar()->message(tr("Closing file..."));

    StLViewerView* m = (StLViewerView*)pWorkspace->activeWindow();
    if( m )
    {
	StLViewerDoc* doc=m->getDocument();
	doc->closeDocument();
    }

    statusBar()->message(tr("Ready."));
}

void StLViewerApp::slotFilePrint()
{
    statusBar()->message(tr("Printing..."));

    StLViewerView* m = (StLViewerView*) pWorkspace->activeWindow();
    if ( m )
	m->print( printer );

    statusBar()->message(tr("Ready."));
}

void StLViewerApp::slotFileQuit()
{ 
    statusBar()->message(tr("Exiting application..."));
    ///////////////////////////////////////////////////////////////////
    // exits the Application
    //  if(doc->isModified())
    //  {
    //    if(queryExit())
    //    {
    //      qApp->quit();
    //    }
    //    else
    //    {
    //
    //    };
    //  }
    //  else
    //  {
    qApp->quit();
    //  };

    statusBar()->message(tr("Ready."));
}

void StLViewerApp::slotEditUndo()
{
    statusBar()->message(tr("Reverting last action..."));

    StLViewerView* m = (StLViewerView*) pWorkspace->activeWindow();
    if ( m )
	//   m->undo();

	statusBar()->message(tr("Ready."));
}

void StLViewerApp::slotEditCut()
{
    statusBar()->message(tr("Cutting selection..."));

    StLViewerView* m = (StLViewerView*) pWorkspace->activeWindow();
    if ( m )
	//  m->cut();	

	statusBar()->message(tr("Ready."));
}

void StLViewerApp::slotEditCopy()
{
    statusBar()->message(tr("Copying selection to clipboard..."));

    StLViewerView* m = (StLViewerView*) pWorkspace->activeWindow();
    if ( m )
	//  m->copy();

	statusBar()->message(tr("Ready."));
}

void StLViewerApp::slotEditPaste()
{
    statusBar()->message(tr("Inserting clipboard contents..."));

    StLViewerView* m = (StLViewerView*) pWorkspace->activeWindow();
    if ( m )
	//   m->paste();

	statusBar()->message(tr("Ready."));
}


void StLViewerApp::slotViewToolBar(bool toggle)
{
    statusBar()->message(tr("Toggle toolbar..."));
    ///////////////////////////////////////////////////////////////////
    // turn Toolbar on or off
    if (toggle== false)
    {
	fileToolbar->hide();
    }
    else
    {
	fileToolbar->show();
    };

    statusBar()->message(tr("Ready."));
}

void StLViewerApp::slotViewStatusBar(bool toggle)
{
    statusBar()->message(tr("Toggle statusbar..."));
    ///////////////////////////////////////////////////////////////////
    //turn Statusbar on or off

    if (toggle == false)
    {
	statusBar()->hide();
    }
    else
    {
	statusBar()->show();
    }

    statusBar()->message(tr("Ready."));
}

void StLViewerApp::slotWindowNewWindow()
{
    statusBar()->message(tr("Opening new document view..."));

    StLViewerView* m = (StLViewerView*) pWorkspace->activeWindow();
    if ( m ){
	StLViewerDoc* doc = m->getDocument();
	createClient(doc);
    }

    statusBar()->message(tr("Ready."));
}

void StLViewerApp::slotHelpAbout()
{
    QMessageBox::about(this,tr("About..."),
	    tr("StLViewer\nVersion " VERSION
		"\n(c) 2003 by Sharjith"));
}

void StLViewerApp::slotStatusHelpMsg(const QString &text)
{
    ///////////////////////////////////////////////////////////////////
    // change status message of whole statusbar temporary (text, msec)
    statusBar()->message(text, 2000);
}

void StLViewerApp::windowMenuAboutToShow()
{
    pWindowMenu->clear();	
    windowNewWindow->addTo(pWindowMenu);
    windowCascade->addTo(pWindowMenu);
    windowTile->addTo(pWindowMenu);

    if ( pWorkspace->windowList().isEmpty() )
    {
	windowAction->setEnabled(false);
    }
    else
    {
	windowAction->setEnabled(true);
    }

    pWindowMenu->insertSeparator();

    QWidgetList windows = pWorkspace->windowList();
    for ( int i = 0; i < int(windows.count()); ++i )
    {
	int id = pWindowMenu->insertItem(QString("&%1 ").arg(i+1)+windows.at(i)->caption(), this, SLOT( windowMenuActivated( int ) ) );
	pWindowMenu->setItemParameter( id, i );
	pWindowMenu->setItemChecked( id, pWorkspace->activeWindow() == windows.at(i) );
    }
}

void StLViewerApp::windowMenuActivated( int id )
{
    QWidget* w = pWorkspace->windowList().at( id );
    if ( w )
	w->setFocus();
}

//viewer slots
void StLViewerApp::slotTopView()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	StLViewerView* view = (StLViewerView*)mw->centralWidget();
	if( view )
	{
	    view->setFocus();
	    view->GetView()->TopView();
	}
    }	
}

void StLViewerApp::slotBottomView()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	StLViewerView* view = (StLViewerView*)mw->centralWidget();
	if( view )
	{
	    view->setFocus();
	    view->GetView()->BottomView();
	}
    }
}

void StLViewerApp::slotFrontView()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	StLViewerView* view = (StLViewerView*)mw->centralWidget();
	if( view )
	{
	    view->setFocus();
	    view->GetView()->FrontView();
	}
    }
}

void StLViewerApp::slotBackView()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	StLViewerView* view = (StLViewerView*)mw->centralWidget();
	if( view )
	{
	    view->setFocus();
	    view->GetView()->BackView();
	}
    }
}

void StLViewerApp::slotLeftView()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	StLViewerView* view = (StLViewerView*)mw->centralWidget();
	if( view )
	{
	    view->setFocus();
	    view->GetView()->LeftView();
	}
    }
}

void StLViewerApp::slotRightView()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	StLViewerView* view = (StLViewerView*)mw->centralWidget();
	if( view )
	{
	    view->setFocus();
	    view->GetView()->RightView();
	}
    }
}

void StLViewerApp::slotAxoView()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	StLViewerView* view = (StLViewerView*)mw->centralWidget();
	if( view )
	{
	    view->setFocus();
	    view->GetView()->AxonView();
	}
    }
}

void StLViewerApp::slotFitall()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	StLViewerView* view = (StLViewerView*)mw->centralWidget();
	if( view )
	{
	    view->setFocus();
	    view->GetView()->FitAll();
	}
    }
}

void StLViewerApp::slotZoomwin()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	StLViewerView* view = (StLViewerView*)mw->centralWidget();
	if( view )
	{
	    view->setFocus();
	    view->winZoom = true;
	}
    }
}

void StLViewerApp::slotReset()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	StLViewerView* view = (StLViewerView*)mw->centralWidget();
	if( view )
	{
	    view->setFocus();
	    view->GetView()->ResetView();
	}
    }
}

void StLViewerApp::slotDmode()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	StLViewerView* view = (StLViewerView*)mw->centralWidget();
	if( view )
	{    	
	    view->setFocus();
	    view->dShaded = ! view->dShaded;
	    if(!view->dShaded)
		view->GetView()->SetDisplayMode(GLSHADED);
	    else
		view->GetView()->SetDisplayMode(GLWIREFRAME);
	}
    }
}

void StLViewerApp::slotHLR()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	StLViewerView* view = (StLViewerView*)mw->centralWidget();
	if( view )
	{
	    view->setFocus();
	    view->GetView()->SetDisplayMode(GLHLREMOVED);
	}
    }
}

void StLViewerApp::slotAntial()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	StLViewerView* view = (StLViewerView*)mw->centralWidget();
	if( view )
	{
	    view->setFocus();
	    view->antialiased = !view->antialiased;
	    if(!view->antialiased)
		view->GetView()->SetAntiAliasing(false);
	    else
		view->GetView()->SetAntiAliasing(true);
	}
    }
}

//stl slot
void StLViewerApp::slotReadStLFile()
{
    const QString s( QFileDialog::getOpenFileName( QString::null, "StL Files (*.stl *.STL)", this ) );
    if ( s.isEmpty() )
	return;

    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	StLViewerView* pView = (StLViewerView*)mw->centralWidget();
	if( pView )
	{
	    StLViewerDoc* pDoc=pView->getDocument();
	    pDoc->ReadStLFile(s);
	}
    }
}

//popup
void StLViewerApp::slotBackColor()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	StLViewerView* view = (StLViewerView*)mw->centralWidget();
	if( view )
	{	
	    view->setFocus();
	    QColor col(0,0,0);
	    col = QColorDialog::getColor ( col, this, 0 );
	    if(!col.isValid())
		return;
	    view->GetView()->SetBackgroundColor((GLfloat)col.red()/255.0, (GLfloat)col.green()/255.0, (GLfloat)col.blue()/255.0);
	}
    }
}

void StLViewerApp::slotDisplayAll()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	StLViewerView* pView = (StLViewerView*)mw->centralWidget();
	if( pView )
	{
	    StLViewerDoc* pDoc=pView->getDocument();
	    pDoc->GetDisplayContext()->DisplayAll();
	}
    }
}

void StLViewerApp::slotColor()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	StLViewerView* view = (StLViewerView*)mw->centralWidget();
	if( view )
	{	
	    view->setFocus();
	    QColor initial(255,0,0);
	    initial = QColorDialog::getColor ( initial, this, 0 );
	    if(!initial.isValid())
		return;
	    CGLDisplayContext* ctx = view->getDocument()->dContext;
	    for(ctx->InitSelected(); ctx->MoreSelected(); ctx->NextSelected())
	    {
		CGLObject* O = ctx->CurrentSelected();
		O->SetColor(initial.red(), initial.green(), initial.blue());
	    }
	}
    }
}

#include "MaterialDlg.h"
#include <GLMaterial.h>
void StLViewerApp::slotMaterial()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	StLViewerView* view = (StLViewerView*)mw->centralWidget();
	if( view )
	{	
	    view->setFocus();

	    MatlEditor e;	    
	    if(e.exec()==QDialog::Accepted)
	    {
		CGLMaterial mat = e.GetMaterial();
		CGLDisplayContext* ctx = view->getDocument()->dContext;
		for(ctx->InitSelected(); ctx->MoreSelected(); ctx->NextSelected())
		{
		    CGLObject* O = ctx->CurrentSelected();
		    O->SetMaterial(mat);
		}
	    }
	}
    }
}

void StLViewerApp::slotErase()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	StLViewerView* pView = (StLViewerView*)mw->centralWidget();
	if( pView )
	{
	    StLViewerDoc* pDoc=pView->getDocument();
	    pDoc->GetDisplayContext()->EraseSelected();
	}
    }
}

void StLViewerApp::slotDelete()
{
    MDIWindow* mw = (MDIWindow*)pWorkspace->activeWindow();
    if(mw)
    {
	StLViewerView* pView = (StLViewerView*)mw->centralWidget();
	if( pView )
	{
	    StLViewerDoc* pDoc=pView->getDocument();
	    pDoc->GetDisplayContext()->DeleteSelected();
	}
    }
}

