// Copyright (c) 2023-2025 Orange. All rights reserved.
// This software is distributed under the BSD 3-Clause-clear License, the text of which is available
// at https://spdx.org/licenses/BSD-3-Clause-Clear.html or see the "LICENSE" file for more details.

#ifndef KRLEARNINGPROBLEM_H
#define KRLEARNINGPROBLEM_H

#include "KWPredictorNaiveBayes.h"
#include "SNBPredictorSelectiveNaiveBayes.h"
#include "KWLearningProblem.h"
#include "KWLearningBenchmark.h"
#include "KRModelingSpec.h"
#include "KRPredictorRule.h"

////////////////////////////////////////////////////////////
// Classe KRLearningProblem
//    Khiops: preparation des donnees et modelisation
// Extension de la classe KWLearningProblem avec les
// familles de predicteurs naive Bayes et data grid,
// ainsi que des actions etendues sur les benchmarks
class KRLearningProblem : public KWLearningProblem
{
public:
	// Constructeur
	KRLearningProblem();
	~KRLearningProblem();

	// Benchmark de classifier
	KWLearningBenchmark* GetClassifierBenchmark();
	KWLearningBenchmark* GetRegressorBenchmark();

	// Recherche des predicteurs a utiliser
	// Redefinition de la methode pour rechercher les predicteurs SNB, NB et DG
	void CollectPredictors(KWClassStats* classStats, ObjectArray* oaPredictors);

	////////////////////////////////////////////////////////
	//// Implementation
protected:
	// Extension des sous-parties du probleme d'apprentissage
	KWLearningBenchmark* classifierBenchmark;
	KWLearningBenchmark* regressorBenchmark;
};

////////////////////////////////////////////////////////////
// Classe KRAnalysisSpec
//    Specialisation des Analysis parameters
//    notamment pour avoir  une version etendue des Modeling specs
//    avec parametrage des predicteurs SNB, NB et DG
class KRAnalysisSpec : public KWAnalysisSpec
{
public:
	// Constructeur
	KRAnalysisSpec();
	~KRAnalysisSpec();
};

#endif // KRLEARNINGPROBLEM_H
