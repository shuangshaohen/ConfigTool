#include <QDebug>
#include <QStringBuilder>
#include <QLineEdit>
#include "HBaseSpinBox.h"

class HBaseSpinBoxPrivate
{
public:
    HBaseSpinBoxPrivate(HBaseSpinBox *spinbox);
    int validateAndInterpret(QString &input, int &pos, QValidator::State &state);
    int base;
    HBaseSpinBox * p;
};

HBaseSpinBoxPrivate::HBaseSpinBoxPrivate(HBaseSpinBox *spinbox)
    :base(10), p(spinbox)
{

}

int HBaseSpinBoxPrivate::validateAndInterpret(QString &input, int &pos, QValidator::State &state)
{
    Q_UNUSED(pos);
    QString copy = input;
    if (!p->prefix().isEmpty())
        copy = copy.mid(p->prefix().size());
    if (!p->suffix().isEmpty())
        copy.chop(p->suffix().size());
    const int max = p->maximum();
    const int min = p->minimum();
    state = QValidator::Acceptable;
    int num = min;
    if (!copy.size()) {
        //允许空输入
        state = QValidator::Intermediate;
    } else if (min<0 && copy[0]=='-'){
        state = QValidator::Intermediate;
    } else {
        bool ok = false;
        num = copy.toInt(&ok, base);
        if (!ok) {
            //转换失败，不可接受
            state = QValidator::Invalid;
        } else if (num >= min && num <= max) {
            //正常值
            state = copy == copy.toUpper()? QValidator::Acceptable : QValidator::Intermediate;
        } else if ((num >=0 && num > max) || (num < 0 && num < min)) {
            //数值越界且不可通过后续输入校正
            state = QValidator::Invalid;
        } else {
            state = QValidator::Intermediate;
        }
    }
    return num;
}

HBaseSpinBox::HBaseSpinBox(QWidget *parent)
    : QSpinBox(parent),d(new HBaseSpinBoxPrivate(this))
{
}
HBaseSpinBox::~HBaseSpinBox()
{
    delete d;
}
void HBaseSpinBox::fixup(QString &input) const
{
    QString copy = input;
    int pos = lineEdit()->cursorPosition();
    QValidator::State state;
    int num = d->validateAndInterpret(copy, pos, state);
    input = prefix() % QString::number(num, d->base).toUpper() % suffix();
    //qDebug()<<input;
}
QValidator::State HBaseSpinBox::validate(QString& input, int& pos) const
{
    QValidator::State state;
    d->validateAndInterpret(input, pos, state);
    return state;
}
int HBaseSpinBox::base() const
{
    return d->base;
}
void HBaseSpinBox::setBase(int b)
{
    if (b<2 || b>36)
        qWarning("base must between 2 and 36");
    d->base = qBound(2, b, 36);
}
QString HBaseSpinBox::textFromValue(int value) const
{
    //不包括前缀和后缀
    return QString::number(value, d->base).toUpper();
}
int HBaseSpinBox::valueFromText(const QString& text) const
{
    QString copy = text;
    int pos = lineEdit()->cursorPosition();
    QValidator::State state;
    return d->validateAndInterpret(copy, pos, state);
}
