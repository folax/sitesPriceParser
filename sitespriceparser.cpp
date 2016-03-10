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
    if (!QFile(QApplication::applicationDirPath() + "/data.xml").exists())
    {
        outputFile.setFileName(QApplication::applicationDirPath() + "/data.xml");
        if  (!outputFile.open(QIODevice::WriteOnly))
        {
            qDebug() << "Coul'd not open file.";
            return;
        }
    }

    QDomDocument xmlDoc("MyML");
    QDomElement root = xmlDoc.createElement("MyML");
    xmlDoc.appendChild(root);

    QDomElement productName = xmlDoc.createElement(m_pLeProductName->text());
    root.appendChild(productName);

    QDomText productLinks = xmlDoc.createTextNode(m_pTeLinksList->toPlainText());
    productName.appendChild(productLinks);

    QString xml = xmlDoc.toString();

    QTextStream textStream(&outputFile);
    textStream << xml;
    outputFile.close();
}

void parserOperationData::readData()
{

}

parserOperationData::~parserOperationData()
{

}

