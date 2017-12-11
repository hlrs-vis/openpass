/******************************************************************************
* Copyright (c) 2017 ITK Engineering GmbH.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
******************************************************************************/

#ifndef OBSERVATION_OSC_IMPLEMENTATION_H
#define OBSERVATION_OSC_IMPLEMENTATION_H

#include <string>
#include <tuple>
#include <QFile>
#include <QTextStream>
#include <QXmlStreamWriter>
#include "observationInterface.h"
#include "observationtypes.h"
#include <list>


/** \addtogroup Observation_Osc
* @{
* \brief logs the current X position of every agent at each time step.
*
* This component logs the Oscs of the agents that are necessary for the
* visualisation or the evaluation into a file.
*
* The file format is xml to easily access time series or statistics data in an
* arbitrary editor.
*
* This component in this version is just for debug purpose.
*
* \section Observation_Osc_Inputs Inputs
* none
*
* \section Observation_Osc_Outputs Outputs
* none
*
* \section Observation_Osc_ConfigParameters Parameters to be specified in runConfiguration.xml
* type | id | meaning | corresponding external paramter
* -----|----|---------|----------------------------------
* string | 0 | folder where simulation output shall be stored      | Par_folder
* string | 1 | filename for temporary simulation output            | Par_tmpFilename
* string | 2 | filename for final simulation output (*.xml)        | Par_finalFilename
* bool   | 0 | flag if visualization data shall be written to file | Par_WriteOutputForVisualisation
*
* @} */

/*!
 * \brief logs the current X position of every agent at each time step.
 *
 * This component logs the Oscs of the agents that are necessary for the
 * visualisation or the evaluation into a file.
 *
 * The file format is xml to easily access time series or statistics data in an
 * arbitrary editor.
 *
 * This component in this version is just for debug purpose.
 *
 * \ingroup Observation_Osc
 */

class VehicleState {
private:
    // Positions X,Y
    double xpos;
    double ypos;

    // Vehicle Orientation
    double yawangle;

    // Time
    double time;
public:

    VehicleState(double ixpos, double iypos, double iyawangle, int itime){
        xpos = ixpos;
        ypos = iypos;
        yawangle = iyawangle;
        time = (double) itime; // itime als double speichern
        time = time/1000.0; // umrechnung von ms in s
        time = round(time*100)/100; // runde auf 100tel

    }

    double getxpos(){
        return xpos;
    }
    double getypos(){
        return ypos;
    }
    double getyawangle(){
        return yawangle;
    }
    double gettime(){
        return time;
    }


};
typedef std::list<VehicleState> stateList;

class Observation_Osc_Implementation : public ObservationInterface
{

public:
    const std::string COMPONENTNAME = "Observation_Osc";

    Observation_Osc_Implementation(StochasticsInterface *stochastics,
                                   WorldInterface *world,
                                   const ParameterInterface *parameters,
                                   const CallbackInterface *callbacks);
    Observation_Osc_Implementation(const Observation_Osc_Implementation&) = delete;
    Observation_Osc_Implementation(Observation_Osc_Implementation&&) = delete;
    Observation_Osc_Implementation& operator=(const Observation_Osc_Implementation&) = delete;
    Observation_Osc_Implementation& operator=(Observation_Osc_Implementation&&) = delete;
    virtual ~Observation_Osc_Implementation() = default;

    //-----------------------------------------------------------------------------
    //! Called by framework in master before each simulation run starts
    //-----------------------------------------------------------------------------
    virtual void MasterPreHook(){}

    //-----------------------------------------------------------------------------
    //! Called by framework in master after each simulation run ends
    //!
    //! @param[in]     filename      Name of file containing the simulation run results from the slave
    //-----------------------------------------------------------------------------
    virtual void MasterPostHook(const std::string &filename){Q_UNUSED(filename)}

    virtual void SlavePreHook(const std::string &path);

    //-----------------------------------------------------------------------------
    //! Called by framework in slave before each simulation run starts.
    //-----------------------------------------------------------------------------
    virtual void SlavePreRunHook();

    //-----------------------------------------------------------------------------
    //! Called by framework in slave at each time step.
    //! Observation module can indicate end of simulation run here.
    //!
    //! @param[in]     time        Current scheduling time
    //! @param[in,out] runResult   Reference to run result
    //-----------------------------------------------------------------------------
    virtual void SlaveUpdateHook(int time, RunResultInterface &runResult);

    //-----------------------------------------------------------------------------
    //! Called by framework in slave after each simulation run ends.
    //! Observation module can observe the current simulation run here.
    //!
    //! @param[in]     runResult   Reference to run result
    //-----------------------------------------------------------------------------
    virtual void SlavePostRunHook(const RunResultInterface &runResult);

    //-----------------------------------------------------------------------------
    //! Called by framework in slave after all simulation runs end.
    //-----------------------------------------------------------------------------
    virtual void SlavePostHook();

    //-----------------------------------------------------------------------------
    //! Called by framework in slave after all simulation runs end to transfer the
    //! observation module results to the master.
    //!
    //! @return                      File to be transferred
    //-----------------------------------------------------------------------------
    virtual const std::string SlaveResultFile(){return "";} //dummy

    std::vector<stateList> AgentLists;



private:
    void RecordAllAgents(int time);
    void RecordAgentState(int time, const AgentInterface *agent);



    static const std::string PeriodicTypeStrings[];
    static const std::string EventTypeStrings[];

    /**
    * \addtogroup Observation_Osc
    * @{
    *    @name External Parameters
    *       @{*/
    // from Parameters
    //! This flag states whether an output for visualisation should be written out or not.
    bool Par_WriteOutputForVisualisation = true;
    /** @} @} */

    //std::vector<int> timeChannel;                                //!< time values
    //std::map<std::string, std::vector<std::string>> channels;    //!< samples
    //std::map<int, std::map<int, double>> agentsPositionX;
    std::shared_ptr<QXmlStreamWriter> fileStream;
    std::shared_ptr<QXmlStreamWriter> fileStreamXosc;

    // XML Tags/Attributes/Values
    const QString OpenSCENARIOTag = "OpenSCENARIO";

    const QString FileHeaderTag = "FileHeader";
    const QString revMajorAttribute = "revMajor";
    const QString revMinorAttribute = "revMinor";
    const QString revMajorAttributeValue = "0";
    const QString revMinorAttributeValue = "0";

    const QString dateAttribute = "date";
    const QString dateAttributeValue = "2017-01-01T01:01:01+01:00";
        //#include <chrono> //auto end = std::chrono::system_clock::now();
    const QString descriptionAttribute = "description";
    const QString descriptionAttributeValue = "Studienarbeit";
    const QString authorAttribute = "author";
    const QString authorAttributeValue = "Christoph Kirsch";

    const QString ParameterDeclarationTag = "ParameterDeclaration";

    // Tags and Attributes for Trajectory file only
    const QString CatalogTag = "Catalog";
    const QString CatalogNameAttribute = "name";
    QString CatalogNameAttributeValue;

    const QString TrajectoryTag = "Trajectory";
    const QString NameAttribute = "name";
    QString TrajectoryNameAttributeValue; // = Agent_X_Trajectory


    const QString VertexTag = "Vertex";
    const QString PositionTag = "Position";

    const QString WorldTag = "World";
    const QString ShapeTag = "Shape";
    const QString referenceAttribute = "reference";
    const QString PolylineTag = "Polyline";

    // Tags and Attributes for XOSC file only
    const QString CatalogsTag = "Catalogs";
    const QString DirectoryTag = "Directory";
    const QString DirectoryAttribute = "path";

    const QString VehicleCatalogTag = "VehicleCatalog";
    const QString VehicleCatalogValue = "Catalogs/VehicleCatalogs";

    const QString DriverCatalogTag = "DriverCatalog";
    const QString DriverCatalogValue = "Catalogs/DriverCatalogs";

    const QString PedestrianCatalogTag = "PedestrianCatalog";
    const QString PedestrianCatalogValue = "Catalogs/ObserverCatalogs";

    const QString PedestrianControllerCatalogTag = "PedestrianControllerCatalog";
    const QString PedestrianControllerCatalogValue = "Catalogs/PedestrianCatalogs";

    const QString MiscObjectCatalogTag = "MiscObjectCatalog";
    const QString MiscObjectCatalogValue = "Catalogs/MiscObjectCatalogs";

    const QString EnvironmentCatalogTag = "EnvironmentCatalog";
    const QString EnvironmentCatalogValue = "Catalogs/EnvironmentCatalogs";

    const QString ManeuverCatalogTag = "ManeuverCatalog";
    const QString ManeuverCatalogValue = "Catalogs/ManeuverCatalogs";

    const QString TrajectoryCatalogTag = "TrajectoryCatalog";
    const QString TrajectoryCatalogTagValue = "Catalogs/TrajectoryCatalogs";

    const QString RouteCatalogTag = "RouteCatalog";
    const QString RouteCatalogValue = "Catalogs/RoutingCatalogs";

    const QString RoadNetworkTag = "RoadNetwork";
    const QString LogicsTag = "Logics";
    const QString filepathAttribute = "filepath";
    const QString LogicsFilePathValue = "szenario.xodr";

    const QString SceneGraphTag = "SceneGraph";
    const QString SceneGraphFilepathValue = "Tracks/Bin/RQ31_130Limit/RQ31_130Limit.opt.osgb";

    const QString EntitiesTag = "Entities";
    const QString ObjectTag = "Object";
    QString ObjectNameValue; //= AgentX

    const QString CatalogReferenceTag = "CatalogReference";
    const QString CatalogReferenceAttribute = "catalogName";
    const QString entryNameAttribute = "entryName";

    //std::list<std::string> CarModel {"golf","seat","volvo"};
    const QString carModel = "golf";
    const QString ControllerTag = "Controller";
    const QString driverModel = "DefaultDriver";

    const QString StoryboardTag = "Storyboard";
    const QString InitTag = "Init";
    const QString ActionsTag = "Actions";
    const QString PrivteTag = "Private";
    const QString objectAttribute = "object";
    const QString ActionTag = "Action";
    const QString LongitudinalTag = "Longitudinal";
    const QString SpeedTag = "Speed";
    const QString DynamicsTag = "Dynamics";
    const QString rateAttribute = "rate";
    const QString shapeAttribute = "shape";
    const QString DynamicsRateValue = "0";
    const QString DynamicsShapeValue = "step";
    const QString TargetTag = "Target";
    const QString AbsoluteTag = "Absolute";
    const QString AbsoluteValueValue = "0";
    const QString valueAttribute = "value";
    const QString LaneTag = "Lane";
    const QString laneIDAttribue = "laneId";
    const QString offsetAttribute = "offset";
    const QString roadIdAttribute = "roadId";
    const QString sAttribute = "s";
    const QString laneIdValue = "1";
    const QString offsetValue = "0.0";
    const QString roadIdValue = "0";
    const QString sValue = "10.0";

    const QString StoryTag = "Story";
    QString StoryNameValue; // = "MyStoryX"
    const QString ownerAttribute = "owner";
    QString StoryOwnerValue; // = "AgentX"
    const QString ActTag = "Act";
    QString ActNameValue;
    const QString SequenceTag = "Sequence";
    QString SequenceNameValue;
    const QString numOfExecAttribute = "numberOfExecutions";
    const QString numOfExecValue = "1";
    const QString ActorsTag = "Actors";
    const QString EntityTag = "Entity";
    const QString EntityNameValue = "$owner";
    const QString ManeuverTag = "Maneuver";
    QString ManeuverNameValue;
    const QString EventTag = "Event";
    QString EventNameValue;
    const QString EventPrioAttribute = "priority";
    const QString EventPrioValue = "overwrite";
    QString ActionNameValue;
    const QString RoutingTag = "Routing";
    const QString FollowTrajectoryTag = "FollowTrajectory";
    const QString Nonetag = "None";
    const QString LateralTag = "Lateral";
    const QString purposeAttribute = "purpose";
    const QString LateralPurposeValue = "position";
    const QString ConditionsTag = "Conditions";
    const QString StartTag = "Start";
    const QString ConditionGroupTag = "ConditionGroup";
    const QString ConditionTag = "Condition";
    const QString delayAttribute = "delay";
    const QString ConditionDelayValue = "0.00";
    const QString edgeAttribute = "edge";
    const QString ConditionEdgeValue = "rising";
    const QString ConditionNameValue = "";
    const QString ByValueTag = "ByValue";
    const QString SimulationTimeTag = "SimulationTime";
    const QString ruleAttribute = "rule";
    const QString SimulationTimeRuleValue = "equal-to";
    double SimulationTimeStart;
    const QString SimulationStartDelay = "5.0";
    const QString EndTag = "End";
    double SimulationTimeEnd;




    //!< run number
    int runNumber = 0;

    // File and Path Variables
    std::string Par_folder;
    std::string Run_folder;
    std::string Par_tmpFilename;
    std::string Par_finalFilename;
    std::string tmpPath;
    std::string finalPath;

    std::string Par_tmpXoscName;
    std::string Par_finalXoscName;
    std::string tmpXoscPath;
    std::string finalXoscPath;

    std::shared_ptr<QFile> file;
    std::shared_ptr<QFile> xoscfile;

    // coordinates of previous timestep
    double x;
    double y;
    double yaw;
    double time;

    // coordinates of current timestep
    double xnew;
    double ynew;
    double yawnew;

    // delta of current and previous timestep
    double dx;
    double dy;
    double dyaw;



};



#endif // OBSERVATION_OSC_IMPLEMENTATION_H
