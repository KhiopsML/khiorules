// Copyright (c) 2023-2025 Orange. All rights reserved.
// This software is distributed under the BSD 3-Clause-clear License, the text of which is available
// at https://spdx.org/licenses/BSD-3-Clause-Clear.html or see the "LICENSE" file for more details.

#include "KRRule.h"

//////////////////////////////////////////////////////////////////////////////////////////
// Classe KRRule

KRRule::KRRule()
{
	nInformativeAttributeNumber = 0;
	targetAttribute = NULL;
	targetType = KWType::Unknown;
	nTotalFrequency = 0;
	dCost = 0;
	dBodyCost = 0;
	dOutBodyCost = 0;
	dNullCost = 0;
}

KRRule::~KRRule()
{
	DeleteAll();
}

void KRRule::Initialize(int nAttributeNumber, int nTargetType, int nTargetValueNumber)
{
	int nAttribute;
	KRRAttribute* attribute;
	require(0 <= nAttributeNumber);
	require(0 <= nTargetValueNumber); // 0 dans le cas de la regression ou du groupement de valeurs de classes

	// Nettoyage initial
	DeleteAll();

	// Creation du tableau d'attributs et de son contenu
	targetAttribute = NULL;
	targetType = nTargetType;
	oaAttributes.SetSize(nAttributeNumber);
	//cout << "oaAttributes Size : " << oaAttributes.GetSize() << endl;
	for (nAttribute = 0; nAttribute < nAttributeNumber; nAttribute++)
	{
		// creation de l'attribut dans le tableau
		attribute = new KRRAttribute;
		oaAttributes.SetAt(nAttribute, attribute);

		if (nAttribute == nAttributeNumber - 1 and GetTargetType() == KWType::Continuous)
		{
			SetTargetAttribute(attribute);
			attribute->SetAttributeTargetFunction(true);
		}

		// Initialisation de l'attribut pour le mettre dans un etat coherent
		attribute->rule = this;
		attribute->nAttributeIndex = nAttribute;
	}

	// Creation des vecteurs d'effectifs et de classes cibles
	ivTargetValueFrequencies.SetSize(nTargetValueNumber);
	ivInBodyTargetValueFrequencies.SetSize(nTargetValueNumber);
	svTargetValues.SetSize(nTargetValueNumber);
}

int KRRule::GetAttributeNumber() const
{
	return oaAttributes.GetSize();
}

int KRRule::GetTargetValueNumber() const
{
	return svTargetValues.GetSize();
}

boolean KRRule::IsEmpty() const
{
	return GetAttributeNumber() == 0;
}

void KRRule::SetTargetValueAt(int nIndex, Symbol sValue)
{
	// require(GetTargetAttribute()->GetAttributeType() == KWType::Symbol);
	require(0 <= nIndex and nIndex < GetTargetValueNumber());

	svTargetValues.SetAt(nIndex, sValue);
}

Symbol KRRule::GetTargetValueAt(int nIndex) const
{
	// require(GetTargetAttribute()->GetAttributeType() == KWType::Symbol);
	require(0 <= nIndex and nIndex < GetTargetValueNumber());

	return svTargetValues.GetAt(nIndex);
}

int KRRule::ComputeTargetValueIndex(Symbol sValue) const
{
	require(GetTargetType() == KWType::Symbol);
	int nIndex;
	bool bFound;

	bFound = false;
	for (nIndex = 0; nIndex < svTargetValues.GetSize(); nIndex++)
	{
		if (svTargetValues.GetAt(nIndex) == sValue)
		{
			bFound = true;
			break;
		}
	}
	if (bFound)
		return nIndex;
	else
		return -1;
}

int KRRule::ComputeContinuousTargetValuePartIndex(Continuous cValue) const
{
	require(GetTargetType() == KWType::Continuous);

	KRRAttribute* krrAttribute;
	int nIndex;
	bool bFound;

	krrAttribute = GetTargetAttribute();
	require(krrAttribute != NULL);

	bFound = false;
	for (nIndex = 0; nIndex < krrAttribute->GetPartNumber(); nIndex++)
	{
		//if (krrAttribute->GetPartAt(nIndex)->GetInterval()->CheckValue(cValue))
		if (cValue <= krrAttribute->GetPartAt(nIndex)->GetInterval()->GetUpperBound())
		{
			bFound = true;
			break;
		}
	}
	if (bFound)
		return nIndex;
	else
		return -1;
}

KRRAttribute* KRRule::GetAttributeAt(int nAttributeIndex) const
{
	require(0 <= nAttributeIndex and nAttributeIndex < GetAttributeNumber());
	ensure(oaAttributes.GetAt(nAttributeIndex) != NULL);

	return cast(KRRAttribute*, oaAttributes.GetAt(nAttributeIndex));
}

KRRAttribute* KRRule::SearchAttribute(const ALString& sAttributeName) const
{
	KRRAttribute* attribute;
	int nAttributeIndex;

	// Parcours des attributs
	for (nAttributeIndex = 0; nAttributeIndex < GetAttributeNumber(); nAttributeIndex++)
	{
		attribute = GetAttributeAt(nAttributeIndex);
		if (attribute->GetAttributeName() == sAttributeName)
			return attribute;
	}
	return NULL;
}

KRRAttribute* KRRule::GetTargetAttribute() const
{
	require(GetTargetValueNumber() == 0);
	return targetAttribute;
}

int KRRule::GetTargetType() const
{
	return targetType;
}

boolean KRRule::IsTargetPartitioned() const
{
	return GetTargetAttribute()->GetAttributeType() == KWType::Symbol and GetTargetValueNumber() == 0;
}

void KRRule::SetTargetValueFrequencyAt(int nIndex, int nValue)
{
	ivTargetValueFrequencies.SetAt(nIndex, nValue);
}

int KRRule::GetTargetValueFrequencyAt(int nIndex) const
{
	return ivTargetValueFrequencies.GetAt(nIndex);
}

void KRRule::SetInBodyTargetValueFrequencyAt(int nIndex, int nValue)
{
	ivInBodyTargetValueFrequencies.SetAt(nIndex, nValue);
}

int KRRule::GetInBodyTargetValueFrequencyAt(int nIndex) const
{
	return ivInBodyTargetValueFrequencies.GetAt(nIndex);
}

int KRRule::GetInBodyTotalFrequency() const
{
	int nIndex;
	int nTotal;

	nTotal = 0;
	for (nIndex = 0; nIndex < ivInBodyTargetValueFrequencies.GetSize(); nIndex++)
	{
		require(ivInBodyTargetValueFrequencies.GetAt(nIndex) >= 0);
		nTotal += ivInBodyTargetValueFrequencies.GetAt(nIndex);
	}
	return nTotal;
}

void KRRule::UpdateTargetVectorsFrequency()
{
	int nIndex;

	ivTargetValueFrequencies.SetSize(GetTargetAttribute()->GetPartNumber());
	ivInBodyTargetValueFrequencies.SetSize(GetTargetAttribute()->GetPartNumber());

	for (nIndex = 0; nIndex < ivInBodyTargetValueFrequencies.GetSize(); nIndex++)
	{
		SetTargetValueFrequencyAt(nIndex, 0);
		SetInBodyTargetValueFrequencyAt(nIndex, 0);
	}
	ensure(ivTargetValueFrequencies.GetSize() == GetTargetAttribute()->GetPartNumber() and ivInBodyTargetValueFrequencies.GetSize() == GetTargetAttribute()->GetPartNumber());
}

boolean KRRule::IsTargetVectorSizeValid() const
{
	return ivTargetValueFrequencies.GetSize() == GetTargetValueNumber() and ivInBodyTargetValueFrequencies.GetSize() == GetTargetValueNumber();
	//DDD MB return ivTargetValueFrequencies.GetSize() == GetTargetAttribute()->GetPartNumber() and ivInBodyTargetValueFrequencies.GetSize() == GetTargetAttribute()->GetPartNumber();
}

void KRRule::SetTotalFrequency(int nValue)
{
	nTotalFrequency = nValue;
}

int KRRule::GetTotalFrequency() const
{
	return nTotalFrequency;
}

void KRRule::SetCost(double dValue)
{
	dCost = dValue;
}

double KRRule::GetCost() const
{
	return dCost;
}

void KRRule::SetBodyCost(double dValue)
{
	dBodyCost = dValue;
}

double KRRule::GetBodyCost() const
{
	return dBodyCost;
}

void KRRule::SetOutBodyCost(double dValue)
{
	dOutBodyCost = dValue;
}

double KRRule::GetOutBodyCost() const
{
	return dOutBodyCost;
}

void KRRule::SetNullCost(double dValue)
{
	dNullCost = dValue;
}

double KRRule::GetNullCost() const
{
	return dNullCost;
}

double KRRule::GetTotalCost() const
{
	double dTotalCost;
	int nAttribute;
	KRRAttribute* attribute;

	dTotalCost = 0;
	// cout local de la regle
	dTotalCost += GetCost();

	// cout local des attibuts et parties
	for (nAttribute = 0; nAttribute < GetAttributeNumber(); nAttribute++)
	{
		attribute = GetAttributeAt(nAttribute);

		// Cout de l'attribut
		dTotalCost += attribute->GetCost();

		//// Cout des parties
		//for (nPart = 0; nPart < attribute->GetPartNumber(); nPart++)
		//{
		//	part = attribute->GetPartAt(nPart);
		//	dTotalCost += part->GetCost();
		//}
	}

	// Prise en compte du corps et de l'exterieur
	dTotalCost += GetBodyCost();
	dTotalCost += GetOutBodyCost();

	return dTotalCost;
}

double KRRule::GetLevel() const
{
	double dTotalCost;

	dTotalCost = GetTotalCost();
	if (GetNullCost() != 0)
		return 1 - dTotalCost / GetNullCost();
	else
		return 0;
}

//double KRRule::GetLevel() const
//{
//	return dLevel;
//}

//void KRRule::SetRuleCosts(const KRRuleCosts* krrCosts)
//{
//	ruleCosts = krrCosts;
//}
//
//const KRRuleCosts* KRRule::GetRuleCosts() const
//{
//	return ruleCosts;
//}

void KRRule::InitializeAllCosts(const KRRuleCosts* krrCosts)
{
	int nAttribute;
	KRRAttribute* attribute;
	int nPart;
	KRRPart* part;

	require(krrCosts != NULL);
	//require(GetInformativeAttributeNumber() == GetAttributeNumber());
	// Initialisation du cout du rule
	SetCost(krrCosts->ComputeRuleCost(this, GetInformativeAttributeNumber()));

	// Prise en compte des attributs
	for (nAttribute = 0; nAttribute < GetAttributeNumber(); nAttribute++)
	{
		attribute = GetAttributeAt(nAttribute);

		// cout de l'attribut
		attribute->SetCost(krrCosts->ComputeAttributeCost(attribute, attribute->GetPartNumber()));

		// cout des parties
		for (nPart = 0; nPart < attribute->GetPartNumber(); nPart++)
		{
			part = attribute->GetPartAt(nPart);
			part->SetCost(krrCosts->ComputePartCost(part));
		}
	}

	// Prise en compte du corps et de l'exterieur
	SetBodyCost(krrCosts->ComputeBodyCost(this));
	SetOutBodyCost(krrCosts->ComputeOutBodyCost(this));

	SetNullCost(krrCosts->ComputeNullRuleCost(this));
}

boolean KRRule::CheckAllCosts(const KRRuleCosts* krrCosts)
{
	boolean bOk = true;
	ALString sTmp;
	int nAttribute;
	KRRAttribute* attribute;
	int nPart;
	KRRPart* part;

	require(krrCosts != NULL);

	// Check du cout local de la rule
	if (GetCost() != krrCosts->ComputeRuleCost(this, GetInformativeAttributeNumber()))
	{
		AddError(sTmp + "Rule Cost (" + DoubleToString(GetCost()) + ") is incorrect");
		bOk = false;
	}

	// Check des attributs
	for (nAttribute = 0; nAttribute < GetAttributeNumber(); nAttribute++)
	{
		attribute = GetAttributeAt(nAttribute);

		// cout de l'attribut
		if (attribute->GetCost() != krrCosts->ComputeAttributeCost(attribute, attribute->GetPartNumber()))
		{
			attribute->AddError(sTmp + "Attribute cost (" + DoubleToString(attribute->GetCost()) + ") is incorrect");
			bOk = false;
		}

		// cout des parties
		for (nPart = 0; nPart < attribute->GetPartNumber(); nPart++)
		{
			part = attribute->GetPartAt(nPart);
			if (part->GetCost() != krrCosts->ComputePartCost(part))
			{
				part->AddError(sTmp + "Part cost (" + DoubleToString(part->GetCost()) + ") is incorrect");
				bOk = false;
				break;
			}
		}

		// arret si incoherence dans les couts
		if (not bOk)
			break;
	}

	// Check du corps et de l'exterieur
	if (GetBodyCost() != krrCosts->ComputeBodyCost(this))
	{
		AddError(sTmp + "Body cost (" + DoubleToString(GetBodyCost()) + ") is incorrect");
		bOk = false;
	}
	if (GetOutBodyCost() != krrCosts->ComputeOutBodyCost(this))
	{
		AddError(sTmp + "Out-Body cost (" + DoubleToString(GetOutBodyCost()) + ") is incorrect");
		bOk = false;
	}

	// check du coup total de la rule
	if (GetTotalCost() != krrCosts->ComputeRuleTotalCost(this))
	{
		AddError(sTmp + "Total rule cost (" + DoubleToString(GetTotalCost()) + ") is incorrect");
	}

	return bOk;
}

void KRRule::DeleteNonInformativeAttributes()
{
	int nAttribute;
	KRRAttribute* attribute;
	//int nInformativeAttributeNumber;

	// Parcours des attributs et destruction des attributs non informatifs
	nInformativeAttributeNumber = 0;
	for (nAttribute = 0; nAttribute < oaAttributes.GetSize(); nAttribute++)
	{
		attribute = cast(KRRAttribute*, oaAttributes.GetAt(nAttribute));
		//cout << attribute->GetAttributeName() << " : " << attribute->GetPartNumber() << " parts" << endl;
		assert(attribute->GetPartNumber() >= 1);

		// Supression de l'attribut s'il n'est pas informatif
		if (attribute->GetPartNumber() <= 1 and (not attribute->GetAttributeTargetFunction()))
			delete attribute;
		// Sinon, on le range a son nouvel index dans le tableau des attributs
		else
		{
			attribute->nAttributeIndex = nInformativeAttributeNumber;
			oaAttributes.SetAt(attribute->nAttributeIndex, attribute);
			nInformativeAttributeNumber++;
		}
	}
	oaAttributes.SetSize(nInformativeAttributeNumber);
}

int KRRule::GetInformativeAttributeNumber() const
{
	//require(nInformativeAttributeNumber == ComputeInformativeAttributeNumber());
	return nInformativeAttributeNumber;
}

int KRRule::ComputeInformativeAttributeNumber()
{
	int nResult;
	int nAttribute;
	KRRAttribute* attribute;

	// Calcul par parcours des attributs
	nResult = 0;
	for (nAttribute = 0; nAttribute < GetAttributeNumber(); nAttribute++)
	{
		attribute = cast(KRRAttribute*, oaAttributes.GetAt(nAttribute));
		if (attribute->GetPartNumber() > 1 and (not attribute->GetAttributeTargetFunction()))
			nResult++;
	}
	nInformativeAttributeNumber = nResult;
	return nResult;
}

void KRRule::DeleteAll()
{
	// Destruction des attributs
	oaAttributes.DeleteAll();
	oaAttributes.SetSize(0);

	// Destruction des valeurs cibles et frequences
	svTargetValues.SetSize(0);
	ivInBodyTargetValueFrequencies.SetSize(0);
	ivTargetValueFrequencies.SetSize(0);

	// Reinit pour le mode debug
	//debug(targetAttribute = NULL);
}

void KRRule::Write(ostream& ost) const
{
	// Identification
	ost << GetClassLabel() << "\t" << GetObjectLabel() << "\n";
	//WriteAttributes(ost);
	WriteAttributeParts(ost);
	if (GetTargetType() == KWType::Symbol)
		WriteTargetValues(ost);
	if (GetTargetType() == KWType::Continuous)
		WriteTargetIntervals(ost);
	WriteContingencyTable(ost);
	WriteLevel(ost);
}

void KRRule::WriteTargetValues(ostream& ost) const
{
	require(GetTargetType() == KWType::Symbol);
	int nTargetIndex;

	// Liste des valeurs cibles
	ost << "Target values"
	    << "\t" << GetTargetValueNumber() << "\n";
	for (nTargetIndex = 0; nTargetIndex < GetTargetValueNumber(); nTargetIndex++)
		ost << "\t" << svTargetValues.GetAt(nTargetIndex);
	ost << endl;
}

void KRRule::WriteTargetIntervals(ostream& ost) const
{
	require(GetTargetType() == KWType::Continuous);
	int nTargetIndex;

	// Liste des intervalles de la variable cible numériue
	ost << "Target intervals"
	    << "\t" << GetTargetAttribute()->GetPartNumber() << "\n";
	for (nTargetIndex = 0; nTargetIndex < GetTargetAttribute()->GetPartNumber(); nTargetIndex++)
		ost << "\t" << GetTargetAttribute()->GetPartAt(nTargetIndex)->GetObjectLabel();
	ost << endl;
}

void KRRule::WriteAttributes(ostream& ost) const
{
	int nAttribute;
	KRRAttribute* attribute;

	// Liste des attributs
	ost << "Variables"
	    << "\t" << GetAttributeNumber() << "\n";
	for (nAttribute = 0; nAttribute < GetAttributeNumber(); nAttribute++)
	{
		attribute = cast(KRRAttribute*, oaAttributes.GetAt(nAttribute));
		ost << "\t" << attribute->GetAttributeName() << "\t" << KWType::ToString(attribute->GetAttributeType()) << "\n";
	}
}

void KRRule::WriteAttributeParts(ostream& ost) const
{
	int nAttribute;
	int nPart;
	KRRAttribute* attribute;
	KRRPart* part;
	const int nMaxDisplayedValue = 20;
	int nDisplayedValue;
	KRRValue* value;

	// Liste des attributs et de leurs parties
	for (nAttribute = 0; nAttribute < GetAttributeNumber(); nAttribute++)
	{
		attribute = cast(KRRAttribute*, oaAttributes.GetAt(nAttribute));
		ost << "\t" << attribute->GetAttributeName() << "\t" << attribute->GetPartNumber() << "\n";

		// Parties de l'attribut
		for (nPart = 0; nPart < attribute->GetPartNumber(); nPart++)
		{
			part = attribute->GetPartAt(nPart);
			ost << "\t\t" << part->GetObjectLabel();
			if (attribute->GetBodyPartIndex() == nPart)
				ost << "*";

			// Affichage des premiere valeurs dans le cas d'un attribut Symbol
			if (attribute->GetAttributeType() == KWType::Symbol)
			{
				nDisplayedValue = 0;
				value = part->GetValueSet()->GetHeadValue();
				while (value != NULL)
				{
					nDisplayedValue++;
					if (nDisplayedValue > nMaxDisplayedValue)
					{
						ost << "\t...";
						break;
					}
					else
						ost << "\t" << value->GetValue();
					part->GetValueSet()->GetNextValue(value);
				}
			}

			// Fin de ligne
			ost << "\n";
		}
	}
}

void KRRule::WriteLevel(ostream& ost) const
{
	ost << "\nRule Level : " << GetLevel() << endl;
}

void KRRule::WriteContingencyTable(ostream& ost) const
{
	require(IsTargetVectorSizeValid());
	require(not IsEmpty());
	IntVector ivOutBodyTargetValueFrequencies;
	int nTarget;
	int nTargetIndex;

	//Creation frequences hors du corps
	if (GetTargetType() == KWType::Symbol)
	{
		ivOutBodyTargetValueFrequencies.SetSize(GetTargetValueNumber());
		ivOutBodyTargetValueFrequencies.Initialize();
		for (nTarget = 0; nTarget < GetTargetValueNumber(); nTarget++)
		{
			ivOutBodyTargetValueFrequencies.SetAt(nTarget,
							      GetTargetValueFrequencyAt(nTarget) - GetInBodyTargetValueFrequencyAt(nTarget));
		}
	}
	if (GetTargetType() == KWType::Continuous)
	{
		ivOutBodyTargetValueFrequencies.SetSize(GetTargetAttribute()->GetPartNumber());
		ivOutBodyTargetValueFrequencies.Initialize();
		for (nTarget = 0; nTarget < GetTargetAttribute()->GetPartNumber(); nTarget++)
		{
			ivOutBodyTargetValueFrequencies.SetAt(nTarget,
							      GetTargetValueFrequencyAt(nTarget) - GetInBodyTargetValueFrequencyAt(nTarget));
		}
	}

	// Affichage des valeurs ou intervalles cibles
	if (GetTargetType() == KWType::Symbol)
	{
		for (nTarget = 0; nTarget < GetTargetValueNumber(); nTarget++)
			ost << "\t" << GetTargetValueAt(nTarget);
	}
	if (GetTargetType() == KWType::Continuous)
	{
		for (nTargetIndex = 0; nTargetIndex < GetTargetAttribute()->GetPartNumber(); nTargetIndex++)
			ost << "\t" << GetTargetAttribute()->GetPartAt(nTargetIndex)->GetObjectLabel();
	}
	ost << "\tFrequency\tCoverage\n";

	//Table de contingence body et outbody
	ost << "Body";
	for (nTarget = 0; nTarget < ivOutBodyTargetValueFrequencies.GetSize(); nTarget++)
		ost << "\t" << 1.0 * GetInBodyTargetValueFrequencyAt(nTarget) / GetInBodyTotalFrequency();
	ost << "\t" << GetInBodyTotalFrequency() << "\t"
	    << 1.0 * GetInBodyTotalFrequency() / GetTotalFrequency() << "\n";

	ost << "OutBody";
	for (nTarget = 0; nTarget < ivOutBodyTargetValueFrequencies.GetSize(); nTarget++)
		if (GetTotalFrequency() == GetInBodyTotalFrequency())
			ost << "\t" << 0;
		else
			ost << "\t" << 1.0 * ivOutBodyTargetValueFrequencies.GetAt(nTarget) / (GetTotalFrequency() - GetInBodyTotalFrequency());
	ost << "\t" << GetTotalFrequency() - GetInBodyTotalFrequency() << "\t" << 1.0 - (1.0 * GetInBodyTotalFrequency() / GetTotalFrequency()) << "\n";

	//distribution des classes
	for (nTarget = 0; nTarget < ivOutBodyTargetValueFrequencies.GetSize(); nTarget++)
		ost << "\t" << GetTargetValueFrequencyAt(nTarget);
	ost << "\t" << GetTotalFrequency() << "\t" << 1 << "\n";
}

KRRule* KRRule::Create() const
{
	return NULL;
}

void KRRule::CopyFrom(const KRRule* krrSource)
{
	require(krrSource != NULL);

	int nIndex;

	// copie des proprietes
	Initialize(krrSource->GetAttributeNumber(), krrSource->GetTargetType(), krrSource->GetTargetValueNumber());
	nTotalFrequency = krrSource->GetTotalFrequency();
	dCost = krrSource->GetCost();
	dBodyCost = krrSource->GetBodyCost();
	dOutBodyCost = krrSource->GetOutBodyCost();
	dNullCost = krrSource->GetNullCost();
	nInformativeAttributeNumber = krrSource->GetInformativeAttributeNumber();

	// copie des attributs de la regle
	for (nIndex = 0; nIndex < krrSource->GetAttributeNumber(); nIndex++)
	{
		GetAttributeAt(nIndex)->CopyFrom(krrSource->GetAttributeAt(nIndex));
	}

	// copie des vecteurs
	ivTargetValueFrequencies.CopyFrom(&krrSource->ivTargetValueFrequencies);
	ivInBodyTargetValueFrequencies.CopyFrom(&krrSource->ivInBodyTargetValueFrequencies);
	svTargetValues.CopyFrom(&krrSource->svTargetValues);
	/*DDD
	for (nIndex = 0; nIndex < GetTargetValueNumber(); nIndex++)
	{
		ivTargetValueFrequencies.SetAt(nIndex, krrSource->GetTargetValueFrequencyAt(nIndex));
		ivInBodyTargetValueFrequencies.SetAt(nIndex, krrSource->GetInBodyTargetValueFrequencyAt(nIndex)) ;
		svTargetValues.SetAt(nIndex, krrSource->GetTargetValueAt(nIndex));
	}
	*/
}

KRRule* KRRule::Clone() const
{
	KRRule* krrClone;

	krrClone = new KRRule;
	krrClone->CopyFrom(this);
	return krrClone;
}

const ALString KRRule::GetClassLabel() const
{
	return "Rule";
}

const ALString KRRule::GetObjectLabel() const
{
	ALString sTmp;

	// Libelle base sur le nombre de valeurs, d'attributs
	return sTmp + "(J=" + IntToString(GetTargetValueNumber()) +
	       ", k=" + IntToString(GetAttributeNumber()) + ")";
}

void KRRule::SetTargetAttribute(KRRAttribute* attribute)
{
	targetAttribute = attribute;
}

/////////////////////////////////////////////////////
// KRRAttribute

KRRAttribute::KRRAttribute()
{
	rule = NULL;
	nAttributeType = KWType::Unknown;
	nAttributeIndex = -1;
	nValueNumber = 0;
	nBodyPartIndex = -1;
	bIsIndexed = false;
	starValuePart = NULL;
	dCost = 0;
	bTargetAttribute = false;
}

KRRAttribute::~KRRAttribute()
{
	// Destruction de tous les parties
	DeleteAllParts();

	// Reinitialisation en mode debug, pour faciliter le diagnostique
	debug(rule = NULL);
	debug(nAttributeType = KWType::Unknown);
	debug(nAttributeIndex = -1);
	debug(nValueNumber = 0);
	debug(bIsIndexed = false);
	debug(dCost = 0);
}

KRRule* KRRAttribute::GetRule() const
{
	return rule;
}

void KRRAttribute::SetAttributeName(const ALString& sValue)
{
	sAttributeName = sValue;
}

const ALString& KRRAttribute::GetAttributeName() const
{
	return sAttributeName;
}

void KRRAttribute::SetAttributeType(int nValue)
{
	require(nAttributeType == KWType::Unknown);
	require(KWType::IsSimple(nValue));
	nAttributeType = nValue;
}

int KRRAttribute::GetAttributeType() const
{
	return nAttributeType;
}

void KRRAttribute::SetAttributeTargetFunction(boolean bValue)
{
	bTargetAttribute = bValue;

	// Memorisation d'un pointeur vers l'attribut cible si c'est le cas
	if (bValue and GetRule() != NULL)
	{
		GetRule()->SetTargetAttribute(this);
	}
}

boolean KRRAttribute::GetAttributeTargetFunction() const
{
	return bTargetAttribute;
}

int KRRAttribute::GetAttributeIndex() const
{
	return nAttributeIndex;
}

void KRRAttribute::SetAttributeIndex(int nValue)
{
	nAttributeIndex = nValue;
}

void KRRAttribute::SetValueNumber(int nValue)
{
	require(nValue >= 0);
	nValueNumber = nValue;
}

int KRRAttribute::GetValueNumber() const
{
	return nValueNumber;
}

void KRRAttribute::SetBodyPartIndex(int nValue)
{
	nBodyPartIndex = nValue;
}

int KRRAttribute::GetBodyPartIndex() const
{
	return nBodyPartIndex;
}

boolean KRRAttribute::IsInBodyContinuous(Continuous cValue) const
{
	KRRInterval* interval;
	require(GetAttributeType() == KWType::Continuous);

	interval = GetPartAt(GetBodyPartIndex())->GetInterval();

	/*cout << "Interval lower bound : " << interval->GetLowerBound() << endl;
	cout << "Interval upper bound : " << interval->GetUpperBound() << endl;
	cout << "cValue : " << cValue << endl;*/

	/*if (interval->GetLowerBound() == KWContinuous::GetMinValue() or
		interval->GetLowerBound() == KWContinuous::GetMissingValue())
		return cValue <= interval->GetUpperBound();
	else
		return interval->GetLowerBound() < cValue and cValue <= interval->GetUpperBound();*/

	/*if (interval->GetLowerBound() == KWContinuous::GetMinValue())
			return cValue <= interval->GetUpperBound();
		else if (interval->GetUpperBound() == KWContinuous::GetMissingValue())
			return cValue == KWContinuous::GetMissingValue();
		else
			return interval->GetLowerBound() < cValue and cValue <= interval->GetUpperBound();*/

	if (GetBodyPartIndex() == 0)
		return cValue <= interval->GetUpperBound();
	else //if (GetBodyPartIndex() == 1)
		return interval->GetLowerBound() < cValue and cValue <= interval->GetUpperBound();
}

boolean KRRAttribute::IsInBodySymbol(Symbol sValue) const
{
	//KRRValueSet* valueSet;
	KRRPart* bodyPart;

	require(GetAttributeType() == KWType::Symbol);

	bodyPart = GetPartAt(GetBodyPartIndex());

	return bodyPart == LookupSymbolPart(sValue);
}

void KRRAttribute::SetCost(double dValue)
{
	dCost = dValue;
}

double KRRAttribute::GetCost() const
{
	return dCost;
}

KRRPart* KRRAttribute::AddPart()
{
	KRRPart* part;

	require(GetAttributeType() != KWType::Unknown);
	require((GetPartNumber() <= 2 and GetAttributeType() == KWType::Symbol) or (GetPartNumber() <= 3 and GetAttributeType() == KWType::Continuous));
	//require(KWType::IsSimple(GetAttributeType()));
	require(not IsIndexed());

	// Creation d'une nouvelle partie en fonction du type de l'attribut
	part = new KRRPart;
	part->SetPartType(GetAttributeType());

	// On connecte la partie a l'attribut
	part->attribute = this;

	// Ajout en fin de tableau des parties
	oaParts.Add(part);

	return part;
}

void KRRAttribute::DeletePart(KRRPart* part)
{
	require(part != NULL);
	//require(CheckPart(part));
	//require(not IsIndexed());
	int nIndex;
	for (nIndex = 0; nIndex < GetPartNumber(); nIndex++)
	{
		if (part == GetPartAt(nIndex))
			break;
	}
	oaParts.RemoveAt(nIndex);
	delete part;
}

void KRRAttribute::DeleteAllParts()
{
	oaParts.DeleteAll();
}

boolean KRRAttribute::CheckPart(KRRPart* part) const
{
	return false;
}

int KRRAttribute::GetPartNumber() const
{
	return oaParts.GetSize();
}

KRRPart* KRRAttribute::GetPartAt(int nPartIndex) const
{
	require(0 <= nPartIndex and nPartIndex < GetPartNumber());
	return cast(KRRPart*, oaParts.GetAt(nPartIndex));
}

void KRRAttribute::BuildIndexingStructure()
{
	KRRPart* part;
	KRRValueSet* valueSet;
	KRRValue* value;
	int nIndex;

	require(GetAttributeType() != KWType::Unknown);
	//require(KWType::IsSimple(GetAttributeType()));
	//require(Check());

	// Indexation si necessaire
	if (not bIsIndexed)
	{
		//assert(oaIntervals.GetSize() == 0);
		assert(nkdParts.GetCount() == 0);

		// Indexation des intervalles si attribut numerique
		if (GetAttributeType() == KWType::Continuous)
		{
			//// Ajout des parties numeriques dans le tableau des intervalles
			//ExportParts(&oaIntervals);

			//// Tri des intervalles par borne inf
			//oaIntervals.SetCompareFunction(KWDGPartContinuousCompare);
			//oaIntervals.Sort();
		}
		// Sinon, indexation des parties par les valeurs
		else
		{
			cout << "Trying to index\n";
			// Parcours des parties pour les indexer par leurs valeurs
			nkdParts.RemoveAll();
			starValuePart = NULL;
			//part = headPart;
			for (nIndex = 0; nIndex < GetPartNumber(); nIndex++)
			{
				part = GetPartAt(nIndex);
				// Parcours des valeurs de la partie
				valueSet = part->GetValueSet();
				value = valueSet->GetHeadValue();

				while (value != NULL)
				{
					// Ajout de la partie avec la valeur pour cle
					nkdParts.SetAt((NUMERIC)value->GetValue().GetNumericKey(), part);

					// Memorisation de la partie associe a la valeur speciale
					if (value->GetValue() == Symbol::GetStarValue())
						starValuePart = part;

					// Valeur suivante
					valueSet->GetNextValue(value);
				}
			}
			//while (part != NULL)
			//{
			//	// Parcours des valeurs de la partie
			//	valueSet = part->GetValueSet();
			//	value = valueSet->GetHeadValue();
			//	while (value != NULL)
			//	{
			//		// Ajout de la partie avec la valeur pour cle
			//		nkdParts.SetAt((NUMERIC)value->GetValue().GetNumericKey(), part);

			//		// Memorisation de la partie associe a la valeur speciale
			//		if (value->GetValue() == Symbol::GetStarValue())
			//			starValuePart = part;

			//		// Valeur suivante
			//		valueSet->GetNextValue(value);
			//	}

			//	// Partie suivante
			//	part = part->nextPart;
			//}

			assert(starValuePart != NULL);
		}

		// Memorisation du flag d'indexation
		bIsIndexed = true;
	}
}

void KRRAttribute::DeleteIndexingStructure()
{
	require(GetAttributeType() != KWType::Unknown);
	//require(KWType::IsSimple(GetAttributeType()));

	// Suppression de l'indexation si necessaire
	if (bIsIndexed)
	{
		if (GetAttributeType() == KWType::Continuous)
		{
			//oaIntervals.SetSize(0);
		}
		else
		{
			nkdParts.RemoveAll();
			starValuePart = NULL;
		}
		bIsIndexed = false;
	}
}

boolean KRRAttribute::IsIndexed() const
{
	return false;
}

KRRPart* KRRAttribute::LookupSymbolPart(Symbol sValue) const
{
	//KRRPart* part;
	KRRValue* krrvCurrent;

	//require(IsIndexed());
	require(GetAttributeType() == KWType::Symbol);

	krrvCurrent = new KRRValue(sValue, 0);
	if (GetPartAt(0)->GetValueSet()->CheckValue(krrvCurrent))
	{
		delete krrvCurrent;
		return GetPartAt(0);
	}
	else if (GetPartAt(1)->GetValueSet()->CheckValue(krrvCurrent))
	{
		delete krrvCurrent;
		return GetPartAt(1);
	}
	else
	{
		delete krrvCurrent;
		return NULL;
	}

	/*part = cast(KRRPart*, nkdParts.Lookup((NUMERIC)sValue.GetNumericKey()));
	if (part == NULL)
		part = starValuePart;
	ensure(part != NULL);
	return part;*/
}

KRRPart* KRRAttribute::LookupContinuousPart(Continuous cValue) const
{
	require(GetAttributeType() == KWType::Continuous);

	int nIndex;

	for (nIndex = 0; nIndex < GetPartNumber(); nIndex++)
	{
		if (GetPartAt(nIndex)->GetInterval()->CheckValue(cValue))
		{
			break;
		}
	}

	return GetPartAt(nIndex);
}

void KRRAttribute::Write(ostream& ost) const
{
	// En tete de l'attribut
	ost << GetClassLabel() << "\t" << GetObjectLabel() << "\n";

	// Parties de l'attribut
	if (GetPartNumber() > 0)
		WriteParts(ost);
}

void KRRAttribute::WriteParts(ostream& ost) const
{
	int nPart;
	KRRPart* part;
	KRRValue* value;
	int nMaxDisplayedValue = 20;
	int nDisplayedValue;
	ost << "\t" << GetAttributeName() << "\t" << GetPartNumber() << "\n";

	// Parties de l'attribut
	for (nPart = 0; nPart < GetPartNumber(); nPart++)
	{
		part = GetPartAt(nPart);
		ost << "\t\t" << part->GetObjectLabel();
		if (GetBodyPartIndex() == nPart)
			ost << "*";

		// Affichage des premiere valeurs dans le cas d'un attribut Symbol
		if (GetAttributeType() == KWType::Symbol)
		{
			nDisplayedValue = 0;
			value = part->GetValueSet()->GetHeadValue();
			while (value != NULL)
			{
				nDisplayedValue++;
				if (nDisplayedValue > nMaxDisplayedValue)
				{
					ost << "\t...";
					break;
				}
				else
					ost << "\t" << value->GetValue();
				part->GetValueSet()->GetNextValue(value);
			}
		}

		// Fin de ligne
		ost << "\n";
	}
}

void KRRAttribute::WritePartsLine(ostream& ost) const
{
	int nPart;
	KRRPart* part;
	KRRValue* value;
	int nMaxDisplayedValue = 20;
	int nDisplayedValue;
	//ost << "\t" << GetAttributeName() << "\t" << GetPartNumber() << "\n";

	// Parties de l'attribut
	for (nPart = 0; nPart < GetPartNumber(); nPart++)
	{
		part = GetPartAt(nPart);
		ost << "\t\t" << part->GetObjectLabel();
		if (GetBodyPartIndex() == nPart)
			ost << "*";

		// Affichage des premiere valeurs dans le cas d'un attribut Symbol
		if (GetAttributeType() == KWType::Symbol)
		{
			nDisplayedValue = 0;
			value = part->GetValueSet()->GetHeadValue();
			while (value != NULL)
			{
				nDisplayedValue++;
				if (nDisplayedValue > nMaxDisplayedValue)
				{
					ost << "\t...";
					break;
				}
				else
					ost << "\t" << value->GetValue();
				part->GetValueSet()->GetNextValue(value);
			}
		}

		// Fin de ligne
		ost << "\n";
	}
}

KRRAttribute* KRRAttribute::Create() const
{
	return new KRRAttribute;
}

void KRRAttribute::CopyFrom(const KRRAttribute* krraSource)
{
	int nPart;
	KRRPart* sourcePart;
	KRRPart* part;

	require(krraSource != NULL);
	//require(KWType::IsSimple(krraSource->GetAttributeType()));

	// copie des proprietes
	SetAttributeName(krraSource->GetAttributeName());
	if (nAttributeType != krraSource->GetAttributeType())
		SetAttributeType(krraSource->GetAttributeType());
	SetValueNumber(krraSource->GetValueNumber());
	nAttributeIndex = krraSource->GetAttributeIndex();
	SetBodyPartIndex(krraSource->GetBodyPartIndex());
	SetCost(krraSource->GetCost());

	// Nettoyage prealable des parties
	DeleteAllParts();

	// Creation d'autant de parties que necesaire
	for (nPart = 0; nPart < krraSource->GetPartNumber(); nPart++)
		AddPart();

	// Recopie du contenu des parties, selon le type
	if (krraSource->GetAttributeType() == KWType::Continuous)
	{
		// Recopie des parties sources
		for (nPart = 0; nPart < krraSource->GetPartNumber(); nPart++)
		{
			sourcePart = krraSource->GetPartAt(nPart);
			part = GetPartAt(nPart);
			part->GetInterval()->CopyFrom(sourcePart->GetInterval());
		}

		/*DDD
		nPartNumber = krraSource->GetPartNumber();
		oaParts.CopyFrom(&(krraSource->oaParts));
		*/
	}
	else if (krraSource->GetAttributeType() == KWType::Symbol)
	{
		for (nPart = 0; nPart < krraSource->GetPartNumber(); nPart++)
		{
			sourcePart = krraSource->GetPartAt(nPart);
			part = GetPartAt(nPart);
			part->GetValueSet()->CopyFrom(sourcePart->GetValueSet());
		}
	}
}

const ALString KRRAttribute::GetClassLabel() const
{
	return "Attribute";
}

const ALString KRRAttribute::GetObjectLabel() const
{
	ALString sTmp;

	// Libelle base sur le nombre de valeurs, d'attributs
	return sTmp + "(" + IntToString(GetPartNumber()) + ")";
}

///////////////////////////////////////////////////
// KRRPart

KRRPart::KRRPart()
{
	attribute = NULL;
	interval = NULL;
	valueSet = NULL;
	dCost = 0;
}

KRRPart::~KRRPart()
{
	if (interval != NULL)
		delete interval;
	if (valueSet != NULL)
		delete valueSet;

	// Reinitialisation pour faciliter le debug
	debug(attribute = NULL);
	debug(interval = NULL);
	debug(valueSet = NULL);
	debug(dCost = 0);
}

KRRAttribute* KRRPart::GetAttribute() const
{
	return attribute;
}

void KRRPart::SetPartType(int nValue)
{
	require(GetPartType() == KWType::Unknown);
	require(KWType::IsSimple(nValue));

	// Creation de l'objet interval ou ensemble de valeur selon le type
	if (nValue == KWType::Continuous)
		interval = new KRRInterval;
	else
		valueSet = new KRRValueSet;

	ensure(GetPartType() != KWType::Unknown);
}

int KRRPart::GetPartType() const
{
	require(interval == NULL or valueSet == NULL);

	if (interval != NULL)
		return KWType::Continuous;
	else if (valueSet != NULL)
		return KWType::Symbol;
	else
		return KWType::Unknown;
}

KRRInterval* KRRPart::GetInterval() const
{
	require(GetPartType() == KWType::Continuous);

	ensure(interval != NULL);
	return interval;
}

KRRValueSet* KRRPart::GetValueSet() const
{
	require(GetPartType() == KWType::Symbol);

	ensure(valueSet != NULL);
	return valueSet;
}

void KRRPart::SetCost(double dValue)
{
	dCost = dValue;
}

double KRRPart::GetCost() const
{
	return dCost;
}

void KRRPart::Write(ostream& ost) const
{
	// Identification de la partie
	ost << GetClassLabel() << "\t" << GetObjectLabel() << "\n";

	// Valeurs de la partie
	if (GetPartType() == KWType::Symbol and valueSet->GetValueNumber() > 0)
		WriteValues(ost);
}

void KRRPart::WriteValues(ostream& ost) const
{
	// Des valeurs sont a afficher uniquement dans le cas symbolique
	// (l'intervalle est le libelle de la partie dans le cas continu)
	if (GetPartType() == KWType::Symbol)
		valueSet->WriteValues(ost);
}

const ALString KRRPart::GetClassLabel() const
{
	if (GetPartType() == KWType::Continuous)
		return interval->GetClassLabel();
	else if (GetPartType() == KWType::Symbol)
		return valueSet->GetClassLabel();
	else
		return "Part";
}

const ALString KRRPart::GetObjectLabel() const
{
	if (GetPartType() == KWType::Continuous)
		return interval->GetObjectLabel();
	else if (GetPartType() == KWType::Symbol)
		return valueSet->GetObjectLabel();
	else
		return "";
}

////////////////////////////////////////////////////
// KRRInterval
KRRInterval::KRRInterval()
{
	cLowerBound = 0;
	cUpperBound = 0;
}

KRRInterval::~KRRInterval()
{
}

void KRRInterval::SetLowerBound(Continuous cValue)
{
	cLowerBound = cValue;
}

Continuous KRRInterval::GetLowerBound() const
{
	return cLowerBound;
}

void KRRInterval::SetUpperBound(Continuous cValue)
{
	cUpperBound = cValue;
}

Continuous KRRInterval::GetUpperBound() const
{
	return cUpperBound;
}

Continuous KRRInterval::GetMinLowerBound()
{
	assert(KWContinuous::GetMissingValue() < KWContinuous::GetMinValue());
	return KWContinuous::GetMissingValue();
}

Continuous KRRInterval::GetMaxUpperBound()
{
	return KWContinuous::GetMaxValue();
}

boolean KRRInterval::Check() const
{
	return false;
}

boolean KRRInterval::CheckValue(Continuous cValue) const
{
	return cLowerBound < cValue and cValue <= cUpperBound;
}

void KRRInterval::Import(KRRInterval* sourceInterval)
{
}

void KRRInterval::CopyFrom(const KRRInterval* sourceInterval)
{
	require(sourceInterval != NULL);

	cLowerBound = sourceInterval->cLowerBound;
	cUpperBound = sourceInterval->cUpperBound;
}

void KRRInterval::Write(ostream& ost) const
{
	// Identification
	ost << GetClassLabel() << "\t" << GetObjectLabel() << "\n";
}

const ALString KRRInterval::GetClassLabel() const
{
	return "Interval";
}

const ALString KRRInterval::GetObjectLabel() const
{
	ALString sLabel;

	// Cas particulier d'un intervalle reduit a la valeur manquante
	if (cUpperBound == KWContinuous::GetMissingValue())
		sLabel = "Missing";
	// Cas standard
	else
	{
		if (cLowerBound == KWContinuous::GetMissingValue())
			sLabel = "]Missing;";
		else if (cLowerBound == GetMinLowerBound())
			sLabel = "]-inf;";
		else
			sLabel = sLabel + "]" + KWContinuous::ContinuousToString(cLowerBound) + ";";
		if (cUpperBound == GetMaxUpperBound())
			sLabel += "+inf[";
		else
			sLabel = sLabel + KWContinuous::ContinuousToString(cUpperBound) + "]";
	}
	return sLabel;
}

////////////////////////////////////////////////////////
// KRRValueSet
KRRValueSet::KRRValueSet()
{
	headValue = NULL;
	tailValue = NULL;
	nValueNumber = 0;
	bIsIndexed = false;
}

KRRValueSet::~KRRValueSet()
{
	nkdValues.RemoveAll();
	DeleteAllValues();
}

KRRValue* KRRValueSet::AddValue(Symbol sValue, int nFrequency)
{
	KRRValue* value;

	require(nFrequency >= 0);

	// Creation de la valeur
	value = new KRRValue(sValue, nFrequency);

	// Ajout en fin de la liste des valeurs
	nValueNumber++;
	if (headValue == NULL)
		headValue = value;
	if (tailValue != NULL)
	{
		tailValue->nextValue = value;
		value->prevValue = tailValue;
	}
	tailValue = value;

	// On retourne la valeur cree
	return value;
}

void KRRValueSet::DeleteValue(KRRValue* value)
{
	require(value != NULL);

	// Supression de la liste des valuees
	nValueNumber--;
	if (value->prevValue != NULL)
		value->prevValue->nextValue = value->nextValue;
	if (value->nextValue != NULL)
		value->nextValue->prevValue = value->prevValue;
	if (headValue == value)
		headValue = value->nextValue;
	if (tailValue == value)
		tailValue = value->prevValue;

	// Destruction de la valeur
	delete value;
}

void KRRValueSet::DeleteAllValues()
{
	KRRValue* value;
	KRRValue* valueToDelete;

	// Destruction des valeurs
	value = headValue;
	while (value != NULL)
	{
		valueToDelete = value;
		value = value->nextValue;
		delete valueToDelete;
	}

	// Reinitialisation de la gestion de la liste des valeurs
	headValue = NULL;
	tailValue = NULL;
	nValueNumber = 0;
}

int KRRValueSet::GetValueNumber() const
{
	return nValueNumber;
}

KRRValue* KRRValueSet::GetHeadValue() const
{
	return headValue;
}

KRRValue* KRRValueSet::GetTailValue() const
{
	return tailValue;
}

void KRRValueSet::GetNextValue(KRRValue*& value) const
{
	require(value != NULL);
	value = value->nextValue;
}

void KRRValueSet::GetPrevValue(KRRValue*& value) const
{
	require(value != NULL);
	value = value->prevValue;
}

void KRRValueSet::CompressValueSet()
{
}

void KRRValueSet::BuildIndexingStructure()
{
	KRRValue* value;

	if (not bIsIndexed)
	{
		nkdValues.RemoveAll();
		value = GetHeadValue();
		while (value != NULL)
		{
			// Ajout de la valeur avec la valeur (symbolique) pour cle
			nkdValues.SetAt((NUMERIC)value->GetValue().GetNumericKey(), value);

			// Valeur suivante
			GetNextValue(value);
		}
		bIsIndexed = true;
	}
}

void KRRValueSet::DeleteIndexingStructure()
{
	// Suppression de l'indexation si necessaire
	if (bIsIndexed)
		nkdValues.RemoveAll();
	bIsIndexed = false;
}

boolean KRRValueSet::IsIndexed() const
{
	return bIsIndexed;
}

boolean KRRValueSet::CheckValue(KRRValue* value) const
{
	boolean bOk;
	KRRValue* currentValue;
	Symbol sValue;

	require(value != NULL);
	/*require(IsIndexed());

	sValue = value->GetValue();
	currentValue = cast(KRRValue*, nkdValues.Lookup((NUMERIC)sValue.GetNumericKey()));

	if (currentValue == NULL)
		bOk = false;
	else
		bOk = true;

	return bOk;*/

	// Test si la valeur fait partie de la partie
	bOk = false;
	currentValue = headValue;
	while (currentValue != NULL)
	{
		// comparaison de la valeur seulement
		if (value->GetValue() == currentValue->GetValue())
		{
			bOk = true;
			break;
		}

		/*if (value == currentValue)
		{
			bOk = true;
			break;
		}*/
		currentValue = currentValue->nextValue;
	}
	return bOk;
}

boolean KRRValueSet::Check() const
{
	return true;
}

int KRRValueSet::ComputeTotalFrequency() const
{
	int nTotalFrequency;
	KRRValue* value;

	// Parcours des valeurs de la partie pour calcul l'effectif cumule
	nTotalFrequency = 0;
	value = GetHeadValue();
	while (value != NULL)
	{
		nTotalFrequency += value->GetValueFrequency();
		GetNextValue(value);
	}
	return nTotalFrequency;
}

void KRRValueSet::Import(KRRValueSet* sourceValueSet)
{
}

void KRRValueSet::CopyFrom(const KRRValueSet* sourceValueSet)
{
	KRRValue* value;

	require(sourceValueSet != NULL);

	// Nettoyage des valeurs actuelles
	DeleteAllValues();

	// Recopie de la liste de valeurs source
	value = sourceValueSet->GetHeadValue();
	while (value != NULL)
	{
		AddValue(value->GetValue(), value->GetValueFrequency());
		sourceValueSet->GetNextValue(value);
	}
}

void KRRValueSet::UpgradeFrom(const KRRValueSet* sourceValueSet)
{
	KRRValue* value;

	require(sourceValueSet != NULL);

	// Recopie de la liste de valeurs source
	value = sourceValueSet->GetHeadValue();
	while (value != NULL)
	{
		AddValue(value->GetValue(), value->GetValueFrequency());
		sourceValueSet->GetNextValue(value);
	}
}

void KRRValueSet::SortValues()
{
	ObjectArray oaValues;
	int i;
	KRRValue* value;

	// Rangement des valeurs dans un tableau
	oaValues.SetSize(GetValueNumber());
	value = GetHeadValue();
	i = 0;
	while (value != NULL)
	{
		oaValues.SetAt(i, value);
		i++;
		GetNextValue(value);
	}

	// Tri des valeurs par effectif decroissant
	oaValues.SetCompareFunction(KRRValueCompareDecreasingFrequency);
	oaValues.Sort();

	// Rangement des valeurs dans la liste, selon l'ordre du tableau trie
	headValue = NULL;
	tailValue = NULL;
	for (i = 0; i < oaValues.GetSize(); i++)
	{
		value = cast(KRRValue*, oaValues.GetAt(i));

		// Reinitialisation du chainage de la partie
		value->prevValue = NULL;
		value->nextValue = NULL;

		// Ajout de la valeur en fin de liste
		if (headValue == NULL)
			headValue = value;
		if (tailValue != NULL)
		{
			tailValue->nextValue = value;
			value->prevValue = tailValue;
		}
		tailValue = value;
	}
}

void KRRValueSet::Write(ostream& ost) const
{
	// Identification
	ost << GetClassLabel() << "\t" << GetObjectLabel() << "\n";

	// Affichage des valeurs
	WriteValues(ost);
}

void KRRValueSet::WriteValues(ostream& ost) const
{
	KRRValue* value;

	// Affichage des valeurs
	cout << "Values"
	     << "\t" << GetValueNumber() << "\n";
	value = GetHeadValue();
	while (value != NULL)
	{
		ost << "\t" << *value << "\n";
		GetNextValue(value);
	}
}

const ALString KRRValueSet::GetClassLabel() const
{
	return "Value set";
}

const ALString KRRValueSet::GetObjectLabel() const
{
	ALString sLabel;
	int nValue;
	KRRValue* dgValue;

	// Libelle base sur l'ensemble des valeurs
	sLabel = "{";
	nValue = 0;
	dgValue = GetHeadValue();
	while (dgValue != NULL)
	{
		// On n'utilise la modalite speciale pour fabriquer le libelle
		if (dgValue->GetValue() != Symbol::GetStarValue())
		{
			// Prise en compte si moins de trois valeurs
			if (nValue < 3)
			{
				if (nValue > 0)
					sLabel += ", ";
				sLabel += dgValue->GetValue();
				nValue++;
			}
			// Arret si au moins quatre valeurs
			else
			{
				sLabel += ", ...";
				break;
			}
		}
		GetNextValue(dgValue);
	}
	sLabel += "}";
	return sLabel;
}

boolean KRRValueSet::GetEmulated() const
{
	return false;
}

///////////////////////////////////////////////////////
// KRRValue
KRRValue::KRRValue(Symbol sValue, int nFrequency)
{
	require(nFrequency >= 0);

	sSymbolValue = sValue;
	nValueFrequency = nFrequency;
	prevValue = NULL;
	nextValue = NULL;
}

KRRValue::~KRRValue()
{
}

Symbol KRRValue::GetValue() const
{
	return sSymbolValue;
}

int KRRValue::GetValueFrequency() const
{
	return nValueFrequency;
}

void KRRValue::Write(ostream& ost) const
{
	ost << sSymbolValue << "\t" << nValueFrequency;
}

int KRRValueCompareDecreasingFrequency(const void* elem1, const void* elem2)
{
	KRRValue* value1;
	KRRValue* value2;

	require(elem1 != NULL);
	require(elem2 != NULL);

	// Acces a la valueies
	value1 = cast(KRRValue*, *(Object**)elem1);
	value2 = cast(KRRValue*, *(Object**)elem2);

	// Comparaison
	return -value1->GetValueFrequency() + value2->GetValueFrequency();
}
