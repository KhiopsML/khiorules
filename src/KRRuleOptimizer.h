// Copyright (c) 2023-2025 Orange. All rights reserved.
// This software is distributed under the BSD 3-Clause-clear License, the text of which is available
// at https://spdx.org/licenses/BSD-3-Clause-Clear.html or see the "LICENSE" file for more details.

#ifndef KRRULEOPTIMIZER_H
#define KRRULEOPTIMIZER_H

#include "KWClassStats.h"
#include "KRRule.h"
#include "KRRuleCosts.h"
#include "KRPreprocessedAttribute.h"
#include "KRObjectCounters.h"

//////////////////////////////////////////////////////////////////////////////////
// Classe KRRuleOptimizer
// Optimisation d'une regle MODL Baye-optimale.
class KRRuleOptimizer : public Object
{
public:
	// Constructeur
	KRRuleOptimizer();
	~KRRuleOptimizer();


	// Parametrage du nombre de regles voulu en sortie
	void SetMaxRuleNumber(int nNumber);
	const int GetMaxRuleNumber() const;

	// Parametrage du nombre de regles voulu en sortie
	void SetTimeAllowed(int nNumber);
	const int GetTimeAllowed() const;

	// Parametrage par des statistiques sur le probleme d'apprentissage
	// Permet l'utilisation des statistiques univariees pour optimiser les regles
	// Memoire: les specifications sont referencees et destinee a etre partagees
	void SetClassStats(KWClassStats* stats);
	KWClassStats* GetClassStats() const;

	// Parametrage de la structure de couts de la regle
	void SetRuleCosts(KRRuleCosts* costs);
	KRRuleCosts* GetRuleCosts() const;

	////////////////////////////////////////////////////////////
	// Methodes utilitaires de recherche par nom d'un KRPreprocessedAttribute particulier
	// dans le oaPreprocessedAttribute

	// Construction de la structure d'indexation
	void BuildIndexingStructure();

	// Destruction de la structure d'indexation
	void DeleteIndexingStructure();

	// Indicateur d'indexation
	boolean IsIndexed() const;

	// recherche d'attribut par nom
	KRPreprocessedAttribute* LookUpPreprocessedAttribute(const ALString& sName) const;

	//Initialisation des compteurs d'objets a partir d'une regle initialisee
	void InitializeObjectsCounters(KWDatabase* kwdb, KRRule* rule);

	// verification de la coherence des compteurs avec une regle
	boolean CheckObjectCounters(KWDatabase* kwdb, KRRule* rule);

	// Algorithme d'extraction de regles MODL Bayes-optimales
	// En sortie, on obtient un tableau de KRRules optimisees
	// voir les details de l'algorithme dans le rapport/article
	void OptimizeRuleSet(KWDatabase* kwdb, ObjectArray* oaRuleSet);

	// Initialisation/Pretraitement des structures necessaires a l'optimisation
	void Preprocess(KWDatabase* kwdb);

	// Affichage des donnees de pretraitement
	void WritePreprocessing(ostream& ost);


protected:
	//////////////////////////////////////////////////////
	// Implementation

	// Selection aleatoire d'un sous-ensemble de min(numberOfAttributes , log2(numberOfObjects) + 1)
	// d'indices d'attributs
	// Utilise pour l'optimisation d'une regle dont le corps est construit sur ces attributs
	void RandomAttributeSubsetSelection(KWDatabase* kwdb, ObjectArray* oaAttributes);

	// Initialisation d'une regle a partir d'un sous-ensemble d'attributs
	// et qui couvre un objet de la base. Le level de la regle est renvoye
	double InitializeRule(KWDatabase* kwdb,
		ObjectArray* oaAttributes,
		KWObject* kwoInBody,
		KRRule* krrInitRule);
	void InitAttributeIntervals(KWDatabase* database, KWObject* kwoRandomObject, KWAttribute* kwaAttribute, KRRAttribute* krrAttribute);
	void InitAttributeValueSets(KWDatabase* database, KWObject* kwoRandomObject, KWAttribute* kwaAttribute, KRRAttribute* krrAttribute);

	// Mise a jour des statistiques
	// Mise a jour totale/globale de la regle
	void UpdateAllStatistics(KWDatabase* database, KRRule* krrInitRule);
	// Mise a jour seulement sur la sous base construite
	void UpdateSubStatistics(ObjectArray* oaWorkingInstances, KRRule* krrInitRule);
	// Mise a jour de la partie corps et exterieur
	void UpdateBodyStatistics(KWDatabase* database, KRRule* krrInitRule, IntVector* ivTargetValuesFrequencyDiff);
	// Mise a jour de la partie locale attribut en cours d'optimisation (lorsque le nb de parties change)
	void UpdateAttributeStatistics(KWDatabase* database, KRRule* krrInitRule, KRRAttribute* attribute);
	// Mise a jour de la partie locale Rule (lorsque le nombre d'attributs change)
	void UpdateRuleStatistics(KWDatabase* database, KRRule* krrInitRule);

	double ComputeRuleLevel(KWDatabase* database, KRRule* krrInitRule);

	// Services temproraires
	boolean IsInBody(KWObject* kwoOject, KRRule* rule);

	// Optimisation d'un attribut de la regle. La regle appelee est la regle optimisee en sortie.
	// Le level de la regle optimisee est renvoye
	double OptimizeRuleAttribute(KWDatabase* kwdb,
		KRRAttribute* krrAttribute,
		KWObject* kwoInBody,
		KRRule* krrCurrentRule);

	// Optimisation d'un attribut numérique
	double OptimizeContinuousAttribute(KWDatabase* kwdb,
		KRRAttribute* krrAttribute,
		KWObject* kwoInBody,
		KRRule* krrCurrentRule);

	// Attribut numérique à 2 parties
	void Optimize2PartsContinuousAttribute(KWDatabase* kwdb,
		KRRAttribute* krrAttribute,
		KWObject* kwoInBody,
		KRRule* krrCurrentRule);

	// Attribut numérique à 3 parties
	void Optimize3PartsContinuousAttribute(KWDatabase* kwdb,
		KRRAttribute* krrAttribute,
		KWObject* kwoInBody,
		KRRule* krrCurrentRule);

	// Attribut categoriel
	double OptimizeSymbolAttribute(KWDatabase* kwdb,
		KRRAttribute* krrAttribute,
		KWObject* kwoInBody,
		KRRule* krrCurrentRule);
	// changement de partie pour un valeur symbolique
	void ChangeValuePart(Symbol sValue,
		KRRPart* krrpOrigin, KRRPart* krrpDestination) const;

	// Optimisation d'uen regle. La regle appelee est laregle optimisee en sortie
	// Le level de la regle optimisee est renvoye
	double OptimizeRule(KWDatabase* kwdb, KWObject* kwoInBody, KRRule* krrCurrentRule);

	//Attributs
	int nMaxRuleNumber; // par defaut 10
	int nTimeAllowed; // par defaut 600
	KWClassStats* classStats;
	KRRuleCosts* ruleCosts;
	ObjectArray oaPreprocessedAttributes;
	ObjectDictionary odPreprocessedAttributes;
	boolean bIsIndexed;
	KRObjectCounters krocCounters;
	double dEpsilon;
};

#endif // KRRULEOPTIMIZER_H
