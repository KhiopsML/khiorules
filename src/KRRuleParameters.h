// Copyright (c) 2023-2025 Orange. All rights reserved.
// This software is distributed under the BSD 3-Clause-clear License, the text of which is available
// at https://spdx.org/licenses/BSD-3-Clause-Clear.html or see the "LICENSE" file for more details.

////////////////////////////////////////////////////////////
// Mon May 23 11:49:33 2011
// File generated  with GenereTable
// Insert your specific code inside "//## " sections


#ifndef KRRULEPARAMETERS_H
#define KRRULEPARAMETERS_H


#include "Object.h"

//## Custom includes

//##


////////////////////////////////////////////////////////////
// Classe KRRuleParameters
//    Rule parameters
class KRRuleParameters : public Object
{
public:
    // Constructeur
    KRRuleParameters();
    ~KRRuleParameters();

    // Copie et duplication
    void CopyFrom(const KRRuleParameters* aSource);
    KRRuleParameters* Clone() const;


    ////////////////////////////////////////////////////////
    // Acces aux attributs


    // Time allowed for training
    int GetTimeAllowed() const;
    void SetTimeAllowed(int nValue);

    // Maximum number of rules to mine
    int GetMaxRuleNumber() const;
    void SetMaxRuleNumber(int nValue);

    // Adding rules as new features
    boolean GetAddingFeatures() const;
    void SetAddingFeatures(boolean bValue);

    // Number of passes over database for small data
    int GetPassesOverDatabase() const;
    void SetPassesOverDatabase(int nValue);

    // Number of optimization loops over the attributes of the rule
    int GetRuleOptimizationLoops() const;
    void SetRuleOptimizationLoops(int nValue);

    // Number of optimization loops over a continuous attribute
    int GetContinuousOptimizationLoops() const;
    void SetContinuousOptimizationLoops(int nValue);

    // Number of optimization loops over a categorical attribute
    int GetSymbolOptimizationLoops() const;
    void SetSymbolOptimizationLoops(int nValue);

    // End predictor
    const ALString& GetSubPredictor() const;
    void SetSubPredictor(const ALString& sValue);


    ////////////////////////////////////////////////////////
    // Divers

    // Ecriture
    void Write(ostream& ost) const;

    // Libelles utilisateur
    const ALString GetClassLabel() const;
    const ALString GetObjectLabel() const;


    //## Custom declarations

    //##

    ////////////////////////////////////////////////////////
    //// Implementation
protected:
    // Attributs de la classe
    int nTimeAllowed;
    int nMaxRuleNumber;
    boolean bAddingFeatures;
    int nPassesOverDatabase;
    int nRuleOptimizationLoops;
    int nContinuousOptimizationLoops;
    int nSymbolOptimizationLoops;
    ALString sSubPredictor;

    //## Custom implementation

    //##
};




////////////////////////////////////////////////////////////
// Implementations inline


inline int KRRuleParameters::GetTimeAllowed() const
{
    return nTimeAllowed;
}

inline void KRRuleParameters::SetTimeAllowed(int nValue)
{
    nTimeAllowed = nValue;
}

inline int KRRuleParameters::GetMaxRuleNumber() const
{
    return nMaxRuleNumber;
}

inline void KRRuleParameters::SetMaxRuleNumber(int nValue)
{
    nMaxRuleNumber = nValue;
}

inline boolean KRRuleParameters::GetAddingFeatures() const
{
    return bAddingFeatures;
}

inline void KRRuleParameters::SetAddingFeatures(boolean bValue)
{
    bAddingFeatures = bValue;
}

inline int KRRuleParameters::GetPassesOverDatabase() const
{
    return nPassesOverDatabase;
}

inline void KRRuleParameters::SetPassesOverDatabase(int nValue)
{
    nPassesOverDatabase = nValue;
}

inline int KRRuleParameters::GetRuleOptimizationLoops() const
{
    return nRuleOptimizationLoops;
}

inline void KRRuleParameters::SetRuleOptimizationLoops(int nValue)
{
    nRuleOptimizationLoops = nValue;
}

inline int KRRuleParameters::GetContinuousOptimizationLoops() const
{
    return nContinuousOptimizationLoops;
}

inline void KRRuleParameters::SetContinuousOptimizationLoops(int nValue)
{
    nContinuousOptimizationLoops = nValue;
}

inline int KRRuleParameters::GetSymbolOptimizationLoops() const
{
    return nSymbolOptimizationLoops;
}

inline void KRRuleParameters::SetSymbolOptimizationLoops(int nValue)
{
    nSymbolOptimizationLoops = nValue;
}

inline const ALString& KRRuleParameters::GetSubPredictor() const
{
    return sSubPredictor;
}

inline void KRRuleParameters::SetSubPredictor(const ALString& sValue)
{
    sSubPredictor = sValue;
}


//## Custom inlines

//##


#endif // KRRULEPARAMETERS_H
