// Copyright (c) 2023-2025 Orange. All rights reserved.
// This software is distributed under the BSD 3-Clause-clear License, the text of which is available
// at https://spdx.org/licenses/BSD-3-Clause-Clear.html or see the "LICENSE" file for more details.

#ifndef KRLEARNINGPROJECT_H
#define KRLEARNINGPROJECT_H

#include "KWLearningProject.h"
#include "KRLearningProblem.h"
#include "KRLearningProblemView.h"
#include "KRPredictorRule.h"
#include "KRPredictorRuleView.h"

// Service de lancement du projet Khiops
class KRLearningProject : public KWLearningProject
{
public:
	// Constructeur
	KRLearningProject();
	~KRLearningProject();

	///////////////////////////////////////////////////////////////////////
	///// Implementation
protected:
	// Reimplementation des methodes virtuelles
	void OpenLearningEnvironnement();
	KWLearningProblem* CreateLearningProblem();
	KWLearningProblemView* CreateLearningProblemView();
};


#endif // KRLEARNINGPROJECT_H
