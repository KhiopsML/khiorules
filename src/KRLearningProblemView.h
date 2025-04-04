// Copyright (c) 2023-2025 Orange. All rights reserved.
// This software is distributed under the BSD 3-Clause-clear License, the text of which is available
// at https://spdx.org/licenses/BSD-3-Clause-Clear.html or see the "LICENSE" file for more details.

#ifndef KRLEARNINGPROBLEMVIEW_H
#define KRLEARNINGPROBLEMVIEW_H

#include "KRLearningProblem.h"
#include "KWLearningProblemView.h"
#include "KWLearningBenchmarkView.h"
#include "KWAnalysisSpecView.h"
#include "KRModelingSpecView.h"


////////////////////////////////////////////////////////////
// Classe KRLearningProblemView
//    Khiops: preparation des donnees
// Editeur de KRLearningProblemView
class KRLearningProblemView : public KWLearningProblemView
{
 public:
    // Constructeur
    KRLearningProblemView();
    ~KRLearningProblemView();

	// Actions etendues
    void ClassifierBenchmark();
    void RegressorBenchmark();

	// Acces au probleme d'apprentissage
    void SetObject(Object* object);
    KRLearningProblem* GetMyLearningProblem();
};


////////////////////////////////////////////////////////////
// Classe KRLearningProblemExtendedActionView
//    Actions d'analyse etendues deportees de KRLearningProblemView
//    Fiche ne contenant que des actions (pour un menu supplementaire)
//     sans maquettage de champ a l'interface
class KRLearningProblemExtendedActionView : public UIObjectView
{
 public:

    // Constructeur
    KRLearningProblemExtendedActionView();
    ~KRLearningProblemExtendedActionView();


    ////////////////////////////////////////////////////////
    // Redefinition des methodes a reimplementer obligatoirement

    // Mise a jour de l'objet par les valeurs de l'interface
    void EventUpdate(Object* object);

    // Mise a jour des valeurs de l'interface par l'objet
    void EventRefresh(Object* object);

    // Actions de menu
    void ClassifierBenchmark();
    void RegressorBenchmark();

    // Acces au probleme d'apprentissage
    KRLearningProblem* GetMyLearningProblem();

	// Acces a la vue principale sur le probleme d'apprentissage
    KRLearningProblemView* GetMyLearningProblemView();
};


////////////////////////////////////////////////////////////
// Classe KRAnalysisSpecView
//    Version specialisee des Analysis parameters permettant
//     de preciser les parametre de modelisation
// Editeur de KRAnalysisSpec
class KRAnalysisSpecView : public KWAnalysisSpecView
{
 public:

    // Constructeur
    KRAnalysisSpecView();
    ~KRAnalysisSpecView();
};


#endif // KRLEARNINGPROBLEMVIEW_H
