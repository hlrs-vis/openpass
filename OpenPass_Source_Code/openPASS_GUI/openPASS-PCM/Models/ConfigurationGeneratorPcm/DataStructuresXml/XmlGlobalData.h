#ifndef XMLGLOBALDATA_H
#define XMLGLOBALDATA_H

#include "pcm_globalData.h"
#include "pcm_helper.h"
#include "XmlBaseClass.h"

class XmlGlobalData: public XmlBaseClass
{
public:
    XmlGlobalData() = default;
    XmlGlobalData(PCM_GlobalData *globalData);

    bool WriteToXml( QXmlStreamWriter *xmlWriter );

private:
    PCM_GlobalData *globalData = nullptr;              //!< PCM global data
};

#endif // XMLGLOBALDATA_H
