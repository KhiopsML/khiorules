// Copyright (c) 2023-2025 Orange. All rights reserved.
// This software is distributed under the BSD 3-Clause-clear License, the text of which is available
// at https://spdx.org/licenses/BSD-3-Clause-Clear.html or see the "LICENSE" file for more details.

////////////////////////////////////////////////////////////
// Fri Apr 22 09:28:19 2011
// File generated  with GenereTable
// Insert your specific code inside "//## " sections


#include "KRModelingSpec.h"


KRModelingSpec::KRModelingSpec()
{
    bRuleBasedPredictor = false;
    bSelectiveNaiveBayesPredictor = false;
    bNaiveBayesPredictor = false;

    //## Custom constructor

    // Valeurs par defaut standard
    bRuleBasedPredictor = true;
    bSelectiveNaiveBayesPredictor = false;
    bNaiveBayesPredictor = false;

	//##
}


KRModelingSpec::~KRModelingSpec()
{

    //## Custom destructor

    //##
}


void KRModelingSpec::CopyFrom(const KRModelingSpec* aSource)
{
    require(aSource != NULL);

    KWModelingSpec::CopyFrom(aSource);
    bRuleBasedPredictor = aSource->bRuleBasedPredictor;
    bSelectiveNaiveBayesPredictor = aSource->bSelectiveNaiveBayesPredictor;
    bNaiveBayesPredictor = aSource->bNaiveBayesPredictor;

    //## Custom copyfrom

	//##
}


KRModelingSpec* KRModelingSpec::Clone() const
{
    KRModelingSpec* aClone;

    aClone = new KRModelingSpec;
    aClone->CopyFrom(this);

    //## Custom clone

    //##
    return aClone;
}


void KRModelingSpec::Write(ostream& ost) const
{
    KWModelingSpec::Write(ost);
    ost << "Rule Based predictor\t" << BooleanToString(GetRuleBasedPredictor()) << "\n";
    ost << "Selective Naive Bayes predictor\t" << BooleanToString(GetSelectiveNaiveBayesPredictor()) << "\n";
    ost << "Naive Bayes predictor\t" << BooleanToString(GetNaiveBayesPredictor()) << "\n";
}


const ALString KRModelingSpec::GetClassLabel() const
{
    return "Modeling parameters";
}


//## Method implementation

const ALString KRModelingSpec::GetObjectLabel() const
{
    ALString sLabel;

    return sLabel;
}

KRPredictorRule* KRModelingSpec::GetPredictorRule()
{
	return &predictorRule;
}

//##
