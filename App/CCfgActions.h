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
    void CreateMenuAndToolBar() override;

    void CreateFileMenu() override;
    void CreateEditMenu() override;

    void CreateFileToolBar() override;
    void CreateEditFileToolBar() override;
    void CreateSaveToolBar() override;

    void CreateAppToolMenu() override;
    void CreateAppFileToolBar() override;

public:

private:
    CCfgMainWnd			*	m_pCfgMainWnd;

};

#endif // CCFGACTIONS_H
