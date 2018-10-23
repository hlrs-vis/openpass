#ifndef XMLCOMPONENTSIGNAL_H
#define XMLCOMPONENTSIGNAL_H

#include "XmlBaseClass.h"

class XmlComponentSignal: public XmlBaseClass
{
public:
    XmlComponentSignal(int id, bool isInput, QString name, int channelRef);

    bool WriteToXml( QXmlStreamWriter *xmlWriter );

private:
    bool isInput;      //!< boolean defining whether signal is an input or not (=output)
    QString name;      //!< name of the signal
    int channelRef;    //!< channel reference (its id) of the signal
};

#endif // XMLCOMPONENTSIGNAL_H
