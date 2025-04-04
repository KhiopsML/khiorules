// Copyright (c) 2023-2025 Orange. All rights reserved.
// This software is distributed under the BSD 3-Clause-clear License, the text of which is available
// at https://spdx.org/licenses/BSD-3-Clause-Clear.html or see the "LICENSE" file for more details.

#include "KRPredictorRule.h"

/////////////////////////////////////
// Classe KRPredictorRule

KRPredictorRule::KRPredictorRule()
{
	//	oaRuleSet = NULL;
}

KRPredictorRule::~KRPredictorRule()
{
}

boolean KRPredictorRule::IsTargetTypeManaged(int nType) const
{
	// return nType == KWType::Symbol;
	return nType == KWType::Symbol or nType == KWType::Continuous or nType == KWType::None;
}

KWPredictor* KRPredictorRule::Create() const
{
	return new KRPredictorRule;
}

const ALString KRPredictorRule::GetName() const
{
	return "Rule-Based";
}

const ALString KRPredictorRule::GetPrefix() const
{
	return "KR";
}

KRRuleParameters* KRPredictorRule::GetRuleParameters()
{
	return &ruleParameters;
}

KRPredictorRuleReport* KRPredictorRule::GetPredictorRuleReport()
{
	return cast(KRPredictorRuleReport*, GetPredictorReport());
}

void KRPredictorRule::CopyFrom(const KWPredictor* kwpSource)
{
	KRPredictorRule* krprSource;

	require(kwpSource != NULL);

	// Appel de la methode ancetre
	KWPredictor::CopyFrom(kwpSource);

	// Recopie des parametres de selection de variable
	krprSource = cast(KRPredictorRule*, kwpSource);

	ruleParameters.CopyFrom(&krprSource->ruleParameters);
}

void KRPredictorRule::CreatePredictorReport()
{
	require(bIsTraining);
	require(predictorReport == NULL);

	predictorReport = new KRPredictorRuleReport;
	cast(KRPredictorRuleReport*, predictorReport);
	predictorReport->SetLearningSpec(GetLearningSpec());
	predictorReport->SetPredictorName(GetName());
	/*cast(KRPredictorRuleReport*, predictorReport)->SetRuleSet(oaRuleSet);
	cout << "MOUT\n";*/
}

void KRPredictorRule::FillPredictorRuleReport(ObjectArray* oaRuleSet)
{
	KRPredictorRuleReport* ruleReport;
	int nIndex;
	KRSelectedRuleReport* selectedRuleReport;
	KRRule* rule;

	//Initialisation du rapport
	ruleReport = GetPredictorRuleReport();
	ruleReport->GetSelectedRuleReports()->DeleteAll();

	// Transfert des regles vers le rapport
	for (nIndex = 0; nIndex < oaRuleSet->GetSize(); nIndex++)
	{
		rule = cast(KRRule*, oaRuleSet->GetAt(nIndex));

		// creation d'un rapport de regle
		selectedRuleReport = new KRSelectedRuleReport;
		ruleReport->GetSelectedRuleReports()->Add(selectedRuleReport);
		selectedRuleReport->SetPreparedRule(rule);
		//DDD selectedRuleReport->SetPreparedRule(rule->Clone());
		//DDD selectedRuleReport->SetPreparedRule(new KRRule);
	}

	// Vidage du tableau de regles
	oaRuleSet->RemoveAll();
}

KWClass* KRPredictorRule::CreateRuleSetClass(ObjectArray* oaRuleSet)
{
	//const ALString sRuleBaseName = "Rule";
	KWClassDomain* ruleClassDomain;
	KWClass* ruleClass;
	int nIndex;
	KRRule* rule;

	require(oaRuleSet != NULL);

	// Creation de la classe de regles par duplication de la classe d'apprentissage
	ruleClassDomain = KWClassDomain::GetCurrentDomain()->CloneFromClass(GetClass());
	ruleClass = ruleClassDomain->LookupClass(GetClass()->GetName());

	// Passage de tous les attributs en Unused, sauf la classe
	if (not GetRuleParameters()->GetAddingFeatures())
		ruleClass->SetAllAttributesUsed(false);
	ruleClass->LookupAttribute(GetTargetAttributeName())->SetUsed(true);

	for (nIndex = 0; nIndex < oaRuleSet->GetSize(); nIndex++)
	{
		rule = cast(KRRule*, oaRuleSet->GetAt(nIndex));
		UpdateRuleClass(ruleClass, rule, nIndex);
	}

	// Completion des informations de type
	ruleClass->CompleteTypeInfo();

	// Compilation de la classe
	ruleClass->Compile();
	return ruleClass;
}

void KRPredictorRule::UpdateRuleClass(KWClass* ruleClass, KRRule* rule, int ruleIndex)
{
	const ALString sRuleBaseName = "Rule";
	KWAttribute* ruleAttribute;
	KWDRAnd* andRule;
	KWDerivationRuleOperand* andOperandTemplate;
	KWDRG* gRule;
	KWDRLE* leRule;
	KWDRAsSymbol* asSymbolRule;
	int nIndex;
	KRRAttribute* krrAttribute;
	KWDRGroupIndex* giRule;
	KWDRValueGroups* vgsRule;
	KWDRValueGroup* vgBodyRule;
	KWDRValueGroup* vgOutBodyRule;
	KRRValueSet* krrValueSet;
	KRRValue* currentValue;
	int nValueIndex;
	int nOutBodyPartIndex;
	KWDREQ* eqRule;

	andRule = new KWDRAnd;
	andOperandTemplate = andRule->GetFirstOperand()->Clone();
	andOperandTemplate->SetOrigin(KWDerivationRuleOperand::OriginRule);
	andRule->DeleteAllOperands();
	if (rule->GetAttributeNumber() != rule->GetInformativeAttributeNumber())
		if (rule->GetTargetType() != KWType::Continuous)
			cout << "PB nb attributes\n";
	for (nIndex = 0; nIndex < rule->GetAttributeNumber(); nIndex++)
	{
		krrAttribute = cast(KRRAttribute*, rule->GetAttributeAt(nIndex));

		if (krrAttribute->GetAttributeTargetFunction())
			continue;

		// Attribut numerique
		if (krrAttribute->GetAttributeType() == KWType::Continuous)
		{
			if (krrAttribute->GetBodyPartIndex() == 0)
			{
				leRule = new KWDRLE;
				leRule->GetFirstOperand()->SetOrigin(KWDerivationRuleOperand::OriginAttribute);
				leRule->GetFirstOperand()->SetAttributeName(krrAttribute->GetAttributeName());
				leRule->GetSecondOperand()->SetOrigin(KWDerivationRuleOperand::OriginConstant);
				leRule->GetSecondOperand()->SetContinuousConstant(krrAttribute->GetPartAt(0)->GetInterval()->GetUpperBound());
				andRule->AddOperand(andOperandTemplate->Clone());
				andRule->GetOperandAt(andRule->GetOperandNumber() - 1)->SetDerivationRule(leRule);
			}
			else //krrAttribute->GetBodyPartIndex() == 1
			    if (krrAttribute->GetPartNumber() == 2)
			{
				gRule = new KWDRG;
				gRule->GetFirstOperand()->SetOrigin(KWDerivationRuleOperand::OriginAttribute);
				gRule->GetFirstOperand()->SetAttributeName(krrAttribute->GetAttributeName());
				gRule->GetSecondOperand()->SetOrigin(KWDerivationRuleOperand::OriginConstant);
				gRule->GetSecondOperand()->SetContinuousConstant(krrAttribute->GetPartAt(1)->GetInterval()->GetLowerBound());
				andRule->AddOperand(andOperandTemplate->Clone());
				andRule->GetOperandAt(andRule->GetOperandNumber() - 1)->SetDerivationRule(gRule);
			}
			else //krrAttribute->GetPartNumber() == 3
			{
				leRule = new KWDRLE;
				leRule->GetFirstOperand()->SetOrigin(KWDerivationRuleOperand::OriginAttribute);
				leRule->GetFirstOperand()->SetAttributeName(krrAttribute->GetAttributeName());
				leRule->GetSecondOperand()->SetOrigin(KWDerivationRuleOperand::OriginConstant);
				leRule->GetSecondOperand()->SetContinuousConstant(krrAttribute->GetPartAt(1)->GetInterval()->GetUpperBound());
				gRule = new KWDRG;
				gRule->GetFirstOperand()->SetOrigin(KWDerivationRuleOperand::OriginAttribute);
				gRule->GetFirstOperand()->SetAttributeName(krrAttribute->GetAttributeName());
				gRule->GetSecondOperand()->SetOrigin(KWDerivationRuleOperand::OriginConstant);
				gRule->GetSecondOperand()->SetContinuousConstant(krrAttribute->GetPartAt(1)->GetInterval()->GetLowerBound());
				andRule->AddOperand(andOperandTemplate->Clone());
				andRule->GetOperandAt(andRule->GetOperandNumber() - 1)->SetDerivationRule(gRule);
				andRule->AddOperand(andOperandTemplate->Clone());
				andRule->GetOperandAt(andRule->GetOperandNumber() - 1)->SetDerivationRule(leRule);
			}
		} // Fin attribut numerique

		// Attribut symbolique
		if (rule->GetAttributeAt(nIndex)->GetAttributeType() == KWType::Symbol)
		{
			// Construction du ValueGroups fait de ValueGroup corps et hors-corps
			vgsRule = new KWDRValueGroups;
			vgsRule->SetValueGroupNumber(2);

			//DDD attention cas des star value a regarder
			vgBodyRule = cast(KWDRValueGroup*, vgsRule->GetFirstOperand()->GetDerivationRule());
			vgOutBodyRule = cast(KWDRValueGroup*, vgsRule->GetSecondOperand()->GetDerivationRule());

			// Construction du ValueGroup du corps
			krrValueSet = krrAttribute->GetPartAt(krrAttribute->GetBodyPartIndex())->GetValueSet();
			assert(krrValueSet->GetValueNumber() > 0);
			vgBodyRule->SetValueNumber(krrValueSet->GetValueNumber());
			currentValue = krrValueSet->GetHeadValue();
			nValueIndex = 0;
			while (currentValue != NULL)
			{
				vgBodyRule->SetValueAt(nValueIndex, currentValue->GetValue());
				krrValueSet->GetNextValue(currentValue);
				nValueIndex++;
			}

			// Construction du ValueGroup du hors-corps
			nOutBodyPartIndex = 0;
			if (krrAttribute->GetBodyPartIndex() == 0)
				nOutBodyPartIndex = 1;
			krrValueSet = krrAttribute->GetPartAt(nOutBodyPartIndex)->GetValueSet();
			assert(krrValueSet->GetValueNumber() > 0);
			vgOutBodyRule->SetValueNumber(krrValueSet->GetValueNumber());
			currentValue = krrValueSet->GetHeadValue();
			nValueIndex = 0;
			while (currentValue != NULL)
			{
				vgOutBodyRule->SetValueAt(nValueIndex, currentValue->GetValue());
				krrValueSet->GetNextValue(currentValue);
				nValueIndex++;
			}

			// Construction du GroupId a partir du valueGroups
			giRule = new KWDRGroupIndex;
			giRule->GetFirstOperand()->SetOrigin(KWDerivationRuleOperand::OriginRule);
			giRule->GetFirstOperand()->SetDerivationRule(vgsRule);
			giRule->GetSecondOperand()->SetOrigin(KWDerivationRuleOperand::OriginAttribute);
			giRule->GetSecondOperand()->SetAttributeName(krrAttribute->GetAttributeName());

			// Regle de derivation finale pour les attributs categoriels EQ(GroupId( ValueGroups(VG1,VG2)) , Att ) , 1)
			eqRule = new KWDREQ;
			eqRule->GetFirstOperand()->SetOrigin(KWDerivationRuleOperand::OriginRule);
			eqRule->GetFirstOperand()->SetDerivationRule(giRule);
			eqRule->GetSecondOperand()->SetOrigin(KWDerivationRuleOperand::OriginConstant);
			eqRule->GetSecondOperand()->SetContinuousConstant(1);

			//cout << *eqRule << endl;
			// Ajout au ANDRULE
			andRule->AddOperand(andOperandTemplate->Clone());
			andRule->GetOperandAt(andRule->GetOperandNumber() - 1)->SetDerivationRule(eqRule);

		} // Fin attribut symbolique
		  //cout << *andRule << endl;
	}

	// Ajout d'un attribut de regle dans la classe
	ruleAttribute = new KWAttribute;
	ruleAttribute->SetName(ruleClass->BuildAttributeName(sRuleBaseName + IntToString(ruleIndex + 1)));
	asSymbolRule = new KWDRAsSymbol;
	asSymbolRule->GetFirstOperand()->SetOrigin(KWDerivationRuleOperand::OriginRule);
	asSymbolRule->GetFirstOperand()->SetDerivationRule(andRule);
	ruleAttribute->SetDerivationRule(asSymbolRule);
	ruleClass->InsertAttribute(ruleAttribute);

	// Nettoyage
	delete andOperandTemplate;
}

boolean KRPredictorRule::InternalTrain()
{
	KWClass* ruleClass;
	KWDataPreparationClass dataPreparationClass;
	KWDatabase* kwdb;
	KRRuleOptimizer ruleOptimizer;
	ObjectArray oaRuleSet;
	KRRuleCosts* costs;

	require(Check());
	require(GetClassStats() != NULL);
	require(GetClassStats()->IsStatsComputed());
	require(GetTargetAttributeType() == KWType::Symbol or GetTargetAttributeType() == KWType::Continuous or GetTargetAttributeType() == KWType::None);
	require(IsTraining());

	//kwdb = new KWDatabase;

	// Apprentissage si au moins une valeur cible
	if (GetTargetDescriptiveStats()->GetValueNumber() > 0)
	{
		//lecture de la base
		kwdb = GetDatabase();
		kwdb->ReadAll();
		ensure(kwdb->ReadAll());

		//GetRuleParameters()->Write(cout);
		// Parametrage du RuleOptimizer
		ruleOptimizer.SetMaxRuleNumber(GetRuleParameters()->GetMaxRuleNumber());
		ruleOptimizer.SetTimeAllowed(GetRuleParameters()->GetTimeAllowed());
		//ruleOptimizer.SetMaxRuleNumber(10);
		ruleOptimizer.SetClassStats(GetClassStats());
		costs = NULL;
		if (GetTargetAttributeType() == KWType::Symbol)
			costs = new KRRuleClassificationCosts;
		if (GetTargetAttributeType() == KWType::Continuous)
			costs = new KRRuleRegressionCosts;
		assert(costs != NULL);
		costs->SetClassStats(GetClassStats());
		ruleOptimizer.SetRuleCosts(costs);

		// Pretraitement necessaire a l'optimisation
		ruleOptimizer.Preprocess(kwdb);
		//ruleOptimizer.WritePreprocessing(cout);

		// Optimisation
		oaRuleSet.SetSize(ruleOptimizer.GetMaxRuleNumber());
		ruleOptimizer.OptimizeRuleSet(kwdb, &oaRuleSet);

		// Creation d'une classe pour l'ensemble de regles
		ruleClass = CreateRuleSetClass(&oaRuleSet);
		// cout << *ruleClass << endl;

		// Apprentissage a partir de la base de regles
		InternalTrainPredictorFromRuleClass(ruleClass);

		// Destrction de la classe de regles et de son domaine
		delete ruleClass->GetDomain();

		// Attention, les regles sont transferees au report
		// et le oaRuleSet est vide a l'issue de cette methode
		FillPredictorRuleReport(&oaRuleSet);

		kwdb->DeleteAll();
		delete costs;

		// debug
		if (not IsTrained())
			cout << "Not IsTrained\n";
		if (not((predictorReport == NULL and trainedPredictor == NULL) or (predictorReport != NULL and trainedPredictor != NULL)))
			cout << "bug1\n"
			     << "predictor report : \n"
			     << predictorReport << "\ntrained predictor : \n"
			     << trainedPredictor << endl;
		if (not((predictorReport == NULL and trainedPredictor == NULL) or trainedPredictor->GetTargetType() == GetTargetAttributeType()))
			cout << "bug2\n";
		if (not((predictorReport == NULL and trainedPredictor == NULL) or trainedPredictor->Check()))
			cout << "bug3\n";

		return true;
	}

	return false;
}

void KRPredictorRule::InternalTrainPredictorFromRuleClass(KWClass* ruleClass)
{
	KWClassDomain* currentDomain;
	KWPredictor* subPredictor;
	KWPredictorReport* subPredictorReport;
	KWTrainedPredictor* subTrainedPredictor;
	KWLearningSpec subLearningSpec;
	KWClassStats subClassStats;

	require(ruleClass != NULL);
	require(ruleClass->IsCompiled());
	require(Check());
	require(GetClassStats() != NULL);
	require(GetClassStats()->IsStatsComputed());
	require(GetTargetAttributeType() == KWType::Symbol or GetTargetAttributeType() == KWType::Continuous);
	require(IsTraining());

	// Changement de domaine courant pour prendre en compte la classe de regles
	currentDomain = KWClassDomain::GetCurrentDomain();
	KWClassDomain::SetCurrentDomain(ruleClass->GetDomain());

	// Creation d'un predicteur "sous-traitant"
	subPredictor = NULL;
	assert(GetRuleParameters()->GetSubPredictor() == "SNB" or
	       GetRuleParameters()->GetSubPredictor() == "NB" or
	       GetRuleParameters()->GetSubPredictor() == "BU");
	if (GetRuleParameters()->GetSubPredictor() == "SNB")
		subPredictor = new SNBPredictorSelectiveNaiveBayes; //DDD MB
	else if (GetRuleParameters()->GetSubPredictor() == "NB")
		subPredictor = new KWPredictorNaiveBayes;
	else if (GetRuleParameters()->GetSubPredictor() == "BU")
		subPredictor = new KWPredictorUnivariate;
	else
		cout << "what is subpredictor ?\n";
	assert(subPredictor != NULL);

	// Nettoyage du sliceset des classStats en cours
	GetClassStats()->DeleteDataTableSliceSet();

	// Creation d'un learningSpec pour la classe basee sur les regles
	subLearningSpec.CopyFrom(GetLearningSpec());
	subLearningSpec.SetClass(ruleClass);

	// Apprentissage des stats pour classe de regles
	subClassStats.SetLearningSpec(&subLearningSpec);
	subClassStats.ComputeStats();

	//DEBUG
	//DDD MB subClassStats.WriteReportFile("RuleClassStats.xls");

	// Parametrage du predicteur
	subPredictor->SetLearningSpec(&subLearningSpec);
	subPredictor->SetClassStats(&subClassStats);

	// Apprentissage
	subPredictor->Train();

	// On recupere les resultats d'apprentissage
	subPredictorReport = subPredictor->GetPredictorReport();
	subTrainedPredictor = subPredictor->GetTrainedPredictor();
	subPredictor->RemoveTrainedResults();

	// On a pas besoin du rapport du sous-predicteur
	// A voir...
	delete subPredictorReport;

	// On transfert le predicteur appris vers le predicteur courant
	check(trainedPredictor);
	delete trainedPredictor;
	check(subTrainedPredictor);
	trainedPredictor = subTrainedPredictor;

	// Nettoyage
	delete subPredictor;
	KWClassDomain::SetCurrentDomain(currentDomain);
}

//////////////////////////////////
// Classe KRPredictorRuleReport

KRPredictorRuleReport::KRPredictorRuleReport()
{
}

KRPredictorRuleReport::~KRPredictorRuleReport()
{
	oaSelectedRuleReports.DeleteAll();
}

ObjectArray* KRPredictorRuleReport::GetSelectedRuleReports()
{
	return &oaSelectedRuleReports;
}

void KRPredictorRuleReport::WriteReport(ostream& ost)
{
	//int nIndex;
	//require(oaSelectedRuleReports != NULL);
	//KRSelectedRuleReport* report;

	// Appel de la methode ancetre
	KWPredictorReport::WriteReport(ost);

	// Calcul des identifiants de grilles
	ComputeRankIdentifiers(&oaSelectedRuleReports);

	// Ecriture des rapport synthetiques et detailles
	WriteArrayLineReport(ost, "Rules summary", &oaSelectedRuleReports);
	WriteArrayReport(ost, "Rules details", &oaSelectedRuleReports);
	/*DDD
	ost << "coin!\n";

	for (nIndex = 0; nIndex < oaSelectedRuleReports.GetSize(); nIndex++)
	{
		report = cast(KRSelectedRuleReport*, oaSelectedRuleReports.GetAt(nIndex));
		report->WriteReport(ost);
		ost << "\n----------------------------------\n";
	}
	*/
}

//////////////////////////////////////////////////////////////////////////////
// Classe KRSelectedRuleReport

KRSelectedRuleReport::KRSelectedRuleReport()
{
	preparedRule = NULL;
}

KRSelectedRuleReport::~KRSelectedRuleReport()
{
	if (preparedRule != NULL)
		delete preparedRule;
}

void KRSelectedRuleReport::SetPreparedRule(KRRule* rule)
{
	if (preparedRule != NULL)
		delete preparedRule;
	preparedRule = rule;
}

const KRRule* KRSelectedRuleReport::GetPreparedRule() const
{
	return preparedRule;
}

void KRSelectedRuleReport::WriteHeaderLineReport(ostream& ost)
{
	require(preparedRule != NULL);

	// rang
	ost << "Rank\t";

	// Identification des attributs
	ost << "Names\t";

	// Nombre d'attributs
	ost << "Nb Attributs\t";

	// Evaluation univariee
	ost << "Level\n";
}

void KRSelectedRuleReport::WriteLineReport(ostream& ost)
{
	require(preparedRule != NULL);

	// Rang
	ost << GetIdentifier() << "\t";

	// Nom de la regle
	ost << GetSortName();
	ost << "\t";

	// Nb attributs
	ost << preparedRule->GetInformativeAttributeNumber() << "\t";

	// Level
	ost << preparedRule->GetLevel() << "\n";
}

void KRSelectedRuleReport::WriteReport(ostream& ost)
{
	require(preparedRule != NULL);

	// rang
	ost << GetIdentifier() << "\n";
	// regle detaillee
	preparedRule->Write(ost);
}

const ALString KRSelectedRuleReport::GetSortName() const
{
	ALString sSortName;
	int i;

	if (preparedRule->GetInformativeAttributeNumber() == 0)
		sSortName = "Void";

	for (i = 0; i < preparedRule->GetInformativeAttributeNumber(); i++)
	{
		if (i > 0)
			sSortName += "&";
		sSortName += preparedRule->GetAttributeAt(i)->GetAttributeName();
	}
	return sSortName;
}

double KRSelectedRuleReport::GetSortValue() const
{
	return preparedRule->GetLevel();
}
