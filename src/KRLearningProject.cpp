// Copyright (c) 2023-2025 Orange. All rights reserved.
// This software is distributed under the BSD 3-Clause-clear License, the text of which is available
// at https://spdx.org/licenses/BSD-3-Clause-Clear.html or see the "LICENSE" file for more details.

#include "KRLearningProject.h"

KRLearningProject::KRLearningProject() {}

KRLearningProject::~KRLearningProject() {}

void KRLearningProject::OpenLearningEnvironnement() {
  // Appel de la methode ancetre
  KWLearningProject::OpenLearningEnvironnement();

  // Parametrage du nom du module applicatif
  SetLearningModuleName("KhioRules");

  // Enregistrement du predicteur
  KWPredictor::RegisterPredictor(new KRPredictorRule);

  // Enregistrement d'une vue sur le predicteur
  KWPredictorView::RegisterPredictorView(new KRPredictorRuleView);

  // Enregistrement de la regle de derivation pour PredictorRule
  KWDerivationRule::RegisterDerivationRule(new KRDRPredictorRule);

  // Enregistrement des regles specifiques eventuelles
}

KWLearningProblem *KRLearningProject::CreateLearningProblem() {
  return new KRLearningProblem;
}

KWLearningProblemView *KRLearningProject::CreateLearningProblemView() {
  return new KRLearningProblemView;
}
