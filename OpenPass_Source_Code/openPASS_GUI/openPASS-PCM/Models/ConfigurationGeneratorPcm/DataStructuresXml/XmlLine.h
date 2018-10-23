#ifndef XMLLINE_H
#define XMLLINE_H

#include "pcm_line.h"
#include "XmlPoint.h"

class XmlLine : public XmlBaseClass
{
public:
    XmlLine() = default;
    XmlLine(const PCM_Line *line);

    bool WriteToXml( QXmlStreamWriter *xmlWriter );

private:
    const PCM_Line *line = nullptr;              //!< PCM line
};

#endif // XMLLINE_H
