// Copyright (c) 2023-2025 Orange. All rights reserved.
// This software is distributed under the BSD 3-Clause-clear License, the text of which is available
// at https://spdx.org/licenses/BSD-3-Clause-Clear.html or see the "LICENSE" file for more details.

#include "KRLearningProblemView.h"

KRLearningProblemView::KRLearningProblemView()
{
	KRAnalysisSpecView* analysisSpecView;
	const ALString sAnalysisSpecIdentifier="AnalysisSpec";

	// Specialisation de la fiche des parametres d'analyse,
	// en remplacant l'ancienne version par une sous-classe
    analysisSpecView = new KRAnalysisSpecView;
	ReplaceCardField(sAnalysisSpecIdentifier, analysisSpecView);

	// Parametrage de liste d'aide pour le nom de l'attribut cible
	analysisSpecView->GetFieldAt("TargetAttributeName")->SetStyle("HelpedComboBox");
	analysisSpecView->GetFieldAt("TargetAttributeName")->SetParameters(
		"Attributes:Name");

	// Parametrage de liste d'aide pour les modalites de l'attribut cible
	analysisSpecView->GetFieldAt("MainTargetModality")->SetStyle("HelpedComboBox");
	analysisSpecView->GetFieldAt("MainTargetModality")->SetParameters(
		"Modalities:Value");

	// Fonctionnalites avancees, disponible uniquement pour l'auteur
    //if (GetLearningExpertMode())
    {
		AddCardField("LearningProblemStudy", "Benchmark",
			new KRLearningProblemExtendedActionView);
    }

	// Libelles
    SetIdentifier("KRLearningProblem");
    SetLabel("Rule-Based Classifier");
}


KRLearningProblemView::~KRLearningProblemView()
{
}


void KRLearningProblemView::ClassifierBenchmark()
{
    KWLearningBenchmark* classifierBenchmark;
    KWLearningBenchmarkView view;

    // Acces au parametrage du benchmark
    classifierBenchmark = GetMyLearningProblem()->GetClassifierBenchmark();
	assert(classifierBenchmark->GetTargetAttributeType() == KWType::Symbol);

    // Ouverture de la fenetre
    view.SetObject(classifierBenchmark);
    view.Open();
}


void KRLearningProblemView::RegressorBenchmark()
{
    KWLearningBenchmark* regressorBenchmark;
    KWLearningBenchmarkView view;

    // Acces au parametrage du benchmark
    regressorBenchmark = GetMyLearningProblem()->GetRegressorBenchmark();
	assert(regressorBenchmark->GetTargetAttributeType() == KWType::Continuous);

    // Ouverture de la fenetre
    view.SetObject(regressorBenchmark);
    view.Open();
}


void KRLearningProblemView::SetObject(Object* object)
{
    KRLearningProblem* learningProblem;

	require(object != NULL);

	// Appel de la methode ancetre
	KWLearningProblemView::SetObject(object);

	// Acces a l'objet edite
	learningProblem = cast(KRLearningProblem*, object);

    // Fonctionnalites avancees, disponible uniquement pour l'auteur
    //if (GetLearningExpertMode())
    {
		cast(KRLearningProblemExtendedActionView*, GetFieldAt("LearningProblemStudy"))->
			SetObject(learningProblem);
    }
}


KRLearningProblem* KRLearningProblemView::GetMyLearningProblem()
{
    return cast(KRLearningProblem*, objValue);
}


/////////////////////////////////////////////////////////////////////////


KRLearningProblemExtendedActionView::KRLearningProblemExtendedActionView()
{
	// Libelles
    SetIdentifier("KRLearningExtendedProblemAction");
    SetLabel("Study");

	// Benchmarks
	AddAction("ClassifierBenchmark", "Evaluate classifiers...",
		(ActionMethod)(&KRLearningProblemExtendedActionView::ClassifierBenchmark));
	AddAction("RegressorBenchmark", "Evaluate regressors...",
		(ActionMethod)(&KRLearningProblemExtendedActionView::RegressorBenchmark));
}


KRLearningProblemExtendedActionView::~KRLearningProblemExtendedActionView()
{
}


void KRLearningProblemExtendedActionView::EventUpdate(Object* object)
{
    KRLearningProblem* editedObject;

    require(object != NULL);

    editedObject = cast(KRLearningProblem*, object);
}


void KRLearningProblemExtendedActionView::EventRefresh(Object* object)
{
    KRLearningProblem* editedObject;

    require(object != NULL);

    editedObject = cast(KRLearningProblem*, object);
}


void KRLearningProblemExtendedActionView::ClassifierBenchmark()
{
	GetMyLearningProblemView()->ClassifierBenchmark();
}


void KRLearningProblemExtendedActionView::RegressorBenchmark()
{
	GetMyLearningProblemView()->RegressorBenchmark();
}


KRLearningProblem* KRLearningProblemExtendedActionView::GetMyLearningProblem()
{
    require(objValue != NULL);

    return cast(KRLearningProblem*, objValue);
}


KRLearningProblemView* KRLearningProblemExtendedActionView::GetMyLearningProblemView()
{
    require(GetParent() != NULL);

    return cast(KRLearningProblemView*, GetParent());
}


////////////////////////////////////////////////////////////
// Classe KRAnalysisSpecView

KRAnalysisSpecView::KRAnalysisSpecView()
{

	KRModelingSpecView* modelingSpecView;
	const ALString sModelingSpecIdentifier="ModelingSpec";

	// Specialisation de la fiche des parametres de modelisation,
	// en remplacant l'ancienne version par une sous-classe
    modelingSpecView = new KRModelingSpecView;
	ReplaceCardField(sModelingSpecIdentifier, modelingSpecView);
}


KRAnalysisSpecView::~KRAnalysisSpecView()
{
}
