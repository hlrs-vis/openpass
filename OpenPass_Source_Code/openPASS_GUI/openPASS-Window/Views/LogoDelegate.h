/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#ifndef LOGODELEGATE_H
#define LOGODELEGATE_H

#include <QObject>
#include <QPixmap>

class LogoDelegate : public QObject
{
    Q_OBJECT

public:
    explicit LogoDelegate() = delete;
    virtual ~LogoDelegate() = default;

public:
    static QPixmap logoRequest();

};

#endif // LOGODELEGATE_H
