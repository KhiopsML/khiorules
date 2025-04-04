// Copyright (c) 2023-2025 Orange. All rights reserved.
// This software is distributed under the BSD 3-Clause-clear License, the text of which is available
// at https://spdx.org/licenses/BSD-3-Clause-Clear.html or see the "LICENSE" file for more details.

#include "KRObjectCounters.h"


KRObjectCounters::KRObjectCounters()
{
}


KRObjectCounters::~KRObjectCounters()
{
	// Destruction des compteurs
	nkdObjectCounters.DeleteAll();
}


void KRObjectCounters::InitializeObjects(ObjectArray* oaObjects)
{
	int nObject;
	KWObject* kwoObject;
	IntObject* ioCounter;

	require(oaObjects != NULL);

	// Nettoyage prealable
	CleanObjects();

	// Parcours des objets pour cree les compteurs
	for (nObject = 0; nObject < oaObjects->GetSize(); nObject++)
	{
		kwoObject = cast(KWObject*, oaObjects->GetAt(nObject));
		check(kwoObject);

		// Creation et memorisation d'un compteur
		ioCounter = new IntObject;
		assert(nkdObjectCounters.Lookup((NUMERIC)kwoObject) == NULL);
		nkdObjectCounters.SetAt((NUMERIC)kwoObject, ioCounter);
	}
	ensure(nkdObjectCounters.GetCount() == oaObjects->GetSize());
}


void KRObjectCounters::CleanObjects()
{
	nkdObjectCounters.DeleteAll();
	//nkdObjectCounters.RemoveAll();
}


void KRObjectCounters::ResetCounters()
{
	POSITION position;
	NUMERIC rKey;
	Object* oElement;
	IntObject* ioCounter;
	int i;//DDDDDD
	//require(nkdObjectCounters.Check());

	// Parcours des objers du dictionnaire  pour remetter tous les compteurs a zero
	i = 0;
	position = nkdObjectCounters.GetStartPosition();
	while (position != NULL)
	{
		nkdObjectCounters.GetNextAssoc(position, rKey, oElement);
		ioCounter = cast(IntObject*, oElement);
		ioCounter->SetInt(0);
		i++;
	}
}


void KRObjectCounters::Write(ostream& ost) const
{
	POSITION position;
	NUMERIC rKey;
	Object* oElement;
	IntObject* ioCounter;
	IntVector ivCounterObjectNumbers;
	int nCount;

	// Parcours des objers du dictionnaire  pour remetter tous les compteurs a zero
	position = nkdObjectCounters.GetStartPosition();
	while (position != NULL)
	{
		nkdObjectCounters.GetNextAssoc(position, rKey, oElement);
		ioCounter = cast(IntObject*, oElement);

		// Mise a jour du nombre d'objets pour ce compte
		nCount = ioCounter->GetInt();
		assert(nCount >= 0);

		// Mise a jour de la taille du vecteur memorisants els nombre d'objets par compte
		if (nCount > ivCounterObjectNumbers.GetSize() - 1)
			ivCounterObjectNumbers.SetSize(nCount + 1);

		// Mise a jour du nombre d'objets pour ce compte
		ivCounterObjectNumbers.UpgradeAt(nCount, 1);
	}

	// Affichage des nombres d'objets par compte
	ost << "Count\tObject number\n";
	for (nCount = 0; nCount < ivCounterObjectNumbers.GetSize(); nCount++)
	{
		if (ivCounterObjectNumbers.GetAt(nCount) > 0)
			ost << nCount << "\t" << ivCounterObjectNumbers.GetAt(nCount) << "\n";
	}
}
