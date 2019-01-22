/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "ViewPcm_Eval.h"
#include "ui_ViewPcm_Eval.h"

const WindowInterface::ID ViewPcm_Eval::ViewID = QStringLiteral("openPASS.PCM_Eval");

ViewPcm_Eval::ViewPcm_Eval(WindowInterface *const window,
                           QWidget *parent)
    : QWidget(parent)
    , window(window)
    , ui(new Ui::ViewPcm_Eval)
    , actionMenuBrowse(WindowInterface::createAction(tr("Browse Result Files"), this,
                                                     &ViewPcm_Eval::ActionBrowse))

{
    // Create UI
    ui->setupUi(this);

    // Customize the menu
    actionMenuBrowse->setEnabled(true);
    // Customize the view

    // Update the view

    // Signals-Slots connection declaration for the Menu-bar Actions

    tabwidget = ui->tabWidget;
    tabwidget->setTabsClosable(false);
    connect(tabwidget, &QTabWidget::tabCloseRequested,
            this, &ViewPcm_Eval::OnTabClose);

    trafficScene = new TrafficScene(this);
    ui->graphicsView->setScene(trafficScene);
    ui->graphicsView->scale(1, -1);

    // Register view
    window->add(ViewID, WindowInterface::createButton(tr("PCM-Evaluation"), 0, 00100), this,
    {actionMenuBrowse});
    /*!
      * Registering the view in QHash structure
      * allowing creating a button (name, type, position ->00100)
      * allowing connecting with the correspondant widget
      * allowing creating menu ({} -> empty in this case)
      */
}

ViewPcm_Eval::~ViewPcm_Eval()
{
    for (auto widget : tabwidget->findChildren<QWidget *>
            (QString(), Qt::FindDirectChildrenOnly))
    {
        delete widget;
    }
    delete tabwidget;
    delete trafficScene;

    // Deregister view
    window->remove(ViewID);

    // Destroy UI
    delete ui;
}

void ViewPcm_Eval::AddTableModel(QAbstractTableModel *tableModel, const QString &tableName)
{
    QTableView *tableView = new QTableView(this);
    tableView->setObjectName(tableName);
    tableView->setModel(tableModel);

    tabwidget->addTab(tableView, tableName);
}

void ViewPcm_Eval::SetModelPcm(QAbstractItemModel *model)
{
    ui->treeViewResults->setModel(model);
}

QItemSelectionModel *ViewPcm_Eval::GetSelectionModelPcm()
{
    return ui->treeViewResults->selectionModel();
}

void ViewPcm_Eval::AddTrajectory(QVector<QPointF> *trajectoryData)
{
    trafficScene->AddTrajectory(trajectoryData);
    ui->graphicsView->fitInView(trafficScene->sceneRect(), Qt::KeepAspectRatio);
}

void ViewPcm_Eval::AddMarks(QVector<QPointF> *marksData, MarkType markType)
{
    trafficScene->AddMark(marksData, markType);
    ui->graphicsView->fitInView(trafficScene->sceneRect(), Qt::KeepAspectRatio);
}

void ViewPcm_Eval::AddObject(QVector<QPointF> *objectData, ObjectType objectType)
{
    trafficScene->AddObject(objectData, objectType);
    ui->graphicsView->fitInView(trafficScene->sceneRect(), Qt::KeepAspectRatio);
}

void ViewPcm_Eval::OnClear()
{
    tabwidget->clear();
    trafficScene->clear();
    delete trafficScene;
    trafficScene = new TrafficScene(this);
    ui->graphicsView->setScene(trafficScene);
    ui->graphicsView->fitInView(trafficScene->sceneRect(), Qt::KeepAspectRatio);
}

void ViewPcm_Eval::ActionBrowse()
{
    QDir const root = QDir(QCoreApplication::applicationDirPath());
    QString const resultPath = QFileDialog::getExistingDirectory(
                                   this, tr("Open Result Folder"), root.canonicalPath(),
                                   QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (!resultPath.isNull())
    {
        Q_EMIT ResultPathChanged(resultPath);
    }

    ui->treeViewResults->expandAll();
}

void ViewPcm_Eval::OnTabClose(int index)
{
    tabwidget->removeTab(index);
    trafficScene->RemoveTrajectory(index);
}

void ViewPcm_Eval::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    ui->graphicsView->fitInView(trafficScene->sceneRect(), Qt::KeepAspectRatio);
}

void ViewPcm_Eval::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    ui->graphicsView->fitInView(trafficScene->sceneRect(), Qt::KeepAspectRatio);
}
