// Copyright (c) 2023-2025 Orange. All rights reserved.
// This software is distributed under the BSD 3-Clause-clear License, the text of which is available
// at https://spdx.org/licenses/BSD-3-Clause-Clear.html or see the "LICENSE" file for more details.

#include "KRPredictorRuleView.h"


KRPredictorRuleView::KRPredictorRuleView()
{
	KRPredictorRule predictor;

	// Nom de la vue (le meme que celui de l'objet edite)
	sName = predictor.GetName();

	// Parametrage principal de l'interface
	SetIdentifier("PredictorRule");
	SetLabel("Rule-based Predictor");

    // Ajout des sous-fiches
    AddCardField("RuleParameters", "Rule parameters", new KRRuleParametersView);
}


KRPredictorRuleView::~KRPredictorRuleView()
{
}


KWPredictorView* KRPredictorRuleView::Create() const
{
	return new KRPredictorRuleView;
}

const ALString KRPredictorRuleView::GetName() const
{
    return "Rule-Based";
}

void KRPredictorRuleView::EventUpdate(Object* object)
{
    KRPredictorRule* editedObject;

    require(object != NULL);

    editedObject = cast(KRPredictorRule*, object);
}


void KRPredictorRuleView::EventRefresh(Object* object)
{
    KRPredictorRule* editedObject;

    require(object != NULL);

    editedObject = cast(KRPredictorRule*, object);
}


void KRPredictorRuleView::SetObject(Object* object)
{
    KRPredictorRule* predictor;

	require(object != NULL);

	// Acces a l'objet edite
	predictor = cast(KRPredictorRule*, object);

	// Appel de la methode ancetre
	KWPredictorView::SetObject(object);

	// Parametrage des sous-fiches
	 cast(KRRuleParametersView*, GetFieldAt("RuleParameters"))->
		SetObject(predictor->GetRuleParameters());
}


KRPredictorRule* KRPredictorRuleView::GetPredictorRule()
{
    return cast(KRPredictorRule*, objValue);
}
