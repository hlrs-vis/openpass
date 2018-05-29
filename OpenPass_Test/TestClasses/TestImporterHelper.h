#include <QDomDocument>
#include <QString>

namespace
{
    static QDomElement documentRootFromString(std::string content)
    {
        static QDomDocument document;
        document.setContent(QString::fromStdString(content));
        return document.documentElement();
    }
}
