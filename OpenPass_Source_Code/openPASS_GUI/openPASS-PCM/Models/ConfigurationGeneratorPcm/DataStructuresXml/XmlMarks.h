#ifndef XMLMARKS_H
#define XMLMARKS_H

#include "pcm_marks.h"
#include "XmlLine.h"

class XmlMarks: public XmlBaseClass
{
public:
    XmlMarks(PCM_Marks *marks);

    bool WriteToXml( QXmlStreamWriter *xmlWriter );

private:
    PCM_Marks *marks;              //!< PCM marks
};

#endif // XMLMARKS_H
