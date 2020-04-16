#ifndef CCFGACTIONS_H
#define CCFGACTIONS_H

class CCfgMainWnd;

#include "CCommActions.h"

class CCfgActions : public CCommActions
{
    Q_OBJECT
public:
    CCfgActions(CCfgMainWnd *);
    void CreateActions();
    void CreateMenuAndToolBar();

    void CreateFileMenu();
    void CreateEditMenu();

    void CreateFileToolBar();
    void CreateEditFileToolBar();

    void CreateAppToolMenu();
    void CreateAppFileToolBar();

public:

private:
    CCfgMainWnd			*	m_pCfgMainWnd;

};

#endif // CCFGACTIONS_H
