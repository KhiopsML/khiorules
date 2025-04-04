// Copyright (c) 2023-2025 Orange. All rights reserved.
// This software is distributed under the BSD 3-Clause-clear License, the text of which is available
// at https://spdx.org/licenses/BSD-3-Clause-Clear.html or see the "LICENSE" file for more details.

#ifndef KRPREPROCESSEDATTRIBUTE_H
#define KRPREPROCESSEDATTRIBUTE_H

class KRPreprocessedAttribute;
class KRPreprocessedContinuousAttribute;
class KRPreprocessedSymbolAttribute;
class KRPreprocessedValue;

#include "Vector.h"
#include "KWType.h"
#include "KWSymbol.h"
#include "KWDatabase.h"
#include "KWClassStats.h"
#include "KRRuleOptimizer.h"
#include "KRRule.h"
#include "KRObjectCounters.h"

// Classe permettant de trier une base
// Permet de simuler une ancienne fonctionnalité des KWDatabase du temps de Khiops V8
// pour eviter une migration plus complete du code
class KWDatabaseSorter : public Object
{
public:
	// Tri de la base de donnees base suivant un attribut Continuous
	static void SortDatabaseObjectsOnContinuous(KWDatabase* database, const ALString& sAttributeName);

	// Tri de la base de donnees base suivant un attribut Symbol
	// Tri rapide base sur l'identifiant des Symbol
	static void SortDatabaseObjectsOnSymbol(KWDatabase* database, const ALString& sAttributeName);

	///////////////////////////////////////////////////////////////////////////////////
	// Implementation
protected:
	// Tri de la base de donnees
	// On se base sur un algorithme plus rapide que le Sort
	// des ObjectArray
	static void SortObjects(KWDatabase* database, CompareFunction fCompare);
};

////////////////////////////////////////////////////////////////////////////////////
// Classe KRPreprocessedAttribute
// Structure de donnees de pretraitement pour l'optimisation de regles
// principalement caracterise par:
//		nom de l'attribut
//		tableau d'objects tries selon la valeur de l'attribut

class KRPreprocessedAttribute : public Object
{
public:
	//Constructeur
	KRPreprocessedAttribute();
	~KRPreprocessedAttribute();

	/////////////////////////////////
	// Initialisation de l'attribut
	virtual void Initialize(KWDatabase* kwdb, KWAttribute* attribute);

	////////////////////////////////
	// Specifications de l'attribut
	// Les methodes virtuelles sont a redefinir dans les sous-classes

	// Nom de l'attribut
	void SetAttributeName(const ALString& sValue);
	const ALString& GetAttributeName() const;

	// Type de l'attribut (Symbol ou Continuous)
	virtual int GetAttributeType() const = 0;

	// Acces aux objects pre-tries
	ObjectArray* GetObjects();

	// Parametrage par des statistiques sur le probleme d'apprentissage
	// Permet l'utilisation des statistiques univariees pour optimiser les regles
	// Memoire: les specifications sont referencees et destinee a etre partagees
	void SetClassStats(KWClassStats* stats);
	KWClassStats* GetClassStats() const;

	///////////////////////////////
	// Services divers

	// Service exploitant la structure de pretraitement pour l'optimisation
	// Creation de la sous-base utile a l'optimisation de l'attribut pour une regle
	virtual void PrepareWorkingInstances(KRRAttribute* attribute, KRRule* rule,
					     KRObjectCounters* counters, ObjectArray* oaWorkingInstances) const;

	// Controle d'integrite local a l'attribut
	boolean Check() const;

	// Affichage complet
	virtual void Write(ostream& ost) const;

protected:
	// Attributs
	ALString sAttributeName;
	ObjectArray oaPreSortedObjects;
	KWLoadIndex liLoadedAttributeIndex;
	KWLoadIndex liLoadedTargetAttributeIndex;
	KWClassStats* classStats;
};

////////////////////////////////////////////////////////////////////////////////////
// Classe KRPreprocessedContinuousAttribute
// Structure de donnees de pretraitement pour l'optimisation de regles
// principalement caracterise par:
//		nom de l'attribut
//		tableau d'objects tries selon la valeur de l'attribut

class KRPreprocessedContinuousAttribute : public KRPreprocessedAttribute
{
public:
	//Constructeur
	KRPreprocessedContinuousAttribute();
	~KRPreprocessedContinuousAttribute();

	////////////////////////////////
	// Specifications de l'attribut
	// Les methodes virtuelles sont a redefinir dans les sous-classes

	/////////////////////////////////
	// Initialisation de l'attribut
	void Initialize(KWDatabase* kwdb, KWAttribute* attribute);

	// Type de l'attribut (Symbol ou Continuous)
	int GetAttributeType() const;

	///////////////////////////////
	// Services divers

	// Service exploitant la structure de pretraitement pour l'optimisation
	// Creation de la sous-base utile a l'optimisation de l'attribut pour une regle
	void PrepareWorkingInstances(KRRAttribute* attribute, KRRule* rule,
				     KRObjectCounters* counters, ObjectArray* oaWorkingInstances) const;

	// Affichage complet
	void Write(ostream& ost) const;
};

////////////////////////////////////////////////////////////////////////////////////
// Classe KRPreprocessedSymbolAttribute
// Structure de donnees de pretraitement pour l'optimisation de regles
// principalement caracterise par:
//		nom de l'attribut
//		tableau d'objects tries selon la valeur de l'attribut

class KRPreprocessedSymbolAttribute : public KRPreprocessedAttribute
{
public:
	//Constructeur
	KRPreprocessedSymbolAttribute();
	~KRPreprocessedSymbolAttribute();

	////////////////////////////////
	// Specifications de l'attribut
	// Les methodes virtuelles sont a redefinir dans les sous-classes

	/////////////////////////////////
	// Initialisation de l'attribut
	void Initialize(KWDatabase* kwdb, KWAttribute* attribute);

	// Type de l'attribut (Symbol ou Continuous)
	int GetAttributeType() const;

	////////////////////////////////////////////////////////////
	// Methodes utilitaires de recherche par valeur d'une KRPreprocessedValue particuliere
	// dans le oaPreprocessedValues

	// Construction de la structure d'indexation
	void BuildIndexingStructure();

	// Destruction de la structure d'indexation
	void DeleteIndexingStructure();

	// Indicateur d'indexation
	boolean IsIndexed() const;

	// recherche de valeur
	KRPreprocessedValue* LookUpPreprocessedValue(Symbol sValue);

	///////////////////////////////
	// Services divers

	// Service exploitant la structure de pretraitement pour l'optimisation
	// Creation de la sous-base utile a l'optimisation de l'attribut pour une regle
	void PrepareWorkingInstances(KRRAttribute* attribute, KRRule* rule,
				     KRObjectCounters* counters, ObjectArray* oaWorkingInstances) const;

	// test
	void Test();

	// Affichage complet
	void Write(ostream& ost) const;

protected:
	//Attributs
	ObjectArray oaPreprocessedValues;
	NumericKeyDictionary nkdValues;
	boolean bIsIndexed;
};

////////////////////////////////////////////////////////////////////////////////////
// Classe KRPreprocessedValue
// Structure de donnees de pretraitement pour l'optimisation de regles
// contient les informations necessaires sur la valeur d'une variable categorielle
// principalement caracterise par:
//		nom de la valeur (symbol)
//		frequence de la valeur dans la base
//		le premier index d'apparition de la valeur dans la BD triee
//		le dernier index d'apparition de la valeur dans la BD triee

class KRPreprocessedValue : public Object
{
public:
	//Constructeur
	KRPreprocessedValue(Symbol sValue, int nFrequency, int nFirstIndex, int nLastIndex);
	~KRPreprocessedValue();

	////////////////////////////////
	// Specification de la valeur

	// Valeur
	Symbol GetValue() const;

	// Effectif lie a la valeur
	int GetValueFrequency() const;

	// Premier index d'apparition dans la BD triee
	int GetFirstIndex() const;

	// Dernier index d'apparition dans la BD triee
	int GetLastIndex() const;

	///////////////////////////////
	// Services divers

	// Affichage complet
	void Write(ostream& ost) const;

protected:
	//Attributs
	Symbol sSymbolValue;
	int nValueFrequency;
	int nValueFirstIndex;
	int nValueLastIndex;
};

#endif // KRPREPROCESSEDATTRIBUTE_H
