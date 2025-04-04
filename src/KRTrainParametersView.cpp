// Copyright (c) 2023-2025 Orange. All rights reserved.
// This software is distributed under the BSD 3-Clause-clear License, the text of which is available
// at https://spdx.org/licenses/BSD-3-Clause-Clear.html or see the "LICENSE" file for more details.

////////////////////////////////////////////////////////////
// Mon May 23 11:49:33 2011
// File generated  with GenereTable
// Insert your specific code inside "//## " sections




#include "KRTrainParametersView.h"


KRTrainParametersView::KRTrainParametersView()
{
    SetIdentifier("KRTrainParameters");
    SetLabel("Train parameters");
    AddIntField("TimeAllowed", "Time allowed for training", 0);
    AddIntField("MaxRuleNumber", "Maximum number of rules to mine", 0);
    AddIntField("PassesOverDatabase", "Number of passes over database for small data", 0);

    // Parametrage des styles;
    GetFieldAt("TimeAllowed")->SetStyle("Spinner");
    GetFieldAt("MaxRuleNumber")->SetStyle("Spinner");
    GetFieldAt("PassesOverDatabase")->SetStyle("Spinner");

    //## Custom constructor

    //##
}


KRTrainParametersView::~KRTrainParametersView()
{

    //## Custom destructor

    //##
}


void KRTrainParametersView::EventUpdate(Object* object)
{
    KRTrainParameters* editedObject;

    require(object != NULL);

    editedObject = cast(KRTrainParameters*, object);
    editedObject->SetTimeAllowed(GetIntValueAt("TimeAllowed"));
    editedObject->SetMaxRuleNumber(GetIntValueAt("MaxRuleNumber"));
    editedObject->SetPassesOverDatabase(GetIntValueAt("PassesOverDatabase"));

    //## Custom update

    //##
}


void KRTrainParametersView::EventRefresh(Object* object)
{
    KRTrainParameters* editedObject;

    require(object != NULL);

    editedObject = cast(KRTrainParameters*, object);
    SetIntValueAt("TimeAllowed", editedObject->GetTimeAllowed());
    SetIntValueAt("MaxRuleNumber", editedObject->GetMaxRuleNumber());
    SetIntValueAt("PassesOverDatabase", editedObject->GetPassesOverDatabase());

    //## Custom refresh

    //##
}


const ALString KRTrainParametersView::GetClassLabel() const
{
    return "Train parameters";
}



//## Method implementation

//##
