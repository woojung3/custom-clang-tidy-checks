//===-- AutocryptTidyModule.cpp - clang-tidy ----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "../ClangTidy.h"
#include "../ClangTidyModule.h"
#include "../ClangTidyModuleRegistry.h"
#include "../bugprone/ExceptionEscapeCheck.h"
#include "../bugprone/UseAfterMoveCheck.h"
#include "../bugprone/ReservedIdentifierCheck.h"
#include "../bugprone/SpuriouslyWakeUpFunctionsCheck.h"
#include "../bugprone/DanglingHandleCheck.h"
#include "../bugprone/UnhandledSelfAssignmentCheck.h"
#include "../cppcoreguidelines/ProTypeConstCastCheck.h"
#include "../cppcoreguidelines/ProBoundsPointerArithmeticCheck.h"
#include "../cert/StrToNumCheck.h"
#include "../cert/VariadicFunctionDefCheck.h"
#include "../cert/LimitedRandomnessCheck.h"
#include "../cert/ProperlySeededRandomGeneratorCheck.h"
#include "../cert/DontModifyStdNamespaceCheck.h"
#include "../cert/SetLongJmpCheck.h"
#include "../cert/StaticObjectExceptionCheck.h"
#include "../cert/NonTrivialTypesLibcMemoryCallsCheck.h"
#include "../cert/MutatingCopyCheck.h"
#include "../misc/ThrowByValueCatchByReferenceCheck.h"
#include "../misc/NewDeleteOverloadsCheck.h"
#include "../google/UnnamedNamespaceInHeaderCheck.h"
#include "EmptyCheck.h"

namespace clang::tidy {
namespace autocrypt {

class AutocryptModule : public ClangTidyModule {
public:
  void addCheckFactories(ClangTidyCheckFactories &CheckFactories) override {
    // Cert - C++ checkers
    // DCL
    CheckFactories.registerCheck<cert::VariadicFunctionDefCheck>("autocrypt-mobis-cert-cpp-dcl50");
    CheckFactories.registerCheck<bugprone::ReservedIdentifierCheck>("autocrypt-mobis-cert-cpp-dcl51");
    CheckFactories.registerCheck<misc::NewDeleteOverloadsCheck>("autocrypt-mobis-cert-cpp-dcl54");
    CheckFactories.registerCheck<bugprone::ExceptionEscapeCheck>("autocrypt-mobis-cert-cpp-dcl57");
    CheckFactories.registerCheck<cert::DontModifyStdNamespaceCheck>("autocrypt-mobis-cert-cpp-dcl58");
    CheckFactories.registerCheck<google::build::UnnamedNamespaceInHeaderCheck>("autocrypt-mobis-cert-cpp-dcl59");
    // EXP
    CheckFactories.registerCheck<bugprone::DanglingHandleCheck>("autocrypt-mobis-cert-cpp-exp54");
    CheckFactories.registerCheck<cppcoreguidelines::ProTypeConstCastCheck>("autocrypt-mobis-cert-cpp-exp55");
    CheckFactories.registerCheck<bugprone::UseAfterMoveCheck>("autocrypt-mobis-cert-cpp-exp63");
    // CTR
    CheckFactories.registerCheck<cppcoreguidelines::ProBoundsPointerArithmeticCheck>("autocrypt-mobis-cert-cpp-ctr54");
    CheckFactories.registerCheck<cppcoreguidelines::ProBoundsPointerArithmeticCheck>("autocrypt-mobis-cert-cpp-ctr55");
    CheckFactories.registerCheck<cppcoreguidelines::ProBoundsPointerArithmeticCheck>("autocrypt-mobis-cert-cpp-ctr56");
    // ERR
    CheckFactories.registerCheck<cert::SetLongJmpCheck>("autocrypt-mobis-cert-cpp-err52");
    CheckFactories.registerCheck<cert::StaticObjectExceptionCheck>("autocrypt-mobis-cert-cpp-err58");
    CheckFactories.registerCheck<misc::ThrowByValueCatchByReferenceCheck>("autocrypt-mobis-cert-cpp-err61");
    CheckFactories.registerCheck<cert::StrToNumCheck>("autocrypt-mobis-cert-cpp-err62");
    // OOP
    CheckFactories.registerCheck<bugprone::UnhandledSelfAssignmentCheck>("autocrypt-mobis-cert-cpp-oop54");
    CheckFactories.registerCheck<cert::NonTrivialTypesLibcMemoryCallsCheck>("autocrypt-mobis-cert-cpp-oop57");
    CheckFactories.registerCheck<cert::MutatingCopyCheck>("autocrypt-mobis-cert-cpp-oop58");
    // CON
    CheckFactories.registerCheck<bugprone::SpuriouslyWakeUpFunctionsCheck>("autocrypt-mobis-cert-cpp-con54");
    // MSC
    CheckFactories.registerCheck<cert::LimitedRandomnessCheck>("autocrypt-mobis-cert-cpp-msc50");
    CheckFactories.registerCheck<cert::ProperlySeededRandomGeneratorCheck>("autocrypt-mobis-cert-cpp-msc51");
  }

  ClangTidyOptions getModuleOptions() override {
    ClangTidyOptions Options;
    ClangTidyOptions::OptionMap &Opts = Options.CheckOptions;
    Opts["cert-oop54-cpp.WarnOnlyIfThisHasSuspiciousField"] = "false";
    return Options;
  }
};

// Register the AutocryptModule using this statically initialized variable.
static ClangTidyModuleRegistry::Add<AutocryptModule>
    X("autocrypt-module", 
      "Adds link checks corresponding to Autocrypt secure coding guidelines.");

} // namespace autocrypt

// This anchor is used to force the linker to link in the generated object file
// and thus register the AutocryptModule.
volatile int AutocryptModuleAnchorSource = 0;

} // namespace clang::tidy
