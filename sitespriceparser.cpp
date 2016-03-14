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

sitesPriceParserGUI::sitesPriceParserGUI(QWidget *parent) : QDialog(parent)
{
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint & Qt::WindowMinimized);

    //buttons
    m_pBtnAddProductToList = new QPushButton(tr("Add Product"));
    m_pBtnEditProductInList = new QPushButton(tr("Edit product"));
    m_pBtnRemoveProductFromList = new QPushButton(tr("Remove product"));

    //lineedits
    m_pTeProductList = new QTextEdit();

    //layouts
    m_pProductLayout = new QVBoxLayout();
    m_pProductLayout->addWidget(m_pBtnAddProductToList);
    m_pProductLayout->addWidget(m_pBtnEditProductInList);
    m_pProductLayout->addWidget(m_pBtnRemoveProductFromList);
    m_pProductLayout->addWidget(m_pTeProductList);

    m_pMainLayout = new QHBoxLayout(this);
    m_pMainLayout->addLayout(m_pProductLayout);
    setLayout(m_pMainLayout);

    m_pParserOpData = new parserOperationData(this);

    //connect
    connect(m_pBtnAddProductToList, &QPushButton::clicked, this, &sitesPriceParserGUI::addProduct);
}

sitesPriceParserGUI::~sitesPriceParserGUI()
{

}

void sitesPriceParserGUI::addProduct()
{
    m_pParserOpData->setModal(true);
    m_pParserOpData->show();
}

//class parserOperationData
parserOperationData::parserOperationData(QWidget *parent) : QDialog(parent)
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
    connect(m_pBtnCancel, &QPushButton::clicked, this, &parserOperationData::close);
    connect(m_pBtnAddProduct, &QPushButton::clicked, this, &parserOperationData::addProduct);
}

void parserOperationData::addProduct()
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
                QDomElement new_link = doc.createElement("link");
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

bool parserOperationData::productExists(const QString &_productName)
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

    while(!n.isNull()) {
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

void parserOperationData::createXMLStructureInDocument()
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

void parserOperationData::readData()
{

}

parserOperationData::~parserOperationData()
{

}


