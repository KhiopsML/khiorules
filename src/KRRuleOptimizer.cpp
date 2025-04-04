// Copyright (c) 2023-2025 Orange. All rights reserved.
// This software is distributed under the BSD 3-Clause-clear License, the text of which is available
// at https://spdx.org/licenses/BSD-3-Clause-Clear.html or see the "LICENSE" file for more details.

#include "KRRuleOptimizer.h"

//////////////////////////////////////////////////////////////////////////////////
// Classe KRRuleOptimizer

KRRuleOptimizer::KRRuleOptimizer()
{
	nMaxRuleNumber = 10;
	nTimeAllowed = 600;
	bIsIndexed = false;
	classStats = NULL;
	ruleCosts = NULL;
	dEpsilon = 1e-5;
}

KRRuleOptimizer::~KRRuleOptimizer()
{
	DeleteIndexingStructure();
	//odPreprocessedAttributes.RemoveAll();
	oaPreprocessedAttributes.DeleteAll();
}

void KRRuleOptimizer::SetMaxRuleNumber(int nNumber)
{
	nMaxRuleNumber = nNumber;
}

const int KRRuleOptimizer::GetMaxRuleNumber() const
{
	return nMaxRuleNumber;
}

void KRRuleOptimizer::SetTimeAllowed(int nNumber)
{
	nTimeAllowed = nNumber;
}

const int KRRuleOptimizer::GetTimeAllowed() const
{
	return nTimeAllowed;
}

void KRRuleOptimizer::SetClassStats(KWClassStats* stats)
{
	classStats = stats;
}

KWClassStats* KRRuleOptimizer::GetClassStats() const
{
	return classStats;
}

void KRRuleOptimizer::SetRuleCosts(KRRuleCosts* costs)
{
	ruleCosts = costs;
}

KRRuleCosts* KRRuleOptimizer::GetRuleCosts() const
{
	return ruleCosts;
}

void KRRuleOptimizer::BuildIndexingStructure()
{
	int nIndex;
	KRPreprocessedAttribute* krpaAttribute;
	ALString sName;

	//parcours des PreprocessedAttributes pour les indexer par nom
	odPreprocessedAttributes.DeleteAll();
	for (nIndex = 0; nIndex < oaPreprocessedAttributes.GetSize(); nIndex++)
	{
		krpaAttribute = cast(KRPreprocessedAttribute*, oaPreprocessedAttributes.GetAt(nIndex));
		sName = krpaAttribute->GetAttributeName();
		odPreprocessedAttributes.SetAt(sName, krpaAttribute);
	}
	bIsIndexed = true;
}

void KRRuleOptimizer::DeleteIndexingStructure()
{
	// Suppression de l'indexation si necessaire
	if (bIsIndexed)
		odPreprocessedAttributes.RemoveAll();
	bIsIndexed = false;
}

boolean KRRuleOptimizer::IsIndexed() const
{
	return bIsIndexed;
}

KRPreprocessedAttribute* KRRuleOptimizer::LookUpPreprocessedAttribute(const ALString& sName) const
{
	KRPreprocessedAttribute* krpaAttribute;

	require(IsIndexed());

	krpaAttribute = cast(KRPreprocessedAttribute*, odPreprocessedAttributes.Lookup(sName));
	if (krpaAttribute == NULL)
		cout << "Attribute NULL = " << sName << endl;
	ensure(krpaAttribute != NULL);
	return krpaAttribute;
}

void KRRuleOptimizer::InitializeObjectsCounters(KWDatabase* kwdb, KRRule* rule)
{
	int nObject;
	KWObject* kwoObject;
	const KWClass* kwcSortClass;
	int nAttributeIndex;
	KRRAttribute* krrAttribute;
	KWAttribute* kwaAttribute;
	KWLoadIndex liKWAttributeIndex;
	KRRPart* krrpBodyPart;
	Continuous cCurrentValue;
	Symbol sCurrentValue;

	require(rule != NULL);
	require(kwdb->GetObjects() != NULL);

	for (nObject = 0; nObject < kwdb->GetObjects()->GetSize(); nObject++)
	{
		kwoObject = cast(KWObject*, kwdb->GetObjects()->GetAt(nObject));
		kwcSortClass = kwoObject->GetClass();
		for (nAttributeIndex = 0; nAttributeIndex < rule->GetAttributeNumber(); nAttributeIndex++)
		{
			krrAttribute = rule->GetAttributeAt(nAttributeIndex);
			kwaAttribute = kwcSortClass->LookupAttribute(krrAttribute->GetAttributeName());
			liKWAttributeIndex = kwaAttribute->GetLoadIndex();
			krrpBodyPart = krrAttribute->GetPartAt(krrAttribute->GetBodyPartIndex());
			// cas categoriel
			if (krrAttribute->GetAttributeType() == KWType::Symbol)
			{
				sCurrentValue = kwoObject->GetSymbolValueAt(liKWAttributeIndex);
				if (krrAttribute->IsInBodySymbol(sCurrentValue))
				//if (krrAttribute->LookupSymbolPart(sCurrentValue) == krrpBodyPart)
				// si valeur de l'attribut de l'objet n'est pas dans la partie active du corps
				{
					krocCounters.IncrementBodyCountAt(kwoObject);
				}
			}
			// cas numerique
			if (krrAttribute->GetAttributeType() == KWType::Continuous)
			{
				cCurrentValue = kwoObject->GetContinuousValueAt(liKWAttributeIndex);
				//if (krrpBodyPart->GetInterval()->GetLowerBound() < cCurrentValue
				//		and krrpBodyPart->GetInterval()->GetUpperBound() >= cCurrentValue)
				// si valeur de l'attribut de l'objet n'est pas dans la partie active du corps
				if (krrAttribute->IsInBodyContinuous(cCurrentValue))
				{
					krocCounters.IncrementBodyCountAt(kwoObject);
				}
			}
		}
	}
}

boolean KRRuleOptimizer::CheckObjectCounters(KWDatabase* kwdb, KRRule* rule)
{
	int nObject;
	KWObject* kwoObject;
	const KWClass* kwcSortClass;
	int nAttributeIndex;
	KRRAttribute* krrAttribute;
	KWAttribute* kwaAttribute;
	KWLoadIndex liKWAttributeIndex;
	KRRPart* krrpBodyPart;
	Continuous cCurrentValue;
	Symbol sCurrentValue;
	boolean bOk = true;
	int nCount;

	require(rule != NULL);
	require(kwdb->GetObjects() != NULL);

	for (nObject = 0; nObject < kwdb->GetObjects()->GetSize(); nObject++)
	{
		nCount = 0;
		kwoObject = cast(KWObject*, kwdb->GetObjects()->GetAt(nObject));
		kwcSortClass = kwoObject->GetClass();
		for (nAttributeIndex = 0; nAttributeIndex < rule->GetAttributeNumber(); nAttributeIndex++)
		{
			krrAttribute = rule->GetAttributeAt(nAttributeIndex);
			kwaAttribute = kwcSortClass->LookupAttribute(krrAttribute->GetAttributeName());
			liKWAttributeIndex = kwaAttribute->GetLoadIndex();
			krrpBodyPart = krrAttribute->GetPartAt(krrAttribute->GetBodyPartIndex());
			// cas categoriel
			if (krrAttribute->GetAttributeType() == KWType::Symbol)
			{
				sCurrentValue = kwoObject->GetSymbolValueAt(liKWAttributeIndex);
				if (krrAttribute->LookupSymbolPart(sCurrentValue) == krrpBodyPart)
				// si valeur de l'attribut de l'objet n'est pas dans la partie active du corps
				{
					nCount++;
				}
			}
			// cas numerique
			if (krrAttribute->GetAttributeType() == KWType::Continuous)
			{
				cCurrentValue = kwoObject->GetContinuousValueAt(liKWAttributeIndex);
				// si valeur de l'attribut de l'objet n'est pas dans la partie active du corps
				if (krrAttribute->IsInBodyContinuous(cCurrentValue))
				{
					nCount++;
				}
			}
		}

		// verification des compteurs
		if (not krocCounters.GetBodyCountAt(kwoObject) == nCount)
		{
			bOk = false;
			break;
		}
	}
	return bOk;
}

void KRRuleOptimizer::OptimizeRuleSet(KWDatabase* kwdb, ObjectArray* oaRuleSet)
{
	require(GetClassStats() != NULL);

	int nRuleStoredCounter;
	int nRuleTestedCounter;
	KWObject* kwoRandomObject;
	int nRandomIndex;
	KRRule* krrInitRule;
	ObjectArray* oaAttributes;
	//DDD MB ofstream output("output.txt", ios::out);
	Timer tCounter;
	Timer tExtractCounter;
	ALString sTmp;

	// Debut de tache
	TaskProgression::BeginTask();
	TaskProgression::DisplayMainLabel("Mining Rules");

	// Initialisation des compteurs
	nRuleStoredCounter = 1;
	nRuleTestedCounter = 1;
	//tExtractCounter.Start();

	//DDD MB cout << nTimeAllowed << endl;
	//DDD MB cout << "oaRuleSetSize : " << oaRuleSet->GetSize() << endl;

	while (nRuleTestedCounter <= nMaxRuleNumber)
	{
		// Gestion du compteur temps
		/*tExtractCounter.Stop();
		if (tExtractCounter.GetElapsedTime() > nTimeAllowed)
		{
			delete oaAttributes;
			tExtractCounter.Start();
			break;
		}
		cout << tExtractCounter.GetElapsedTime() << endl;
		tExtractCounter.Start();*/

		// Taux d'avancement
		TaskProgression::DisplayLabel(sTmp + "Rules (stored/tested/total): " + IntToString(nRuleStoredCounter - 1) + "/" + IntToString(nRuleTestedCounter - 1) + "/" + IntToString(nMaxRuleNumber));
		TaskProgression::DisplayProgression((int)(100 * (nRuleTestedCounter * 1.0) / nMaxRuleNumber));
		if (TaskProgression::IsInterruptionRequested())
		{
			oaRuleSet->SetSize(nRuleStoredCounter - 1);
			break;
		}

		// Choix de l'objet au hasard
		nRandomIndex = RandomInt(GetClassStats()->GetInstanceNumber() - 1);
		//DDD MB output << "\nnRandomIndex = " << nRandomIndex << endl;
		kwoRandomObject = cast(KWObject*, kwdb->GetObjects()->GetAt(nRandomIndex));
		//output << "randomObject : \n";
		//DDD MB kwoRandomObject->Write(output);

		// Choix des index des attributs au hasard
		oaAttributes = new ObjectArray;
		RandomAttributeSubsetSelection(kwdb, oaAttributes);
		//oaAttributes->Write(cout);

		// Initialisation de la regle avec un ensemble d'attributs et un objet au hasard
		krrInitRule = new KRRule;

		InitializeRule(kwdb, oaAttributes, kwoRandomObject, krrInitRule);
		InitializeObjectsCounters(kwdb, krrInitRule);
		ensure(CheckObjectCounters(kwdb, krrInitRule));
		//DDD MB krrInitRule->Write(output);

		// Optimisation de la regle
		tCounter.Start();
		OptimizeRule(kwdb, kwoRandomObject, krrInitRule);
		tCounter.Stop();
		assert(krrInitRule->GetTotalCost() != KWContinuous::GetMissingValue() and krrInitRule->GetTotalCost() != KWContinuous::GetForbiddenValue());
		//DDD MB output << "Time to optimize rule #" << nRuleTestedCounter << " : " << tCounter.GetElapsedTime() << "\nFinal Level : " << krrInitRule->GetLevel() << "\n";
		tCounter.Reset();

		// Ecriture de la regle dans un fichier de sortie temporaire
		//DDD MB krrInitRule->Write(output);
		//DDD MB output << "\n**************************************\n";

		// Ajout de la regle au tableau si interessante
		if (krrInitRule->GetLevel() > dEpsilon)
		{
			oaRuleSet->SetAt(nRuleStoredCounter - 1, krrInitRule);
			nRuleStoredCounter++;
		}
		else
		{
			delete krrInitRule;
		}
		nRuleTestedCounter++;

		// liberation de memoire
		delete oaAttributes;
	}

	//tExtractCounter.Stop();

	oaRuleSet->SetSize(nRuleStoredCounter - 1);
	//DDD MB cout << "oaRuleSetSize : " << oaRuleSet->GetSize() << endl;

	Global::AddSimpleMessage(sTmp + "Rules extracted and stored: " + IntToString(nRuleStoredCounter - 1));

	// Fin de tache
	TaskProgression::EndTask();
}

void KRRuleOptimizer::WritePreprocessing(ostream& ost)
{
	int nIndex;

	require(oaPreprocessedAttributes.GetSize() != 0);

	for (nIndex = 0; nIndex < oaPreprocessedAttributes.GetSize(); nIndex++)
		ost << *oaPreprocessedAttributes.GetAt(nIndex);
}

void KRRuleOptimizer::Preprocess(KWDatabase* kwdb)
{
	int nIndex;
	KWClass* kwcClass;
	KWAttribute* attribute;
	KRPreprocessedContinuousAttribute* krpcAttribute;
	KRPreprocessedSymbolAttribute* krpsAttribute;

	require(kwdb != NULL);

	// Acces a la classe
	kwcClass = GetClassStats()->GetClass();
	/*cout << "NB attributes : " << GetClassStats()->GetClass()->GetAttributeNumber() << endl;;
	cout << "NB Loaded attributes : " << GetClassStats()->GetClass()->GetLoadedAttributeNumber() << endl;;*/

	// Initialisation pour pretraitement de chaque attribut de la base
	for (nIndex = 0; nIndex < kwcClass->GetLoadedAttributeNumber(); nIndex++)
	{
		attribute = kwcClass->GetLoadedAttributeAt(nIndex);
		if (attribute->GetName() != GetClassStats()->GetTargetAttributeName() or GetClassStats()->GetTargetAttributeType() == KWType::Continuous)
		{
			// cas attribut continu
			//if (KWType::IsSimple(attribute->GetType()) and attribute->GetType() == KWType::Continuous)
			if (attribute->GetType() == KWType::Continuous)
			{
				krpcAttribute = new KRPreprocessedContinuousAttribute;
				krpcAttribute->SetClassStats(GetClassStats());
				krpcAttribute->Initialize(kwdb, attribute);
				oaPreprocessedAttributes.Add(krpcAttribute);
			}

			//cas attribut categoriel
			//if (KWType::IsSimple(attribute->GetType()) and attribute->GetType() == KWType::Symbol)
			if (attribute->GetType() == KWType::Symbol)
			{
				krpsAttribute = new KRPreprocessedSymbolAttribute;
				krpsAttribute->SetClassStats(GetClassStats());
				krpsAttribute->Initialize(kwdb, attribute);
				oaPreprocessedAttributes.Add(krpsAttribute);
				//krpsAttribute->Test();
			}
		}
	}

	// construction de la structure d'indexation
	BuildIndexingStructure();

	// Initialisation de la structure de compteurs d'objets
	krocCounters.InitializeObjects(kwdb->GetObjects());
}

void KRRuleOptimizer::RandomAttributeSubsetSelection(KWDatabase* kwdb, ObjectArray* oaAttributes)
{
	KWClass* kwcClass;
	//ObjectArray* oaAttributes; // Parametre contenbant le resultat en sortie
	int nAttribute;
	KWAttribute* kwaAttribute;
	int nMaxAttributeNumber;
	int nRandomAttributeNumber;

	//oaAttributes = new ObjectArray;
	// Acces a la classe
	kwcClass = GetClassStats()->GetClass();

	// Initialisation avec l'ensemble des attributs de la classe, potentiellement utilisables dans une regle
	for (nAttribute = 0; nAttribute < kwcClass->GetLoadedAttributeNumber(); nAttribute++)
	{
		kwaAttribute = kwcClass->GetLoadedAttributeAt(nAttribute);
		//DDD if (kwaAttribute->GetName() != GetClassStats()->GetTargetAttributeName() and
		//	KWType::IsSimple(kwaAttribute->GetType()))
		if (kwaAttribute->GetName() != GetClassStats()->GetTargetAttributeName())
			oaAttributes->Add(kwaAttribute);
	}

	// Choix d'un sous-ensemble d'attribut au hasard
	nMaxAttributeNumber = (int)(log(GetClassStats()->GetInstanceNumber() * 1.0) / log(2.0) + 1.0);
	if (oaAttributes->GetSize() < nMaxAttributeNumber)
		nMaxAttributeNumber = oaAttributes->GetSize();
	oaAttributes->Shuffle();
	nRandomAttributeNumber = RandomInt(nMaxAttributeNumber);
	if (nRandomAttributeNumber == 0)
		oaAttributes->SetSize(1);
	else
		oaAttributes->SetSize(nRandomAttributeNumber);
	if (GetClassStats()->GetTargetAttributeType() == KWType::Continuous)
	{
		kwaAttribute = kwcClass->LookupAttribute(GetClassStats()->GetTargetAttributeName());
		oaAttributes->Add(kwaAttribute);
	}
}

double KRRuleOptimizer::InitializeRule(KWDatabase* kwdb,
				       ObjectArray* oaAttributes,
				       KWObject* kwoInBody,
				       KRRule* krrInitRule)
{
	int nIndex;
	const KWClass* kwcSortClass;
	const KWClass* kwcClass;
	KWAttribute* kwaAttribute;
	KRRAttribute* krrAttribute;
	IntVector ivPartFrequencies;
	KWDGSAttributeSymbolValues* kwdgsasvTarget;
	int nInformativeAttributeNumber;
	const KWDGSAttributePartition* partition;
	//KRRuleCosts* costs;

	// require(krrInitRule->GetTargetType() != KWType::Unknown);

	// Initialisation de l'objet KRRule
	partition = GetClassStats()->GetTargetValueStats()->GetAttributeAt(GetClassStats()->GetTargetValueStats()->GetFirstTargetAttributeIndex());
	if (GetClassStats()->GetTargetAttributeType() == KWType::Symbol)
		krrInitRule->Initialize(oaAttributes->GetSize(), GetClassStats()->GetTargetAttributeType(),
					GetClassStats()->GetTargetDescriptiveStats()->GetValueNumber());
	else // regression case
		krrInitRule->Initialize(oaAttributes->GetSize(), GetClassStats()->GetTargetAttributeType(), 0);

	krrInitRule->SetTotalFrequency(kwdb->GetObjects()->GetSize());

	// Init distribution des classes dans la base et dans le corps pour la classification supervisée simple
	if (GetClassStats()->GetTargetAttributeType() == KWType::Symbol)
	{
		GetClassStats()->GetTargetValueStats()->ExportAttributePartFrequenciesAt(GetClassStats()->GetTargetValueStats()->GetFirstTargetAttributeIndex(),
											 &ivPartFrequencies);
		/*ivPartFrequencies.Write(cout);*/
		for (int i = 0; i < krrInitRule->GetTargetValueNumber(); i++)
		{
			//cout << krrInitRule->GetTargetValueNumber() << " targets" << endl;
			//cout << "TargetValue : " << i << endl;
			krrInitRule->SetTargetValueFrequencyAt(i, ivPartFrequencies.GetAt(i));

			//DDD
			//const KWDGSAttributePartition* partition = GetClassStats()->GetTargetValueStats()->GetAttributeAt(GetClassStats()->GetTargetValueStats()->GetFirstTargetAttributeIndex());
			//kwdgsasvTarget = cast(KWDGSAttributeSymbolValues*, partition);
			kwdgsasvTarget = cast(KWDGSAttributeSymbolValues*,
					      GetClassStats()->GetTargetValueStats()->GetAttributeAt(GetClassStats()->GetTargetValueStats()->GetFirstTargetAttributeIndex()));
			//cout << "cast ok" << endl;
			//kwdgsasvTarget->Write(cout);
			krrInitRule->SetTargetValueAt(i, kwdgsasvTarget->GetValueAt(i));
			//krrInitRule->SetInBodyTargetValueFrequencyAt(i, ivPartFrequencies.GetAt(i));
		}
	}
	//cout << "Init rule 2 ok" << endl;
	// Acces a la classe
	kwcClass = GetClassStats()->GetClass();

	for (nIndex = 0; nIndex < oaAttributes->GetSize(); nIndex++)
	{
		kwcSortClass = kwoInBody->GetClass();
		kwaAttribute = cast(KWAttribute*, oaAttributes->GetAt(nIndex));
		kwaAttribute = kwcSortClass->LookupAttribute(kwaAttribute->GetName());
		//cout << "att : " << kwaAttribute->GetName() << endl;
		// attributs numeriques
		// Construction de l'attribut de la regle
		krrAttribute = krrInitRule->GetAttributeAt(nIndex);
		if (kwaAttribute->GetType() == KWType::Continuous)
		{
			InitAttributeIntervals(kwdb, kwoInBody, kwaAttribute, krrAttribute);
		}
		if (kwaAttribute->GetType() == KWType::Symbol)
		{
			InitAttributeValueSets(kwdb, kwoInBody, kwaAttribute, krrAttribute);
		}
		//krrAttribute->Write(cout);
	}

	krrInitRule->DeleteNonInformativeAttributes();
	nInformativeAttributeNumber = krrInitRule->ComputeInformativeAttributeNumber();
	/*cout << "*****************\n";
	kwoInBody->Write(cout);
	krrInitRule->Write(cout);
	cout << "*****************\n";*/

	assert(IsInBody(kwoInBody, krrInitRule));
	UpdateAllStatistics(kwdb, krrInitRule);

	//nInformativeAttributeNumber = krrInitRule->ComputeInformativeAttributeNumber();
	//krrInitRule->InitializeAllCosts(GetRuleCosts());
	//ruleCosts->ComputeRuleTotalCost(krrInitRule);
	//ruleCosts->ComputeRuleCompressionCoefficient(krrInitRule);
	//assert(krrInitRule->CheckAllCosts(GetRuleCosts()));
	return 0;
}

void KRRuleOptimizer::InitAttributeIntervals(KWDatabase* database, KWObject* kwoRandomObject, KWAttribute* kwaAttribute, KRRAttribute* krrAttribute)
{
	KWLoadIndex liAttributeLoadIndex;
	Continuous cRandomObjectValue;
	KWObject* kwoObject;
	int nObjectIndex;
	int nLowerIndex;
	int nUpperIndex;
	Continuous cLowerValue;
	Continuous cUpperValue;
	Continuous cValue;
	KRRInterval* interval;
	Continuous cLowerBound;
	Continuous cUpperBound;
	KRPreprocessedAttribute* krpcaAttribute;

	require(database != NULL);
	require(kwoRandomObject != NULL);
	require(kwaAttribute != NULL);
	require(kwaAttribute->GetType() == KWType::Continuous);
	require(krrAttribute != NULL);
	require(krrAttribute->GetAttributeType() == KWType::Unknown);

	// Initilaisation de base de l'attribut de la regle
	krrAttribute->SetAttributeName(kwaAttribute->GetName());
	krrAttribute->SetAttributeType(kwaAttribute->GetType());
	krrAttribute->SetValueNumber(database->GetObjects()->GetSize());

	// Acces a l'index de l'objet dans la base
	liAttributeLoadIndex = kwaAttribute->GetLoadIndex();

	// Acces a la valeur "centrale" de l'objet random
	cRandomObjectValue = kwoRandomObject->GetContinuousValueAt(liAttributeLoadIndex);

	// Tri de la base selon l'attribut
	krpcaAttribute = cast(KRPreprocessedContinuousAttribute*, LookUpPreprocessedAttribute(krrAttribute->GetAttributeName()));
	//database->SortObjectsOnContinuous(kwaAttribute->GetName());

	// Recherche de l'index de l'objet random, selon cet ordre
	for (nObjectIndex = 0; nObjectIndex < krpcaAttribute->GetObjects()->GetSize(); nObjectIndex++)
	{
		kwoObject = cast(KWObject*, krpcaAttribute->GetObjects()->GetAt(nObjectIndex));
		if (kwoObject == kwoRandomObject)
			break;
	}
	assert(0 <= nObjectIndex and nObjectIndex < krpcaAttribute->GetObjects()->GetSize());

	// Recherche d'une borne d'intervalle precedent l'objet courant
	nLowerIndex = RandomInt(nObjectIndex);
	cLowerValue = KWContinuous::GetForbiddenValue();
	cLowerBound = KWContinuous::GetForbiddenValue();
	while (nLowerIndex >= 0)
	{
		kwoObject = cast(KWObject*, krpcaAttribute->GetObjects()->GetAt(nLowerIndex));
		cLowerValue = kwoObject->GetContinuousValueAt(liAttributeLoadIndex);

		// Arret des que l'on a une valeur strictement inferieure a la valeur de l'objet random
		if (cLowerValue < cRandomObjectValue)
		{
			// Calcul de la borne de l'intervalle, valeur moyenne de la valeur avec la suivante
			while (nLowerIndex <= nObjectIndex)
			{
				kwoObject = cast(KWObject*, krpcaAttribute->GetObjects()->GetAt(nLowerIndex));
				cValue = kwoObject->GetContinuousValueAt(liAttributeLoadIndex);
				if (cValue > cLowerValue)
				{
					cLowerBound = KWContinuous::GetLowerMeanValue(cLowerValue, cValue);
					break;
				}
				nLowerIndex++;
			}
			break;
		}
		nLowerIndex--;
	}
	assert(cLowerValue != KWContinuous::GetForbiddenValue() and cLowerValue <= cRandomObjectValue);

	// Recherche d'une borne d'intervalle suivant l'objet courant
	nUpperIndex = nObjectIndex + RandomInt(database->GetObjects()->GetSize() - nObjectIndex - 1);
	cUpperValue = KWContinuous::GetForbiddenValue();
	cUpperBound = KWContinuous::GetForbiddenValue();
	while (nUpperIndex < database->GetObjects()->GetSize())
	{
		kwoObject = cast(KWObject*, krpcaAttribute->GetObjects()->GetAt(nUpperIndex));
		cUpperValue = kwoObject->GetContinuousValueAt(liAttributeLoadIndex);

		// Arret des que l'on a une valeur strictement superieure a la valeur de l'objet random
		if (cUpperValue > cRandomObjectValue)
		{
			// Calcul de la borne de l'intervalle, valeur moyenne de la valeur avec la precedente
			while (nUpperIndex >= nObjectIndex)
			{
				kwoObject = cast(KWObject*, krpcaAttribute->GetObjects()->GetAt(nUpperIndex));
				cValue = kwoObject->GetContinuousValueAt(liAttributeLoadIndex);
				if (cValue < cUpperValue)
				{
					cUpperBound = KWContinuous::GetLowerMeanValue(cValue, cUpperValue);
					break;
				}
				nUpperIndex--;
			}
			break;
		}
		nUpperIndex++;
	}
	assert(cUpperValue != KWContinuous::GetForbiddenValue() and cUpperValue >= cRandomObjectValue);

	// Creation d'un seul intervalle
	if (cLowerValue == cRandomObjectValue and cRandomObjectValue == cUpperValue)
	{
		krrAttribute->AddPart();
		krrAttribute->SetBodyPartIndex(0);

		// Initialisation des bornes des intervalles
		interval = krrAttribute->GetPartAt(0)->GetInterval();
		interval->SetLowerBound(KRRInterval::GetMinLowerBound());
		interval->SetUpperBound(KRRInterval::GetMaxUpperBound());
	}
	// Creation de deux intervalles, dont un avant la valeur de l'objet random
	else if (cLowerValue < cRandomObjectValue and cRandomObjectValue == cUpperValue)
	{
		assert(cLowerBound != KWContinuous::GetForbiddenValue());
		krrAttribute->AddPart();
		krrAttribute->AddPart();
		krrAttribute->SetBodyPartIndex(1);

		// Initialisation des bornes des intervalles
		interval = krrAttribute->GetPartAt(0)->GetInterval();
		interval->SetLowerBound(KRRInterval::GetMinLowerBound());
		interval->SetUpperBound(cLowerBound);
		interval = krrAttribute->GetPartAt(1)->GetInterval();
		interval->SetLowerBound(cLowerBound);
		interval->SetUpperBound(KRRInterval::GetMaxUpperBound());
	}
	// Creation de deux intervalles, dont un apres la valeur de l'objet random
	else if (cLowerValue == cRandomObjectValue and cRandomObjectValue < cUpperValue)
	{
		assert(cUpperBound != KWContinuous::GetForbiddenValue());
		krrAttribute->AddPart();
		krrAttribute->AddPart();
		krrAttribute->SetBodyPartIndex(0);

		// Initialisation des bornes des intervalles
		interval = krrAttribute->GetPartAt(0)->GetInterval();
		interval->SetLowerBound(KRRInterval::GetMinLowerBound());
		interval->SetUpperBound(cUpperBound);
		interval = krrAttribute->GetPartAt(1)->GetInterval();
		interval->SetLowerBound(cUpperBound);
		interval->SetUpperBound(KRRInterval::GetMaxUpperBound());
	}
	// Creation de 3 intervalles
	else
	{
		assert(cLowerValue < cRandomObjectValue and cRandomObjectValue < cUpperValue);
		assert(cLowerBound != KWContinuous::GetForbiddenValue());
		assert(cUpperBound != KWContinuous::GetForbiddenValue());
		krrAttribute->AddPart();
		krrAttribute->AddPart();
		krrAttribute->AddPart();
		krrAttribute->SetBodyPartIndex(1);

		// Initialisation des bornes des intervalles
		interval = krrAttribute->GetPartAt(0)->GetInterval();
		interval->SetLowerBound(KRRInterval::GetMinLowerBound());
		interval->SetUpperBound(cLowerBound);
		interval = krrAttribute->GetPartAt(1)->GetInterval();
		interval->SetLowerBound(cLowerBound);
		interval->SetUpperBound(cUpperBound);
		interval = krrAttribute->GetPartAt(2)->GetInterval();
		interval->SetLowerBound(cUpperBound);
		interval->SetUpperBound(KRRInterval::GetMaxUpperBound());
	}
}

void KRRuleOptimizer::InitAttributeValueSets(KWDatabase* database, KWObject* kwoRandomObject, KWAttribute* kwaAttribute, KRRAttribute* krrAttribute)
{
	KWLoadIndex liAttributeLoadIndex;
	int nTotalValueNumber;
	int nObjectIndex;
	int nRandomPartIndex;
	KWObject* kwoObject;
	Symbol sRandomObjectValue;
	Symbol sCurrentValue;
	Symbol sPrecValue;
	SymbolVector svAttributeValues;
	Symbol sSourceValue;
	Symbol sSourceRef;
	int nValueFrequency;
	int nValueSmallestFrequency;
	KRPreprocessedSymbolAttribute* krpsaAttribute;

	require(database != NULL);
	require(kwoRandomObject != NULL);
	require(kwaAttribute != NULL);
	require(kwaAttribute->GetType() == KWType::Symbol);
	require(krrAttribute != NULL);
	require(krrAttribute->GetAttributeType() == KWType::Unknown);

	// Initilaisation de base de l'attribut de la regle
	krrAttribute->SetAttributeName(kwaAttribute->GetName());
	krrAttribute->SetAttributeType(kwaAttribute->GetType());

	// Acces a l'index de l'objet dans la base
	liAttributeLoadIndex = kwaAttribute->GetLoadIndex();

	// Acces a la valeur "centrale" de l'objet random
	sRandomObjectValue = kwoRandomObject->GetSymbolValueAt(liAttributeLoadIndex);

	// Tri de la base sur cet attribut
	// DDDdatabase->SortObjectsOnSymbol(kwaAttribute->GetName());
	krpsaAttribute = cast(KRPreprocessedSymbolAttribute*, LookUpPreprocessedAttribute(krrAttribute->GetAttributeName()));

	// Creation des deux parties de l'attribut (dans et hors du corps)
	krrAttribute->AddPart(); // part 0 dans le corps
	krrAttribute->AddPart(); // part 1 hors du corps
	krrAttribute->SetBodyPartIndex(0);
	nTotalValueNumber = 1;
	nValueFrequency = 0;
	nValueSmallestFrequency = krpsaAttribute->GetObjects()->GetSize() + 1;
	sPrecValue = cast(KWObject*, krpsaAttribute->GetObjects()->GetAt(0))->GetSymbolValueAt(liAttributeLoadIndex);
	//DDD nValueSmallestFrequency = database->GetObjects()->GetSize()+1;
	//DDD sPrecValue = cast(KWObject*, database->GetObjects()->GetAt(0))->GetSymbolValueAt(liAttributeLoadIndex);
	// Recherche de l'index de l'objet random, selon cet ordre
	//DDDfor (nObjectIndex = 0; nObjectIndex < database->GetObjects()->GetSize(); nObjectIndex++)
	for (nObjectIndex = 0; nObjectIndex < krpsaAttribute->GetObjects()->GetSize(); nObjectIndex++)
	{
		kwoObject = cast(KWObject*, krpsaAttribute->GetObjects()->GetAt(nObjectIndex));
		//DDD kwoObject = cast(KWObject*, database->GetObjects()->GetAt(nObjectIndex));
		sCurrentValue = kwoObject->GetSymbolValueAt(liAttributeLoadIndex);

		if (sCurrentValue != sPrecValue)
		{
			assert(nValueFrequency > 0);
			// La base est censee triee
			assert(nObjectIndex == 0 or sCurrentValue.GetNumericKey() > sPrecValue.GetNumericKey());

			// Incrementation du nombre de valeurs de l'attribut
			nTotalValueNumber++;

			// Ajout de la valeur precedente dans un des valueSet (dans ou hors du corps) au hasard
			nRandomPartIndex = RandomInt(1);
			if (sPrecValue == sRandomObjectValue) // valeur de l'objet reference dans le corps
			{
				krrAttribute->GetPartAt(0)->GetValueSet()->AddValue(sPrecValue, nValueFrequency);
			}
			else
			{
				krrAttribute->GetPartAt(nRandomPartIndex)->GetValueSet()->AddValue(sPrecValue, nValueFrequency);
			}

			// Reinitialisation des variables de travail
			sPrecValue = sCurrentValue;
			nValueFrequency = 1;
		}
		else
		{
			nValueFrequency++;
		}

		// Pour la derniere valeur
		if (nObjectIndex == krpsaAttribute->GetObjects()->GetSize() - 1)
			if (sCurrentValue == sRandomObjectValue)
				krrAttribute->GetPartAt(0)->GetValueSet()->AddValue(sCurrentValue, nValueFrequency);
			else
			{
				nRandomPartIndex = RandomInt(1);
				krrAttribute->GetPartAt(nRandomPartIndex)->GetValueSet()->AddValue(sCurrentValue, nValueFrequency);
			}
	}

	// Ajout de la modalite speciale dans le corps
	//nRandomPartIndex = RandomInt(1);
	assert(nTotalValueNumber > 1);
	krrAttribute->GetPartAt(0)->GetValueSet()->AddValue(Symbol::GetStarValue(), 0);
	nTotalValueNumber++;

	// Parametrage du nombre total de valeurs
	krrAttribute->SetValueNumber(nTotalValueNumber);
	assert(krrAttribute->GetPartNumber() >= 1);
	assert(krrAttribute->GetValueNumber() >= krrAttribute->GetPartNumber());

	//Indexation des ValueSets
	/*krrAttribute->GetPartAt(0)->GetValueSet()->BuildIndexingStructure();
	krrAttribute->GetPartAt(1)->GetValueSet()->BuildIndexingStructure();*/
}

void KRRuleOptimizer::UpdateSubStatistics(ObjectArray* oaWorkingInstances, KRRule* krrInitRule)
{
	int nObjectIndex;
	int nIndex;
	KWObject* kwoTempObject;
	const KWClass* kwcSortClass;
	Symbol sCurrentValue;
	KWLoadIndex liTargetAttributeIndex;
	Symbol sObjectTargetValue;
	int nObjectTargetValueIndex;
	int nInformativeAttributeNumber;

	// remise a zero des vecteurs de frequence par classe avant update
	for (nIndex = 0; nIndex < krrInitRule->GetTargetValueNumber(); nIndex++)
	{
		krrInitRule->SetInBodyTargetValueFrequencyAt(nIndex, 0);
	}

	for (nObjectIndex = 0; nObjectIndex < oaWorkingInstances->GetSize(); nObjectIndex++)
	{
		kwoTempObject = cast(KWObject*, oaWorkingInstances->GetAt(nObjectIndex));
		kwcSortClass = kwoTempObject->GetClass();

		// si l'objet est couvert par la regle
		if (krocCounters.GetBodyCountAt(kwoTempObject) == krrInitRule->GetAttributeNumber())
		{
			// modification des vecteurs de frequences de la ragle
			liTargetAttributeIndex = GetClassStats()->GetClass()->LookupAttribute(GetClassStats()->GetTargetAttributeName())->GetLoadIndex();
			sObjectTargetValue = kwoTempObject->GetSymbolValueAt(liTargetAttributeIndex);
			nObjectTargetValueIndex = krrInitRule->ComputeTargetValueIndex(sObjectTargetValue);
			require(nObjectTargetValueIndex >= 0 and nObjectTargetValueIndex < krrInitRule->GetTargetValueNumber());
			krrInitRule->SetInBodyTargetValueFrequencyAt(nObjectTargetValueIndex,
								     krrInitRule->GetInBodyTargetValueFrequencyAt(nObjectTargetValueIndex) + 1);
		}
	}
	nInformativeAttributeNumber = krrInitRule->ComputeInformativeAttributeNumber();
	// recalcul de tous les cous locaux pour le moment
	krrInitRule->InitializeAllCosts(GetRuleCosts());
	//ruleCosts->ComputeRuleTotalCost(krrInitRule);
	ruleCosts->ComputeRuleCompressionCoefficient(krrInitRule);
	assert(krrInitRule->CheckAllCosts(GetRuleCosts()));
	//krrInitRule->SetLevel(ComputeRuleLevel(database, krrInitRule));
}

///DDD
void KRRuleOptimizer::UpdateAllStatistics(KWDatabase* database, KRRule* krrInitRule)
{
	int nObjectIndex;
	int nIndex;
	int nAttributeIndex;
	KWObject* kwoTempObject;
	bool bCovered;
	KWAttribute* kwaAttribute;
	const KWClass* kwcSortClass;
	KRRAttribute* krrAttribute;
	KWLoadIndex liAttributeIndex;
	Continuous cCurrentValue;
	Symbol sCurrentValue;
	KRRPart* krrpBodyPart;
	KWLoadIndex liTargetAttributeIndex;
	Symbol sObjectTargetValue;
	Continuous cObjectTargetValue;
	int nObjectTargetValueIndex;
	int nInformativeAttributeNumber;

	// remise a zero des vecteurs inBody de frequence par classe avant update
	for (nIndex = 0; nIndex < krrInitRule->GetTargetValueNumber(); nIndex++)
	{
		krrInitRule->SetInBodyTargetValueFrequencyAt(nIndex, 0);
	}

	// remise a zero du vecteur de classe avant update (cas de la regression)
	if (krrInitRule->GetTargetType() == KWType::Continuous)
	{
		krrInitRule->UpdateTargetVectorsFrequency();
		ensure(krrInitRule->IsTargetVectorSizeValid());
		for (nIndex = 0; nIndex < krrInitRule->GetTargetAttribute()->GetPartNumber(); nIndex++)
		{
			krrInitRule->SetTargetValueFrequencyAt(nIndex, 0);
		}
	}

	for (nObjectIndex = 0; nObjectIndex < database->GetObjects()->GetSize(); nObjectIndex++)
	{
		kwoTempObject = cast(KWObject*, database->GetObjects()->GetAt(nObjectIndex));
		kwcSortClass = kwoTempObject->GetClass();
		bCovered = true;
		for (nAttributeIndex = 0; nAttributeIndex < krrInitRule->GetAttributeNumber(); nAttributeIndex++)
		{
			krrAttribute = krrInitRule->GetAttributeAt(nAttributeIndex);
			kwaAttribute = kwcSortClass->LookupAttribute(krrAttribute->GetAttributeName());
			liAttributeIndex = kwaAttribute->GetLoadIndex();
			krrpBodyPart = krrAttribute->GetPartAt(krrAttribute->GetBodyPartIndex());
			// cas categoriel : test de couverture
			if (krrAttribute->GetAttributeType() == KWType::Symbol and (not krrAttribute->GetAttributeTargetFunction()))
			{
				//krrAttribute->BuildIndexingStructure();
				sCurrentValue = kwoTempObject->GetSymbolValueAt(liAttributeIndex);
				//if (not (krrAttribute->LookupSymbolPart(sCurrentValue) == krrpBodyPart))
				if (not krrAttribute->IsInBodySymbol(sCurrentValue))
				// si valeur de l'attribut de l'objet n'est pas dans la partie active du corps
				{
					bCovered = false;
					//krrAttribute->DeleteIndexingStructure();
					break;
				}
			}
			// cas numerique : test de couverture
			if (krrAttribute->GetAttributeType() == KWType::Continuous and (not krrAttribute->GetAttributeTargetFunction()))
			{
				cCurrentValue = kwoTempObject->GetContinuousValueAt(liAttributeIndex);
				//if (not (krrpBodyPart->GetInterval()->GetLowerBound() < cCurrentValue
				//		and krrpBodyPart->GetInterval()->GetUpperBound() >= cCurrentValue))
				if (not krrAttribute->IsInBodyContinuous(cCurrentValue))
				// si valeur de l'attribut de l'objet n'est pas dans la partie active du corps
				{
					bCovered = false;
					break;
				}
			}
		}
		// Mise a jour du InBodyTargetValueFrequency pour les objects couverts par la regle
		if (bCovered) // Si l'objet est couvert par la regle
		{
			// Modification des vecteurs de frequences de la regle

			// classification supervisée
			if (krrInitRule->GetTargetType() == KWType::Symbol)
			{
				liTargetAttributeIndex = GetClassStats()->GetClass()->LookupAttribute(GetClassStats()->GetTargetAttributeName())->GetLoadIndex();
				sObjectTargetValue = kwoTempObject->GetSymbolValueAt(liTargetAttributeIndex);
				nObjectTargetValueIndex = krrInitRule->ComputeTargetValueIndex(sObjectTargetValue);
				require(nObjectTargetValueIndex >= 0 and nObjectTargetValueIndex < krrInitRule->GetTargetValueNumber());
				krrInitRule->SetInBodyTargetValueFrequencyAt(nObjectTargetValueIndex,
									     krrInitRule->GetInBodyTargetValueFrequencyAt(nObjectTargetValueIndex) + 1);
			}
			else // regression
			{
				require(krrInitRule->GetTargetType() == KWType::Continuous);
				/*krrInitRule->UpdateTargetVectorsFrequency();
				ensure(krrInitRule->IsTargetVectorSizeValid());*/
				liTargetAttributeIndex = GetClassStats()->GetClass()->LookupAttribute(GetClassStats()->GetTargetAttributeName())->GetLoadIndex();
				cObjectTargetValue = kwoTempObject->GetContinuousValueAt(liTargetAttributeIndex);
				nObjectTargetValueIndex = krrInitRule->ComputeContinuousTargetValuePartIndex(cObjectTargetValue);
				require(nObjectTargetValueIndex >= 0 and nObjectTargetValueIndex < krrInitRule->GetTargetAttribute()->GetPartNumber());
				krrInitRule->SetInBodyTargetValueFrequencyAt(nObjectTargetValueIndex,
									     krrInitRule->GetInBodyTargetValueFrequencyAt(nObjectTargetValueIndex) + 1);
			}
		}

		// Mise a jour du TargetValueFrequency pour tous les objets
		if (krrInitRule->GetTargetType() == KWType::Continuous)
		{
			liTargetAttributeIndex = GetClassStats()->GetClass()->LookupAttribute(GetClassStats()->GetTargetAttributeName())->GetLoadIndex();
			cObjectTargetValue = kwoTempObject->GetContinuousValueAt(liTargetAttributeIndex);
			nObjectTargetValueIndex = krrInitRule->ComputeContinuousTargetValuePartIndex(cObjectTargetValue);
			require(nObjectTargetValueIndex >= 0 and nObjectTargetValueIndex < krrInitRule->GetTargetAttribute()->GetPartNumber());
			krrInitRule->SetTargetValueFrequencyAt(nObjectTargetValueIndex,
							       krrInitRule->GetTargetValueFrequencyAt(nObjectTargetValueIndex) + 1);
		}
	}
	nInformativeAttributeNumber = krrInitRule->ComputeInformativeAttributeNumber();
	// recalcul de tous les cous locaux pour le moment
	krrInitRule->InitializeAllCosts(GetRuleCosts());
	//ruleCosts->ComputeRuleTotalCost(krrInitRule);
	ruleCosts->ComputeRuleCompressionCoefficient(krrInitRule);
	assert(krrInitRule->CheckAllCosts(GetRuleCosts()));
	//krrInitRule->SetLevel(ComputeRuleLevel(database, krrInitRule));
}

void KRRuleOptimizer::UpdateBodyStatistics(KWDatabase* database, KRRule* krrInitRule, IntVector* ivTargetValuesFrequencyDiff)
{
	//int nIndex;
	//int nInformativeAttributeNumber;

	//require(ivTargetValuesFrequencyDiff != NULL);
	//require(ivTargetValuesFrequencyDiff->GetSize() == krrInitRule->GetTargetValueNumber()
	//	or (krrInitRule->GetTargetAttribute() != NULL
	//	and ivTargetValuesFrequencyDiff->GetSize() == krrInitRule->GetTargetAttribute()->GetPartNumber()));

	//// intuitivement, ce qui est rajoute au corps N_X est enlevede l'exterieur du corps N_nonX
	//

	////// calcul de la somme des objets supplementaires dans le corps
	////nInBodyDiffSum = 0;
	////for (nIndex = 0; nIndex < ivTargetValuesFrequencyDiff->GetSize(); nIndex++)
	////{
	////	require(ivTargetValuesFrequencyDiff->GetAt(nIndex) >= 0);
	////	nInBodyDiffSum += ivTargetValuesFrequencyDiff->GetAt(nIndex);
	////}
	////
	////// calcul du differentiel de cout
	////// en quatre partie N_X, N_nonX, N_Xcj et N_nonXcj
	////dDiffBodyCost = 0;
	////dDiffOutBodyCost = 0;
	////
	////// partie du corps : ajout de log de N_X + J à N_X + J + nInBodySum
	////for (nIndex = krrInitRule->GetInBodyTotalFrequency() + krrInitRule->GetTargetValueNumber();
	////	nIndex <= krrInitRule->GetInBodyTotalFrequency() + krrInitRule->GetTargetValueNumber() + nInBodyDiffSum - 1;
	////	nIndex++)
	////{
	////	dDiffBodyCost += log(nIndex * 1.0);
	////}

	////// et hors du corps : soustraction de log de N_nonX + J à N_X + J + nInBodySum
	////nOutBodyFrequency = krrInitRule->GetTotalFrequency() - krrInitRule->GetInBodyTotalFrequency();
	////for (nIndex = nOutBodyFrequency + krrInitRule->GetTargetValueNumber() - 1;
	////	nIndex >= nOutBodyFrequency + krrInitRule->GetTargetValueNumber() - nInBodyDiffSum;
	////	nIndex--)
	////{
	////	dDiffOutBodyCost -= log(nIndex * 1.0);
	////}
	////// distribution des classes
	////for (nIndex = 0; nIndex < ivTargetValuesFrequencyDiff->GetSize(); nIndex++)
	////{
	////	// in body
	////	for (nCpt = krrInitRule->GetInBodyTargetValueFrequencyAt(nIndex) + 1;
	////		nCpt <= krrInitRule->GetInBodyTargetValueFrequencyAt(nIndex) + ivTargetValuesFrequencyDiff->GetAt(nIndex);
	////		nCpt++)
	////	{
	////		dDiffBodyCost -= log(nCpt * 1.0);
	////	}
	////
	////	// out body
	////	nOutBodyTargetValueFrequency = krrInitRule->GetTargetValueFrequencyAt(nIndex) - krrInitRule->GetInBodyTargetValueFrequencyAt(nIndex);
	////	for (nCpt = nOutBodyTargetValueFrequency;
	////		nCpt >= nOutBodyTargetValueFrequency - ivTargetValuesFrequencyDiff->GetAt(nIndex);
	////		nCpt--)
	////	{
	////		dDiffOutBodyCost += log(nCpt * 1.0);
	////	}
	////}
	////
	////// MAJ a jour des couts de la regle
	////krrInitRule->SetBodyCost(krrInitRule->GetBodyCost() + dDiffBodyCost);
	////krrInitRule->SetOutBodyCost(krrInitRule->GetOutBodyCost() + dDiffOutBodyCost);

	//// Modification des vecteurs de frequence de la regle
	//for (nIndex = 0; nIndex < ivTargetValuesFrequencyDiff->GetSize(); nIndex++)
	//{
	//	//cout << krrInitRule->GetInBodyTargetValueFrequencyAt(nIndex) << " : " << ivTargetValuesFrequencyDiff->GetAt(nIndex) << endl;
	//	assert(krrInitRule->GetInBodyTargetValueFrequencyAt(nIndex) >= 0);
	//	krrInitRule->SetInBodyTargetValueFrequencyAt(nIndex,
	//			krrInitRule->GetInBodyTargetValueFrequencyAt(nIndex) + ivTargetValuesFrequencyDiff->GetAt(nIndex));
	//	assert(krrInitRule->GetInBodyTargetValueFrequencyAt(nIndex) >= 0);
	//}
	//
	//nInformativeAttributeNumber = krrInitRule->ComputeInformativeAttributeNumber();
	//// recalcul de tous les cous locaux pour le moment
	//krrInitRule->InitializeAllCosts(GetRuleCosts());
	////ruleCosts->ComputeRuleTotalCost(krrInitRule);
	//ruleCosts->ComputeRuleCompressionCoefficient(krrInitRule);
	//assert(krrInitRule->CheckAllCosts(GetRuleCosts()));

	//DDD
	// UpdateAllStatistics pour le moment car bug
	// lors de l'optimisation de l'attribut cible
	// et de la mise a jour des statistiques lié a cette optimisation
	UpdateAllStatistics(database, krrInitRule);
}

void KRRuleOptimizer::UpdateAttributeStatistics(KWDatabase* database, KRRule* krrInitRule, KRRAttribute* attribute)
{
	// pour le passage de 3 a 2 parties
	// numerique
	//	double dDiffCost;
	double dAttributeCost;

	require(attribute->GetPartNumber() == 3);

	/*dDiffCost = log(2.0) + log(krrInitRule->GetTotalFrequency() - 3.0) + log(2.0);
	attribute->SetCost(attribute->GetCost() + dDiffCost);*/

	dAttributeCost = GetRuleCosts()->ComputeAttributeCost(attribute, 2);
	attribute->SetCost(dAttributeCost);

	//assert(krrInitRule->CheckAllCosts(GetRuleCosts()));
}

void KRRuleOptimizer::UpdateRuleStatistics(KWDatabase* database, KRRule* krrInitRule)
{
	double dRuleCost;

	// recalcul du cout local de la regle
	dRuleCost = GetRuleCosts()->ComputeRuleCost(krrInitRule, krrInitRule->ComputeInformativeAttributeNumber());
	krrInitRule->SetCost(dRuleCost);

	assert(krrInitRule->CheckAllCosts(GetRuleCosts()));
}

double KRRuleOptimizer::ComputeRuleLevel(KWDatabase* database, KRRule* krrInitRule)
{
	KRRuleCosts rCosts;
	double dRuleCost;
	double dNullRuleCost;

	rCosts.SetClassStats(GetClassStats());
	dRuleCost = rCosts.ComputeRuleTotalCost(krrInitRule);
	dNullRuleCost = rCosts.ComputeNullRuleCost(krrInitRule);

	return 1 - (dRuleCost / dNullRuleCost);
}

boolean KRRuleOptimizer::IsInBody(KWObject* kwoOject, KRRule* rule)
{
	boolean bOk;
	int nIndex;
	KWLoadIndex liAttributeLoadIndex;
	KRRAttribute* attribute;

	bOk = true;
	for (nIndex = 0; nIndex < rule->GetAttributeNumber(); nIndex++)
	{
		attribute = rule->GetAttributeAt(nIndex);
		//require(KWType::IsSimple(attribute->GetAttributeType()));
		liAttributeLoadIndex = GetClassStats()->GetClass()->LookupAttribute(attribute->GetAttributeName())->GetLoadIndex();
		// attribut numerique
		if (attribute->GetAttributeTargetFunction())
			continue;
		if (attribute->GetAttributeType() == KWType::Continuous and
		    not attribute->IsInBodyContinuous(kwoOject->GetContinuousValueAt(liAttributeLoadIndex)))
		{
			//cout << "Attribute failed : " << attribute->GetAttributeName() << endl;
			bOk = false;
			break;
		}
		//attribut symbolique
		if (attribute->GetAttributeType() == KWType::Symbol and
		    not attribute->IsInBodySymbol(kwoOject->GetSymbolValueAt(liAttributeLoadIndex)))
		{
			bOk = false;
			break;
		}
	}
	return bOk;
}

double KRRuleOptimizer::OptimizeRuleAttribute(KWDatabase* database,
					      KRRAttribute* krrAttribute,
					      KWObject* kwoInBody,
					      KRRule* krrCurrentRule)
{
	require(KWType::IsSimple(krrAttribute->GetAttributeType()));

	if (krrAttribute->GetAttributeType() == KWType::Continuous)
	{
		//cout << "Optimizing Attribute : " << krrAttribute->GetAttributeName() << endl;
		return OptimizeContinuousAttribute(database, krrAttribute, kwoInBody, krrCurrentRule);
	}
	else
	{
		assert(krrAttribute->GetAttributeType() == KWType::Symbol);
		//cout << "Optimizing Attribute : " << krrAttribute->GetAttributeName() << endl;
		return OptimizeSymbolAttribute(database, krrAttribute, kwoInBody, krrCurrentRule);
	}
}

double KRRuleOptimizer::OptimizeContinuousAttribute(KWDatabase* database,
						    KRRAttribute* krrAttribute,
						    KWObject* kwoInBody,
						    KRRule* krrCurrentRule)
{
	require(database != NULL);
	require(krrAttribute != NULL);
	require(kwoInBody != NULL);
	require(krrCurrentRule != NULL);
	require(krrAttribute->GetPartNumber() == 2 or krrAttribute->GetPartNumber() == 3);

	// Si deux parties
	if (krrAttribute->GetPartNumber() == 2)
	{
		Optimize2PartsContinuousAttribute(database, krrAttribute, kwoInBody, krrCurrentRule);
		//cout << "attribute optim ok?\n";
	}

	// Si trois parties
	if (krrAttribute->GetPartNumber() == 3)
	{
		Optimize3PartsContinuousAttribute(database, krrAttribute, kwoInBody, krrCurrentRule);
	}
	return krrCurrentRule->GetLevel();
}

void KRRuleOptimizer::Optimize2PartsContinuousAttribute(KWDatabase* database,
							KRRAttribute* krrAttribute,
							KWObject* kwoInBody,
							KRRule* krrCurrentRule)
{
	KWLoadIndex liAttributeLoadIndex;
	Continuous cValue;
	Continuous cPrecValue;
	Continuous cInBodyObjectValue;
	int nObjectIndex;
	KWObject* kwoObject;
	double dMaxRuleLevel;
	int nLowerIndex;
	Continuous cLowerBound;
	Continuous cOptimizedLowerBound;
	int nUpperIndex;
	Continuous cUpperBound;
	Continuous cOptimizedUpperBound;
	KRPreprocessedContinuousAttribute* krpcaAttribute;
	ObjectArray* oaWorkingInstances;
	IntVector ivTargetFrequencyValuesDiff;
	KWLoadIndex liTargetAttributeIndex;
	Symbol sObjectTargetValue;
	Continuous cObjectTargetValue;
	int nObjectTargetValueIndex;

	require(database != NULL);
	//kwoInBody->Write(cout);
	//krrCurrentRule->Write(cout);
	require(IsInBody(kwoInBody, krrCurrentRule));
	//require(KWType::IsSimple(krrAttribute->GetAttributeType()));
	require(krrAttribute->GetBodyPartIndex() == 0 or krrAttribute->GetBodyPartIndex() == 1);

	// level de la regle a ameliorer
	//dMaxRuleLevel = ruleCosts->ComputeRuleCompressionCoefficient(krrCurrentRule);
	dMaxRuleLevel = krrCurrentRule->GetLevel();

	// Index dans la base de l'attribut a optimiser
	liAttributeLoadIndex = GetClassStats()->GetClass()->LookupAttribute(krrAttribute->GetAttributeName())->GetLoadIndex();

	// valeur de l'objet reference dans le corps
	cInBodyObjectValue = kwoInBody->GetContinuousValueAt(liAttributeLoadIndex);

	// Tri de la base selon l'attribut a optimiser
	krpcaAttribute = cast(KRPreprocessedContinuousAttribute*, LookUpPreprocessedAttribute(krrAttribute->GetAttributeName()));

	krocCounters.ResetCounters();
	InitializeObjectsCounters(database, krrCurrentRule);
	ensure(CheckObjectCounters(database, krrCurrentRule));
	//cout << krocCounters;

	oaWorkingInstances = new ObjectArray;
	krpcaAttribute->PrepareWorkingInstances(krrAttribute, krrCurrentRule, &krocCounters, oaWorkingInstances);

	for (nObjectIndex = 0; nObjectIndex < oaWorkingInstances->GetSize(); nObjectIndex++)
	{
		kwoObject = cast(KWObject*, oaWorkingInstances->GetAt(nObjectIndex));
		if (kwoObject == kwoInBody)
			break;
	}

	assert(0 <= nObjectIndex and nObjectIndex < oaWorkingInstances->GetSize());

	cOptimizedLowerBound = KWContinuous::GetForbiddenValue();
	cOptimizedUpperBound = KWContinuous::GetForbiddenValue();
	kwoObject = cast(KWObject*, oaWorkingInstances->GetAt(nObjectIndex));

	//tableau des differentielles de frequences par classe
	if (krrCurrentRule->GetTargetType() == KWType::Symbol)
		ivTargetFrequencyValuesDiff.SetSize(krrCurrentRule->GetTargetValueNumber());
	if (krrCurrentRule->GetTargetType() == KWType::Continuous)
		ivTargetFrequencyValuesDiff.SetSize(krrCurrentRule->GetTargetAttribute()->GetPartNumber());
	ivTargetFrequencyValuesDiff.Initialize();

	// la parite 0 est active dans le corps
	if (krrAttribute->GetBodyPartIndex() == 0)
	{
		nUpperIndex = nObjectIndex;
		cPrecValue = kwoObject->GetContinuousValueAt(liAttributeLoadIndex);
		cOptimizedUpperBound = krrAttribute->GetPartAt(0)->GetInterval()->GetUpperBound();

		// Positionnement au point de depart de l'optimisation d'une borne juste a cote de l'objet
		while (nUpperIndex < oaWorkingInstances->GetSize())
		{
			kwoObject = cast(KWObject*, oaWorkingInstances->GetAt(nUpperIndex));
			cValue = kwoObject->GetContinuousValueAt(liAttributeLoadIndex);
			if (cValue > cPrecValue)
			{
				cUpperBound = KWContinuous::GetLowerMeanValue(cValue, cPrecValue);
				krrAttribute->GetPartAt(0)->GetInterval()->SetUpperBound(cUpperBound);
				krrAttribute->GetPartAt(1)->GetInterval()->SetLowerBound(cUpperBound);
				UpdateAllStatistics(database, krrCurrentRule);
				cPrecValue = cValue;
				break;
			}
			nUpperIndex++;
		}

		while (nUpperIndex < oaWorkingInstances->GetSize())
		{
			//kwoObject = cast(KWObject*, krpcaAttribute->GetObjects()->GetAt(nUpperIndex));
			kwoObject = cast(KWObject*, oaWorkingInstances->GetAt(nUpperIndex));
			cValue = kwoObject->GetContinuousValueAt(liAttributeLoadIndex);

			// MAJ de ivTargetFrequencyValuesDiff
			if (cValue == cPrecValue)
			{
				liTargetAttributeIndex = GetClassStats()->GetClass()->LookupAttribute(GetClassStats()->GetTargetAttributeName())->GetLoadIndex();
				if (krrCurrentRule->GetTargetType() == KWType::Symbol)
				{
					sObjectTargetValue = kwoObject->GetSymbolValueAt(liTargetAttributeIndex);
					nObjectTargetValueIndex = krrCurrentRule->ComputeTargetValueIndex(sObjectTargetValue);
					require(nObjectTargetValueIndex >= 0 and nObjectTargetValueIndex < krrCurrentRule->GetTargetValueNumber());
					ivTargetFrequencyValuesDiff.UpgradeAt(nObjectTargetValueIndex, 1);
				}
				if (krrCurrentRule->GetTargetType() == KWType::Continuous)
				{
					cObjectTargetValue = kwoObject->GetContinuousValueAt(liTargetAttributeIndex);
					nObjectTargetValueIndex = krrCurrentRule->ComputeContinuousTargetValuePartIndex(cObjectTargetValue);
					require(nObjectTargetValueIndex >= 0 and nObjectTargetValueIndex < krrCurrentRule->GetTargetAttribute()->GetPartNumber());
					ivTargetFrequencyValuesDiff.UpgradeAt(nObjectTargetValueIndex, 1);
				}
			}
			if (cValue > cPrecValue)
			{
				cUpperBound = KWContinuous::GetLowerMeanValue(cValue, cPrecValue);
				krrAttribute->GetPartAt(0)->GetInterval()->SetUpperBound(cUpperBound);
				krrAttribute->GetPartAt(1)->GetInterval()->SetLowerBound(cUpperBound);
				UpdateAllStatistics(database, krrCurrentRule);
				//DDD UpdateSubStatistics(oaWorkingInstances, krrCurrentRule);
				//DDD UpdateBodyStatistics(database, krrCurrentRule, &ivTargetFrequencyValuesDiff);
				if (krrCurrentRule->GetLevel() > dMaxRuleLevel)
				{
					dMaxRuleLevel = krrCurrentRule->GetLevel();
					cOptimizedUpperBound = cUpperBound;
					//krrCurrentRule->Write(cout);
				}
				// reinitialisation  du vecteur de difference et prise en compte de l'objet en cours
				ivTargetFrequencyValuesDiff.Initialize();
				liTargetAttributeIndex = GetClassStats()->GetClass()->LookupAttribute(GetClassStats()->GetTargetAttributeName())->GetLoadIndex();
				if (krrCurrentRule->GetTargetType() == KWType::Symbol)
				{
					sObjectTargetValue = kwoObject->GetSymbolValueAt(liTargetAttributeIndex);
					nObjectTargetValueIndex = krrCurrentRule->ComputeTargetValueIndex(sObjectTargetValue);
					require(nObjectTargetValueIndex >= 0 and nObjectTargetValueIndex < krrCurrentRule->GetTargetValueNumber());
					ivTargetFrequencyValuesDiff.UpgradeAt(nObjectTargetValueIndex, 1);
				}
				if (krrCurrentRule->GetTargetType() == KWType::Continuous)
				{
					cObjectTargetValue = kwoObject->GetContinuousValueAt(liTargetAttributeIndex);
					nObjectTargetValueIndex = krrCurrentRule->ComputeContinuousTargetValuePartIndex(cObjectTargetValue);
					require(nObjectTargetValueIndex >= 0 and nObjectTargetValueIndex < krrCurrentRule->GetTargetAttribute()->GetPartNumber());
					ivTargetFrequencyValuesDiff.UpgradeAt(nObjectTargetValueIndex, 1);
				}

				cPrecValue = cValue;
			}
			nUpperIndex++;
		}

		// cas borne extreme ~> vers un seul intervalle donc attribut non-informatif
		cUpperBound = KRRInterval::GetMaxUpperBound();
		krrAttribute->GetPartAt(0)->GetInterval()->SetUpperBound(cUpperBound);
		krrAttribute->GetPartAt(1)->GetInterval()->SetLowerBound(cUpperBound);
		UpdateAllStatistics(database, krrCurrentRule); // calcul fausse par 2 intervalles au lieu de 1.
		//DDD UpdateSubStatistics(oaWorkingInstances, krrCurrentRule);
		//DDD UpdateBodyStatistics(database, krrCurrentRule, &ivTargetFrequencyValuesDiff);
		UpdateRuleStatistics(database, krrCurrentRule); // car changement du nb d'attributs informatifs
		if (krrCurrentRule->GetLevel() >= dMaxRuleLevel)
		{
			dMaxRuleLevel = krrCurrentRule->GetLevel();
			cOptimizedUpperBound = cUpperBound;
			//krrCurrentRule->Write(cout);
		}

		krrAttribute->GetPartAt(0)->GetInterval()->SetUpperBound(cOptimizedUpperBound);
		krrAttribute->GetPartAt(1)->GetInterval()->SetLowerBound(cOptimizedUpperBound);
	}
	else // alors krrAttribute->GetBodyPartIndex() ==1
	{
		nLowerIndex = nObjectIndex;
		cPrecValue = kwoObject->GetContinuousValueAt(liAttributeLoadIndex);
		cOptimizedLowerBound = krrAttribute->GetPartAt(1)->GetInterval()->GetLowerBound();

		// Positionnement au point de depart de l'optimisation d'une borne juste a cote de l'objet
		while (nLowerIndex >= 0)
		{
			kwoObject = cast(KWObject*, oaWorkingInstances->GetAt(nLowerIndex));
			cValue = kwoObject->GetContinuousValueAt(liAttributeLoadIndex);
			if (cValue < cPrecValue)
			{
				cLowerBound = KWContinuous::GetLowerMeanValue(cValue, cPrecValue);
				krrAttribute->GetPartAt(1)->GetInterval()->SetLowerBound(cLowerBound);
				krrAttribute->GetPartAt(0)->GetInterval()->SetUpperBound(cLowerBound);
				//UpdateSubStatistics(oaWorkingInstances, krrCurrentRule);
				UpdateAllStatistics(database, krrCurrentRule);
				cPrecValue = cValue;
				break;
			}
			nLowerIndex--;
		}
		while (nLowerIndex >= 0)
		{
			kwoObject = cast(KWObject*, oaWorkingInstances->GetAt(nLowerIndex));
			cValue = kwoObject->GetContinuousValueAt(liAttributeLoadIndex);

			// MAJ de ivTargetFrequencyValuesDiff
			if (cValue == cPrecValue)
			{
				liTargetAttributeIndex = GetClassStats()->GetClass()->LookupAttribute(GetClassStats()->GetTargetAttributeName())->GetLoadIndex();
				if (krrCurrentRule->GetTargetType() == KWType::Symbol)
				{
					sObjectTargetValue = kwoObject->GetSymbolValueAt(liTargetAttributeIndex);
					nObjectTargetValueIndex = krrCurrentRule->ComputeTargetValueIndex(sObjectTargetValue);
					require(nObjectTargetValueIndex >= 0 and nObjectTargetValueIndex < krrCurrentRule->GetTargetValueNumber());
					ivTargetFrequencyValuesDiff.UpgradeAt(nObjectTargetValueIndex, 1);
				}
				if (krrCurrentRule->GetTargetType() == KWType::Continuous)
				{
					cObjectTargetValue = kwoObject->GetContinuousValueAt(liTargetAttributeIndex);
					nObjectTargetValueIndex = krrCurrentRule->ComputeContinuousTargetValuePartIndex(cObjectTargetValue);
					require(nObjectTargetValueIndex >= 0 and nObjectTargetValueIndex < krrCurrentRule->GetTargetAttribute()->GetPartNumber());
					ivTargetFrequencyValuesDiff.UpgradeAt(nObjectTargetValueIndex, 1);
				}
			}

			if (cValue < cPrecValue)
			{
				cLowerBound = KWContinuous::GetLowerMeanValue(cValue, cPrecValue);
				krrAttribute->GetPartAt(1)->GetInterval()->SetLowerBound(cLowerBound);
				krrAttribute->GetPartAt(0)->GetInterval()->SetUpperBound(cLowerBound);
				UpdateAllStatistics(database, krrCurrentRule);
				//DDD UpdateSubStatistics(oaWorkingInstances, krrCurrentRule);
				//DDD UpdateBodyStatistics(database, krrCurrentRule, &ivTargetFrequencyValuesDiff);
				if (krrCurrentRule->GetLevel() > dMaxRuleLevel)
				{
					dMaxRuleLevel = krrCurrentRule->GetLevel();
					cOptimizedLowerBound = cLowerBound;
					//krrCurrentRule->Write(cout);
				}
				// reinitialisation  du vecteur de difference et prise en compte de l'objet en cours
				ivTargetFrequencyValuesDiff.Initialize();
				liTargetAttributeIndex = GetClassStats()->GetClass()->LookupAttribute(GetClassStats()->GetTargetAttributeName())->GetLoadIndex();
				if (krrCurrentRule->GetTargetType() == KWType::Symbol)
				{
					sObjectTargetValue = kwoObject->GetSymbolValueAt(liTargetAttributeIndex);
					nObjectTargetValueIndex = krrCurrentRule->ComputeTargetValueIndex(sObjectTargetValue);
					require(nObjectTargetValueIndex >= 0 and nObjectTargetValueIndex < krrCurrentRule->GetTargetValueNumber());
					ivTargetFrequencyValuesDiff.UpgradeAt(nObjectTargetValueIndex, 1);
				}
				if (krrCurrentRule->GetTargetType() == KWType::Continuous)
				{
					cObjectTargetValue = kwoObject->GetContinuousValueAt(liTargetAttributeIndex);
					nObjectTargetValueIndex = krrCurrentRule->ComputeContinuousTargetValuePartIndex(cObjectTargetValue);
					require(nObjectTargetValueIndex >= 0 and nObjectTargetValueIndex < krrCurrentRule->GetTargetAttribute()->GetPartNumber());
					ivTargetFrequencyValuesDiff.UpgradeAt(nObjectTargetValueIndex, 1);
				}

				cPrecValue = cValue;
			}
			nLowerIndex--;
		}

		// cas borne extreme ~> 1 intervalle
		cLowerBound = KRRInterval::GetMinLowerBound();
		krrAttribute->GetPartAt(0)->GetInterval()->SetUpperBound(cLowerBound);
		krrAttribute->GetPartAt(1)->GetInterval()->SetLowerBound(cLowerBound);
		UpdateAllStatistics(database, krrCurrentRule); // calcul fausse par 2 intervalles au lieu de 1.
		//DDD UpdateSubStatistics(oaWorkingInstances, krrCurrentRule);
		//DDD UpdateBodyStatistics(database, krrCurrentRule, &ivTargetFrequencyValuesDiff);
		UpdateRuleStatistics(database, krrCurrentRule); // car changement du nb d'attributs informatifs
		if (krrCurrentRule->GetLevel() >= dMaxRuleLevel)
		{
			dMaxRuleLevel = krrCurrentRule->GetLevel();
			cOptimizedLowerBound = cLowerBound;
			//krrCurrentRule->Write(cout);
		}

		krrAttribute->GetPartAt(0)->GetInterval()->SetUpperBound(cOptimizedLowerBound);
		krrAttribute->GetPartAt(1)->GetInterval()->SetLowerBound(cOptimizedLowerBound);
	}

	//regularisation si attribut partie vide
	if (cOptimizedLowerBound == KRRInterval::GetMinLowerBound())
	{
		krrAttribute->DeletePart(krrAttribute->GetPartAt(0));
		krrAttribute->SetBodyPartIndex(0);
	}
	if (cOptimizedUpperBound == KRRInterval::GetMaxUpperBound())
		krrAttribute->DeletePart(krrAttribute->GetPartAt(1));

	// mise jour sur toute la base obligatoire avec la nouvelle borne ?
	//DDD
	//krrCurrentRule->DeleteNonInformativeAttributes();
	UpdateAllStatistics(database, krrCurrentRule);
	//UpdateSubStatistics(oaWorkingInstances, krrCurrentRule);

	delete oaWorkingInstances;
}

void KRRuleOptimizer::Optimize3PartsContinuousAttribute(KWDatabase* database,
							KRRAttribute* krrAttribute,
							KWObject* kwoInBody,
							KRRule* krrCurrentRule)
{
	KWLoadIndex liAttributeLoadIndex;
	Continuous cValue;
	Continuous cPrecValue;
	Continuous cInBodyObjectValue;
	int nObjectIndex;
	KWObject* kwoObject;
	double dMaxRuleLevel;
	int nLowerIndex;
	Continuous cLowerBound;
	Continuous cOptimizedLowerBound;
	int nUpperIndex;
	Continuous cUpperBound;
	Continuous cOptimizedUpperBound;
	KRPreprocessedContinuousAttribute* krpcaAttribute;
	ObjectArray* oaWorkingInstances;
	IntVector ivTargetFrequencyValuesDiff;
	KWLoadIndex liTargetAttributeIndex;
	Symbol sObjectTargetValue;
	Continuous cObjectTargetValue;
	int nObjectTargetValueIndex;

	//kwoInBody->Write(cout);
	//krrCurrentRule->Write(cout);
	require(IsInBody(kwoInBody, krrCurrentRule));

	// level de la regle a ameliorer
	dMaxRuleLevel = krrCurrentRule->GetLevel();

	// Index dans la base de l'attribut a optimiser
	liAttributeLoadIndex = GetClassStats()->GetClass()->LookupAttribute(krrAttribute->GetAttributeName())->GetLoadIndex();

	// valeur de l'objet reference dans le corps
	cInBodyObjectValue = kwoInBody->GetContinuousValueAt(liAttributeLoadIndex);
	assert(krrAttribute->IsInBodyContinuous(cInBodyObjectValue));

	// Tri de la base selon l'attribut a optimiser
	krpcaAttribute = cast(KRPreprocessedContinuousAttribute*, LookUpPreprocessedAttribute(krrAttribute->GetAttributeName()));

	krocCounters.ResetCounters();
	//cout << krocCounters;
	InitializeObjectsCounters(database, krrCurrentRule);
	assert(krocCounters.GetBodyCountAt(kwoInBody) == krrCurrentRule->GetAttributeNumber());
	ensure(CheckObjectCounters(database, krrCurrentRule));
	//cout << krocCounters;
	oaWorkingInstances = new ObjectArray;
	krpcaAttribute->PrepareWorkingInstances(krrAttribute, krrCurrentRule, &krocCounters, oaWorkingInstances);
	assert(CheckObjectCounters(database, krrCurrentRule));

	// Calcul de l'index de l'objet reference dans la nouvelle structure oaWorkingInstances
	for (nObjectIndex = 0; nObjectIndex < oaWorkingInstances->GetSize(); nObjectIndex++)
	{
		kwoObject = cast(KWObject*, oaWorkingInstances->GetAt(nObjectIndex));
		if (kwoObject == kwoInBody)
			break;
	}
	//cout << "oaWorking Instances Size : " << oaWorkingInstances->GetSize() << " ; nObjectIndex : " << nObjectIndex << endl;
	assert(0 <= nObjectIndex and nObjectIndex < oaWorkingInstances->GetSize());

	//krrCurrentRule->Write(cout);

	//tableau des differentielles de frequences par classe
	if (krrCurrentRule->GetTargetType() == KWType::Symbol)
		ivTargetFrequencyValuesDiff.SetSize(krrCurrentRule->GetTargetValueNumber());
	if (krrCurrentRule->GetTargetType() == KWType::Continuous)
		ivTargetFrequencyValuesDiff.SetSize(krrCurrentRule->GetTargetAttribute()->GetPartNumber());
	ivTargetFrequencyValuesDiff.Initialize();

	////////////
	// recherche de la borne inferieure optimale
	nLowerIndex = nObjectIndex;
	kwoObject = cast(KWObject*, oaWorkingInstances->GetAt(nObjectIndex));
	cPrecValue = kwoObject->GetContinuousValueAt(liAttributeLoadIndex);
	cOptimizedLowerBound = krrAttribute->GetPartAt(1)->GetInterval()->GetLowerBound();

	// Positionnement au point de depart de l'optimisation d'une borne juste a cote de l'objet
	while (nLowerIndex >= 0)
	{
		kwoObject = cast(KWObject*, oaWorkingInstances->GetAt(nLowerIndex));
		cValue = kwoObject->GetContinuousValueAt(liAttributeLoadIndex);
		if (cValue < cPrecValue)
		{
			cLowerBound = KWContinuous::GetLowerMeanValue(cValue, cPrecValue);
			krrAttribute->GetPartAt(1)->GetInterval()->SetLowerBound(cLowerBound);
			krrAttribute->GetPartAt(0)->GetInterval()->SetUpperBound(cLowerBound);
			//UpdateSubStatistics(oaWorkingInstances, krrCurrentRule);
			UpdateAllStatistics(database, krrCurrentRule);
			cPrecValue = cValue;
			break;
		}
		nLowerIndex--;
	}

	while (nLowerIndex >= 0)
	{
		kwoObject = cast(KWObject*, oaWorkingInstances->GetAt(nLowerIndex));
		cValue = kwoObject->GetContinuousValueAt(liAttributeLoadIndex);

		// MAJ de ivTargetFrequencyValuesDiff
		if (cValue == cPrecValue)
		{
			liTargetAttributeIndex = GetClassStats()->GetClass()->LookupAttribute(GetClassStats()->GetTargetAttributeName())->GetLoadIndex();
			if (krrCurrentRule->GetTargetType() == KWType::Symbol)
			{
				sObjectTargetValue = kwoObject->GetSymbolValueAt(liTargetAttributeIndex);
				nObjectTargetValueIndex = krrCurrentRule->ComputeTargetValueIndex(sObjectTargetValue);
				require(nObjectTargetValueIndex >= 0 and nObjectTargetValueIndex < krrCurrentRule->GetTargetValueNumber());
				ivTargetFrequencyValuesDiff.UpgradeAt(nObjectTargetValueIndex, 1);
			}
			if (krrCurrentRule->GetTargetType() == KWType::Continuous)
			{
				cObjectTargetValue = kwoObject->GetContinuousValueAt(liTargetAttributeIndex);
				nObjectTargetValueIndex = krrCurrentRule->ComputeContinuousTargetValuePartIndex(cObjectTargetValue);
				require(nObjectTargetValueIndex >= 0 and nObjectTargetValueIndex < krrCurrentRule->GetTargetAttribute()->GetPartNumber());
				ivTargetFrequencyValuesDiff.UpgradeAt(nObjectTargetValueIndex, 1);
			}
		}

		if (cValue < cPrecValue)
		{
			cLowerBound = KWContinuous::GetLowerMeanValue(cValue, cPrecValue);
			krrAttribute->GetPartAt(1)->GetInterval()->SetLowerBound(cLowerBound);
			krrAttribute->GetPartAt(0)->GetInterval()->SetUpperBound(cLowerBound);
			UpdateAllStatistics(database, krrCurrentRule);
			//DDD UpdateSubStatistics(oaWorkingInstances, krrCurrentRule);
			//DDD UpdateBodyStatistics(database, krrCurrentRule, &ivTargetFrequencyValuesDiff);
			if (krrCurrentRule->GetLevel() > dMaxRuleLevel)
			{
				dMaxRuleLevel = krrCurrentRule->GetLevel();
				cOptimizedLowerBound = cLowerBound;
				//krrCurrentRule->Write(cout);
			}
			// reinitialisation  du vecteur de difference et prise en compte de l'objet en cours
			ivTargetFrequencyValuesDiff.Initialize();
			liTargetAttributeIndex = GetClassStats()->GetClass()->LookupAttribute(GetClassStats()->GetTargetAttributeName())->GetLoadIndex();
			if (krrCurrentRule->GetTargetType() == KWType::Symbol)
			{
				sObjectTargetValue = kwoObject->GetSymbolValueAt(liTargetAttributeIndex);
				nObjectTargetValueIndex = krrCurrentRule->ComputeTargetValueIndex(sObjectTargetValue);
				require(nObjectTargetValueIndex >= 0 and nObjectTargetValueIndex < krrCurrentRule->GetTargetValueNumber());
				ivTargetFrequencyValuesDiff.UpgradeAt(nObjectTargetValueIndex, 1);
			}
			if (krrCurrentRule->GetTargetType() == KWType::Continuous)
			{
				cObjectTargetValue = kwoObject->GetContinuousValueAt(liTargetAttributeIndex);
				nObjectTargetValueIndex = krrCurrentRule->ComputeContinuousTargetValuePartIndex(cObjectTargetValue);
				require(nObjectTargetValueIndex >= 0 and nObjectTargetValueIndex < krrCurrentRule->GetTargetAttribute()->GetPartNumber());
				ivTargetFrequencyValuesDiff.UpgradeAt(nObjectTargetValueIndex, 1);
			}

			cPrecValue = cValue;
		}
		nLowerIndex--;
	}
	// cas borne extreme ~> 2 intervalles dont 1 apres l'objet
	cLowerBound = KRRInterval::GetMinLowerBound();
	krrAttribute->GetPartAt(1)->GetInterval()->SetLowerBound(cLowerBound);
	krrAttribute->GetPartAt(0)->GetInterval()->SetUpperBound(cLowerBound);
	UpdateAllStatistics(database, krrCurrentRule); // calcul fausse par 3 intervalles au lieu de 2.
	//DDD UpdateSubStatistics(oaWorkingInstances, krrCurrentRule);
	//DDD UpdateBodyStatistics(database, krrCurrentRule, &ivTargetFrequencyValuesDiff);
	UpdateAttributeStatistics(database, krrCurrentRule, krrAttribute);
	if (krrCurrentRule->GetLevel() >= dMaxRuleLevel)
	{
		dMaxRuleLevel = krrCurrentRule->GetLevel();
		cOptimizedLowerBound = cLowerBound;
		//krrCurrentRule->Write(cout);
	}

	krrAttribute->GetPartAt(1)->GetInterval()->SetLowerBound(cOptimizedLowerBound);
	krrAttribute->GetPartAt(0)->GetInterval()->SetUpperBound(cOptimizedLowerBound);

	if (cOptimizedLowerBound == KRRInterval::GetMinLowerBound())
	{
		krrAttribute->DeletePart(krrAttribute->GetPartAt(0));
		krrAttribute->SetBodyPartIndex(0);
		//cout << "extreme lower bound set \n";
	}

	UpdateAllStatistics(database, krrCurrentRule);

	require(IsInBody(kwoInBody, krrCurrentRule));
	//krrCurrentRule->Write(cout);

	if (krrAttribute->GetPartNumber() == 2)
	{
		delete oaWorkingInstances;
		Optimize2PartsContinuousAttribute(database, krrAttribute, kwoInBody, krrCurrentRule);
	}
	else
	{

		///////////
		//Re initalisation avant d'optimiser l'autre borne
		krocCounters.ResetCounters();
		//cout << krocCounters << endl;
		InitializeObjectsCounters(database, krrCurrentRule);
		ensure(CheckObjectCounters(database, krrCurrentRule));
		//cout << krocCounters << endl;
		oaWorkingInstances->SetSize(0);
		delete oaWorkingInstances;
		//oaWorkingInstances = NULL;
		oaWorkingInstances = new ObjectArray;
		krpcaAttribute->PrepareWorkingInstances(krrAttribute, krrCurrentRule, &krocCounters, oaWorkingInstances);
		assert(CheckObjectCounters(database, krrCurrentRule));
		// recherche de l'index de l'object dans la nouvelle structure
		for (nObjectIndex = 0; nObjectIndex < oaWorkingInstances->GetSize(); nObjectIndex++)
		{
			kwoObject = cast(KWObject*, oaWorkingInstances->GetAt(nObjectIndex));
			if (kwoObject == kwoInBody)
				break;
		}
		//cout << "oaWorking Instances Size : " << oaWorkingInstances->GetSize() << " ; nObjectIndex : " << nObjectIndex << endl;
		assert(0 <= nObjectIndex and nObjectIndex < oaWorkingInstances->GetSize());
		//tableau des differentielles de frequences par classe
		ivTargetFrequencyValuesDiff.Initialize();

		////////
		// recherche de la borne superieure optimale
		nUpperIndex = nObjectIndex;
		kwoObject = cast(KWObject*, oaWorkingInstances->GetAt(nObjectIndex));
		cPrecValue = kwoObject->GetContinuousValueAt(liAttributeLoadIndex);
		cOptimizedUpperBound = krrAttribute->GetPartAt(1)->GetInterval()->GetUpperBound();

		// Positionnement au point de depart de l'optimisation d'une borne juste a cote de l'objet
		while (nUpperIndex < oaWorkingInstances->GetSize())
		{
			kwoObject = cast(KWObject*, oaWorkingInstances->GetAt(nUpperIndex));
			cValue = kwoObject->GetContinuousValueAt(liAttributeLoadIndex);
			if (cValue > cPrecValue)
			{
				cUpperBound = KWContinuous::GetLowerMeanValue(cValue, cPrecValue);
				krrAttribute->GetPartAt(1)->GetInterval()->SetUpperBound(cUpperBound);
				krrAttribute->GetPartAt(2)->GetInterval()->SetLowerBound(cUpperBound);
				UpdateAllStatistics(database, krrCurrentRule);
				cPrecValue = cValue;
				break;
			}
			nUpperIndex++;
		}

		while (nUpperIndex < oaWorkingInstances->GetSize())
		{
			kwoObject = cast(KWObject*, oaWorkingInstances->GetAt(nUpperIndex));
			cValue = kwoObject->GetContinuousValueAt(liAttributeLoadIndex);

			// MAJ de ivTargetFrequencyValuesDiff
			if (cValue == cPrecValue)
			{
				liTargetAttributeIndex = GetClassStats()->GetClass()->LookupAttribute(GetClassStats()->GetTargetAttributeName())->GetLoadIndex();
				if (krrCurrentRule->GetTargetType() == KWType::Symbol)
				{
					sObjectTargetValue = kwoObject->GetSymbolValueAt(liTargetAttributeIndex);
					nObjectTargetValueIndex = krrCurrentRule->ComputeTargetValueIndex(sObjectTargetValue);
					require(nObjectTargetValueIndex >= 0 and nObjectTargetValueIndex < krrCurrentRule->GetTargetValueNumber());
					ivTargetFrequencyValuesDiff.UpgradeAt(nObjectTargetValueIndex, 1);
				}
				if (krrCurrentRule->GetTargetType() == KWType::Continuous)
				{
					cObjectTargetValue = kwoObject->GetContinuousValueAt(liTargetAttributeIndex);
					nObjectTargetValueIndex = krrCurrentRule->ComputeContinuousTargetValuePartIndex(cObjectTargetValue);
					require(nObjectTargetValueIndex >= 0 and nObjectTargetValueIndex < krrCurrentRule->GetTargetAttribute()->GetPartNumber());
					ivTargetFrequencyValuesDiff.UpgradeAt(nObjectTargetValueIndex, 1);
				}
			}

			if (cValue > cPrecValue)
			{
				cUpperBound = KWContinuous::GetLowerMeanValue(cValue, cPrecValue);
				krrAttribute->GetPartAt(1)->GetInterval()->SetUpperBound(cUpperBound);
				krrAttribute->GetPartAt(2)->GetInterval()->SetLowerBound(cUpperBound);
				UpdateAllStatistics(database, krrCurrentRule);
				//DDD UpdateSubStatistics(oaWorkingInstances, krrCurrentRule);
				//DDD UpdateBodyStatistics(database, krrCurrentRule, &ivTargetFrequencyValuesDiff);
				if (krrCurrentRule->GetLevel() > dMaxRuleLevel)
				{
					dMaxRuleLevel = krrCurrentRule->GetLevel();
					cOptimizedUpperBound = cUpperBound;
					//krrCurrentRule->Write(cout);
				}
				// reinitialisation  du vecteur de difference et prise en compte de l'objet en cours
				ivTargetFrequencyValuesDiff.Initialize();
				liTargetAttributeIndex = GetClassStats()->GetClass()->LookupAttribute(GetClassStats()->GetTargetAttributeName())->GetLoadIndex();
				if (krrCurrentRule->GetTargetType() == KWType::Symbol)
				{
					sObjectTargetValue = kwoObject->GetSymbolValueAt(liTargetAttributeIndex);
					nObjectTargetValueIndex = krrCurrentRule->ComputeTargetValueIndex(sObjectTargetValue);
					require(nObjectTargetValueIndex >= 0 and nObjectTargetValueIndex < krrCurrentRule->GetTargetValueNumber());
					ivTargetFrequencyValuesDiff.UpgradeAt(nObjectTargetValueIndex, 1);
				}
				if (krrCurrentRule->GetTargetType() == KWType::Continuous)
				{
					cObjectTargetValue = kwoObject->GetContinuousValueAt(liTargetAttributeIndex);
					nObjectTargetValueIndex = krrCurrentRule->ComputeContinuousTargetValuePartIndex(cObjectTargetValue);
					require(nObjectTargetValueIndex >= 0 and nObjectTargetValueIndex < krrCurrentRule->GetTargetAttribute()->GetPartNumber());
					ivTargetFrequencyValuesDiff.UpgradeAt(nObjectTargetValueIndex, 1);
				}

				cPrecValue = cValue;
			}
			nUpperIndex++;
		}
		// cas borne extreme ~> 2 intervalles dont 1 avant l'objet
		cUpperBound = KRRInterval::GetMaxUpperBound();
		krrAttribute->GetPartAt(1)->GetInterval()->SetUpperBound(cUpperBound);
		krrAttribute->GetPartAt(2)->GetInterval()->SetLowerBound(cUpperBound);
		UpdateAllStatistics(database, krrCurrentRule); // calcul fausse par 3 intervalles au lieu de 2.
		//DDD UpdateSubStatistics(oaWorkingInstances, krrCurrentRule);
		//DDD UpdateBodyStatistics(database, krrCurrentRule, &ivTargetFrequencyValuesDiff);
		UpdateAttributeStatistics(database, krrCurrentRule, krrAttribute);
		if (krrCurrentRule->GetLevel() >= dMaxRuleLevel)
		{
			dMaxRuleLevel = krrCurrentRule->GetLevel();
			cOptimizedUpperBound = cUpperBound;
			//krrCurrentRule->Write(cout);
		}

		krrAttribute->GetPartAt(1)->GetInterval()->SetUpperBound(cOptimizedUpperBound);
		krrAttribute->GetPartAt(2)->GetInterval()->SetLowerBound(cOptimizedUpperBound);

		//krrCurrentRule->Write(cout);

		// regularisation si parties vides
		if (cOptimizedUpperBound == KRRInterval::GetMaxUpperBound())
		{
			krrAttribute->DeletePart(krrAttribute->GetPartAt(2));
			//cout << "extreme upper bound set\n";
		}

		//krrCurrentRule->Write(cout);
		//DDD
		UpdateAllStatistics(database, krrCurrentRule);
		//UpdateSubStatistics(oaWorkingInstances, krrCurrentRule);

		//krrCurrentRule->Write(cout);
		require(IsInBody(kwoInBody, krrCurrentRule));

		delete oaWorkingInstances;

	} // end else 2 parts
}

double KRRuleOptimizer::OptimizeSymbolAttribute(KWDatabase* database,
						KRRAttribute* krrAttribute,
						KWObject* kwoInBody,
						KRRule* krrCurrentRule)
{
	KWLoadIndex liAttributeLoadIndex;
	double dMaxRuleLevel;
	Symbol sInBodyObjectValue;
	SymbolVector svInBodyValues;
	SymbolVector svOutBodyValues;
	SymbolVector svTemp;
	KRRValueSet* krrvsInBody;
	KRRValueSet* krrvsOutBody;
	KRRValue* value;
	int nRun;
	int nRandPart;
	KRPreprocessedSymbolAttribute* krpsaAttribute;
	KWObject* kwoObject;
	IntVector ivTargetFrequencyValuesDiff;
	int nIndex;
	Symbol sMovingValue;
	KWLoadIndex liTargetAttributeIndex;
	Symbol sObjectTargetValue;
	Continuous cObjectTargetValue;
	int nObjectTargetValueIndex;
	Symbol sValue;

	require(IsInBody(kwoInBody, krrCurrentRule));
	require(krrAttribute->GetAttributeType() == KWType::Symbol);
	//require(krrAttribute->GetPartNumber() == 2);

	// level de la regle a ameliorer
	dMaxRuleLevel = krrCurrentRule->GetLevel();

	// Index dans la base de l'attribut a optimiser
	//cout << "Optimizing attribute : " << krrAttribute->GetAttributeName() << endl;
	liAttributeLoadIndex = GetClassStats()->GetClass()->LookupAttribute(krrAttribute->GetAttributeName())->GetLoadIndex();

	// index de la classe dans la base
	liTargetAttributeIndex = GetClassStats()->GetClass()->LookupAttribute(GetClassStats()->GetTargetAttributeName())->GetLoadIndex();

	// valeur de l'objet reference dans le corps
	sInBodyObjectValue = kwoInBody->GetSymbolValueAt(liAttributeLoadIndex);
	assert(krrAttribute->IsInBodySymbol(sInBodyObjectValue));

	//// Tri de la base selon l'attribut a optimiser
	krpsaAttribute = cast(KRPreprocessedSymbolAttribute*, LookUpPreprocessedAttribute(krrAttribute->GetAttributeName()));

	krocCounters.ResetCounters();
	//cout << krocCounters;
	InitializeObjectsCounters(database, krrCurrentRule);
	assert(krocCounters.GetBodyCountAt(kwoInBody) == krrCurrentRule->GetAttributeNumber());
	ensure(CheckObjectCounters(database, krrCurrentRule));
	//cout << krocCounters;
	//oaWorkingInstances = new ObjectArray;
	//krpsaAttribute->PrepareWorkingInstances(krrAttribute, krrCurrentRule, &krocCounters, oaWorkingInstances);
	//assert(CheckObjectCounters(database, krrCurrentRule));
	//
	//// Calcul de l'index de l'objet reference dans la nouvelle structure oaWorkingInstances
	//for (nObjectIndex = 0; nObjectIndex < oaWorkingInstances->GetSize(); nObjectIndex++)
	//{
	//	kwoObject = cast(KWObject*, oaWorkingInstances->GetAt(nObjectIndex));
	//	if (kwoObject == kwoInBody)
	//		break;
	//}
	//cout << "oaWorking Instances Size : " << oaWorkingInstances->GetSize() << " ; nObjectIndex : " << nObjectIndex << endl;
	//assert(0 <= nObjectIndex and nObjectIndex < oaWorkingInstances->GetSize());
	//delete oaWorkingInstances;

	//tableau des differentielles de frequences par classe

	if (krrCurrentRule->GetTargetType() == KWType::Symbol)
		ivTargetFrequencyValuesDiff.SetSize(krrCurrentRule->GetTargetValueNumber());
	else
		ivTargetFrequencyValuesDiff.SetSize(krrCurrentRule->GetTargetAttribute()->GetPartNumber());
	ivTargetFrequencyValuesDiff.Initialize();

	//krrCurrentRule->Write(cout);
	/*krrAttribute->WriteParts(cout);
	krrCurrentRule->WriteContingencyTable(cout);
	cout << krrCurrentRule->GetLevel() << endl;*/

	////DDD a mettre a jour
	for (nRun = 1; nRun <= 4; nRun++)
	{
		krocCounters.ResetCounters();
		InitializeObjectsCounters(database, krrCurrentRule);
		assert(krocCounters.GetBodyCountAt(kwoInBody) == krrCurrentRule->GetAttributeNumber());
		ensure(CheckObjectCounters(database, krrCurrentRule));
		//cout << krocCounters << endl;

		//cout << "\n*****\nRun : #" << nRun << endl;
		ivTargetFrequencyValuesDiff.Initialize();
		if (krrAttribute->GetPartNumber() == 2)
		{
			// creation des SymbolVector
			// symbolvector pour le corps
			krrvsInBody = krrAttribute->GetPartAt(0)->GetValueSet();
			value = krrvsInBody->GetHeadValue();
			while (value != NULL)
			{
				if (value->GetValue() != sInBodyObjectValue and value->GetValue() != Symbol::GetStarValue())
				{
					svInBodyValues.Add(value->GetValue());
				}
				krrvsInBody->GetNextValue(value);
			}
			svInBodyValues.Shuffle();
			// symbolvector pour le hors du corps
			krrvsOutBody = krrAttribute->GetPartAt(1)->GetValueSet();
			value = krrvsOutBody->GetHeadValue();
			while (value != NULL)
			{
				svOutBodyValues.Add(value->GetValue());
				krrvsOutBody->GetNextValue(value);
			}
			svOutBodyValues.Shuffle();

			//Optimisation par changement de Valueset pour chaque value
			while (svInBodyValues.GetSize() != 0 or svOutBodyValues.GetSize() != 0)
			{
				ivTargetFrequencyValuesDiff.Initialize();
				nRandPart = RandomInt(1);
				if (nRandPart == 0 and svInBodyValues.GetSize() != 0)
				// randPart = 0 -> bodypart
				{
					////DDD
					//krocCounters.ResetCounters();
					//InitializeObjectsCounters(database, krrCurrentRule);
					//assert(krocCounters.GetBodyCountAt(kwoInBody) == krrCurrentRule->GetAttributeNumber());
					//ensure(CheckObjectCounters(database, krrCurrentRule));
					////cout << krocCounters << endl;

					//cout << "origin part : body\n";
					// passage d'une valeur de l'interieur vers l'exterieur du corps
					sMovingValue = svInBodyValues.GetAt(svInBodyValues.GetSize() - 1);
					/*cout << sMovingValue << "(" <<
						krpsaAttribute->LookUpPreprocessedValue(sMovingValue)->GetLastIndex() - krpsaAttribute->LookUpPreprocessedValue(sMovingValue)->GetFirstIndex()
						<< ")" << "->\n";*/
					ChangeValuePart(sMovingValue, krrAttribute->GetPartAt(0), krrAttribute->GetPartAt(1));

					// Mise a jour du ivTargetFrequencyValuesDiff
					for (nIndex = krpsaAttribute->LookUpPreprocessedValue(sMovingValue)->GetFirstIndex();
					     nIndex <= krpsaAttribute->LookUpPreprocessedValue(sMovingValue)->GetLastIndex();
					     nIndex++)
					{
						//DDD
						kwoObject = cast(KWObject*, krpsaAttribute->GetObjects()->GetAt(nIndex));
						sValue = kwoObject->GetSymbolValueAt(liAttributeLoadIndex);
						require(sValue == sMovingValue);
						//kwoObject = cast(KWObject*, oaWorkingInstances->GetAt(nIndex));
						if (krocCounters.GetBodyCountAt(kwoObject) == krrCurrentRule->GetAttributeNumber())
						//if ( (krrAttribute->IsInBodySymbol(sValue) and krocCounters.GetBodyCountAt(kwoObject) == krrCurrentRule->GetAttributeNumber())
						//	or ((not krrAttribute->IsInBodySymbol(sValue)) and krocCounters.GetBodyCountAt(kwoObject) == krrCurrentRule->GetAttributeNumber() - 1) )
						{
							if (krrCurrentRule->GetTargetType() == KWType::Symbol)
							{
								sObjectTargetValue = kwoObject->GetSymbolValueAt(liTargetAttributeIndex);
								nObjectTargetValueIndex = krrCurrentRule->ComputeTargetValueIndex(sObjectTargetValue);
								require(nObjectTargetValueIndex >= 0 and nObjectTargetValueIndex < krrCurrentRule->GetTargetValueNumber());
								ivTargetFrequencyValuesDiff.UpgradeAt(nObjectTargetValueIndex, -1);
							}
							if (krrCurrentRule->GetTargetType() == KWType::Continuous)
							{
								cObjectTargetValue = kwoObject->GetContinuousValueAt(liTargetAttributeIndex);
								nObjectTargetValueIndex = krrCurrentRule->ComputeContinuousTargetValuePartIndex(cObjectTargetValue);
								require(nObjectTargetValueIndex >= 0 and nObjectTargetValueIndex < krrCurrentRule->GetTargetAttribute()->GetPartNumber());
								ivTargetFrequencyValuesDiff.UpgradeAt(nObjectTargetValueIndex, -1);
							}
						}
					}
					//krrCurrentRule->Write(cout);
					/*krrAttribute->WriteParts(cout);
					krrCurrentRule->WriteContingencyTable(cout);
					cout << krrCurrentRule->GetLevel() << endl;*/
					UpdateAllStatistics(database, krrCurrentRule);
					//DDD UpdateBodyStatistics(database, krrCurrentRule, &ivTargetFrequencyValuesDiff);
					ivTargetFrequencyValuesDiff.Initialize();
					if (krrCurrentRule->GetLevel() > dMaxRuleLevel)
					{
						dMaxRuleLevel = krrCurrentRule->GetLevel();
						////DDD
						//krocCounters.ResetCounters();
						//InitializeObjectsCounters(database, krrCurrentRule);
						//assert(krocCounters.GetBodyCountAt(kwoInBody) == krrCurrentRule->GetAttributeNumber());
						//ensure(CheckObjectCounters(database, krrCurrentRule));
						////cout << krocCounters << endl;
					}
					else
					{
						////DDD
						//krocCounters.ResetCounters();
						//InitializeObjectsCounters(database, krrCurrentRule);
						//assert(krocCounters.GetBodyCountAt(kwoInBody) == krrCurrentRule->GetAttributeNumber());
						//ensure(CheckObjectCounters(database, krrCurrentRule));
						////cout << krocCounters << endl;

						/*cout << sMovingValue << "(" <<
							krpsaAttribute->LookUpPreprocessedValue(sMovingValue)->GetLastIndex() - krpsaAttribute->LookUpPreprocessedValue(sMovingValue)->GetFirstIndex()
							<< ")" << "<- retour\n";*/
						ChangeValuePart(sMovingValue, krrAttribute->GetPartAt(1), krrAttribute->GetPartAt(0));
						// Mise a jour du ivTargetFrequencyValuesDiff
						for (nIndex = krpsaAttribute->LookUpPreprocessedValue(sMovingValue)->GetFirstIndex();
						     nIndex <= krpsaAttribute->LookUpPreprocessedValue(sMovingValue)->GetLastIndex();
						     nIndex++)
						{
							//DDD
							kwoObject = cast(KWObject*, krpsaAttribute->GetObjects()->GetAt(nIndex));
							sValue = kwoObject->GetSymbolValueAt(liAttributeLoadIndex);
							require(sValue == sMovingValue);
							//kwoObject = cast(KWObject*, oaWorkingInstances->GetAt(nIndex));
							if (krocCounters.GetBodyCountAt(kwoObject) == krrCurrentRule->GetAttributeNumber())
							//if ( (krrAttribute->IsInBodySymbol(sValue) and krocCounters.GetBodyCountAt(kwoObject) == krrCurrentRule->GetAttributeNumber())
							//	or ((not krrAttribute->IsInBodySymbol(sValue)) and krocCounters.GetBodyCountAt(kwoObject) == krrCurrentRule->GetAttributeNumber() - 1) )
							{
								if (krrCurrentRule->GetTargetType() == KWType::Symbol)
								{
									sObjectTargetValue = kwoObject->GetSymbolValueAt(liTargetAttributeIndex);
									nObjectTargetValueIndex = krrCurrentRule->ComputeTargetValueIndex(sObjectTargetValue);
									require(nObjectTargetValueIndex >= 0 and nObjectTargetValueIndex < krrCurrentRule->GetTargetValueNumber());
									ivTargetFrequencyValuesDiff.UpgradeAt(nObjectTargetValueIndex, 1);
								}
								if (krrCurrentRule->GetTargetType() == KWType::Continuous)
								{
									cObjectTargetValue = kwoObject->GetContinuousValueAt(liTargetAttributeIndex);
									nObjectTargetValueIndex = krrCurrentRule->ComputeContinuousTargetValuePartIndex(cObjectTargetValue);
									require(nObjectTargetValueIndex >= 0 and nObjectTargetValueIndex < krrCurrentRule->GetTargetAttribute()->GetPartNumber());
									ivTargetFrequencyValuesDiff.UpgradeAt(nObjectTargetValueIndex, 1);
								}
							}
						}
						/*krrAttribute->WriteParts(cout);
						krrCurrentRule->WriteContingencyTable(cout);
						cout << krrCurrentRule->GetLevel() << endl;*/
						UpdateAllStatistics(database, krrCurrentRule);
						//DDD UpdateBodyStatistics(database, krrCurrentRule, &ivTargetFrequencyValuesDiff);
						ivTargetFrequencyValuesDiff.Initialize();
					}
					svInBodyValues.SetSize(svInBodyValues.GetSize() - 1);
				}

				// transfert de l'exterieur vers l'interieur
				if (nRandPart == 1 and svOutBodyValues.GetSize() != 0)
				{
					////DDD
					//krocCounters.ResetCounters();
					//InitializeObjectsCounters(database, krrCurrentRule);
					//assert(krocCounters.GetBodyCountAt(kwoInBody) == krrCurrentRule->GetAttributeNumber());
					//ensure(CheckObjectCounters(database, krrCurrentRule));
					////cout << krocCounters << endl;

					//cout << "origin part : out-body\n";
					// passage d'une valeur de l'exterieur vers l'intereiur du corps
					sMovingValue = svOutBodyValues.GetAt(svOutBodyValues.GetSize() - 1);
					/*cout << sMovingValue << "(" <<
						krpsaAttribute->LookUpPreprocessedValue(sMovingValue)->GetLastIndex() - krpsaAttribute->LookUpPreprocessedValue(sMovingValue)->GetFirstIndex()
						<< ")" << "<-\n";*/
					ChangeValuePart(sMovingValue, krrAttribute->GetPartAt(1), krrAttribute->GetPartAt(0));
					// Mise a jour du ivTargetFrequencyValuesDiff
					for (nIndex = krpsaAttribute->LookUpPreprocessedValue(sMovingValue)->GetFirstIndex();
					     nIndex <= krpsaAttribute->LookUpPreprocessedValue(sMovingValue)->GetLastIndex();
					     nIndex++)
					{
						//DDD
						kwoObject = cast(KWObject*, krpsaAttribute->GetObjects()->GetAt(nIndex));
						sValue = kwoObject->GetSymbolValueAt(liAttributeLoadIndex);
						require(sValue == sMovingValue);
						//kwoObject = cast(KWObject*, oaWorkingInstances->GetAt(nIndex));
						if (krocCounters.GetBodyCountAt(kwoObject) == krrCurrentRule->GetAttributeNumber() - 1)
						//if ( (krrAttribute->IsInBodySymbol(sValue) and krocCounters.GetBodyCountAt(kwoObject) == krrCurrentRule->GetAttributeNumber())
						//	or ((not krrAttribute->IsInBodySymbol(sValue)) and krocCounters.GetBodyCountAt(kwoObject) == krrCurrentRule->GetAttributeNumber() - 1) )
						{
							if (krrCurrentRule->GetTargetType() == KWType::Symbol)
							{
								sObjectTargetValue = kwoObject->GetSymbolValueAt(liTargetAttributeIndex);
								nObjectTargetValueIndex = krrCurrentRule->ComputeTargetValueIndex(sObjectTargetValue);
								require(nObjectTargetValueIndex >= 0 and nObjectTargetValueIndex < krrCurrentRule->GetTargetValueNumber());
								ivTargetFrequencyValuesDiff.UpgradeAt(nObjectTargetValueIndex, 1);
							}
							if (krrCurrentRule->GetTargetType() == KWType::Continuous)
							{
								cObjectTargetValue = kwoObject->GetContinuousValueAt(liTargetAttributeIndex);
								nObjectTargetValueIndex = krrCurrentRule->ComputeContinuousTargetValuePartIndex(cObjectTargetValue);
								require(nObjectTargetValueIndex >= 0 and nObjectTargetValueIndex < krrCurrentRule->GetTargetAttribute()->GetPartNumber());
								ivTargetFrequencyValuesDiff.UpgradeAt(nObjectTargetValueIndex, 1);
							}
						}
					}
					UpdateAllStatistics(database, krrCurrentRule);
					/*krrAttribute->WriteParts(cout);
					krrCurrentRule->WriteContingencyTable(cout);
					cout << krrCurrentRule->GetLevel() << endl;*/
					//DDD UpdateBodyStatistics(database, krrCurrentRule, &ivTargetFrequencyValuesDiff);
					ivTargetFrequencyValuesDiff.Initialize();
					if (krrCurrentRule->GetLevel() > dMaxRuleLevel)
					{
						dMaxRuleLevel = krrCurrentRule->GetLevel();
						////DDD
						//krocCounters.ResetCounters();
						//InitializeObjectsCounters(database, krrCurrentRule);
						//assert(krocCounters.GetBodyCountAt(kwoInBody) == krrCurrentRule->GetAttributeNumber());
						//ensure(CheckObjectCounters(database, krrCurrentRule));
						////cout << krocCounters << endl;
					}
					else
					{
						////DDD
						//krocCounters.ResetCounters();
						//InitializeObjectsCounters(database, krrCurrentRule);
						//assert(krocCounters.GetBodyCountAt(kwoInBody) == krrCurrentRule->GetAttributeNumber());
						//ensure(CheckObjectCounters(database, krrCurrentRule));
						////cout << krocCounters << endl;

						ChangeValuePart(sMovingValue, krrAttribute->GetPartAt(0), krrAttribute->GetPartAt(1));
						/*cout << sMovingValue << "(" <<
							krpsaAttribute->LookUpPreprocessedValue(sMovingValue)->GetLastIndex() - krpsaAttribute->LookUpPreprocessedValue(sMovingValue)->GetFirstIndex()
							<< ")" << "-> retour\n";*/
						for (nIndex = krpsaAttribute->LookUpPreprocessedValue(sMovingValue)->GetFirstIndex();
						     nIndex <= krpsaAttribute->LookUpPreprocessedValue(sMovingValue)->GetLastIndex();
						     nIndex++)
						{
							//DDD
							kwoObject = cast(KWObject*, krpsaAttribute->GetObjects()->GetAt(nIndex));
							sValue = kwoObject->GetSymbolValueAt(liAttributeLoadIndex);
							require(sValue == sMovingValue);
							//kwoObject = cast(KWObject*, oaWorkingInstances->GetAt(nIndex));
							if (krocCounters.GetBodyCountAt(kwoObject) == krrCurrentRule->GetAttributeNumber() - 1)
							//if ( (krrAttribute->IsInBodySymbol(sValue) and krocCounters.GetBodyCountAt(kwoObject) == krrCurrentRule->GetAttributeNumber())
							//	or ((not krrAttribute->IsInBodySymbol(sValue)) and krocCounters.GetBodyCountAt(kwoObject) == krrCurrentRule->GetAttributeNumber() - 1) )
							{
								if (krrCurrentRule->GetTargetType() == KWType::Symbol)
								{
									sObjectTargetValue = kwoObject->GetSymbolValueAt(liTargetAttributeIndex);
									nObjectTargetValueIndex = krrCurrentRule->ComputeTargetValueIndex(sObjectTargetValue);
									require(nObjectTargetValueIndex >= 0 and nObjectTargetValueIndex < krrCurrentRule->GetTargetValueNumber());
									ivTargetFrequencyValuesDiff.UpgradeAt(nObjectTargetValueIndex, -1);
								}
								if (krrCurrentRule->GetTargetType() == KWType::Continuous)
								{
									cObjectTargetValue = kwoObject->GetContinuousValueAt(liTargetAttributeIndex);
									nObjectTargetValueIndex = krrCurrentRule->ComputeContinuousTargetValuePartIndex(cObjectTargetValue);
									require(nObjectTargetValueIndex >= 0 and nObjectTargetValueIndex < krrCurrentRule->GetTargetAttribute()->GetPartNumber());
									ivTargetFrequencyValuesDiff.UpgradeAt(nObjectTargetValueIndex, -1);
								}
							}
						}
						UpdateAllStatistics(database, krrCurrentRule);
						/*krrAttribute->WriteParts(cout);
						krrCurrentRule->WriteContingencyTable(cout);
						cout << krrCurrentRule->GetLevel() << endl;*/
						//DDD UpdateBodyStatistics(database, krrCurrentRule, &ivTargetFrequencyValuesDiff);
						ivTargetFrequencyValuesDiff.Initialize();
					}
					svOutBodyValues.SetSize(svOutBodyValues.GetSize() - 1);
				}
				/*cout << "InBodyValuesSize : " << svInBodyValues.GetSize() << endl;
				cout << "OutBodyValuesSize : " << svOutBodyValues.GetSize() << endl;
				cout << "Rule level : " << krrCurrentRule->GetLevel() << "\n";*/
			}
			UpdateAllStatistics(database, krrCurrentRule);
		}
		UpdateAllStatistics(database, krrCurrentRule);
	} // fin optim

	//Regularisation si partie vide
	// la partie vide ne peut etre que celle hors du corps (1)
	if (krrAttribute->GetPartAt(1)->GetValueSet()->GetValueNumber() == 0)
		krrAttribute->DeletePart(krrAttribute->GetPartAt(1));

	//UpdateBodyStatistics(database, krrCurrentRule, &ivTargetFrequencyValuesDiff);
	UpdateAllStatistics(database, krrCurrentRule);

	require(IsInBody(kwoInBody, krrCurrentRule));

	return krrCurrentRule->GetLevel();
}

void KRRuleOptimizer::ChangeValuePart(Symbol sValue, KRRPart* krrpOrigin, KRRPart* krrpDestination) const
{
	KRRValue* value;

	value = krrpOrigin->GetValueSet()->GetHeadValue();
	while (value != NULL)
	{
		if (value->GetValue() == sValue)
		{
			krrpDestination->GetValueSet()->AddValue(sValue, value->GetValueFrequency());
			krrpOrigin->GetValueSet()->DeleteValue(value);
			break;
		}
		krrpOrigin->GetValueSet()->GetNextValue(value);
	}
}

double KRRuleOptimizer::OptimizeRule(KWDatabase* kwdb, KWObject* kwoInBody, KRRule* krrCurrentRule)
{
	int nIndex;
	int nOptimizationDepth;
	double dMinLevel;
	//double dCurrentLevel;
	int nbAttributes;

	dMinLevel = krrCurrentRule->GetLevel();
	//dCurrentLevel = dMinLevel;
	//cout << "*****************\n";
	//cout << "Nombre d'attributs : " << krrCurrentRule->GetAttributeNumber() << endl;

	// nombre d'optimisations controles
	for (nOptimizationDepth = 1; nOptimizationDepth <= 10; nOptimizationDepth++)
	{
		// DDD nbAttributes = krrCurrentRule->GetInformativeAttributeNumber();
		nbAttributes = krrCurrentRule->GetAttributeNumber();
		for (nIndex = 0; nIndex < nbAttributes; nIndex++)
		{
			/*cout << "************************\n";
			cout << "Optimize Attribute : " << krrCurrentRule->GetAttributeAt(nIndex)->GetAttributeName() << endl;
			cout << "nb inf att : " << krrCurrentRule->GetInformativeAttributeNumber() << endl;
			cout << "nb att : " << krrCurrentRule->GetAttributeNumber() << endl;*/
			OptimizeRuleAttribute(kwdb, krrCurrentRule->GetAttributeAt(nIndex), kwoInBody, krrCurrentRule);
			// DDD Attention a ne pas changer le nb d'attributs informatifs lors de l'optimisation
			//krrCurrentRule->Write(cout);
		}
		krrCurrentRule->DeleteNonInformativeAttributes();
		UpdateAllStatistics(kwdb, krrCurrentRule);

		if (krrCurrentRule->GetLevel() <= dMinLevel or krrCurrentRule->GetInformativeAttributeNumber() == 0)
		{
			/*cout << "Convergence : no more level improvement\n";
			cout << "Nombre d'attributs : " << krrCurrentRule->GetAttributeNumber() << endl;*/
			break;
		}
		else
		{
			/*cout << "Level improvement : " << krrCurrentRule->GetLevel() - dMinLevel << endl;
			cout << "Nombre d'attributs : " << krrCurrentRule->GetAttributeNumber() << endl;*/
			dMinLevel = krrCurrentRule->GetLevel();
		}
	}

	//Debug
	/*for (nIndex = 0; nIndex < krrCurrentRule->GetAttributeNumber(); nIndex++)
	{
		if (krrCurrentRule->GetAttributeAt(nIndex)->GetAttributeType() == KWType::Symbol)
		{
			if (krrCurrentRule->GetAttributeAt(nIndex)->GetPartAt(0)->GetValueSet()->GetValueNumber() == 0)
			{
				cout << "body vide\n";
				cout << *krrCurrentRule << endl;
			}
			if (krrCurrentRule->GetAttributeAt(nIndex)->GetPartAt(1)->GetValueSet()->GetValueNumber() == 0)
			{
				cout << "out-body vide\n";
				krrCurrentRule->GetAttributeAt(nIndex)->DeletePart(krrCurrentRule->GetAttributeAt(nIndex)->GetPartAt(1));
				cout << *krrCurrentRule << endl;
			}
		}

	}
	krrCurrentRule->DeleteNonInformativeAttributes();*/

	// Remise a zero des compteurs d'objets apres l'optimisation de la regle pour la nouvelle regle
	krocCounters.ResetCounters();
	//cout << krocCounters << endl;

	return krrCurrentRule->GetLevel();
}
