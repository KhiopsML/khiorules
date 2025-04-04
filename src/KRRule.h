// Copyright (c) 2023-2025 Orange. All rights reserved.
// This software is distributed under the BSD 3-Clause-clear License, the text of which is available
// at https://spdx.org/licenses/BSD-3-Clause-Clear.html or see the "LICENSE" file for more details.

///////////////////////////////////////
// Classe modifiee pour adaptation
// aux regression rules
// changement identifie par //RegRule
//////////////////////////////////////
#ifndef KRRULE_H
#define KRRULE_H

class KRRule;
class KRRAttribute;
class KRRPart;
class KRRInterval;
class KRRValueSet;
class KRRValue;

#include "Vector.h"
#include "KWType.h"
#include "KWSymbol.h"
#include "KWContinuous.h"
#include "KWDatabase.h"
#include "KRRuleCosts.h"

////////////////////////////////////////////////////////////////////////////////////
// Classe KRRule
// Structure de donnees permettant l'enregistrement
// d'une regle MODL Bayes-optimale. Cette structure est adaptee pour l'optimisation
// de regle (geree par KRRuleOptimizer).
class KRRule : public Object
{
public:
	//Constructeur
	KRRule();
	~KRRule();

	////////////////////////////////////////////////////////////////////////////////
	// Specification des donnees d'une KRRule
	// Une KRRule est representee sous forme d'un graphe d'objets:
	//		- au premier niveau, un tableau d'attributs
	//		(incluant l'attribut classe si regression ou regroupement de valeurs de classes)
	//		- ainsi les vecteurs d'effectifs des classes cibles
	//			dans la base et dans le corps de regle (seulement pour la classif supervisee)
	// Chaque attribut est defini principalement par :
	//		- son type, le nombre de valeurs
	//		- son tableau de parties (2 ou 3)
	//		- son dictionnaire des valeurs de ValueSet dans le corps
	//		  pour acces/recherche rapide (geree par hash table)
	// Chaque partie est definie par :
	//		- sa liste de valeurs (KRvalueSet ou KRInterval)
	//
	// La specification d'une KRRule se fait selon les etapes suivantes:
	//   - nombre d'attributs et de classes cibles: Initialize
	//   - specification des attributs: GetAttributeAt, puis methodes des attributs
	//   - specification des parties: ajout et specifications depuis les attributs

	//Initialisation du nombre d'attributs d'une KRRule
	// Le nombre de valeurs de classes cibles est forcément supérieur ou égal à 2
	// La structure de KRRule s'adapte en fonction de la tache:
	// classif sup (avec ou sans regroupement de valeurs), regression
	// classif sup : avec un seul attribut classe (implicite qui n'apparait pas en temps qu'attribut de la KRRule)
	//				a plusieurs valeurs sans regroupement de valeurs possibles
	// regression ou classif sup avec regroupement: l'attribut cible fait partie du trableau d'attributs
	void Initialize(int nAttributeNumber, int nTargetType, int nTargetValueNumber);
	int GetAttributeNumber() const;
	int GetTargetValueNumber() const;

	// Test si la KRRule est dans un etat vide (identique a l'etat initial)
	boolean IsEmpty() const;

	// Parametrage des valeurs cibles (uniquement pour le reporting)
	void SetTargetValueAt(int nIndex, Symbol sValue);
	Symbol GetTargetValueAt(int nIndex) const;
	// acces a l'index d'une valeur cible dans le tableau de valeurs cibles
	// -1 si pas trouve
	int ComputeTargetValueIndex(Symbol sValue) const;

	// acces a l'index d'une valeur continue cible dans le tableau des parties de l'attribut
	// -1 si pas trouve
	int ComputeContinuousTargetValuePartIndex(Continuous cValue) const;

	// Acces aux attributs de la KRRule
	// L'appelant se sert de ces point d'entree pour creer la structure de KRRule
	KRRAttribute* GetAttributeAt(int nAttributeIndex) const;

	// Recherche d'un attribut par son nom (recherche couteuse, par parcours exhaustif des attributs)
	// Renvoie NULL si non trouve
	KRRAttribute* SearchAttribute(const ALString& sAttributeName) const;

	// Acces a l'attribut cible (s'il existe, c'est le dernier attribut de la regle, NULL sinon)
	// Cas d'un attribut cible avec groupement de valeur (regression ou classification
	// avec valeurs cibles a grouper).
	KRRAttribute* GetTargetAttribute() const;

	// Acces au type d'attribut cible
	// Utile lorsque celui-ci est implicite (cas classif supervisée sans regroupement)
	int GetTargetType() const;

	// Nature de la variable cible (partitionee ou non)
	boolean IsTargetPartitioned() const;

	// Mise a jour des vecteurs des effectifs des classes cibles
	void SetTargetValueFrequencyAt(int nIndex, int nValue);
	int GetTargetValueFrequencyAt(int nIndex) const;

	// Mise a jour des vecteurs des effectifs des classes cibles du corps
	void SetInBodyTargetValueFrequencyAt(int nIndex, int nValue);
	int GetInBodyTargetValueFrequencyAt(int nIndex) const;
	int GetInBodyTotalFrequency() const;

	// Mise a jour et maintenance de la taille des vecteurs de frequences pour la regression
	void UpdateTargetVectorsFrequency();
	boolean IsTargetVectorSizeValid() const;

	// Acces frequence totale
	void SetTotalFrequency(int nValue);
	int GetTotalFrequency() const;

	// Cout local d'une regle
	void SetCost(double dValue);
	double GetCost() const;

	// Cout local du corps de regle
	void SetBodyCost(double dValue);
	double GetBodyCost() const;
	void SetOutBodyCost(double dValue);
	double GetOutBodyCost() const;

	// Cout global d'une regle
	void SetNullCost(double dValue);
	double GetNullCost() const;

	double GetTotalCost() const;
	double GetLevel() const;

	// Parametrage de la structure des couts de la regle
	// Memoire: les specifications sont referencees et destinees a etre partagees par plusieurs algorithmes
	//DDDvoid SetRuleCosts(const KRRuleCosts* krrCosts);
	//DDDconst KRRuleCosts* GetRuleCosts() const;

	// Initialisation des couts des entites de la rule
	// (rule, attribute, part, body/out)
	// Methode de type utilitaire, permettant de memoriser les couts par entite d'une rule
	// et d'y avoir acces ensuite par les methodes GetCost()
	void InitializeAllCosts(const KRRuleCosts* krrCosts);
	boolean CheckAllCosts(const KRRuleCosts* krrCosts);

	// Nettoyage des attributs non informatifs (reduits a une seules partie)
	// Prerequis: les attributs doivent etre initialises avec leur partition en intervalles/groupes,
	// mais aucune cellule ne doit etre cree
	// En sortie: les attributs non informatifs sont detruits, les autres etant reindexes
	void DeleteNonInformativeAttributes();
	// Calcul du nombre d'attribut informatifs (ayant strictement plus de une partie)
	int GetInformativeAttributeNumber() const;
	int ComputeInformativeAttributeNumber();

	// Nettoyage complet
	void DeleteAll();

	// Affichage
	void Write(ostream& ost) const;
	void WriteTargetValues(ostream& ost) const;
	void WriteTargetIntervals(ostream& ost) const;
	void WriteAttributes(ostream& ost) const;
	void WriteAttributeParts(ostream& ost) const;
	void WriteLevel(ostream& ost) const;

	// Affichage des statistiques sous forme de table de contingence
	void WriteContingencyTable(ostream& ost) const;

	// Creation/Copie de regle
	KRRule* Create() const;
	void CopyFrom(const KRRule* krrSource);
	KRRule* Clone() const;

	// Libelles utilisateur
	const ALString GetClassLabel() const;
	const ALString GetObjectLabel() const;


	//////////////////////////////////////
	// Implementation
protected:

	friend class KRRAttribute;

	// Modification de l'attribut cible
	void SetTargetAttribute(KRRAttribute* attribute);

	// Tableau des attributs de la KRRule
	ObjectArray oaAttributes;

	// nombre d'attributs informatifs
	int nInformativeAttributeNumber;

	// nombre total d'objets dans la base
	int nTotalFrequency;

	//RegRule
	// Attribut cible
	KRRAttribute* targetAttribute;

	// Type de l'attribut cible
	// Symbol, Continuous ou None pour le cas non-supervise
	int targetType;

	// Vecteur des valeurs cibles
	SymbolVector svTargetValues;

	// Vecteur des frequences des classes cibles
	IntVector ivTargetValueFrequencies;

	// Vecteur des frequences des classes dans le corps de la KRRule
	IntVector ivInBodyTargetValueFrequencies;

	// Parametrage de la structure des couts
	//const KRRuleCosts* ruleCosts;

	// Cout et Level de la KRRule
	//cout local
	double dCost;
	double dBodyCost;
	double dOutBodyCost;

	double dNullCost;
	//double dLevel; //
};

////////////////////////////////////////////////////////////////////////////////////
// Classe KRRAttribute
// Attribut d'une KRRule
// Un KRRAttribute est caracterise par :
//		- le type de l'attribut (Symbol ou Continuous)
//		- nombre de valeurs de l'attribut
//		- le tableau de ses parties (2 ou 3)
//		- un index indiquant la partie qui agit dans le corps de la KRRule
//		- un dictionnaire des valeurs du ValueSet du corps (pour acces rapide par hash table)
class KRRAttribute : public Object
{
public:
	//Constructeur
	KRRAttribute();
	~KRRAttribute();

	// Regle dont l'attribut fait partie
	KRRule* GetRule() const;

	/////////////////////////////////////////////
	// Specifications de l'attribut operande

	// Nom de l'attribut
	void SetAttributeName(const ALString& sValue);
	const ALString& GetAttributeName() const;

	// Type de l'attribut (Symbol ou Continuous)
	// Le type n'est modifiable qu'une seule fois, pour l'initialisation
	void SetAttributeType(int nValue);
	int GetAttributeType() const;

	// Fonction de l'attribut (source ou cible, source par defaut)
	// Seul le dernier attribut d'une grille peut être cible
	void SetAttributeTargetFunction(boolean bTargetAttribute);
	boolean GetAttributeTargetFunction() const;

	// Index de l'attribut dans le tableau des KRRAttribute de la KRRule
	int GetAttributeIndex() const;
	void SetAttributeIndex(int nValue);

	// Nombre de valeurs de l'attribut
	// (nombre d'instances total pour les attributs continus, nombre de valeurs distinctes
	// pour les attributs symboliques)
	void SetValueNumber(int nValue);
	int GetValueNumber() const;

	// Index de la partie qui agit dans le corps de la regle
	void SetBodyPartIndex(int nValue);
	int GetBodyPartIndex() const;

	// Indique si une valeur (continue ou symbolique) est dans le bodyPart
	boolean IsInBodyContinuous(Continuous cValue) const;
	boolean IsInBodySymbol(Symbol sValue) const;

	// Cout local d'un attribut
	void SetCost(double dValue);
	double GetCost() const;

	////////////////////////////////////////////////
	// Gestion des parties de l'attribut
	// Memoire: les parties appartiennent a l'attribut

	// Creation d'une partie et ajout en fin de tableau
	// Le type de l'attribut doit avoir ete specifie en prealable.
	// Renvoie la partie cree (avec le type de l'attribut)
	KRRPart* AddPart();

	// Destruction d'une partie et de son contenu, de facon coherente avec la Rule
	void DeletePart(KRRPart* part);

	// Destruction de tous les parties
	void DeleteAllParts();

	// Test de validite d'une partie (s'il appartient a l'attribut)
	// Attention: operation couteuse en O(n)
	boolean CheckPart(KRRPart* part) const;

	// Nombre de parties
	int GetPartNumber() const;

	// Renvoie une partie
	KRRPart* GetPartAt(int nPartIndex) const;

	///////////////////////////////////////////////////////////////////////////
	// Methodes utilitaires de recherche de la partie (de son ValueSet) contenant
	// une valeur particuliere (pour attribut symbolique)
	// Ces methodes ne sont operationnelles que quand les parties et leurs
	// valeurs sont entierement specifies. Il faut alors creer une structure
	// d'indexation des parties par les valeurs a fins d'optimisation.
	// Cette structure doit etre cree explicitement, puis utilisee (si possible
	// plusieurs fois), et enfin detruite

	// Construction des structures d'indexation
	void BuildIndexingStructure();

	// Destruction des structures d'indexation
	void DeleteIndexingStructure();

	// Indicateur d'indexation
	boolean IsIndexed() const;

	// Recherche de la partie contenant une valeur symbolique
	// (doit etre compatible avec le type de l'attribut)
	// Attention a ne pas modifier les valeurs (ensemble de valeurs)
	// pendant l'utilisation de l'indexation
	KRRPart* LookupSymbolPart(Symbol sValue) const;

	// Recherche de la partie contenant une valeur continue
	// (doit etre compatible avec le type de l'attribut)
	KRRPart* LookupContinuousPart(Continuous cValue) const;

	// Affichage
	void Write(ostream& ost) const;
	void WriteParts(ostream& ost) const;
	void WritePartsLine(ostream& ost) const;

	// Creation/Copie d'attribut
	KRRAttribute* Create() const;
	void CopyFrom(const KRRAttribute* krraSource);

	// Libelles utilisateur
	const ALString GetClassLabel() const;
	const ALString GetObjectLabel() const;

protected:
	friend class KRRule;

	// Attributs
	KRRule* rule;
	ALString sAttributeName;
	int nAttributeType;
	int nValueNumber;
	int nAttributeIndex;
	boolean bTargetAttribute;

	//Gestion des parties
	ObjectArray oaParts;
	// index indiquant la partie qui agit dans le corps de regle
	// 1 ou 2 pour symbolique, 1, 2 ou 3 pour numerique
	int nBodyPartIndex;

	// Structure d'indexation des parties dans le cas symbolique
	// Dictionnaire des parties indexe par les valeurs (Symbol) des parties
	NumericKeyDictionary nkdParts;
	KRRPart* starValuePart;
	boolean bIsIndexed;

	// Cout
	double dCost;
};

////////////////////////////////////////////////////////////////////////////////////
// Classe KRRPart
// Partie d'une KRAttribute
// Une KRRPart est caracterisee par :
//		- sa liste de valeurs
//			KRRValueSet (pour les attributs symboliques)
//			KRRInterval (pour les attributs numériques)
class KRRPart : public Object
{
public:
	//Constructeur
	KRRPart();
	~KRRPart();

	// Attribut auquel appartient la partie
	KRRAttribute* GetAttribute() const;

	// Type de partie (Symbol ou Continuous)
	// Le type n'est modifiable qu'une seule fois, pour l'initialisation
	void SetPartType(int nValue);
	int GetPartType() const;

	// Acces aux valeurs, selon le type
	KRRInterval* GetInterval() const;
	KRRValueSet* GetValueSet() const;

	// Cout local d'une partie
	void SetCost(double dValue);
	double GetCost() const;

	// Affichage
	void Write(ostream& ost) const;
	void WriteValues(ostream& ost) const;

	// Libelles utilisateur
	const ALString GetClassLabel() const;
	const ALString GetObjectLabel() const;

protected:
	friend class KRRule;
	friend class KRRAttribute;

	// Memorisation de l'attribut
	KRRAttribute* attribute;

	// Gestion des valeurs de la parties
	// Un seul objet, correspondant au type de la partie, doit etre non nul
	KRRInterval* interval;
	KRRValueSet* valueSet;

	// cout
	double dCost;
};

//////////////////////////////////////////////////////////////////////////////
// Classe KRRInterval
// Intervalle de valeurs
class KRRInterval : public Object
{
public:
	// Constructeur
	KRRInterval();
	~KRRInterval();

	// Borne inf de l'intervalle
	// (doit etre egal a KRRInterval::GetMinLowerBound pour le premier intervalle)
	void SetLowerBound(Continuous cValue);
	Continuous GetLowerBound() const;

	// Borne sup de l'intervalle
	// (doit etre egal a KRRInterval::GetMaxUpperBound pour le dernier intervalle)
	void SetUpperBound(Continuous cValue);
	Continuous GetUpperBound() const;

	// Valeurs extremes des bornes des intervalles
	static Continuous GetMinLowerBound();
	static Continuous GetMaxUpperBound();

	///////////////////////////////
	// Services divers

	// Controle d'integrite
	boolean Check() const;

	// verifie si une valeur est bien dans l'intervalle
	boolean CheckValue(Continuous cValue) const;

	// Import des valeurs d'un intervalle source, devant etre adjacent au premier
	// intervalle. L'intervalle source est reinitialise
	void Import(KRRInterval* sourceInterval);

	// Copie
	void CopyFrom(const KRRInterval* sourceInterval);

	// Affichage
	void Write(ostream& ost) const;

	// Libelles utilisateur
	const ALString GetClassLabel() const;
	const ALString GetObjectLabel() const;

	///////////////////////////////
	///// Implementation
protected:
	// Bornes de l'intervalle
	Continuous cLowerBound;
	Continuous cUpperBound;
};

//////////////////////////////////////////////////////////////////////////////
// Classe KRRValueSet
// Ensemble de valeurs d'une partie symbolique
class KRRValueSet : public Object
{
public:
	// Constructeur
	KRRValueSet();
	~KRRValueSet();


	////////////////////////////////////////////////////////////////
	// Gestion des valeurs de la parties sous forme de liste
	// Memoire: les valeurs appartiennent a la partie

	// Creation d'une valeur et ajout en fin de liste
	// Renvoie la valeur cree
	KRRValue* AddValue(Symbol sValue, int nFrequency);

	// Destruction d'une valeur de la liste
	void DeleteValue(KRRValue* value);

	// Destruction de toutes les valeurs
	void DeleteAllValues();

	// Nombre de valeurs
	int GetValueNumber() const;

	// Parcours de tous les valeurs
	KRRValue* GetHeadValue() const;
	KRRValue* GetTailValue() const;
	void GetNextValue(KRRValue*& value) const;
	void GetPrevValue(KRRValue*& value) const;

	// Compression des valeurs (pour raison de memoire)
	// La liste des valeurs est remplacee par une seule valeur (la valeur speciale StarValue)
	// Les statistiques (ValueNumber et Frequency) sont conservees
	void CompressValueSet();

	///////////////////////////////
	// Services lies a l'indexation des valeurs du valueSet

	// Construction de la structure d'indexation
	void BuildIndexingStructure();

	// Destruction de la structure d'indexation
	void DeleteIndexingStructure();

	// Indicateur d'indexation
	boolean IsIndexed() const;

	// Test de validite d'une valeur (si elle appartient a la partie)
	boolean CheckValue(KRRValue* value) const;

	///////////////////////////////
	// Services divers

	// Controle d'integrite
	boolean Check() const;

	// Calcul de l'effectif cumule des valeurs
	int ComputeTotalFrequency() const;

	// Import des valeurs d'une partie source, devant etre disjointe de la premiere
	// partie. La partie source est reinitialise
	void Import(KRRValueSet* sourceValueSet);

	// Copie
	void CopyFrom(const KRRValueSet* sourceValueSet);

	// Ajout de nouvelles valeurs recopiees depuis une source
	void UpgradeFrom(const KRRValueSet* sourceValueSet);

	// Tri des valeurs par effectif decroissant, pour preparer l'affichage
	void SortValues();

	// Affichage
	void Write(ostream& ost) const;
	void WriteValues(ostream& ost) const;

	// Libelles utilisateur
	const ALString GetClassLabel() const;
	const ALString GetObjectLabel() const;


	///////////////////////////////
	///// Implementation
protected:

	// Methode indiquant si les donnees sont emulee
	virtual boolean GetEmulated() const;

	// Gestion de la liste doublement chainee des cellules
	KRRValue* headValue;
	KRRValue* tailValue;
	int nValueNumber;
	NumericKeyDictionary nkdValues;
	boolean bIsIndexed;
};



//////////////////////////////////////////////////////////////////////////////
// Classe KRRValue
// Valeur symbolique appartenant a une partie de valeur
class KRRValue : public Object
{
public:
	// Constructeur
	KRRValue(Symbol sValue, int nFrequency);
	~KRRValue();

	// Valeur
	Symbol GetValue() const;

	// Effectif lie a la valeur
	int GetValueFrequency() const;

	// Affichage
	void Write(ostream& ost) const;

	///////////////////////////////
	///// Implementation
protected:
	friend class KRRValueSet;

	// Attributs
	Symbol sSymbolValue;
	int nValueFrequency;
	KRRValue* prevValue;
	KRRValue* nextValue;
};

// Comparaison de deux valeurs symboliques, par effectif decroissant
int KRRValueCompareDecreasingFrequency(const void* elem1, const void* elem2);


#endif // KRRULE_H
