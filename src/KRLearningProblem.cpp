// Copyright (c) 2023-2025 Orange. All rights reserved.
// This software is distributed under the BSD 3-Clause-clear License, the text of which is available
// at https://spdx.org/licenses/BSD-3-Clause-Clear.html or see the "LICENSE" file for more details.

#include "KRLearningProblem.h"

////////////////////////////////////////////////////////////
// Classe KRLearningProblem

KRLearningProblem::KRLearningProblem()
{
	// Specialisation des specifications d'analyse,
	// en detruisant le sous-objet cree dans la classe ancetre et en le remplacant par une version dediee
	delete analysisSpec;
	analysisSpec = new KRAnalysisSpec;

	// Creation explicite des sous-objets supplementaire,
	// ce qui permet de creer des sous-objets specifiques dans des sous-classes
	classifierBenchmark = new KWLearningBenchmark;
	regressorBenchmark = new KWLearningBenchmark;

	// Specilisation du parametrage des benchmarks, principalement pour les classifieurs
	classifierBenchmark->SetTargetAttributeType(KWType::Symbol);
	regressorBenchmark->SetTargetAttributeType(KWType::Continuous);

	// Specialisation des noms de fichier resultats en ajoutant un prefixe KR
	analysisResults->SetReportFileName("KR_" + analysisResults->GetReportFileName());
}

KRLearningProblem::~KRLearningProblem()
{
	delete classifierBenchmark;
	delete regressorBenchmark;
}

KWLearningBenchmark* KRLearningProblem::GetClassifierBenchmark()
{
	// On retourne alors l'objet demande
	return classifierBenchmark;
}

KWLearningBenchmark* KRLearningProblem::GetRegressorBenchmark()
{
	// On retourne alors l'objet demande
	return regressorBenchmark;
}

void KRLearningProblem::CollectPredictors(KWClassStats* classStats, ObjectArray* oaPredictors)
{
	KRPredictorRule* predictorRule;
	SNBPredictorSelectiveNaiveBayes* predictorSelectiveNaiveBayes;
	KWPredictorNaiveBayes* predictorNaiveBayes;
	KRModelingSpec* krModelingSpec;

	require(classStats != NULL);
	require(classStats->IsStatsComputed());
	require(oaPredictors != NULL);
	require(classStats != NULL);
	require(classStats->IsStatsComputed());

	// Acces a la version specialisee des specification de modelisation
	krModelingSpec = cast(KRModelingSpec*, analysisSpec->GetModelingSpec());

	// Predicteur Bayesien Naif Selectif
	if (krModelingSpec->GetSelectiveNaiveBayesPredictor())
	{
		predictorSelectiveNaiveBayes = cast(SNBPredictorSelectiveNaiveBayes*,
			KWPredictor::ClonePredictor("Selective Naive Bayes", classStats->GetTargetAttributeType()));
		if (predictorSelectiveNaiveBayes != NULL)
		{
			predictorSelectiveNaiveBayes->CopyFrom(krModelingSpec->GetPredictorSelectiveNaiveBayes());
			oaPredictors->Add(predictorSelectiveNaiveBayes);
		}
		else
			AddWarning("Selective naive Bayes " + KWType::GetPredictorLabel(classStats->GetTargetAttributeType()) +
				" is not available");
	}

	// Predicteur Bayesien Naif
	if (krModelingSpec->GetNaiveBayesPredictor())
	{
		predictorNaiveBayes = cast(KWPredictorNaiveBayes*,
			KWPredictor::ClonePredictor("Naive Bayes", classStats->GetTargetAttributeType()));
		if (predictorNaiveBayes != NULL)
			oaPredictors->Add(predictorNaiveBayes);
		else
			AddWarning("Naive Bayes " + KWType::GetPredictorLabel(classStats->GetTargetAttributeType()) +
				" is not available");
	}

	// Predicteur a base de regles
	// On ajoute ce predicteur en dernier, car il fonctionne de façon atypique
	// Les predicteurs SNB et NB se base sur un pretraitement preliminaire des variables descriptives, qu'elles exploitent
	// ensuite en modelisation
	// Le predicteur a base de regle cree de nouvelle variables lors de sa phase de modelisation, puis les pretraite,
	// et enfin apprend un predicteur SNB exploitant ces nouvelles variables pretraitees.
	// Dans son etat actuel, le code existant "perturbe" la coherence entre variable pretraitees et predicteur, ce qui
	// impose que ce predicteur doit etre appris en dernier pour ne pas empecher l'apprentissage des autres predicteurs
	if (krModelingSpec->GetRuleBasedPredictor())
	{
		predictorRule = cast(KRPredictorRule*,
			KWPredictor::ClonePredictor("Rule-Based", classStats->GetTargetAttributeType()));
		if (predictorRule != NULL)
		{
			predictorRule->CopyFrom(krModelingSpec->GetPredictorRule());
			oaPredictors->Add(predictorRule);
		}
		else
			AddWarning("Rule-based " + KWType::GetPredictorLabel(classStats->GetTargetAttributeType()) +
				" is not available");
	}
}

////////////////////////////////////////////////////////////
// Classe KRAnalysisSpec

KRAnalysisSpec::KRAnalysisSpec()
{
	// Specialisation des specifications dde modelisation
	// en detruisant le sous-objet cree dans la classe ancetre et en le remplacant par une version dediee
	delete modelingSpec;
	modelingSpec = new KRModelingSpec;
}

KRAnalysisSpec::~KRAnalysisSpec()
{
}
