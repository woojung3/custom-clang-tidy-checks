//===--- EmptyCheck.cpp - clang-tidy --------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "EmptyCheck.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::autocrypt {

void EmptyCheck::registerMatchers(MatchFinder *Finder) {
    Finder->addMatcher(
            cxxMemberCallExpr(
                on(hasType(cxxRecordDecl(hasName("AaslkdjfAasdlkfjzoxcib")))),
                callee(functionDecl(
                    decl().bind("nowhere_decl"),
                    hasName("fooAaslkdjfAasdlkfjzoxcibfoo"),
                    hasParameter(0, hasType(isInteger()))))),
            this);
}

void EmptyCheck::check(const MatchFinder::MatchResult &Result) {
  // Do nothing
}

} // namespace clang::tidy::autocrypt
