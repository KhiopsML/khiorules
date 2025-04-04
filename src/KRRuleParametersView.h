// Copyright (c) 2023-2025 Orange. All rights reserved.
// This software is distributed under the BSD 3-Clause-clear License, the text of which is available
// at https://spdx.org/licenses/BSD-3-Clause-Clear.html or see the "LICENSE" file for more details.

////////////////////////////////////////////////////////////
// Mon May 23 11:49:33 2011
// File generated  with GenereTable
// Insert your specific code inside "//## " sections


#ifndef KRRULEPARAMETERSVIEW_H
#define KRRULEPARAMETERSVIEW_H

#include "UserInterface.h"

#include "KRRuleParameters.h"

//## Custom includes

//##


////////////////////////////////////////////////////////////
// Classe KRRuleParametersView
//    Rule parameters
// Editeur de KRRuleParameters
class KRRuleParametersView : public UIObjectView
{
 public:

    // Constructeur
    KRRuleParametersView();
    ~KRRuleParametersView();


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


#endif  // KRRULEPARAMETERSVIEW_H
