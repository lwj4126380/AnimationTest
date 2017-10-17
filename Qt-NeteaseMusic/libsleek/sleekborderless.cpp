#include "sleekborderless.h"
#include <QApplication>
#include <QPushButton>
#include <QScrollArea>
#include <QSpacerItem>
#include <windowsx.h>
#include <windows.h>
#include <QWindow>
#include <QDebug>
#include "hoverablewidget.h"
#include "searchlineedit.h"

SleekBorderless::SleekBorderless(HWND hWnd, QWidget *mainPanel) : QWinWidget(hWnd),
    _isResizeable(true)
    //_windowTitle(this)
{
    _handle = hWnd;
    _mainPanel = mainPanel;
    _mainPanel->setObjectName( "mainPanel" );

    _titleLayout = new QHBoxLayout();
    //titleWidget.setStyleSheet("background-color:pink;");
    _titleLayout->setObjectName("titleLayout");
    _titleLayout->setSpacing( 0 );
    _titleLayout->setContentsMargins(0, 0, 10, 0);

    _titleButton = new QPushButton();
    _titleButton->setProperty("ButtonType", "TitleButton");
    _titleButton->setCursor(Qt::PointingHandCursor);
    _titleButton->setObjectName("titleButton");
    _titleButton->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Expanding );
    _titleLayout->addWidget(_titleButton);

    //spaceitem
    QSpacerItem *spaceItem = new QSpacerItem(80, 20, QSizePolicy::Fixed, QSizePolicy::Expanding);
    _titleLayout->addItem(spaceItem);

    //prev next search
    SearchLineEdit *se = new SearchLineEdit();
    _titleLayout->addWidget(se);


    _titleLayout->addStretch();

    //function button
    QHBoxLayout *topRightLayout = new QHBoxLayout();
    _titleLayout->addLayout(topRightLayout);
    topRightLayout->setObjectName("topRightLayout");
    topRightLayout->setSpacing(0);
    topRightLayout->setContentsMargins(0, 0, 0, 5);

    HoverableWidget *hb = new HoverableWidget();
    hb->setTypeOne(QString::fromLocal8Bit("未登录"), "headArrBtn");
    topRightLayout->addWidget(hb);

    //Skin button
    QPushButton *pushButtonSkin = new QPushButton();
    pushButtonSkin->setProperty("ButtonType", "functionButton");
    pushButtonSkin->setCursor(Qt::PointingHandCursor);
    pushButtonSkin->setObjectName( "pushButtonSkin" );
    topRightLayout->addWidget( pushButtonSkin );

    //Msg button
    QPushButton *pushButtonMsg = new QPushButton();
    pushButtonMsg->setProperty("ButtonType", "functionButton");
    pushButtonMsg->setCursor(Qt::PointingHandCursor);
    pushButtonMsg->setObjectName( "pushButtonMsg" );
    topRightLayout->addWidget( pushButtonMsg );

    //Set button
    QPushButton *pushButtonSet = new QPushButton();
    pushButtonSet->setProperty("ButtonType", "functionButton");
    pushButtonSet->setCursor(Qt::PointingHandCursor);
    pushButtonSet->setObjectName( "pushButtonSet" );
    topRightLayout->addWidget( pushButtonSet );

    QLabel *label = new QLabel();
    label->setObjectName("vertical1pixLabel");
    topRightLayout->addWidget(label);

    // System buttons

    QHBoxLayout *systemButtonLayout = new QHBoxLayout();
    topRightLayout->addLayout(systemButtonLayout);
    systemButtonLayout->setObjectName("systemButtonLayout");
    systemButtonLayout->setSpacing(0);
    systemButtonLayout->setContentsMargins(15, 0, 0, 0);

    //Minimode
    QPushButton *pushButtonMiniMode = new QPushButton();
    pushButtonMiniMode->setProperty("ButtonType", "systemButton");
    pushButtonMiniMode->setCursor(Qt::PointingHandCursor);
    pushButtonMiniMode->setObjectName( "pushButtonMiniMode" );
    systemButtonLayout->addWidget( pushButtonMiniMode );

    // Minimize
    QPushButton *pushButtonMinimize = new QPushButton();
    pushButtonMinimize->setProperty("ButtonType", "systemButton");
    pushButtonMinimize->setCursor(Qt::PointingHandCursor);
    pushButtonMinimize->setObjectName( "pushButtonMinimize" );
    systemButtonLayout->addWidget( pushButtonMinimize );
    QObject::connect( pushButtonMinimize, SIGNAL( clicked() ), this, SLOT( pushButtonMinimizeClicked() ) );

    // Maximize
    QPushButton *pushButtonMaximize = new QPushButton();
    pushButtonMaximize->setProperty("ButtonType", "systemButton");
    pushButtonMaximize->setCursor(Qt::PointingHandCursor);
    pushButtonMaximize->setObjectName("pushButtonMaximize");
    systemButtonLayout->addWidget( pushButtonMaximize );
    QObject::connect( pushButtonMaximize, SIGNAL( clicked() ), this, SLOT( pushButtonMaximizeClicked() ) );

    //Close
    QPushButton *pushButtonClose = new QPushButton();
    pushButtonClose->setProperty("ButtonType", "systemButton");
    pushButtonClose->setCursor(Qt::PointingHandCursor);
    pushButtonClose->setObjectName( "pushButtonClose" );
    systemButtonLayout->addWidget( pushButtonClose );
    QObject::connect( pushButtonClose, SIGNAL( clicked() ), this, SLOT( pushButtonCloseClicked() ) );

    // Main panel layout
    QGridLayout *mainGridLayout = new QGridLayout();
    mainGridLayout->setSpacing( 0 );
    mainGridLayout->setMargin( 0 );
    setLayout(mainGridLayout);

    // Central widget
    _centralWidget = new QWidget(this);
    _centralWidget->setObjectName( "centralWidget" );
    _centralWidget->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );

    // Main panel scroll area
    QScrollArea *scrollArea = new QScrollArea( this );
    scrollArea->setWidgetResizable( true );
    scrollArea->setObjectName( "mainPanelScrollArea" );
    scrollArea->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
    scrollArea->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    scrollArea->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    scrollArea->setFrameShape(QFrame::NoFrame);

    // Vertical layout example
    _verticalLayout = new QVBoxLayout();
    _verticalLayout->setSpacing( 0 );
    _verticalLayout->setMargin( 0 );
    //verticalLayout.setAlignment( Qt::AlignTop );
    //verticalLayout.setAlignment(Qt::AlignCenter);
    //verticalLayout.addLayout( horizontalLayout );

    // Show
    _titleWidget = new QWidget(this);
    _titleWidget->setObjectName( "titleWidget" );
    _titleWidget->setLayout( _titleLayout );
    _centralWidget->setLayout( _verticalLayout );
    scrollArea->setWidget( _centralWidget );
    mainGridLayout->addWidget( _titleWidget );
    //mainGridLayout->addWidget( &centralWidget );
    mainGridLayout->addWidget( scrollArea );

    _verticalLayout->addWidget(_mainPanel);
    show();

    //centralWidget.setFocusPolicy(Qt::NoFocus);
    //titleWidget.setFocusPolicy(Qt::NoFocus);
}

SleekBorderless::~SleekBorderless()
{
    _mainPanel->setParent(0); //To avoid Qt from deleting this as well. (This is the SleekWindow the user of libsleek inherits from.)
    //qDebug() << "SleekBorderless: DESTRUCT";
}

// Button events
void SleekBorderless::pushButtonMinimizeClicked()
{
    ShowWindow( getParentWindow(), SW_MINIMIZE );
}

void SleekBorderless::pushButtonMaximizeClicked()
{
    WINDOWPLACEMENT wp;
    wp.length = sizeof( WINDOWPLACEMENT );
    GetWindowPlacement( getParentWindow(), &wp );
    if ( wp.showCmd == SW_MAXIMIZE ) {
        ShowWindow( getParentWindow(), SW_RESTORE );
    } else {
        ShowWindow( getParentWindow(), SW_MAXIMIZE );
        QPushButton* pushButtonMaximize = _titleWidget->findChild<QPushButton*>("pushButtonMaximize");
        if (pushButtonMaximize != _titleWidget->childAt(_titleWidget->mapFromGlobal(QCursor::pos())))
            pushButtonMaximize->setAttribute(Qt::WA_UnderMouse, false); //To prevent maximize to be highlighted after maximizing the window.
    }
}

void SleekBorderless::pushButtonCloseClicked()
{
    emit closing();
}

bool SleekBorderless::nativeEvent( const QByteArray &, void *msg, long *result)
{
    MSG *message = ( MSG * )msg;
    switch( message->message ) {
    case WM_SYSKEYDOWN: {
        if ( message->wParam == VK_SPACE )
        {
            RECT winrect;
            GetWindowRect( _handle, &winrect );
            TrackPopupMenu( GetSystemMenu( _handle, false ), TPM_TOPALIGN | TPM_LEFTALIGN, winrect.left + 5, winrect.top + 5, 0, _handle, NULL);
            break;
        }
    }
    case WM_KEYDOWN:
    {
        if ( message->wParam == VK_F5 ||
             message->wParam == VK_F6 ||
             message->wParam == VK_F7
             )
        {
            SendMessage( _handle, WM_KEYDOWN, message->wParam, message->lParam );
            break;
        }
    }
    case WM_NCHITTEST:
    {
        *result = 0;

        WINDOWPLACEMENT wp;
        wp.length = sizeof( WINDOWPLACEMENT );
        GetWindowPlacement( getParentWindow(), &wp );
        if ( wp.showCmd == SW_MAXIMIZE )
            return false;

        const LONG border_width = 5; //in pixels
        RECT winrect;
        GetWindowRect((HWND)window()->winId(), &winrect);

        long x = GET_X_LPARAM(message->lParam);
        long y = GET_Y_LPARAM(message->lParam);

        if (getResizeable())
        {
            //left border
            if (x >= winrect.left && x < winrect.left + border_width)
            {
                *result  = HTTRANSPARENT;
                return true;
            }
            //right border
            if (x < winrect.right && x >= winrect.right - border_width)
            {
                *result  = HTTRANSPARENT;
                return true;
            }
            //bottom border
            if (y < winrect.bottom && y >= winrect.bottom - border_width)
            {
                *result  = HTTRANSPARENT;
                return true;
            }
            //top border
            if (y >= winrect.top && y < winrect.top + border_width)
            {
                *result  = HTTRANSPARENT;
                return true;
            }
        }
        break;
    } //end case WM_NCHITTEST
    }

    return false;
}

void SleekBorderless::mousePressEvent(QMouseEvent *event)
{
    if ( event->button() == Qt::LeftButton )
    {
        ReleaseCapture();
        SendMessage( _handle, WM_NCLBUTTONDOWN, HTCAPTION, 0 );
    }

    if (_isResizeable)
    {
        if ( event->type() == QEvent::MouseButtonDblClick )
        {
            if (event -> button() == Qt::LeftButton)
            {
                WINDOWPLACEMENT wp;
                wp.length = sizeof( WINDOWPLACEMENT );
                GetWindowPlacement( getParentWindow(), &wp );
                if ( wp.showCmd == SW_MAXIMIZE )
                {
                    ShowWindow( getParentWindow(), SW_RESTORE );
                }
                else
                {
                    ShowWindow( getParentWindow(), SW_MAXIMIZE );
                }
            }
        }
    }
}

void SleekBorderless::setResizeable(bool isResizeable)
{
    _isResizeable = isResizeable;
}

bool SleekBorderless::getResizeable()
{
    return _isResizeable;
}
