// Copyright (c) 2023-2025 Orange. All rights reserved.
// This software is distributed under the BSD 3-Clause-clear License, the text of which is available
// at https://spdx.org/licenses/BSD-3-Clause-Clear.html or see the "LICENSE" file for more details.

#ifndef KRPREDICTORRULE_H
#define KRPREDICTORRULE_H

#include "KWPredictor.h"
#include "KWPredictorNaiveBayes.h"
#include "KWPredictorUnivariate.h"
#include "KRDRPredictorRule.h"
#include "KWClassStats.h"
#include "KRRuleCosts.h"
#include "KRRule.h"
#include "KWStat.h"
#include "KRRuleOptimizer.h"
#include "KRRuleParameters.h"
#include "KWDRLogical.h"
#include "KWDRCompare.h"
#include "KWDRStandard.h"
#include "SNBPredictorSelectiveNaiveBayes.h"

class KRPredictorRule;
class KRPredictorRuleReport;

/////////////////////////////////////////////////////////////////////
// Classe KRPredictorRule
// Les fonctionnalites de cette classe permettent une classification
// a base de regles Bayes-optimales MODL
// Le coeur de tout classifieur est constitue par sa methode InternalTrain().
class KRPredictorRule : public KWPredictor
{
public:
	// Constructeur
	KRPredictorRule();
	~KRPredictorRule();

	///////////////////////////////////////////////////////////////////////////
	// Parametrage obligatoire du predicteur, a redefinir dans la sous-classe

	// Type de predicteur disponible
	//   KWType::Symbol: classification
	//   KWType::Continuous: regression
	//   KWType::None: non supervise
	boolean IsTargetTypeManaged(int nType) const;

	// Creation pour renvoyer une instance du meme type dynamique
	// Doit etre reimplementer dans les sous-classes
	// La reimplementation typique est:
	//      KWPredictor* KWSpecificPredictor::Create() const
	//      {
	//          return new KWSpecificPredictor;
	//      }
	KWPredictor* Create() const;

	// Nom du predicteur
	// Doit etre reimplementer dans les sous-classes
	const ALString GetName() const;

	// Prefixe du predicteur, utilisable pour le nommage de la classe en deploiement (par defaut: "P_")
	const ALString GetPrefix() const;

	//////////////////////////////////////////////////////////////////////////////////////
	// Parametrage utilisateur des attributs a evaluer ou selectionner
	// Rappel: on peut acceder a GetTrainParameters() par une methode ancetre
	//KRTrainParameters* GetTrainParameters();

	// Parametre d'optimization au niveau de la regle et ses attributs
	// Memoire: appartient a l'appele
	KRRuleParameters* GetRuleParameters();

	////////////////////////////////////////////////////////////////////
	// Acces aux resultats apres apprentissage
	// L'apprentissage est gere avec suivi des taches (MainLabel, Progression, Label).
	// En cas d'interruption, l'apprentissage est invalide

	// Rapport d'apprentissage, avec la description des regles utilisees
	// Memoire: l'objet rendu appartient a l'appele
	KRPredictorRuleReport* GetPredictorRuleReport();

	// Recopie des specifications du predicteurs
	void CopyFrom(const KWPredictor* kwpSource);

	///////////////////////////////////////////////////////
	//// Implementation
protected:
	// Redefinition de la methode de creation du rapport, pour integrer la description
	// des regles extraites (renvoie un KWPredictorSelectionReport)
	void CreatePredictorReport();

	// Alimentation du rapport de prediction par les statistiques sur les regles selectionnees
	void FillPredictorRuleReport(ObjectArray* oaRuleSet);

	// Creation d'une classe avec des nouveaux attributs pour chaque regle
	KWClass* CreateRuleSetClass(ObjectArray* oaRuleSet);
	void UpdateRuleClass(KWClass* ruleClass, KRRule* rule, int ruleIndex);

	// Redefinition de la methode d'apprentissage
	boolean InternalTrain();

	// Creation d'un predicteur a partir de la classe de regles
	void InternalTrainPredictorFromRuleClass(KWClass* ruleClass);

	// Parametres d'optimization pour les regles
	KRRuleParameters ruleParameters;
};

///////////////////////////////////////////////////////////////////////////////
// Rapport d'apprentissage, avec utilisation des regles produites
// Ces resultat sont alimentes par la methode Train des predicteurs
class KRPredictorRuleReport : public KWPredictorReport
{
public:
	// Constructeur
	KRPredictorRuleReport();
	~KRPredictorRuleReport();

	ObjectArray* GetSelectedRuleReports();

	// Ecriture d'un rapport detaille du predicteur, avec les regles extraites
	void WriteReport(ostream& ost);

	/////////////////////////////////////////////////////////
	///// Implementation
protected:
	//Tableau des regles utilisees
	ObjectArray oaSelectedRuleReports;
};

//////////////////////////////////////////////////////////////////////////////
// Informations sur une regle selectionnee
class KRSelectedRuleReport : public KWLearningReport
{
public:
	// Constructeur
	KRSelectedRuleReport();
	~KRSelectedRuleReport();

	// Regle de preparation
	// Memoire: la regle en parametre appartient a l'appele
	//   (le setter detruit et remplace l'eventuelle grille precedente)
	void SetPreparedRule(KRRule* rule);
	const KRRule* GetPreparedRule() const;

	// Rapport synthetique destine a rentrer dans un tableau
	// Tous les champs sont affiches: noms des attributs, Level
	void WriteHeaderLineReport(ostream& ost);
	void WriteLineReport(ostream& ost);

	// Ecriture d'un rapport
	void WriteReport(ostream& ost);

	// Redefinition des criteres de tri pour trier les regles d'un classifieur
	const ALString GetSortName() const;
	double GetSortValue() const;

	/////////////////////////////////////////////////////////
	///// Implementation
protected:
	KRRule* preparedRule;
};

#endif // KRPREDICTORRULE_H
