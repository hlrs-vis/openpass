#ifndef XMLVIEWOBJECT_H
#define XMLVIEWOBJECT_H

#include "pcm_viewObject.h"
#include "XmlLine.h"

class XmlViewObject: public XmlBaseClass
{
public:
    XmlViewObject() = default;
    XmlViewObject(PCM_ViewObject *viewObject);

    bool WriteToXml( QXmlStreamWriter *xmlWriter );

private:
    PCM_ViewObject *viewObject = nullptr;              //!< PCM viewObject
};

#endif // XMLVIEWOBJECT_H
