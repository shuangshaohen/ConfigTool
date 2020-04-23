#ifndef CCFGTOOLBOX_H
#define CCFGTOOLBOX_H

#include <QTabWidget>
#include "CDataBase.h"

class Ui_ToolBox;
class CResourceTreeWnd;

class CCfgToolBox : public QTabWidget
{
    Q_OBJECT
private:
    Ui_ToolBox          *m_pUi_ToolBox;
    CResourceTreeWnd    *m_pResourceTreeWnd;
private slots:
    void CurrentPageChanged(int index);
public:
    CCfgToolBox(QWidget *parent);
    virtual ~CCfgToolBox(void);
    QSize sizeHint () const;
    void CreateResourceTreeNodes(CDataBase* pXercesXML);
    void RefreshResourceTreeNodes();
    void ShowCurrentNodeInfoWnd();

    void setIEDPage();
    void setParaPage();

    void ClearTreeNode();
    //void UpdateIEDTreeNode(DOMElement* pElement,bool bDel);
signals:

};

#endif // CCFGTOOLBOX_H
