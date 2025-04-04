// Copyright (c) 2023-2025 Orange. All rights reserved.
// This software is distributed under the BSD 3-Clause-clear License, the text of which is available
// at https://spdx.org/licenses/BSD-3-Clause-Clear.html or see the "LICENSE" file for more details.

////////////////////////////////////////////////////////////
// Mon May 23 11:49:33 2011
// File generated  with GenereTable
// Insert your specific code inside "//## " sections


#include "KRTrainParameters.h"


KRTrainParameters::KRTrainParameters()
{
    nTimeAllowed = 0;
    nMaxRuleNumber = 0;
    nPassesOverDatabase = 0;

    //## Custom constructor

    //##
}


KRTrainParameters::~KRTrainParameters()
{

    //## Custom destructor

    //##
}


void KRTrainParameters::CopyFrom(const KRTrainParameters* aSource)
{
    require(aSource != NULL);

    nTimeAllowed = aSource->nTimeAllowed;
    nMaxRuleNumber = aSource->nMaxRuleNumber;
    nPassesOverDatabase = aSource->nPassesOverDatabase;

    //## Custom copyfrom

    //##
}


KRTrainParameters* KRTrainParameters::Clone() const
{
    KRTrainParameters* aClone;

    aClone = new KRTrainParameters;
    aClone->CopyFrom(this);

    //## Custom clone

    //##
    return aClone;
}


void KRTrainParameters::Write(ostream& ost) const
{
    ost << "Time allowed for training\t" << GetTimeAllowed() << "\n";
    ost << "Maximum number of rules to mine\t" << GetMaxRuleNumber() << "\n";
    ost << "Number of passes over database for small data\t" << GetPassesOverDatabase() << "\n";
}


const ALString KRTrainParameters::GetClassLabel() const
{
    return "Train parameters";
}


//## Method implementation

const ALString KRTrainParameters::GetObjectLabel() const
{
    ALString sLabel;

    return sLabel;
}

//##
