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
class QNetworkAccessManager;
class QNetworkReply;

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
    void parseProducts();

private:
    QTextEdit *m_pTeProductList;

    //layouts
    QHBoxLayout *m_pMainLayout;
    QVBoxLayout *m_pProductLayout;

    //buttons
    QPushButton *m_pBtnAddProduct;
    QPushButton *m_pBtnRemoveProduct;
    QPushButton *m_pBtnEditProduct;
    QPushButton *m_pBtnParseProducts;
    QPushButton *m_pBtnClose;
};

//__________base class operations with XML document

class baseOperations : public QObject
{
    Q_OBJECT

public:
    explicit baseOperations();
    virtual ~baseOperations();

    //work with XML
    void createXMLStructureInDocument();
    bool productExists(const QString&);
    void updateProduct(const QString&, const QString&);

    //    //read XML
    bool readAllDataFromXML();
    const QStringList getProductsNames();
    const QStringList getProductLinks(const QString&);
    int getProductNumbers();

    //    //add, remove item to XML
    bool addItemToXML(const QString&, const QString&);
    bool removeItemsFromXML(const QStringList&);

private:
    QString m_strFileName;
    QVector< QPair<QString, QStringList> > m_dataFromXML;
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
    baseOperations m_pOperations;
};

#endif // SITESPRICEPARSER_H

//__________product edit class

class sitePriceProductEditGUI : public QDialog
{
    Q_OBJECT

public:
    explicit sitePriceProductEditGUI(QWidget *parent = 0);
    void readDataFromXMLToGUI();
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

    baseOperations m_pOperations;
};

//__________product remove class

class sitePriceProductRemoveGUI : public QDialog
{
    Q_OBJECT

public:
    explicit sitePriceProductRemoveGUI(QWidget *parent = 0);
    void readDataFromXMLToGUI();
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

    baseOperations m_pOperations;
};

//__________web pages downloader class

class webpageDownloader : public QObject
{
    Q_OBJECT

public:
    explicit webpageDownloader(QObject *parent = 0);
    ~webpageDownloader();
    void download(const QStringList&);

private slots:
    void slotFinished(QNetworkReply*);

signals:
    void downloadProgress(quint64, quint64);
    void done(const QUrl&, const QByteArray&);
    void error();

private:
    QNetworkAccessManager *m_pNam;
};

class webpageDownloaderGUI : public QDialog
{
    Q_OBJECT

public:
    explicit webpageDownloaderGUI(QWidget *parent = 0);
    void readDataFromXMLToGUI();

private slots:
    void slotCheckAll();

private:
    QListWidget *m_pLwProductsNames;

    QLabel *m_pLblProducts;

    QPushButton *m_pBtnCheckAll;
    QPushButton *m_pBtnParse;
    QPushButton *m_pBtnStopParse;

    QVBoxLayout *m_pProductsTab;
    QVBoxLayout *m_pActionsTab;
    QHBoxLayout *m_pMainLayout;

    baseOperations m_operations;
};

//__________programm template class

class newStyle : public QProxyStyle
{
    Q_OBJECT

public:
    newStyle();

    void polish(QPalette& pal)Q_DECL_OVERRIDE;
};
