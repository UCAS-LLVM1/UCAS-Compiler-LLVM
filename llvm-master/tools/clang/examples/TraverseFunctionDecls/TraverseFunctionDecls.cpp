//===- PrintFunctionNames.cpp ---------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Example clang plugin which simply prints the names of all the top-level decls
// in the input file.
//
//===----------------------------------------------------------------------===//

#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/Frontend/CompilerInstance.h"
#include "llvm/Support/raw_ostream.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"
#include <map>
using namespace clang;

namespace {
class TraverseFunctionDeclsVisitor
    : public RecursiveASTVisitor<TraverseFunctionDeclsVisitor> {
public:
    explicit TraverseFunctionDeclsVisitor(ASTContext *Context)
        : Context(Context) {}

    bool TraverseDecl(Decl *DeclNode) {
        if (DeclNode == NULL) {
            return true;
        }
        if (const FunctionDecl *FD = dyn_cast<FunctionDecl>(DeclNode)) {
            std::string name = FD -> getNameAsString();
//Place to modify
            unsigned rule = 0;
            //unsigned rule = FD -> getAsCheckRule();
            funcNamesToAsCheckRule[FD->getNameAsString()] = rule;
/*
            if(rule != 0) {
                funcNamesToAsCheckRule[FD->getNameAsString()] = FD -> getAsCheckRule();
            } else {
				std::map<std::string, unsigned>::iterator it = funcNamesToAsCheckRule.find(name);
				if(it == funcNamesToAsCheckRule.end())
					funcNamesToAsCheckRule[FD->getNameAsString()] = FD -> getAsCheckRule();
            }
*/
        }
        return RecursiveASTVisitor<TraverseFunctionDeclsVisitor>::TraverseDecl(DeclNode);
    }
    void OutputAsCheckRules() {
        for(std::map<std::string, unsigned>::iterator it = funcNamesToAsCheckRule.begin(); it != funcNamesToAsCheckRule.end(); ++it) {
            llvm::outs() << it -> first << ": " << it -> second << "\n";
        }
    }
private:
    ASTContext *Context;
    std::map<std::string, unsigned> funcNamesToAsCheckRule;
};

class TraverseFunctionDeclsConsumer : public ASTConsumer {
public:
    explicit TraverseFunctionDeclsConsumer(ASTContext *Context)
        : Visitor(Context) {}

    virtual void HandleTranslationUnit(ASTContext &Context) {
        Visitor.TraverseDecl(Context.getTranslationUnitDecl());
        Visitor.OutputAsCheckRules();
    }
private:
    TraverseFunctionDeclsVisitor Visitor;
};

class TraverseFunctionDeclsAction : public PluginASTAction {
protected:
    ASTConsumer *CreateASTConsumer(CompilerInstance &CI, llvm::StringRef) {
        return new TraverseFunctionDeclsConsumer(&CI.getASTContext());
    }

    bool ParseArgs(const CompilerInstance &CI,
                   const std::vector<std::string>& args) {
        return true;
    }

};
}

static FrontendPluginRegistry::Add<TraverseFunctionDeclsAction>
X("traverse-fn-decls", "print function names and as-check rule");
