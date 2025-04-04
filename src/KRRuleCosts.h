// Copyright (c) 2023-2025 Orange. All rights reserved.
// This software is distributed under the BSD 3-Clause-clear License, the text of which is available
// at https://spdx.org/licenses/BSD-3-Clause-Clear.html or see the "LICENSE" file for more details.

#ifndef KRRULECOSTS_H
#define KRRULECOSTS_H

class KRRuleCosts;

#include "KRRule.h"
#include "KWStat.h"
#include "KWClassStats.h"


////////////////////////////////////////////////////////////////////////////
// Definition de la structure des couts d'une regle
// Les couts par entite, nuls par defaut, sont redefinissables das les sous-classes
class KRRuleCosts : public Object
{
public:
	// Constructeur
	KRRuleCosts();
	~KRRuleCosts();

	// Duplication
	//DDD virtual KRRuleCosts* Clone() const;

	// Parametrage par des statistiques sur le probleme d'apprentissage
	// Permet l'utilisation des statistiques univariees pour optimiser les regles
    // Memoire: les specifications sont referencees et destinee a etre partagees
    void SetClassStats(KWClassStats* stats);
    KWClassStats* GetClassStats() const;

	/////////////////////////////////////////////////////////////////////////
	// Calcul des couts locaux par entite de la regle, portant
	// sur la regle, les attributs, les parties
	// Methodes virtuelles, renvoyant 0, a reimplementer dans els sous-classes

	// Calcul du cout local d'une regle, en fonction
	// du nombre d'attributs informatifs
	// Les attributs informatifs sont ceux ayant strictement plus de une partie
	virtual double ComputeRuleCost(const KRRule* rule, int nInformativeAttributeNumber) const;

	// Calcul du cout local d'un attribut, pour un nombre de parties donnees
	virtual double ComputeAttributeCost(const KRRAttribute* attribute, int nPartitionSize) const;

	// Calcul du cout local d'une partie
	virtual double ComputePartCost(const KRRPart* part) const;
	virtual double ComputePartUnionCost(const KRRPart* part1, const KRRPart* part2) const;
	virtual double ComputeBodyCost(const KRRule* rule) const;
	virtual double ComputeOutBodyCost(const KRRule* rule) const;

	/////////////////////////////////////////////////////////////////////////
	// Calcul du cout total d'une regle
	//	- cout de la regle
	//  - cout des attributs
	//  - cout des parties
	// Une initialisation prealable est necessaire pour evaluer une fois pour toutes
	// les couts par defaut (cout par attribut non informatif)
	// Le cout global peut alors etre evalue y compris sur une regle partielle,
	// contenant un sous-ensemble des attribut

	// Taux de compression d'une regle (1 - TotalCost/NullRuleCost)
	double ComputeRuleCompressionCoefficient(const KRRule* rule) const;
	virtual double ComputeNullRuleCost(const KRRule* rule) const;

	// Calcul du cout total d'une regle
	double ComputeRuleTotalCost(const KRRule* rule) const;

	// Calcul du cout total d'une rule
	// Similaire au cout total d'une Rule, en utilisant les cout bufferises
	// dans les methodes GetCost() des entites du Rule
	// Des verifications sont effectuees en modes debug
	virtual double ComputeRuleBufferizedTotalCost(const KRRule* rule) const;

	// Verification de la consistance d'un Rule avec la structure de couts constants
	//DDD boolean CheckRule(const KRRule* rule) const;

	// Initialisation des couts constants pour une regle
	//DDD void InitializeCosts(const KRRule* rule);

	//DDD void CleanCosts();
	boolean IsInitialized() const;

	// Acces aux cout total de la regle par defaut
	//DDD double GetTotalDefaultCost() const;

	// Acces aux cout constant par attribut non informatif (cout attribut plus celui de son unique partie)
	// attributs non classe
	int GetTotalAttributeNumber() const;
	//DDD const ALString& GetAttributeNameAt(int nIndex) const;
	//DDD double GetAttributeDefaultCostAt(int nIndex) const;

	// Calcul du cout par defaut des attributs (et parties) absent d'une regle
	//double ComputeRuleTotalMissingAttributeCost(const KRRule* rule) const;


	/////////////////////////////////////////////////////////////////////////
	// Calcul des couts cumulatifs par entite de la regle

	// Calcul du cout cumulatif de la regle (avec ses attributs, parties)
	double ComputeRuleCumulativeCost(const KRRule* rule) const;

	// Calcul du cout cumulatif d'un attribut (avec ses parties)
	double ComputeAttributeCumulativeCost(const KRRAttribute* attribute) const;

	// Calcul du cout cumulatif de la partie (in body or outbody)
	double ComputePartCumulativeCost(const KRRPart* part) const;


	///////////////////////////////////////////////////////////////////////////
	// Affichage des couts par entite

	// Affichage des couts de toutes les entites de la rule
    void WriteRuleAllCosts(const KRRule* rule, ostream& ost) const;
    void WriteRuleCost(const KRRule* rule, ostream& ost) const;

	// Affichage du cout d'un attribut
    void WriteAttributeCostHeaderLine(const KRRAttribute* attribute, ostream& ost) const;
    void WriteAttributeCostLine(const KRRAttribute* attribute, ostream& ost) const;

	// Affichage du cout d'une partie
    void WritePartCostHeaderLine(const KRRPart* part, ostream& ost) const;
    void WritePartCostLine(const KRRPart* part, ostream& ost) const;

	// Affichage du cout du corps et de l'exterieur
	void WriteBodyCostHeaderLine(const KRRule* rule, ostream& ost) const;
    void WriteBodyCostLine(const KRRule* rule, ostream& ost) const;

	void WriteOutBodyCostHeaderLine(const KRRule* rule, ostream& ost) const;
    void WriteOutBodyCostLine(const KRRule* rule, ostream& ost) const;

	// Libelle de la classe
	const ALString GetClassLabel() const;


	///////////////////////////////
    ///// Implementation
protected:
	double dEpsilon;
	KWClassStats* classStats;
};


////////////////////////////////////////////////////////////////////////////
// Structure des couts d'une regle dans le cas de la classification supervisee
class KRRuleClassificationCosts : public KRRuleCosts
{
public:
	// Constructeur
	KRRuleClassificationCosts();
	~KRRuleClassificationCosts();

	// Duplication
	//DDD KRRuleCosts* Clone() const;

	//// Parametrage par des statistiques sur le probleme d'apprentissage
	//// Permet l'utilisation des statistiques univariees pour optimiser les regles
 //   // Memoire: les specifications sont referencees et destinee a etre partagees
 //   void SetClassStats(KWClassStats* stats);
 //   KWClassStats* GetClassStats() const;

	/////////////////////////////////////////////////////////////////////////
	// Calcul des couts locaux par entite de la regle, portant
	// sur la regle, les attributs, les parties

	// Calcul du cout local d'une regle, en fonction
	// du nombre d'attribut informatifs
	//  Les attribut informatif sont ceux ayant strictement plus de une partie
	double ComputeRuleCost(const KRRule* rule, int nInformativeAttributeNumber) const;

	// Calcul du cout local d'un attribut, pour un nombre de parties donnees
	double ComputeAttributeCost(const KRRAttribute* attribute, int nPartitionSize) const;

	// Calcul du cout local d'une partie
	double ComputePartCost(const KRRPart* part) const;
	double ComputePartUnionCost(const KRRPart* part1, const KRRPart* part2) const;
	double ComputeBodyCost(const KRRule* rule) const;
	double ComputeOutBodyCost(const KRRule* rule) const;

	/////////////////////////////////////////////////////////////////////////
	// Calcul du cout total d'une regle
	//	- cout de la regle
	//  - cout des attributs
	//  - cout des parties
	// Une initialisation prealable est necessaire pour evaluer une fois pour toutes
	// les couts par defaut (cout par attribut non informatif)
	// Le cout global peut alors etre evalue y compris sur une regle partielle,
	// contenant un sous-ensemble des attribut

	// Taux de compression d'une regle (1 - TotalCost/NullRuleCost)
	double ComputeRuleCompressionCoefficient(const KRRule* rule) const;
	double ComputeNullRuleCost(const KRRule* rule) const;

	// Calcul du cout total d'une regle
	double ComputeRuleTotalCost(const KRRule* rule) const;

	// Calcul du cout total d'une rule
	// Similaire au cout total d'une Rule, en utilisant les cout bufferises
	// dans les methodes GetCost() des entites du Rule
	// Des verifications sont effectuees en modes debug
	double ComputeRuleBufferizedTotalCost(const KRRule* rule) const;

	// Verification de la consistance d'un Rule avec la structure de couts constants
	//DDD boolean CheckRule(const KRRule* rule) const;

	// Initialisation des couts constants pour une regle
	//DDD void InitializeCosts(const KRRule* rule);

	//DDD void CleanCosts();
	boolean IsInitialized() const;

	// Acces aux cout total de la regle par defaut
	//DDD double GetTotalDefaultCost() const;

	// Acces aux cout constant par attribut non informatif (cout attribut plus celui de son unique partie)
	// attributs non classe
	int GetTotalAttributeNumber() const;
	//DDD const ALString& GetAttributeNameAt(int nIndex) const;
	//DDD double GetAttributeDefaultCostAt(int nIndex) const;

	// Calcul du cout par defaut des attributs (et parties) absent d'une regle
	//double ComputeRuleTotalMissingAttributeCost(const KRRule* rule) const;


	/////////////////////////////////////////////////////////////////////////
	// Calcul des couts cumulatifs par entite de la regle

	// Calcul du cout cumulatif de la regle (avec ses attributs, parties)
	double ComputeRuleCumulativeCost(const KRRule* rule) const;

	// Calcul du cout cumulatif d'un attribut (avec ses parties)
	double ComputeAttributeCumulativeCost(const KRRAttribute* attribute) const;

	// Calcul du cout cumulatif de la partie (in body or outbody)
	double ComputePartCumulativeCost(const KRRPart* part) const;


	/////////////////////////////////////////////////////////////////////////////
	//// Affichage des couts par entite

	//// Affichage des couts de toutes les entites de la rule
 //   void WriteRuleAllCosts(const KRRule* rule, ostream& ost) const;
 //   void WriteRuleCost(const KRRule* rule, ostream& ost) const;

	//// Affichage du cout d'un attribut
 //   void WriteAttributeCostHeaderLine(const KRRAttribute* attribute, ostream& ost) const;
 //   void WriteAttributeCostLine(const KRRAttribute* attribute, ostream& ost) const;

	//// Affichage du cout d'une partie
 //   void WritePartCostHeaderLine(const KRRPart* part, ostream& ost) const;
 //   void WritePartCostLine(const KRRPart* part, ostream& ost) const;

	//// Affichage du cout du corps et de l'exterieur
	//void WriteBodyCostHeaderLine(const KRRule* rule, ostream& ost) const;
 //   void WriteBodyCostLine(const KRRule* rule, ostream& ost) const;
	//
	//void WriteOutBodyCostHeaderLine(const KRRule* rule, ostream& ost) const;
 //   void WriteOutBodyCostLine(const KRRule* rule, ostream& ost) const;

	// Libelle de la classe
	const ALString GetClassLabel() const;


	///////////////////////////////
    ///// Implementation
protected:
	double dEpsilon;
	KWClassStats* classStats;
};


////////////////////////////////////////////////////////////////////////////
// Structure des couts d'une regle dans le cas de la regression
class KRRuleRegressionCosts : public KRRuleCosts
{
public:
	// Constructeur
	KRRuleRegressionCosts();
	~KRRuleRegressionCosts();

	// Duplication
	//DDD KRRuleCosts* Clone() const;

	//// Parametrage par des statistiques sur le probleme d'apprentissage
	//// Permet l'utilisation des statistiques univariees pour optimiser les regles
 //   // Memoire: les specifications sont referencees et destinee a etre partagees
 //   void SetClassStats(KWClassStats* stats);
 //   KWClassStats* GetClassStats() const;

	/////////////////////////////////////////////////////////////////////////
	// Calcul des couts locaux par entite de la regle, portant
	// sur la regle, les attributs, les parties

	// Calcul du cout local d'une regle, en fonction
	// du nombre d'attribut informatifs
	//  Les attribut informatif sont ceux ayant strictement plus de une partie
	double ComputeRuleCost(const KRRule* rule, int nInformativeAttributeNumber) const;

	// Calcul du cout local d'un attribut, pour un nombre de parties donnees
	double ComputeAttributeCost(const KRRAttribute* attribute, int nPartitionSize) const;

	// Calcul du cout local d'une partie
	double ComputePartCost(const KRRPart* part) const;
	double ComputePartUnionCost(const KRRPart* part1, const KRRPart* part2) const;
	double ComputeBodyCost(const KRRule* rule) const;
	double ComputeOutBodyCost(const KRRule* rule) const;

	/////////////////////////////////////////////////////////////////////////
	// Calcul du cout total d'une regle
	//	- cout de la regle
	//  - cout des attributs
	//  - cout des parties
	// Une initialisation prealable est necessaire pour evaluer une fois pour toutes
	// les couts par defaut (cout par attribut non informatif)
	// Le cout global peut alors etre evalue y compris sur une regle partielle,
	// contenant un sous-ensemble des attribut

	// Taux de compression d'une regle (1 - TotalCost/NullRuleCost)
	double ComputeRuleCompressionCoefficient(const KRRule* rule) const;
	double ComputeNullRuleCost(const KRRule* rule) const;

	// Calcul du cout total d'une regle
	double ComputeRuleTotalCost(const KRRule* rule) const;

	// Calcul du cout total d'une rule
	// Similaire au cout total d'une Rule, en utilisant les cout bufferises
	// dans les methodes GetCost() des entites du Rule
	// Des verifications sont effectuees en modes debug
	double ComputeRuleBufferizedTotalCost(const KRRule* rule) const;

	// Verification de la consistance d'un Rule avec la structure de couts constants
	//DDD boolean CheckRule(const KRRule* rule) const;

	// Initialisation des couts constants pour une regle
	//DDD void InitializeCosts(const KRRule* rule);

	//DDD void CleanCosts();
	boolean IsInitialized() const;

	// Acces aux cout total de la regle par defaut
	//DDD double GetTotalDefaultCost() const;

	// Acces aux cout constant par attribut non informatif (cout attribut plus celui de son unique partie)
	// attributs non classe
	int GetTotalAttributeNumber() const;
	//DDD const ALString& GetAttributeNameAt(int nIndex) const;
	//DDD double GetAttributeDefaultCostAt(int nIndex) const;

	// Calcul du cout par defaut des attributs (et parties) absent d'une regle
	//double ComputeRuleTotalMissingAttributeCost(const KRRule* rule) const;


	/////////////////////////////////////////////////////////////////////////
	// Calcul des couts cumulatifs par entite de la regle

	// Calcul du cout cumulatif de la regle (avec ses attributs, parties)
	double ComputeRuleCumulativeCost(const KRRule* rule) const;

	// Calcul du cout cumulatif d'un attribut (avec ses parties)
	double ComputeAttributeCumulativeCost(const KRRAttribute* attribute) const;

	// Calcul du cout cumulatif de la partie (in body or outbody)
	double ComputePartCumulativeCost(const KRRPart* part) const;


	///////////////////////////////////////////////////////////////////////////
	// Affichage des couts par entite

	// Affichage des couts de toutes les entites de la rule
    void WriteRuleAllCosts(const KRRule* rule, ostream& ost) const;
    void WriteRuleCost(const KRRule* rule, ostream& ost) const;

	// Affichage du cout d'un attribut
    void WriteAttributeCostHeaderLine(const KRRAttribute* attribute, ostream& ost) const;
    void WriteAttributeCostLine(const KRRAttribute* attribute, ostream& ost) const;

	// Affichage du cout d'une partie
    void WritePartCostHeaderLine(const KRRPart* part, ostream& ost) const;
    void WritePartCostLine(const KRRPart* part, ostream& ost) const;

	// Affichage du cout du corps et de l'exterieur
	void WriteBodyCostHeaderLine(const KRRule* rule, ostream& ost) const;
    void WriteBodyCostLine(const KRRule* rule, ostream& ost) const;

	void WriteOutBodyCostHeaderLine(const KRRule* rule, ostream& ost) const;
    void WriteOutBodyCostLine(const KRRule* rule, ostream& ost) const;

	// Libelle de la classe
	const ALString GetClassLabel() const;


	///////////////////////////////
    ///// Implementation
protected:
	double dEpsilon;
	KWClassStats* classStats;
};

#endif // KRRULECOSTS_H
