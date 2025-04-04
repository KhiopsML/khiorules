// Copyright (c) 2023-2025 Orange. All rights reserved.
// This software is distributed under the BSD 3-Clause-clear License, the text of which is available
// at https://spdx.org/licenses/BSD-3-Clause-Clear.html or see the "LICENSE" file for more details.

////////////////////////////////////////////////////////////
// Fri Apr 22 09:28:19 2011
// File generated  with GenereTable
// Insert your specific code inside "//## " sections

#include "KRModelingSpecView.h"

KRModelingSpecView::KRModelingSpecView()
{
	SetIdentifier("KRModelingSpec");
	SetLabel("Modeling parameters");
	AddBooleanField("RuleBasedPredictor", "Rule Based predictor", true);
	AddBooleanField("SelectiveNaiveBayesPredictor", "Selective Naive Bayes predictor", false);
	AddBooleanField("NaiveBayesPredictor", "Naive Bayes predictor", false);

	// Parametrage des styles;
	GetFieldAt("RuleBasedPredictor")->SetStyle("CheckBox");
	GetFieldAt("SelectiveNaiveBayesPredictor")->SetStyle("CheckBox");
	GetFieldAt("NaiveBayesPredictor")->SetStyle("CheckBox");

	//## Custom constructor

	// Deplacement des nouveaux champs vers le haut, avant ceux de la classe ancetre
	ALString sFirstFieldId = GetFieldAtIndex(0)->GetIdentifier();
	MoveFieldBefore("RuleBasedPredictor", sFirstFieldId);
	MoveFieldBefore("SelectiveNaiveBayesPredictor", sFirstFieldId);
	MoveFieldBefore("NaiveBayesPredictor", sFirstFieldId);

	// Declaration des actions
	AddAction("InspectSelectiveNaiveBayesParameters", "Selective Naive Bayes expert parameters",
		  (ActionMethod)(&KRModelingSpecView::InspectSelectiveNaiveBayesParameters));
	GetActionAt("InspectSelectiveNaiveBayesParameters")->SetStyle("Button");

	AddAction("InspectRuleBasedPredictorParameters", "Rule-based predictor expert parameters",
		  (ActionMethod)(&KRModelingSpecView::InspectRuleBasedPredictorParameters));
	GetActionAt("InspectRuleBasedPredictorParameters")->SetStyle("Button");

	//##
}

KRModelingSpecView::~KRModelingSpecView()
{

	//## Custom destructor

	//##
}

void KRModelingSpecView::EventUpdate(Object* object)
{
	KRModelingSpec* editedObject;

	require(object != NULL);

	KWModelingSpecView::EventUpdate(object);
	editedObject = cast(KRModelingSpec*, object);
	editedObject->SetRuleBasedPredictor(GetBooleanValueAt("RuleBasedPredictor"));
	editedObject->SetSelectiveNaiveBayesPredictor(GetBooleanValueAt("SelectiveNaiveBayesPredictor"));
	editedObject->SetNaiveBayesPredictor(GetBooleanValueAt("NaiveBayesPredictor"));

	//## Custom update

	//##
}

void KRModelingSpecView::EventRefresh(Object* object)
{
	KRModelingSpec* editedObject;

	require(object != NULL);

	KWModelingSpecView::EventRefresh(object);
	editedObject = cast(KRModelingSpec*, object);
	SetBooleanValueAt("RuleBasedPredictor", editedObject->GetRuleBasedPredictor());
	SetBooleanValueAt("SelectiveNaiveBayesPredictor", editedObject->GetSelectiveNaiveBayesPredictor());
	SetBooleanValueAt("NaiveBayesPredictor", editedObject->GetNaiveBayesPredictor());

	//## Custom refresh

	//##
}

const ALString KRModelingSpecView::GetClassLabel() const
{
	return "Modeling parameters";
}

//## Method implementation

void KRModelingSpecView::InspectSelectiveNaiveBayesParameters()
{
	SNBPredictorSelectiveNaiveBayesView predictorSelectiveNaiveBayesView;
	KWModelingSpec* modelingSpec;

	// Acces a l'objet edite
	modelingSpec = cast(KWModelingSpec*, GetObject());
	check(modelingSpec);

	// Ouverture de la sous-fiche
	predictorSelectiveNaiveBayesView.SetObject(modelingSpec->GetPredictorSelectiveNaiveBayes());
	predictorSelectiveNaiveBayesView.Open();
}

void KRModelingSpecView::InspectRuleBasedPredictorParameters()
{
	KRPredictorRuleView predictorRuleView;
	KRModelingSpec* modelingSpec;

	// Acces a l'objet edite
	modelingSpec = cast(KRModelingSpec*, GetObject());
	check(modelingSpec);

	// Ouverture de la sous-fiche
	predictorRuleView.SetObject(modelingSpec->GetPredictorRule());
	predictorRuleView.Open();
}

//##
