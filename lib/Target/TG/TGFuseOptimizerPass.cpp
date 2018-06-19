#include "TG.h"
#include "TGBackend.h"
#include <onnc/Core/ModulePass.h>
#include <onnc/Core/PassSupport.h>
#include <onnc/IR/Dump.h>
#include <onnx/common/ir.h>

using namespace onnc;

namespace {

class ONNXFuseOpt : public ModulePass
{
public:
  static char ID;

public:
  ONNXFuseOpt(TGBackend *pTarget) : ModulePass(ID), m_pTarget(pTarget) {}

  Pass::ReturnType runOnModule(Module &pModule) override;

private:
  TGBackend *m_pTarget; // NOLINT
};

Pass::ReturnType ONNXFuseOpt::runOnModule(Module &pModule)
{
  TGFuseOptimizer *TFO = m_pTarget->getFuseOptimizr();
  TFO->PrepareFuseOptimizer(pModule);
  onnx::Graph *graph = pModule.getGraphIR().get();
  bool changed = TFO->FuseOptimization(graph);
  // TODO Module::print is unfinished
  // plz uncomment below code for debugging..
  // DumpGraph(*graph);
  return changed ? Pass::kModuleChanged : Pass::kModuleNoChanged;
}

} // namespace
char ONNXFuseOpt::ID = 0;

ModulePass *onnc::createONNXFuseOptPass(TGBackend *pTarget)
{
  return new ONNXFuseOpt(pTarget);
}