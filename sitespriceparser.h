#ifndef SITESPRICEPARSER_H
#define SITESPRICEPARSER_H

#include <QDialog>
#include <QObject>
#include <QSettings>
#include <QProxyStyle>

class QVBoxLayout;
class QHBoxLayout;
class QLineEdit;
class QTextEdit;
class QLabel;
class QDomNode;
class QFile;
class QSplitter;
class QComboBox;
class QListWidget;

class sitesPriceParserGUI : public QDialog
{
    Q_OBJECT

public:
    explicit sitesPriceParserGUI(QWidget *parent = 0);
    ~sitesPriceParserGUI();

public slots:
    void addProduct();
    void editProducts();
    void removeProducts();

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
};

//__________product add class

class sitePriceProductAddGUI : public QDialog
{
    Q_OBJECT

public:
    explicit sitePriceProductAddGUI(QWidget *parent = 0);
    ~sitePriceProductAddGUI();

public slots:
    void addProduct();

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

//__________product edit class

class sitePriceProductEditGUI : public QDialog
{
    Q_OBJECT

public:
    explicit sitePriceProductEditGUI(QWidget *parent = 0);
    ~sitePriceProductEditGUI();

public slots:
    void loadProductData(QString);
    void updateProductData();

private:
    QLabel *m_pLblProductsCaption;
    QLabel *m_pLblStatusProducts;
    QLabel *m_pLblEditCaption;
    QLabel *m_pLblEditName;
    QLabel *m_pLblEditLinks;

    QLineEdit *m_pLeProductNewName;

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
    QVector< QPair<QString, QStringList> > m_productLinks;

    void readProductsFromXML();
    void updateXMLDocument();
};

//__________product remove class

class sitePriceProductRemoveGUI : public QDialog
{
    Q_OBJECT

public:
    explicit sitePriceProductRemoveGUI(QWidget *parent = 0);
    ~sitePriceProductRemoveGUI();

public slots:
    void removeProducts();

private:
    QPushButton *m_pRemoveProductBtn;
    QPushButton *m_pCloseBtn;

    QVBoxLayout *m_pMainLayout;
    QListWidget *m_plWProductList;

    QString fileName;
    QStringList m_productNamesToDelete;

    void loadDataFromXML();
};

//__________base class operations with XML document

class baseOperations : public QObject
{
    Q_OBJECT

public:
    explicit baseOperations();
    virtual ~baseOperations();

    //work with XML
//    void createXMLStructureInDocument();
//    bool productExists(const QString&);
//    void updateXMLDocument();

//    //read XML
//    void readAllDataFromXML();
//    void readProductsFromXML();

//    //add, remove item to XML
      void addItemToXML(const QString&, const QStringList&);
//    void removeItemFromXML();

private:
      QString m_strFileName;

};


//__________programm template class

class newStyle : public QProxyStyle
{
    Q_OBJECT

public:
    newStyle();

    void polish(QPalette& pal)Q_DECL_OVERRIDE;
};
