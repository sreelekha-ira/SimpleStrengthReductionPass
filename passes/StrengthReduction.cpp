\
    #include "llvm/IR/IRBuilder.h"
    #include "llvm/IR/Instructions.h"
    #include "llvm/IR/PassManager.h"
    #include "llvm/Pass.h"
    #include "llvm/Support/raw_ostream.h"
    #include "llvm/IR/Constants.h"
    #include "llvm/IR/PatternMatch.h"

    using namespace llvm;
    using namespace llvm::PatternMatch;

    namespace {

    class StrengthReductionPass : public PassInfoMixin<StrengthReductionPass> {
    public:
      PreservedAnalyses run(Function &F, FunctionAnalysisManager &) {
        bool Changed = false;

        for (BasicBlock &BB : F) {
          for (Instruction &I : make_early_inc_range(BB)) {
            // match integer mul
            // pattern: mul iX %v, const 2  OR mul iX const 2, %v
            Value *Op0 = I.getOperand(0);
            Value *Op1 = I.getOperand(1);

            ConstantInt *C = nullptr;
            Value *V = nullptr;

            // check both orders
            if ((C = dyn_cast<ConstantInt>(Op1))) {
              V = Op0;
            } else if ((C = dyn_cast<ConstantInt>(Op0))) {
              V = Op1;
            }

            if (!C) continue;
            if (!I.getType()->isIntegerTy()) continue;

            // check it's an integer 2
            if (C->equalsInt(2)) {
              // ensure instruction is an integer mul
              if (I.getOpcode() == Instruction::Mul) {
                IRBuilder<> B(&I);
                // shift amount must be same bitwidth (constant)
                Value *Shift = B.getIntN(C->getBitWidth(), 1);
                Value *Shl = B.CreateShl(V, Shift, "sr_shl");
                errs() << "StrengthReduction: replaced mul by 2 with shl in function "
                       << F.getName() << "\\n";
                I.replaceAllUsesWith(Shl);
                I.eraseFromParent();
                Changed = true;
                // 'I' is erased; continue to next
              }
            }
          }
        }

        return Changed ? PreservedAnalyses::none() : PreservedAnalyses::all();
      }
    };

    } // namespace

    // Plugin interface
    extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
    llvmGetPassPluginInfo() {
      return {
          LLVM_PLUGIN_API_VERSION, "strength-reduction", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "strength-reduction") {
                    FPM.addPass(StrengthReductionPass());
                    return true;
                  }
                  return false;
                });
          }};
    }
