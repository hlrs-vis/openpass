#ifndef XMLPARAMETER_H
#define XMLPARAMETER_H

#include "XmlBaseClass.h"

class XmlParameter: public XmlBaseClass
{
public:
    XmlParameter(int id, XML_PARAMETER_TYPE type, QString name, QString value);

    bool WriteToXml( QXmlStreamWriter *xmlWriter );

private:
    XML_PARAMETER_TYPE type = XML_PARAMETER_TYPE::undefined;   //!< parameter type
    QString name;      //!< name of the parameter
    QString value;     //!< value of the parameter
};

#endif // XMLPARAMETER_H
