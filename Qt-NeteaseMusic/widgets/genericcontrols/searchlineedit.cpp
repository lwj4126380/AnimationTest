#include "searchlineedit.h"
#include <QHBoxLayout>

SearchLineEdit::SearchLineEdit(QWidget *parent)
    : QLineEdit(parent)
{
    setObjectName("searchLineEdit");
    setPlaceholderText(tr("search music, user"));
    searchBtn = new SvgButton(":/uiresource/svg/topbar/search.svg", 13, 13, "#c77373", "white");
    searchBtn->setObjectName("pButtonKeyboard");
    searchBtn->setFixedSize(QSize(15, 15));
    searchBtn->setCursor(QCursor(Qt::PointingHandCursor));

    QHBoxLayout* layout = new QHBoxLayout();
    layout->addStretch();
    layout->addWidget(searchBtn);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 8, 0);

    setLayout(layout);
}
