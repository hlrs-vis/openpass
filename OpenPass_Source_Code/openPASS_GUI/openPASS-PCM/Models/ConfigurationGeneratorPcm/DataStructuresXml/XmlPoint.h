#ifndef XMLPOINT_H
#define XMLPOINT_H

#include "pcm_point.h"
#include "XmlBaseClass.h"

class XmlPoint : public XmlBaseClass
{
public:
    XmlPoint() = default;
    XmlPoint(const PCM_Point *point);

    bool WriteToXml( QXmlStreamWriter *xmlWriter );

private:
    const PCM_Point *point = nullptr;              //!< PCM point
};

#endif // XMLPOINT_H
