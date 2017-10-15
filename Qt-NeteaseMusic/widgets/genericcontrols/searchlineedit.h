#ifndef SEARCHLINEEDIT_H
#define SEARCHLINEEDIT_H

#include <QLineEdit>
#include "svgbutton.h"

class SearchLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit SearchLineEdit(QWidget *parent=Q_NULLPTR);

private:
    SvgButton *searchBtn;
};

#endif // SEARCHLINEEDIT_H
