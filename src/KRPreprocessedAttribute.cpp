// Copyright (c) 2023-2025 Orange. All rights reserved.
// This software is distributed under the BSD 3-Clause-clear License, the text of which is available
// at https://spdx.org/licenses/BSD-3-Clause-Clear.html or see the "LICENSE" file for more details.

#include "KRPreprocessedAttribute.h"

////////////////////////////////////////
// Classe KWDatabaseSorter

KWLoadIndex KWDatabaseSorterAttributeIndex;

// Fonction de comparaison de deux KWObject base sur Continuous
int KWDatabaseSorterCompareOnContinuous(const void* elem1, const void* elem2)
{
	KWObject* kwoObject1;
	KWObject* kwoObject2;

	require(KWDatabaseSorterAttributeIndex.IsValid());

	// Acces aux objets
	kwoObject1 = cast(KWObject*, *(Object**)elem1);
	kwoObject2 = cast(KWObject*, *(Object**)elem2);

	// Comparaison
	return KWContinuous::Compare(
	    kwoObject1->GetContinuousValueAt(KWDatabaseSorterAttributeIndex),
	    kwoObject2->GetContinuousValueAt(KWDatabaseSorterAttributeIndex));
}

// Fonction de comparaison de deux KWObject base sur Symbol (pointeur)
int KWDatabaseSorterCompareOnSymbol(const void* elem1, const void* elem2)
{
	KWObject* kwoObject1;
	KWObject* kwoObject2;

	require(KWDatabaseSorterAttributeIndex.IsValid());

	// Acces aux objets
	kwoObject1 = cast(KWObject*, *(Object**)elem1);
	kwoObject2 = cast(KWObject*, *(Object**)elem2);

	// Comparaison
	return kwoObject1->GetSymbolValueAt(KWDatabaseSorterAttributeIndex).Compare(kwoObject2->GetSymbolValueAt(KWDatabaseSorterAttributeIndex));
}

void KWDatabaseSorter::SortDatabaseObjectsOnContinuous(KWDatabase* database, const ALString& sAttributeName)
{
	KWObject* kwoFirstObject;
	const KWClass* kwcSortClass;
	KWAttribute* attribute;

	require(database->Check());
	require(not KWDatabaseSorterAttributeIndex.IsValid());

	// Recherche de la classe
	if (database->GetObjects()->GetSize() == 0)
		return;
	else
	{
		kwoFirstObject = cast(KWObject*, database->GetObjects()->GetAt(0));
		assert(kwoFirstObject->Check());
		kwcSortClass = kwoFirstObject->GetClass();
	}

	// Recherche de l'attribut et verifications
	attribute = kwcSortClass->LookupAttribute(sAttributeName);
	require(attribute != NULL);
	require(attribute->GetType() == KWType::Continuous);
	require(attribute->GetUsed());
	require(attribute->GetLoaded());

	// Parametrage de la fonction de comparaison
	KWDatabaseSorterAttributeIndex = attribute->GetLoadIndex();

	// Tri
	SortObjects(database, KWDatabaseSorterCompareOnContinuous);

	// Reinitialisation
	KWDatabaseSorterAttributeIndex.Reset();
}

void KWDatabaseSorter::SortDatabaseObjectsOnSymbol(KWDatabase* database, const ALString& sAttributeName)
{
	KWObject* kwoFirstObject;
	const KWClass* kwcSortClass;
	KWAttribute* attribute;

	require(database->Check());
	require(not KWDatabaseSorterAttributeIndex.IsValid());

	// Recherche de la classe
	if (database->GetObjects()->GetSize() == 0)
		return;
	else
	{
		kwoFirstObject = cast(KWObject*, database->GetObjects()->GetAt(0));
		assert(kwoFirstObject->Check());
		kwcSortClass = kwoFirstObject->GetClass();
	}

	// Recherche de l'attribut et verifications
	attribute = kwcSortClass->LookupAttribute(sAttributeName);
	require(attribute != NULL);
	require(attribute->GetType() == KWType::Symbol);
	require(attribute->GetUsed());
	require(attribute->GetLoaded());

	// Parametrage de la fonction de comparaison
	KWDatabaseSorterAttributeIndex = attribute->GetLoadIndex();

	// Tri
	SortObjects(database, KWDatabaseSorterCompareOnSymbol);

	// Reinitialisation
	KWDatabaseSorterAttributeIndex.Reset();
}

void KWDatabaseSorter::SortObjects(KWDatabase* database, CompareFunction fCompare)
{
	SortedList sortedList(fCompare);
	int nObject;
	KWObject* kwoObject;
	POSITION position;

	require(database->Check());

	//////////////////////////////////////////////////////////////////////
	// On utilise une liste triee intermediaire pour effectuer le tri du
	// tableau d'objets.
	// En effet, la methode Sort de ObjectArray est basee sur le QuickSort
	// (qsort) de Visual C++), qui est particulierement inefficace dans
	// le vase de gros volumes de donnees (dans un rapport de 1 a 10 au moins).
	// Un QuickSort efficace recupere sur internet a ete teste. Il devient
	// vraiment competitif, legerement plus rapide que notre tri par liste triee,
	// sans demandee de memoire pour la gestion de la liste (mais avec
	// potentiellement beaucoup de stack pour cause de recursivite du QuickSort).
	// En revanche, il apparait souvent en analyse de donnees que le nombre de
	// modalite effectives soit tres inferieur au nombre d'instances a trier.
	// Dans ce cas, l'avantage revient nettement a tri par liste, qui ne garde
	// qu'une instance de reference par modalite, ce qui diminue de facon
	// importante le nombre de comparaison a effectuer. On a donc, aux constantes
	// pres, un QuickSort en N*log(N) (N: nombre d'instances, et un tri par liste
	// en M*log(M) (M: nombre de modalites).
	// On prefere alors le tri par liste.

	// Ajout des elements dans la liste triee
	// Le tri par la liste triee est beaucoup plus rapide
	// que le tri par le tableau
	for (nObject = 0; nObject < database->GetObjects()->GetSize(); nObject++)
	{
		kwoObject = cast(KWObject*, database->GetObjects()->GetAt(nObject));
		sortedList.Add(kwoObject);
	}

	// On remet les elements tries dans la liste
	position = sortedList.GetHeadPosition();
	nObject = 0;
	while (position != NULL)
	{
		kwoObject = cast(KWObject*,
				 sortedList.GetNext(position));
		database->GetObjects()->SetAt(nObject, kwoObject);
		nObject++;
	}
}

////////////////////////////////////////
// Classe KRPreprocessedAttribute
KRPreprocessedAttribute::KRPreprocessedAttribute()
{
}

KRPreprocessedAttribute::~KRPreprocessedAttribute()
{
}

void KRPreprocessedAttribute::Initialize(KWDatabase* kwdb, KWAttribute* attribute)
{
	sAttributeName = attribute->GetName();
	liLoadedAttributeIndex = attribute->GetLoadIndex();
	liLoadedTargetAttributeIndex = GetClassStats()->GetClass()->LookupAttribute(GetClassStats()->GetTargetAttributeName())->GetLoadIndex();
}

void KRPreprocessedAttribute::SetAttributeName(const ALString& sValue)
{
	sAttributeName = sValue;
}

const ALString& KRPreprocessedAttribute::GetAttributeName() const
{
	return sAttributeName;
}

ObjectArray* KRPreprocessedAttribute::GetObjects()
{
	return &oaPreSortedObjects;
}

void KRPreprocessedAttribute::SetClassStats(KWClassStats* stats)
{
	classStats = stats;
}

KWClassStats* KRPreprocessedAttribute::GetClassStats() const
{
	return classStats;
}

void KRPreprocessedAttribute::PrepareWorkingInstances(KRRAttribute* attribute, KRRule* rule,
						      KRObjectCounters* counters, ObjectArray* oaWorkingInstances) const
{
}

boolean KRPreprocessedAttribute::Check() const
{
	return true;
}

void KRPreprocessedAttribute::Write(ostream& ost) const
{
	ost << "\nAttribute Name : " << sAttributeName << endl;
	ost << "InDB Index : " << liLoadedAttributeIndex << endl;
	ost << "Target Attribute Index : " << liLoadedTargetAttributeIndex << endl;
}

////////////////////////////////////////
// Classe KRPreprocessedContinuousAttribute
KRPreprocessedContinuousAttribute::KRPreprocessedContinuousAttribute()
{
}

KRPreprocessedContinuousAttribute::~KRPreprocessedContinuousAttribute()
{
}

void KRPreprocessedContinuousAttribute::Initialize(KWDatabase* kwdb, KWAttribute* attribute)
{
	// appel de la methode ancetre
	KRPreprocessedAttribute::Initialize(kwdb, attribute);

	KWDatabaseSorter::SortDatabaseObjectsOnContinuous(kwdb, attribute->GetName());
	oaPreSortedObjects.CopyFrom(kwdb->GetObjects());
}

int KRPreprocessedContinuousAttribute::GetAttributeType() const
{
	return KWType::Continuous;
}

void KRPreprocessedContinuousAttribute::PrepareWorkingInstances(KRRAttribute* attribute, KRRule* rule,
								KRObjectCounters* counters, ObjectArray* oaWorkingInstances) const
{
	int nIndex;
	Continuous cValue;
	KWObject* kwoObject;

	require(attribute != NULL);
	require(attribute->GetAttributeType() == KWType::Continuous);
	require(rule != NULL);
	require(attribute->GetAttributeName() == sAttributeName);

	for (nIndex = 0; nIndex < oaPreSortedObjects.GetSize(); nIndex++)
	{
		kwoObject = cast(KWObject*, oaPreSortedObjects.GetAt(nIndex));
		cValue = kwoObject->GetContinuousValueAt(liLoadedAttributeIndex);

		// si objet dans le corps de l'attribut  et compteur == k
		if (attribute->IsInBodyContinuous(cValue) and counters->GetBodyCountAt(kwoObject) == rule->GetAttributeNumber())
			oaWorkingInstances->Add(kwoObject);

		// si objet pas dans le corps de l'attribut  et compteur == k-1
		if ((not attribute->IsInBodyContinuous(cValue)) and counters->GetBodyCountAt(kwoObject) == rule->GetAttributeNumber() - 1)
			oaWorkingInstances->Add(kwoObject);
	}
	//cout << "Working Instances prepared. Size : " << oaWorkingInstances->GetSize() << "\n";
	//cout << *oaWorkingInstances;
}

void KRPreprocessedContinuousAttribute::Write(ostream& ost) const
{
	int nIndex;
	KWObject* kwoCurrent;

	// appel de la mehode ancetre
	KRPreprocessedAttribute::Write(ost);
	ost << "\n";
	for (nIndex = 0; nIndex < oaPreSortedObjects.GetSize(); nIndex++)
	{
		kwoCurrent = cast(KWObject*, oaPreSortedObjects.GetAt(nIndex));
		ost << kwoCurrent->GetContinuousValueAt(liLoadedAttributeIndex) << ", ";
	}
}

////////////////////////////////////////
// Classe KRPreprocessedSymbolAttribute
KRPreprocessedSymbolAttribute::KRPreprocessedSymbolAttribute()
{
	bIsIndexed = false;
}

KRPreprocessedSymbolAttribute::~KRPreprocessedSymbolAttribute()
{
	DeleteIndexingStructure();
	//nkdValues.RemoveAll();
	oaPreprocessedValues.DeleteAll();
}

void KRPreprocessedSymbolAttribute::Initialize(KWDatabase* kwdb, KWAttribute* attribute)
{
	int nIndex;
	KWObject* kwoCurrent;
	Symbol sCurrentValue;
	Symbol sPrecValue;
	int nFrequency;
	int nLastIndex;
	int nFirstIndex;
	KRPreprocessedValue* krpvValue;

	// appel de la methode ancetre
	KRPreprocessedAttribute::Initialize(kwdb, attribute);

	KWDatabaseSorter::SortDatabaseObjectsOnSymbol(kwdb, attribute->GetName());
	oaPreSortedObjects.CopyFrom(kwdb->GetObjects());
	require(oaPreSortedObjects.GetSize() != 0);

	// creation du tableau des preprocessed values
	kwoCurrent = cast(KWObject*, oaPreSortedObjects.GetAt(0));
	sPrecValue = kwoCurrent->GetSymbolValueAt(liLoadedAttributeIndex);
	nFrequency = 0;
	nFirstIndex = 0;
	nLastIndex = 0;
	for (nIndex = 0; nIndex < oaPreSortedObjects.GetSize(); nIndex++)
	{
		kwoCurrent = cast(KWObject*, oaPreSortedObjects.GetAt(nIndex));
		sCurrentValue = kwoCurrent->GetSymbolValueAt(liLoadedAttributeIndex);
		if (sCurrentValue != sPrecValue)
		{
			krpvValue = new KRPreprocessedValue(sPrecValue, nLastIndex - nFirstIndex + 1, nFirstIndex, nLastIndex);
			oaPreprocessedValues.Add(krpvValue);
			nFirstIndex = nLastIndex + 1;
			nLastIndex = nIndex;
			nFrequency = 1;
			sPrecValue = sCurrentValue;
		}
		else
		{
			if (nIndex != 0)
			{
				nLastIndex++;
			} // cas particulier du premier objet
			nFrequency++;
		}

		// pour la derniere valeur
		if (nIndex == oaPreSortedObjects.GetSize() - 1)
		{
			if (sCurrentValue != sPrecValue)
			{
				krpvValue = new KRPreprocessedValue(sCurrentValue, 1, nIndex, nIndex);
				oaPreprocessedValues.Add(krpvValue);
			}
			else
			{
				krpvValue = new KRPreprocessedValue(sCurrentValue, nIndex - nFirstIndex + 1, nFirstIndex, nIndex);
				oaPreprocessedValues.Add(krpvValue);
			}
		}
	}
	// construction de la structure d'indexation
	BuildIndexingStructure();
}

int KRPreprocessedSymbolAttribute::GetAttributeType() const
{
	return KWType::Symbol;
}

void KRPreprocessedSymbolAttribute::BuildIndexingStructure()
{
	int nIndex;
	KRPreprocessedValue* krpvValue;
	Symbol sValue;

	require(GetAttributeType() != KWType::Unknown);
	//require(KWType::IsSimple(GetAttributeType()));
	//require(check());

	//parcours des PreprocessedValues pour les indexer par valeurs
	nkdValues.DeleteAll();
	for (nIndex = 0; nIndex < oaPreprocessedValues.GetSize(); nIndex++)
	{
		krpvValue = cast(KRPreprocessedValue*, oaPreprocessedValues.GetAt(nIndex));
		sValue = krpvValue->GetValue();
		nkdValues.SetAt((NUMERIC)sValue.GetNumericKey(), krpvValue);
	}
	bIsIndexed = true;
}

void KRPreprocessedSymbolAttribute::DeleteIndexingStructure()
{
	require(GetAttributeType() != KWType::Unknown);
	//require(KWType::IsSimple(GetAttributeType()));

	// Suppression de l'indexation si necessaire
	if (bIsIndexed)
		nkdValues.RemoveAll();
	bIsIndexed = false;
}

boolean KRPreprocessedSymbolAttribute::IsIndexed() const
{
	return bIsIndexed;
}

KRPreprocessedValue* KRPreprocessedSymbolAttribute::LookUpPreprocessedValue(Symbol sValue)
{
	KRPreprocessedValue* krpvValue;

	require(IsIndexed());
	require(GetAttributeType() == KWType::Symbol);

	krpvValue = cast(KRPreprocessedValue*, nkdValues.Lookup((NUMERIC)sValue.GetNumericKey()));
	ensure(krpvValue != NULL);
	return krpvValue;
}

void KRPreprocessedSymbolAttribute::PrepareWorkingInstances(KRRAttribute* attribute, KRRule* rule,
							    KRObjectCounters* counters, ObjectArray* oaWorkingInstances) const
{
	int nIndex;
	Symbol sValue;
	KWObject* kwoObject;

	require(attribute != NULL);
	require(attribute->GetAttributeType() == KWType::Symbol);
	require(rule != NULL);
	require(attribute->GetAttributeName() == sAttributeName);

	for (nIndex = 0; nIndex < oaPreSortedObjects.GetSize(); nIndex++)
	{
		kwoObject = cast(KWObject*, oaPreSortedObjects.GetAt(nIndex));
		sValue = kwoObject->GetSymbolValueAt(liLoadedAttributeIndex);

		// si objet dans le corps de l'attribut  et compteur == k
		if (attribute->IsInBodySymbol(sValue) and counters->GetBodyCountAt(kwoObject) == rule->GetAttributeNumber())
			oaWorkingInstances->Add(kwoObject);

		// si objet pas dans le corps de l'attribut  et compteur == k-1
		if ((not attribute->IsInBodySymbol(sValue)) and counters->GetBodyCountAt(kwoObject) == rule->GetAttributeNumber() - 1)
			oaWorkingInstances->Add(kwoObject);
	}
	/*cout << "Working Instances prepared. Size : " << oaWorkingInstances->GetSize() << "\n";
	cout << *oaWorkingInstances;*/
}

void KRPreprocessedSymbolAttribute::Test()
{
	KRPreprocessedValue* krpvValue;
	KRPreprocessedValue* krpv;
	int nIndex;

	nIndex = RandomInt(oaPreprocessedValues.GetSize() - 1);

	krpvValue = cast(KRPreprocessedValue*, oaPreprocessedValues.GetAt(nIndex));
	//BuildIndexingStructure();
	krpv = LookUpPreprocessedValue(krpvValue->GetValue());
	cout << "\n";
	krpv->Write(cout);
	cout << endl;
	//DeleteIndexingStructure();
}

void KRPreprocessedSymbolAttribute::Write(ostream& ost) const
{
	int nIndex;
	//KWObject* kwoCurrent;
	KRPreprocessedValue* krpvValue;

	// appel de la mehode ancetre
	KRPreprocessedAttribute::Write(ost);
	/*ost << "\nPlain:\n";
	for (nIndex = 0; nIndex < oaPreSortedObjects.GetSize(); nIndex++)
	{
		kwoCurrent = cast(KWObject*, oaPreSortedObjects.GetAt(nIndex));
		ost << kwoCurrent->GetSymbolValueAt(liLoadedAttributeIndex) << ", ";
	}*/
	ost << "\nCondensed:\n";
	for (nIndex = 0; nIndex < oaPreprocessedValues.GetSize(); nIndex++)
	{
		krpvValue = cast(KRPreprocessedValue*, oaPreprocessedValues.GetAt(nIndex));
		krpvValue->Write(ost);
		ost << ", ";
	}
	ost << endl;
}

////////////////////////////////////////
// Classe KRPreprocessedValue
KRPreprocessedValue::KRPreprocessedValue(Symbol sValue, int nFrequency, int nFirstIndex, int nLastIndex)
{
	require(nValueFrequency > 0);

	sSymbolValue = sValue;
	nValueFrequency = nFrequency;
	nValueFirstIndex = nFirstIndex;
	nValueLastIndex = nLastIndex;
}

KRPreprocessedValue::~KRPreprocessedValue()
{
}

Symbol KRPreprocessedValue::GetValue() const
{
	return sSymbolValue;
}

int KRPreprocessedValue::GetValueFrequency() const
{
	return nValueFrequency;
}

int KRPreprocessedValue::GetFirstIndex() const
{
	return nValueFirstIndex;
}

int KRPreprocessedValue::GetLastIndex() const
{
	return nValueLastIndex;
}

void KRPreprocessedValue::Write(ostream& ost) const
{
	ost << "(" << sSymbolValue << " [" << nValueFirstIndex << "-" << nValueLastIndex << "] :" << nValueFrequency << ")";
}
