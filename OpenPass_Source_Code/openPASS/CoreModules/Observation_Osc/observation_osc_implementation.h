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
#include <stdlib.h>

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
    QString getStrDate();
    QString getCarModel();



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
    QString dateAttributeValue;
        //#include <chrono> //auto end = std::chrono::system_clock::now();

    // Tags and Attributes for Trajectory file only
    QString CatalogNameAttributeValue;

    QString TrajectoryNameAttributeValue; // = Agent_X_Trajectory




    // Tags and Attributes for XOSC file only
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

    const QString LogicsFilePathValue = "szenario.xodr";

    const QString SceneGraphFilepathValue = "Tracks/Bin/RQ31_130Limit/RQ31_130Limit.opt.osgb";

    QString ObjectNameValue; //= AgentX


    std::vector<QString> CarModelList {"golf","seat","volvo"};
    QString CarModel;

    const QString driverModel = "DefaultDriver";

    // Init Position rel Street
    const QString laneIdValue = "1";
    const QString offsetValue = "0";
    const QString roadIdValue = "0";
    double sValue;

    // init Position with world coordinates
    double init_xPos;
    double init_yPos;
    double init_zPos = 0; // no output of z-Coordinate
    double init_yawAngle;


    const QString StoryNameValue = "MyStory";
    QString StoryOwnerValue; // = "AgentX"
    QString ActNameValue;
    QString SequenceNameValue;

    QString EntityNameValue;
    QString ManeuverNameValue;

    QString EventNameValue;

    QString ActionNameValue;

    double SimulationTimeStart;
    double SimulationTimeEnd;


    int randomCarModelIndex;

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


};



#endif // OBSERVATION_OSC_IMPLEMENTATION_H
