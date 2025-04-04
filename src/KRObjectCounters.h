// Copyright (c) 2023-2025 Orange. All rights reserved.
// This software is distributed under the BSD 3-Clause-clear License, the text of which is available
// at https://spdx.org/licenses/BSD-3-Clause-Clear.html or see the "LICENSE" file for more details.

#ifndef KROBJECTCOUNTERS_H
#define KROBJECTCOUNTERS_H

#include "Object.h"
#include "KWObject.h"


////////////////////////////////////////////////////////////////////
// Classe KRObjectCounters
// Gestionnaire d'un compteur par KWObject d'une base
// Permet dans le cas de l'optimisation des regles de gerer un compteur
// par nombre de participatiuon au corps d'une regle
class KRObjectCounters : public Object
{
public:
	// Constructeur
	KRObjectCounters();
	~KRObjectCounters();

	// Initialisation a partir d'une tableau de KWObjects
	// Tous les KWObjects sont declare, avec leur compteur a 0
	void InitializeObjects(ObjectArray* oaObjects);

	// Nettoyage, supression des KWObject
	void CleanObjects();

	// Remise a zero de tous les compteurs
	void ResetCounters();

	// Acces au compteur d'un objet
	void SetBodyCountAt(const KWObject* kwoObject, int nCount);
	int GetBodyCountAt(const KWObject* kwoObject) const;

	// Incrementation/decrementation d'un compteur d'objet
	void IncrementBodyCountAt(const KWObject* kwoObject);
	void DecrementBodyCountAt(const KWObject* kwoObject);

	// Affichage: Nombre d'objet pour chaque valeur de compte
	void Write(ostream& ost) const;

	///////////////////////////////////////////////////////
	///// Implementation
protected:
	// Dictionnaire des compteurs
	//  cle: un KWObject (cle numerique)
	//  objet accede: IntObject pour memoriser un compte
	NumericKeyDictionary nkdObjectCounters;
};


///// Methode en inline

inline void KRObjectCounters::SetBodyCountAt(const KWObject* kwoObject, int nCount)
{
	IntObject* ioCounter;

	require(kwoObject != NULL);
	require(nkdObjectCounters.Lookup((NUMERIC)kwoObject) != NULL);
	require(nCount >= 0);

	// Acces au compteur et modification
	ioCounter = cast(IntObject*, nkdObjectCounters.Lookup((NUMERIC)kwoObject));
	ioCounter->SetInt(nCount);
}

inline int KRObjectCounters::GetBodyCountAt(const KWObject* kwoObject) const
{
	IntObject* ioCounter;

	require(kwoObject != NULL);
	require(nkdObjectCounters.Lookup((NUMERIC)kwoObject) != NULL);

	// Acces au compteur et modification
	ioCounter = cast(IntObject*, nkdObjectCounters.Lookup((NUMERIC)kwoObject));
	ensure(ioCounter->GetInt() >= 0);
	return ioCounter->GetInt();
}

inline void KRObjectCounters::IncrementBodyCountAt(const KWObject* kwoObject)
{
	IntObject* ioCounter;

	require(kwoObject != NULL);
	require(nkdObjectCounters.Lookup((NUMERIC)kwoObject) != NULL);

	// Acces au compteur et modification
	ioCounter = cast(IntObject*, nkdObjectCounters.Lookup((NUMERIC)kwoObject));
	ensure(ioCounter->GetInt() >= 0);
	ioCounter->SetInt(ioCounter->GetInt() + 1);
}

inline void KRObjectCounters::DecrementBodyCountAt(const KWObject* kwoObject)
{
	IntObject* ioCounter;

	require(kwoObject != NULL);
	require(nkdObjectCounters.Lookup((NUMERIC)kwoObject) != NULL);

	// Acces au compteur et modification
	ioCounter = cast(IntObject*, nkdObjectCounters.Lookup((NUMERIC)kwoObject));
	ensure(ioCounter->GetInt() >= 1);
	ioCounter->SetInt(ioCounter->GetInt() - 1);
}





#endif // KROBJECTCOUNTERS_H
