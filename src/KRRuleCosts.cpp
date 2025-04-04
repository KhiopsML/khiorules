// Copyright (c) 2023-2025 Orange. All rights reserved.
// This software is distributed under the BSD 3-Clause-clear License, the text of which is available
// at https://spdx.org/licenses/BSD-3-Clause-Clear.html or see the "LICENSE" file for more details.

#include "KRRuleCosts.h"


////////////////////////////////////////////////////////////////////////////////////////
// Classe KRRuleCosts


KRRuleCosts::KRRuleCosts()
{
	dEpsilon = 1e-5;
	classStats = NULL;
}


KRRuleCosts::~KRRuleCosts()
{
}


//KRRuleCosts* KRRuleCosts::Clone() const
//{
//	return new KRRuleCosts;
//}

void KRRuleCosts::SetClassStats(KWClassStats* stats)
{
	classStats = stats;
}

KWClassStats* KRRuleCosts::GetClassStats() const
{
	return classStats;
}

double KRRuleCosts::ComputeRuleCost(const KRRule* rule, int nInformativeAttributeNumber) const
{
	require(rule != NULL);
	require(nInformativeAttributeNumber >= 0);
	require(nInformativeAttributeNumber <= GetTotalAttributeNumber());

	return 0;
}


double KRRuleCosts::ComputeAttributeCost(const KRRAttribute* attribute, int nPartitionSize) const
{
	require(attribute != NULL);
	require(attribute->GetValueNumber() > 0);
	require(nPartitionSize >= 1);
	require(nPartitionSize <= attribute->GetValueNumber());
	//require(KWType::IsSimple(attribute->GetAttributeType()));

	return 0;
}


double KRRuleCosts::ComputePartCost(const KRRPart* part) const
{
	require(part != NULL);
	return 0;
}


double KRRuleCosts::ComputePartUnionCost(const KRRPart* part1, const KRRPart* part2) const
{
	require(part1 != NULL);
	require(part2 != NULL);
	return 0;
}


double KRRuleCosts::ComputeBodyCost(const KRRule* rule) const
{
	require(rule != NULL);
	//require(bodyPart->GetAttribute()->GetPartAt(bodyPart->GetAttribute()->GetBodyPartIndex()) == part);

	return 0;
}

double KRRuleCosts::ComputeOutBodyCost(const KRRule* rule) const
{
	require(rule != NULL);
	//require(bodyPart->GetAttribute()->GetPartAt(bodyPart->GetAttribute()->GetBodyPartIndex()) == part);

	return 0;
}


double KRRuleCosts::ComputeRuleCompressionCoefficient(const KRRule* rule) const
{
	double dTotalCost;

	dTotalCost = ComputeRuleTotalCost(rule);
	if (rule->GetNullCost() != 0)
		return 1 - dTotalCost / rule->GetNullCost();
	else
		return 0;
}

double KRRuleCosts::ComputeNullRuleCost(const KRRule* rule) const
{
	require(GetTotalAttributeNumber() != 0);
	require(IsInitialized());

	return 0;
}

double KRRuleCosts::ComputeRuleTotalCost(const KRRule* rule) const
{
	double dTotalCost;

	//require(CheckRule(rule));

	// Initialisation avec le cout cumulatif du Rule
	dTotalCost = ComputeRuleCumulativeCost(rule);

	// Prise en compte du cout des valeurs
	//dTotalCost += dAllValuesDefaultCost;

	// Calcul du cout par defaut des attributs (et parties) absent d'une grille
	//dTotalCost += ComputeRuleTotalMissingAttributeCost(rule);

	return dTotalCost;
}


double KRRuleCosts::ComputeRuleBufferizedTotalCost(const KRRule* rule) const
{
	//require(CheckRule(rule));

	// Initialisation avec le cout du Rule
	//assert(fabs(rule->GetCost() - ComputeRuleCost(rule, rule->GetInformativeAttributeNumber())) < dEpsilon);
	return 0;
}


//boolean KRRuleCosts::CheckRule(const KRRule* rule) const
//{
//	boolean bOk = true;
//	int nDefaultAttribute;
//	KRRAttribute* defaultAttribute;
//	KRRMAttribute* defaultAttributeM;
//	int nAttribute;
//	ALString sAttributeName;
//	boolean bAttributeFound;
//
//	require(rule != NULL);
//
//	// Les couts par defauts doivent être initialises
//	if (not IsInitialized())
//	{
//		AddError("Default data grid costs not initialiszed");
//		bOk = false;
//	}
//
//	// Test de consistance des attributs avec les attributs des couts par defaut
//	// Les attributs sont censes etre dans un ordre coherent
//	nAttribute = 0;
//	if (nAttribute < rule->GetAttributeNumber())
//	{
//		sAttributeName = rule->GetAttributeAt(nAttribute)->GetAttributeName();
//		for (nDefaultAttribute = 0; nDefaultAttribute < ruleDefaultCosts->GetAttributeNumber(); nDefaultAttribute++)
//		{
//			defaultAttribute = ruleDefaultCosts->GetAttributeAt(nDefaultAttribute);
//			defaultAttributeM = cast(KRRMAttribute*, defaultAttribute);
//
//			// Recherche de l'index de l'attribut correspondant parmi les attribut de la grille partielle
//			bAttributeFound = defaultAttribute->GetAttributeName() == sAttributeName;
//
//			// Recherche du prochain nom d'attribut partiel si attribut trouve
//			if (bAttributeFound)
//			{
//				nAttribute++;
//				if (nAttribute < rule->GetAttributeNumber())
//					sAttributeName = rule->GetAttributeAt(nAttribute)->GetAttributeName();
//				else
//					break;
//			}
//			// Erreur si non trouve et plus d'attribut
//			else if (nDefaultAttribute == ruleDefaultCosts->GetAttributeNumber()-1)
//			{
//				AddError("Variable " + sAttributeName + " not found in default data grid");
//				bOk = false;
//				break;
//			}
//		}
//	}
//
//	return bOk;
//}


//void KRRuleCosts::InitializeCosts(const KRRule* rule)
//{
//	boolean bOptimizeMemorySpace = true;
//	KRRuleManager ruleManager;
//	int nAttribute;
//	KRRAttribute* defaultAttribute;
//	KRRPart* defaultPart;
//
//	require(rule != NULL);
//	require(rule->Check());
//
//	// Nettoyage prealable
//	CleanCosts();
//
//	// Creation du Rule des couts par defaut
//	ruleDefaultCosts = new KRRuleMerger;
//	ruleDefaultCosts->SetRuleCosts(this);
//
//	// Creation d'une grille par default correspondant a la grille source
//	ruleManager.SetSourceRule(rule);
//	ruleManager.ExportTerminalRule(ruleDefaultCosts);
//
//	// Initialisation des couts par defaut par entite
//	ruleDefaultCosts->InitializeAllCosts();
//
//	// Memorisation des couts des valeurs symboliques
//	dAllValuesDefaultCost = ComputeRuleAllValuesCost(ruleDefaultCosts);
//
//	// Memorisation du cout total
//	dTotalDefaultCost = ComputeRuleMergerTotalCost(ruleDefaultCosts);
//
//	// Optimisation de la place memoire, en remplacant dans chaque attribut symbolique
//	// l'ensemble des valeurs par une seule valeur "*"
//	// Initialisation des attributs avec une seule partie
//	if (bOptimizeMemorySpace)
//	{
//		for (nAttribute = 0; nAttribute < ruleDefaultCosts->GetAttributeNumber(); nAttribute++)
//		{
//			// Recherche de l'attribut default
//			defaultAttribute = ruleDefaultCosts->GetAttributeAt(nAttribute);
//
//			// Modification de l'ensemble des valeurs des attributs symbolique
//			if (defaultAttribute->GetAttributeType() == KWType::Symbol)
//			{
//				// Acces a la partie unique de l'attribut
//				assert(defaultAttribute->GetPartNumber() == 1);
//				defaultPart = defaultAttribute->GetHeadPart();
//
//				// Remplacement de ses valeurs par une valeur unique
//				defaultPart->GetValueSet()->CompressValueSet();
//			}
//		}
//	}
//	ensure(ruleManager.CheckRule(ruleDefaultCosts));
//}
//
//
//void KRRuleCosts::CleanCosts()
//{
//}
//
//
boolean KRRuleCosts::IsInitialized() const
{
	return classStats != NULL;
}


int KRRuleCosts::GetTotalAttributeNumber() const
{
	//require(IsInitialized());
	return GetClassStats()->GetClass()->GetAttributeNumber() - 1;
}


//double KRRuleCosts::GetTotalDefaultCost() const
//{
//	require(IsInitialized());
//	return dTotalDefaultCost;
//}


//const ALString& KRRuleCosts::GetAttributeNameAt(int nIndex) const
//{
//	require(IsInitialized());
//	require(0 <= nIndex and nIndex < GetTotalAttributeNumber());
//	return krrRule->GetAttributeAt(nIndex)->GetAttributeName();
//}


//double KRRuleCosts::GetAttributeDefaultCostAt(int nIndex) const
//{
//	double dDefaultCost;
//	KRRMAttribute* defaultAttributeM;
//	KRRMPart* defaultPartM;
//
//	require(IsInitialized());
//	require(0 <= nIndex and nIndex < GetTotalAttributeNumber());
//
//	// Acces a l'attribut
//	defaultAttributeM = cast(KRRMAttribute*, ruleDefaultCosts->GetAttributeAt(nIndex));
//	assert(fabs(defaultAttributeM->GetCost() - ComputeAttributeCost(defaultAttributeM, 1)) < dEpsilon);
//	dDefaultCost = defaultAttributeM->GetCost();
//
//	// Cout de la partie
//	assert(defaultAttributeM->GetPartNumber() == 1);
//	defaultPartM = cast(KRRMPart*, defaultAttributeM->GetHeadPart());
//	assert(fabs(defaultPartM->GetCost() - ComputePartCost(defaultPartM)) < 1e-5);
//	dDefaultCost += defaultPartM->GetCost();
//
//	return dDefaultCost;
//}


//double KRRuleCosts::GetAllValuesDefaultCost() const
//{
//	require(IsInitialized());
//	return dAllValuesDefaultCost;
//}
//
//
//double KRRuleCosts::ComputeRuleTotalMissingAttributeCost(const KRRule* rule) const
//{
//	double dTotalCost;
//	int nDefaultAttribute;
//	KRRAttribute* defaultAttribute;
//	KRRPart* part;
//	KRRMAttribute* defaultAttributeM;
//	KRRMPart* partM;
//	int nAttribute;
//	ALString sAttributeName;
//	int nUnusedAttributeNumber;
//	boolean bAttributeFound;
//	KRRAttribute* targetAttribute;
//	KRRAttribute* defaultTargetAttribute;
//
//	require(CheckRule(rule));
//
//	// Recherche des attributs cibles courant et par defaut
//	// Le cout par defaut se rapporte en effet a un attribut cible ayant une seule partie,
//	// alors qu'un attribut source non informatif peut disparaitre. Dans ce cas, le cout
//	// par defaut de ce dernier doit etre recalcule avec le bon nombre de partie cible
//	targetAttribute = rule->GetTargetAttribute();
//	defaultTargetAttribute = ruleDefaultCosts->GetTargetAttribute();
//	assert(defaultTargetAttribute == NULL or defaultTargetAttribute->GetPartNumber() == 1);
//
//	// Actualisation du bon nombre de partie cible pour la grille par defaut
//	if (defaultTargetAttribute != NULL and targetAttribute != NULL and
//		targetAttribute->GetPartNumber() > 1)
//	{
//		while (defaultTargetAttribute->GetPartNumber() < targetAttribute->GetPartNumber())
//			defaultTargetAttribute->AddPart();
//	}
//
//	// Recherche des attributs par defaut ne se trouvant pas dans la grille a evaluer
//	// Les attributs sont censes etre dans un ordre coherent
//	dTotalCost = 0;
//	nUnusedAttributeNumber = 0;
//	nAttribute = 0;
//	if (nAttribute < rule->GetAttributeNumber())
//		sAttributeName = rule->GetAttributeAt(nAttribute)->GetAttributeName();
//	for (nDefaultAttribute = 0; nDefaultAttribute < ruleDefaultCosts->GetAttributeNumber(); nDefaultAttribute++)
//	{
//		defaultAttribute = ruleDefaultCosts->GetAttributeAt(nDefaultAttribute);
//		defaultAttributeM = cast(KRRMAttribute*, defaultAttribute);
//
//		// Recherche de l'index de l'attribut correspondant parmi les attribut de la grille partielle
//		bAttributeFound = defaultAttribute->GetAttributeName() == sAttributeName;
//
//		// Prise en compte du cout de l'attribut terminal s'il n'est pas utilise dans la grille partielle
//		if (not bAttributeFound)
//		{
//			nUnusedAttributeNumber++;
//
//			// Cout par defaut si une seule partie cibme
//			if (defaultTargetAttribute == NULL or defaultTargetAttribute->GetPartNumber() == 1)
//			{
//				// Cout de l'attribut par defaut
//				dTotalCost += defaultAttributeM->GetCost();
//
//				// Cout de la partie
//				assert(defaultAttribute->GetPartNumber() == 1);
//				part = defaultAttribute->GetHeadPart();
//				partM = cast(KRRMPart*, part);
//				dTotalCost += partM->GetCost();
//			}
//			// Sinon, on recalcul les couts des attributs manquants pour tenir compte du nombre
//			// de parties cibles
//			else
//			{
//				// Cout de l'attribut par defaut
//				assert(defaultAttributeM->GetPartNumber() == 1);
//				dTotalCost += ComputeAttributeCost(defaultAttributeM, 1);
//
//				// Cout de la partie
//				assert(defaultAttribute->GetPartNumber() == 1);
//				part = defaultAttribute->GetHeadPart();
//				partM = cast(KRRMPart*, part);
//				dTotalCost += ComputePartCost(partM);
//			}
//		}
//		// Sinon, recherche du prochain nom d'attribut partiel
//		else
//		{
//			nAttribute++;
//			if (nAttribute < rule->GetAttributeNumber())
//				sAttributeName = rule->GetAttributeAt(nAttribute)->GetAttributeName();
//			else
//				sAttributeName = "";
//		}
//	}
//	assert(ruleDefaultCosts->GetAttributeNumber() == rule->GetAttributeNumber() + nUnusedAttributeNumber);
//
//	// Reactualisation a une seule partie cible pour la grille par defaut
//	if (defaultTargetAttribute != NULL and targetAttribute != NULL and
//		targetAttribute->GetPartNumber() > 1)
//	{
//		while (defaultTargetAttribute->GetPartNumber() > 1)
//		{
//			part = defaultTargetAttribute->GetTailPart();
//			defaultTargetAttribute->DeletePart(part);
//		}
//	}
//	assert(defaultTargetAttribute == NULL or defaultTargetAttribute->GetPartNumber() == 1);
//
//	return dTotalCost;
//}


double KRRuleCosts::ComputeRuleCumulativeCost(const KRRule* rule) const
{
	double dGlobalCost = 0;
	int nAttribute;
	KRRAttribute* attribute;

	require(rule != NULL);

	// Initialisation avec le cout du Rule
	dGlobalCost += ComputeRuleCost(rule, rule->GetInformativeAttributeNumber());

	// Prise en compte des attributs
	for (nAttribute = 0; nAttribute < rule->GetAttributeNumber(); nAttribute++)
	{
		attribute = rule->GetAttributeAt(nAttribute);

		// Cout de l'attribut
		dGlobalCost += ComputeAttributeCost(attribute, attribute->GetPartNumber());

		// Cout des parties
		/*for (nPart = 0; nPart < attribute->GetPartNumber(); nPart++)
		{
			part = attribute->GetPartAt(nPart);
			dGlobalCost += ComputePartCost(part);
		}*/
	}

	// Prise en compte du corps et de l'exterieur
	dGlobalCost += ComputeBodyCost(rule);
	dGlobalCost += ComputeOutBodyCost(rule);

	return dGlobalCost;
}


double KRRuleCosts::ComputeAttributeCumulativeCost(const KRRAttribute* attribute) const
{
	double dGlobalValue;
	KRRPart* part;
	int nPart;

	require(attribute != NULL);

	// Cout de l'attribut
	dGlobalValue = ComputeAttributeCost(attribute, attribute->GetPartNumber());

	// Cout globale des parties
	for (nPart = 0; nPart < attribute->GetPartNumber(); nPart++)
	{
		part = attribute->GetPartAt(nPart);
		dGlobalValue += ComputePartCumulativeCost(part);
	}

	return dGlobalValue;
}


double KRRuleCosts::ComputePartCumulativeCost(const KRRPart* part) const
{
	double dGlobalValue;

	require(part != NULL);

	// Valeur de la partie
	dGlobalValue = ComputePartCost(part);

	return dGlobalValue;
}

void KRRuleCosts::WriteRuleAllCosts(const KRRule* rule, ostream& ost) const
{
	int nAttribute;
	KRRAttribute* attribute;

	// Cout du Rule
	WriteRuleCost(rule, ost);

	// Couts des attributs
	for (nAttribute = 0; nAttribute < rule->GetAttributeNumber(); nAttribute++)
	{
		attribute = rule->GetAttributeAt(nAttribute);

		// Cout de l'attribut
		if (nAttribute == 0)
			WriteAttributeCostHeaderLine(attribute, ost);
		WriteAttributeCostLine(attribute, ost);
	}

	// Couts des parties
	//for (nAttribute = 0; nAttribute < rule->GetAttributeNumber(); nAttribute++)
	//{
	//	attribute = rule->GetAttributeAt(nAttribute);

	//	// Cout des parties
	//	for (nPart = 0; nPart < attribute->GetPartNumber(); nPart++)
	//	{
	//		if (nAttribute == 0 and nPart == 0)
	//			WritePartCostHeaderLine(part, ost);
	//		WritePartCostLine(part, ost);
	//	}
	//}

	// Cout du corps et de l'exterieur
	WriteBodyCostHeaderLine(rule, ost);
	WriteBodyCostLine(rule, ost);
	WriteOutBodyCostHeaderLine(rule, ost);
	WriteOutBodyCostLine(rule, ost);


}


void KRRuleCosts::WriteRuleCost(const KRRule* rule, ostream& ost) const
{
	double dRuleCost;
	double dRuleTotalCost;

	require(rule != NULL);
	require(IsInitialized());

	// Calcul des couts locaux et globaux de la grille de donnees
	dRuleCost = ComputeRuleCost(rule,
		rule->GetInformativeAttributeNumber());
	dRuleTotalCost = ComputeRuleTotalCost(rule);

	// Affichage
	ost << rule->GetClassLabel() << "\t" << rule->GetObjectLabel() << "\n";
	cout << "\tRule\t" << dRuleCost << "\n";
	cout << "\tRuleTotal\t" << dRuleTotalCost << "\n";
}


void KRRuleCosts::WriteAttributeCostHeaderLine(const KRRAttribute* attribute, ostream& ost) const
{
	ost << attribute->GetClassLabel() << "\t" <<
		"ValueNumber\tPartNumber\tCost\tCumulativeCost\n";
}


void KRRuleCosts::WriteAttributeCostLine(const KRRAttribute* attribute, ostream& ost) const
{
	double dAttributeCost;
	double dAttributeCumulativeCost;

	require(attribute != NULL);

	// Calcul des couts locaux et globaux de la grille de donnees
	dAttributeCost = ComputeAttributeCost(attribute, attribute->GetPartNumber());
	dAttributeCumulativeCost = ComputeAttributeCumulativeCost(attribute);

	// Affichage
	ost << attribute->GetObjectLabel() << "\t" <<
		attribute->GetValueNumber() << "\t" << attribute->GetPartNumber() << "\t" <<
		dAttributeCost << "\t" << dAttributeCumulativeCost << "\n";
}


void KRRuleCosts::WritePartCostHeaderLine(const KRRPart* part, ostream& ost) const
{
	ost << "Partition\tAttribute\tPart\tCost\tCumulativeCost\n";
}


void KRRuleCosts::WritePartCostLine(const KRRPart* part, ostream& ost) const
{
	double dPartCost;
	double dPartCumulativeCost;

	require(part != NULL);

	// Calcul des couts locaux et globaux de la grille de donnees
	dPartCost = ComputePartCost(part);
	dPartCumulativeCost = ComputePartCumulativeCost(part);

	// Affichage
	ost << part->GetClassLabel() << "\t";
	if (part->GetAttribute() != NULL)
		ost << part->GetAttribute()->GetObjectLabel() << "\t";
	ost << part->GetObjectLabel() << "\t";
	ost << dPartCost << "\t" << dPartCumulativeCost << "\n";
}

void KRRuleCosts::WriteBodyCostHeaderLine(const KRRule* rule, ostream& ost) const
{

}

void KRRuleCosts::WriteBodyCostLine(const KRRule* rule, ostream& ost) const
{

}

void KRRuleCosts::WriteOutBodyCostHeaderLine(const KRRule* rule, ostream& ost) const
{

}

void KRRuleCosts::WriteOutBodyCostLine(const KRRule* rule, ostream& ost) const
{

}

const ALString KRRuleCosts::GetClassLabel() const
{
	return "Rule costs";
}

////////////////////////////////////////////////////////////////////////////////////////
// Classe KRRuleClassificationCosts


KRRuleClassificationCosts::KRRuleClassificationCosts()
{
}


KRRuleClassificationCosts::~KRRuleClassificationCosts()
{
}


double KRRuleClassificationCosts::ComputeRuleCost(const KRRule* rule, int nInformativeAttributeNumber) const
{
	double dRuleCost;

	require(rule != NULL);
	require(nInformativeAttributeNumber >= 0);
	require(nInformativeAttributeNumber <= GetTotalAttributeNumber());

	// Cout de selection des variables
	dRuleCost = 0;
	if (GetTotalAttributeNumber() > 0)
	{
		dRuleCost += log(GetTotalAttributeNumber() + 1.0);
		dRuleCost += KWStat::LnFactorial(GetTotalAttributeNumber() + nInformativeAttributeNumber - 1);
		dRuleCost -= KWStat::LnFactorial(GetTotalAttributeNumber() - 1);
		dRuleCost -= KWStat::LnFactorial(nInformativeAttributeNumber);
	}
	return dRuleCost;
}


double KRRuleClassificationCosts::ComputeAttributeCost(const KRRAttribute* attribute, int nPartitionSize) const
{
	double dAttributeCost = 0;
	double dTemp = 0;
	int nValueNumber;

	require(attribute != NULL);
	require(attribute->GetValueNumber() > 0);
	require(nPartitionSize >= 1);
	require(nPartitionSize <= attribute->GetValueNumber());
	//require(KWType::IsSimple(attribute->GetAttributeType()));

	// Cout uniquement si attribut selectionne (partition univariee non nulle)
	if (nPartitionSize == 1)
		dAttributeCost = 0;
	else
	{
		// Cout de structure si attribut continu
		nValueNumber = attribute->GetValueNumber();
		if (attribute->GetAttributeType() == KWType::Continuous)
		{
			// Cout de codage du nombre d'intervalles
			dAttributeCost = 0;
			if (nValueNumber > 1)
				dAttributeCost += log(2.0);

			// cout de codage du choix du body intervalle
			if (attribute->GetPartNumber() == 2)
				dTemp = 1.0;
			dAttributeCost += log(1.0 + dTemp);

			// Cout de codage des intervalles
			dAttributeCost += KWStat::LnFactorial(nValueNumber - 1);
			dAttributeCost -= KWStat::LnFactorial(nValueNumber - nPartitionSize);
			dAttributeCost -= KWStat::LnFactorial(nPartitionSize - 1);
		}
		// Cout de structure si attribut symbolique
		else
		{
			// Cout de codage du nombre de parties (forcement 2)
			dAttributeCost = 0;

			// cout de codage du choix du body part
			dAttributeCost += log(2.0);

			// Cout de codage du choix des parties
			require(nValueNumber > 1);
			dAttributeCost += (nValueNumber - 1) * log(2.0);
			if (nValueNumber <= 50)
				dAttributeCost += log(1.0 - 1 / pow(2.0, nValueNumber - 1));
			else
				dAttributeCost += log(1.0);
			//DDD dAttributeCost += KWStat::LnBell(nValueNumber, 2);
			//DDD dAttributeCost += log(pow(2.0, nValueNumber -1) -1);
		}
	}
	return dAttributeCost;
}


double KRRuleClassificationCosts::ComputePartCost(const KRRPart* part) const
{
	require(part != NULL);
	return 0;
}


double KRRuleClassificationCosts::ComputePartUnionCost(const KRRPart* part1, const KRRPart* part2) const
{
	require(part1 != NULL);
	require(part2 != NULL);
	return 0;
}


double KRRuleClassificationCosts::ComputeBodyCost(const KRRule* rule) const
{
	double dBodyCost;
	int nBodyFrequency;
	int nIndex;

	require(rule != NULL);
	//require(bodyPart->GetAttribute()->GetPartAt(bodyPart->GetAttribute()->GetBodyPartIndex()) == part);

	nBodyFrequency = 0;
	dBodyCost = 0;

	for (nIndex = 0; nIndex < rule->GetTargetValueNumber(); nIndex++)
	{
		nBodyFrequency += rule->GetInBodyTargetValueFrequencyAt(nIndex);
		dBodyCost -= KWStat::LnFactorial(rule->GetInBodyTargetValueFrequencyAt(nIndex));
	}

	ensure(nBodyFrequency == rule->GetInBodyTotalFrequency());

	dBodyCost += KWStat::LnFactorial(nBodyFrequency + rule->GetTargetValueNumber() - 1);
	dBodyCost -= KWStat::LnFactorial(rule->GetTargetValueNumber() - 1);

	// simplification du log(NX)! du prior et de la vraisemblance
	return dBodyCost;
}

double KRRuleClassificationCosts::ComputeOutBodyCost(const KRRule* rule) const
{
	double dOutBodyCost;
	int nOutBodyFrequency;
	int nIndex;

	require(rule != NULL);
	//require(bodyPart->GetAttribute()->GetPartAt(bodyPart->GetAttribute()->GetBodyPartIndex()) == part);

	nOutBodyFrequency = rule->GetTotalFrequency() - rule->GetInBodyTotalFrequency();
	dOutBodyCost = 0;

	for (nIndex = 0; nIndex < rule->GetTargetValueNumber(); nIndex++)
	{
		dOutBodyCost -= KWStat::LnFactorial(rule->GetTargetValueFrequencyAt(nIndex) - rule->GetInBodyTargetValueFrequencyAt(nIndex));
	}
	dOutBodyCost += KWStat::LnFactorial(nOutBodyFrequency + rule->GetTargetValueNumber() - 1);
	dOutBodyCost -= KWStat::LnFactorial(rule->GetTargetValueNumber() - 1);

	// simplification du log(NnonX)! du prior et de la vraisemblance
	return dOutBodyCost;
}


double KRRuleClassificationCosts::ComputeRuleCompressionCoefficient(const KRRule* rule) const
{
	double dTotalCost;

	dTotalCost = ComputeRuleTotalCost(rule);
	if (rule->GetNullCost() != 0)
		return 1 - dTotalCost / rule->GetNullCost();
	else
		return 0;
}

double KRRuleClassificationCosts::ComputeNullRuleCost(const KRRule* rule) const
{
	double dNullRuleCost;
	int nIndex;

	require(GetTotalAttributeNumber() != 0);
	require(IsInitialized());

	dNullRuleCost = log(GetTotalAttributeNumber() + 1.0)
		+ KWStat::LnFactorial(rule->GetTotalFrequency() + rule->GetTargetValueNumber() - 1)
		- KWStat::LnFactorial(rule->GetTargetValueNumber() - 1)
		- KWStat::LnFactorial(rule->GetTotalFrequency());
	dNullRuleCost += KWStat::LnFactorial(rule->GetTotalFrequency());
	for (nIndex = 0; nIndex < rule->GetTargetValueNumber(); nIndex++)
	{
		dNullRuleCost -= KWStat::LnFactorial(rule->GetTargetValueFrequencyAt(nIndex));
	}
	return dNullRuleCost;
}

double KRRuleClassificationCosts::ComputeRuleTotalCost(const KRRule* rule) const
{
	double dTotalCost;

	//require(CheckRule(rule));

	// Initialisation avec le cout cumulatif du Rule
	dTotalCost = ComputeRuleCumulativeCost(rule);

	// Prise en compte du cout des valeurs
	//dTotalCost += dAllValuesDefaultCost;

	// Calcul du cout par defaut des attributs (et parties) absent d'une grille
	//dTotalCost += ComputeRuleTotalMissingAttributeCost(rule);

	return dTotalCost;
}


double KRRuleClassificationCosts::ComputeRuleBufferizedTotalCost(const KRRule* rule) const
{
	double dTotalCost;
	int nAttribute;
	int nPart;
	KRRAttribute* attribute;
	KRRPart* part;

	//require(CheckRule(rule));

	// Initialisation avec le cout du Rule
	assert(fabs(rule->GetCost() - ComputeRuleCost(rule, rule->GetInformativeAttributeNumber())) < dEpsilon);
	dTotalCost = rule->GetCost();

	// Prise en compte des attributs
	for (nAttribute = 0; nAttribute < rule->GetAttributeNumber(); nAttribute++)
	{
		attribute = rule->GetAttributeAt(nAttribute);
		assert(fabs(attribute->GetCost() - ComputeAttributeCost(attribute, attribute->GetPartNumber())) < dEpsilon);

		// Cout de l'attribut
		dTotalCost += attribute->GetCost();

		// Cout des parties
		for (nPart = 0; nPart < attribute->GetPartNumber(); nPart++)
		{
			part = attribute->GetPartAt(nPart);
			assert(fabs(part->GetCost() - ComputePartCost(part)) < dEpsilon);
			dTotalCost += part->GetCost();
		}
	}

	// Prise en compte du corps et de l'exterieur
	dTotalCost += rule->GetBodyCost();
	dTotalCost += rule->GetOutBodyCost();

	// Prise en compte du cout des valeurs
	//dTotalCost += dAllValuesDefaultCost;

	// Calcul du cout par defaut des attributs (et parties) absent d'une grille
	//dTotalCost += ComputeRuleTotalMissingAttributeCost(ruleMerger);

	return dTotalCost;
}


//boolean KRRuleCosts::CheckRule(const KRRule* rule) const
//{
//	boolean bOk = true;
//	int nDefaultAttribute;
//	KRRAttribute* defaultAttribute;
//	KRRMAttribute* defaultAttributeM;
//	int nAttribute;
//	ALString sAttributeName;
//	boolean bAttributeFound;
//
//	require(rule != NULL);
//
//	// Les couts par defauts doivent être initialises
//	if (not IsInitialized())
//	{
//		AddError("Default data grid costs not initialiszed");
//		bOk = false;
//	}
//
//	// Test de consistance des attributs avec les attributs des couts par defaut
//	// Les attributs sont censes etre dans un ordre coherent
//	nAttribute = 0;
//	if (nAttribute < rule->GetAttributeNumber())
//	{
//		sAttributeName = rule->GetAttributeAt(nAttribute)->GetAttributeName();
//		for (nDefaultAttribute = 0; nDefaultAttribute < ruleDefaultCosts->GetAttributeNumber(); nDefaultAttribute++)
//		{
//			defaultAttribute = ruleDefaultCosts->GetAttributeAt(nDefaultAttribute);
//			defaultAttributeM = cast(KRRMAttribute*, defaultAttribute);
//
//			// Recherche de l'index de l'attribut correspondant parmi les attribut de la grille partielle
//			bAttributeFound = defaultAttribute->GetAttributeName() == sAttributeName;
//
//			// Recherche du prochain nom d'attribut partiel si attribut trouve
//			if (bAttributeFound)
//			{
//				nAttribute++;
//				if (nAttribute < rule->GetAttributeNumber())
//					sAttributeName = rule->GetAttributeAt(nAttribute)->GetAttributeName();
//				else
//					break;
//			}
//			// Erreur si non trouve et plus d'attribut
//			else if (nDefaultAttribute == ruleDefaultCosts->GetAttributeNumber()-1)
//			{
//				AddError("Variable " + sAttributeName + " not found in default data grid");
//				bOk = false;
//				break;
//			}
//		}
//	}
//
//	return bOk;
//}


//void KRRuleCosts::InitializeCosts(const KRRule* rule)
//{
//	boolean bOptimizeMemorySpace = true;
//	KRRuleManager ruleManager;
//	int nAttribute;
//	KRRAttribute* defaultAttribute;
//	KRRPart* defaultPart;
//
//	require(rule != NULL);
//	require(rule->Check());
//
//	// Nettoyage prealable
//	CleanCosts();
//
//	// Creation du Rule des couts par defaut
//	ruleDefaultCosts = new KRRuleMerger;
//	ruleDefaultCosts->SetRuleCosts(this);
//
//	// Creation d'une grille par default correspondant a la grille source
//	ruleManager.SetSourceRule(rule);
//	ruleManager.ExportTerminalRule(ruleDefaultCosts);
//
//	// Initialisation des couts par defaut par entite
//	ruleDefaultCosts->InitializeAllCosts();
//
//	// Memorisation des couts des valeurs symboliques
//	dAllValuesDefaultCost = ComputeRuleAllValuesCost(ruleDefaultCosts);
//
//	// Memorisation du cout total
//	dTotalDefaultCost = ComputeRuleMergerTotalCost(ruleDefaultCosts);
//
//	// Optimisation de la place memoire, en remplacant dans chaque attribut symbolique
//	// l'ensemble des valeurs par une seule valeur "*"
//	// Initialisation des attributs avec une seule partie
//	if (bOptimizeMemorySpace)
//	{
//		for (nAttribute = 0; nAttribute < ruleDefaultCosts->GetAttributeNumber(); nAttribute++)
//		{
//			// Recherche de l'attribut default
//			defaultAttribute = ruleDefaultCosts->GetAttributeAt(nAttribute);
//
//			// Modification de l'ensemble des valeurs des attributs symbolique
//			if (defaultAttribute->GetAttributeType() == KWType::Symbol)
//			{
//				// Acces a la partie unique de l'attribut
//				assert(defaultAttribute->GetPartNumber() == 1);
//				defaultPart = defaultAttribute->GetHeadPart();
//
//				// Remplacement de ses valeurs par une valeur unique
//				defaultPart->GetValueSet()->CompressValueSet();
//			}
//		}
//	}
//	ensure(ruleManager.CheckRule(ruleDefaultCosts));
//}
//
//
//void KRRuleCosts::CleanCosts()
//{
//}
//
//
boolean KRRuleClassificationCosts::IsInitialized() const
{
	return classStats != NULL;
}


int KRRuleClassificationCosts::GetTotalAttributeNumber() const
{
	//require(IsInitialized());
	return GetClassStats()->GetClass()->GetAttributeNumber() - 1;
}


//double KRRuleCosts::GetTotalDefaultCost() const
//{
//	require(IsInitialized());
//	return dTotalDefaultCost;
//}


//const ALString& KRRuleCosts::GetAttributeNameAt(int nIndex) const
//{
//	require(IsInitialized());
//	require(0 <= nIndex and nIndex < GetTotalAttributeNumber());
//	return krrRule->GetAttributeAt(nIndex)->GetAttributeName();
//}


//double KRRuleCosts::GetAttributeDefaultCostAt(int nIndex) const
//{
//	double dDefaultCost;
//	KRRMAttribute* defaultAttributeM;
//	KRRMPart* defaultPartM;
//
//	require(IsInitialized());
//	require(0 <= nIndex and nIndex < GetTotalAttributeNumber());
//
//	// Acces a l'attribut
//	defaultAttributeM = cast(KRRMAttribute*, ruleDefaultCosts->GetAttributeAt(nIndex));
//	assert(fabs(defaultAttributeM->GetCost() - ComputeAttributeCost(defaultAttributeM, 1)) < dEpsilon);
//	dDefaultCost = defaultAttributeM->GetCost();
//
//	// Cout de la partie
//	assert(defaultAttributeM->GetPartNumber() == 1);
//	defaultPartM = cast(KRRMPart*, defaultAttributeM->GetHeadPart());
//	assert(fabs(defaultPartM->GetCost() - ComputePartCost(defaultPartM)) < 1e-5);
//	dDefaultCost += defaultPartM->GetCost();
//
//	return dDefaultCost;
//}


//double KRRuleCosts::GetAllValuesDefaultCost() const
//{
//	require(IsInitialized());
//	return dAllValuesDefaultCost;
//}
//
//
//double KRRuleCosts::ComputeRuleTotalMissingAttributeCost(const KRRule* rule) const
//{
//	double dTotalCost;
//	int nDefaultAttribute;
//	KRRAttribute* defaultAttribute;
//	KRRPart* part;
//	KRRMAttribute* defaultAttributeM;
//	KRRMPart* partM;
//	int nAttribute;
//	ALString sAttributeName;
//	int nUnusedAttributeNumber;
//	boolean bAttributeFound;
//	KRRAttribute* targetAttribute;
//	KRRAttribute* defaultTargetAttribute;
//
//	require(CheckRule(rule));
//
//	// Recherche des attributs cibles courant et par defaut
//	// Le cout par defaut se rapporte en effet a un attribut cible ayant une seule partie,
//	// alors qu'un attribut source non informatif peut disparaitre. Dans ce cas, le cout
//	// par defaut de ce dernier doit etre recalcule avec le bon nombre de partie cible
//	targetAttribute = rule->GetTargetAttribute();
//	defaultTargetAttribute = ruleDefaultCosts->GetTargetAttribute();
//	assert(defaultTargetAttribute == NULL or defaultTargetAttribute->GetPartNumber() == 1);
//
//	// Actualisation du bon nombre de partie cible pour la grille par defaut
//	if (defaultTargetAttribute != NULL and targetAttribute != NULL and
//		targetAttribute->GetPartNumber() > 1)
//	{
//		while (defaultTargetAttribute->GetPartNumber() < targetAttribute->GetPartNumber())
//			defaultTargetAttribute->AddPart();
//	}
//
//	// Recherche des attributs par defaut ne se trouvant pas dans la grille a evaluer
//	// Les attributs sont censes etre dans un ordre coherent
//	dTotalCost = 0;
//	nUnusedAttributeNumber = 0;
//	nAttribute = 0;
//	if (nAttribute < rule->GetAttributeNumber())
//		sAttributeName = rule->GetAttributeAt(nAttribute)->GetAttributeName();
//	for (nDefaultAttribute = 0; nDefaultAttribute < ruleDefaultCosts->GetAttributeNumber(); nDefaultAttribute++)
//	{
//		defaultAttribute = ruleDefaultCosts->GetAttributeAt(nDefaultAttribute);
//		defaultAttributeM = cast(KRRMAttribute*, defaultAttribute);
//
//		// Recherche de l'index de l'attribut correspondant parmi les attribut de la grille partielle
//		bAttributeFound = defaultAttribute->GetAttributeName() == sAttributeName;
//
//		// Prise en compte du cout de l'attribut terminal s'il n'est pas utilise dans la grille partielle
//		if (not bAttributeFound)
//		{
//			nUnusedAttributeNumber++;
//
//			// Cout par defaut si une seule partie cibme
//			if (defaultTargetAttribute == NULL or defaultTargetAttribute->GetPartNumber() == 1)
//			{
//				// Cout de l'attribut par defaut
//				dTotalCost += defaultAttributeM->GetCost();
//
//				// Cout de la partie
//				assert(defaultAttribute->GetPartNumber() == 1);
//				part = defaultAttribute->GetHeadPart();
//				partM = cast(KRRMPart*, part);
//				dTotalCost += partM->GetCost();
//			}
//			// Sinon, on recalcul les couts des attributs manquants pour tenir compte du nombre
//			// de parties cibles
//			else
//			{
//				// Cout de l'attribut par defaut
//				assert(defaultAttributeM->GetPartNumber() == 1);
//				dTotalCost += ComputeAttributeCost(defaultAttributeM, 1);
//
//				// Cout de la partie
//				assert(defaultAttribute->GetPartNumber() == 1);
//				part = defaultAttribute->GetHeadPart();
//				partM = cast(KRRMPart*, part);
//				dTotalCost += ComputePartCost(partM);
//			}
//		}
//		// Sinon, recherche du prochain nom d'attribut partiel
//		else
//		{
//			nAttribute++;
//			if (nAttribute < rule->GetAttributeNumber())
//				sAttributeName = rule->GetAttributeAt(nAttribute)->GetAttributeName();
//			else
//				sAttributeName = "";
//		}
//	}
//	assert(ruleDefaultCosts->GetAttributeNumber() == rule->GetAttributeNumber() + nUnusedAttributeNumber);
//
//	// Reactualisation a une seule partie cible pour la grille par defaut
//	if (defaultTargetAttribute != NULL and targetAttribute != NULL and
//		targetAttribute->GetPartNumber() > 1)
//	{
//		while (defaultTargetAttribute->GetPartNumber() > 1)
//		{
//			part = defaultTargetAttribute->GetTailPart();
//			defaultTargetAttribute->DeletePart(part);
//		}
//	}
//	assert(defaultTargetAttribute == NULL or defaultTargetAttribute->GetPartNumber() == 1);
//
//	return dTotalCost;
//}


double KRRuleClassificationCosts::ComputeRuleCumulativeCost(const KRRule* rule) const
{
	double dGlobalCost = 0;
	int nAttribute;
	KRRAttribute* attribute;

	require(rule != NULL);

	// Initialisation avec le cout du Rule
	dGlobalCost += ComputeRuleCost(rule, rule->GetInformativeAttributeNumber());

	// Prise en compte des attributs
	for (nAttribute = 0; nAttribute < rule->GetAttributeNumber(); nAttribute++)
	{
		attribute = rule->GetAttributeAt(nAttribute);

		// Cout de l'attribut
		dGlobalCost += ComputeAttributeCost(attribute, attribute->GetPartNumber());

		// Cout des parties
		/*for (nPart = 0; nPart < attribute->GetPartNumber(); nPart++)
		{
			part = attribute->GetPartAt(nPart);
			dGlobalCost += ComputePartCost(part);
		}*/
	}

	// Prise en compte du corps et de l'exterieur
	dGlobalCost += ComputeBodyCost(rule);
	dGlobalCost += ComputeOutBodyCost(rule);

	return dGlobalCost;
}


double KRRuleClassificationCosts::ComputeAttributeCumulativeCost(const KRRAttribute* attribute) const
{
	double dGlobalValue;
	KRRPart* part;
	int nPart;

	require(attribute != NULL);

	// Cout de l'attribut
	dGlobalValue = ComputeAttributeCost(attribute, attribute->GetPartNumber());

	// Cout globale des parties
	for (nPart = 0; nPart < attribute->GetPartNumber(); nPart++)
	{
		part = attribute->GetPartAt(nPart);
		dGlobalValue += ComputePartCumulativeCost(part);
	}

	return dGlobalValue;
}


double KRRuleClassificationCosts::ComputePartCumulativeCost(const KRRPart* part) const
{
	double dGlobalValue;

	require(part != NULL);

	// Valeur de la partie
	dGlobalValue = ComputePartCost(part);

	return dGlobalValue;
}

//void KRRuleCosts::WriteRuleAllCosts(const KRRule* rule, ostream& ost) const
//{
//	int nAttribute;
//	KRRAttribute* attribute;
//
//	// Cout du Rule
//	WriteRuleCost(rule, ost);
//
//	// Couts des attributs
//	for (nAttribute = 0; nAttribute < rule->GetAttributeNumber(); nAttribute++)
//	{
//		attribute = rule->GetAttributeAt(nAttribute);
//
//		// Cout de l'attribut
//		if (nAttribute == 0)
//			WriteAttributeCostHeaderLine(attribute, ost);
//		WriteAttributeCostLine(attribute, ost);
//	}
//
//	// Couts des parties
//	//for (nAttribute = 0; nAttribute < rule->GetAttributeNumber(); nAttribute++)
//	//{
//	//	attribute = rule->GetAttributeAt(nAttribute);
//
//	//	// Cout des parties
//	//	for (nPart = 0; nPart < attribute->GetPartNumber(); nPart++)
//	//	{
//	//		if (nAttribute == 0 and nPart == 0)
//	//			WritePartCostHeaderLine(part, ost);
//	//		WritePartCostLine(part, ost);
//	//	}
//	//}
//
//	// Cout du corps et de l'exterieur
//	WriteBodyCostHeaderLine(rule, ost);
//	WriteBodyCostLine(rule, ost);
//	WriteOutBodyCostHeaderLine(rule, ost);
//	WriteOutBodyCostLine(rule, ost);
//
//
//}
//
//
//void KRRuleCosts::WriteRuleCost(const KRRule* rule, ostream& ost) const
//{
//	double dRuleCost;
//	double dRuleTotalCost;
//
//	require(rule != NULL);
//	require(IsInitialized());
//
//	// Calcul des couts locaux et globaux de la grille de donnees
//	dRuleCost = ComputeRuleCost(rule,
//		rule->GetInformativeAttributeNumber());
//	dRuleTotalCost = ComputeRuleTotalCost(rule);
//
//	// Affichage
//	ost << rule->GetClassLabel() << "\t" << rule->GetObjectLabel() << "\n";
//	cout << "\tRule\t" << dRuleCost << "\n";
//	cout << "\tRuleTotal\t" << dRuleTotalCost << "\n";
//}
//
//
//void KRRuleCosts::WriteAttributeCostHeaderLine(const KRRAttribute* attribute, ostream& ost) const
//{
//	ost << attribute->GetClassLabel() << "\t" <<
//		"ValueNumber\tPartNumber\tCost\tCumulativeCost\n";
//}
//
//
//void KRRuleCosts::WriteAttributeCostLine(const KRRAttribute* attribute, ostream& ost) const
//{
//	double dAttributeCost;
//	double dAttributeCumulativeCost;
//
//	require(attribute != NULL);
//
//	// Calcul des couts locaux et globaux de la grille de donnees
//	dAttributeCost = ComputeAttributeCost(attribute, attribute->GetPartNumber());
//	dAttributeCumulativeCost = ComputeAttributeCumulativeCost(attribute);
//
//	// Affichage
//	ost << attribute->GetObjectLabel() << "\t" <<
//		attribute->GetValueNumber() << "\t" << attribute->GetPartNumber() << "\t" <<
//		dAttributeCost << "\t" << dAttributeCumulativeCost << "\n";
//}
//
//
//void KRRuleCosts::WritePartCostHeaderLine(const KRRPart* part, ostream& ost) const
//{
//	ost << "Partition\tAttribute\tPart\tCost\tCumulativeCost\n";
//}
//
//
//void KRRuleCosts::WritePartCostLine(const KRRPart* part, ostream& ost) const
//{
//	double dPartCost;
//	double dPartCumulativeCost;
//
//	require(part != NULL);
//
//	// Calcul des couts locaux et globaux de la grille de donnees
//	dPartCost = ComputePartCost(part);
//	dPartCumulativeCost = ComputePartCumulativeCost(part);
//
//	// Affichage
//	ost << part->GetClassLabel() << "\t";
//	if (part->GetAttribute() != NULL)
//		ost << part->GetAttribute()->GetObjectLabel() << "\t";
//	ost << part->GetObjectLabel() << "\t";
//	ost << dPartCost << "\t" << dPartCumulativeCost << "\n";
//}
//
//void KRRuleCosts::WriteBodyCostHeaderLine(const KRRule* rule, ostream& ost) const
//{
//
//}
//
//void KRRuleCosts::WriteBodyCostLine(const KRRule* rule, ostream& ost) const
//{
//
//}
//
//void KRRuleCosts::WriteOutBodyCostHeaderLine(const KRRule* rule, ostream& ost) const
//{
//
//}
//
//void KRRuleCosts::WriteOutBodyCostLine(const KRRule* rule, ostream& ost) const
//{
//
//}

const ALString KRRuleClassificationCosts::GetClassLabel() const
{
	return "Rule Classification costs";
}

////////////////////////////////////////////////////////////////////////////////////////
// Classe KRRuleRegressionCosts


KRRuleRegressionCosts::KRRuleRegressionCosts()
{
}


KRRuleRegressionCosts::~KRRuleRegressionCosts()
{
}

double KRRuleRegressionCosts::ComputeRuleCost(const KRRule* rule, int nInformativeAttributeNumber) const
{
	double dRuleCost;

	require(rule != NULL);
	require(nInformativeAttributeNumber >= 0);
	require(nInformativeAttributeNumber <= GetTotalAttributeNumber());

	// Cout de selection des variables + cout de la varibale classe numerique
	dRuleCost = 0;
	if (GetTotalAttributeNumber() > 0)
	{
		dRuleCost += log(GetTotalAttributeNumber() + 1.0); // log(m+1)
		dRuleCost += KWStat::LnFactorial(GetTotalAttributeNumber() + nInformativeAttributeNumber - 1); // log(m+k-1)!
		dRuleCost -= KWStat::LnFactorial(GetTotalAttributeNumber() - 1); // log(m+k-1-k)!
		dRuleCost -= KWStat::LnFactorial(nInformativeAttributeNumber); // log(k)!
	}
	return dRuleCost;
}


double KRRuleRegressionCosts::ComputeAttributeCost(const KRRAttribute* attribute, int nPartitionSize) const
{
	double dAttributeCost = 0;
	double dTemp = 0;
	int nValueNumber;
	int nIndex;

	require(attribute != NULL);
	require(attribute->GetValueNumber() > 0);
	require(nPartitionSize >= 1);
	require(nPartitionSize <= attribute->GetValueNumber());
	//require(KWType::IsSimple(attribute->GetAttributeType()));

	// Cout uniquement si attribut selectionne (partition univariee non nulle)
	if (nPartitionSize == 1)
		dAttributeCost = 0;
	else
	{
		// Cout de structure si attribut continu
		nValueNumber = attribute->GetValueNumber();
		if (attribute->GetAttributeType() == KWType::Continuous)
		{
			if (attribute->GetAttributeTargetFunction()) // cas particulier variable classe cible numerique
			{
				dAttributeCost += log(2.0); // log(2) choix du nombre d'intervalles de la variable cible numerique (2 ou 3)
				// Partie likelihood specifique a la variable cible numerique
				for (nIndex=0; nIndex < attribute->GetPartNumber(); nIndex++)
				{
					dAttributeCost += KWStat::LnFactorial(attribute->GetRule()->GetTargetValueFrequencyAt(nIndex));
				}
			}
			else // autre attribut pas cible
			{
				// Cout de codage du nombre d'intervalles
				dAttributeCost = 0;
				if (nValueNumber > 1)
					dAttributeCost += log(2.0);

				// cout de codage du choix du body intervalle
				if (attribute->GetPartNumber() == 2)
					dTemp = 1.0;
				dAttributeCost += log(1.0 + dTemp);

				// Cout de codage des intervalles
				dAttributeCost += KWStat::LnFactorial(nValueNumber - 1);
				dAttributeCost -= KWStat::LnFactorial(nValueNumber - nPartitionSize);
				dAttributeCost -= KWStat::LnFactorial(nPartitionSize - 1);
			}
		}
		// Cout de structure si attribut symbolique
		else
		{
			// Cout de codage du nombre de parties (forcement 2)
			dAttributeCost = 0;

			// cout de codage du choix du body part
			dAttributeCost += log(2.0);

			// Cout de codage du choix des parties
			require(nValueNumber > 1);
			dAttributeCost += (nValueNumber - 1) * log(2.0);
			if (nValueNumber <= 50)
				dAttributeCost += log(1.0 - 1 / pow(2.0, nValueNumber - 1));
			else
				dAttributeCost += log(1.0);
			//DDD dAttributeCost += KWStat::LnBell(nValueNumber, 2);
			//DDD dAttributeCost += log(pow(2.0, nValueNumber -1) -1);
		}
	}
	return dAttributeCost;
}


double KRRuleRegressionCosts::ComputePartCost(const KRRPart* part) const
{
	require(part != NULL);
	return 0;
}


double KRRuleRegressionCosts::ComputePartUnionCost(const KRRPart* part1, const KRRPart* part2) const
{
	require(part1 != NULL);
	require(part2 != NULL);
	return 0;
}


double KRRuleRegressionCosts::ComputeBodyCost(const KRRule* rule) const
{
	double dBodyCost;
	int nBodyFrequency;
	int nIndex;

	require(rule != NULL);
	//require(bodyPart->GetAttribute()->GetPartAt(bodyPart->GetAttribute()->GetBodyPartIndex()) == part);

	nBodyFrequency = 0;
	dBodyCost = 0;
	// Classification supervisee : fin de prior + likelihood
	if (rule->GetTargetType() == KWType::Symbol)
	{
		for (nIndex = 0; nIndex < rule->GetTargetValueNumber(); nIndex++)
		{
			nBodyFrequency += rule->GetInBodyTargetValueFrequencyAt(nIndex);
			dBodyCost -= KWStat::LnFactorial(rule->GetInBodyTargetValueFrequencyAt(nIndex));
		}

		ensure(nBodyFrequency == rule->GetInBodyTotalFrequency());

		dBodyCost += KWStat::LnFactorial(nBodyFrequency + rule->GetTargetValueNumber() - 1);
		dBodyCost -= KWStat::LnFactorial(rule->GetTargetValueNumber() - 1);
	}

	// Regression : fin de prior + likelihood
	if (rule->GetTargetType() == KWType::Continuous)
	{
		for (nIndex = 0; nIndex < rule->GetTargetAttribute()->GetPartNumber(); nIndex++)
		{
			nBodyFrequency += rule->GetInBodyTargetValueFrequencyAt(nIndex);
			dBodyCost -= KWStat::LnFactorial(rule->GetInBodyTargetValueFrequencyAt(nIndex));
		}

		ensure(nBodyFrequency == rule->GetInBodyTotalFrequency());

		dBodyCost += KWStat::LnFactorial(nBodyFrequency + rule->GetTargetAttribute()->GetPartNumber() - 1);
		dBodyCost -= KWStat::LnFactorial(rule->GetTargetAttribute()->GetPartNumber() - 1);
	}

	return dBodyCost;
}

double KRRuleRegressionCosts::ComputeOutBodyCost(const KRRule* rule) const
{
	double dOutBodyCost;
	int nOutBodyFrequency;
	int nIndex;

	require(rule != NULL);
	//require(bodyPart->GetAttribute()->GetPartAt(bodyPart->GetAttribute()->GetBodyPartIndex()) == part);

	nOutBodyFrequency = rule->GetTotalFrequency() - rule->GetInBodyTotalFrequency();
	dOutBodyCost = 0;

	// Classification supervisée
	if (rule->GetTargetType() == KWType::Symbol)
	{
		for (nIndex = 0; nIndex < rule->GetTargetValueNumber(); nIndex++)
		{
			dOutBodyCost -= KWStat::LnFactorial(rule->GetTargetValueFrequencyAt(nIndex) - rule->GetInBodyTargetValueFrequencyAt(nIndex));
		}
		dOutBodyCost += KWStat::LnFactorial(nOutBodyFrequency + rule->GetTargetValueNumber() - 1);
		dOutBodyCost -= KWStat::LnFactorial(rule->GetTargetValueNumber() - 1);
	}

	// Regression
	if (rule->GetTargetType() == KWType::Continuous)
	{
		for (nIndex = 0; nIndex < rule->GetTargetAttribute()->GetPartNumber(); nIndex++)
		{
			//cout << "TVF : " << rule->GetTargetValueFrequencyAt(nIndex) << " - " << "IBTVF : " << rule->GetInBodyTargetValueFrequencyAt(nIndex) << endl;
			dOutBodyCost -= KWStat::LnFactorial(rule->GetTargetValueFrequencyAt(nIndex) - rule->GetInBodyTargetValueFrequencyAt(nIndex));
		}
		dOutBodyCost += KWStat::LnFactorial(nOutBodyFrequency + rule->GetTargetAttribute()->GetPartNumber() - 1);
		dOutBodyCost -= KWStat::LnFactorial(rule->GetTargetAttribute()->GetPartNumber() - 1);
	}

	return dOutBodyCost;
}


double KRRuleRegressionCosts::ComputeRuleCompressionCoefficient(const KRRule* rule) const
{
	double dTotalCost;

	dTotalCost = ComputeRuleTotalCost(rule);
	if (rule->GetNullCost() != 0)
		return 1 - dTotalCost / rule->GetNullCost();
	else
		return 0;
}

double KRRuleRegressionCosts::ComputeNullRuleCost(const KRRule* rule) const
{
	double dNullRuleCost;

	require(GetTotalAttributeNumber() != 0);
	require(IsInitialized());

	dNullRuleCost = log(GetTotalAttributeNumber() + 1.0)
		+ KWStat::LnFactorial(rule->GetTotalFrequency());

	return dNullRuleCost;
}

double KRRuleRegressionCosts::ComputeRuleTotalCost(const KRRule* rule) const
{
	double dTotalCost;

	//require(CheckRule(rule));

	// Initialisation avec le cout cumulatif du Rule
	dTotalCost = ComputeRuleCumulativeCost(rule);

	// Prise en compte du cout des valeurs
	//dTotalCost += dAllValuesDefaultCost;

	// Calcul du cout par defaut des attributs (et parties) absent d'une grille
	//dTotalCost += ComputeRuleTotalMissingAttributeCost(rule);

	return dTotalCost;
}


double KRRuleRegressionCosts::ComputeRuleBufferizedTotalCost(const KRRule* rule) const
{
	double dTotalCost;
	int nAttribute;
	int nPart;
	KRRAttribute* attribute;
	KRRPart* part;

	//require(CheckRule(rule));

	// Initialisation avec le cout du Rule
	assert(fabs(rule->GetCost() - ComputeRuleCost(rule, rule->GetInformativeAttributeNumber())) < dEpsilon);
	dTotalCost = rule->GetCost();

	// Prise en compte des attributs
	for (nAttribute = 0; nAttribute < rule->GetAttributeNumber(); nAttribute++)
	{
		attribute = rule->GetAttributeAt(nAttribute);
		assert(fabs(attribute->GetCost() - ComputeAttributeCost(attribute, attribute->GetPartNumber())) < dEpsilon);

		// Cout de l'attribut
		dTotalCost += attribute->GetCost();

		// Cout des parties
		for (nPart = 0; nPart < attribute->GetPartNumber(); nPart++)
		{
			part = attribute->GetPartAt(nPart);
			assert(fabs(part->GetCost() - ComputePartCost(part)) < dEpsilon);
			dTotalCost += part->GetCost();
		}
	}

	// Prise en compte du corps et de l'exterieur
	dTotalCost += rule->GetBodyCost();
	dTotalCost += rule->GetOutBodyCost();

	// Prise en compte du cout des valeurs
	//dTotalCost += dAllValuesDefaultCost;

	// Calcul du cout par defaut des attributs (et parties) absent d'une grille
	//dTotalCost += ComputeRuleTotalMissingAttributeCost(ruleMerger);

	return dTotalCost;
}


//boolean KRRuleCosts::CheckRule(const KRRule* rule) const
//{
//	boolean bOk = true;
//	int nDefaultAttribute;
//	KRRAttribute* defaultAttribute;
//	KRRMAttribute* defaultAttributeM;
//	int nAttribute;
//	ALString sAttributeName;
//	boolean bAttributeFound;
//
//	require(rule != NULL);
//
//	// Les couts par defauts doivent être initialises
//	if (not IsInitialized())
//	{
//		AddError("Default data grid costs not initialiszed");
//		bOk = false;
//	}
//
//	// Test de consistance des attributs avec les attributs des couts par defaut
//	// Les attributs sont censes etre dans un ordre coherent
//	nAttribute = 0;
//	if (nAttribute < rule->GetAttributeNumber())
//	{
//		sAttributeName = rule->GetAttributeAt(nAttribute)->GetAttributeName();
//		for (nDefaultAttribute = 0; nDefaultAttribute < ruleDefaultCosts->GetAttributeNumber(); nDefaultAttribute++)
//		{
//			defaultAttribute = ruleDefaultCosts->GetAttributeAt(nDefaultAttribute);
//			defaultAttributeM = cast(KRRMAttribute*, defaultAttribute);
//
//			// Recherche de l'index de l'attribut correspondant parmi les attribut de la grille partielle
//			bAttributeFound = defaultAttribute->GetAttributeName() == sAttributeName;
//
//			// Recherche du prochain nom d'attribut partiel si attribut trouve
//			if (bAttributeFound)
//			{
//				nAttribute++;
//				if (nAttribute < rule->GetAttributeNumber())
//					sAttributeName = rule->GetAttributeAt(nAttribute)->GetAttributeName();
//				else
//					break;
//			}
//			// Erreur si non trouve et plus d'attribut
//			else if (nDefaultAttribute == ruleDefaultCosts->GetAttributeNumber()-1)
//			{
//				AddError("Variable " + sAttributeName + " not found in default data grid");
//				bOk = false;
//				break;
//			}
//		}
//	}
//
//	return bOk;
//}


//void KRRuleCosts::InitializeCosts(const KRRule* rule)
//{
//	boolean bOptimizeMemorySpace = true;
//	KRRuleManager ruleManager;
//	int nAttribute;
//	KRRAttribute* defaultAttribute;
//	KRRPart* defaultPart;
//
//	require(rule != NULL);
//	require(rule->Check());
//
//	// Nettoyage prealable
//	CleanCosts();
//
//	// Creation du Rule des couts par defaut
//	ruleDefaultCosts = new KRRuleMerger;
//	ruleDefaultCosts->SetRuleCosts(this);
//
//	// Creation d'une grille par default correspondant a la grille source
//	ruleManager.SetSourceRule(rule);
//	ruleManager.ExportTerminalRule(ruleDefaultCosts);
//
//	// Initialisation des couts par defaut par entite
//	ruleDefaultCosts->InitializeAllCosts();
//
//	// Memorisation des couts des valeurs symboliques
//	dAllValuesDefaultCost = ComputeRuleAllValuesCost(ruleDefaultCosts);
//
//	// Memorisation du cout total
//	dTotalDefaultCost = ComputeRuleMergerTotalCost(ruleDefaultCosts);
//
//	// Optimisation de la place memoire, en remplacant dans chaque attribut symbolique
//	// l'ensemble des valeurs par une seule valeur "*"
//	// Initialisation des attributs avec une seule partie
//	if (bOptimizeMemorySpace)
//	{
//		for (nAttribute = 0; nAttribute < ruleDefaultCosts->GetAttributeNumber(); nAttribute++)
//		{
//			// Recherche de l'attribut default
//			defaultAttribute = ruleDefaultCosts->GetAttributeAt(nAttribute);
//
//			// Modification de l'ensemble des valeurs des attributs symbolique
//			if (defaultAttribute->GetAttributeType() == KWType::Symbol)
//			{
//				// Acces a la partie unique de l'attribut
//				assert(defaultAttribute->GetPartNumber() == 1);
//				defaultPart = defaultAttribute->GetHeadPart();
//
//				// Remplacement de ses valeurs par une valeur unique
//				defaultPart->GetValueSet()->CompressValueSet();
//			}
//		}
//	}
//	ensure(ruleManager.CheckRule(ruleDefaultCosts));
//}
//
//
//void KRRuleCosts::CleanCosts()
//{
//}
//
//
boolean KRRuleRegressionCosts::IsInitialized() const
{
	return classStats != NULL;
}


int KRRuleRegressionCosts::GetTotalAttributeNumber() const
{
	//require(IsInitialized());
	return GetClassStats()->GetClass()->GetAttributeNumber() - 1;
}


//double KRRuleCosts::GetTotalDefaultCost() const
//{
//	require(IsInitialized());
//	return dTotalDefaultCost;
//}


//const ALString& KRRuleCosts::GetAttributeNameAt(int nIndex) const
//{
//	require(IsInitialized());
//	require(0 <= nIndex and nIndex < GetTotalAttributeNumber());
//	return krrRule->GetAttributeAt(nIndex)->GetAttributeName();
//}


//double KRRuleCosts::GetAttributeDefaultCostAt(int nIndex) const
//{
//	double dDefaultCost;
//	KRRMAttribute* defaultAttributeM;
//	KRRMPart* defaultPartM;
//
//	require(IsInitialized());
//	require(0 <= nIndex and nIndex < GetTotalAttributeNumber());
//
//	// Acces a l'attribut
//	defaultAttributeM = cast(KRRMAttribute*, ruleDefaultCosts->GetAttributeAt(nIndex));
//	assert(fabs(defaultAttributeM->GetCost() - ComputeAttributeCost(defaultAttributeM, 1)) < dEpsilon);
//	dDefaultCost = defaultAttributeM->GetCost();
//
//	// Cout de la partie
//	assert(defaultAttributeM->GetPartNumber() == 1);
//	defaultPartM = cast(KRRMPart*, defaultAttributeM->GetHeadPart());
//	assert(fabs(defaultPartM->GetCost() - ComputePartCost(defaultPartM)) < 1e-5);
//	dDefaultCost += defaultPartM->GetCost();
//
//	return dDefaultCost;
//}


//double KRRuleCosts::GetAllValuesDefaultCost() const
//{
//	require(IsInitialized());
//	return dAllValuesDefaultCost;
//}
//
//
//double KRRuleCosts::ComputeRuleTotalMissingAttributeCost(const KRRule* rule) const
//{
//	double dTotalCost;
//	int nDefaultAttribute;
//	KRRAttribute* defaultAttribute;
//	KRRPart* part;
//	KRRMAttribute* defaultAttributeM;
//	KRRMPart* partM;
//	int nAttribute;
//	ALString sAttributeName;
//	int nUnusedAttributeNumber;
//	boolean bAttributeFound;
//	KRRAttribute* targetAttribute;
//	KRRAttribute* defaultTargetAttribute;
//
//	require(CheckRule(rule));
//
//	// Recherche des attributs cibles courant et par defaut
//	// Le cout par defaut se rapporte en effet a un attribut cible ayant une seule partie,
//	// alors qu'un attribut source non informatif peut disparaitre. Dans ce cas, le cout
//	// par defaut de ce dernier doit etre recalcule avec le bon nombre de partie cible
//	targetAttribute = rule->GetTargetAttribute();
//	defaultTargetAttribute = ruleDefaultCosts->GetTargetAttribute();
//	assert(defaultTargetAttribute == NULL or defaultTargetAttribute->GetPartNumber() == 1);
//
//	// Actualisation du bon nombre de partie cible pour la grille par defaut
//	if (defaultTargetAttribute != NULL and targetAttribute != NULL and
//		targetAttribute->GetPartNumber() > 1)
//	{
//		while (defaultTargetAttribute->GetPartNumber() < targetAttribute->GetPartNumber())
//			defaultTargetAttribute->AddPart();
//	}
//
//	// Recherche des attributs par defaut ne se trouvant pas dans la grille a evaluer
//	// Les attributs sont censes etre dans un ordre coherent
//	dTotalCost = 0;
//	nUnusedAttributeNumber = 0;
//	nAttribute = 0;
//	if (nAttribute < rule->GetAttributeNumber())
//		sAttributeName = rule->GetAttributeAt(nAttribute)->GetAttributeName();
//	for (nDefaultAttribute = 0; nDefaultAttribute < ruleDefaultCosts->GetAttributeNumber(); nDefaultAttribute++)
//	{
//		defaultAttribute = ruleDefaultCosts->GetAttributeAt(nDefaultAttribute);
//		defaultAttributeM = cast(KRRMAttribute*, defaultAttribute);
//
//		// Recherche de l'index de l'attribut correspondant parmi les attribut de la grille partielle
//		bAttributeFound = defaultAttribute->GetAttributeName() == sAttributeName;
//
//		// Prise en compte du cout de l'attribut terminal s'il n'est pas utilise dans la grille partielle
//		if (not bAttributeFound)
//		{
//			nUnusedAttributeNumber++;
//
//			// Cout par defaut si une seule partie cibme
//			if (defaultTargetAttribute == NULL or defaultTargetAttribute->GetPartNumber() == 1)
//			{
//				// Cout de l'attribut par defaut
//				dTotalCost += defaultAttributeM->GetCost();
//
//				// Cout de la partie
//				assert(defaultAttribute->GetPartNumber() == 1);
//				part = defaultAttribute->GetHeadPart();
//				partM = cast(KRRMPart*, part);
//				dTotalCost += partM->GetCost();
//			}
//			// Sinon, on recalcul les couts des attributs manquants pour tenir compte du nombre
//			// de parties cibles
//			else
//			{
//				// Cout de l'attribut par defaut
//				assert(defaultAttributeM->GetPartNumber() == 1);
//				dTotalCost += ComputeAttributeCost(defaultAttributeM, 1);
//
//				// Cout de la partie
//				assert(defaultAttribute->GetPartNumber() == 1);
//				part = defaultAttribute->GetHeadPart();
//				partM = cast(KRRMPart*, part);
//				dTotalCost += ComputePartCost(partM);
//			}
//		}
//		// Sinon, recherche du prochain nom d'attribut partiel
//		else
//		{
//			nAttribute++;
//			if (nAttribute < rule->GetAttributeNumber())
//				sAttributeName = rule->GetAttributeAt(nAttribute)->GetAttributeName();
//			else
//				sAttributeName = "";
//		}
//	}
//	assert(ruleDefaultCosts->GetAttributeNumber() == rule->GetAttributeNumber() + nUnusedAttributeNumber);
//
//	// Reactualisation a une seule partie cible pour la grille par defaut
//	if (defaultTargetAttribute != NULL and targetAttribute != NULL and
//		targetAttribute->GetPartNumber() > 1)
//	{
//		while (defaultTargetAttribute->GetPartNumber() > 1)
//		{
//			part = defaultTargetAttribute->GetTailPart();
//			defaultTargetAttribute->DeletePart(part);
//		}
//	}
//	assert(defaultTargetAttribute == NULL or defaultTargetAttribute->GetPartNumber() == 1);
//
//	return dTotalCost;
//}


double KRRuleRegressionCosts::ComputeRuleCumulativeCost(const KRRule* rule) const
{
	double dGlobalCost = 0;
	int nAttribute;
	KRRAttribute* attribute;

	require(rule != NULL);

	// Initialisation avec le cout du Rule
	dGlobalCost += ComputeRuleCost(rule, rule->GetInformativeAttributeNumber());

	// Prise en compte des attributs
	for (nAttribute = 0; nAttribute < rule->GetAttributeNumber(); nAttribute++)
	{
		attribute = rule->GetAttributeAt(nAttribute);

		// Cout de l'attribut
		dGlobalCost += ComputeAttributeCost(attribute, attribute->GetPartNumber());

		// Cout des parties
		/*for (nPart = 0; nPart < attribute->GetPartNumber(); nPart++)
		{
			part = attribute->GetPartAt(nPart);
			dGlobalCost += ComputePartCost(part);
		}*/
	}

	// Prise en compte du corps et de l'exterieur
	dGlobalCost += ComputeBodyCost(rule);
	dGlobalCost += ComputeOutBodyCost(rule);

	return dGlobalCost;
}


double KRRuleRegressionCosts::ComputeAttributeCumulativeCost(const KRRAttribute* attribute) const
{
	double dGlobalValue;
	KRRPart* part;
	int nPart;

	require(attribute != NULL);

	// Cout de l'attribut
	dGlobalValue = ComputeAttributeCost(attribute, attribute->GetPartNumber());

	// Cout globale des parties
	for (nPart = 0; nPart < attribute->GetPartNumber(); nPart++)
	{
		part = attribute->GetPartAt(nPart);
		dGlobalValue += ComputePartCumulativeCost(part);
	}

	return dGlobalValue;
}


double KRRuleRegressionCosts::ComputePartCumulativeCost(const KRRPart* part) const
{
	double dGlobalValue;

	require(part != NULL);

	// Valeur de la partie
	dGlobalValue = ComputePartCost(part);

	return dGlobalValue;
}

const ALString KRRuleRegressionCosts::GetClassLabel() const
{
	return "Rule Regression costs";
}
