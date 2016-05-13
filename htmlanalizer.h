#ifndef HTMLANALIZER_H
#define HTMLANALIZER_H

class QString;

class HtmlAnalizer
{

public:
    explicit HtmlAnalizer();
    double getItemPrice(const QString&);
    ~HtmlAnalizer();

private:
    double itemPrice;

};

#endif // HTMLANALIZER_H
