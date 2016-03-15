#ifndef SITESPRICEPARSER_H
#define SITESPRICEPARSER_H

#include <QDialog>
#include <QSettings>
#include <QProxyStyle>

class QVBoxLayout;
class QHBoxLayout;
class QLineEdit;
class QTextEdit;
class QLabel;
class sitePriceProductEditGUI;
class sitePriceProductAddGUI;
class QDomNode;
class QFile;
class QSplitter;
class QComboBox;

class sitesPriceParserGUI : public QDialog
{
    Q_OBJECT

public:
    explicit sitesPriceParserGUI(QWidget *parent = 0);
    ~sitesPriceParserGUI();

public slots:
    void addProduct();
    void editProducts();

private:
    QTextEdit *m_pTeProductList;

    //layouts
    QHBoxLayout *m_pMainLayout;
    QVBoxLayout *m_pProductLayout;

    //buttons
    QPushButton *m_pBtnAddProduct;
    QPushButton *m_pBtnRemoveProduct;
    QPushButton *m_pBtnEditProduct;
    QPushButton *m_pBtnClose;

    sitePriceProductAddGUI *m_pParserOpData;
    sitePriceProductEditGUI *m_pProductEditGUI;
};

class sitePriceProductAddGUI : public QDialog
{
    Q_OBJECT

public:
    explicit sitePriceProductAddGUI(QWidget *parent = 0);
    ~sitePriceProductAddGUI();

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

class sitePriceProductEditGUI : public QDialog
{
    Q_OBJECT

public:
    explicit sitePriceProductEditGUI(QWidget *parent = 0);
    ~sitePriceProductEditGUI();

public slots:
    void loadProductData(QString);

private:
    QLabel *m_pLblProductsCaption;
    QLabel *m_pLblStatusProducts;
    QLabel *m_pLblEditCaption;
    QLabel *m_pLblEditName;
    QLabel *m_pLblEditLinks;

    QLineEdit *m_pLeProductSelector;

    QTextEdit *m_pTeProductsList;
    QTextEdit *m_pTeProductLinks;

    QPushButton *m_pBtnUpdateProduct;
    QPushButton *m_pBtnClose;

    QHBoxLayout *m_pMainLayout;
    QVBoxLayout *m_pProductsListLayout;
    QVBoxLayout *m_pProductEditLayout;

    QComboBox *m_pCbProduct;
    QString fileName;

     QStringList m_productName;
     QPair<QString, QStringList> m_productLinks;

    void readProductsFromXML();
};

class newStyle : public QProxyStyle
{
    Q_OBJECT

public:
    newStyle();

    void polish(QPalette& pal)Q_DECL_OVERRIDE;
};
