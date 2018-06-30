/**********************************************
* Daimler AG *
***********************************************/

//-----------------------------------------------------------------------------
//! @file  vectorSignals.h
//! @brief This file contains signals
//!
//-----------------------------------------------------------------------------
#ifndef VECTOR_SIGNALS
#define VECTOR_SIGNALS

#include "modelInterface.h"

//-----------------------------------------------------------------------------
//! Primitive signal class for double vectors
//-----------------------------------------------------------------------------
template <typename DT>
class SignalVector : public SignalInterface
{
public:
    // Construct from std::vector
    SignalVector(std::vector<double> inValue) : value(inValue)
    {}
    // Construct from SignalVector by copy
    SignalVector(const SignalVector &inSignal) : value(inSignal.value)
    {}
    // Construct from SignalVector by moving
    SignalVector(SignalVector &&inSignal) : value(std::move(inSignal.value))
    {}

    // Assign from SignalVector by copy
    SignalVector &operator=(const SignalVector &inSignal)
    {
        if (value.size() != inSignal.value.size())
        {
            value.resize(inSignal.value.size());
        }
        value = inSignal.value;
        return *this;
    }
    // Assign from SignalVector by moving
    SignalVector &operator=(SignalVector &&inSignal)
    {
        value = std::move(inSignal.value);
        return *this;
    }
    virtual ~SignalVector() = default;

    //-----------------------------------------------------------------------------
    //! Converts signal to string
    //-----------------------------------------------------------------------------
    virtual operator std::string() const
    {
        std::string signalString;
        for (auto elem : value)
        {
            signalString += std::to_string(elem);
            signalString += "_";
        }
        signalString.erase(signalString.end());
        return signalString;
    }

    std::vector<DT> value; //!< signal content
};

typedef SignalVector<double> SignalVectorDouble;
typedef SignalVector<int> SignalVectorInt;
typedef SignalVector<bool> SignalVectorBool;

#endif // VECTOR_SIGNALS