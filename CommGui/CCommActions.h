#ifndef CCOMMACTIONS_H
#define CCOMMACTIONS_H

#include <QObject>
#include <QColor>
#include <QIcon>
#include <QMenuBar>
#include <QAction>
#include <QActionGroup>
#include "CommGui_global.h"

class CMainWnd;

QT_BEGIN_NAMESPACE

class QMenu;
class QAction;
class QToolBar;
class QActionGroup;
class QComboBox;
class QFontComboBox;
class QToolButton;
QT_END_NAMESPACE

class COMMGUI_EXPORT CCommActions : public QObject
{
    Q_OBJECT
public:

    CCommActions(CMainWnd *);
    virtual void CreateMenuAndToolBar()=0;

    virtual void CreateMenuBar();
    virtual void CreateFileMenu()=0;
    virtual	void CreateEditMenu()=0;
    virtual void CreateViewMenu();
    virtual void CreateVerifyMenu();
    virtual void CreateAppToolMenu()=0;
    virtual void CreateHelpMenu();

    virtual void CreateToolBar();
    virtual void CreateFileToolBar()=0;
    virtual void CreateSaveToolBar()=0;
    virtual	void CreateEditFileToolBar()=0;
    virtual void CreateVerifyFileToolBar();
    virtual void CreateAppFileToolBar()=0;

protected:

protected slots:

public:

    QMenuBar		*	m_pMenuBar;

    QMenu			*	m_pFileMenu;
    QMenu			*	m_pVerifyMenu;
    QMenu			*	m_pHelpMenu;
    QMenu			*	m_pEditMenu;
    QMenu			*	m_pViewMenu;
    QMenu			*	m_pAppToolMenu;

    QToolBar		*	m_pFileToolBar;
    QToolBar		*	m_pVerifyToolBar;
    QToolBar		*	m_pEditToolBar;
    QToolBar		*	m_pSaveToolBar;
    QToolBar		*	m_pAppToolBar;

    QAction			*	m_pAboutAction;

    QAction			*	m_pFileToolBarDockAction;
    QAction			*	m_pEditToolBarDockAction;
    QAction			*	m_pVerifyToolBarDockAction;
    QAction			*	m_pAppToolBarDockAction;
    QAction			*	m_pOutputWndDockAction;
    QAction			*	m_pToolBoxDockAction;

private:
    CMainWnd	*	m_pMainWnd;
};

#endif // CCOMMACTIONS_H
