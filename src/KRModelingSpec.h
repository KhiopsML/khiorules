// Copyright (c) 2023-2025 Orange. All rights reserved.
// This software is distributed under the BSD 3-Clause-clear License, the text of which is available
// at https://spdx.org/licenses/BSD-3-Clause-Clear.html or see the "LICENSE" file for more details.

////////////////////////////////////////////////////////////
// Fri Apr 22 09:28:19 2011
// File generated  with GenereTable
// Insert your specific code inside "//## " sections


#ifndef KRMODELINGSPEC_H
#define KRMODELINGSPEC_H


#include "Object.h"
#include "KWModelingSpec.h"

//## Custom includes

#include "KRPredictorRule.h"
//##


////////////////////////////////////////////////////////////
// Classe KRModelingSpec
//    Modeling parameters
class KRModelingSpec : public KWModelingSpec
{
public:
    // Constructeur
    KRModelingSpec();
    ~KRModelingSpec();

    // Copie et duplication
    void CopyFrom(const KRModelingSpec* aSource);
    KRModelingSpec* Clone() const;


    ////////////////////////////////////////////////////////
    // Acces aux attributs


    // Rule Based predictor
    boolean GetRuleBasedPredictor() const;
    void SetRuleBasedPredictor(boolean bValue);

    // Selective Naive Bayes predictor
    boolean GetSelectiveNaiveBayesPredictor() const;
    void SetSelectiveNaiveBayesPredictor(boolean bValue);

    // Naive Bayes predictor
    boolean GetNaiveBayesPredictor() const;
    void SetNaiveBayesPredictor(boolean bValue);


    ////////////////////////////////////////////////////////
    // Divers

    // Ecriture
    void Write(ostream& ost) const;

    // Libelles utilisateur
    const ALString GetClassLabel() const;
    const ALString GetObjectLabel() const;


    //## Custom declarations

	// Parametrage d'un classifieur a base de regles
	KRPredictorRule* GetPredictorRule();

    //##

    ////////////////////////////////////////////////////////
    //// Implementation
protected:
    // Attributs de la classe
    boolean bRuleBasedPredictor;
    boolean bSelectiveNaiveBayesPredictor;
    boolean bNaiveBayesPredictor;

    //## Custom implementation

	//Parametrage du classifieur
	KRPredictorRule predictorRule;
    //##
};




////////////////////////////////////////////////////////////
// Implementations inline


inline boolean KRModelingSpec::GetRuleBasedPredictor() const
{
    return bRuleBasedPredictor;
}

inline void KRModelingSpec::SetRuleBasedPredictor(boolean bValue)
{
    bRuleBasedPredictor = bValue;
}

inline boolean KRModelingSpec::GetSelectiveNaiveBayesPredictor() const
{
    return bSelectiveNaiveBayesPredictor;
}

inline void KRModelingSpec::SetSelectiveNaiveBayesPredictor(boolean bValue)
{
    bSelectiveNaiveBayesPredictor = bValue;
}

inline boolean KRModelingSpec::GetNaiveBayesPredictor() const
{
    return bNaiveBayesPredictor;
}

inline void KRModelingSpec::SetNaiveBayesPredictor(boolean bValue)
{
    bNaiveBayesPredictor = bValue;
}


//## Custom inlines

//##


#endif // KRMODELINGSPEC_H
