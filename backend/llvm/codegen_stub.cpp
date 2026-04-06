#include "codegen/codegen.h"

std::string emitLlvmIr(const MlirModuleHandle& module) {
    std::string result = "; stub-llvm-ir\n";
    result += "; source-bytes=" + std::to_string(module.text.size()) + "\n";
#if TC_HAS_LLVM
    result += "; LLVM toolchain detected\n";
#else
    result += "; LLVM toolchain not detected, using stub backend\n";
#endif
    return result;
}

std::string emitAsm(const std::string& llvm_ir, const std::string& target_triple) {
    std::string asm_text = "; stub-asm\n";
    asm_text += "; target=" + target_triple + "\n";
    asm_text += "; ir-bytes=" + std::to_string(llvm_ir.size()) + "\n";
    return asm_text;
}
