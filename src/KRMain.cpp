// Copyright (c) 2023-2025 Orange. All rights reserved.
// This software is distributed under the BSD 3-Clause-clear License, the text of which is available
// at https://spdx.org/licenses/BSD-3-Clause-Clear.html or see the "LICENSE" file for more details.

#include "KRMain.h"

void StartKhioRules(int argv, char **argc) {
  KRLearningProject learningProject;

  // Lancement du projet
  learningProject.Start(argv, argc);
}

int main(int argv, char **argc) {
  // Parametrage de l'arret de l'allocateur
  // MemSetAllocIndexExit(11042);
  // MemSetAllocSizeExit(73724);
  // MemSetAllocBlockExit((void*)0x00F42044);

#ifdef KWLearningBatchMode
  // Mode textuel, sans JNI, pour l'integration a Clementine
  UIObject::SetUIMode(UIObject::Textual);
#endif // KWLearningBatchMode

  // Lancement de l'outil
  StartKhioRules(argv, argc);
  return 0;
}

#ifdef KWLearningBatchMode
/********************************************************************
 * Le source suivant permet de compiler des sources developpes avec *
 * l'environnement Norm, d'utiliser le mode UIObject::Textual et    *
 * de ne pas linker avec jvm.lib (a eviter absoluement).            *
 * Moyennant ces conditions, on peut livrer un executable en mode   *
 * textuel ne necessitant pas l'intallation prealable du JRE Java   *
 ********************************************************************/

extern "C" {

#ifdef _MSC_VER
int __stdcall _imp__JNI_CreateJavaVM(void **pvm, void **penv, void *args) {
  exit(0);
}
#endif // _MSC_VER

#ifdef __UNIX__
int JNICALL JNI_CreateJavaVM(void **pvm, void **penv, void *args) { exit(0); }
#endif // __UNIX__
}
#endif // KWLearningBatchMode
