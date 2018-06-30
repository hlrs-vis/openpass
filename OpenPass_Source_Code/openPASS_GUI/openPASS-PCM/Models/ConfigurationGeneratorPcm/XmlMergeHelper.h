#ifndef XMLMERGEHELPER_H
#define XMLMERGEHELPER_H

#include <QDomDocument>
#include <QFile>
#include <QTextStream>
#include <iostream>

class XmlMergeHelper
{
public:
    XmlMergeHelper() = delete;
    XmlMergeHelper(const XmlMergeHelper &) = delete;
    XmlMergeHelper(XmlMergeHelper &&) = delete;
    XmlMergeHelper &operator=(const XmlMergeHelper &) = delete;
    XmlMergeHelper &operator=(XmlMergeHelper &&) = delete;
    virtual ~XmlMergeHelper() = delete;

    static bool loadDomDocument(QString filename, QDomDocument &document);
    static bool setIdOfSystem(QDomElement &root, int id);
};

#endif // XMLMERGEHELPER_H
