#ifndef SITESPRICEPARSER_H
#define SITESPRICEPARSER_H

#include <QDialog>
#include <QSettings>

class QVBoxLayout;
class QHBoxLayout;
class QLineEdit;
class QTextEdit;
class QLabel;
class parserOperationData;
class QDomNode;
class QFile;

class sitesPriceParserGUI : public QDialog
{
    Q_OBJECT

public:
    explicit sitesPriceParserGUI(QWidget *parent = 0);
    ~sitesPriceParserGUI();

public slots:
    void addProduct();


private:
    QTextEdit *m_pTeProductList;

    //layouts
    QHBoxLayout *m_pMainLayout;
    QVBoxLayout *m_pProductLayout;

    //buttons
    QPushButton *m_pBtnAddProductToList;
    QPushButton *m_pBtnRemoveProductFromList;
    QPushButton *m_pBtnEditProductInList;

    parserOperationData *m_pParserOpData;
};

class parserOperationData : public QDialog
{
    Q_OBJECT

public:
    explicit parserOperationData(QWidget *parent = 0);
    ~parserOperationData();

public slots:
    void addProduct();
    void readData();

private:
    QVBoxLayout *m_pMainLayout;
    QLabel *m_pLblCaption;

    QLineEdit *m_pLeProductName;
    QTextEdit *m_pTeLinksList;

    QPushButton *m_pBtnAddProduct;
    QPushButton *m_pBtnCancel;

    QString fileName;

    bool productExists(const QString&);
    void createXMLStructureInDocument();
};

#endif // SITESPRICEPARSER_H
