# /*********************************************************************
# * Copyright (c) 2017, 2018, 2019 in-tech GmbH
# *
# * This program and the accompanying materials are made
# * available under the terms of the Eclipse Public License 2.0
# * which is available at https://www.eclipse.org/legal/epl-2.0/
# *
# * SPDX-License-Identifier: EPL-2.0
# **********************************************************************/

TEMPLATE = subdirs
SUBDIRS = \
        CoreFramework/OpenPassMaster \
        CoreFramework/OpenPassSlave \
        CoreModules/EventDetector \
        CoreModules/Manipulator \
        CoreModules/Observation_Log \
        CoreModules/SpawnPoint_OSI \
        CoreModules/Stochastics \
        CoreModules/World_OSI \
        Components/Action_LongitudinalDriver \
        Components/Action_SecondaryDriverTasks \
        Components/AgentUpdater \
        Components/Algorithm_AEB \
        Components/Algorithm_Lateral \
        Components/Algorithm_Longitudinal \
        Components/AlgorithmAgentFollowingDriverModel \
        Components/ComponentController \
        Components/Dynamics_Collision \
        Components/Dynamics_RegularDriving \
        Components/Dynamics_TrajectoryFollower \
        Components/LimiterAccelerationVehicleComponents \
        Components/Parameters_Vehicle \
        Components/Sensor_Driver \
        Components/Sensor_OSI \
        Components/Sensor_RecordState \
        Components/SensorFusion_OSI \
        Components/SignalPrioritizer
