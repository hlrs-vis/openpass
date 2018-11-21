/*********************************************************************
* Copyright c 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include "DatabaseReaderPcm.h"

DatabaseReader::DatabaseReader()
{
}

DatabaseReader::~DatabaseReader()
{
    CloseDataBase();
}

bool DatabaseReader::SetDatabase(QString &dbName)
{
    if (!QFileInfo::exists(dbName))
    {
        return false;
    }

    if (IsDataBaseOpen())
    {
        CloseDataBase();
    }

    databaseName = dbName;

    return true;
}

bool DatabaseReader::OpenDataBase()
{
    if (!QFileInfo::exists(databaseName))
    {
        return false;
    }

    bool success = true;

    if (IsDataBaseOpen())
    {
        CloseDataBase();
    }

    db = QSqlDatabase::addDatabase("QODBC");
    QString dbString = "Driver={Microsoft Access Driver (*.mdb)};DSN='';DBQ="
                       + databaseName;
    db.setDatabaseName(dbString);
    connection = db.connectionName();

    success =  db.open();
    if (!success)
    {
        CloseDataBase();
    }

    return success;
}

bool DatabaseReader::CloseDataBase()
{
    if (IsDataBaseOpen())
    {
        db.close();
        db = QSqlDatabase();
        QSqlDatabase::removeDatabase(connection);
        connection = "";
    }

    return true;
}

bool DatabaseReader::IsDataBaseOpen()
{
    return db.isOpen();
}

bool DatabaseReader::ReadCaseList(QStringList &caseList)
{
    if (!IsDataBaseOpen())
    {
        return false;
    }

    // Read the Cases from the table "participant_data"
    QSqlQuery query;

    query.exec("SELECT DISTINCT FALL FROM participant_data ORDER BY FALL ASC");
    while (query.next())
    {
        caseList.append(QString("%1").arg(query.value(0).toInt()));
    }

    query.clear();

    return true;

}

bool DatabaseReader::ReadAllData(const QString &pcmCase,
                                 std::vector<PCM_ParticipantData> &participants,
                                 std::vector<PCM_InitialValues> &initials,
                                 std::vector<PCM_Trajectory *> &trajectories,
                                 std::vector<PCM_Marks *> &marksVec,
                                 PCM_Object &object,
                                 PCM_ViewObject &viewObject,
                                 PCM_IntendedCourses &intendedCourses,
                                 PCM_GlobalData &globalData)
{
    if (!ReadParticipantData(pcmCase,
                             participants))
    {
        std::cout << "Error (ConfigGenerator): could not read participant data" << std::endl;
        return false;
    }

    if (!ReadDynamicsData(pcmCase,
                          initials))
    {
        std::cout << "Error (ConfigGenerator): could not read dynamics data" << std::endl;
        return false;
    }

    if (!ReadTrajectoryData(pcmCase,
                            trajectories))
    {
        std::cout << "Error (ConfigGenerator): could not read trajectory data" << std::endl;
        return false;
    }

    if (!ReadMarksData(pcmCase,
                       marksVec))
    {
        std::cout << "Error (ConfigGenerator): could not read marks data" << std::endl;
        return false;
    }

    if (!ReadObjectsData(pcmCase,
                         object))
    {
        std::cout << "Error (ConfigGenerator): could not read object data" << std::endl;
        return false;
    }

    if (!ReadViewObjectsData(pcmCase,
                             viewObject))
    {
        std::cout << "Error (ConfigGenerator): could not read viewObject data" << std::endl;
        return false;
    }

    if (!ReadIntendedCourseData(pcmCase,
                                intendedCourses))
    {
        std::cout << "Error (ConfigGenerator): could not read intendedCourses data" << std::endl;
        return false;
    }

    if (!ReadGlobalData(pcmCase,
                        globalData))
    {
        std::cout << "Error (ConfigGenerator): could not read intendedCourses data" << std::endl;
        return false;
    }

    return true;
}

bool DatabaseReader::ReadParticipantData(const QString &pcmCase,
                                         std::vector<PCM_ParticipantData> &participants)
{
    if (!IsDataBaseOpen())
    {
        return false;
    }

    QSqlQuery query;
    query.exec("SELECT TYPEPCM,WIDTH,LENGTH,DISTCGFA,WEIGHT,HEIGHTCG,WHEELBASE,IXX,IYY,IZZ,MUE,TRACKWIDTH,HEIGHT,CGFRONT FROM participant_data WHERE FALL = "
               + pcmCase);
    while (query.next())
    {
        participants.push_back(PCM_ParticipantData(query.value(0).toString(),
                                                   query.value(1).toString(),
                                                   query.value(2).toString(),
                                                   query.value(3).toString(),
                                                   query.value(4).toString(),
                                                   query.value(5).toString(),
                                                   query.value(6).toString(),
                                                   query.value(7).toString(),
                                                   query.value(8).toString(),
                                                   query.value(9).toString(),
                                                   query.value(10).toString(),
                                                   query.value(11).toString(),
                                                   query.value(12).toString(),
                                                   query.value(13).toString()));
    }

    query.clear();

    return !(participants.empty());
}

bool DatabaseReader::ReadDynamicsData(const QString &pcmCase,
                                      std::vector<PCM_InitialValues> &initials)
{
    if (!IsDataBaseOpen())
    {
        return false;
    }

    QSqlQuery query;
    query.exec("SELECT XPOS,YPOS,VX,VY,AX,AY,PSI FROM dynamics WHERE STEP = 0 AND FALL = " + pcmCase);
    while (query.next())
    {
        initials.push_back(PCM_InitialValues(query.value(0).toString(),
                                             query.value(1).toString(),
                                             query.value(2).toString(),
                                             query.value(3).toString(),
                                             query.value(4).toString(),
                                             query.value(5).toString(),
                                             query.value(6).toString()));
    }

    query.clear();

    return !(initials.empty());
}

bool DatabaseReader::ReadTrajectoryData(const QString &pcmCase,
                                        std::vector<PCM_Trajectory *> &trajectories)
{
    if (!IsDataBaseOpen())
    {
        return false;
    }

    QSqlQuery query;
    query.exec("SELECT BETNR FROM dynamics WHERE FALL = " + pcmCase + " GROUP BY BETNR");
    std::vector<QString>betNr;
    while (query.next())
    {
        betNr.push_back(query.value(0).toString());
    }

    query.clear();

    for (size_t i = 0; i < betNr.size(); i++)
    {
        query.exec( "SELECT STEP,XPOS,YPOS,VX,VY,PSI FROM dynamics WHERE BETNR = "
                    + betNr.at(i) + "AND FALL = " + pcmCase + " ORDER BY STEP ASC" );


        std::vector<int> *timeVec = new std::vector<int>;
        std::vector<double> *xPosVec = new std::vector<double>;
        std::vector<double> *yPosVec = new std::vector<double>;
        std::vector<double> *uVelVec = new std::vector<double>;
        std::vector<double> *vVelVec = new std::vector<double>;
        std::vector<double> *psiVec = new std::vector<double>;

        while (query.next())
        {
            timeVec->push_back((qRound(query.value(0).toDouble() * 1000)));
            xPosVec->push_back(query.value(1).toDouble());
            yPosVec->push_back(query.value(2).toDouble());
            uVelVec->push_back(query.value(3).toDouble());
            vVelVec->push_back(query.value(4).toDouble());
            psiVec->push_back(query.value(5).toDouble());
        }

        trajectories.push_back(new PCM_Trajectory(timeVec,
                                                  xPosVec,
                                                  yPosVec,
                                                  uVelVec,
                                                  vVelVec,
                                                  psiVec));
    }

    query.clear();

    return !(trajectories.empty());
}

bool DatabaseReader::ReadMarksData(const QString &pcmCase, std::vector<PCM_Marks *> &marksVec)
{
    if (!IsDataBaseOpen())
    {
        return false;
    }

    for (size_t i = 1; i < static_cast<int>(MarkType::NumberOfMarkTypes); i++)
    {
        PCM_Marks *marks = new PCM_Marks(static_cast<MarkType>(i));

        QString marksTypeName = QString::fromStdString(PCM_Helper::ConvertMarkTypeToDBString(
                                                           marks->GetMarkType()));

        QSqlQuery query;
        query.exec( "SELECT LINENO,POINTNO,X,Y,Z FROM " + marksTypeName + " WHERE FALL = " + pcmCase +
                    " ORDER BY LINENO, POINTNO");

        while (query.next())
        {
            int lineNo = query.value(0).toInt();
            int pointNo = query.value(1).toInt();
            double x = query.value(2).toDouble();
            double y = query.value(3).toDouble();
            double z = query.value(4).toDouble();

            AddLineData(marks, lineNo, pointNo, x, y, z);
        }
        marksVec.push_back(marks);
        query.clear();
    }

    return true;
}

bool DatabaseReader::ReadObjectsData(const QString &pcmCase, PCM_Object &objects)
{
    if (!IsDataBaseOpen())
    {
        return false;
    }

    QString objectsName = QString::fromStdString(PCM_Helper::ConvertObjectTypeToDBString(
                                                     ObjectType::OBJECT));

    QSqlQuery query;
    query.exec( "SELECT LINENO,POINTNO,X,Y,Z,OBJTYPE FROM " + objectsName + " WHERE FALL = " + pcmCase +
                " ORDER BY LINENO, POINTNO");

    while (query.next())
    {
        int lineNo = query.value(0).toInt();
        int pointNo = query.value(1).toInt();
        double x = query.value(2).toDouble();
        double y = query.value(3).toDouble();
        double z = query.value(4).toDouble();
        int objectType = query.value(5).toInt();

        AddLineData(&objects, lineNo, pointNo, x, y, z);
        objects.SetObjectType(objectType);
    }

    query.clear();

    return true;
}

bool DatabaseReader::ReadViewObjectsData(const QString &pcmCase, PCM_ViewObject &viewObject)
{
    if (!IsDataBaseOpen())
    {
        return false;
    }

    QString viewObjectsName = QString::fromStdString(PCM_Helper::ConvertObjectTypeToDBString(
                                                         ObjectType::VIEWOBJECT));

    QSqlQuery query;
    query.exec( "SELECT LINENO,POINTNO,X,Y,Z FROM " + viewObjectsName + " WHERE FALL = " + pcmCase +
                " ORDER BY LINENO, POINTNO");

    while (query.next())
    {
        int lineNo = query.value(0).toInt();
        int pointNo = query.value(1).toInt();
        double x = query.value(2).toDouble();
        double y = query.value(3).toDouble();
        double z = query.value(4).toDouble();

        AddLineData(&viewObject, lineNo, pointNo, x, y, z);
    }

    query.clear();

    return true;
}

bool DatabaseReader::ReadIntendedCourseData(const QString &pcmCase,
                                            PCM_IntendedCourses &intendedCources)
{
    if (!IsDataBaseOpen())
    {
        return false;
    }

    QString intendedCourseName = QString::fromStdString(
                                     PCM_Helper::GetIntendedCourseDBString());

    QSqlQuery query;
    query.exec( "SELECT BETNR,POINTNO,X,Y,Z FROM " + intendedCourseName + " WHERE FALL = " + pcmCase +
                " ORDER BY BETNR, POINTNO");

    while (query.next())
    {
        int betNr = query.value(0).toInt();
        int pointNo = query.value(1).toInt();
        double x = query.value(2).toDouble();
        double y = query.value(3).toDouble();
        double z = query.value(4).toDouble();

        if (!intendedCources.IsCoursePresent(betNr))
        {
            intendedCources.AddPCM_Course(PCM_Course(betNr));
        }
        PCM_Course *course = intendedCources.GetCourseByBetNr(betNr);

        if (!course->IsPointPresent(pointNo))
        {
            PCM_Point *newPoint = new PCM_Point(pointNo, x, y, z);
            course->AddPCM_Point(newPoint);
        }
        else
        {
            std::cout << "WARNING: Point: " << std::to_string(pointNo)
                      << " already exists in course: "
                      << std::to_string(betNr)
                      << ". Point is not added to line." << std::endl;
        }

    }

    query.clear();

    return true;
}

bool DatabaseReader::ReadGlobalData(const QString &pcmCase, PCM_GlobalData &globalData)
{
    if (!IsDataBaseOpen())
    {
        return false;
    }

    QString globalDataName = QString::fromStdString(PCM_Helper::GetGlobalDataDBString());

    QSqlQuery query;
    query.exec( "SELECT OFFSETX,OFFSETY,PARTICIP,SIMUVERS FROM " + globalDataName + " WHERE FALL = " +
                pcmCase);

    while (query.next())
    {
        double offsetX = query.value(0).toDouble();
        double offsetY = query.value(1).toDouble();
        int participants = query.value(2).toInt();
        int simulationVersion = query.value(3).toInt();

        globalData.SetOffsetX(offsetX);
        globalData.SetOffsetY(offsetY);
        globalData.SetParticipants(participants);
        globalData.SetSimulationVersion(simulationVersion);
    }
    query.clear();

    return true;
}

bool DatabaseReader::AddLineData(PCM_LineContainer *lineContainer, int lineNo, int pointNo,
                                 double x, double y, double z)
{
    if (!lineContainer->IsLinePresent(lineNo))
    {
        PCM_Line *newPcmLine = new PCM_Line(lineNo);
        lineContainer->AddPCM_Line(newPcmLine);
    }
    PCM_Line *line = lineContainer->GetLineById(lineNo);

    if (!line->IsPointPresent(pointNo))
    {
        PCM_Point *newPoint = new PCM_Point(pointNo, x, y, z);
        line->AddPCM_Point(newPoint);
    }
    else
    {
        std::cout << "WARNING: Point: " << std::to_string(pointNo)
                  << " already exists in line: "
                  << std::to_string(lineNo)
                  << ". Point is not added to line." << std::endl;
        return false;
    }

    return true;
}
