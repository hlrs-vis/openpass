/*********************************************************************
* Copyright (c) 2017, 2018 ITK Engineering GmbH
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#ifndef COMPONENTPORTS_H
#define COMPONENTPORTS_H

#include <map>
#include <memory>
#include <QtGlobal>
#include "signalInterface.h"

/*!
 * \brief A Class to manage ports of a component.
 */
class ComponentPort
{
public:
    //! Constructor
    //! \param[in] id           Id of component
    //! \param[in,out] portMap  map of componentPort this componentPort shall be added
    ComponentPort(int id,
                  std::map<int, ComponentPort *> *portMap)
    {
        this->id = id;
        portMap->emplace(std::make_pair(id, this));
    }

    //! Destructor
    virtual ~ComponentPort() = default;

    ComponentPort(const ComponentPort &) = delete;
    ComponentPort(ComponentPort &&) = delete;
    ComponentPort &operator=(const ComponentPort &) = delete;
    ComponentPort &operator=(ComponentPort &&) = delete;

    //! Retrieve the id of the component
    //! \return id of component
    int GetId()
    {
        return id;
    }

    //! Set the Signal
    //! \param[in] signal with data
    //! \return
    virtual bool SetSignalValue(const std::shared_ptr<SignalInterface const> &data)
    {
        Q_UNUSED(data);
        return false;
    }

    //! Retrieve the Signal
    //! \param[out] signal with data
    //! \return
    virtual bool GetSignalValue(std::shared_ptr<SignalInterface const> &data)
    {
        Q_UNUSED(data);
        return false;
    }

private:
    int id; //!< id of component
};

template<typename SignalType, typename ValueType>
/*!
 * A Template Class to manage input ports.
 */
class InputPort : public ComponentPort
{
public:

    //! Constructor
    //! \param[in] id           Id of input port
    //! \param[in,out] portMap  map of input port this input port shall be added
    InputPort(int id,
              std::map<int, ComponentPort *> *portMap):
        ComponentPort(id, portMap)
    {}

    //! Destructor
    virtual ~InputPort() = default;

    InputPort(const InputPort &) = delete;
    InputPort(InputPort &&) = delete;
    InputPort &operator=(const InputPort &) = delete;
    InputPort &operator=(InputPort &&) = delete;

    //! Retrieve the current value of the input port
    //! \return value of input port
    ValueType GetValue()
    {
        return value;
    }

    bool SetSignalValue(const std::shared_ptr<SignalInterface const> &data)
    {
        const std::shared_ptr<SignalType const> signal = std::dynamic_pointer_cast<SignalType const>(data);
        if (!signal)
        {
            return false;
        }
        value = signal->value;

        return true;
    }

    bool SetDefaultValue(const ValueType &inValue)
    {
        try
        {
            value = inValue;
        }
        catch (...)
        {
            return false;
        }
        return true;
    }

private:
    ValueType value; //!< current value of input port
};

template<typename SignalType, typename ValueType>
/*!
 * A Template Class to manage output ports.
 */
class OutputPort : public ComponentPort
{
public:
    //! Constructor
    //! \param[in] id           Id of output port
    //! \param[in,out] portMap  map of output port this output port shall be added
    OutputPort(int id,
               std::map<int, ComponentPort *> *portMap):
        ComponentPort(id, portMap)
    {}

    //! Destructor
    virtual ~OutputPort() = default;

    OutputPort(const OutputPort &) = delete;
    OutputPort(OutputPort &&) = delete;
    OutputPort &operator=(const OutputPort &) = delete;
    OutputPort &operator=(OutputPort &&) = delete;

    //! Set the value of the output port
    //! \param value    value to be set
    void SetValue(ValueType value)
    {
        this->value = value;
    }

    bool GetSignalValue(std::shared_ptr<SignalInterface const> &data)
    {
        try
        {
            data = std::make_shared<SignalType const>(value);
            return true;
        }
        catch (const std::bad_alloc &)
        {
            return false;
        }
    }

private:
    ValueType value; //!< internal value
};

template<typename ValueType>
/*!
 * A Template Class to manage external parameter of component
 */
class externalParameter
{
public:
    //! Constructor
    //! \param[in] id           Id of externalParameter
    //! \param[in,out] portMap  map of externalParameter this externalParameter shall be added
    externalParameter(int id,
                      std::map<int, externalParameter<ValueType>*> *externalParameterMap):
        id(id)
    {
        externalParameterMap->emplace(std::make_pair(id, this));
    }

    //! Destructor
    virtual ~externalParameter() = default;

    externalParameter(const externalParameter &) = delete;
    externalParameter(externalParameter &&) = delete;
    externalParameter &operator=(const externalParameter &) = delete;
    externalParameter &operator=(externalParameter &&) = delete;

    //! Retrieve the id of the parameter
    //! \return id
    int GetId()
    {
        return id;
    }

    //! Retrieve the value of the parameter
    //! \return value
    ValueType GetValue()
    {
        return value;
    }

    //! Set the value of the parameter
    //! \param[in] value to be set
    void SetValue(ValueType value)
    {
        this->value = value;
    }

private:
    int id;             //!< id of parameter
    ValueType value;    //!< value of parameter
};


#endif // COMPONENTPORTS_H
