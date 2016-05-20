#include "sitespriceparser.h"
#include <QDebug>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>
#include <QFile>
#include <QApplication>
#include <QDomDocument>
#include <algorithm>
#include <QSplitter>
#include <QComboBox>
#include <QStyleFactory>
#include <QVector>
#include <QListWidget>
#include <QProgressBar>
#include <QList>
#include <QPair>

#include <QWebElement>
#include <QWebFrame>
#include <QWebPage>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QUrl>
#include <QEventLoop>
#include <QFile>
#include <QWebView>

#include "singleton.h"

sitesPriceParserGUI::sitesPriceParserGUI(QWidget *parent) : QDialog(parent)
{
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint & Qt::WindowMinimized);
    QApplication::setStyle(new newStyle);

    //buttons
    m_pBtnAddProduct = new QPushButton(tr("Add Product"));
    m_pBtnEditProduct = new QPushButton(tr("Edit products"));
    m_pBtnRemoveProduct = new QPushButton(tr("Remove product"));
    m_pBtnClose = new QPushButton(tr("Quit"));
    m_pBtnParseProducts = new QPushButton(tr("Parse products"));

    //lineedits
    m_pTeProductList = new QTextEdit();

    //layouts
    m_pProductLayout = new QVBoxLayout();
    m_pProductLayout->addWidget(m_pBtnAddProduct);
    m_pProductLayout->addWidget(m_pBtnEditProduct);
    m_pProductLayout->addWidget(m_pBtnRemoveProduct);
    m_pProductLayout->addWidget(m_pBtnParseProducts);
    m_pProductLayout->addWidget(m_pBtnClose);
    m_pProductLayout->addWidget(m_pTeProductList);

    m_pMainLayout = new QHBoxLayout(this);
    m_pMainLayout->addLayout(m_pProductLayout);
    setLayout(m_pMainLayout);

    //connect
    connect(m_pBtnClose, &QPushButton::clicked, this, &sitesPriceParserGUI::close);
    connect(m_pBtnAddProduct, &QPushButton::clicked, this, &sitesPriceParserGUI::addProduct);
    connect(m_pBtnEditProduct, &QPushButton::clicked, this, &sitesPriceParserGUI::editProducts);
    connect(m_pBtnRemoveProduct, &QPushButton::clicked, this, &sitesPriceParserGUI::removeProducts);
    connect(m_pBtnParseProducts, &QPushButton::clicked, this, &sitesPriceParserGUI::parseProducts);
}

sitesPriceParserGUI::~sitesPriceParserGUI()
{

}

void sitesPriceParserGUI::addProduct()
{
    Singleton< sitePriceProductAddGUI > wnd;
    wnd.Instance()->setModal(true);
    wnd.Instance()->show();
}

void sitesPriceParserGUI::editProducts()
{
    Singleton< sitePriceProductEditGUI > wnd;
    wnd.Instance()->readDataFromXMLToGUI();
    wnd.Instance()->setModal(true);
    wnd.Instance()->show();
}

void sitesPriceParserGUI::removeProducts()
{
    Singleton< sitePriceProductRemoveGUI > wnd;
    wnd.Instance()->readDataFromXMLToGUI();
    wnd.Instance()->setModal(true);
    wnd.Instance()->show();
}

void sitesPriceParserGUI::parseProducts()
{
    Singleton< webpageDownloaderGUI > wnd;
    wnd.Instance()->readDataFromXMLToGUI();
    wnd.Instance()->setModal(true);
    wnd.Instance()->show();
}

//class parserOperationData
sitePriceProductAddGUI::sitePriceProductAddGUI(QWidget *parent) : QDialog(parent)
{
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint & Qt::WindowMinimized);
    //labels
    m_pLblCaption = new QLabel(QObject::tr("Add Product menu"));
    m_pLblCaption->setAlignment(Qt::AlignCenter);

    //buttons
    m_pBtnAddProduct = new QPushButton(QObject::tr("Add product"));
    m_pBtnCancel = new QPushButton(QObject::tr("Cancel"));

    //lineedit
    m_pLeProductName = new QLineEdit(QObject::tr("Add name here"));

    //textedit
    m_pTeLinksList = new QTextEdit(QObject::tr("Add links here, across enter"));

    //qvboxlayout
    m_pMainLayout = new QVBoxLayout(this);
    m_pMainLayout->addWidget(m_pLblCaption);
    m_pMainLayout->addWidget(m_pBtnAddProduct);
    m_pMainLayout->addWidget(m_pBtnCancel);
    m_pMainLayout->addWidget(m_pLeProductName);
    m_pMainLayout->addWidget(m_pTeLinksList);
    setLayout(m_pMainLayout);

    //variables
    fileName = QApplication::applicationDirPath() + "/data.xml";

    //connect
    QObject::connect(m_pBtnCancel, &QPushButton::clicked, this, &sitePriceProductAddGUI::close);
    QObject::connect(m_pBtnAddProduct, &QPushButton::clicked, this, &sitePriceProductAddGUI::addProduct);
}

void sitePriceProductAddGUI::addProduct()
{
    if (m_pLeProductName->text().isEmpty())
    {
        QMessageBox::warning(this, "Empty product name", "Please chose product name!");
        return;
    }
    if (!m_pOperations.addItemToXML(m_pLeProductName->text(), m_pTeLinksList->toPlainText()))
        QMessageBox::information(this, tr("Error!"), tr("Can't create product ! Error...."));
    else
    {
        QMessageBox::information(this, tr("Sucess!"), tr("Product added!"));
        m_pLeProductName->setText(tr("Add name here"));
        m_pTeLinksList->setText(tr("Add links here, across enter"));
    }
}

sitePriceProductAddGUI::~sitePriceProductAddGUI()
{

}

//__________product edit class

sitePriceProductEditGUI::sitePriceProductEditGUI(QWidget *parent) : QDialog(parent)
{
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint & Qt::WindowMinimized);
    setWindowTitle("Products Editor");
    //variables
    fileName = QApplication::applicationDirPath() + "/data.xml";

    //buttons
    m_pBtnUpdateProduct = new QPushButton(tr("Update product"));
    m_pBtnClose = new QPushButton(tr("Quit"));

    //lineedit
    m_pLeProductNewName = new QLineEdit;

    //labels
    m_pLblProductsCaption = new QLabel(tr("All products list"));
    m_pLblProductsCaption->setAlignment(Qt::AlignCenter);
    m_pLblStatusProducts = new QLabel(tr("Total: "));
    m_pLblEditCaption = new QLabel(tr("Select product:"));
    //m_pLblEditCaption->setAlignment(Qt::AlignCenter);
    m_pLblEditName = new QLabel(tr("Edit name:"));
    m_pLblEditName->setBuddy(m_pLeProductNewName);
    m_pLblEditLinks = new QLabel(tr("Edit links:"));

    //textedit
    m_pTeProductsList = new QTextEdit();
    m_pTeProductsList->setReadOnly(true);
    m_pTeProductsList->setFixedWidth(200);
    m_pTeProductLinks = new QTextEdit();

    //combobox
    m_pCbProduct = new QComboBox();

    //layouts
    m_pProductsListLayout = new QVBoxLayout();
    m_pProductsListLayout->addWidget(m_pLblProductsCaption);
    m_pProductsListLayout->addWidget(m_pTeProductsList);
    m_pProductsListLayout->addWidget(m_pLblStatusProducts);

    m_pProductEditLayout = new QVBoxLayout();
    m_pProductEditLayout->addWidget(m_pLblEditCaption);
    m_pProductEditLayout->addWidget(m_pCbProduct);
    m_pProductEditLayout->addWidget(m_pLblEditName);
    m_pProductEditLayout->addWidget(m_pLeProductNewName);
    m_pProductEditLayout->addWidget(m_pLblEditLinks);
    m_pProductEditLayout->addWidget(m_pTeProductLinks);
    m_pProductEditLayout->addWidget(m_pBtnUpdateProduct);
    m_pProductEditLayout->addWidget(m_pBtnClose);

    m_pMainLayout = new QHBoxLayout(this);
    m_pMainLayout->addLayout(m_pProductsListLayout);
    m_pMainLayout->addLayout(m_pProductEditLayout);

    setLayout(m_pMainLayout);
    //connect
    connect(m_pBtnClose, &QPushButton::clicked, this, &sitePriceProductEditGUI::close);
    connect(m_pCbProduct, &QComboBox::currentTextChanged, this, &sitePriceProductEditGUI::loadProductData);
    connect(m_pBtnUpdateProduct, &QPushButton::clicked, this, &sitePriceProductEditGUI::updateProductData);
}

void sitePriceProductEditGUI::readDataFromXMLToGUI()
{
    //clear GUI elements
    m_pTeProductsList->clear();
    m_pTeProductLinks->clear();
    m_productLinks.clear();
    m_pCbProduct->clear();
    m_productName.clear();
    m_pLblStatusProducts->clear();

    m_pOperations.readAllDataFromXML();
    for (const QString &str : m_pOperations.getProductsNames())
    {
        m_pTeProductsList->append(str);
        m_pCbProduct->addItem(str);
    }

    m_pLblStatusProducts->setText("Total: " + QString::number(m_pOperations.getProductNumbers()) + " products");
}

void sitePriceProductEditGUI::loadProductData(QString _product)
{
    m_pTeProductLinks->clear();
    m_pLeProductNewName->setText(_product);

    for (const QString &str : m_pOperations.getProductLinks(_product))
    {
        m_pTeProductLinks->append(str);
    }
}

void sitePriceProductEditGUI::updateProductData()
{
    QMessageBox::StandardButton reply;
    if (QString(m_pLeProductNewName->text()).size() > 1)
    {
        reply = QMessageBox::question(this, "Confirm edit product", "Confirm edit?",
                                      QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            m_pOperations.updateProduct(m_pLeProductNewName->text(), m_pTeProductLinks->toPlainText());
        }
    }
    else
        QMessageBox::warning(this, "Product not activated", "Please choose product!");
    m_pLblEditName->setText(tr(""));
    m_pTeProductLinks->setText(tr(""));
    readDataFromXMLToGUI();
}

sitePriceProductEditGUI::~sitePriceProductEditGUI()
{

}

//__________programm template class

newStyle::newStyle() : QProxyStyle(QStyleFactory::create("Fusion"))
{

}

void newStyle::polish(QPalette &darkPalette)
{
    darkPalette.setColor(QPalette::Window, QColor(53,53,53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25,25,25)); //listWidget color
    darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, QColor(216, 224, 240));
    darkPalette.setColor(QPalette::Button, QColor(4, 90, 135));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);
}

//__________product remove class

sitePriceProductRemoveGUI::sitePriceProductRemoveGUI(QWidget *parent) : QDialog(parent)
{
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint & Qt::WindowMinimized);
    setWindowTitle("Products remover");

    //buttons
    m_pRemoveProductBtn = new QPushButton(tr("Remove element"));
    m_pCloseBtn = new QPushButton(tr("Exit"));

    //listwidget
    m_plWProductList = new QListWidget();
    m_plWProductList->setSelectionMode(QAbstractItemView::ExtendedSelection);

    //layouts
    m_pMainLayout = new QVBoxLayout(this);
    m_pMainLayout->addWidget(m_plWProductList);
    m_pMainLayout->addWidget(m_pRemoveProductBtn);
    m_pMainLayout->addWidget(m_pCloseBtn);

    this->setLayout(m_pMainLayout);

    fileName = QApplication::applicationDirPath() + "/data.xml";

    //connect
    connect(m_pCloseBtn, &QPushButton::clicked, this, &sitePriceProductRemoveGUI::close);
    connect(m_pRemoveProductBtn, &QPushButton::clicked, this, &sitePriceProductRemoveGUI::removeProducts);
    //connect(m_plWProductList, &QListWidget::itemSelectionChanged, this, &sitePriceProductRemoveGUI::getProductNames);
}

void sitePriceProductRemoveGUI::removeProducts()
{
    //qDebug() << "void sitePriceProductRemoveGUI::removeProducts()";
    m_productNamesToDelete.clear();
    foreach(QListWidgetItem* item, m_plWProductList->selectedItems())
    {
        m_productNamesToDelete.append(item->text());
    }

    if (m_productNamesToDelete.size() > 0)
    {
        if (m_pOperations.removeItemsFromXML(m_productNamesToDelete))
            QMessageBox::information(this, tr("Sucess"), tr("Opearion sucessful"));
    }
    else
        QMessageBox::information(this, tr("Not selected!"), tr("Please chose element!"));
    readDataFromXMLToGUI();
}

void sitePriceProductRemoveGUI::readDataFromXMLToGUI()
{
    //qDebug() << "void sitePriceProductRemoveGUI::loadDataFromXML()";
    m_plWProductList->clear();
    for (const QString &str : m_pOperations.getProductsNames())
    {
        m_plWProductList->addItem(str);
    }
}

sitePriceProductRemoveGUI::~sitePriceProductRemoveGUI()
{

}

//__________base class operations with XML document

baseOperations::baseOperations()
{
    m_strFileName = QApplication::applicationDirPath() + "/products.xml";
    m_strSignName = QApplication::applicationDirPath() + "/signatures.xml";
}

void baseOperations::createXMLStructureInDocument()
{
    //qDebug() << "*** createXMLStructureInDocument() ***";
    QDomDocument doc("products");
    QDomElement domElement = doc.createElement("products_to_parse_list");
    doc.appendChild(domElement);

    //QDomElement product = doc.createElement("product");
    //product.setAttribute("name", "empty");
    //product.setAttribute("link", "empty");

    //QDomElement link = doc.createElement("link");
    //link.setAttribute("link", "empty");
    //product.appendChild(link);

    //domElement.appendChild(product);

    QFile file(m_strFileName);
    if(file.open(QIODevice::WriteOnly))
    {
        QTextStream(&file) << doc.toString();
        file.close();
    }
}



bool baseOperations::productExists(const QString &_productName)
{
    //qDebug() << "*** productExists() ***";
    QDomDocument doc;
    QFile file(m_strFileName);
    if (!file.open(QIODevice::ReadOnly) || !doc.setContent(&file))
    {
        qDebug() << "productExists() = File not open or not have xml structure.";
        createXMLStructureInDocument();
    }
    QDomElement docElem = doc.documentElement();
    QDomNode n = docElem.firstChild();

    while(!n.isNull())
    {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull())
        {
            if (e.tagName() == "product")
            {
                if (e.attribute("name") == _productName)
                {
                    QMessageBox::warning(NULL, "Product name exists!", "Product name exists, please choose another name of product");
                    return true;
                }
            }
        }
        n = n.nextSibling();
    }
    return false;
}

void baseOperations::updateProduct(const QString &_productName, const QString &_productLinks)
{
    QFile outputFile;
    QDomDocument doc;
    outputFile.setFileName(m_strFileName);
    if (!outputFile.open(QIODevice::ReadOnly) || !doc.setContent(&outputFile))
    {
        qDebug() << "Function can't open file, or file don't have XML structure.";
        return;
    }
    //delete old element
    QDomElement docElem = doc.documentElement();
    QDomNode n = docElem.firstChild(); //product
    while(!n.isNull())
    {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull())
        {
            if (e.tagName() == "product" && e.attribute("name") == _productName)
            {
                docElem.removeChild(n);
            }
        }
        n = n.nextSibling();
    }

    //add element with changes
    QDomElement new_product = doc.createElement("product");
    new_product.setAttribute("name", _productName);
    docElem.appendChild(new_product);

    //parse links from QTextEdit
    QStringList links = _productLinks.split('\n');
    links.erase(std::unique(links.begin(), links.end()), links.end());

    for (int i(0); i < links.size(); ++i)
    {
        QString tmp = links.at(i);
        tmp.remove(" ");
        if (tmp.size() > 2)
        {
            QDomElement new_link = doc.createElement("links");
            new_link.setAttribute("link", links.at(i));
            new_product.appendChild(new_link);
        }
    }

    outputFile.close();
    outputFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream(&outputFile) << doc.toString();
    outputFile.close();
    readAllDataFromXML();
}

bool baseOperations::readAllDataFromXML()
{
    //check file
    QFile outFile;
    if (!QFile(m_strFileName).exists())
    {
        outFile.setFileName(m_strFileName);
        if (!outFile.open(QIODevice::WriteOnly))
        {
            qDebug() << "bool baseOperations::readAllDataFromXML() : " << "Coul'd not create file";
            return false;
        }
        createXMLStructureInDocument();
    }
    outFile.close();


    //clear data file
    m_dataFromXML.clear();
    //read data from file
    QDomDocument doc;
    outFile.setFileName(m_strFileName);
    if (!outFile.open(QIODevice::ReadOnly) || !doc.setContent(&outFile))
    {
        qDebug() << "Function can't open file, or file don't have XML structure.";
        return false;
    }
    QDomElement docElem = doc.documentElement();
    QDomNode n = docElem.firstChild();

    QString productName;
    QStringList productLinks;
    while(!n.isNull())
    {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull())
        {
            if (e.tagName() == "product")
            {
                productName = e.attribute("name");

                //get product links
                QDomNode node = n.firstChild();
                while(!node.isNull())
                {
                    QDomElement element = node.toElement();
                    if(!element.isNull())
                    {
                        productLinks.push_back(element.attribute("link"));
                    }
                    node = node.nextSibling();
                }
            }
        }
        n = n.nextSibling();
        m_dataFromXML.push_back( qMakePair(productName, productLinks) );
        productLinks.clear();
    }
    return true;
}

const QStringList baseOperations::getProductsNames()
{
    //qDebug() << "const QStringList baseOperations::getProductsNames()";
    QStringList _productNames;
    if (readAllDataFromXML())
    {
        for( QPair<QString, QStringList> pair : m_dataFromXML )
        {
            _productNames.push_back(pair.first);
        }
    }
    return _productNames;
}

const QStringList baseOperations::getProductLinks(const QString &_productName)
{
    //qDebug() << "const QStringList baseOperations::getProductsNames()";
    QStringList _productLinks;
    if (readAllDataFromXML())
    {
        for( QPair<QString, QStringList> pair : m_dataFromXML )
            if (pair.first == _productName)
            {
                for( QString str : pair.second ){
                    _productLinks.push_back(str);
                }
            }
    }
    return _productLinks;
}

int baseOperations::getProductNumbers()
{
    //qDebug() << "const QStringList baseOperations::getProductLinks(const QString &_productName)";
    if (readAllDataFromXML())
    {
        return m_dataFromXML.size();
    }
    return 0;
}

bool baseOperations::addItemToXML(const QString &_productName, const QString &_productLinks)
{
    if (_productName.isEmpty())
    {
        QMessageBox::warning(NULL, "Empty product name", "Please chose product name!");
        return false;
    }
    QFile outputFile;

    //if file don't exists create them
    if (!QFile(m_strFileName).exists())
    {
        outputFile.setFileName(m_strFileName);
        if (!outputFile.open(QIODevice::WriteOnly))
        {
            qDebug() << "Coul'd not open file.";
            return false;
        }
        outputFile.close();
    }
    //open file to work
    outputFile.setFileName(m_strFileName);
    if (!outputFile.open(QIODevice::ReadWrite))
    {
        qDebug() << "Coul'd not open file!";
        return false;
    }
    QDomDocument doc;
    if(!doc.setContent(&outputFile))
    {
        qDebug( "Failed to parse the file into a DOM tree." );
        createXMLStructureInDocument();
        doc.setContent(&outputFile);
    }

    //add product to xml document
    if (!productExists(_productName))
    {
        QDomElement root = doc.documentElement();
        QDomElement new_product = doc.createElement("product");
        new_product.setAttribute("name", _productName);
        root.appendChild(new_product);

        //links separation from string
        QStringList links = _productLinks.split('\n');
        links.erase(std::unique(links.begin(), links.end()), links.end());

        for (int i(0); i < links.size(); ++i)
        {
            QString tmp = links.at(i);
            tmp.remove(" ");
            if (tmp.size() > 2)
            {
                QDomElement new_link = doc.createElement("links");
                new_link.setAttribute("link", links.at(i));
                new_product.appendChild(new_link);
            }
        }

        outputFile.close();
        outputFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QTextStream(&outputFile) << doc.toString();
    }
    else
        return false;
    outputFile.close();
    return true;
}

bool baseOperations::removeItemsFromXML(const QStringList &_productNames)
{
    QFile outputFile;
    QDomDocument doc;
    outputFile.setFileName(m_strFileName);
    if (!outputFile.open(QIODevice::ReadOnly) || !doc.setContent(&outputFile))
    {
        qDebug() << "Function can't open file, or file don't have XML structure.";
        return false;
    }
    for (int i(0); i < _productNames.size(); ++i)
    {
        //delete old element
        QDomElement docElem = doc.documentElement();
        QDomNode n = docElem.firstChild(); //product
        while(!n.isNull())
        {
            QDomElement e = n.toElement(); // try to convert the node to an element.
            if(!e.isNull())
            {
                if (e.tagName() == "product" && e.attribute("name") == _productNames.at(i))
                {
                    docElem.removeChild(n);
                }
            }
            n = n.nextSibling();
        }
    }
    outputFile.close();
    outputFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream(&outputFile) << doc.toString();
    outputFile.close();
    readAllDataFromXML();
    return true;
}

baseOperations::~baseOperations()
{

}

//__________web pages downloader class

webpageDownloader::webpageDownloader(QObject *parent) : QObject(parent)
{

}

void webpageDownloader::download(const QString &name, const QStringList &links)
{
    htmlAnalize = new HtmlAnalizer;
    QVector<webData> tmpVec;
    for(int i(0); i < links.size(); ++i)
    {
        tmpVec.clear();
        QEventLoop eventLoop;
        QNetworkAccessManager *nam = new QNetworkAccessManager(this);

        connect(nam, &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit);

        QNetworkRequest request;
        request.setUrl(QUrl(links.at(i)));

        QNetworkReply *reply = nam->get(request);
        eventLoop.exec();

        //read data from reply
        QString bytes;
        if(reply->error() == QNetworkReply::NoError)
        {
            bytes = reply->readAll();
        }
        webData wD;
        wD.name = name;
        wD.link = links.at(i);
        wD.price = htmlAnalize->getItemPrice(bytes);
        tmpVec.push_back(wD);
    }
    m_data.append(tmpVec);
    delete htmlAnalize;
}

const QVector<QVector<webData> > webpageDownloader::getData()
{
    qDebug() << "m_data size: " << m_data.size();
    return this->m_data;
}


webpageDownloader::~webpageDownloader()
{

}

void webpageDownloader::clearBuffer()
{
    if (m_data.size() > 0)
    {
        m_data.clear();
    }
    else
        QMessageBox::warning(0, tr("Buffer is empty"), tr("Buffer already empty!"));
}

//__________ webpage DownloaderGUI;

webpageDownloaderGUI::webpageDownloaderGUI(QWidget *parent) : QDialog(parent)
{
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint & Qt::WindowMinimized);
    setWindowTitle("Webpage downloader");
    //showMaximized();

    m_pWpDownloader = new webpageDownloader(this);

    //progress bar

    //list widget
    m_pLwProductsNames = new QListWidget();
    m_pLwProductsNames->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_pLwResultList = new QListWidget();


    //labels
    m_pLblProducts = new QLabel(tr("Products:"));

    //buttons
    m_pBtnCheckAll = new QPushButton(tr("Check All"));
    m_pBtnParse = new QPushButton(tr("Parse"));
    m_pBtnStopParse = new QPushButton(tr("Stop"));
    m_pBtnClearBuffer = new QPushButton(tr("Clear Buffer"));

    //layouts
    m_pActionsTab = new QVBoxLayout();
    m_pActionsTab->setAlignment(Qt::AlignTop);
    m_pActionsTab->addWidget(m_pBtnParse);
    m_pActionsTab->addWidget(m_pBtnStopParse);
    m_pActionsTab->addWidget(m_pBtnClearBuffer);

    m_pProductsTab = new QVBoxLayout();
    m_pProductsTab->addWidget(m_pLblProducts);
    m_pProductsTab->addWidget(m_pLwProductsNames);
    m_pProductsTab->addWidget(m_pBtnCheckAll);

    m_pMainLayout = new QHBoxLayout(this);
    m_pMainLayout->addLayout(m_pProductsTab);
    m_pMainLayout->addLayout(m_pActionsTab);
    m_pMainLayout->addWidget(m_pLwResultList);
    setLayout(m_pMainLayout);

    //connect
    connect(m_pBtnCheckAll, &QPushButton::clicked, this, &webpageDownloaderGUI::slotCheckAll);
    connect(m_pBtnParse, &QPushButton::clicked, this, &webpageDownloaderGUI::slotParseProducts);
    connect(m_pBtnClearBuffer, &QPushButton::clicked, m_pWpDownloader, &webpageDownloader::clearBuffer);
}

void webpageDownloaderGUI::readDataFromXMLToGUI()
{
    m_pLwProductsNames->clear();
    for (const QString &str : m_operations.getProductsNames())
    {
        m_pLwProductsNames->addItem(str);
    }
}

void webpageDownloaderGUI::slotCheckAll()
{
    m_pLwProductsNames->selectAll();
}

void webpageDownloaderGUI::slotParseProducts()
{
    m_pBtnParse->setEnabled(false);
    m_sLProductName.clear();
    m_pLblProducts->clear();
    m_pLwResultList->clear();

    foreach(QListWidgetItem *item, m_pLwProductsNames->selectedItems())
    {
        m_sLProductName.append(item->text());
    }

    if (m_sLProductName.size() > 0)
    {
        for (int i(0); i < m_sLProductName.size(); ++i)
        {
            m_pWpDownloader->download(m_sLProductName.at(i), m_operations.getProductLinks(m_sLProductName.at(i)));
        }
    }
    QVector<QVector <webData> > dataToView = m_pWpDownloader->getData();

    //name of the aim
    for(int i(0); i < dataToView.size(); ++i)
    {
        for (int j(0); j < dataToView.at(j).size(); ++j)
        {
            m_pLwResultList->addItem(dataToView.at(i).at(j).name);
        }
    }

    m_pBtnParse->setEnabled(true);
}

