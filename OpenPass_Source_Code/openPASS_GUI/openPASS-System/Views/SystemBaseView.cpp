/****************************************************************************** 
* Copyright (c) 2017 Volkswagen Group of America. 
* 
* This program and the accompanying materials are made 
* available under the terms of the Eclipse Public License 2.0 
* which is available at https://www.eclipse.org/legal/epl-2.0/ 
* 
* SPDX-License-Identifier: EPL-2.0 
******************************************************************************/ 

#include "Views/SystemBaseView.h"

#include "openPASS-System/SystemComponentInputItemInterface.h"
#include "openPASS-System/SystemComponentOutputItemInterface.h"
#include "openPASS-System/SystemComponentItemInterface.h"
#include "openPASS-System/SystemItemInterface.h"
#include "Views/SystemComponentInputItemView.h"
#include "Views/SystemComponentOutputItemView.h"
#include "Views/SystemComponentItemView.h"
#include <QEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPaintEvent>
#include <QSignalBlocker>

QPoint const SystemBaseView::FrameSize = QPoint(40, 20);
QMap<bool, QColor> const SystemBaseView::ConnectionsCurveColorMap = {
    {false, QColor(QStringLiteral("#cdcdcd"))},
    {true, QColor(QStringLiteral("#777777"))},
};

SystemBaseView::SystemBaseView(SystemItemInterface * const system,
                               QWidget * const parent)
    : QWidget(parent)
    , system(system)
    , componentsModifyMode(false)
    , connectionsModifyMode(false)
    , inputActive(nullptr)
    , inputConnection(nullptr)
    , outputActive(nullptr)
    , outputConnection(nullptr)
    , internalConnectionsCurveMap({{false, {}}, {true, {}}})
{
    // Connect SystemComponent*Interface with SystemBaseView
    connect(system->getComponents(), &SystemComponentMapInterface::mode,
            this, &SystemBaseView::componentsMode);
    connect(system->getComponents(), &SystemComponentMapInterface::cleared,
            this, &SystemBaseView::componentsClear);
    connect(system->getComponents(), &SystemComponentMapInterface::added,
            this, &SystemBaseView::componentsAdd);
    connect(system->getComponents(), &SystemComponentMapInterface::removed,
            this, &SystemBaseView::componentsRemove);
    connect(system->getConnections(), &SystemConnectionMapInterface::mode,
            this, &SystemBaseView::connectionsMode);
    connect(system->getConnections(), &SystemConnectionMapInterface::modified,
            this, &SystemBaseView::connectionsUpdate);

    // Update size
    updateSize();
}

void SystemBaseView::updateInternalComponents()
{
    // Update SystemComponentItemInterface to SystemComponentItemView map
    internalComponentsMap.clear();
    for (SystemComponentItemView * const componentView : findChildren<SystemComponentItemView *>(
             QString(), Qt::FindDirectChildrenOnly))
    {
        internalComponentsMap.insert(componentView->getInterface(), componentView);
    }

    // Update SystemComponentInputItemInterface to SystemComponentInputItemView map
    internalComponentsInputMap.clear();
    for (SystemComponentInputItemView * const inputView : findChildren<SystemComponentInputItemView *>())
    {
        internalComponentsInputMap.insert(inputView->getInterface(), inputView);
    }

    // Update SystemComponentOutputItemInterface to SystemComponentOutputItemView map
    internalComponentsOutputMap.clear();
    for (SystemComponentOutputItemView * const outputView : findChildren<SystemComponentOutputItemView *>())
    {
        internalComponentsOutputMap.insert(outputView->getInterface(), outputView);
    }
}

void SystemBaseView::updateInternalConnections()
{
    // Update SystemComponentInputItemView list and SystemComponentOutputItemView list with connections
    internalConnectionsInputList.clear();
    internalConnectionsOutputList.clear();
    for (SystemConnectionItemInterface const * const relation : *system->getConnections())
    {
        if ((internalComponentsInputMap.contains(relation->getTarget())) &&
                (!internalConnectionsInputList.contains(internalComponentsInputMap[relation->getTarget()])))
        {
            internalConnectionsInputList.append(internalComponentsInputMap[relation->getTarget()]);
        }
        if ((internalComponentsOutputMap.contains(relation->getSource())) &&
                (!internalConnectionsOutputList.contains(internalComponentsOutputMap[relation->getSource()])))
        {
            internalConnectionsOutputList.append(internalComponentsOutputMap[relation->getSource()]);
        }
    }
}

void SystemBaseView::updateInternalConnectionsCurves()
{
    // Update list of connection curves indexed by connection selection
    internalConnectionsCurveMap[false].clear();
    internalConnectionsCurveMap[true].clear();
    for (SystemConnectionItemInterface const * const connection : *system->getConnections())
    {
        // Is connection valid?
        if (internalComponentsOutputMap.contains(connection->getSource()) &&
                internalComponentsMap.contains(connection->getSource()->getComponent()) &&
                internalComponentsInputMap.contains(connection->getTarget()) &&
                internalComponentsMap.contains(connection->getTarget()->getComponent()))
        {
            // Is connection selected?
            bool const selected =
                    ((inputActive != nullptr) && (connection->getTarget() == inputActive->getInterface())) ||
                    ((outputActive != nullptr) && (connection->getSource() == outputActive->getInterface()));

            // Calculate connection curve
            QPoint point0 = internalComponentsMap[connection->getSource()->getComponent()]->pos();
            QPoint point1 = internalComponentsMap[connection->getTarget()->getComponent()]->pos();
            if (point0.x() - point1.x() > 0)
            {
                point0 += (internalComponentsOutputMap[connection->getSource()]->geometry().topLeft() + QPoint(0, 11));
                point1 += (internalComponentsInputMap[connection->getTarget()]->geometry().topRight() + QPoint(0, 11));
            }
            else
            {
                point0 += (internalComponentsOutputMap[connection->getSource()]->geometry().topRight() + QPoint(0, 11));
                point1 += (internalComponentsInputMap[connection->getTarget()]->geometry().topLeft() + QPoint(0, 11));
            }

            // Store connection curve
            internalConnectionsCurveMap[selected].append({point0, point1});
        }
    }

    // Update widget
    update();
}

void SystemBaseView::updateSize()
{
    // Initialize size
    QSize size = QSize(0, 0);

    // Calculate required space by components
    QRect space;
    for (SystemComponentItemView * const componentView : internalComponentsMap)
    {
        space = space.united(componentView->geometry());
    }

    // Is the component space valid?
    if (space.isValid())
    {
        // Is the component space correctly framed?
        if (space.topLeft() != FrameSize)
        {
            // Move all components to be correctly framed
            QPoint const pointMoveBy = FrameSize - space.topLeft();
            for (SystemComponentItemView * const componentView : internalComponentsMap)
            {
                // Block component view and move component interface
                QSignalBlocker blocker(componentView);
                componentView->getInterface()->setPosition(
                            componentView->getInterface()->getPosition() + pointMoveBy);
                blocker.unblock();
            }

            // Move component space
            space.moveTo(FrameSize);
        }

        // Set size to component space in frame
        QPoint const pointBottomRight = space.bottomRight() + FrameSize;
        size = QSize(pointBottomRight.x(), pointBottomRight.y());
    }

    // Update size
    resize(size);

    // Update connection curves internals
    updateInternalConnectionsCurves();
}

void SystemBaseView::componentsMode(bool const & modify)
{
    // Update component modify mode and components
    componentsModifyMode = modify;
    componentsUpdate();
}

void SystemBaseView::componentsUpdate()
{
    // Is the component modify mode inactive?
    if (!componentsModifyMode)
    {
        // Update component internals and size
        updateInternalComponents();
        updateSize();
    }
}

void SystemBaseView::componentsClear()
{
    // Destroy all internal stored component views
    for (SystemComponentItemView * const componentView : internalComponentsMap)
    {
        // Destroy component view
        delete componentView;
    }

    // Update components
    componentsUpdate();
}

void SystemBaseView::componentsAdd(SystemComponentItemInterface * const component)
{
    // Create component view
    SystemComponentItemView * const componentView = new SystemComponentItemView(component, this);

    // Connect SystemComponentItemView with SystemBaseView
    connect(componentView, &SystemComponentItemView::moved,
            this, &SystemBaseView::updateSize);
    connect(componentView, &SystemComponentItemView::inputMouseEnter,
            this, &SystemBaseView::inputMouseEnter);
    connect(componentView, &SystemComponentItemView::inputMouseLeave,
            this, &SystemBaseView::inputMouseLeave);
    connect(componentView, &SystemComponentItemView::inputConnectionAdd,
            this, &SystemBaseView::inputConnectionAdd);
    connect(componentView, &SystemComponentItemView::inputConnectionRemove,
            this, &SystemBaseView::inputConnectionRemove);
    connect(componentView, &SystemComponentItemView::outputMouseEnter,
            this, &SystemBaseView::outputMouseEnter);
    connect(componentView, &SystemComponentItemView::outputMouseLeave,
            this, &SystemBaseView::outputMouseLeave);
    connect(componentView, &SystemComponentItemView::outputConnectionAdd,
            this, &SystemBaseView::outputConnectionAdd);
    connect(componentView, &SystemComponentItemView::outputConnectionRemove,
            this, &SystemBaseView::outputConnectionRemove);
    connect(componentView, &SystemComponentItemView::pressEscape,
            this, &SystemBaseView::pressEscape);

    // Update components
    componentsUpdate();
}

void SystemBaseView::componentsRemove(SystemComponentItemInterface * const component)
{
    // Does the component interface have a corresponding component view?
    if (internalComponentsMap.contains(component))
    {
        // Destroy corresponding component view
        delete internalComponentsMap[component];

        // Update components
        componentsUpdate();
    }
}

void SystemBaseView::connectionsMode(bool const & modify)
{
    // Update connection modify mode and connections
    connectionsModifyMode = modify;
    connectionsUpdate();
}

void SystemBaseView::connectionsUpdate()
{
    // Is the connection modify mode inactive?
    if (!connectionsModifyMode)
    {
        // Update connection internals
        updateInternalConnections();
        updateInternalConnectionsCurves();
    }
}

void SystemBaseView::inputMouseEnter(SystemComponentInputItemView * const input)
{
    // Update active input
    inputActive = input;

    // Does the active input have any connections?
    if (internalConnectionsInputList.contains(input))
    {
        // Update connection curves internals
        updateInternalConnectionsCurves();
    }
}

void SystemBaseView::inputMouseLeave(SystemComponentInputItemView * const input)
{
    // Has this event been emitted by the active input?
    if (inputActive == input)
    {
        // Reset active input
        inputActive = nullptr;

        // Does the previous active input have any connections?
        if (internalConnectionsInputList.contains(input))
        {
            // Update connection curves internals
            updateInternalConnectionsCurves();
        }
    }
}

void SystemBaseView::inputConnectionAdd(SystemComponentInputItemView * const input)
{
    // Have we already selected the input interface?
    if (inputConnection == input)
    {
        // Reset input interface
        inputConnection = nullptr;

        // Enable all input widgets
        for (SystemComponentInputItemView * const inputView : internalComponentsInputMap)
        {
            inputView->setAddEnabled(true);
        }

        // Uncheck input widget
        input->setAddChecked(false);

        // Enable all output widgets
        for (SystemComponentOutputItemView * const outputView : internalComponentsOutputMap)
        {
            outputView->setAddEnabled(true);
        }
    }
    // Have we selected an output interface?
    else if (outputConnection)
    {
        // Add connection between output and input
        system->getConnections()->add(outputConnection->getInterface(),
                                      input->getInterface());

        // Uncheck output widget
        outputConnection->setAddChecked(false);

        // Reset output interface
        outputConnection = nullptr;

        // Enable all output widgets
        for (SystemComponentOutputItemView * const outputView : internalComponentsOutputMap)
        {
            outputView->setAddEnabled(true);
        }

        // Uncheck input widget
        input->setAddChecked(false);

        // Enable all input widgets
        for (SystemComponentInputItemView * const inputView : internalComponentsInputMap)
        {
            inputView->setAddEnabled(true);
        }
    }
    // No output interface nor input interface is selected
    else
    {
        // Select input interface
        inputConnection = input;

        // Disable all input widgets
        for (SystemComponentInputItemView * const inputView : internalComponentsInputMap)
        {
            inputView->setAddEnabled(false);
        }

        // Disable all output widgets
        for (SystemComponentOutputItemView * const outputView : internalComponentsOutputMap)
        {
            outputView->setAddEnabled(false);
        }

        // Check and enable input widget
        input->setAddChecked(true);
        input->setAddEnabled(true);

        // Enable compatible output widgets
        for (SystemComponentOutputItemInterface const * const outputInterface :
             system->getCompatibleSources(input->getInterface()))
        {
            if (internalComponentsOutputMap.contains(outputInterface))
            {
                internalComponentsOutputMap[outputInterface]->setAddEnabled(true);
            }
        }
    }
}

void SystemBaseView::inputConnectionRemove(SystemComponentInputItemView * const input)
{
    // Remove all connections connecting an output to the input
    system->getConnections()->remove(input->getInterface());
}

void SystemBaseView::outputMouseEnter(SystemComponentOutputItemView * const output)
{
    // Update active output
    outputActive = output;

    // Does the active output have any connections?
    if (internalConnectionsOutputList.contains(output))
    {
        // Update connection curves internals
        updateInternalConnectionsCurves();
    }
}

void SystemBaseView::outputMouseLeave(SystemComponentOutputItemView * const output)
{
    // Has this event been emitted by the active output?
    if (outputActive == output)
    {
        // Reset active output
        outputActive = nullptr;

        // Does the previous active output have any connections?
        if (internalConnectionsOutputList.contains(output))
        {
            // Update connection curves internals
            updateInternalConnectionsCurves();
        }
    }
}

void SystemBaseView::outputConnectionAdd(SystemComponentOutputItemView * const output)
{
    // Have we already selected the output interface?
    if (outputConnection == output)
    {
        // Reset output interface
        outputConnection = nullptr;

        // Enable all output widgets
        for (SystemComponentOutputItemView * const outputView : internalComponentsOutputMap)
        {
            outputView->setAddEnabled(true);
        }

        // Uncheck output widget
        output->setAddChecked(false);

        // Enable all input widgets
        for (SystemComponentInputItemView * const inputView : internalComponentsInputMap)
        {
            inputView->setAddEnabled(true);
        }
    }
    // Have we selected an input interface?
    else if (inputConnection)
    {
        // Add connection between output and input
        system->getConnections()->add(output->getInterface(),
                                      inputConnection->getInterface());

        // Uncheck input widget
        inputConnection->setAddChecked(false);

        // Reset input interface
        inputConnection = nullptr;

        // Enable all input widgets
        for (SystemComponentInputItemView * const inputView : internalComponentsInputMap)
        {
            inputView->setAddEnabled(true);
        }

        // Uncheck output widget
        output->setAddChecked(false);

        // Enable all output widgets
        for (SystemComponentOutputItemView * const outputView : internalComponentsOutputMap)
        {
            outputView->setAddEnabled(true);
        }
    }
    // No output interface nor input interface is selected
    else
    {
        // Select output interface
        outputConnection = output;

        // Disable all output widgets
        for (SystemComponentOutputItemView * const outputView : internalComponentsOutputMap)
        {
            outputView->setAddEnabled(false);
        }

        // Disable all input widgets
        for (SystemComponentInputItemView * const inputView : internalComponentsInputMap)
        {
            inputView->setAddEnabled(false);
        }

        // Check and enable output widget
        output->setAddChecked(true);
        output->setAddEnabled(true);

        // Enable compatible input widgets
        for (SystemComponentInputItemInterface const * const inputInterface :
             system->getCompatibleTargets(output->getInterface()))
        {
            if (internalComponentsInputMap.contains(inputInterface))
            {
                internalComponentsInputMap[inputInterface]->setAddEnabled(true);
            }
        }
    }
}

void SystemBaseView::outputConnectionRemove(SystemComponentOutputItemView * const output)
{
    // Remove all connections connecting the output to an input
    system->getConnections()->remove(output->getInterface());
}

/*
 * Unchecking any activated input or output by pressing the Escape key.*/
void SystemBaseView::pressEscape()
{
    if (outputConnection)
    {
        // Uncheck output widget
        outputConnection->setAddChecked(false);

        // Reset output interface
        outputConnection = nullptr;
    }

    for (SystemComponentOutputItemView * const outputView : internalComponentsOutputMap)
    {
        // Enable all output widgets
        outputView->setAddEnabled(true);
    }

    if (inputConnection)
    {
        // Uncheck input widget
        inputConnection->setAddChecked(false);

        // Reset input interface
        inputConnection = nullptr;
    }

    for (SystemComponentInputItemView * const inputView : internalComponentsInputMap)
    {
        // Enable all input widgets
        inputView->setAddEnabled(true);
    }
}

void SystemBaseView::paintEvent(QPaintEvent *)
{
    // Initialize painter
    QPainter painter(this);

    // Draw all precalculated connection curves between components
    for (bool const & selected : {false, true})
    {
        painter.setPen(ConnectionsCurveColorMap[selected]);
        for (QPair<QPoint, QPoint> const & curve : internalConnectionsCurveMap[selected])
        {
            // Draw cubic curve between first point and second point
            QPainterPath path(curve.first);
            path.cubicTo(curve.first.x() + .6 * (curve.second.x() - curve.first.x()), curve.first.y(),
                         curve.first.x() + .4 * (curve.second.x() - curve.first.x()), curve.second.y(),
                         curve.second.x(), curve.second.y());
            painter.drawPath(path);
        }
    }
}
