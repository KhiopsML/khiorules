// Copyright (c) 2023-2025 Orange. All rights reserved.
// This software is distributed under the BSD 3-Clause-clear License, the text of which is available
// at https://spdx.org/licenses/BSD-3-Clause-Clear.html or see the "LICENSE" file for more details.

////////////////////////////////////////////////////////////
// Mon May 23 11:49:33 2011
// File generated  with GenereTable
// Insert your specific code inside "//## " sections


#ifndef KRTRAINPARAMETERS_H
#define KRTRAINPARAMETERS_H


#include "Object.h"

//## Custom includes

//##


////////////////////////////////////////////////////////////
// Classe KRTrainParameters
//    Train parameters
class KRTrainParameters : public Object
{
public:
    // Constructeur
    KRTrainParameters();
    ~KRTrainParameters();

    // Copie et duplication
    void CopyFrom(const KRTrainParameters* aSource);
    KRTrainParameters* Clone() const;


    ////////////////////////////////////////////////////////
    // Acces aux attributs


    // Time allowed for training
    int GetTimeAllowed() const;
    void SetTimeAllowed(int nValue);

    // Maximum number of rules to mine
    int GetMaxRuleNumber() const;
    void SetMaxRuleNumber(int nValue);

    // Number of passes over database for small data
    int GetPassesOverDatabase() const;
    void SetPassesOverDatabase(int nValue);


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
    int nPassesOverDatabase;

    //## Custom implementation

    //##
};




////////////////////////////////////////////////////////////
// Implementations inline


inline int KRTrainParameters::GetTimeAllowed() const
{
    return nTimeAllowed;
}

inline void KRTrainParameters::SetTimeAllowed(int nValue)
{
    nTimeAllowed = nValue;
}

inline int KRTrainParameters::GetMaxRuleNumber() const
{
    return nMaxRuleNumber;
}

inline void KRTrainParameters::SetMaxRuleNumber(int nValue)
{
    nMaxRuleNumber = nValue;
}

inline int KRTrainParameters::GetPassesOverDatabase() const
{
    return nPassesOverDatabase;
}

inline void KRTrainParameters::SetPassesOverDatabase(int nValue)
{
    nPassesOverDatabase = nValue;
}


//## Custom inlines

//##


#endif // KRTRAINPARAMETERS_H
