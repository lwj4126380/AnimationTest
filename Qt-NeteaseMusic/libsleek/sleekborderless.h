#ifndef SLEEKBORDERLESS_H
#define SLEEKBORDERLESS_H

#include <QMouseEvent>
#include <QGridLayout>
#include <memory>
#include <QLabel>
#include "svgbutton.h"

#include "qwinwidget.h"
class SleekBorderless : public QWinWidget {
  Q_OBJECT
public:
  SleekBorderless(HWND hWnd, QWidget *mainPanel);
  ~SleekBorderless();

  bool nativeEvent(const QByteArray &, void *msg, long *result);
  void mousePressEvent( QMouseEvent *event );
  void setResizeable(bool isResizeable);
  bool getResizeable();

signals:
    void closing();

public slots:
    void pushButtonMinimizeClicked();
    void pushButtonMaximizeClicked();
    void pushButtonCloseClicked();

private:
    HWND _handle;
    bool _isResizeable;

protected:
    QVBoxLayout* _verticalLayout;
    QHBoxLayout* _titleLayout;
    QWidget* _centralWidget;
    QWidget* _titleWidget;
    SvgLabel* _titleLabel;
    QWidget* _mainPanel;
    //QVBoxLayout* _verticalLayout;
    //QHBoxLayout* _titleLayout;
    //QWidget* _centralWidget;
    //QWidget* _titleWidget;
    //QLabel _windowTitle;
};

#endif // SLEEKBORDERLESS_H
