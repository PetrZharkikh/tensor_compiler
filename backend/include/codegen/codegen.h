#pragma once

#include <string>

#include "ir/lowering.h"

std::string emitLlvmIr(const MlirModuleHandle& module);
std::string emitAsm(const std::string& llvm_ir, const std::string& target_triple);
