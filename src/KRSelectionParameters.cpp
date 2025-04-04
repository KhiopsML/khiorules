// Copyright (c) 2023-2025 Orange. All rights reserved.
// This software is distributed under the BSD 3-Clause-clear License, the text of which is available
// at https://spdx.org/licenses/BSD-3-Clause-Clear.html or see the "LICENSE" file for more details.

////////////////////////////////////////////////////////////
// Mon May 23 11:49:33 2011
// File generated  with GenereTable
// Insert your specific code inside "//## " sections


#include "KRSelectionParameters.h"


KRSelectionParameters::KRSelectionParameters()
{
    nMaxSelectedAttributeNumber = 0;
    nOptimizationLevel = 0;
    nTraceLevel = 0;
    bTraceSelectedAttributes = false;

    //## Custom constructor

    //##
}


KRSelectionParameters::~KRSelectionParameters()
{

    //## Custom destructor

    //##
}


void KRSelectionParameters::CopyFrom(const KRSelectionParameters* aSource)
{
    require(aSource != NULL);

    nMaxSelectedAttributeNumber = aSource->nMaxSelectedAttributeNumber;
    sSelectionCriterion = aSource->sSelectionCriterion;
    sOptimizationAlgorithm = aSource->sOptimizationAlgorithm;
    nOptimizationLevel = aSource->nOptimizationLevel;
    nTraceLevel = aSource->nTraceLevel;
    bTraceSelectedAttributes = aSource->bTraceSelectedAttributes;

    //## Custom copyfrom

    //##
}


KRSelectionParameters* KRSelectionParameters::Clone() const
{
    KRSelectionParameters* aClone;

    aClone = new KRSelectionParameters;
    aClone->CopyFrom(this);

    //## Custom clone

    //##
    return aClone;
}


void KRSelectionParameters::Write(ostream& ost) const
{
    ost << "Max number of selected variables\t" << GetMaxSelectedAttributeNumber() << "\n";
    ost << "Selection criterion\t" << GetSelectionCriterion() << "\n";
    ost << "Optimization algorithm\t" << GetOptimizationAlgorithm() << "\n";
    ost << "Optimization level\t" << GetOptimizationLevel() << "\n";
    ost << "Trace level\t" << GetTraceLevel() << "\n";
    ost << "Trace selected variables\t" << BooleanToString(GetTraceSelectedAttributes()) << "\n";
}


const ALString KRSelectionParameters::GetClassLabel() const
{
    return "Selection parameters";
}


//## Method implementation

const ALString KRSelectionParameters::GetObjectLabel() const
{
    ALString sLabel;

    return sLabel;
}

//##
