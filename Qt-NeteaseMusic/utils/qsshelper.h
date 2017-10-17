#ifndef QSSHEPLER_H
#define QSSHEPLER_H

#include <QWidget>

class QssHelper : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString colorOne READ getColorOne WRITE setColorOne DESIGNABLE true)
    Q_PROPERTY(QString colorTwo READ getColorTwo WRITE setColorTwo DESIGNABLE true)
    Q_PROPERTY(QString colorThree READ getColorThree WRITE setColorThree DESIGNABLE true)
    Q_PROPERTY(QString colorFour READ getColorFour WRITE setColorFour DESIGNABLE true)
    Q_PROPERTY(QString colorFive READ getColorFive WRITE setColorFive DESIGNABLE true)

private:
    QssHelper(QWidget *parent = 0);
    ~QssHelper();
    static QssHelper*   pUniqueData;

public:
    static QssHelper* getInstance();
    static void destroyInstance();


    QString QssHelper::getColorOne() const { return colorOne; }
    void QssHelper::setColorOne(QString c ) { colorOne = c; }

    QString QssHelper::getColorTwo() const { return colorTwo; }
    void QssHelper::setColorTwo(QString c ) { colorTwo = c; }

    QString QssHelper::getColorThree() const { return colorThree; }
    void QssHelper::setColorThree(QString c ) { colorThree = c; }

    QString QssHelper::getColorFour() const { return colorFour; }
    void QssHelper::setColorFour(QString c ) { colorFour = c; }

    QString QssHelper::getColorFive() const { return colorFive; }
    void QssHelper::setColorFive(QString c ) { colorFive = c; }

private:
    QString colorOne;
    QString colorTwo;
    QString colorThree;
    QString colorFour;
    QString colorFive;
};

#endif // QSSHEPLER_H
