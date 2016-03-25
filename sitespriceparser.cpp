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

    //lineedits
    m_pTeProductList = new QTextEdit();

    //layouts
    m_pProductLayout = new QVBoxLayout();
    m_pProductLayout->addWidget(m_pBtnAddProduct);
    m_pProductLayout->addWidget(m_pBtnEditProduct);
    m_pProductLayout->addWidget(m_pBtnRemoveProduct);
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
    wnd.Instance()->setModal(true);
    wnd.Instance()->show();
}

void sitesPriceParserGUI::removeProducts()
{
    Singleton< sitePriceProductRemoveGUI > wnd;
    wnd.Instance()->setModal(true);
    wnd.Instance()->show();
}

//class parserOperationData
sitePriceProductAddGUI::sitePriceProductAddGUI(QWidget *parent) : QDialog(parent)
{
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint & Qt::WindowMinimized);
    //labels
    m_pLblCaption = new QLabel(tr("Add Product menu"));
    m_pLblCaption->setAlignment(Qt::AlignCenter);

    //buttons
    m_pBtnAddProduct = new QPushButton(tr("Add product"));
    m_pBtnCancel = new QPushButton(tr("Cancel"));

    //lineedit
    m_pLeProductName = new QLineEdit(tr("Add name here"));

    //textedit
    m_pTeLinksList = new QTextEdit(tr("Add links here, across enter"));

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
    connect(m_pBtnCancel, &QPushButton::clicked, this, &sitePriceProductAddGUI::close);
    connect(m_pBtnAddProduct, &QPushButton::clicked, this, &sitePriceProductAddGUI::addProduct);
}

void sitePriceProductAddGUI::addProduct()
{
    if (m_pLeProductName->text().isEmpty())
    {
        QMessageBox::warning(this, "Empty product name", "Please chose product name!");
        return;
    }
    QFile outputFile;

    //if file don't exists create them
    if (!QFile(fileName).exists())
    {
        outputFile.setFileName(fileName);
        if  (!outputFile.open(QIODevice::WriteOnly))
        {
            qDebug() << "Coul'd not open file.";
            return;
        }
        outputFile.close();
    }
    //open file to work
    outputFile.setFileName(fileName);
    if  (!outputFile.open(QIODevice::ReadWrite))
    {
        qDebug() << "Coul'd not open file!";
        return;
    }
    //проверить есть ли в документе позиция с таким названием
    QDomDocument doc;
    if(!doc.setContent(&outputFile))
    {
        qDebug( "Failed to parse the file into a DOM tree." );
        createXMLStructureInDocument();
        doc.setContent(&outputFile);
    }

    //add product to xml document
    if (!productExists(m_pLeProductName->text()))
    {
        QDomElement root = doc.documentElement();
        QDomElement new_product = doc.createElement("product");
        new_product.setAttribute("name", m_pLeProductName->text());
        root.appendChild(new_product);

        //parse links from QTextEdit
        QStringList links = QString(m_pTeLinksList->toPlainText()).split('\n');
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

    outputFile.close();
}

bool sitePriceProductAddGUI::productExists(const QString &_productName)
{
    qDebug() << "*** productExists() ***";
    QDomDocument doc;
    QFile file(fileName);
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
                    QMessageBox::warning(this, "Product name exists!", "Product name exists, please choose another name of product");
                    return true;
                }
            }
        }
        n = n.nextSibling();
    }
    return false;
}

void sitePriceProductAddGUI::createXMLStructureInDocument()
{
    qDebug() << "*** createXMLStructureInDocument() ***";
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

    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly))
    {
        QTextStream(&file) << doc.toString();
        file.close();
    }
}

void sitePriceProductAddGUI::readData()
{

}

sitePriceProductAddGUI::~sitePriceProductAddGUI()
{

}



//product edit class
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
    readProductsFromXML();
    //connect
    connect(m_pBtnClose, &QPushButton::clicked, this, &sitePriceProductEditGUI::close);
    connect(m_pCbProduct, &QComboBox::currentTextChanged, this, &sitePriceProductEditGUI::loadProductData);
    connect(m_pBtnUpdateProduct, &QPushButton::clicked, this, &sitePriceProductEditGUI::updateProductData);
}

void sitePriceProductEditGUI::readProductsFromXML()
{
    //clear GUI elements
    m_pTeProductsList->clear();
    m_pTeProductLinks->clear();
    m_productLinks.clear();
    m_pCbProduct->clear();
    m_productName.clear();
    m_pLblStatusProducts->clear();

    //read data from file
    QFile outputFile;
    QDomDocument doc;
    outputFile.setFileName(fileName);
    if (!outputFile.open(QIODevice::ReadOnly) || !doc.setContent(&outputFile))
    {
        qDebug() << "Function can't open file, or file don't have XML structure.";
        m_pTeProductsList->append("Can't open file, or file don't have XML structure. Create product first!");
        return;
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
                m_productName.push_back(e.attribute("name"));
                m_pTeProductsList->append(e.attribute("name"));
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
        m_productLinks.push_back( qMakePair(productName, productLinks) );
        productLinks.clear();
    }
    m_pLblStatusProducts->setText(m_pLblStatusProducts->text()
                                  + QString::number(m_productName.size()) + " products");
    m_pCbProduct->addItems(m_productName);
}

sitePriceProductEditGUI::~sitePriceProductEditGUI()
{

}

void sitePriceProductEditGUI::loadProductData(QString _product)
{
    m_pTeProductLinks->clear();
    m_pLeProductNewName->setText(_product);

    //load links from pair array
    for( QPair<QString, QStringList> pair : m_productLinks ){
        for( QString str : pair.second )
        {
            if (pair.first == _product)
                m_pTeProductLinks->append(str);
        }
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
            QStringList newLinks = QString(m_pTeProductLinks->toPlainText()).split('\n');
            newLinks.erase(std::unique(newLinks.begin(), newLinks.end()), newLinks.end());
            QString newName = m_pLeProductNewName->text();
            updateXMLDocument();
        }
    }
    else
        QMessageBox::warning(this, "Product not activated", "Please choose product!");
}

void sitePriceProductEditGUI::updateXMLDocument()
{
    //delete element

    QFile outputFile;
    QDomDocument doc;
    outputFile.setFileName(fileName);
    if (!outputFile.open(QIODevice::ReadOnly) || !doc.setContent(&outputFile))
    {
        qDebug() << "Function can't open file, or file don't have XML structure.";
        m_pTeProductsList->append("Can't open file, or file don't have XML structure. Create product first!");
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
            if (e.tagName() == "product" && e.attribute("name") == m_pLeProductNewName->text())
            {
                docElem.removeChild(n);
            }
        }
        n = n.nextSibling();
    }

    //add element with changes
    QDomElement new_product = doc.createElement("product");
    new_product.setAttribute("name", m_pLeProductNewName->text());
    docElem.appendChild(new_product);

    //parse links from QTextEdit
    QStringList links = QString(m_pTeProductLinks->toPlainText()).split('\n');
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
    readProductsFromXML();
}


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
    loadDataFromXML();

    //connect
    connect(m_pCloseBtn, &QPushButton::clicked, this, &sitePriceProductRemoveGUI::close);
    connect(m_pRemoveProductBtn, &QPushButton::clicked, this, &sitePriceProductRemoveGUI::removeProducts);
    //connect(m_plWProductList, &QListWidget::itemSelectionChanged, this, &sitePriceProductRemoveGUI::getProductNames);
}

sitePriceProductRemoveGUI::~sitePriceProductRemoveGUI()
{

}

void sitePriceProductRemoveGUI::removeProducts()
{
    qDebug() << "sitePriceProductRemoveGUI::removeProducts()";
    m_productNamesToDelete.clear();
    foreach(QListWidgetItem* item, m_plWProductList->selectedItems())
    {
        m_productNamesToDelete.append(item->text());
    }

    QMessageBox::information(NULL,"QListWidget - Selected Items List",
                             "Selected items are:\n"+m_productNamesToDelete.join("\n"));

    loadDataFromXML();
}

void sitePriceProductRemoveGUI::loadDataFromXML()
{
    qDebug() << "sitePriceProductRemoveGUI::loadDataFromXML()";
    m_plWProductList->clear();
    //read data from file
    QFile outputFile;
    QDomDocument doc;
    outputFile.setFileName(fileName);
    if (!outputFile.open(QIODevice::ReadOnly) || !doc.setContent(&outputFile))
    {
        qDebug() << "Function can't open file, or file don't have XML structure.";
        m_plWProductList->addItem("Can't open file, or file don't have XML structure. Create product first!");
        return;
    }
    QDomElement docElem = doc.documentElement();
    QDomNode n = docElem.firstChild();

    QStringList productNames;
    while(!n.isNull())
    {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull())
        {
            if (e.tagName() == "product")
            {
                productNames.push_back(e.attribute("name"));
            }
        }
        n = n.nextSibling();
    }
    m_plWProductList->addItems(productNames);
    productNames.clear();
}
