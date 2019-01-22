/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#ifndef RESULTDIRITEMMODEL_H
#define RESULTDIRITEMMODEL_H

#include <QStandardItemModel>
#include <QStandardItem>

class ResultDirItemModel : public QStandardItemModel
{

public:
    ResultDirItemModel(QObject* parent = 0);
    ~ResultDirItemModel();

    void setRootResultDir(QString resultDir);

private:
    QStandardItem *rootItem;
    QIcon dirIcon;
    QIcon fileIcon;

    void createDirectoryItem(QString dirPath, QStandardItem *parentItem, int dirDepth);
};

#endif // RESULTDIRITEMMODEL_H
