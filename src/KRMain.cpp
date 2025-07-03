// Copyright (c) 2023-2025 Orange. All rights reserved.
// This software is distributed under the BSD 3-Clause-clear License, the text of which is available
// at https://spdx.org/licenses/BSD-3-Clause-Clear.html or see the "LICENSE" file for more details.

#include "KRMain.h"

void StartKhioRules(int argv, char** argc) {
	KRLearningProject learningProject;

	// Lancement du projet
	learningProject.Start(argv, argc);
}

int main(int argv, char** argc)
{
	// Parametrage de l'arret de l'allocateur
	// MemSetAllocIndexExit(11042);
	// MemSetAllocSizeExit(73724);
	// MemSetAllocBlockExit((void*)0x00F42044);

	// Lancement de l'outil
	StartKhioRules(argv, argc);

	// On renvoie 0 si tout s'est bien passe, 1 en cas de FatalError (dans Standard.cpp)
	return EXIT_SUCCESS;
}
