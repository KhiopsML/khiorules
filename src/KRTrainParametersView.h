// Copyright (c) 2023-2025 Orange. All rights reserved.
// This software is distributed under the BSD 3-Clause-clear License, the text of which is available
// at https://spdx.org/licenses/BSD-3-Clause-Clear.html or see the "LICENSE" file for more details.

////////////////////////////////////////////////////////////
// Mon May 23 11:49:33 2011
// File generated  with GenereTable
// Insert your specific code inside "//## " sections


#ifndef KRTRAINPARAMETERSVIEW_H
#define KRTRAINPARAMETERSVIEW_H

#include "UserInterface.h"

#include "KRTrainParameters.h"

//## Custom includes

//##


////////////////////////////////////////////////////////////
// Classe KRTrainParametersView
//    Train parameters
// Editeur de KRTrainParameters
class KRTrainParametersView : public UIObjectView
{
 public:

    // Constructeur
    KRTrainParametersView();
    ~KRTrainParametersView();


    ////////////////////////////////////////////////////////
    // Redefinition des methodes a reimplementer obligatoirement

    // Mise a jour de l'objet par les valeurs de l'interface
    void EventUpdate(Object* object);

    // Mise a jour des valeurs de l'interface par l'objet
    void EventRefresh(Object* object);


    // Libelles utilisateur
    const ALString GetClassLabel() const;

    //## Custom declarations

    //##
    ////////////////////////////////////////////////////////
    //// Implementation
protected:

    //## Custom implementation

    //##
};

//## Custom inlines

//##


#endif  // KRTRAINPARAMETERSVIEW_H
