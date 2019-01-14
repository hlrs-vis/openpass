# /*********************************************************************
# * Copyright (c) 2017, 2018 ITK Engineering GmbH
# *
# * This program and the accompanying materials are made
# * available under the terms of the Eclipse Public License 2.0
# * which is available at https://www.eclipse.org/legal/epl-2.0/
# *
# * SPDX-License-Identifier: EPL-2.0
# **********************************************************************/

HEADERS += \
    $$PWD/ModelPcm.h \
    $$PWD/ConfigurationGeneratorPcm/ConfigGeneratorPcm.h \
    $$PWD/ConfigurationGeneratorPcm/DatabaseReaderPcm.h \
    $$PWD/ConfigurationGeneratorPcm/ConfigWriter.h \
    $$PWD/ConfigurationGeneratorPcm/stochasticsPCM.h \
    $$PWD/ConfigurationGeneratorPcm/DataStructuresXml/XmlComponentSignal.h \
    $$PWD/ConfigurationGeneratorPcm/DataStructuresXml/XmlParameter.h \
    $$PWD/ConfigurationGeneratorPcm/DataStructuresXml/XmlComponentObservation.h \
    $$PWD/ConfigurationGeneratorPcm/DataStructuresXml/XmlComponent.h \
    $$PWD/ConfigurationGeneratorPcm/DataStructuresXml/XmlAgentEquipment.h \
    $$PWD/ConfigurationGeneratorPcm/DataStructuresXml/XmlSpawnPoint.h \
    $$PWD/ConfigurationGeneratorPcm/DataStructuresXml/XmlAgent.h \
    $$PWD/ConfigurationGeneratorPcm/DataStructuresXml/XmlObservation.h \
    $$PWD/ConfigurationGeneratorPcm/DataStructuresXml/XmlRunConfig.h \
    $$PWD/ConfigurationGeneratorPcm/DataStructuresXml/XmlMarks.h \
    $$PWD/ConfigurationGeneratorPcm/DataStructuresXml/XmlObject.h \
    $$PWD/ConfigurationGeneratorPcm/DataStructuresXml/XmlTrajectory.h \
    $$PWD/ConfigurationGeneratorPcm/DataStructuresXml/XmlViewObject.h \
    $$PWD/ConfigurationGeneratorPcm/DataStructuresXml/XmlIntendedCourses.h \
    $$PWD/ConfigurationGeneratorPcm/DataStructuresXml/XmlGlobalData.h \
    $$PWD/ConfigurationGeneratorPcm/DataStructuresXml/XmlScenery.h \
    $$PWD/ConfigurationGeneratorPcm/XmlMergeHelper.h \
    $$PWD/FileHelper.h \
    $$PWD/ConfigurationGeneratorPcm/DataStructuresXml/XmlLine.h \
    $$PWD/ConfigurationGeneratorPcm/DataStructuresXml/XmlPoint.h

SOURCES += \
    $$PWD/ModelPcm.cpp \
    $$PWD/ConfigurationGeneratorPcm/ConfigGeneratorPcm.cpp \
    $$PWD/ConfigurationGeneratorPcm/DatabaseReaderPcm.cpp \
    $$PWD/ConfigurationGeneratorPcm/ConfigWriter.cpp \
    $$PWD/ConfigurationGeneratorPcm/stochasticsPCM.cpp \
    $$PWD/ConfigurationGeneratorPcm/DataStructuresXml/XmlComponentSignal.cpp \
    $$PWD/ConfigurationGeneratorPcm/DataStructuresXml/XmlParameter.cpp \
    $$PWD/ConfigurationGeneratorPcm/DataStructuresXml/XmlComponentObservation.cpp \
    $$PWD/ConfigurationGeneratorPcm/DataStructuresXml/XmlComponent.cpp \
    $$PWD/ConfigurationGeneratorPcm/DataStructuresXml/XmlAgentEquipment.cpp \
    $$PWD/ConfigurationGeneratorPcm/DataStructuresXml/XmlSpawnPoint.cpp \
    $$PWD/ConfigurationGeneratorPcm/DataStructuresXml/XmlAgent.cpp \
    $$PWD/ConfigurationGeneratorPcm/DataStructuresXml/XmlObservation.cpp \
    $$PWD/ConfigurationGeneratorPcm/DataStructuresXml/XmlRunConfig.cpp \
    $$PWD/ConfigurationGeneratorPcm/DataStructuresXml/XmlMarks.cpp \
    $$PWD/ConfigurationGeneratorPcm/DataStructuresXml/XmlObject.cpp \
    $$PWD/ConfigurationGeneratorPcm/DataStructuresXml/XmlTrajectory.cpp \
    $$PWD/ConfigurationGeneratorPcm/DataStructuresXml/XmlViewObject.cpp \
    $$PWD/ConfigurationGeneratorPcm/DataStructuresXml/XmlIntendedCourses.cpp \
    $$PWD/ConfigurationGeneratorPcm/DataStructuresXml/XmlGlobalData.cpp \
    $$PWD/ConfigurationGeneratorPcm/DataStructuresXml/XmlScenery.cpp \
    $$PWD/ConfigurationGeneratorPcm/XmlMergeHelper.cpp \
    $$PWD/FileHelper.cpp \
    $$PWD/ConfigurationGeneratorPcm/DataStructuresXml/XmlLine.cpp \
    $$PWD/ConfigurationGeneratorPcm/DataStructuresXml/XmlPoint.cpp
