// Copyright (c) 2023-2025 Orange. All rights reserved.
// This software is distributed under the BSD 3-Clause-clear License, the text of which is available
// at https://spdx.org/licenses/BSD-3-Clause-Clear.html or see the "LICENSE" file for more details.

////////////////////////////////////////////////////////////
// Mon May 23 11:49:33 2011
// File generated  with GenereTable
// Insert your specific code inside "//## " sections


#ifndef KRSELECTIONPARAMETERS_H
#define KRSELECTIONPARAMETERS_H


#include "Object.h"

//## Custom includes

//##


////////////////////////////////////////////////////////////
// Classe KRSelectionParameters
//    Selection parameters
class KRSelectionParameters : public Object
{
public:
    // Constructeur
    KRSelectionParameters();
    ~KRSelectionParameters();

    // Copie et duplication
    void CopyFrom(const KRSelectionParameters* aSource);
    KRSelectionParameters* Clone() const;


    ////////////////////////////////////////////////////////
    // Acces aux attributs


    // Max number of selected variables
    int GetMaxSelectedAttributeNumber() const;
    void SetMaxSelectedAttributeNumber(int nValue);

    // Selection criterion
    const ALString& GetSelectionCriterion() const;
    void SetSelectionCriterion(const ALString& sValue);

    // Optimization algorithm
    const ALString& GetOptimizationAlgorithm() const;
    void SetOptimizationAlgorithm(const ALString& sValue);

    // Optimization level
    int GetOptimizationLevel() const;
    void SetOptimizationLevel(int nValue);

    // Trace level
    int GetTraceLevel() const;
    void SetTraceLevel(int nValue);

    // Trace selected variables
    boolean GetTraceSelectedAttributes() const;
    void SetTraceSelectedAttributes(boolean bValue);


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
    int nMaxSelectedAttributeNumber;
    ALString sSelectionCriterion;
    ALString sOptimizationAlgorithm;
    int nOptimizationLevel;
    int nTraceLevel;
    boolean bTraceSelectedAttributes;

    //## Custom implementation

    //##
};




////////////////////////////////////////////////////////////
// Implementations inline


inline int KRSelectionParameters::GetMaxSelectedAttributeNumber() const
{
    return nMaxSelectedAttributeNumber;
}

inline void KRSelectionParameters::SetMaxSelectedAttributeNumber(int nValue)
{
    nMaxSelectedAttributeNumber = nValue;
}

inline const ALString& KRSelectionParameters::GetSelectionCriterion() const
{
    return sSelectionCriterion;
}

inline void KRSelectionParameters::SetSelectionCriterion(const ALString& sValue)
{
    sSelectionCriterion = sValue;
}

inline const ALString& KRSelectionParameters::GetOptimizationAlgorithm() const
{
    return sOptimizationAlgorithm;
}

inline void KRSelectionParameters::SetOptimizationAlgorithm(const ALString& sValue)
{
    sOptimizationAlgorithm = sValue;
}

inline int KRSelectionParameters::GetOptimizationLevel() const
{
    return nOptimizationLevel;
}

inline void KRSelectionParameters::SetOptimizationLevel(int nValue)
{
    nOptimizationLevel = nValue;
}

inline int KRSelectionParameters::GetTraceLevel() const
{
    return nTraceLevel;
}

inline void KRSelectionParameters::SetTraceLevel(int nValue)
{
    nTraceLevel = nValue;
}

inline boolean KRSelectionParameters::GetTraceSelectedAttributes() const
{
    return bTraceSelectedAttributes;
}

inline void KRSelectionParameters::SetTraceSelectedAttributes(boolean bValue)
{
    bTraceSelectedAttributes = bValue;
}


//## Custom inlines

//##


#endif // KRSELECTIONPARAMETERS_H
