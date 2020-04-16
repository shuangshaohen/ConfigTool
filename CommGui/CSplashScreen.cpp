#include <QtGui>
//#include "CPubClass.h"
#include "CSplashScreen.h"

CSplashScreen *CSplashScreen::Instance()
{
    static CSplashScreen *_pSplashScreen = NULL;

    if( NULL==_pSplashScreen)
    {
        _pSplashScreen = new CSplashScreen();
        if( false/*CPubClass::IsDebug()*/ )
        {
            _pSplashScreen->m_bDebug = true;
        }
        else
        {
            _pSplashScreen->m_bDebug = false;
        }
    }

    return _pSplashScreen;

}


CSplashScreen::CSplashScreen(): QSplashScreen( QPixmap("://Icon/SplashSreenImage.png"), Qt::WindowStaysOnTopHint )//
{
     m_pTimer = new QTimer(this);
     connect(m_pTimer, SIGNAL(timeout()), this, SLOT(update()));
}

void CSplashScreen::update()
{
    if( !isHidden())
    {
        m_nMilliseconds = m_Time.elapsed();
        m_strTime = QString( "耗时: %1ms").arg(m_nMilliseconds);
        repaint();
    }
}

void CSplashScreen::StartShow()
{
    if( isHidden() && !m_bDebug )
    {
        show();
        m_strTime = "耗时: 0ms";
        m_Time.restart();
        m_nMilliseconds = 0;
        m_pTimer->start(500);
    }
}

void CSplashScreen::EndShow()
{
    hide();
    if( !m_bDebug )
    {
        m_nMilliseconds = m_Time.elapsed();
        m_strTime = QString( "耗时: %1ms").arg(m_nMilliseconds);

        QString str = m_strContent + QString( "----耗时%1ms").arg(m_nMilliseconds);
        qDebug() << str;
        m_pTimer->start(500);
    }

}

void CSplashScreen::drawContents ( QPainter * painter )
{
    QRect r = rect();
    int nX = r.x();
    int nY = r.y();
    int nW = r.width();
    int nH = r.height();
    painter->setPen(QColor(Qt::white));
    painter->drawText( nX + 10, nY + nH - 25, m_strContent );
    painter->drawText( nX + nW - 100, nY + 20, m_strTime );
    QFont font;
    font.setFamily(tr("微软雅黑"));
    font.setPointSize(9);
    painter->setFont(font);
    painter->drawText( nX + nW - 235, nY + nH - 10, "Cfg配置工具V1.00" );
}

void CSplashScreen::ShowMessage ( const QString & message )
{
    m_strContent = message;
    repaint();
}
