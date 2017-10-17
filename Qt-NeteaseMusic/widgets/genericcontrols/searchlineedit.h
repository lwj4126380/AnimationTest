#ifndef SEARCHLINEEDIT_H
#define SEARCHLINEEDIT_H

#include <QLineEdit>
#include <QPushButton>

class SearchLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit SearchLineEdit(QWidget *parent=Q_NULLPTR);

private:
    QPushButton *searchBtn;
};

#endif // SEARCHLINEEDIT_H
