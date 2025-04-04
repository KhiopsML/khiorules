// Copyright (c) 2023-2025 Orange. All rights reserved.
// This software is distributed under the BSD 3-Clause-clear License, the text of which is available
// at https://spdx.org/licenses/BSD-3-Clause-Clear.html or see the "LICENSE" file for more details.

////////////////////////////////////////////////////////////
// Mon May 23 11:49:33 2011
// File generated  with GenereTable
// Insert your specific code inside "//## " sections


#include "KRRuleParameters.h"


KRRuleParameters::KRRuleParameters()
{
    nTimeAllowed = 600;
    nMaxRuleNumber = 10;
    bAddingFeatures = true;
    nPassesOverDatabase = 0;
    nRuleOptimizationLoops = 0;
    nContinuousOptimizationLoops = 0;
    nSymbolOptimizationLoops = 0;

    //## Custom constructor

	sSubPredictor = "SNB";

    //##
}


KRRuleParameters::~KRRuleParameters()
{

    //## Custom destructor

    //##
}


void KRRuleParameters::CopyFrom(const KRRuleParameters* aSource)
{
    require(aSource != NULL);

    nTimeAllowed = aSource->nTimeAllowed;
    nMaxRuleNumber = aSource->nMaxRuleNumber;
    bAddingFeatures = aSource->bAddingFeatures;
    nPassesOverDatabase = aSource->nPassesOverDatabase;
    nRuleOptimizationLoops = aSource->nRuleOptimizationLoops;
    nContinuousOptimizationLoops = aSource->nContinuousOptimizationLoops;
    nSymbolOptimizationLoops = aSource->nSymbolOptimizationLoops;
    sSubPredictor = aSource->sSubPredictor;

    //## Custom copyfrom

    //##
}


KRRuleParameters* KRRuleParameters::Clone() const
{
    KRRuleParameters* aClone;

    aClone = new KRRuleParameters;
    aClone->CopyFrom(this);

    //## Custom clone

    //##
    return aClone;
}


void KRRuleParameters::Write(ostream& ost) const
{
    ost << "Time allowed for training\t" << GetTimeAllowed() << "\n";
    ost << "Maximum number of rules to mine\t" << GetMaxRuleNumber() << "\n";
    ost << "Adding rules as new features\t" << BooleanToString(GetAddingFeatures()) << "\n";
    ost << "Number of passes over database for small data\t" << GetPassesOverDatabase() << "\n";
    ost << "Number of optimization loops over the attributes of the rule\t" << GetRuleOptimizationLoops() << "\n";
    ost << "Number of optimization loops over a continuous attribute\t" << GetContinuousOptimizationLoops() << "\n";
    ost << "Number of optimization loops over a categorical attribute\t" << GetSymbolOptimizationLoops() << "\n";
    ost << "End predictor\t" << GetSubPredictor() << "\n";
}


const ALString KRRuleParameters::GetClassLabel() const
{
    return "Rule parameters";
}


//## Method implementation

const ALString KRRuleParameters::GetObjectLabel() const
{
    ALString sLabel;

    return sLabel;
}

//##
