// Copyright (c) 2023-2025 Orange. All rights reserved.
// This software is distributed under the BSD 3-Clause-clear License, the text of which is available
// at https://spdx.org/licenses/BSD-3-Clause-Clear.html or see the "LICENSE" file for more details.

#ifndef KRPREDICTORRULEVIEW_H
#define KRPREDICTORRULEVIEW_H

#include "UserInterface.h"
#include "KWPredictorView.h"
#include "KRPredictorRule.h"
#include "KWSelectionParametersView.h"
#include "KRTrainParametersView.h"
#include "KRRuleParametersView.h"


////////////////////////////////////////////////////////////////////////////////
// Vue sur le parametrage specifique d'un classifieur a base de regles
class KRPredictorRuleView : public KWPredictorView
{
public:
    // Constructeur
    KRPredictorRuleView();
    ~KRPredictorRuleView();

	// Constructeur generique
    KWPredictorView* Create() const;

	// Nom du predicteur edite
	const ALString GetName() const;


    ////////////////////////////////////////////////////////
    // Methodes a reimplementer dans les sous-classes

    // Mise a jour du classifieur specifique par les valeurs de l'interface
    void EventUpdate(Object* object);

    // Mise a jour des valeurs de l'interface par le predicteur specifique
    void EventRefresh(Object* object);

	// Acces au predicteur
    void SetObject(Object* object);
    KRPredictorRule* GetPredictorRule();

};


#endif // KRPREDICTORRULEVIEW_H
