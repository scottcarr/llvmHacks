#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/InstrTypes.h"

using namespace llvm;

struct MyAnnotation{
    public:
    void* p0;
    void* p1;
    void* p2;
    int i0;
};

void printType(Type &type) {
    Type::TypeID tid = type.getTypeID();
    Type* pointee;
    switch (tid) {
        case Type::PointerTyID: 
            pointee = type.getPointerElementType();
            errs() << "*";
            printType(*pointee);
            break;
        case Type::IntegerTyID:
            errs() << "int\n";
            break;
        default:
            errs() << "unknown type \n";
            return;
    }
}

void printPointeeString(Constant *c) {
    Constant* o0 = (Constant*)c->getOperand(0);
    Constant* o1 = (Constant*)c->getOperand(1);
    Constant* o2 = (Constant*)c->getOperand(2);

    //errs() << o00 << "\n";
    ConstantDataSequential* o00 = (ConstantDataSequential*)o0->getOperand(0);
    //ArrayType* aty = o00->getType();
    //errs() << aty->getNumElements() << "\n";
    errs() << o00->getAsCString() << "\n";
}

namespace {
    struct mine: public ModulePass {
        static char ID;
        mine() : ModulePass(ID) {}
        virtual bool runOnModule(Module &M) {
            //errs() << "Hello: ";
            //errs().write_escaped(M.getModuleIdentifier()) << "\n";
            for (Module::global_iterator I = M.global_begin(), E = M.global_end();
                    I != E;
                    ++I) {
                if (I->getName() == "llvm.global.annotations") {
                    //errs() << "found it\n";
                    //errs() << I->getSection() << "\n";
                    //I->printAsOperand(errs(), true, &M);
                    //errs() << "\n";
                    //errs() << I->getNumOperands() << "\n";

                    Value *Op0 = I->getOperand(0);
                    //Op0->printAsOperand(errs(), true, &M);
                    //errs() << "\n";

                    ConstantArray *arr = (ConstantArray*)(Op0);
                    ConstantStruct *annoStruct = (ConstantStruct*)(arr->getOperand(0));

                    //errs() << annoStruct->getNumOperands() << "\n";

                    Constant* cast = annoStruct->getOperand(0);
                    Value* val = cast->getOperand(0);
                    //Value* var = annoStruct->getOperand(0);
                    Constant* ann = annoStruct->getOperand(1);
                    Constant* loc = annoStruct->getOperand(2);
                    Constant* num = annoStruct->getOperand(3);


                    //errs() << val->getNumOperands() << "\n";

                    errs() << "Annotated variable: " << val->getName() << " : ";
                    printType(*val->getType());
                    errs() << "Annotated as: ";
                    printPointeeString(ann);

                    //val->printAsOperand(errs(), true, &M);
                    //errs() << "\n";
                    //ann->printAsOperand(errs(), true, &M);
                    //errs() << "\n";
                    //errs() << ann->getNumOperands() << "\n";
                    //o00->printAsOperand(errs(), true, &M);
                    //errs() << "\n";

                    //errs() << o00->getSplatValue << "\n";
                    //o0->printAsOperand(errs(), true, &M);
                    //errs() << "\n";
                    //o1->printAsOperand(errs(), true, &M);
                    //errs() << "\n";
                    //o2->printAsOperand(errs(), true, &M);
                    //errs() << "\n";

                    errs() << "In file: ";
                    printPointeeString(loc);
                    //loc->printAsOperand(errs(), true, &M);
                    //errs() << "\n";

                    // I have no idea what this number means
                    num->printAsOperand(errs(), true, &M);
                    errs() << "\n";

                    //Value *Op1 = Op0->getOperand(0);
                    //Op1->printAsOperand(errs(), true, &M);
                    //errs() << "\n";
                    //MyAnnotation *ann = (MyAnnotation*)Op0;
                    //errs() << ann->p0 << "\n";
                    //errs() << ann->p1 << "\n";
                    //errs() << ann->p2 << "\n";
                    //errs() << ann->i0 << "\n";

                }
            }
            return false;
        }
    };
}

char mine::ID = 0;
static RegisterPass<mine> X("mine", "mine pass", false, false);

/*
namespace {
    struct mine: public FunctionPass {
        static char ID;
        mine() : FunctionPass(ID) {}
        virtual bool runOnFunction(Function &F) {
            errs() << "Hello: ";
            errs().write_escaped(F.getName()) << "\n";
            return false;
        }
    };
}

char mine::ID = 0;
static RegisterPass<mine> X("mine", "mine pass", false, false);
*/
