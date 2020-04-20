#include "CBitSelectedDialog.h"
#include "ui_BitSelectedDialog.h"

CBitSelectedDialog::CBitSelectedDialog(unsigned int val , QStringList list, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CBitSelectedDialog)
{
    ui->setupUi(this);

    setWindowTitle("位设置");

    m_boxes.clear();
    m_boxes.push_back(ui->checkBox_1);
    m_boxes.push_back(ui->checkBox_2);
    m_boxes.push_back(ui->checkBox_3);
    m_boxes.push_back(ui->checkBox_4);
    m_boxes.push_back(ui->checkBox_5);
    m_boxes.push_back(ui->checkBox_6);
    m_boxes.push_back(ui->checkBox_7);
    m_boxes.push_back(ui->checkBox_8);
    m_boxes.push_back(ui->checkBox_9);
    m_boxes.push_back(ui->checkBox_10);
    m_boxes.push_back(ui->checkBox_11);
    m_boxes.push_back(ui->checkBox_12);
    m_boxes.push_back(ui->checkBox_13);
    m_boxes.push_back(ui->checkBox_14);
    m_boxes.push_back(ui->checkBox_15);
    m_boxes.push_back(ui->checkBox_16);
    m_boxes.push_back(ui->checkBox_17);
    m_boxes.push_back(ui->checkBox_18);
    m_boxes.push_back(ui->checkBox_19);
    m_boxes.push_back(ui->checkBox_20);
    m_boxes.push_back(ui->checkBox_21);
    m_boxes.push_back(ui->checkBox_22);
    m_boxes.push_back(ui->checkBox_23);
    m_boxes.push_back(ui->checkBox_24);
    m_boxes.push_back(ui->checkBox_25);
    m_boxes.push_back(ui->checkBox_26);
    m_boxes.push_back(ui->checkBox_27);
    m_boxes.push_back(ui->checkBox_28);
    m_boxes.push_back(ui->checkBox_29);
    m_boxes.push_back(ui->checkBox_30);
    m_boxes.push_back(ui->checkBox_31);
    m_boxes.push_back(ui->checkBox_32);

    for(int i = 0; i < list.size() && i < m_boxes.size(); i++)
    {
        m_boxes[i]->setText(list[i]);
    }

    unsigned int cmp = 0x1;
    for (int i = 0; i < m_boxes.size(); i++)
    {
        QCheckBox * pCheckBox = m_boxes.at(i);

        if(val & cmp)
            pCheckBox->setChecked(true);
        else
            pCheckBox->setChecked(false);

        cmp = cmp << 1;
    }
}

CBitSelectedDialog::~CBitSelectedDialog()
{
    delete ui;
}

unsigned int CBitSelectedDialog::getVal()
{
    unsigned int val = 0;
    unsigned int cmp = 0x1;
    for (int i = 0; i < m_boxes.size(); i++)
    {
        QCheckBox * pCheckBox = m_boxes.at(i);

        if(pCheckBox->isChecked())
            val += cmp;

        cmp = cmp << 1;
    }
    return val;
}
