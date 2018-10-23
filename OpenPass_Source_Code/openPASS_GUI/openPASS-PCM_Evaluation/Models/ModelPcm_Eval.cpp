#include "Models/ModelPcm_Eval.h"

ModelPcm_Eval::ModelPcm_Eval(QObject *parent)
    : QObject(parent)
{
    listModelPcm = new QStringListModel(this);
    listModelPcm->setStringList(resultList);
}

ModelPcm_Eval::~ModelPcm_Eval()
{
    for (auto tableModel : tableMap)
    {
        delete tableModel;
    }

    if (listModelPcm != nullptr)
    {
        delete listModelPcm;
    }
}

void ModelPcm_Eval::LoadPathData(const QString &pathName)
{
    rootPath = pathName;

    Q_EMIT Clear();
    selectedIndexList.clear();

    QDir pathDir(pathName);
    resultList = pathDir.entryList(QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);
    listModelPcm->setStringList(resultList);
}

void ModelPcm_Eval::OnSelectionChanged(const QItemSelection &selected,
                                       const QItemSelection &deselected)
{
    Q_UNUSED(deselected);

    Q_EMIT Clear();

    for (QModelIndex index : selected.indexes())
    {
        selectedIndexList.append(index);
    }
    for (QModelIndex index : deselected.indexes())
    {
        selectedIndexList.removeOne(index);
    }

    for (QModelIndex index : selectedIndexList)
    {
        QString pcmCase = index.data().toString();
        QString resultFolder = rootPath + "/" + pcmCase + "/Results";
        QDir resultDir(resultFolder);
        QStringList resultsFileList = resultDir.entryList(QDir::Files | QDir::NoSymLinks);
        for (QString file : resultsFileList)
        {
            QString resultFile = resultFolder + "/" + file;
            LoadFileData(resultFile);
        }

        QString sceneryFile = rootPath + "/" + pcmCase + "/sceneryConfiguration.xml";
        LoadSceneryData(sceneryFile);
    }
}

QAbstractItemModel *ModelPcm_Eval::GetItemModelPcm() const
{
    return listModelPcm;
}

void ModelPcm_Eval::SetSelectionModelPcm(QItemSelectionModel *selectionModel)
{
    selectionModelPcm = selectionModel;
    connect(selectionModelPcm, &QItemSelectionModel::selectionChanged,
            this, &ModelPcm_Eval::OnSelectionChanged);
}

bool ModelPcm_Eval::LoadFileData(const QString &fileName)
{
    QStringList stringList = fileName.split(".");
    if (stringList.last() == "csv")
    {
        TableModelCsv *tableModel;
        if (tableMap.contains(fileName))
        {
            tableModel = tableMap.value(fileName);
        }
        else
        {
            tableModel = new TableModelCsv(this, fileName);
            tableMap.insert(fileName, tableModel);
        }

        QFileInfo fileInfo(fileName);
        Q_EMIT TableModelCreated(tableModel, fileInfo.fileName());

        QVector<QPointF> *trajectoryData = tableModel->GetTrajectoryData();
        Q_EMIT TrajectoryCreated(trajectoryData);

        return true;
    }

    return false;
}

bool ModelPcm_Eval::LoadSceneryData(const QString &sceneryFile)
{
    PCM_Data pcmData;
    SceneryImporterPCM sceneryImporter;
    std::map<int, PCM_Trajectory> trajectories;
    if (!sceneryImporter.Import(sceneryFile.toStdString(), pcmData, trajectories))
    {
        return false;
    }

    std::vector<const PCM_Marks *> *marksVec = pcmData.GetMarksVec();

    for (const PCM_Marks *marks : *marksVec)
    {
        for (const std::pair<const int, const PCM_Line *> pcmLinePair : *marks->GetLineMap())
        {
            QVector<QPointF> line;
            const PCM_Line *pcmLine = pcmLinePair.second;
            ConvertPcmLine(pcmLine, line);
            Q_EMIT MarksCreated(&line, marks->GetMarkType());
        }
    }

    const PCM_Object *object = pcmData.GetObject();
    for (const std::pair<const int, const PCM_Line *> pcmLinePair : *object->GetLineMap())
    {
        QVector<QPointF> line;
        const PCM_Line *pcmLine = pcmLinePair.second;
        ConvertPcmLine(pcmLine, line);
        Q_EMIT ObjectCreated(&line, ObjectType::OBJECT);
    }

    const PCM_ViewObject *viewObject = pcmData.GetViewObject();
    for (const std::pair<const int, const PCM_Line *> pcmLinePair : *viewObject->GetLineMap())
    {
        QVector<QPointF> line;
        const PCM_Line *pcmLine = pcmLinePair.second;
        ConvertPcmLine(pcmLine, line);
        Q_EMIT ObjectCreated(&line, ObjectType::VIEWOBJECT);
    }

    return true;
}

QVector<QPointF> ModelPcm_Eval::ConvertPcmLine(const PCM_Line *pcmLine, QVector<QPointF> &line)
{
    for (const std::pair<int, const PCM_Point *> pcmPointPair : *pcmLine->GetPointMap())
    {
        const PCM_Point *pcmPoint = pcmPointPair.second;
        QPointF point(pcmPoint->GetX(), pcmPoint->GetY());
        line.push_back(point);
    }

    return line;
}
