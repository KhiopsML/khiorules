// Copyright (c) 2023-2025 Orange. All rights reserved.
// This software is distributed under the BSD 3-Clause-clear License, the text of which is available
// at https://spdx.org/licenses/BSD-3-Clause-Clear.html or see the "LICENSE" file for more details.

////////////////////////////////////////////////////////////
// Fri Apr 22 09:28:19 2011
// File generated  with GenereTable
// Insert your specific code inside "//## " sections

#ifndef KRMODELINGSPECVIEW_H
#define KRMODELINGSPECVIEW_H

#include "UserInterface.h"

#include "KRModelingSpec.h"
#include "KWModelingSpecView.h"

//## Custom includes

#include "KRPredictorRuleView.h"
#include "SNBPredictorSelectiveNaiveBayesView.h"

//##

////////////////////////////////////////////////////////////
// Classe KRModelingSpecView
//    Modeling parameters
// Editeur de KRModelingSpec
class KRModelingSpecView : public KWModelingSpecView
{
public:
	// Constructeur
	KRModelingSpecView();
	~KRModelingSpecView();

	////////////////////////////////////////////////////////
	// Redefinition des methodes a reimplementer obligatoirement

	// Mise a jour de l'objet par les valeurs de l'interface
	void EventUpdate(Object* object);

	// Mise a jour des valeurs de l'interface par l'objet
	void EventRefresh(Object* object);

	// Libelles utilisateur
	const ALString GetClassLabel() const;

	//## Custom declarations

	// Inspection des parametres avances du predicteur Bayesien Naif Selectif
	void InspectSelectiveNaiveBayesParameters();

	// Inspection des parametres avances du predicteur a base de regles
	void InspectRuleBasedPredictorParameters();

	//##
	////////////////////////////////////////////////////////
	//// Implementation
protected:
	//## Custom implementation

	//##
};

//## Custom inlines

//##

#endif // KRMODELINGSPECVIEW_H
