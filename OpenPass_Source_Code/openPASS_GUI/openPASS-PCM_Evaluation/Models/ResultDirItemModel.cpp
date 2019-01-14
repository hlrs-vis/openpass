/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "ResultDirItemModel.h"

#include <QApplication>
#include <QStyle>
#include <QDir>
#include <QDebug>
#include <QDirIterator>
#include "GUI_Definitions.h"

ResultDirItemModel::ResultDirItemModel(QObject *parent) : QStandardItemModel(parent)
{
    rootItem = this->invisibleRootItem();
    dirIcon = QApplication::style()->standardIcon(QStyle::SP_DirIcon);      //icon for directories
    fileIcon = QApplication::style()->standardIcon(QStyle::SP_FileIcon);    //icon for files
}

ResultDirItemModel::~ResultDirItemModel()
{
}

void ResultDirItemModel::setRootResultDir(QString resultDir)
{
    if(!resultDir.isEmpty())
    {
        // clear all existing data (except for the invisible root) in the tree model
        rootItem->removeRows(0, rootItem->rowCount());

        QString dirName = QFileInfo(resultDir).baseName();
        QStandardItem* parent = new QStandardItem(dirIcon, dirName);  //create the parent directory item
        parent->setAccessibleDescription(dirName);     //set actual path to item
        rootItem->appendRow(parent);                //add the parent item to root item
        createDirectoryItem(resultDir, parent, 0);          //Iterate and populate the contents
    }
}

/*
  Method to populate the contents of the given directory (up to 2 levels) in recursive manner.
  Each itme in level 1 is a case number like "1000648", while each item in level 2 is the config name like "0-0-0"
*/
void ResultDirItemModel::createDirectoryItem(QString dirPath, QStandardItem *parentItem, int dirDepth)
{
    QString subDirName;
    QStandardItem* child;

    QDirIterator it(dirPath, QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);
    while (it.hasNext())
    {
        subDirName = QFileInfo(it.next()).baseName(); // get the base name of the subfolder without path prefix

        if(((dirDepth == 0) && (QRegExp(REGEX_CASE_NUMBER).exactMatch(subDirName))) ||
           ((dirDepth == 1) && (QRegExp(REGEX_CASE_SYSTEM).exactMatch(subDirName))) ||
            (dirDepth == 2))
        {
            child = new QStandardItem(dirDepth<2 ? dirIcon : fileIcon, subDirName);
            child->setAccessibleDescription(subDirName);

            parentItem->appendRow(child);

            if(dirDepth < 2)
            {
                createDirectoryItem(dirPath + "/" + subDirName, child, dirDepth+1);
            }
        }
    }
}
