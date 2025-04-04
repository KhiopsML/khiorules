// Copyright (c) 2023-2025 Orange. All rights reserved.
// This software is distributed under the BSD 3-Clause-clear License, the text of which is available
// at https://spdx.org/licenses/BSD-3-Clause-Clear.html or see the "LICENSE" file for more details.

////////////////////////////////////////////////////////////
// Mon May 23 11:49:33 2011
// File generated  with GenereTable
// Insert your specific code inside "//## " sections




#include "KRSelectionParametersView.h"


KRSelectionParametersView::KRSelectionParametersView()
{
    SetIdentifier("KRSelectionParameters");
    SetLabel("Selection parameters");
    AddIntField("MaxSelectedAttributeNumber", "Max number of selected variables", 0);
    AddStringField("SelectionCriterion", "Selection criterion", "");
    AddStringField("OptimizationAlgorithm", "Optimization algorithm", "");
    AddIntField("OptimizationLevel", "Optimization level", 0);
    AddIntField("TraceLevel", "Trace level", 0);
    AddBooleanField("TraceSelectedAttributes", "Trace selected variables", false);

    // Parametrage des styles;
    GetFieldAt("MaxSelectedAttributeNumber")->SetStyle("Spinner");
    GetFieldAt("SelectionCriterion")->SetStyle("ComboBox");
    GetFieldAt("OptimizationAlgorithm")->SetStyle("ComboBox");
    GetFieldAt("OptimizationLevel")->SetStyle("Spinner");
    GetFieldAt("TraceLevel")->SetStyle("Spinner");
    GetFieldAt("TraceSelectedAttributes")->SetStyle("CheckBox");

    //## Custom constructor

    //##
}


KRSelectionParametersView::~KRSelectionParametersView()
{

    //## Custom destructor

    //##
}


void KRSelectionParametersView::EventUpdate(Object* object)
{
    KRSelectionParameters* editedObject;

    require(object != NULL);

    editedObject = cast(KRSelectionParameters*, object);
    editedObject->SetMaxSelectedAttributeNumber(GetIntValueAt("MaxSelectedAttributeNumber"));
    editedObject->SetSelectionCriterion(GetStringValueAt("SelectionCriterion"));
    editedObject->SetOptimizationAlgorithm(GetStringValueAt("OptimizationAlgorithm"));
    editedObject->SetOptimizationLevel(GetIntValueAt("OptimizationLevel"));
    editedObject->SetTraceLevel(GetIntValueAt("TraceLevel"));
    editedObject->SetTraceSelectedAttributes(GetBooleanValueAt("TraceSelectedAttributes"));

    //## Custom update

    //##
}


void KRSelectionParametersView::EventRefresh(Object* object)
{
    KRSelectionParameters* editedObject;

    require(object != NULL);

    editedObject = cast(KRSelectionParameters*, object);
    SetIntValueAt("MaxSelectedAttributeNumber", editedObject->GetMaxSelectedAttributeNumber());
    SetStringValueAt("SelectionCriterion", editedObject->GetSelectionCriterion());
    SetStringValueAt("OptimizationAlgorithm", editedObject->GetOptimizationAlgorithm());
    SetIntValueAt("OptimizationLevel", editedObject->GetOptimizationLevel());
    SetIntValueAt("TraceLevel", editedObject->GetTraceLevel());
    SetBooleanValueAt("TraceSelectedAttributes", editedObject->GetTraceSelectedAttributes());

    //## Custom refresh

    //##
}


const ALString KRSelectionParametersView::GetClassLabel() const
{
    return "Selection parameters";
}



//## Method implementation

//##
