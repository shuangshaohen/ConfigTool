#ifndef HBASESPINBOX_H
#define HBASESPINBOX_H

#include <QSpinBox>
#include "CommGui_global.h"

class HBaseSpinBoxPrivate;
class COMMGUI_EXPORT HBaseSpinBox : public QSpinBox
{
    Q_OBJECT
public:
    explicit HBaseSpinBox(QWidget *parent = nullptr);
    ~HBaseSpinBox();

    virtual void fixup(QString& input) const;
    virtual QValidator::State validate(QString& input, int& pos) const;
    int base() const;
    void setBase(int b);
protected:
    virtual QString textFromValue(int value) const;
    virtual int valueFromText(const QString& text) const;
private:
    HBaseSpinBoxPrivate * d;
};

#endif // HBASESPINBOX_H
