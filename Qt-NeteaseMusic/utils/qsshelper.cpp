#include "qsshelper.h"

#include <QPainter>
#include <QDebug>
QssHelper* QssHelper::pUniqueData = Q_NULLPTR;
QssHelper::QssHelper(QWidget *parent) :
    QWidget(parent)
{
    this->setFixedSize(0, 0);
    show();
    hide();
}

QssHelper::~QssHelper()
{
}

QssHelper *QssHelper::getInstance()
{
    if (NULL == pUniqueData) {
        pUniqueData = new QssHelper();
    }

    return pUniqueData;
}

void QssHelper::destroyInstance()
{
    delete pUniqueData;
    pUniqueData = Q_NULLPTR;
}
