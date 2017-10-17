#include "searchlineedit.h"
#include <QHBoxLayout>

SearchLineEdit::SearchLineEdit(QWidget *parent)
    : QLineEdit(parent)
{
    setObjectName("searchLineEdit");
    setPlaceholderText(tr("search music, user"));
    searchBtn = new QPushButton();
    searchBtn->setObjectName("pButtonKeyboard");
    searchBtn->setCursor(QCursor(Qt::PointingHandCursor));

    QHBoxLayout* layout = new QHBoxLayout();
    layout->addStretch();
    layout->addWidget(searchBtn);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 8, 0);

    setLayout(layout);
}
