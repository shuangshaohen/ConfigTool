#include "CSavePara.h"
#include <QFile>
#include <QTextStream>
#include <QDomElement>
#include <QDebug>
#include <QDateTime>

CSavePara::CSavePara()
{

}

void CSavePara::Save(const QString &fileName, tdsTopoPara *ptTopoPara)
{
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly|QFile::Text))
    {
        qDebug()<<QString(" save %1 failed:\n%2.")
                  .arg(fileName)
                  .arg(file.errorString()) ;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");

    m_doc.clear();

    //保存头
    QDomProcessingInstruction instruction = m_doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    m_doc.appendChild(instruction);

    QDomElement root = m_doc.createElement("root");//创建根节点  QDomElemet元素

    QDateTime * time = new QDateTime();
    QString strTime = time->currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz");
    ptTopoPara->date = strTime.toStdString();

    QDomElement topoInfo = m_doc.createElement("TopoInfo");
    topoInfo.setAttribute("name",     QString::fromStdString(ptTopoPara->name));
    topoInfo.setAttribute("desc",     QString::fromStdString(ptTopoPara->desc));
    topoInfo.setAttribute("version",  QString::fromStdString(ptTopoPara->version));
    topoInfo.setAttribute("date",     QString::fromStdString(ptTopoPara->date));
    root.appendChild(topoInfo);

    SavePara(&root,ptTopoPara->ptPara);

    m_doc.appendChild(root);
    m_doc.save(out, 4, QDomNode::EncodingFromTextStream);
    file.close();

    qDebug() << QString(" save %1 success!")
                .arg(fileName);
}

void CSavePara::SavePara(QDomElement *parentDom, tdsPara *ptPara)
{
    QDomElement para = m_doc.createElement("Para");
    para.setAttribute("name",     QString::fromStdString(ptPara->key));
    para.setAttribute("desc",     QString::fromStdString(ptPara->desc));

    for(unsigned int i = 0 ; i < ptPara->items.size(); i++)
    {
        SaveItem(&para,ptPara->items[i]);
    }

    for(unsigned int i = 0 ; i < ptPara->sons.size(); i++)
    {
        SavePara(&para,ptPara->sons[i]);
    }

    parentDom->appendChild(para);
}

void CSavePara::SaveItem(QDomElement *parentDom, tdsItem *ptItem)
{
    QDomElement item = m_doc.createElement("Item");
    item.setAttribute("name",     QString::fromStdString(ptItem->name));
    item.setAttribute("desc",     QString::fromStdString(ptItem->desc));
    item.setAttribute("key",    QString::fromStdString(ptItem->key));
    parentDom->appendChild(item);
}
