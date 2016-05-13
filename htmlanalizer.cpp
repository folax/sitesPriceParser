#include "htmlanalizer.h"
#include <QDebug>
#include <QString>

HtmlAnalizer::HtmlAnalizer() : itemPrice(0)
{

}

double HtmlAnalizer::getItemPrice(const QString &htmlData)
{
    //f.ua
    QString key;
    int pos;
    if (htmlData.contains("F.ua"))
    {
        key.clear();
        pos = 0;
        const int key_size = 13, text_indent = 156;
        //get unique key of product
        pos = htmlData.indexOf("{display:block;}") - 1;
        for(int i(pos); i != pos - key_size; --i)
        {
            key.append(htmlData.at(i));
        }
        std::reverse(std::begin(key), std::end(key));
        key.append("\"");

        //get price of product 156
        pos = htmlData.indexOf(key) + key_size + text_indent;
        key.clear();

        while(htmlData.at(pos) != '<')
        {
            key.append(htmlData.at(pos));
            ++pos;
        }
        key = key.replace(" ", "");
        itemPrice = key.toDouble();
        qDebug() << "F.ua price = " << itemPrice;
        return itemPrice;
    }

    //rozetka.ua
    if (htmlData.contains("Rozetka.ua"))
    {
        key.clear();
        pos = 0;
        const int text_indent = 19;
        pos = htmlData.indexOf("price", Qt::CaseSensitive) + text_indent;
        while(htmlData.at(pos) != '\'')
        {
            key.append(htmlData.at(pos));
            ++pos;
        }
        key = key.replace(" ", "");
        itemPrice = key.toDouble();
        qDebug() << "Rozetka.ua price = " << itemPrice;
        return itemPrice;
    }
    else
        return 0;
}

HtmlAnalizer::~HtmlAnalizer()
{
    qDebug() << "HtmlAnalizer destructor";
}

