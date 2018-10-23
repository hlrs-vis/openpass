#ifndef XMLOBJECT_H
#define XMLOBJECT_H

#include "pcm_object.h"
#include "XmlLine.h"

class XmlObject: public XmlBaseClass
{
public:
    XmlObject() = default;
    XmlObject(PCM_Object *object);

    bool WriteToXml( QXmlStreamWriter *xmlWriter );

private:
    PCM_Object *object = nullptr;              //!< PCM object
};
#endif // XMLOBJECT_H
