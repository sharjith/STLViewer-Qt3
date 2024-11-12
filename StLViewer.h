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

#ifndef STLVIEWER_H
#define STLVIEWER_H

// include files for QT
#include <qapp.h>
#include <qmainwindow.h>
#include <qaction.h>
#include <qworkspace.h>
#include <qmenubar.h>
#include <qtoolbar.h>
#include <qstatusbar.h>
#include <qprogressbar.h>
#include <qwhatsthis.h>
#include <qpopupmenu.h>
#include <qaccel.h>
#include <qtoolbutton.h>
#include <qmsgbox.h>
#include <qfiledialog.h>
#include <qprinter.h>
#include <qstring.h>
#include <qpixmap.h>
#include <qlabel.h>
#include <qhbox.h>

//OpenGL
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <MMath.h>
#include "MDIWindow.h"

// forward declaration of the StLViewer classes
class StLViewerDoc;
class StLViewerView;

/**
  * This Class is the base class for your application. It sets up the main
  * window and providing a menubar, toolbar
  * and statusbar. For the main view, an instance of class StLViewerView is
  * created which creates your view.
  * 	
  * @author Source Framework Automatically Generated by KDevelop, (c) The KDevelop Team.
  * @version KDevelop version 1.3 code generation
  */
class StLViewerApp : public QMainWindow
{
  Q_OBJECT
  
  public:
    /** construtor */
    StLViewerApp();
    /** destructor */
    ~StLViewerApp();

    /** opens a file specified by commandline option
     */
    void openDocumentFile(const char *file=0);

    QHBox* hb;
    QLabel *cViewer;

  protected:
    /** overloaded for Message box on last window exit */
    bool queryExit();
    /** event filter to catch close events for MDI child windows and is installed in createClient() on every child window.
      * Closing a window calls the eventFilter first which removes the view from the connected documents' view list. If the
      * last view is going to be closed, the eventFilter() tests if the document is modified; if yes, it asks the user to
      * save the document. If the document title contains "Untitled", slotFileSaveAs() gets called to get a save name and path.
      */
    virtual bool eventFilter(QObject* object, QEvent* event);
    /** creates a new child window. The document that will be connected to it
     * has to be created before and the instances filled, with e.g. openDocument().
     * Then call createClient() to get a new MDI child window.
     * @see StLViewerDoc#addView
     * @see StLViewerDoc#openDocument
     * @param doc pointer to the document instance that the view will
     * be connected to.
     */
    void createClient(StLViewerDoc* doc);

  private slots:

    /** generate a new document in the actual view */
    void slotFileNew();
    /** open a document */
    void slotFileOpen();
    /** save a document */
    void slotFileSave();
    /** save a document under a different filename*/
    void slotFileSaveAs();
    /** close the actual file */
    void slotFileClose();
    /** print the actual file */
    void slotFilePrint();
    /** exits the application */
    void slotFileQuit();

    /** reverts the last editing action */
    void slotEditUndo();
    /** put the marked text/object into the clipboard and remove
     * it from the document */
    void slotEditCut();
    /** put the marked text/object into the clipboard*/
    void slotEditCopy();
    /** paste the clipboard into the document*/
    void slotEditPaste();

    /** toggle the toolbar*/
    void slotViewToolBar(bool toggle);
    /** toggle the statusbar*/
    void slotViewStatusBar(bool toggle);
    /** creates a new view for the current document */
    void slotWindowNewWindow();
    /** shows an about dlg*/
    void slotHelpAbout();
    /** change the status message of the whole statusbar temporary */
    void slotStatusHelpMsg(const QString &text);
    /** gets called when the window menu is activated; recreates the window menu with all opened window titles. */
    void windowMenuAboutToShow();
    /** activates the MDI child widget when it gets selected from the window menu. */
    void windowMenuActivated( int id );

    //viewer slots
    void slotTopView();
    void slotBottomView();
    void slotFrontView();
    void slotBackView();
    void slotLeftView();
    void slotRightView();
    void slotAxoView();
    void slotFitall();
    void slotZoomwin();
    void slotReset();
    void slotDmode();
    void slotHLR();
    void slotAntial();

    //stl slot
    void slotReadStLFile();

    //popup
    void slotBackColor();
    void slotDisplayAll();

    void slotColor();
    void slotMaterial();
    void slotErase();
    void slotDelete();

  private:

    /** initializes all QActions of the application */
    void initActions();
    /** initMenuBar creates the menu_bar and inserts the menuitems */
    void initMenuBar();
    /** this creates the toolbars. Change the toobar look and add new toolbars in this
     * function */
    void initToolBar();
    /** setup the statusbar */
    void initStatusBar();
    /** setup the mainview*/
    void initView();

    /** file_menu contains all items of the menubar entry "File" */
    QPopupMenu *pFileMenu;
    /** edit_menu contains all items of the menubar entry "Edit" */
    QPopupMenu *pEditMenu;
    /** view_menu contains all items of the menubar entry "View" */
    QPopupMenu *pViewMenu;
    /** view_menu contains all items of the menubar entry "Help" */
    QPopupMenu *pHelpMenu;
    /** the window menu */
    QPopupMenu *pWindowMenu;

    QToolBar *fileToolbar;
    QToolBar *viewToolbar;

    /** pWorkspace is the MDI frame widget that handles MDI child widgets. Inititalized in
      * initView()
      */
    QWorkspace *pWorkspace;
    /** the printer instance */
    QPrinter *printer;
    /** a counter that gets increased each time the user creates a new document with "File"->"New" */
    int untitledCount;
    /** a list of all open documents. If the last window of a document gets closed, the installed eventFilter
     * removes this document from the list. The document list is checked for modified documents when the user
     * is about to close the application. */
    QList<StLViewerDoc> *pDocList;

    QAction *fileNew;
    QAction *fileOpen;
    QAction *fileSave;
    QAction *fileSaveAs;
    QAction *fileClose;
    QAction *filePrint;
    QAction *fileQuit;

    QAction *editUndo;
    QAction *editCut;
    QAction *editCopy;
    QAction *editPaste;

    QAction *viewToolBar;
    QAction *viewStatusBar;

    QActionGroup *windowAction;
    QAction *windowNewWindow;
    QAction *windowTile;
    QAction *windowCascade;

    QAction *helpAboutApp;

    //Viewer Actions;
    QAction *viewTop;
    QAction *viewBottom;
    QAction *viewFront;
    QAction *viewBack;
    QAction *viewLeft;
    QAction *viewRight;
    QAction *viewAxo;
    QAction *viewFitall;
    QAction *viewZoomwin;
    QAction *viewReset;
    QAction *viewDmode;
    QAction *viewHLR;
    QAction *viewAntial;

    //stl Action
    QAction *readStL;

public:
	//popup menu
    QAction *bgColor;
    QAction *displayAll;

    QAction *objColor;
    QAction *objMaterial;
    QAction *eraseObj;
    QAction *deleteObj;
    
    QProgressBar *progBar;
};
#endif
