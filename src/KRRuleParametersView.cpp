// Copyright (c) 2023-2025 Orange. All rights reserved.
// This software is distributed under the BSD 3-Clause-clear License, the text of which is available
// at https://spdx.org/licenses/BSD-3-Clause-Clear.html or see the "LICENSE" file for more details.

////////////////////////////////////////////////////////////
// Mon May 23 11:49:33 2011
// File generated  with GenereTable
// Insert your specific code inside "//## " sections

#include "KRRuleParametersView.h"

KRRuleParametersView::KRRuleParametersView()
{
	SetIdentifier("KRRuleParameters");
	SetLabel("Rule parameters");
	AddIntField("TimeAllowed", "Time allowed for training", 600);
	AddIntField("MaxRuleNumber", "Maximum number of rules to mine", 10);
	AddBooleanField("AddingFeatures", "Adding rules as new features", true);
	AddIntField("PassesOverDatabase", "Number of passes over database for small data", 0);
	AddIntField("RuleOptimizationLoops", "Number of optimization loops over the attributes of the rule", 0);
	AddIntField("ContinuousOptimizationLoops", "Number of optimization loops over a continuous attribute", 0);
	AddIntField("SymbolOptimizationLoops", "Number of optimization loops over a categorical attribute", 0);
	AddStringField("SubPredictor", "End predictor", "");

	// Parametrage des styles;
	GetFieldAt("TimeAllowed")->SetStyle("Spinner");
	GetFieldAt("MaxRuleNumber")->SetStyle("Spinner");
	GetFieldAt("AddingFeatures")->SetStyle("CheckBox");
	GetFieldAt("PassesOverDatabase")->SetStyle("Spinner");
	GetFieldAt("RuleOptimizationLoops")->SetStyle("Spinner");
	GetFieldAt("ContinuousOptimizationLoops")->SetStyle("Spinner");
	GetFieldAt("SymbolOptimizationLoops")->SetStyle("Spinner");
	GetFieldAt("SubPredictor")->SetStyle("ComboBox");

	//## Custom constructor

	// Parametrage avance des styles
	cast(UIIntElement*, GetFieldAt("TimeAllowed"))->SetMinValue(0);
	GetFieldAt("SubPredictor")->SetParameters("SNB\nNB\nBU");

	// Info-bulles
	GetFieldAt("MaxRuleNumber")->SetHelpText("Max number of rules to build\n"
						 "New line");

	// Inactivation des parametres non utilises
	/*DDD MB
	GetFieldAt("TimeAllowed")->SetVisible(false);
	GetFieldAt("PassesOverDatabase")->SetVisible(false);
	GetFieldAt("RuleOptimizationLoops")->SetVisible(false);
	GetFieldAt("ContinuousOptimizationLoops")->SetVisible(false);
	GetFieldAt("SymbolOptimizationLoops")->SetVisible(false);
    */

	//##
}

KRRuleParametersView::~KRRuleParametersView()
{

	//## Custom destructor

	//##
}

void KRRuleParametersView::EventUpdate(Object* object)
{
	KRRuleParameters* editedObject;

	require(object != NULL);

	editedObject = cast(KRRuleParameters*, object);
	editedObject->SetTimeAllowed(GetIntValueAt("TimeAllowed"));
	editedObject->SetMaxRuleNumber(GetIntValueAt("MaxRuleNumber"));
	editedObject->SetAddingFeatures(GetBooleanValueAt("AddingFeatures"));
	editedObject->SetPassesOverDatabase(GetIntValueAt("PassesOverDatabase"));
	editedObject->SetRuleOptimizationLoops(GetIntValueAt("RuleOptimizationLoops"));
	editedObject->SetContinuousOptimizationLoops(GetIntValueAt("ContinuousOptimizationLoops"));
	editedObject->SetSymbolOptimizationLoops(GetIntValueAt("SymbolOptimizationLoops"));
	editedObject->SetSubPredictor(GetStringValueAt("SubPredictor"));

	//## Custom update

	//##
}

void KRRuleParametersView::EventRefresh(Object* object)
{
	KRRuleParameters* editedObject;

	require(object != NULL);

	editedObject = cast(KRRuleParameters*, object);
	SetIntValueAt("TimeAllowed", editedObject->GetTimeAllowed());
	SetIntValueAt("MaxRuleNumber", editedObject->GetMaxRuleNumber());
	SetBooleanValueAt("AddingFeatures", editedObject->GetAddingFeatures());
	SetIntValueAt("PassesOverDatabase", editedObject->GetPassesOverDatabase());
	SetIntValueAt("RuleOptimizationLoops", editedObject->GetRuleOptimizationLoops());
	SetIntValueAt("ContinuousOptimizationLoops", editedObject->GetContinuousOptimizationLoops());
	SetIntValueAt("SymbolOptimizationLoops", editedObject->GetSymbolOptimizationLoops());
	SetStringValueAt("SubPredictor", editedObject->GetSubPredictor());

	//## Custom refresh

	//##
}

const ALString KRRuleParametersView::GetClassLabel() const
{
	return "Rule parameters";
}

//## Method implementation

//##
