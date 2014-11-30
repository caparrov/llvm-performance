//=-------------------- llvm/Support/DynamicAnalysis.h ------======= -*- C++ -*//
//
//                     The LLVM Compiler Infrastructure
//
//  Vtetoria Caparros Cabezas <caparrov@inf.ethz.ch>
//===----------------------------------------------------------------------===//

#define INTERPRETER

#ifdef INTERPRETER
#include "llvm/Support/DynamicAnalysis.h"
#else
#include "DynamicAnalysis.h"
#endif

#ifndef INTERPRETER
#define DEBUG_TYPE "dynamic-analysis"
#endif

//===----------------------------------------------------------------------===//
//                        Constructor of the analyzer
//===----------------------------------------------------------------------===//

DynamicAnalysis::DynamicAnalysis(string TargetFunction,
                                 string Microarchitecture,
                                 unsigned MemoryWordSize,
                                 unsigned CacheLineSize,
                                 unsigned L1CacheSize,
                                 unsigned L2CacheSize,
                                 unsigned LLCCacheSize,
                                 vector<float> ExecutionUnitsLatency,
                                 vector<float> ExecutionUnitsThroughput,
                                 vector<int> ExecutionUnitsParallelIssue,
                                 vector<unsigned>  MemAccessGranularity,
                                 int AddressGenerationUnits,
                                 int InstructionFetchBandwidth,
                                 int ReservationStationSize,
                                 int ReorderBufferSize,
                                 int LoadBufferSize,
                                 int StoreBufferSize,
                                 int LineFillBufferSize,bool WarmCache,
                                 bool x86MemoryModel,
                                 bool SpatialPrefetcher,
                                 bool ConstraintPorts,
                                 bool ConstraintAGUs,
                                 int rep,
                                 bool InOrderExecution,
                                 bool ReportOnlyPerformance,
                                 unsigned PrefetchLevel,
                                 unsigned PrefetchDispatch,
                                 unsigned PrefetchTarget){
  
  
  vector< unsigned > emptyVector;
  // =================== Sandy Bridge config ================================//
  
  if (Microarchitecture.compare("SB") == 0) {
    uarch = SANDY_BRIDGE;
    nExecutionUnits = SANDY_BRIDGE_EXECUTION_UNITS;
    nCompExecutionUnits = SANDY_BRIDGE_COMP_EXECUTION_UNITS;
    nMemExecutionUnits = SANDY_BRIDGE_MEM_EXECUTION_UNITS;
    nCompNodes = SANDY_BRIDGE_COMP_NODES;
    nMemNodes = SANDY_BRIDGE_MEM_NODES;
    nPorts = SANDY_BRIDGE_DISPATCH_PORTS;
    nBuffers = SANDY_BRIDGE_BUFFERS;
    nAGUs = SANDY_BRIDGE_AGU;
    nLoadAGUs = SANDY_BRIDGE_LOAD_AGUS;
    nStoreAGUs = SANDY_BRIDGE_STORE_AGUS;
    nPrefetchNodes = SANDY_BRIDGE_PREFETCH_NODES;
    nNodes = SANDY_BRIDGE_NODES;
    
    // Mapping between nodes and execution units. ExecutionUnit[] vector contains
    // one entry for every type of node, and the associated execution unit.
    
    for (unsigned i = 0; i < nNodes; i++)
      ExecutionUnit.push_back(0);
    
    ExecutionUnit[FP_ADD_NODE] = FP_ADDER;
    ExecutionUnit[FP_MUL_NODE] = FP_MULTIPLIER;
    ExecutionUnit[FP_DIV_NODE] = FP_DIVIDER;
    ExecutionUnit[VECTOR_SHUFFLE_NODE] = FP_SHUFFLE;
    ExecutionUnit[L1_STORE_NODE] = L1_STORE_CHANNEL;
    ExecutionUnit[L1_LOAD_NODE] = L1_LOAD_CHANNEL;
    ExecutionUnit[L2_STORE_NODE] = L2_STORE_CHANNEL;
    ExecutionUnit[L2_LOAD_NODE] = L2_LOAD_CHANNEL;
    ExecutionUnit[L3_STORE_NODE] = L3_STORE_CHANNEL;
    ExecutionUnit[L3_LOAD_NODE] = L3_LOAD_CHANNEL;
    ExecutionUnit[MEM_STORE_NODE] = MEM_STORE_CHANNEL;
    ExecutionUnit[MEM_LOAD_NODE] = MEM_LOAD_CHANNEL;
    
    ExecutionUnit[AGU_NODE] = ADDRESS_GENERATION_UNIT ;
    
    ExecutionUnit[PORT_0_NODE] = PORT_0;
    ExecutionUnit[PORT_1_NODE] = PORT_1;
    ExecutionUnit[PORT_2_NODE] = PORT_2;
    ExecutionUnit[PORT_3_NODE] = PORT_3;
    ExecutionUnit[PORT_4_NODE] = PORT_4;
    
    // Mapping between execution units and dispatch ports
    
    for (unsigned i = 0; i < nCompNodes+nMemNodes; i++)
      DispatchPort.push_back(emptyVector);
    
    /*
     Port 0 -> FP_ADDER
     Port 1 -> FP_MULTIPLIER, FP_DIVIDER, FP_SHUFFLE
     Port 2 -> STORE_CHANNEL (L1, L2, L3 and MEM)
     Port 3 -> LOAD (L1, L2, L3 and MEM)
     Port 4 -> LOAD (L1, L2, L3 and MEM)
     */
    // Associate Dispatch ports to nodes instead of execution resources. Because
    // otherwise there is a problem when different nodes share execution unit
    // but no dispatch ports
    emptyVector.push_back(0);
    DispatchPort[FP_ADD_NODE] = emptyVector;
    emptyVector.clear();
    emptyVector.push_back(1);
    DispatchPort[FP_MUL_NODE] = emptyVector;
    emptyVector.clear();
    emptyVector.push_back(1);
    DispatchPort[FP_DIV_NODE] = emptyVector;
    emptyVector.clear();
    emptyVector.push_back(1);
    DispatchPort[VECTOR_SHUFFLE_NODE] = emptyVector;
    emptyVector.clear();
    emptyVector.push_back(2);
    DispatchPort[L1_STORE_NODE] = emptyVector;
    DispatchPort[L2_STORE_NODE] = emptyVector;
    DispatchPort[L3_STORE_NODE] = emptyVector;
    DispatchPort[MEM_STORE_NODE] = emptyVector;
    emptyVector.clear();
    emptyVector.push_back(3);
    emptyVector.push_back(4);
    DispatchPort[L1_LOAD_NODE] = emptyVector;
    DispatchPort[L2_LOAD_NODE] = emptyVector;
    DispatchPort[L3_LOAD_NODE] = emptyVector;
    DispatchPort[MEM_LOAD_NODE] = emptyVector;
    
    for (unsigned i = 0; i < nPorts; i++) {
      ExecutionPort.push_back(0);
      PortNodes.push_back(0);
    }
    
    ExecutionPort[0] = PORT_0;
    ExecutionPort[1] = PORT_1;
    ExecutionPort[2] = PORT_2;
    ExecutionPort[3] = PORT_3;
    ExecutionPort[4] = PORT_4;
    
    PortNodes[0] = PORT_0_NODE;
    PortNodes[1] = PORT_1_NODE;
    PortNodes[2] = PORT_2_NODE;
    PortNodes[3] = PORT_3_NODE;
    PortNodes[4] = PORT_4_NODE;
  }
  
  // =================== Atom config ================================//
  
  if (Microarchitecture.compare("ATOM") == 0) {
    uarch = ATOM;
    nExecutionUnits = ATOM_EXECUTION_UNITS;
    nPorts = ATOM_DISPATCH_PORTS;
    nBuffers = ATOM_BUFFERS;
    nAGUs = ATOM_AGUS;
    nLoadAGUs = ATOM_LOAD_AGUS;
    nStoreAGUs = ATOM_STORE_AGUS;
  }
  
  
  // =================== Some general checkings ================================//
  
  if(!ExecutionUnitsLatency.empty() && ExecutionUnitsLatency.size() !=
     nExecutionUnits)
    report_fatal_error("The number of latencies does not match the number of execution units");
  
  if(!ExecutionUnitsThroughput.empty() && ExecutionUnitsThroughput.size() !=
     nExecutionUnits)
    report_fatal_error("The number of throughputs does not match the number of execution units");
  
  if(!ExecutionUnitsParallelIssue.empty() && ExecutionUnitsParallelIssue.size() !=
     nExecutionUnits)
    report_fatal_error("The number of execution units parallel issue does not match the number of execution units");
  if (L1CacheSize < CacheLineSize)
    report_fatal_error("L1 cache size < cache line size");
  if (L2CacheSize < CacheLineSize)
    report_fatal_error("L2 cache size < cache line size");
  if (LLCCacheSize < CacheLineSize)
    report_fatal_error("LLC cache size < cache line size");
  
  if (CacheLineSize % MemoryWordSize != 0)
    report_fatal_error("Cache line size is not a multiple of memory word size");
  
  // Initialize local variables with command-line arguemtns
  this->TargetFunction = TargetFunction;
  this->MemoryWordSize = MemoryWordSize;
  this->CacheLineSize = CacheLineSize/(this->MemoryWordSize);
  
  // If caches sizes are not multiple of a power of 2, force it.
  this->L1CacheSize = roundNextPowerOfTwo(L1CacheSize);
  this->L2CacheSize = roundNextPowerOfTwo(L2CacheSize);
  this->LLCCacheSize = roundNextPowerOfTwo(LLCCacheSize);
  this->L1CacheSize = this->L1CacheSize /CacheLineSize;
  this->L2CacheSize = this->L2CacheSize/CacheLineSize;
  this->LLCCacheSize = this->LLCCacheSize/CacheLineSize;
  
  this->AddressGenerationUnits = AddressGenerationUnits;
  this->ReservationStationSize = ReservationStationSize;
  this->InstructionFetchBandwidth = InstructionFetchBandwidth;
  this->ReorderBufferSize = ReorderBufferSize;
  this->LoadBufferSize = LoadBufferSize;
  this->StoreBufferSize = StoreBufferSize;
  this->LineFillBufferSize = LineFillBufferSize;
  this->WarmCache = WarmCache;
  this->x86MemoryModel = x86MemoryModel;
  this->SpatialPrefetcher = SpatialPrefetcher;
  this->ConstraintPorts = ConstraintPorts;
  this->ConstraintAGUs = ConstraintAGUs;
  this->InOrderExecution = InOrderExecution;
  this->ReportOnlyPerformance = ReportOnlyPerformance;
  this->rep = rep;
  
  
  LoadBufferCompletionCyclesTree = NULL;
  DispatchToLoadBufferQueueTree = NULL;
  MinLoadBuffer = 0;
  MaxDispatchToLoadBufferQueueTree = 0;
  
  BitsPerCacheLine = log2(this->CacheLineSize * (this->MemoryWordSize));
  
  
  
  // In reality is if L2, but need to specify the size for the reuse disrance
  switch (PrefetchLevel) {
    case 1:
      this->PrefetchLevel =0;
      PrefetchDestination=L1_LOAD_CHANNEL;
      break;
    case 2:
      this->PrefetchLevel =this->L1CacheSize;
      PrefetchDestination = L2_LOAD_CHANNEL;
      break;
    case 3:
      this->PrefetchLevel =this->L2CacheSize;
      PrefetchDestination = L3_LOAD_CHANNEL;
      break;
    default:
      report_fatal_error("Prefetch level not recognized");
      break;
  }
  
  
  switch (PrefetchDispatch) {
    case 0:
      this->PrefetchDispatch = 0;
      break;
    case 1:
      this->PrefetchDispatch = L1_STORE_NODE;
      break;
    case 2:
      this->PrefetchDispatch = L2_STORE_NODE;
      break;
    case 3:
      this->PrefetchDispatch = L3_STORE_NODE;
      break;
    default:
      report_fatal_error("Prefetch dispatch not recognized");
      break;
  }
  
  switch (PrefetchTarget) {
    case 2:
      this->PrefetchTarget = L1_STORE_CHANNEL;
      break;
    case 3:
      this->PrefetchTarget = L2_STORE_CHANNEL;
      break;
    case 4:
      this->PrefetchTarget = L3_STORE_CHANNEL;
      break;
    default:
      report_fatal_error("Prefetch target not recognized");
      break;
  }
  
  VectorWidth = 1;
  
  
 
  
  
  
  // Make sure that there are no more parallel execution units that dispatch ports associated
  // to these units
  for (unsigned i = 0; i < nCompNodes +nMemNodes; i++) { // Dispatch ports are associated to nodes
    if (ExecutionUnitsParallelIssue[ExecutionUnit[i]] > 0 && DispatchPort[i].size() < (unsigned)ExecutionUnitsParallelIssue[ExecutionUnit[i]]) {
      DEBUG(dbgs() << "ExecutionUnit " << i << "\n");
      DEBUG(dbgs() << "DispatchPort[i].size() " << DispatchPort[i].size() << "\n");
      DEBUG(dbgs() << "ExecutionUnitsParallelIssue[i] " << ExecutionUnitsParallelIssue[i] << "\n");
      report_fatal_error("There are more execution units that ports that can dispatch them\n");
    }
  }
  
  if (!MemAccessGranularity.empty() && MemAccessGranularity.size() != nMemExecutionUnits)
    report_fatal_error("Mem access granularities do not match the number of memory execution units");
  
  // We have latency and throughput for every resource in which we schedule nodes.
  // Latency and throughput of execution resources can be specified via command line.
  for (unsigned i = 0; i< nExecutionUnits; i++) {
    this->ExecutionUnitsLatency.push_back(1); //Default value for latency
    this->ExecutionUnitsThroughput.push_back(-1); // Infinite throughput
    this->ExecutionUnitsParallelIssue.push_back(1); // Infinite throughput
    
    if (i < nCompExecutionUnits) {
      AccessGranularities.push_back(1);
    }else{
      AccessGranularities.push_back(this->MemoryWordSize);
    }
  }
  
  if (!ExecutionUnitsLatency.empty()){
    for (unsigned i = 0; i< nExecutionUnits; i++)
      this->ExecutionUnitsLatency[i] = ceil(ExecutionUnitsLatency[i]);
    RARDependences = true;
  }else
    RARDependences = false;
  
  if (!ExecutionUnitsThroughput.empty())
    for (unsigned i = 0; i< nExecutionUnits; i++)
      this->ExecutionUnitsThroughput[i] = ExecutionUnitsThroughput[i];
  
  if (!ExecutionUnitsParallelIssue.empty())
    for (unsigned i = 0; i< nExecutionUnits; i++)
      this->ExecutionUnitsParallelIssue[i] = ExecutionUnitsParallelIssue[i];
  
  if (!MemAccessGranularity.empty())
    for (unsigned i = 0; i< nMemExecutionUnits; i++)
      AccessGranularities[i+nCompExecutionUnits] = MemAccessGranularity[i];
  
  // Latency and throughput of AGUs
  if (nAGUs > 0) {
    this->ExecutionUnitsLatency.push_back(1);
    if (ConstraintAGUs) {
      this->ExecutionUnitsThroughput.push_back(1);
      this->ExecutionUnitsParallelIssue.push_back(SANDY_BRIDGE_AGUS);
    }else{
      this->ExecutionUnitsThroughput.push_back(-1);
      this->ExecutionUnitsParallelIssue.push_back(-1);
    }
    AccessGranularities.push_back(1);
  }
  
  if (nLoadAGUs > 0) {
    this->ExecutionUnitsLatency.push_back(1);
    this->ExecutionUnitsThroughput.push_back(1);
    this->ExecutionUnitsParallelIssue.push_back(nLoadAGUs);
    AccessGranularities.push_back(1);
  }
  
  if (nStoreAGUs > 0) {
    this->ExecutionUnitsLatency.push_back(1);
    this->ExecutionUnitsThroughput.push_back(1);
    this->ExecutionUnitsParallelIssue.push_back(nStoreAGUs);
    AccessGranularities.push_back(1);
  }
  
  // Latency and throughput of ports
  for (unsigned i = 0; i< nPorts; i++) {
    this->ExecutionUnitsLatency.push_back(1); //Default value for latency
    if (ConstraintPorts) {
      this->ExecutionUnitsThroughput.push_back(1); // Infinite throughput
      this->ExecutionUnitsParallelIssue.push_back(1);
    }else{
      this->ExecutionUnitsThroughput.push_back(-1); // Infinite throughput
      this->ExecutionUnitsParallelIssue.push_back(-1);
    }
    AccessGranularities.push_back(1);
  }
  
  // Latency and throughput of buffers. Although it has no effect, these
  // values are used
  for (unsigned i = 0; i< nBuffers; i++) {
    this->ExecutionUnitsLatency.push_back(1); //Default value for latency
    this->ExecutionUnitsThroughput.push_back(1); // Infinite throughput
    this->ExecutionUnitsParallelIssue.push_back(1);
    AccessGranularities.push_back(1);
  }
  
  
  // We need AccessWidth and Throughput for every resource for which we calculte
  // span, including ports
  for (unsigned i = 0; i < nExecutionUnits + nAGUs +nPorts + nBuffers; i++) {
    
    unsigned IssueCycleGranularity = 0;
    unsigned AccessWidth = 0;
    
    if (i < nCompExecutionUnits){
      AccessWidth = VectorWidth;
    }else{
      if (i >= nCompExecutionUnits && i < nCompExecutionUnits + nMemExecutionUnits) {
        AccessWidth = roundNextMultiple(VectorWidth*MemoryWordSize, AccessGranularities[i]);
        // Round throughput of memory resources to the next multiple of MemoryWordSize
        this->ExecutionUnitsThroughput[i] = roundNextMultiple(this->ExecutionUnitsThroughput[i],this->MemoryWordSize);
      }else{
        AccessWidth = 1;
      }
    }
    
    if (this->ExecutionUnitsThroughput[i]>0) {
      DEBUG(dbgs() << "AccessWidth " << AccessWidth << "\n");
      DEBUG(dbgs() << "ExecutionUnitsThroughput[i] " << this->ExecutionUnitsThroughput[i] << "\n");
      IssueCycleGranularity = ceil(AccessWidth/this->ExecutionUnitsThroughput[i]);
    }else
      IssueCycleGranularity = 1;
    
    AccessWidths.push_back(AccessWidth);
    IssueCycleGranularities.push_back(IssueCycleGranularity);
    DEBUG(dbgs() << "IssueCycleGranularities["<<i<<"]=" << IssueCycleGranularities[i] << "\n");
  }
  
  DEBUG(dbgs() << "Number of resources " << nExecutionUnits + nPorts + nAGUs + nLoadAGUs + nStoreAGUs + nBuffers << "\n");
  
  ResourcesNames.push_back("FP_ADDER");
  ResourcesNames.push_back("FP_MULT");
  ResourcesNames.push_back("FP_DIV");
  ResourcesNames.push_back("FP_SHUF");
  ResourcesNames.push_back("L1_LD");
  ResourcesNames.push_back("L1_ST");
  ResourcesNames.push_back("L2");
  ResourcesNames.push_back("L3");
  ResourcesNames.push_back("MEM");
  
  /*
   ResourcesNames.push_back("L2_PREFETCH");
   ResourcesNames.push_back("L3_PREFETCH");
   ResourcesNames.push_back("MEM_PREFETCH");
   */
  ResourcesNames.push_back("AGU");
  
  ResourcesNames.push_back("PORT_0");
  ResourcesNames.push_back("PORT_1");
  ResourcesNames.push_back("PORT_2");
  ResourcesNames.push_back("PORT_3");
  ResourcesNames.push_back("PORT_4");
  
  ResourcesNames.push_back("RS");
  ResourcesNames.push_back("ROB");
  ResourcesNames.push_back("LB");
  ResourcesNames.push_back("SB");
  ResourcesNames.push_back("LFB");
  
  // Nodes names
  NodesNames.push_back("FP_ADD_NODE");
  NodesNames.push_back("FP_MUL_NODE");
  NodesNames.push_back("FP_DIV_NODE");
  NodesNames.push_back("VECTOR_SHUFFLE_NODE");
  NodesNames.push_back("L1_LOAD_NODE ");
  NodesNames.push_back("L1_STORE_NODE");
  NodesNames.push_back("L2_LOAD_NODE ");
  NodesNames.push_back("L2_STORE_NODE");
  NodesNames.push_back("L3_LOAD_NODE ");
  NodesNames.push_back("L3_STORE_NODE");
  NodesNames.push_back("MEM_LOAD_NODE ");
  NodesNames.push_back("MEM_STORE_NODE");
  
  NodesNames.push_back("AGU_NODE ");
  
  NodesNames.push_back("PORT_0_NODE");
  NodesNames.push_back("PORT_1_NODE");
  NodesNames.push_back("PORT_2_NODE");
  NodesNames.push_back("PORT_3_NODE");
  NodesNames.push_back("PORT_4_NODE");
  
  NodesNames.push_back("RS_STALL_NODE");
  NodesNames.push_back("ROB_STALL_NODE");
  NodesNames.push_back("LB_STALL_NODE");
  NodesNames.push_back("SB_STALL_NODE");
  NodesNames.push_back("LFB_STALL_NODE");
  
  //Some checkings....
  if (ResourcesNames.size() !=  nExecutionUnits + nPorts + nAGUs + nLoadAGUs + nStoreAGUs + nBuffers)
    report_fatal_error("ResourcesNames does not match with number of resources");
  
  if (LoadBufferSize > 0 && ReservationStationSize == 0)
    report_fatal_error("RS cannot be zero if LB exists");
  
  if (StoreBufferSize > 0 && ReservationStationSize == 0)
    report_fatal_error("RS cannot be zero if SB exists");
  
  if (LineFillBufferSize > 0 && LoadBufferSize == 0)
    report_fatal_error("LB cannot be zero if LFB exists");
  
  //Check that access granularities are either memory word size or cache line size
  for (unsigned i = 0; i < MemAccessGranularity.size(); i++)
    if (MemAccessGranularity[i] != MemoryWordSize && MemAccessGranularity[i] != CacheLineSize )
      report_fatal_error("Memory access granularity is not memory word size, nor cache line size\n");
  
  if (InOrderExecution && ReorderBufferSize > 0)
    report_fatal_error("In order execution does not allow to have a ROB of size > 1");
  
  if (InOrderExecution)
    this->ReorderBufferSize =1;
  
  // Itinitalize global variables to zero...
  FunctionCallStack = 0;
  TotalInstructions = 0;
  BasicBlockBarrier = 0;
  BasicBlockLookAhead = 0;
  InstructionFetchCycle = 0;
  LoadDispatchCycle = 0;
  StoreDispatchCycle = 0;
  NFirstAccesses = 0;
  LastLoadIssueCycle = 0;
  LastStoreIssueCycle = 0;
  RemainingInstructionsFetch = InstructionFetchBandwidth;
  ReuseTree = NULL;
  PrefetchReuseTree = NULL;
  PrefetchReuseTreeSize = 0;
  
  // For resources with throughput and latency, i.e., resources for which we insert
  // cycles
  for (unsigned i = 0; i< nExecutionUnits + nPorts + nAGUs + nLoadAGUs + nStoreAGUs + nBuffers; i++) {
    InstructionsCount.push_back(0);
    InstructionsCountExtended.push_back(0);
    InstructionsSpan.push_back(0);
    InstructionsLastIssueCycle.push_back(0);
    IssueSpan.push_back(0);
    SpanGaps.push_back(0);
    FirstNonEmptyLevel.push_back(0);
    DAGLevelsOccupancy.push_back(emptyVector);
    MaxOccupancy.push_back(0);
    NInstructionsStalled.push_back(0);
    FirstIssue.push_back(false);
  }
  
  for (unsigned i = 0; i <nBuffers; i++)
    BuffersOccupancy.push_back(0);
  
  //Initially, FullOccupancyCyclesTree has one element
  FullOccupancyCyclesTree.push_back(NULL);
  
  SplitTreeRange = 100000;
  
  for (unsigned i = 0; i< nExecutionUnits + nPorts + nAGUs + nLoadAGUs + nStoreAGUs; i++)
    AvailableCyclesTree.push_back(NULL);
  
}



//===----------------------------------------------------------------------===//
//                General functions for the analysis
//===----------------------------------------------------------------------===//

#define HANDLE_MEMORY_TYPE(TY, type, operand)  \
Ty = I.getOperand(operand)->getType();  \
if(PointerType* PT = dyn_cast<PointerType>(Ty)){  \
switch (PT->getElementType()->getTypeID()) {  \
case Type::HalfTyID:  return FP_##TY##_16_BITS; \
case Type::FloatTyID:  return FP_##TY##_32_BITS;  \
case Type::DoubleTyID:  return FP_##TY##_64_BITS; \
case Type::X86_FP80TyID:  return FP_##TY##_80_BITS; \
case Type::PPC_FP128TyID:  return FP_##TY##_128_BITS; \
case Type::X86_MMXTyID:  return FP_##TY##_64_BITS;  \
case Type::VectorTyID: \
switch (PT->getElementType()->getVectorElementType()->getTypeID()) {  \
case Type::HalfTyID:  return FP_##TY##_16_BITS; \
case Type::FloatTyID:  \
return FP_##TY##_32_BITS;  \
case Type::DoubleTyID:  \
return FP_##TY##_64_BITS; \
case Type::IntegerTyID: \
IntegerTy = dyn_cast<IntegerType>(PT-> getElementType());  \
switch (IntegerTy -> getBitWidth()){  \
case 4: return INT_##TY##_4_BITS; \
case 8: return INT_##TY##_8_BITS; \
case 16: return INT_##TY##_16_BITS; \
case 32: return INT_##TY##_32_BITS; \
case 64: return INT_##TY##_64_BITS; \
default: return MISC;  \
} \
default: return MISC;\
}\
case Type::IntegerTyID: \
IntegerTy = dyn_cast<IntegerType>(PT-> getElementType());  \
switch (IntegerTy -> getBitWidth()){  \
case 4: return INT_##TY##_4_BITS; \
case 8: return INT_##TY##_8_BITS; \
case 16: return INT_##TY##_16_BITS; \
case 32: return INT_##TY##_32_BITS; \
case 64: return INT_##TY##_64_BITS; \
default: return MISC;  \
} \
default: return MISC; \
} \
}else{  \
errs() << "Trying to load a non-pointer type.\n"; \
} \



// Copy from Instruction.cpp-getOpcodeName()
int
DynamicAnalysis::getInstructionType(Instruction &I){
  
  IntegerType *IntegerTy;
  Type* Ty;
  unsigned Opcode = I.getOpcode();
  
  switch (Opcode) {
      // Terminators
    case Instruction::Ret:    return CTRL;
    case Instruction::Br:     return CTRL;
    case Instruction::Switch: return CTRL;
    case Instruction::IndirectBr: return CTRL;
    case Instruction::Invoke: return CTRL;
    case Instruction::Resume: return CTRL;
    case Instruction::Unreachable: return CTRL;
      
      // Standard binary operators...
    case Instruction::Add: return INT_ADD;
    case Instruction::FAdd: return FP_ADD;
    case Instruction::Sub: return INT_SUB;
    case Instruction::FSub: return FP_SUB;
    case Instruction::Mul: return INT_MUL;
    case Instruction::FMul: return FP_MUL;
    case Instruction::UDiv: return INT_DIV;
    case Instruction::SDiv: return INT_DIV;
    case Instruction::FDiv: return FP_DIV;
    case Instruction::URem: return INT_REM;
    case Instruction::SRem: return INT_REM;
    case Instruction::FRem: return FP_REM;
      
      // Logical operators...
    case Instruction::And: return MISC;
    case Instruction::Or : return MISC;
    case Instruction::Xor: return MISC;
      
      // Memory instructions...
    case Instruction::Alloca:        return MISC_MEM;
    case Instruction::Load:          HANDLE_MEMORY_TYPE(LD, Load, 0)
    case Instruction::Store:         HANDLE_MEMORY_TYPE(ST, Store, 1);
    case Instruction::AtomicCmpXchg: return MISC_MEM;
    case Instruction::AtomicRMW:     return MISC_MEM;
    case Instruction::Fence:         return MISC_MEM;
    case Instruction::GetElementPtr: return MISC_MEM;
      // Convert instructions...
    case Instruction::Trunc:     return MISC;
    case Instruction::ZExt:      return MISC;
    case Instruction::SExt:      return MISC;
    case Instruction::FPTrunc:   return MISC;
    case Instruction::FPExt:     return MISC;
    case Instruction::FPToUI:    return MISC;
    case Instruction::FPToSI:    return MISC;
    case Instruction::UIToFP:    return MISC;
    case Instruction::SIToFP:    return MISC;
    case Instruction::IntToPtr:  return MISC;
    case Instruction::PtrToInt:  return MISC;
    case Instruction::BitCast:   return MISC;
      
      // Other instructions...
    case Instruction::ICmp:           return CTRL;
    case Instruction::FCmp:           return CTRL;
    case Instruction::PHI:            return CTRL;
    case Instruction::Select:         return CTRL;
    case Instruction::Call:           return CTRL;
    case Instruction::Shl:            return MISC;
    case Instruction::LShr:           return MISC;
    case Instruction::AShr:           return MISC;
    case Instruction::VAArg:          return MISC;
    case Instruction::ExtractElement: return MISC;
    case Instruction::InsertElement:  return MISC;
    case Instruction::ShuffleVector:  return VECTOR_SHUFFLE;
    case Instruction::ExtractValue:   return MISC;
    case Instruction::InsertValue:    return MISC;
    case Instruction::LandingPad:     return MISC;
      
    default: return -1;
  }
}



//===----------------------------------------------------------------------===//
//                  Routines for Analysis of ILP
//===----------------------------------------------------------------------===//

uint64_t
DynamicAnalysis::getInstructionValueIssueCycle(Value* v){
  
  uint64_t InstructionIssueCycle = 0;
  map <Value*, uint64_t>::iterator IssueCycleMapIt;
  
  //&I
  IssueCycleMapIt = InstructionValueIssueCycleMap.find(v);
  if (IssueCycleMapIt != InstructionValueIssueCycleMap.end()) {
    InstructionIssueCycle = IssueCycleMapIt->second;
    // Reset the value of issue cyle after reading it so that
    // when the next time this instruction is executed, it it not poluted
    // with a previous value. This problems arises when two instances of the
    // same instruction are represented by the same value.
    //  InstructionValueIssueCycleMap.erase(IssueCycleMapIt);
    IssueCycleMapIt->second = 0;
  }else
    InstructionIssueCycle = 0; // First usage
  
  return InstructionIssueCycle;
}



CacheLineInfo
DynamicAnalysis::getCacheLineInfo(uint64_t v){
  
  CacheLineInfo Info;
  uint64_t CacheLineIssueCycle = 0;
  uint64_t CacheLineLastAccess = 0;
  map <uint64_t, CacheLineInfo>::iterator IssueCycleMapIt;
  
  //&I
  IssueCycleMapIt = CacheLineIssueCycleMap.find(v);
  if (IssueCycleMapIt != CacheLineIssueCycleMap.end()) {
    CacheLineIssueCycle = IssueCycleMapIt->second.IssueCycle;
    CacheLineLastAccess = IssueCycleMapIt->second.LastAccess;
  }else{
    CacheLineIssueCycle = 0; // First usage
    CacheLineLastAccess = 0;
  }
  
  Info.LastAccess = CacheLineLastAccess;
  Info.IssueCycle = CacheLineIssueCycle;
  // Now this is moved to to getMemoryAddressIssueCycle
  // Info.IssueCycle = max(max(InstructionFetchCycle,BasicBlockBarrier),InstructionIssueCycle);
  
  return (Info);
}



uint64_t
DynamicAnalysis::getCacheLineLastAccess(uint64_t v){
  
  uint64_t InstructionLastAccess = 0;
  map <uint64_t, CacheLineInfo>::iterator IssueCycleMapIt;
  
  //&I
  IssueCycleMapIt = CacheLineIssueCycleMap.find(v);
  if (IssueCycleMapIt != CacheLineIssueCycleMap.end())
    InstructionLastAccess = IssueCycleMapIt->second.LastAccess;
  
  return InstructionLastAccess;
}



uint64_t
DynamicAnalysis::getMemoryAddressIssueCycle(uint64_t v){
  uint64_t IssueCycle = 0;
  map <uint64_t, uint64_t>::iterator IssueCycleMapIt;
  
  //&I
  IssueCycleMapIt = MemoryAddressIssueCycleMap.find(v);
  if (IssueCycleMapIt != MemoryAddressIssueCycleMap.end())
    IssueCycle = IssueCycleMapIt->second;
  
  return IssueCycle;
}



void
DynamicAnalysis::insertInstructionValueIssueCycle(Value* v,uint64_t InstructionIssueCycle, bool isPHINode){
  
  map <Value*, uint64_t>::iterator IssueCycleMapIt;
  
  
  IssueCycleMapIt = InstructionValueIssueCycleMap.find(v);
  if (IssueCycleMapIt != InstructionValueIssueCycleMap.end()){
    if (isPHINode == true)
      IssueCycleMapIt->second = InstructionIssueCycle;
    else
      IssueCycleMapIt->second = max(IssueCycleMapIt->second, InstructionIssueCycle/*+1*/);
  }else //Insert an entry for the instrucion.
    InstructionValueIssueCycleMap[v] = InstructionIssueCycle/*+1*/;
  
}



void
DynamicAnalysis::insertCacheLineInfo(uint64_t v,CacheLineInfo Info){
  
  map <uint64_t, CacheLineInfo>::iterator IssueCycleMapIt;
  //*i
  IssueCycleMapIt = CacheLineIssueCycleMap.find(v);
  if (IssueCycleMapIt != CacheLineIssueCycleMap.end()){
    IssueCycleMapIt->second.IssueCycle = max(IssueCycleMapIt->second.IssueCycle, Info.IssueCycle/*+1*/);
    IssueCycleMapIt->second.LastAccess = Info.LastAccess;
  }else{ //Insert an entry for the instrucion.
    CacheLineIssueCycleMap[v].IssueCycle = Info.IssueCycle/*+1*/;
    CacheLineIssueCycleMap[v].LastAccess = Info.LastAccess;
  }
}



void
DynamicAnalysis::insertCacheLineLastAccess(uint64_t v,uint64_t LastAccess){
  
  map <uint64_t, CacheLineInfo>::iterator IssueCycleMapIt;
  //*i
  IssueCycleMapIt = CacheLineIssueCycleMap.find(v);
  if (IssueCycleMapIt != CacheLineIssueCycleMap.end()){
    IssueCycleMapIt->second.LastAccess = LastAccess;
  }else//Insert an entry for the instrucion.
    CacheLineIssueCycleMap[v].LastAccess = LastAccess;
}



void
DynamicAnalysis::insertMemoryAddressIssueCycle(uint64_t v,uint64_t Cycle ){
  map <uint64_t, uint64_t>::iterator IssueCycleMapIt;
  //*i
  IssueCycleMapIt = MemoryAddressIssueCycleMap.find(v);
  if (IssueCycleMapIt != MemoryAddressIssueCycleMap.end()){
    IssueCycleMapIt->second = Cycle;
  }else //Insert an entry for the instrucion.
    MemoryAddressIssueCycleMap[v] = Cycle;
}



unsigned
DynamicAnalysis::FindNextAvailableIssueCyclePortAndThroughtput(unsigned InstructionIssueCycle, unsigned ExtendedInstructionType, unsigned NElementsVector){
  
  unsigned ExecutionResource=ExecutionUnit[ExtendedInstructionType];
  unsigned InstructionIssueCycleThroughputAvailable = InstructionIssueCycle;
  uint64_t InstructionIssueCycleFirstTimeAvailable = 0;
  uint64_t InstructionIssueCyclePortAvailable =InstructionIssueCycle;
  uint64_t Port = 0;
  bool FoundInThroughput = false;
  bool FoundInPort = false;
  
  while (FoundInThroughput == false || FoundInPort == false) {
    DEBUG(dbgs() << "Starting while\n");
    DEBUG(dbgs() << "FoundInThroughput " << FoundInThroughput << "\n");
    DEBUG(dbgs() << "FoundInPort " << FoundInPort << "\n");
    
    // First, find next available issue cycle based on node throughput
    InstructionIssueCycleThroughputAvailable = FindNextAvailableIssueCycle(InstructionIssueCyclePortAvailable, ExecutionResource, ExtendedInstructionType, NElementsVector);
    if(InstructionIssueCycleThroughputAvailable == InstructionIssueCyclePortAvailable)
      FoundInThroughput = true;
    
    
    DEBUG(dbgs() << "Available cycle in my level " << InstructionIssueCycleThroughputAvailable << "\n");
    
    // Check that the port is available
    // Get the ports to which this node binds
    for (unsigned i = 0; i < DispatchPort[ExtendedInstructionType].size(); i++) {
      DEBUG(dbgs() << "Checking availability in port " << ResourcesNames[ExecutionPort[DispatchPort[ExtendedInstructionType][i]]] << "\n");
      InstructionIssueCyclePortAvailable = FindNextAvailableIssueCycle(InstructionIssueCycleThroughputAvailable, ExecutionPort[DispatchPort[ExtendedInstructionType][i]], PortNodes[DispatchPort[ExtendedInstructionType][i]]);
      
      if (InstructionIssueCyclePortAvailable!=InstructionIssueCycleThroughputAvailable){
        if (i==0)
          InstructionIssueCycleFirstTimeAvailable = InstructionIssueCyclePortAvailable;
        else{
          InstructionIssueCyclePortAvailable = min(InstructionIssueCyclePortAvailable, InstructionIssueCycleFirstTimeAvailable);
          if (InstructionIssueCyclePortAvailable == InstructionIssueCycleFirstTimeAvailable) {
            Port = i;
            if (InstructionIssueCyclePortAvailable == InstructionIssueCycleThroughputAvailable) {
              FoundInPort = true;
              break;
            }
          }
        }
      }else{
        // If Node is NULL, it is available for sure.
        DEBUG(dbgs() << "Node is NULL, so port is "<<DispatchPort[ExtendedInstructionType][i] <<"\n");
        FoundInPort = true;
        Port = i;
        break;
      }
    }
  }
  
  //Insert issue cycle in Port and in resource
  InsertNextAvailableIssueCycle(InstructionIssueCyclePortAvailable, ExecutionPort[DispatchPort[ExtendedInstructionType][Port]],PortNodes[DispatchPort[ExtendedInstructionType][Port]]);
  
  // Insert in resource!!!
  InsertNextAvailableIssueCycle(InstructionIssueCyclePortAvailable, ExecutionResource, ExtendedInstructionType);
  
  return InstructionIssueCyclePortAvailable;
}


// Find next available issue cycle depending on resource availability.
// Returns a pointer
unsigned
DynamicAnalysis::FindNextAvailableIssueCycle(unsigned OriginalCycle, unsigned ExecutionResource, uint64_t ExtendedInstructionType, uint8_t NElementsVector,
                                             bool TargetLevel){
  
  uint64_t NextAvailableCycle = OriginalCycle;
  Tree<uint64_t> * Node = AvailableCyclesTree[ExecutionResource];
  Tree<uint64_t> * LastNodeVisited = NULL;
  unsigned IssueCycleGranularity = 0, AccessWidth;
  float AvailableBandwidth;
  bool FoundInFullOccupancyCyclesTree = true;
  bool EnoughBandwidth = false;
  unsigned TmpTreeChunk;
  // Get the node, if any, corresponding to this issue cycle.
  unsigned TreeChunk = NextAvailableCycle/SplitTreeRange;
  if (TreeChunk >= (unsigned)FullOccupancyCyclesTree.size()) {
    for (unsigned i = FullOccupancyCyclesTree.size(); i<= TreeChunk; i++) {
      DEBUG(dbgs() << "Iserting element in FullOccupancyCyclesTree");
      FullOccupancyCyclesTree.push_back(NULL);
    }
  }
  
  // If full is null, then it is available for sure
  // If it is not NULL and there is something scheduled in this cycle..
  // (we don't include the condition FullOccupancyNode->BitVector[ExecutionResource]==1
  // here because it could happen that it cannot be executed because of throughput<1
  // and something executed in earlier or later cycles.
  if (FullOccupancyCyclesTree[TreeChunk] != NULL) {
    
    FullOccupancyCyclesTree[TreeChunk] = splay(NextAvailableCycle, FullOccupancyCyclesTree[TreeChunk]);
    
    DEBUG(dbgs() << "Full is not NULL \n");
    
    while( FoundInFullOccupancyCyclesTree == true && EnoughBandwidth ==false){
      // Check if it is in full, but firs make sure full is not NULL (it could happen it is NULL after
      // changing the NextAvailableCycle).
      if (FullOccupancyCyclesTree[TreeChunk] !=NULL) {
        if ( FullOccupancyCyclesTree[TreeChunk]->key == NextAvailableCycle && FullOccupancyCyclesTree[TreeChunk]->BitVector[ExecutionResource]==1) {
          DEBUG(dbgs() << "Cycle " << NextAvailableCycle << " found in FullOccupancyCyclesTree\n");
          FoundInFullOccupancyCyclesTree = true;
        }else{
          DEBUG(dbgs() << "Cycle " << NextAvailableCycle << " not found in FullOccupancyCyclesTree\n");
          FoundInFullOccupancyCyclesTree = false;
        }
      }else{
        // It is not in Full for sure
        FoundInFullOccupancyCyclesTree = false;
      }
      
      
      // If it is not in full, it is available. But we have to make sure that
      // there is enough bandwidth (to avoid having large trees, we don't include
      // the latency cycles, so we have to make sure we don't issue in in latency cycles)
      if (ExecutionResource <= nExecutionUnits) {
        
        DEBUG(dbgs() << "ExecutionResource <= nExecutionUnits\n");
        DEBUG(dbgs() << "ExecutionResource "<< ExecutionResource<<"\n");
        DEBUG(dbgs() << "nExecutionUnits "<< nExecutionUnits<<"\n");
        
        if (TargetLevel==true && FoundInFullOccupancyCyclesTree == false) {
          
          
          DEBUG(dbgs() << "Making sure there is also enough bandwidth...\n");
          
          AccessWidth = AccessWidths[ExecutionResource];
          AvailableBandwidth= ExecutionUnitsThroughput[ExecutionResource];
          
          IssueCycleGranularity = IssueCycleGranularities[ExecutionResource];
          
          DEBUG(dbgs() << "AccessWidth "<< AccessWidth<<"\n");
          DEBUG(dbgs() << "AvailableBandwidth "<< AvailableBandwidth<<"\n");
          DEBUG(dbgs() << "IssueCycleGranularity "<< IssueCycleGranularity<<"\n");
          
          // Assume initially that there is enough bandwidth
          EnoughBandwidth = true;
          
          //There is enough bandwidth if:
          // 1. The comp/load/store width fits within the level, or the level is empty.
          // 2. If IssueCycleGranularity > 1, we have to make sure that there were no instructions
          // executed with the same IssueCycleGranularity in previous cycles. We have to do this
          // because we don't include latency cycles in AvailableCyclesTree.
          int64_t StartingCycle = 0;
          int64_t tmp = NextAvailableCycle -IssueCycleGranularity+1;
          
          if (tmp < 0) {
            StartingCycle = 0;
          }else
            StartingCycle = NextAvailableCycle -IssueCycleGranularity+1;
          
          DEBUG(dbgs() << "StartingCycle  "<< StartingCycle<<"\n");
          DEBUG(dbgs() << "NextAvailableCycle  "<< NextAvailableCycle<<"\n");
          
          for (uint64_t i = StartingCycle; i < NextAvailableCycle; i++) {
            
            TmpTreeChunk = i/SplitTreeRange;
            if (TmpTreeChunk >= FullOccupancyCyclesTree.size()) {
              for (unsigned j = FullOccupancyCyclesTree.size(); j<= TmpTreeChunk; j++) {
                FullOccupancyCyclesTree.push_back(NULL);
              }
            }
            
            FullOccupancyCyclesTree[TmpTreeChunk] = splay(i,  FullOccupancyCyclesTree[TmpTreeChunk]);
            
            if ( FullOccupancyCyclesTree[TmpTreeChunk]!= NULL && FullOccupancyCyclesTree[TmpTreeChunk]->key == i	&& FullOccupancyCyclesTree[TmpTreeChunk]->BitVector[ExecutionResource]==1) {
              
              FoundInFullOccupancyCyclesTree = true;
              EnoughBandwidth  = false;
              //Every time NextAvailableCycle changes, we need to update TreeChunk
              TreeChunk = NextAvailableCycle/SplitTreeRange;
              if (TreeChunk >= FullOccupancyCyclesTree.size()) {
                for (unsigned j = FullOccupancyCyclesTree.size(); j<= TreeChunk; j++) {
                  DEBUG(dbgs() << "Inserting element into FullOccupancyCyclesTree\n");
                  FullOccupancyCyclesTree.push_back(NULL);
                }
              }
              DEBUG(dbgs() << "There is not enough bandwidth because of issue cycle granularity in previous cycles\n");
              break;
            }
          }
          
          // 3. The same as 2 but for next cycles. If there were loads executed on those cycles,
          // there would not be available bandwith for the current load.
          for (uint64_t i = NextAvailableCycle+1; i < NextAvailableCycle +IssueCycleGranularity; i++) {
            DEBUG(dbgs() << "Checking full occupancy in cycle "<< i<<"\n");
            TmpTreeChunk = i/SplitTreeRange;
            if (TmpTreeChunk >= FullOccupancyCyclesTree.size()) {
              for (unsigned j = FullOccupancyCyclesTree.size(); j<= TmpTreeChunk; j++) {
                FullOccupancyCyclesTree.push_back(NULL);
              }
            }
            FullOccupancyCyclesTree[TmpTreeChunk] = splay(i,  FullOccupancyCyclesTree[TmpTreeChunk]);
            
            if ( FullOccupancyCyclesTree[TmpTreeChunk]!= NULL && FullOccupancyCyclesTree[TmpTreeChunk]->key == i	&&
                FullOccupancyCyclesTree[TmpTreeChunk]->BitVector[ExecutionResource]==1) {
              
              DEBUG(dbgs() << "There is not enough bandwidth because of issue cycle granularity in later cycles\n");
              DEBUG(dbgs() << "Cycle " << i << " is in full\n");
              FoundInFullOccupancyCyclesTree = true;
              EnoughBandwidth = false;
              TreeChunk = NextAvailableCycle/SplitTreeRange;
              
              if (TreeChunk >= FullOccupancyCyclesTree.size()) {
                for (unsigned j = FullOccupancyCyclesTree.size(); j<= TreeChunk; j++) {
                  DEBUG(dbgs() << "Inserting element into FullOccupancyCyclesTree\n");
                  FullOccupancyCyclesTree.push_back(NULL);
                }
              }
              DEBUG(dbgs() << "NextAvailableCycle " << NextAvailableCycle << "\n");
              break;
            }
          }
        }else{
          EnoughBandwidth = true;
        }
        
        if (FoundInFullOccupancyCyclesTree == true || EnoughBandwidth == false) {
          //VCA-Aug next line
          NextAvailableCycle++;
          DEBUG(dbgs() << "Searching NextAvailableCycle for " << NextAvailableCycle << "\n");
          
          OriginalCycle = NextAvailableCycle;
          
          // If we loop over the first while because there is not enough bandwidth,
          // Node might be NULL because this loop has already been executed.
          Node = AvailableCyclesTree[ExecutionResource];
          
          while( Node ) {
            
            if( Node->key > NextAvailableCycle){
              if (NextAvailableCycle == OriginalCycle){ // i.e., it is the first iteration
                NextAvailableCycle = Node-> key;
                LastNodeVisited = Node;
                
              }
              // Search for a smaller one
              Node = Node->left;
            }else if( Node->key < NextAvailableCycle){
              // We comment this out because this will never happen in NextAvailable because
              // for every full node we always insert the next available. The general
              // algorithm that finds the larger, if it exist, should have this code
              // uncommented.
              //UNCOMMENT THIS!!
              /*  if (NextAvailableCycle == OriginalCycle){
               NextAvailableCycle = Node->key;
               LastNodeVisited = Node;
               }*/
              if (Node->key == OriginalCycle) {
                NextAvailableCycle = OriginalCycle;
                LastNodeVisited = Node;
                
                
                break;
              }else if (Node->key > OriginalCycle) {
                //Search for a even smaller one
                NextAvailableCycle =Node-> key;
                LastNodeVisited = Node;
                
                // Search for a smaller one
                Node = Node-> left;
              }else{ //Node->key < OriginalCycle
                // Search for a larger one, but do not store last node visited...
                Node = Node-> right;
              }
            }else{ //Node->key = NextAvailableCycle
              NextAvailableCycle = Node->key;
              LastNodeVisited = Node;
              break;
            }
          }
          
          //LastNodeVisited contains the next available cycle. But we still need to check
          //that it is available for lower and upper levels.
          NextAvailableCycle = LastNodeVisited->key;
          
          TreeChunk = NextAvailableCycle/SplitTreeRange;
          DEBUG(dbgs() << "TreeChunk = "<<TreeChunk<< "\n");
          DEBUG(dbgs() << "FullOccupancyCyclesTree.size() = "<<FullOccupancyCyclesTree.size()<< "\n");
          
          if (TreeChunk >= FullOccupancyCyclesTree.size()) {
            for (unsigned i = FullOccupancyCyclesTree.size(); i<= TreeChunk; i++) {
              DEBUG(dbgs() << "Inserting element into FullOccupancyCyclesTree\n");
              FullOccupancyCyclesTree.push_back(NULL);
            }
          }
          DEBUG(dbgs() << "NextAvailableCycle " << NextAvailableCycle << "\n");
          FoundInFullOccupancyCyclesTree = true;
          EnoughBandwidth = false;
        }
      }else{
        if (FoundInFullOccupancyCyclesTree ==true) {
          
          while (FoundInFullOccupancyCyclesTree) {
            FullOccupancyCyclesTree[TreeChunk] = splay(NextAvailableCycle, FullOccupancyCyclesTree[TreeChunk]);
            //Check if it is in full
            if ( FullOccupancyCyclesTree[TreeChunk] != NULL && FullOccupancyCyclesTree[TreeChunk]->key == NextAvailableCycle && FullOccupancyCyclesTree[TreeChunk]->BitVector[ExecutionResource]==1) {
              DEBUG(dbgs() << "Cycle " << NextAvailableCycle << " found in Full OccupancyCyclesTree\n");
              // Try next cycle
              NextAvailableCycle++;
              TreeChunk = NextAvailableCycle/SplitTreeRange;
              DEBUG(dbgs() << "TreeChunk "<< TreeChunk<<"\n");
              DEBUG(dbgs() << "FullOccupancyCyclesTree.size() "<< FullOccupancyCyclesTree.size()<<"\n");
              if (TreeChunk >= FullOccupancyCyclesTree.size()) {
                for (unsigned i = FullOccupancyCyclesTree.size(); i<= TreeChunk; i++) {
                  DEBUG(dbgs() << "Inserting element into FullOccupancyCyclesTree\n");
                  FullOccupancyCyclesTree.push_back(NULL);
                }
              }
              FoundInFullOccupancyCyclesTree = true;
            }else{
              DEBUG(dbgs() << "Cycle " << NextAvailableCycle << " not found in Full OccupancyCyclesTree\n");
              FoundInFullOccupancyCyclesTree = false;
            }
          }
        }
      }
      
    }
  }
  return NextAvailableCycle;
}



// Find next available issue cycle depending on resource availability
bool
DynamicAnalysis::InsertNextAvailableIssueCycle(uint64_t NextAvailableCycle, unsigned ExecutionResource, uint64_t ExtendedInstructionType, unsigned NElementsVector, bool isPrefetch){
  
  
  DEBUG(dbgs() << "Inserting  NextAvailableCycle " <<  NextAvailableCycle << "\n");
  
  Tree<uint64_t> * Node = AvailableCyclesTree[ExecutionResource];
  unsigned NodeIssueOccupancy = 0;
  unsigned NodeWidthOccupancy = 0;
  unsigned NodeOccupancyPrefetch = 0;
  bool LevelGotFull = false;
  
  // Update Instruction count
  if (InstructionsCountExtended[ExecutionResource]==0) {
    FirstIssue[ExecutionResource] = true;
  }
  
  if (NElementsVector > 1 && ExecutionResource!= FP_SHUFFLE){
    InstructionsCountExtended[ExecutionResource]=InstructionsCountExtended[ExecutionResource]+NElementsVector;
  }else{
    InstructionsCountExtended[ExecutionResource]++;
  }
  
  unsigned AccessWidth = AccessWidths[ExecutionResource];
  
  
  if (FirstIssue[ExecutionResource]==true) {
    FirstNonEmptyLevel[ExecutionResource] = NextAvailableCycle;
    FirstIssue[ExecutionResource] = false;
  }else{
    FirstNonEmptyLevel[ExecutionResource] = min(FirstNonEmptyLevel[ExecutionResource], NextAvailableCycle);
  }
  
  InstructionsLastIssueCycle[ExecutionResource] = max( InstructionsLastIssueCycle[ExecutionResource] ,NextAvailableCycle);
  
  DEBUG(dbgs() << "Updating InstructionsLastIssueCycle of execution resource " << ResourcesNames[ExecutionResource] << " to " <<InstructionsLastIssueCycle[ExecutionResource]  << "\n");
  
  // Insert
  // If it exists already in Available... Inserting it has any effect? No, it simply returns a pointer to the node.
  // Here, use ExtendedInstructionType and not InstructioTypeStats because both prefetched loads and normal loads
  // share the same tree
  // TODO: IF we know in advanced that the available level gets full directly, we can avoid inserting it and removing it
  // from AvailableCyclesTree.
  
  // if (ExecutionResource <= nExecutionUnits) {
  
  DEBUG(dbgs() << "Inserting next available issue cycle "<< NextAvailableCycle <<" in execution unit "<< ResourcesNames[ExecutionResource] <<" for Instruction type "<< NodesNames[ExtendedInstructionType]<<"\n");
  
  
  AvailableCyclesTree[ExecutionResource] = insert_node(NextAvailableCycle,  AvailableCyclesTree[ExecutionResource]);
  Node = AvailableCyclesTree[ExecutionResource];
  
  
  
  //TODO: clean this up
  if (ExecutionUnitsThroughput[ExecutionResource] < 1) {
    if (isPrefetch) {
      Node->occupancyPrefetch++;
    }else{
      Node->issueOccupancy++;
    }
    Node->widthOccupancy += AccessWidth;
    
  }else{
    if (AccessWidth <= ExecutionUnitsThroughput[ExecutionResource]) {
      DEBUG(dbgs() << "Increasing issue Occupancy\n");
      if (isPrefetch) {
        Node->occupancyPrefetch++;
      }else{
        Node->issueOccupancy++;
      }
      Node->widthOccupancy += AccessWidth;
    }else{
      //  if (AccessWidth > ExecutionUnitsThroughput[ExecutionResource])
      // e.g., if I access 64B and bandwidth is 8
      //TODO: Fill in, and take into account that AccessWidth might be > than troughput
      // when finding next available issue cycle.
      if (isPrefetch) {
        Node->occupancyPrefetch++;
      }else{
        Node->issueOccupancy++;
      }
      Node->widthOccupancy += AccessWidth;
    }
  }
  
  
  /* Copy these values becasue later on the Node is not the same anymore */
  NodeIssueOccupancy = Node->issueOccupancy;
  NodeWidthOccupancy = Node->widthOccupancy;
  NodeOccupancyPrefetch = Node->occupancyPrefetch;
  MaxOccupancy[ExecutionResource] = max(MaxOccupancy[ExecutionResource], NodeIssueOccupancy + NodeOccupancyPrefetch);

  
  // If ExecutionUnitsThroughput is INF, the level never gets full
  
  if (ExecutionUnitsThroughput[ExecutionResource] ==INF) {
    LevelGotFull = false;
  }else{
    if (ExecutionUnitsParallelIssue[ExecutionResource] != INF) {
      if (NodeWidthOccupancy == (unsigned)ExecutionUnitsParallelIssue[ExecutionResource]*ExecutionUnitsThroughput[ExecutionResource]) {
        LevelGotFull = true;
      }
      if (NodeIssueOccupancy == (unsigned)ExecutionUnitsParallelIssue[ExecutionResource] ) {
        LevelGotFull = true;
      }
    }else{ // If ParallelIssue is INF, but ExecutionUnitsThroughput is not INF
      if (NodeWidthOccupancy >= ExecutionUnitsThroughput[ExecutionResource]) {
        LevelGotFull = true;
      }
    }
  }
  
  if (LevelGotFull) {
    
   
    DEBUG(dbgs() << "Level got full\n");
    LevelGotFull = true;
    
    // Check whether next cycle is in full. because if it is, it should not be inserted into AvailableCyclesTree
    // Next cycle is not NexAvailableCycle+1, is NextAvailableCycle + 1/Throughput
    // Here is where the distinction betweeen execution resource and instruction type is important.
    //  unsigned NextCycle = CalculateIssueCycleGranularity(ExecutionResource, NElementsVector);
    unsigned NextCycle = IssueCycleGranularities[ExecutionResource];
    
    DEBUG(dbgs() << "AccessWidth " << AccessWidth << "\n");
    DEBUG(dbgs() << "Next Cycle " << NextCycle << "\n");
    
    AvailableCyclesTree[ExecutionResource]= delete_node(NextAvailableCycle,  AvailableCyclesTree[ExecutionResource]);
    
    // Insert node in FullOccupancy
    //    FullOccupancyCyclesTree = insert_node(NextAvailableCycle,)
    DEBUG(dbgs() << "Inserting in FullOccupancyCyclesTree of type " << ResourcesNames[ExecutionResource] << " node with key "<< NextAvailableCycle<<"\n");
    unsigned TreeChunk = NextAvailableCycle/SplitTreeRange;
    if (TreeChunk >= FullOccupancyCyclesTree.size()) {
      for (unsigned i = FullOccupancyCyclesTree.size(); i<= TreeChunk; i++) {
        DEBUG(dbgs() << "Inserting element into FullOccupancyCyclesTree\n");
        FullOccupancyCyclesTree.push_back(NULL);
      }
    }
    FullOccupancyCyclesTree[TreeChunk] =insert_node(NextAvailableCycle, ExecutionResource, FullOccupancyCyclesTree[TreeChunk]);
    
    DEBUG(dbgs() <<"FullOccupancyCyclesTree->BitVector[ExecutionResource] = " << FullOccupancyCyclesTree[TreeChunk]->BitVector[ExecutionResource] << "\n");
    
    // We don't need to do this anymore because it will be larger than LastIssueCycle - TODO
    // Do it only if NextAvailableCycle+1 == InstructionsLastIssueCycle
    // Insert if it is not in full
    DEBUG(dbgs() << "Checking if key "<< NextAvailableCycle + NextCycle <<" is in Full Occupancy Tree\n");
    TreeChunk = (NextAvailableCycle+NextCycle)/SplitTreeRange;
    if (TreeChunk >= FullOccupancyCyclesTree.size()) {
      for (unsigned i = FullOccupancyCyclesTree.size(); i<= TreeChunk; i++) {
        DEBUG(dbgs() << "Inserting element into FullOccupancyCyclesTree\n");
        FullOccupancyCyclesTree.push_back(NULL);
      }
    }
    DEBUG(dbgs() << "FullOccupancyCyclesTree.size() "<<FullOccupancyCyclesTree.size() <<"\n");
    DEBUG(dbgs() << "TreeChunk "<< TreeChunk <<"\n");
    
    // Instead of splay, we need insert_node
    FullOccupancyCyclesTree[TreeChunk] = splay(NextAvailableCycle+NextCycle/*1*/,  FullOccupancyCyclesTree[TreeChunk]);
    
    if (FullOccupancyCyclesTree[TreeChunk] == NULL)
      DEBUG(dbgs() << "FullOccupancyCyclesTree[TreeChunk] == NULL\n");
    
    if (FullOccupancyCyclesTree[TreeChunk] == NULL || (FullOccupancyCyclesTree[TreeChunk]!=NULL && !(FullOccupancyCyclesTree[TreeChunk]->key == NextAvailableCycle+NextCycle &&
                                                                                                     FullOccupancyCyclesTree[TreeChunk]->BitVector[ExecutionResource] ==1) )) {
      DEBUG(dbgs() << "The next node was not in full, so insert in available " << NextAvailableCycle+NextCycle << "\n");
      
      AvailableCyclesTree[ExecutionResource] = insert_node(NextAvailableCycle+NextCycle,  AvailableCyclesTree[ExecutionResource]);
      
      //Update LastIssueCycle -> Distinguish prefetch loads/stores!!
      InstructionsLastIssueCycle[ExecutionResource] = max( InstructionsLastIssueCycle[ExecutionResource] ,NextAvailableCycle+NextCycle);
      
      DEBUG(dbgs() << "Updating InstructionsLastIssueCycle of execution resource " << ResourcesNames[ExecutionResource] << " to " <<InstructionsLastIssueCycle[ExecutionResource]  << "\n");
      
    }else
      DEBUG(dbgs() << "key found in FullOccupancyTree\n");
  }
  //}else{
  //  FullOccupancyCyclesTree[NextAvailableCycle/SplitTreeRange] =insert_node(NextAvailableCycle, ExecutionResource, FullOccupancyCyclesTree[NextAvailableCycle/SplitTreeRange]);
  //}
  return LevelGotFull;
}



//===----------------------------------------------------------------------===//
//                Routines for Analysis of Data Reuse
// From the paper "Program Locality Analysis Using Reuse Distance", by Y. Zhong,
// X. Sheng and C. DIng, 2009
//===----------------------------------------------------------------------===//

int
DynamicAnalysis::ReuseDistance(uint64_t Last, uint64_t Current, uint64_t address,bool FromPrefetchReuseTree){
  
  int Distance = -1;
  int PrefetchReuseTreeDistance = 0;
  if(L1CacheSize != 0){ // Otherwise, does not matter the distance, it is mem access
    
    DEBUG(dbgs() << "Size of reuse tree " << node_size(ReuseTree) << "\n");
    
    int ReuseTreeDistance = ReuseTreeSearchDelete(Last, address, false);
    
    DEBUG(dbgs() << "Memory op reuse distance in ReuseTree " << ReuseTreeDistance << "\n");
    
    if (SpatialPrefetcher == true) {
      bool IsInPrefetchReuseTree = false;
      // To know whether the data item was in PrefetchReuseTree or not, we check whether the
      // element has been removed from the tree (i.e., the size of the tree).
      //int PrefetchReuseTreeSizeBefore = tree_size(PrefetchReuseTree);
      int PrefetchReuseTreeSizeBefore = PrefetchReuseTreeSize;
      PrefetchReuseTreeDistance = ReuseTreeSearchDelete(Last, address, true);
      
      DEBUG(dbgs() << "PrefetchReuseTreeDistance  "<<PrefetchReuseTreeDistance<<"\n");
      
      // int PrefetchReuseTreeSizeAfter = tree_size(PrefetchReuseTree);
      int PrefetchReuseTreeSizeAfter = PrefetchReuseTreeSize;
      
      if (PrefetchReuseTreeSizeAfter < PrefetchReuseTreeSizeBefore){
        DEBUG(dbgs() << "PREFETCHED DATA ITEM\n");
        IsInPrefetchReuseTree = true;
      }else{
        DEBUG(dbgs() << "This data item has not been prefetched\n");
      }
      if (IsInPrefetchReuseTree == false ) {
        if (ReuseTreeDistance > 0 && (uint64_t)ReuseTreeDistance >= PrefetchLevel) {
          if (PrefetchReuseTreeDistance >= 0) {
            Distance = ReuseTreeDistance + PrefetchReuseTreeDistance;
          }else
            Distance = ReuseTreeDistance; // In case PrefetchReuseTreeDistance returns -1
        }else{
          // If is not a prefetched node and ReuseTreeDistance = -1 or is
          // not a prefetched node and ReuseDistance < L2CacheSize
          Distance = ReuseTreeDistance;
        }
      }else{
        DEBUG(dbgs() << "The element is in prefetch tree\n");
        
        // If the data item is a prefetched data item (found in PrefetchReuseTree)
        Distance = PrefetchLevel;
        //   if (!(WarmCache && rep == 0))
        DEBUG(dbgs() << "Distance "<<Distance<<"\n");
        if (ReuseTreeDistance >=0) {
          Distance += ReuseTreeDistance;
          //  if (!(WarmCache && rep == 0))
          DEBUG(dbgs() << "Increasing distance to  "<<Distance<<"\n");
        }else{
          // The data item has only been prefetched. In that case, the distance is
          // the size of L2 plus the data items prefetched since the last access
        }
        if (PrefetchReuseTreeDistance >=0) {
          Distance += PrefetchReuseTreeDistance;
          DEBUG(dbgs() << "Increasing distance to  "<<Distance<<"\n");
          
        }else{
          report_fatal_error("The data item is prefetched, PrefetchReuseTreeDistance >=0, but data item does not seem to be in PrefetchReuseTree");
        }
      }
      
      // In the rest of the cases (ReuseTreeDistance > 0 && ReuseTreeDistance)
    }else
      Distance = ReuseTreeDistance;
    
#ifdef ROUND_REUSE_DISTANCE
    if(Distance >= 0)
      Distance = roundNextPowerOfTwo(Distance);
#endif
    
    // Get a pointer to the resulting tree
    if (FromPrefetchReuseTree == false) {
      ReuseTree = insert_node(Current, ReuseTree, address);
      
    }else{
      PrefetchReuseTree = insert_node(Current, PrefetchReuseTree, address);
      PrefetchReuseTreeSize++;
    }
    
#ifdef DEBUG_REUSE_DISTANCE
    DEBUG(dbgs() << "Memory op reuse distance " << Distance << "\n");
#endif
  }else{
    ReuseTree = insert_node(address,ReuseTree, address);
  }
  return Distance;
  
}



// Return the distance of the closest node with key <= Current, i.e., all the
// nodes that have been prefetched between Last and Current.

int
DynamicAnalysis::ReuseTreeSearchDelete(uint64_t Original, uint64_t address, bool FromPrefetchReuseTree){
  
  Tree<uint64_t> * Node = NULL;
  int Distance = 0;
  
  if (FromPrefetchReuseTree==false) {
    Node = ReuseTree;
  }else
    Node = PrefetchReuseTree;
  
  //Once we find it, calculate the distance without deleting the node.
  
  if (Original ==  0 || Node ==NULL) { // Did not find any node smaller
    Distance = -1;
  }else{
    while (true) {
      // This is the mechanism used in the original algorithm to delete the host
      // node,  decrementing the last_record attribute of the host node, and
      // the size attribute of all parents nodes.
      // Node->size = Node->size-1;
      if (Original < Node->key) {
        if (Node->right != NULL)
          Distance = Distance  + Node->right->size;
        if (Node->left == NULL)
          break;
        
        Distance = Distance + 1/*Node->last_record*/;
        Node = Node->left;
      }else{
        if (Original > Node-> key) {
          if (Node->right == NULL)
            break;
          Node = Node->right;
        }else{ // Last = Node->key, i.e., Node is the host node
          if (Node->right != NULL)
            Distance = Distance  + Node->right->size;
          
          //increase by one so that we can calculate directly the hit rate
          // for a cache size multiple of powers of two.
          
          Distance = Distance+1;
          
          if (Node->address == address && FromPrefetchReuseTree == false){
            ReuseTree = delete_node(Original, ReuseTree);
            
          }else{ if (Node->address == address && FromPrefetchReuseTree == true){
            PrefetchReuseTree = delete_node(Original, PrefetchReuseTree);
            PrefetchReuseTreeSize--;
            
          }
          }
          break;
        }
      }
    }
  }
  
  return Distance;
}



void
DynamicAnalysis::updateReuseDistanceDistribution(int Distance, uint64_t InstructionIssueCycle){
  
  map <int,int>::iterator ReuseDistanceMapIt;
  map<int,map<uint64_t,uint> >::iterator ReuseDistanceExtendedMapIt;
  map<uint64_t,uint>::iterator AuxMapIt;
  
#ifdef NORMAL_REUSE_DISTRIBUTION
  ReuseDistanceMapIt = ReuseDistanceDistribution.find(Distance);
  if (ReuseDistanceMapIt != ReuseDistanceDistribution.end()){
    ReuseDistanceMapIt->second = ReuseDistanceMapIt->second+1 ;
  }else
    ReuseDistanceDistribution[Distance] = 1; // First usage
#else
  ReuseDistanceExtendedMapIt = ReuseDistanceDistributionExtended.find(Distance);
  if (ReuseDistanceExtendedMapIt != ReuseDistanceDistributionExtended.end()){
    AuxMapIt = (ReuseDistanceExtendedMapIt->second).find(InstructionIssueCycle);
    if (AuxMapIt != (ReuseDistanceExtendedMapIt->second).end()){
      AuxMapIt->second = AuxMapIt->second+1 ;
    }else
      (ReuseDistanceExtendedMapIt->second)[InstructionIssueCycle] = 1;
  }else
    ReuseDistanceDistributionExtended[Distance][InstructionIssueCycle]=1; // First usage
#endif
}



// compute the next highest power of 2 of 32-bit v.
// Routine from Bit Twiddling Hacks, University of Standford.
unsigned int
DynamicAnalysis::roundNextPowerOfTwo(unsigned int v){
  
  v--;
  v |= v >> 1;
  v |= v >> 2;
  v |= v >> 4;
  v |= v >> 8;
  v |= v >> 16;
  v++;
  
  return v;
}



unsigned int
DynamicAnalysis::roundNextMultiple(uint64_t num, int factor){
  // This works because factor is always going to be a power of 2
  return (num + factor - 1) & ~(factor - 1);
}



unsigned
DynamicAnalysis::GetMemoryInstructionType(int ReuseDistance, uint64_t MemoryAddress, bool isLoad){
  
  if (ReuseDistance < 0 ){
    if(L1CacheSize == 0){
      dbgs() << "L1CacheSize == 0\n";
      if (isLoad ==true)
        return L1_LOAD_NODE;
      else
        return L1_STORE_NODE;
    }else{
      return MEM_LOAD_NODE;
      dbgs() << "L1CacheSize != 0\n";
    }
  }
  
  if (ReuseDistance <= (int)L1CacheSize){
    if (isLoad ==true)
      return L1_LOAD_NODE;
    else
      return L1_STORE_NODE;
  }
  
  if ((int)L1CacheSize < ReuseDistance && ReuseDistance <= (int)L2CacheSize)
    return L2_LOAD_NODE;
  
  if ((int)L2CacheSize < ReuseDistance && ReuseDistance <= (int)LLCCacheSize)
    return L3_LOAD_NODE;
  
  return MEM_LOAD_NODE;
  
}



unsigned
DynamicAnalysis::GetExtendedInstructionType(int OpCode, int ReuseDistance){
  
  unsigned InstructionType = 0;
  
  switch (OpCode) {
    case Instruction::FAdd:
      return FP_ADD_NODE;
      
    case Instruction::FSub:
      return FP_ADD_NODE;
      
    case  Instruction::FMul:
      return FP_MUL_NODE;
      
    case Instruction::FDiv:
      return FP_DIV_NODE;
      
    case Instruction::ShuffleVector:
      return  VECTOR_SHUFFLE_NODE;
      
    case Instruction::Load:
      
      if (ReuseDistance < 0 && L1CacheSize == 0)
        return L1_LOAD_NODE;
      
      if (ReuseDistance < 0 || (ReuseDistance > (int)LLCCacheSize && LLCCacheSize != 0))
        return MEM_LOAD_NODE;
      if (ReuseDistance <= (int)L1CacheSize)
        return L1_LOAD_NODE;
      if ((int)L1CacheSize < ReuseDistance && (ReuseDistance <= (int)L2CacheSize && L2CacheSize != 0))
        return L2_LOAD_NODE;
      if ((int)L2CacheSize < ReuseDistance && (ReuseDistance <= (int)LLCCacheSize && LLCCacheSize != 0))
        return L3_LOAD_NODE;
      report_fatal_error("Instruction type not associated with a node");
      break;
      
    case Instruction::Store:
      
      if (ReuseDistance < 0 && L1CacheSize == 0)
        return L1_STORE_NODE;
      
      if (ReuseDistance < 0 || (ReuseDistance > (int)LLCCacheSize && LLCCacheSize != 0))
        return MEM_STORE_NODE;
      if (ReuseDistance <= (int)L1CacheSize)
        return L1_STORE_NODE;
      if ((int)L1CacheSize < ReuseDistance && (ReuseDistance <= (int)L2CacheSize && L2CacheSize != 0 ))
        return L2_STORE_NODE;
      
      if ((int)L2CacheSize < ReuseDistance && (ReuseDistance <= (int)LLCCacheSize && LLCCacheSize != 0))
        return L3_STORE_NODE;
      report_fatal_error("Instruction type not associated with a node");
      break;
      
    default:
      report_fatal_error("Instruction type not associated with a node");
  }
  
  return InstructionType;
}



uint64_t
DynamicAnalysis::GetMinIssueCycleReservationStation(){
  
  vector<uint64_t>::iterator it;
  
  uint64_t MinIssueCycle = ReservationStationIssueCycles.front();
  for (it = ReservationStationIssueCycles.begin(); it != ReservationStationIssueCycles.end(); ++it)
    MinIssueCycle = min(MinIssueCycle, *it);
  
  return MinIssueCycle;
}



uint64_t
DynamicAnalysis::GetMinCompletionCycleLoadBuffer(){
  
  vector<uint64_t>::iterator it;
  
  uint64_t MinCompletionCycle = LoadBufferCompletionCycles.front();
  for (it = LoadBufferCompletionCycles.begin(); it != LoadBufferCompletionCycles.end(); ++it) {
    MinCompletionCycle = min(MinCompletionCycle, *it);
  }
  return MinCompletionCycle;
}


uint64_t
DynamicAnalysis::GetMinCompletionCycleLoadBufferTree(){
  
  return MinLoadBuffer;
  //return min(LoadBufferCompletionCyclesTree);
}




uint64_t
DynamicAnalysis::GetMinCompletionCycleStoreBuffer(){
  
  vector<uint64_t>::iterator it;
  
  uint64_t MinCompletionCycle = StoreBufferCompletionCycles.front();
  for (it = StoreBufferCompletionCycles.begin(); it != StoreBufferCompletionCycles.end(); ++it) {
    MinCompletionCycle = min(MinCompletionCycle, *it);
  }
  return MinCompletionCycle;
}



uint64_t
DynamicAnalysis::GetMinCompletionCycleLineFillBuffer(){
  
  vector<uint64_t>::iterator it;
  
  uint64_t MinCompletionCycle = LineFillBufferCompletionCycles.front();
  for (it = LineFillBufferCompletionCycles.begin(); it != LineFillBufferCompletionCycles.end(); ++it) {
    MinCompletionCycle = min(MinCompletionCycle, *it);
  }
  return MinCompletionCycle;
}



bool
DynamicAnalysis::isStallCycle(int ResourceType, uint64_t Level){
  
  StallCycles[ResourceType] = splay(Level,  StallCycles[ResourceType]);
  if (StallCycles[ResourceType] == NULL) {
    return false;
  }else{
    return true;
  }
}



bool
DynamicAnalysis::IsEmptyLevel(unsigned ExecutionResource, uint64_t Level, bool WithPrefetch){
  
  bool IsInAvailableCyclesTree = false;
  bool IsInFullOccupancyCyclesTree = false;
  
  if (ExecutionResource <= nExecutionUnits) {
    if (AvailableCyclesTree[ExecutionResource]!= NULL) {
      AvailableCyclesTree[ExecutionResource] = splay(Level, AvailableCyclesTree[ExecutionResource] );
      if (Level == AvailableCyclesTree[ExecutionResource]->key){
        if( AvailableCyclesTree[ExecutionResource]->issueOccupancy != 0 || (WithPrefetch && AvailableCyclesTree[ExecutionResource]->occupancyPrefetch != 0)){
          IsInAvailableCyclesTree = true;
        }
      }
    }
  }else
    IsInAvailableCyclesTree = false;
  
  int TreeChunk = Level/SplitTreeRange;
  if (FullOccupancyCyclesTree[TreeChunk]!= NULL) {
    FullOccupancyCyclesTree[TreeChunk] = splay(Level, FullOccupancyCyclesTree[TreeChunk]);
    if (Level == FullOccupancyCyclesTree[TreeChunk]->key &&
        FullOccupancyCyclesTree[TreeChunk]->BitVector[ExecutionResource]==1 ){
      IsInFullOccupancyCyclesTree = true;
    }
  }
  
  if (IsInFullOccupancyCyclesTree == false && IsInAvailableCyclesTree == false) {
    return true;
  }else
    return false;
}


// This is not yet working...

uint64_t
DynamicAnalysis::FindNextNonEmptyLevel(unsigned ExecutionResource, uint64_t Level){
  
  bool IsInAvailableCyclesTree = true;
  bool IsInFullOccupancyCyclesTree = true;
  uint64_t Original = 0;
  uint64_t Closest = 0;
  uint64_t NextNonEmptyLevelAvailableCyclesTree = Level;
  uint64_t NextNonEmptyLevelFullOccupancyCyclesTree = Level;
  
  
  if (ExecutionResource <= nExecutionUnits) {
    
    Tree<uint64_t> * Node = NULL;
    Tree<uint64_t> * LastNodeVisited = NULL;
    
    Node = AvailableCyclesTree[ExecutionResource];
    Original = Level+1;
    Closest = Original;
    
    while (IsInAvailableCyclesTree == true) {
      
      
      while( true ) {
        
        if( Node->key > Closest){
          
          if (Closest == Original){ // i.e., it is the first iteration
            Closest = Node-> key;
            LastNodeVisited = Node;
          }
          // Search for a smaller one
          //Node = Node-> left;
          if (Node->left==NULL) {
            //If, moreover, Node->right ==NULL, then break
            if (Node->right==NULL) {
              Closest = Node->key;
              LastNodeVisited = Node;
              
              IsInAvailableCyclesTree = false;
              break;
            }
            
            // The while loop is going to finish.
            // Again, we have to make sure
            if( !(Node->issueOccupancy != 0 ||  Node->occupancyPrefetch != 0)){
              // We don't want the loop to finish.
              // Splay on key and search for the next one.
              AvailableCyclesTree[ExecutionResource]= splay(Node->key,AvailableCyclesTree[ExecutionResource]);
              Node =  AvailableCyclesTree[ExecutionResource];
              //Keep searching starting from the next one
              Original = Node->key+1;
              Closest = Original;
            }
          }else{
            Node = Node-> left;
          }
        }else if( Node->key < Closest){
          if (Node->key == Original) {
            // MODIFICATION with respect to the normal search in the tree.
            if( Node->issueOccupancy != 0 ||  Node->occupancyPrefetch != 0){
              dbgs() << "Is in available cycles for this resource and has nodes scheduled\n";
              Closest = Node->key;
              LastNodeVisited = Node;
              IsInAvailableCyclesTree = false;
              break;
              
            }else{
              dbgs() << "Is in available, but not for this resource, so keep searching\n";
              dbgs() << "Splay tree to "<< Node->key<<"\n";
              AvailableCyclesTree[ExecutionResource]= splay(Node->key,AvailableCyclesTree[ExecutionResource]);
              Node =  AvailableCyclesTree[ExecutionResource];
              dbgs() << "Root of the tree "<< Node->key<<"\n";
              //Keep searching starting from the next one
              Original = Node->key+1;
              Closest = Original;
              
            }
          }else if (Node->key > Original) {
            dbgs() << "Node->key > Original\n";
            Closest =Node-> key;
            LastNodeVisited = Node;
            //Search for a even smaller one
            
            // Node = Node-> left;
            if (Node->left==NULL) {
              dbgs() << "Node->left is NULL\n";
              //If, moreover, Node->right ==NULL, then break
              if (Node->right==NULL) {
                dbgs() << "Node->right is NULL\n";
                Closest = Node->key;
                LastNodeVisited = Node;
                
                IsInAvailableCyclesTree = false;
                break;
              }
              
              // The while loop is going to finish.
              // Again, we have to make sure
              if( !(Node->issueOccupancy != 0 ||  Node->occupancyPrefetch != 0)){
                // We don't want the loop to finish.
                // Splay on key and search for the next one.
                AvailableCyclesTree[ExecutionResource]= splay(Node->key,AvailableCyclesTree[ExecutionResource]);
                Node =  AvailableCyclesTree[ExecutionResource];
                //Keep searching starting from the next one
                Original = Node->key+1;
                Closest = Original;
              }
            }else{
              Node = Node-> left;
            }
            
          }else{ //Node->key < Original
            dbgs() << "Node->key < Original\n";
            
            // Search for a larger one
            
            Node = Node->right;
            if (Node->right==NULL) {
              IsInAvailableCyclesTree = false;
            }
            
          }
        }else{ //Node->key = Closest
          dbgs() << "Node->key = Closest\n";
          // MODIFICATION with respect to the normal search in the tree.
          if( Node->issueOccupancy != 0 ||  Node->occupancyPrefetch != 0){
            dbgs() << "Is in full for this resource\n";
            Closest = Node->key;
            LastNodeVisited = Node;
            IsInAvailableCyclesTree = false;
            
            break;
            
          }else{
            dbgs() << "Is in full, but not for this resource, so keep searching\n";
            AvailableCyclesTree[ExecutionResource] = splay(Node->key,AvailableCyclesTree[ExecutionResource]);
            Node =  AvailableCyclesTree[ExecutionResource];
            dbgs() << "Root of the tree "<< Node->key<<"\n";
            
            Original = Node->key+1;
            Closest = Original;
            
          }
          
        }
      }
      
      
      
      dbgs() << "NodeFull->key " << LastNodeVisited->key<<"\n";
      // Current = LastNodeVisited->key;
    }
    
    if (IsInAvailableCyclesTree == false) { // i.e., there is no non-empty level
      NextNonEmptyLevelAvailableCyclesTree = Level;
    }
    
  }
  
  
  dbgs() << "NextNonEmptyLevelAvailableCyclesTree " << NextNonEmptyLevelAvailableCyclesTree<<"\n";
  
  
  
  TreeBitVector<uint64_t> * Node = NULL;
  TreeBitVector<uint64_t> * LastNodeVisited = NULL;
  
  // Repeat a similar process with FullOccupancyTree
  
  int TreeChunk = Level/SplitTreeRange;
  
  
  Original = Level+1;
  Closest = Original;
  TreeChunk = Original/SplitTreeRange;
  Node = FullOccupancyCyclesTree[TreeChunk];
  
  while (IsInFullOccupancyCyclesTree == true) {
    dbgs() << "Starting the search for level " << Original << "\n";
    
    
    while( true ) {
      dbgs() << "Node->key " << Node->key << "\n";
      dbgs() << "Closest " << Closest << "\n";
      dbgs() << "Original " << Original << "\n";
      
      if( Node->key > Closest){
        dbgs() << " Node->key > Closest\n";
        
        if (Closest == Original){ // i.e., it is the first iteration
          Closest = Node-> key;
          LastNodeVisited = Node;
        }
        // Search for a smaller one
        // Node = Node-> left;
        if (Node->left==NULL) {
          dbgs() << "Node->left is NULL\n";
          //If, moreover, Node->right ==NULL, then break
          if (Node->right==NULL) {
            dbgs() << "Node->right is NULL\n";
            Closest = Node->key;
            LastNodeVisited = Node;
            
            IsInFullOccupancyCyclesTree = false;
            break;
          }
          
          // The while loop is going to finish.
          // Again, we have to make sure
          if( !(Node->BitVector[ExecutionResource]==1)){
            // We don't want the loop to finish.
            // Splay on key and search for the next one.
            FullOccupancyCyclesTree[TreeChunk] = splay(Node->key,FullOccupancyCyclesTree[TreeChunk]);
            Node =  FullOccupancyCyclesTree[TreeChunk];
            //Keep searching starting from the next one
            Original = Node->key+1;
            Closest = Original;
          }
        }else{
          Node = Node-> left;
        }
        
      }else if( Node->key < Closest){
        dbgs() << " Node->key < Closest\n";
        if (Node->key == Original) {
          dbgs() << " Node->key == Original\n";
          // MODIFICATION with respect to the normal search in the tree.
          if( Node->BitVector[ExecutionResource]==1){
            dbgs() << "Is in full for this resource\n";
            Closest = Node->key;
            LastNodeVisited = Node;
            IsInFullOccupancyCyclesTree = false;
            break;
            
          }else{
            dbgs() << "Is in full, but not for this resource, so keep searching\n";
            dbgs() << "Splay tree to "<< Node->key<<"\n";
            FullOccupancyCyclesTree[TreeChunk] = splay(Node->key,FullOccupancyCyclesTree[TreeChunk]);
            Node =  FullOccupancyCyclesTree[TreeChunk];
            dbgs() << "Root of the tree "<< Node->key<<"\n";
            Original = Node->key+1;
            Closest = Original;
            
          }
        }else if (Node->key > Original) {
          dbgs() << "Node->key > Original\n";
          Closest =Node-> key;
          LastNodeVisited = Node;
          //Search for a even smaller one
          
          // Node = Node-> left;
          if (Node->left==NULL) {
            dbgs() << "Node->left is NULL\n";
            //If, moreover, Node->right ==NULL, then break
            if (Node->right==NULL) {
              dbgs() << "Node->right is NULL\n";
              Closest = Node->key;
              LastNodeVisited = Node;
              
              IsInFullOccupancyCyclesTree = false;
              break;
            }
            
            // The while loop is going to finish.
            // Again, we have to make sure
            if( !(Node->BitVector[ExecutionResource]==1)){
              // We don't want the loop to finish.
              // Splay on key and search for the next one.
              FullOccupancyCyclesTree[TreeChunk] = splay(Node->key,FullOccupancyCyclesTree[TreeChunk]);
              Node =  FullOccupancyCyclesTree[TreeChunk];
              //Keep searching starting from the next one
              Original = Node->key+1;
              Closest = Original;
            }
          }else{
            Node = Node-> left;
          }
          
        }else{ //Node->key < Original
          dbgs() << "Node->key < Original\n";
          
          // Search for a larger one
          
          Node = Node->right;
          if (Node == NULL) {
            IsInFullOccupancyCyclesTree = false;
          }
        }
      }else{ //Node->key = Closest
        dbgs() << "Node->key = Closest\n";
        // MODIFICATION with respect to the normal search in the tree.
        if( Node->BitVector[ExecutionResource]==1){
          DEBUG(dbgs() << "Is in full for this resource\n");
          Closest = Node->key;
          LastNodeVisited = Node;
          IsInFullOccupancyCyclesTree = false;
          
          break;
          
        }else{
          DEBUG(dbgs() << "Is in full, but not for this resource, so keep searching\n");
          FullOccupancyCyclesTree[TreeChunk] = splay(Node->key,FullOccupancyCyclesTree[TreeChunk]);
          Node =  FullOccupancyCyclesTree[TreeChunk];
          Original = Node->key+1;
          Closest = Original;
          
        }
        
      }
    }
    DEBUG(dbgs() << "NodeFull->key " << LastNodeVisited->key<<"\n");
    // Current = LastNodeVisited->key;
    
    
  }
  
  if (IsInFullOccupancyCyclesTree == false) { // i.e., there are no non-empty level
    NextNonEmptyLevelFullOccupancyCyclesTree = LastNodeVisited->key;
  }
  
  
  dbgs() << "Next non empty level " << min(NextNonEmptyLevelAvailableCyclesTree, NextNonEmptyLevelFullOccupancyCyclesTree) << "\n";
  if (NextNonEmptyLevelAvailableCyclesTree == Level) {
    return NextNonEmptyLevelFullOccupancyCyclesTree;
  }else{
    if (NextNonEmptyLevelFullOccupancyCyclesTree == Level) {
      return NextNonEmptyLevelAvailableCyclesTree;
    }else{
      //None of them are euqal to Level
      return min(NextNonEmptyLevelAvailableCyclesTree, NextNonEmptyLevelFullOccupancyCyclesTree);
      
    }
  }
  
  
}



/* An alternative to optimize calculateSpan could be merging the
 AvailableCyclesTree and FullOccupancyCyclesTree and doing and
 ca inorder/ postoder travesal */
uint64_t
DynamicAnalysis::CalculateSpan(int ResourceType){
  
  uint64_t Span = 0;
  
  //If there are instructions of this type....
  if (InstructionsCountExtended[ResourceType]>0) {
    
    uint64_t Latency = ExecutionUnitsLatency[ResourceType];
    uint64_t First= FirstNonEmptyLevel[ResourceType];
    uint64_t DominantLevel = First;
    uint64_t LastCycle = LastIssueCycleVector[ResourceType];
    
#ifdef DEBUG_SPAN_CALCULATION
    DEBUG(dbgs() << "First  " << First << "\n");
    DEBUG(dbgs() << "Latency  " << Latency << "\n");
    DEBUG(dbgs() << "LastCycle  " << LastCycle << "\n");
    DEBUG(dbgs() << "Increasing span to  " << Latency << "\n");
#endif
    
    Span+= Latency;
    
    //Start from next level to first non-emtpy level
    //  for(unsigned i=First+IssueCycleGranularity; i <= LastCycle; i+=IssueCycleGranularity){
    for(unsigned i=First+1; i <= LastCycle; i+=1){
      //Check whether there is instruction scheduled in this cycle!
      if (IsEmptyLevel( ResourceType,i)==false) {
        if ( i <= DominantLevel+Latency-1){
          if (i+Latency > DominantLevel+Latency && Latency!=0) {
#ifdef DEBUG_SPAN_CALCULATION
            DEBUG(dbgs() << "Increasing Span by the difference " << ((i+Latency)-max((DominantLevel+Latency),(uint64_t)1)) << "\n");
#endif
            Span+=((i+Latency)-max((DominantLevel+Latency),(uint64_t)1));
            DominantLevel = i;
          }
        }else{
#ifdef DEBUG_SPAN_CALCULATION
          DEBUG(dbgs() << "Increasing Span by " << Latency << "\n");
#endif
          Span+=Latency;
          DominantLevel = i;
        }
      }
    }
  }
  return Span;
}


uint64_t
DynamicAnalysis::GetLastIssueCycle(unsigned ExecutionResource, bool WithPrefetch){
  
  Tree<uint64_t> * NodeAvailable = NULL;
  TreeBitVector<uint64_t> * NodeFull = NULL;
  bool isPrefetchType = false;
  unsigned  IssueCycleGranularity = IssueCycleGranularities[ExecutionResource];
  uint64_t LastCycle = InstructionsLastIssueCycle[ExecutionResource];
  
  DEBUG(dbgs() << "Last cycle in InstructionLastIssueCycle for resource " <<
        ResourcesNames[ExecutionResource]<< ": " << LastCycle << "\n");
  
  if(ExecutionResource <= nExecutionUnits){
    
    AvailableCyclesTree[ExecutionResource] = splay(LastCycle,AvailableCyclesTree[ExecutionResource]);
    NodeAvailable =AvailableCyclesTree[ExecutionResource];
    
    int TreeChunk = LastCycle/SplitTreeRange;
    FullOccupancyCyclesTree[TreeChunk] = splay(LastCycle,FullOccupancyCyclesTree[TreeChunk]);
    NodeFull = FullOccupancyCyclesTree[TreeChunk];
    if (isPrefetchType) {
      if (( NodeAvailable != NULL && NodeAvailable->key== LastCycle && NodeAvailable->occupancyPrefetch == 0 ) ||
          ( NodeFull != NULL && NodeFull->key== LastCycle && NodeFull->BitVector[ExecutionResource]==0 ) ) {
        DEBUG(dbgs() << "Reducing last cycle " << LastCycle << " by issuegranularity\n");
        LastCycle = LastCycle-/*1*/IssueCycleGranularity;
        DEBUG(dbgs() << "LastCycle " << LastCycle << "\n");
      }
    }else{
      if ( NodeAvailable != NULL && NodeAvailable->key== LastCycle && NodeAvailable->issueOccupancy == 0 ) {
        DEBUG(dbgs() << "Reducing last cycle " << LastCycle << " by issuegranularity\n");
        LastCycle = LastCycle-/*1*/IssueCycleGranularity;
        DEBUG(dbgs() << "LastCycle " << LastCycle << "\n");
      }
    }
  }
  return LastCycle;
}



unsigned
DynamicAnalysis::CalculateGroupSpan(vector<int> & ResourcesVector, bool WithPrefetch, bool ForceUnitLatency){
  
  unsigned Span = 0;
  unsigned MaxLatency = 0;
  uint64_t First = 0;
  bool EmptyLevel = true;
  bool IsGap = false;
  int NResources = ResourcesVector.size();
  uint64_t LastCycle = 0;
  uint64_t ResourceLastCycle = 0;
  unsigned MaxLatencyLevel = 0;
  unsigned ResourceType = 0;
  unsigned AccessWidth = 0;
  
  // vector<uint64_t> NextNonEmptyLevelVector;
  // uint64_t NextNonEmptyLevel;
  
#ifdef DEBUG_SPAN_CALCULATION
  DEBUG(dbgs() << "Resources that contribute to Span:\n");
  for (int j= 0; j< NResources; j++) {
    DEBUG(dbgs() << ResourcesVector[j] << "\n");
  }
#endif
  
  //Determine first non-empty level and LastCycle
  for (int j= 0; j< NResources; j++) {
    
    ResourceType = ResourcesVector[j];
    
    if (InstructionsCountExtended[ResourceType]>0) {
      DEBUG(dbgs() << "There are instructions of type "<< ResourceType<<"\n");
      AccessWidth = AccessWidths[ResourceType];
      
      if (EmptyLevel == true) { // This will be only executed the first time of a non-empty level
        EmptyLevel = false;
        First = FirstNonEmptyLevel[ResourceType];
        
        if (ExecutionUnitsThroughput[ResourceType] == INF) {
          MaxLatency = ExecutionUnitsLatency[ResourceType];
        }else
          MaxLatency = max(ExecutionUnitsLatency[ResourceType],(unsigned)ceil(AccessWidth/ExecutionUnitsThroughput[ResourceType]));
      }else{
        if (First == FirstNonEmptyLevel[ResourceType]){
          if (ExecutionUnitsThroughput[ResourceType] == INF) {
            MaxLatency = max(MaxLatency,ExecutionUnitsLatency[ResourceType]);
          }else
            MaxLatency = max(MaxLatency,max(ExecutionUnitsLatency[ResourceType],(unsigned)ceil(AccessWidth/ExecutionUnitsThroughput[ResourceType])));
        }
        else{
          First = min(First,FirstNonEmptyLevel[ResourceType]);{
            if (First == FirstNonEmptyLevel[ResourceType]){
              if (ExecutionUnitsThroughput[ResourceType] == INF) {
                MaxLatency =ExecutionUnitsLatency[ResourceType];
              }else
                MaxLatency = max(ExecutionUnitsLatency[ResourceType],(unsigned)ceil(AccessWidth/ExecutionUnitsThroughput[ResourceType]));
            }
          }
        }
      }
      if (ForceUnitLatency == true)
        MaxLatency = 1;
      
      ResourceLastCycle = LastIssueCycleVector[ResourceType];
      // NextNonEmptyLevelVector.push_back(FirstNonEmptyLevel[ResourceType]);
      
#ifdef DEBUG_SPAN_CALCULATION
      DEBUG(dbgs() << "Calling GetLastIssueCycle with args "<< ResourceType << "  " << WithPrefetch<<"\n");
      DEBUG(dbgs() << "Last cycle returned from ResourceLastCycle "<< ResourceLastCycle<<"\n");
#endif
      LastCycle = max(LastCycle, ResourceLastCycle);
    }
  }
  #ifdef DEBUG_SPAN_CALCULATION
  DEBUG(dbgs() << "First non-empty level  " << First << "\n");
  DEBUG(dbgs() << "MaxLatency  " << MaxLatency << "\n");
  DEBUG(dbgs() << "LastCycle  " << LastCycle << "\n");
#endif
  unsigned DominantLevel = First;
  
  if (EmptyLevel == false) {
    Span+= MaxLatency;
    
    
    for(unsigned i=First+1; i<= LastCycle; i++){
      // For sure there is at least resource for which this level is not empty.
#ifdef DEBUG_SPAN_CALCULATION
      DEBUG(dbgs() << "i =   " << i << "\n");
#endif
      //Determine MaxLatency of Level
      MaxLatencyLevel = 0;
      for(int j=0; j< NResources; j++){
        ResourceType = ResourcesVector[j];
        
        if (i <= LastIssueCycleVector[ResourceType]/*GetLastIssueCycle(ResourceType, WithPrefetch)*/ ) {
          if (IsEmptyLevel(ResourceType, i, WithPrefetch) == false) {
            // dbgs() << "Level non empty\n";
            IsGap = false;
            // MaxLatencyLevel = max(MaxLatencyLevel, GetInstructionLatency(ResourcesVector[j]));
            if (ForceUnitLatency ==true) {
              MaxLatencyLevel = 1;
            }else{
              AccessWidth =AccessWidths[ResourceType];
             
              if (ExecutionUnitsThroughput[ResourceType] == INF) {
                MaxLatencyLevel = max(MaxLatencyLevel, ExecutionUnitsLatency[ResourceType]);
                
                
              }else{
                MaxLatencyLevel = max(MaxLatencyLevel, max(ExecutionUnitsLatency[ResourceType],(unsigned)ceil(AccessWidth/ExecutionUnitsThroughput[ResourceType])));
              }
            }
                      }
        }
      }
      
#ifdef DEBUG_SPAN_CALCULATION
      DEBUG(dbgs() << "MaxLatencyLevel  " << MaxLatencyLevel << "\n");
#endif
      
      //That is, only if there are instructions scheduled in this cycle
      if(MaxLatencyLevel !=0){
        if ( i <= DominantLevel+MaxLatency-1){
          
          if (i+MaxLatencyLevel > DominantLevel+MaxLatency && MaxLatencyLevel!=0) {
#ifdef DEBUG_SPAN_CALCULATION
            DEBUG(dbgs() << "Increasing Span by the difference " << ((i+MaxLatencyLevel)-max((DominantLevel+MaxLatency),(unsigned)1)) << "\n");
#endif
            
            Span+=((i+MaxLatencyLevel)-max((DominantLevel+MaxLatency),(unsigned)1));
            DominantLevel = i;
            MaxLatency = MaxLatencyLevel;
          }
        }else{
#ifdef DEBUG_SPAN_CALCULATION
          DEBUG(dbgs() << "Increasing Span by " << MaxLatencyLevel << "\n");
#endif
          
          Span+=MaxLatencyLevel;
          DominantLevel = i;
          MaxLatency = MaxLatencyLevel;
        }
      }else{
        if ( i > DominantLevel+MaxLatency-1){
          if (NResources==1 && IsGap == false) {
            SpanGaps[ResourceType]++;
            DEBUG(dbgs() << "Increasing span gaps for resource " << ResourcesNames[ResourceType] << "\n");
            IsGap = true;
          }
        }
      }
      
        }
  }
  
  DEBUG(dbgs() << "CalculateGroupSpan returns Span =  " << Span<< "\n");
  
  return Span;
}



unsigned
DynamicAnalysis::CalculateIssueSpan(vector<int> & ResourcesVector){
  
  unsigned Span = 0;
  unsigned MaxLatency = 0;
  uint64_t First = 0;
  bool EmptyLevel = true;
  int NResources = ResourcesVector.size();
  uint64_t LastCycle = 0;
  uint64_t ResourceLastCycle = 0;
  unsigned MaxLatencyLevel = 0;
  unsigned ResourceType = 0;
  unsigned AccessWidth = 0;
  unsigned TmpLatency = 0;
  
#ifdef DEBUG_SPAN_CALCULATION
  DEBUG(dbgs() << "Resources that contribute to Span:\n");
  for (int j= 0; j< NResources; j++) {
    DEBUG(dbgs() << ResourcesVector[j] << "\n");
  }
#endif
  
  //Determine first non-empty level and LastCycle
  for (int j= 0; j< NResources; j++) {
    
    ResourceType = ResourcesVector[j];
    
    if (InstructionsCountExtended[ResourceType]>0) {
      
      AccessWidth = AccessWidths[ResourceType];
      if (ExecutionUnitsThroughput[ResourceType]==INF)
        TmpLatency = 1;
      else
        TmpLatency = ceil(AccessWidth/ExecutionUnitsThroughput[ResourceType]);
      
      DEBUG(dbgs() << "AccessWidth "<< AccessWidth<<"\n");
      DEBUG(dbgs() << "There are instructions of type "<< ResourceType<<"\n");
      
      if (EmptyLevel == true) { // This will be only executed the first time of a non-empty level
        EmptyLevel = false;
        First = FirstNonEmptyLevel[ResourceType];
        //    MaxLatency = ceil(AccessWidth/ExecutionUnitsThroughput[ResourceType]);
        MaxLatency = TmpLatency;
      }else{
        if (First == FirstNonEmptyLevel[ResourceType])
          // MaxLatency = max(MaxLatency,(unsigned)ceil(AccessWidth/ExecutionUnitsThroughput[ResourceType]));
          MaxLatency = max(MaxLatency,TmpLatency);
        else{
          First = min(First,FirstNonEmptyLevel[ResourceType]);{
            if (First == FirstNonEmptyLevel[ResourceType])
              //MaxLatency = ceil(AccessWidth/ExecutionUnitsThroughput[ResourceType]);
              MaxLatency = TmpLatency;
          }
        }
      }
      
      DEBUG(dbgs() << "ResourceType "<< ResourceType<<"\n");
      DEBUG(dbgs() << "LastIssueCycleVector size "<< LastIssueCycleVector.size()<<"\n");
      DEBUG(dbgs() << "LastIssueCycleVector[ResourceType] "<< LastIssueCycleVector[ResourceType]<<"\n");
      
      ResourceLastCycle = LastIssueCycleVector[ResourceType];
      
#ifdef DEBUG_SPAN_CALCULATION
      DEBUG(dbgs() << "Last cycle returned from ResourceLastCycle "<< ResourceLastCycle<<"\n");
#endif
      LastCycle = max(LastCycle, ResourceLastCycle);
    }
  }
  
  
#ifdef DEBUG_SPAN_CALCULATION
  DEBUG(dbgs() << "First non-empty level  " << First << "\n");
  DEBUG(dbgs() << "MaxLatency  " << MaxLatency << "\n");
  DEBUG(dbgs() << "LastCycle  " << LastCycle << "\n");
#endif
  unsigned DominantLevel = First;
  //  if (First >= 0) {
  if (EmptyLevel == false) {
    Span+= MaxLatency;
    
    //Start from next level to first non-emtpy level
    for(unsigned i=First+1; i<= LastCycle; i++){
      //Determine MaxLatency of Level
      MaxLatencyLevel = 0;
      for(int j=0; j< NResources; j++){
        ResourceType = ResourcesVector[j];
        
        if (i <= LastIssueCycleVector[ResourceType]/*GetLastIssueCycle(ResourceType, 0)*/) {
          if (IsEmptyLevel(ResourceType, i, false) == false) {
            //   dbgs() << "Level "<< i<<" got full2\n";
            AccessWidth = AccessWidths[ResourceType];
            if (ExecutionUnitsThroughput[ResourceType]==INF)
              TmpLatency = 1;
            else
              TmpLatency = ceil(AccessWidth/ExecutionUnitsThroughput[ResourceType]);
            MaxLatencyLevel = max(MaxLatencyLevel, TmpLatency);
          }
        }
      }
      
#ifdef DEBUG_SPAN_CALCULATION
      DEBUG(dbgs() << "i =   " << i << "\n");
      DEBUG(dbgs() << "MaxLatencyLevel  " << MaxLatencyLevel << "\n");
#endif
      
      //That is, only if there are instructions scheduled in this cycle
      if(MaxLatencyLevel !=0){
        //   dbgs() << "MaxLatencyLevel !=0\n";
        if ( i <= DominantLevel+MaxLatency-1){
          //      dbgs() << "This should never be executed. i = "<<i<<", DominantLevel = "<< DominantLevel<<", MaxLatency "<<MaxLatency<<", DominantLevel+MaxLatency-1 = "<<DominantLevel+MaxLatency-1<<"\n";
          
          if (i+MaxLatencyLevel > DominantLevel+MaxLatency && MaxLatencyLevel!=0) {
#ifdef DEBUG_SPAN_CALCULATION
            DEBUG(dbgs() << "Increasing Span by the difference " << ((i+MaxLatencyLevel)-max((DominantLevel+MaxLatency),(unsigned)1)) << "\n");
#endif
            
            Span+=((i+MaxLatencyLevel)-max((DominantLevel+MaxLatency),(unsigned)1));
            DominantLevel = i;
            MaxLatency = MaxLatencyLevel;
          }
        }else{
          
          if (ResourceType == L2_LOAD_CHANNEL) {
            //    dbgs() << "Vicky. i = "<<i<<", DominantLevel = "<< DominantLevel<<", MaxLatency "<<MaxLatency<<", DominantLevel+MaxLatency-1 = "<<DominantLevel+MaxLatency-1<<"\n";
            //    dbgs() << "Increasing Span by " << MaxLatencyLevel << "\n";
          }
#ifdef DEBUG_SPAN_CALCULATION
          DEBUG(dbgs() << "Increasing Span by " << MaxLatencyLevel << "\n");
#endif
          
          Span+=MaxLatencyLevel;
          DominantLevel = i;
          MaxLatency = MaxLatencyLevel;
        }
      }
    }
  }
  
  DEBUG(dbgs() << "Span = " << Span << "\n");
  return Span;
}



void
DynamicAnalysis::RemoveFromReservationStation(uint64_t Cycle){
  
  LessThanOrEqualValuePred Predicate = {Cycle};
  ReservationStationIssueCycles.erase(std::remove_if(ReservationStationIssueCycles.begin(), ReservationStationIssueCycles.end(), Predicate), ReservationStationIssueCycles.end());
}



void
DynamicAnalysis::RemoveFromReorderBuffer(uint64_t Cycle){
  
  while (!ReorderBufferCompletionCycles.empty() && ReorderBufferCompletionCycles.front() <= Cycle)
    ReorderBufferCompletionCycles.pop_front();
}



void
DynamicAnalysis::RemoveFromLoadBuffer(uint64_t Cycle){
  
  LessThanOrEqualValuePred Predicate = {Cycle};
  LoadBufferCompletionCycles.erase(std::remove_if(LoadBufferCompletionCycles.begin(), LoadBufferCompletionCycles.end(), Predicate), LoadBufferCompletionCycles.end());
}

void
DynamicAnalysis::RemoveFromLoadBufferTree(uint64_t Cycle){
  
  
  if (LoadBufferCompletionCyclesTree != NULL) {
    
    LoadBufferCompletionCyclesTree = splay(Cycle, LoadBufferCompletionCyclesTree);
    
    if (LoadBufferCompletionCyclesTree->key == Cycle) { // If Cycle found
      LoadBufferCompletionCyclesTree->left = NULL;
      LoadBufferCompletionCyclesTree = delete_node(Cycle,LoadBufferCompletionCyclesTree);
      // If we remove the minimum, the resulting tree has as node the
      // successor of the minimum, which is the next minimum
      if (Cycle == MinLoadBuffer && LoadBufferCompletionCyclesTree != NULL) {
        MinLoadBuffer = LoadBufferCompletionCyclesTree->key;
      }
    }
    
    
  }
  
  
  
}




void
DynamicAnalysis::RemoveFromStoreBuffer(uint64_t Cycle){
  
  LessThanOrEqualValuePred Predicate = {Cycle};
  StoreBufferCompletionCycles.erase(std::remove_if(StoreBufferCompletionCycles.begin(), StoreBufferCompletionCycles.end(), Predicate), StoreBufferCompletionCycles.end());
}



void
DynamicAnalysis::RemoveFromLineFillBuffer(uint64_t Cycle){
  
  LessThanOrEqualValuePred Predicate = {Cycle};
  LineFillBufferCompletionCycles.erase(std::remove_if(LineFillBufferCompletionCycles.begin(), LineFillBufferCompletionCycles.end(), Predicate), LineFillBufferCompletionCycles.end());
}



void
DynamicAnalysis::RemoveFromDispatchToLoadBufferQueue(uint64_t Cycle){
  
  StructMemberLessThanOrEqualThanValuePred Predicate = {Cycle};
  DispatchToLoadBufferQueue.erase(std::remove_if(DispatchToLoadBufferQueue.begin(), DispatchToLoadBufferQueue.end(), Predicate), DispatchToLoadBufferQueue.end());
}



void
DynamicAnalysis::RemoveFromDispatchToStoreBufferQueue(uint64_t Cycle){
  StructMemberLessThanOrEqualThanValuePred Predicate = {Cycle};
  DispatchToStoreBufferQueue.erase(std::remove_if(DispatchToStoreBufferQueue.begin(), DispatchToStoreBufferQueue.end(), Predicate), DispatchToStoreBufferQueue.end());
}



void
DynamicAnalysis::RemoveFromDispatchToLineFillBufferQueue(uint64_t Cycle){
  
  StructMemberLessThanOrEqualThanValuePred Predicate = {Cycle};
  DispatchToLineFillBufferQueue.erase(std::remove_if(DispatchToLineFillBufferQueue.begin(), DispatchToLineFillBufferQueue.end(), Predicate), DispatchToLineFillBufferQueue.end());
}



void
DynamicAnalysis::DispatchToLoadBuffer(uint64_t Cycle){
  vector<InstructionDispatchInfo>::iterator it = DispatchToLoadBufferQueue.begin();
  for(; it != DispatchToLoadBufferQueue.end();) {
    if ((*it).IssueCycle == InstructionFetchCycle) {
      //Erase returns the next valid iterator => insert in LoadBuffer before it is removed
      LoadBufferCompletionCycles.push_back((*it).CompletionCycle);
      it = DispatchToLoadBufferQueue.erase(it);
    }else
      ++it;
  }
}





void DynamicAnalysis::inOrder(uint64_t i, ComplexTree<uint64_t> * n) {
  if(n == NULL) return;
  inOrder(i,n->left);
  if (  n->IssueCycle <= i && n->IssueCycle != 0) {
    if (node_size(LoadBufferCompletionCyclesTree) == 0) {
      MinLoadBuffer = n->key;
    }else{
      MinLoadBuffer = min(MinLoadBuffer,n->key);
    }
    
    
    //dbgs() << "Inserting into LB node with issue cycle " << n->IssueCycle << " and key " << n->key << "\n";
    DEBUG(dbgs() << "Inserting into LB node with issue cycle " << n->IssueCycle << " and key " << n->key << "\n");
    LoadBufferCompletionCyclesTree= insert_node(n->key , LoadBufferCompletionCyclesTree);
    PointersToRemove.push_back(n);
    /*    DEBUG(dbgs()<< "Removing from dispatch\n");
     DEBUG(dbgs()<< "Size before "<<node_size(DispatchToLoadBufferQueueTree)<<"\n");
     dbgs()<< "Removing from dispatch\n";
     dbgs()<< "Size before "<<node_size(DispatchToLoadBufferQueueTree)<<"\n";
     dbgs()<< "DispatchToLoadBufferQueueTree root key "<<DispatchToLoadBufferQueueTree->key<<"\n";
     dbgs()<< "Trying to remove "<<n->key<<"\n";
     DispatchToLoadBufferQueueTree = delete_node(n->key, DispatchToLoadBufferQueueTree);
     n = DispatchToLoadBufferQueueTree;
     if(DispatchToLoadBufferQueueTree==NULL)
     dbgs()<< "DispatchToLoadBufferQueueTree is NULL\n";
     
     dbgs()<< "Size after "<<node_size(DispatchToLoadBufferQueueTree)<<"\n";
     
     DEBUG(dbgs()<< "Size after "<<node_size(DispatchToLoadBufferQueueTree)<<"\n");
     
     if (DispatchToLoadBufferQueueTree!= NULL && i == MaxDispatchToLoadBufferQueueTree) {
     MaxDispatchToLoadBufferQueueTree = DispatchToLoadBufferQueueTree->IssueCycle;
     }
     
     */
  }
  
  inOrder(i,n->right);
  
}




void
DynamicAnalysis::DispatchToLoadBufferTree(uint64_t Cycle){
  
  
  
  inOrder(Cycle, DispatchToLoadBufferQueueTree);
  
  
  for(size_t i = 0; i< PointersToRemove.size(); ++i){
    
    DispatchToLoadBufferQueueTree = delete_node(PointersToRemove.at(i)->key, DispatchToLoadBufferQueueTree);
    
  }
  PointersToRemove.clear();
  
  //DispatchToLoadBufferQueueTree = remove(DispatchToLoadBufferQueueTree,Cycle);
  
  
  
  
  //  dbgs()<< "Removing from dispatch\n";
  // dbgs()<< "Size before "<<node_size(DispatchToLoadBufferQueueTree)<<"\n";
  /*
   DispatchToLoadBufferQueueTree->left = NULL;
   if (DispatchToLoadBufferQueueTree->key == Cycle) {
   DispatchToLoadBufferQueueTree = delete_node(Cycle, DispatchToLoadBufferQueueTree);
   }
   dbgs()<< "Size after "<<node_size(DispatchToLoadBufferQueueTree)<<"\n";
   */
  
  
  //DispatchToLoadBufferQueueTree =  RemoveFromDispatchAndInsertIntoLoad(Cycle, DispatchToLoadBufferQueueTree);
  /*
   vector<InstructionDispatchInfo>::iterator it = DispatchToLoadBufferQueue.begin();
   for(; it != DispatchToLoadBufferQueue.end();) {
   if ((*it).IssueCycle == InstructionFetchCycle) {
   //Erase returns the next valid iterator => insert in LoadBuffer before it is removed
   if (node_size(LoadBufferCompletionCyclesTree) == 0) {
   MinLoadBuffer =(*it).CompletionCycle;
   }else{
   MinLoadBuffer = min(MinLoadBuffer,(*it).CompletionCycle);
   }
   
   LoadBufferCompletionCyclesTree= insert_node((*it).CompletionCycle , LoadBufferCompletionCyclesTree);
   it = DispatchToLoadBufferQueue.erase(it);
   }else
   ++it;
   }
   */
}


void
DynamicAnalysis::DispatchToStoreBuffer(uint64_t Cycle){
  vector<InstructionDispatchInfo>::iterator it = DispatchToStoreBufferQueue.begin();
  for(; it != DispatchToStoreBufferQueue.end(); ) {
    if ((*it).IssueCycle == InstructionFetchCycle) {
      StoreBufferCompletionCycles.push_back((*it).CompletionCycle);
      it = DispatchToStoreBufferQueue.erase(it);
    }else
      ++it;
  }
}



void
DynamicAnalysis::DispatchToLineFillBuffer(uint64_t Cycle){
  vector<InstructionDispatchInfo>::iterator it = DispatchToLineFillBufferQueue.begin();
  for(; it != DispatchToLineFillBufferQueue.end();) {
    
    if ((*it).IssueCycle == InstructionFetchCycle) {
      LineFillBufferCompletionCycles.push_back((*it).CompletionCycle);
      it = DispatchToLineFillBufferQueue.erase(it);
    }else
      ++it;
  }
}



uint64_t
DynamicAnalysis::FindIssueCycleWhenLineFillBufferIsFull(){
  
  size_t BufferSize = DispatchToLineFillBufferQueue.size();
  
  if ( BufferSize== 0) {
    return GetMinCompletionCycleLineFillBuffer();
  }else{
    if (BufferSize >= (unsigned)LineFillBufferSize) {
      // Iterate from end-LineFillBufferSize
      uint64_t EarliestCompletion = DispatchToLineFillBufferQueue.back().CompletionCycle;
      for(vector<InstructionDispatchInfo>::iterator it = DispatchToLineFillBufferQueue.end()-1;
          it >= DispatchToLineFillBufferQueue.end()-LineFillBufferSize; --it){
        if ((*it).CompletionCycle < EarliestCompletion) {
          EarliestCompletion =(*it).CompletionCycle;
        }
      }
      return EarliestCompletion;
    }else{
      sort(LineFillBufferCompletionCycles.begin(), LineFillBufferCompletionCycles.end());
      return LineFillBufferCompletionCycles[BufferSize];
    }
  }
}



uint64_t
DynamicAnalysis::FindIssueCycleWhenLoadBufferIsFull(){
  
  size_t BufferSize = DispatchToLoadBufferQueue.size();
  
  if ( BufferSize== 0) {
    return GetMinCompletionCycleLoadBuffer();
  }else{
    
    // Iterate through the DispathToLoadBufferQueue and get the
    // largest dispatch cycle. The new load cannot be dispatched
    // untill all previous in Dispatch Queue have been dispatched.
    // At the same time,
    uint64_t EarliestDispatchCycle  = 0;
    
    for(vector<InstructionDispatchInfo>::iterator it = DispatchToLoadBufferQueue.begin();
        it != DispatchToLoadBufferQueue.end(); ++it){
      EarliestDispatchCycle = max(EarliestDispatchCycle, (*it).IssueCycle);
    }
    EarliestDispatchCycle = MaxDispatchToLoadBufferQueueTree;
    
    //Traverse LB and count how many elements are there smaller than EarliestDispathCycle
    unsigned counter = 0;
    for(vector<uint64_t>::iterator it = LoadBufferCompletionCycles.begin();
        it != LoadBufferCompletionCycles.end(); ++it){
      if ((*it) <= EarliestDispatchCycle)
        counter++;
    }
    uint64_t IssueCycle = 0;
    // This means that in LB, there are more loads that terminate before or in
    // my dispatch cycle -> IssueCycle is Earliest
    if (counter > BufferSize) {
      IssueCycle = EarliestDispatchCycle;
    }else{
      if (counter == BufferSize) {
        // Iterate thtough both, DispatchBufferQueue and LB to determine the smallest
        // completion cycle which is larger than EarliestDispatchCycle.
        // Initialize with the Completion cycle of the last element of the
        // DispatchToLoadBufferQueue
        IssueCycle = DispatchToLoadBufferQueue.back().CompletionCycle;
        for(vector<InstructionDispatchInfo>::iterator it = DispatchToLoadBufferQueue.begin();
            it != DispatchToLoadBufferQueue.end(); ++it){
          if ((*it).CompletionCycle > EarliestDispatchCycle)
            IssueCycle = min(IssueCycle,(*it).CompletionCycle);
        }
        
        // We have to also iterate over the completion cycles of the LB even
        // if there are more elements in the DispatchQueue than the size
        // of the LB. Because it can happen than all the elements of the
        // DispatchQueue are complemente even before than an element in the
        // LB which is waiting very long for a resource.
        // We could, nevertheless, simplify it. We can keep the max and
        // the min completion cycle always. If the max completion cycle
        // is smaller than the EarliestDispatchCycle, then it is not necessary
        // to iterate over the LB.
        for(vector<uint64_t>::iterator it = LoadBufferCompletionCycles.begin();
            it != LoadBufferCompletionCycles.end(); ++it){
          if ((*it) > EarliestDispatchCycle+1)
            IssueCycle = min(IssueCycle,*it);
        }
        
      }else{
        report_fatal_error("Error in Dispatch to Load Buffer Queue");
        
      }
    }
    
    
    
    // The idea before was that the lower "BufferSize" elements of the sorted
    // LB are associated to the dipatch cycles of the elements in the DispatchQueue.
    // But this sorting is very exepensive.
    /*
     if (BufferSize >= (unsigned)LoadBufferSize) {
     // Iterate from end-LineFillBufferSize
     uint64_t EarliestCompletion = DispatchToLoadBufferQueue.back().CompletionCycle;
     for(vector<InstructionDispatchInfo>::iterator it = DispatchToLoadBufferQueue.end()-1;
     it >= DispatchToLoadBufferQueue.end()-LoadBufferSize; --it){
     if ((*it).CompletionCycle < EarliestCompletion) {
     EarliestCompletion =(*it).CompletionCycle;
     }
     }
     return EarliestCompletion;
     }else{
     sort(LoadBufferCompletionCycles.begin(), LoadBufferCompletionCycles.end());
     return LoadBufferCompletionCycles[BufferSize];
     }*/
    return IssueCycle;
    
  }
  
}




uint64_t
DynamicAnalysis::FindIssueCycleWhenLoadBufferTreeIsFull(){
  
  
  // size_t BufferSize = DispatchToLoadBufferQueue.size();
  size_t BufferSize = node_size(DispatchToLoadBufferQueueTree);
  
  if ( BufferSize== 0) {
    return GetMinCompletionCycleLoadBufferTree();
  }else{
    
    // Iterate through the DispathToLoadBufferQueue and get the
    // largest dispatch cycle. The new load cannot be dispatched
    // untill all previous in Dispatch Queue have been dispatched.
    // At the same time,
    uint64_t EarliestDispatchCycle  = 0;
    uint64_t AvailableSlots = 0;
    /*
     for(vector<InstructionDispatchInfo>::iterator it = DispatchToLoadBufferQueue.begin();
     it != DispatchToLoadBufferQueue.end(); ++it){
     EarliestDispatchCycle = max(EarliestDispatchCycle, (*it).IssueCycle);
     }*/
    EarliestDispatchCycle = MaxDispatchToLoadBufferQueueTree;
    
    DEBUG(dbgs() << "EarliestDispatchCycle " << EarliestDispatchCycle << "\n");
    //Traverse LB and count how many elements are there smaller than EarliestDispathCycle
    uint64_t SlotsCompleteBeforeDispatch = 0;
    uint64_t SlotsCompleteAfterDispatch = 0;
    
    LoadBufferCompletionCyclesTree = splay(EarliestDispatchCycle, LoadBufferCompletionCyclesTree);
    DEBUG(dbgs() << "LoadBufferCompletionCyclesTree->key " << LoadBufferCompletionCyclesTree->key << "\n");
    if (LoadBufferCompletionCyclesTree->left !=NULL){
      DEBUG(dbgs() << "Left was not null\n");
      SlotsCompleteBeforeDispatch = node_size(LoadBufferCompletionCyclesTree->left);
      if (LoadBufferCompletionCyclesTree->key <= EarliestDispatchCycle) {
        DEBUG(dbgs() << "They key equal to Earliest, so increase counter by 1\n");
        SlotsCompleteBeforeDispatch++;
      }
    }else{
      DEBUG(dbgs() << "Left was null\n");
      if (LoadBufferCompletionCyclesTree->key == EarliestDispatchCycle) {
        DEBUG(dbgs() << "but they key equal to Earliest, so counter in 1\n");
        SlotsCompleteBeforeDispatch=1;
      }else{
        SlotsCompleteBeforeDispatch = 0;
      }
    }
    
    
    AvailableSlots = SlotsCompleteBeforeDispatch;
    DEBUG(dbgs() << "AvailableSlots "<<AvailableSlots<<"\n");
    
    // Traverse DispatchToLoadBufferQueuteTree and count how many
    // complete after my EarliestDispatchCycle
    
    /*
     ComplexTree<uint64_t> Node = DispatchToLoadBufferQueueTree;
     
     
     while (true) {
     // This is the mechanism used in the original algorithm to delete the host
     // node,  decrementing the last_record attribute of the host node, and
     // the size attribute of all parents nodes.
     // Node->size = Node->size-1;
     if (EarliestDispatchCycle < Node->key) {
     
     if (Node->left == NULL)
     break;
     if (Node->left->key < EarliestDispatchCycle) {
     break;
     }
     Node = Node->left;
     }else{
     if (EarliestDispatchCycle > Node-> key) {
     if (Node->right == NULL)
     break;
     Node = Node->right;
     }else{ // Last = Node->key, i.e., Node is the host node
     break;
     }
     }
     }
     
     */
    
    DispatchToLoadBufferQueueTree = splay(EarliestDispatchCycle, DispatchToLoadBufferQueueTree);
    if(DispatchToLoadBufferQueueTree->key > EarliestDispatchCycle){
      // All complete after
      SlotsCompleteAfterDispatch = node_size(DispatchToLoadBufferQueueTree);
    }else{
      if (DispatchToLoadBufferQueueTree->right != NULL) {
        SlotsCompleteAfterDispatch = node_size(DispatchToLoadBufferQueueTree->right);
      }else{
        SlotsCompleteAfterDispatch = 1;
      }
    }/*
      for(vector<InstructionDispatchInfo>::iterator it = DispatchToLoadBufferQueue.begin();
      it != DispatchToLoadBufferQueue.end(); ++it){
      if ((*it).CompletionCycle > EarliestDispatchCycle) {
      SlotsCompleteAfterDispatch++;
      };
      }*/
    DEBUG(dbgs() << "SlotsCompleteAfterDispatch "<<SlotsCompleteAfterDispatch<<"\n");
    
    AvailableSlots -= SlotsCompleteAfterDispatch;
    DEBUG(dbgs() << "AvailableSlots "<<AvailableSlots<<"\n");
    
    //Compute how many complete before dispatch from the DispatchQueue
    
    /*
     for(vector<uint64_t>::iterator it = LoadBufferCompletionCycles.begin();
     it != LoadBufferCompletionCycles.end(); ++it){
     if ((*it) <= EarliestDispatchCycle)
     counter++;
     }
     */
    uint64_t IssueCycle = 0;
    // This means that in LB, there are more loads that terminate before or in
    // my dispatch cycle -> IssueCycle is Earliest
    if (AvailableSlots > 0) {
      
      IssueCycle = EarliestDispatchCycle;
    }else{
      // if (counter == BufferSize) {
      DEBUG(dbgs() << "Counter is <= to BufferSize\n");
      // Iterate thtough both, DispatchBufferQueue and LB to determine the smallest
      // completion cycle which is larger than EarliestDispatchCycle.
      // Initialize with the Completion cycle of the last element of the
      // DispatchToLoadBufferQueue
      //DispatchToLoadBufferQueueTree = splay(EarliestDispatchCycle+1, DispatchToLoadBufferQueueTree);
      
      DEBUG(dbgs() << "Find  in DispatchToLoadBufferQueueTree the largest than or equal to "<< EarliestDispatchCycle+1<<"\n");
      
      ComplexTree<uint64_t>* Node = DispatchToLoadBufferQueueTree;
      
      while (true) {
        // This is the mechanism used in the original algorithm to delete the host
        // node,  decrementing the last_record attribute of the host node, and
        // the size attribute of all parents nodes.
        // Node->size = Node->size-1;
        if (EarliestDispatchCycle+1 < Node->key) {
          
          if (Node->left == NULL)
            break;
          if (Node->left->key < EarliestDispatchCycle+1) {
            break;
          }
          Node = Node->left;
        }else{
          if (EarliestDispatchCycle+1 > Node-> key) {
            if (Node->right == NULL)
              break;
            Node = Node->right;
          }else{ // Last = Node->key, i.e., Node is the host node
            break;
          }
        }
      }
      
      IssueCycle = Node->key;
      DEBUG(dbgs() << "IssueCycle "<<IssueCycle<<"\n");
      
      /*
       IssueCycle = DispatchToLoadBufferQueue.back().CompletionCycle;
       for(vector<InstructionDispatchInfo>::iterator it = DispatchToLoadBufferQueue.begin();
       it != DispatchToLoadBufferQueue.end(); ++it){
       if ((*it).CompletionCycle > EarliestDispatchCycle)
       IssueCycle = min(IssueCycle,(*it).CompletionCycle);
       }
       */
      // We have to also iterate over the completion cycles of the LB even
      // if there are more elements in the DispatchQueue than the size
      // of the LB. Because it can happen than all the elements of the
      // DispatchQueue are complemente even before than an element in the
      // LB which is waiting very long for a resource.
      // We could, nevertheless, simplify it. We can keep the max and
      // the min completion cycle always. If the max completion cycle
      // is smaller than the EarliestDispatchCycle, then it is not necessary
      // to iterate over the LB.
      // The root has EraliestDispatchCycle+1 if found, or the smallest otherwise.
      
      /*LoadBufferCompletionCyclesTree = splay(IssueCycle, LoadBufferCompletionCyclesTree);
       if (LoadBufferCompletionCyclesTree->key < IssueCycle && LoadBufferCompletionCyclesTree->key > EarliestDispatchCycle) { */
      
      //Get the closest larger than or equal to EarliestaDispatchCycle
      DEBUG(dbgs() << "The same with LB\n");
      
      
      SimpleTree<uint64_t> * TmpNode = LoadBufferCompletionCyclesTree;
      
      while (true) {
        // This is the mechanism used in the original algorithm to delete the host
        // node,  decrementing the last_record attribute of the host node, and
        // the size attribute of all parents nodes.
        // Node->size = Node->size-1;
        if (EarliestDispatchCycle+1 < TmpNode->key) {
          
          if (TmpNode->left == NULL)
            break;
          if (TmpNode->left->key < EarliestDispatchCycle+1) {
            break;
          }
          TmpNode = TmpNode->left;
        }else{
          if (EarliestDispatchCycle+1 > TmpNode-> key) {
            if (TmpNode->right == NULL)
              break;
            TmpNode = TmpNode->right;
          }else{ // Last = Node->key, i.e., Node is the host node
            break;
          }
        }
      }
      
      /*
       for(vector<uint64_t>::iterator it = LoadBufferCompletionCycles.begin();
       it != LoadBufferCompletionCycles.end(); ++it){
       if ((*it) > EarliestDispatchCycle+1)
       IssueCycle = min(IssueCycle,*it);
       }
       */
      if (TmpNode->key > EarliestDispatchCycle+1) {
        IssueCycle = min(TmpNode->key, IssueCycle);
        
      }
      DEBUG(dbgs() << "IssueCycle "<<IssueCycle<<"\n");
      
      //}
      /*}else{
       dbgs() << "Counter " << counter << "\n";
       dbgs() << "BufferSize " << BufferSize << "\n";
       report_fatal_error("Error in Dispatch to Load Buffer Queue");
       
       }*/
    }
    
    
    
    // The idea before was that the lower "BufferSize" elements of the sorted
    // LB are associated to the dipatch cycles of the elements in the DispatchQueue.
    // But this sorting is very exepensive.
    /*
     if (BufferSize >= (unsigned)LoadBufferSize) {
     // Iterate from end-LineFillBufferSize
     uint64_t EarliestCompletion = DispatchToLoadBufferQueue.back().CompletionCycle;
     for(vector<InstructionDispatchInfo>::iterator it = DispatchToLoadBufferQueue.end()-1;
     it >= DispatchToLoadBufferQueue.end()-LoadBufferSize; --it){
     if ((*it).CompletionCycle < EarliestCompletion) {
     EarliestCompletion =(*it).CompletionCycle;
     }
     }
     return EarliestCompletion;
     }else{
     sort(LoadBufferCompletionCycles.begin(), LoadBufferCompletionCycles.end());
     return LoadBufferCompletionCycles[BufferSize];
     }*/
    return IssueCycle;
    
  }
  
}




uint64_t
DynamicAnalysis::FindIssueCycleWhenStoreBufferIsFull(){
  
  size_t BufferSize = DispatchToStoreBufferQueue.size();
  
  if ( BufferSize== 0) {
    return GetMinCompletionCycleStoreBuffer();
  }else{
    if (BufferSize >= (unsigned)StoreBufferSize) {
      uint64_t EarliestCompletion = DispatchToStoreBufferQueue.back().CompletionCycle;
      for(vector<InstructionDispatchInfo>::iterator it = DispatchToStoreBufferQueue.end()-1;
          it >= DispatchToStoreBufferQueue.end()-StoreBufferSize; --it){
        if ((*it).CompletionCycle < EarliestCompletion) {
          EarliestCompletion =(*it).CompletionCycle;
        }
      }
      return EarliestCompletion;
    }else{
      sort(StoreBufferCompletionCycles.begin(), StoreBufferCompletionCycles.end());
      return StoreBufferCompletionCycles[BufferSize];
    }
  }
}



void
DynamicAnalysis::PrintReorderBuffer(){
  
  DEBUG(dbgs() << "Reorder Buffer:\n");
  for (unsigned i = 0; i < ReorderBufferCompletionCycles.size(); i++) {
    DEBUG(dbgs() << ReorderBufferCompletionCycles[i] << " ");
  }
  DEBUG(dbgs() << "\n");
}



void
DynamicAnalysis::PrintReservationStation(){
  
  DEBUG(dbgs() << "Reservation Station:\n");
  for (unsigned i = 0; i < ReservationStationIssueCycles.size(); i++) {
    DEBUG(dbgs() << ReservationStationIssueCycles[i] << " ");
  }
  DEBUG(dbgs() << "\n");
}



void
DynamicAnalysis::PrintLoadBuffer(){
  
  DEBUG(dbgs() << "Load Buffer:\n");
  for (unsigned i = 0; i < LoadBufferCompletionCycles.size(); i++) {
    DEBUG(dbgs() << LoadBufferCompletionCycles[i] << " ");
  }
  DEBUG(dbgs() << "\n");
}



void
DynamicAnalysis::PrintStoreBuffer(){
  
  DEBUG(dbgs() << "Store Buffer:\n");
  for (unsigned i = 0; i < StoreBufferCompletionCycles.size(); i++) {
    DEBUG(dbgs() << StoreBufferCompletionCycles[i] << " ");
  }
  DEBUG(dbgs() << "\n");
}



void
DynamicAnalysis::PrintLineFillBuffer(){
  
  DEBUG(dbgs() << "Line Fill Buffer:\n");
  for (unsigned i = 0; i < LineFillBufferCompletionCycles.size(); i++) {
    DEBUG(dbgs() << LineFillBufferCompletionCycles[i] << " ");
  }
  DEBUG(dbgs() << "\n");
}



void
DynamicAnalysis::PrintDispatchToLoadBuffer(){
  
  DEBUG(dbgs() << "Dispatch to Load Buffer Issue Cycles:\n");
  for (unsigned i = 0; i < DispatchToLoadBufferQueue.size(); i++) {
    DEBUG(dbgs() << DispatchToLoadBufferQueue[i].IssueCycle <<  " ");
  }
  DEBUG(dbgs() << "\n");
  DEBUG(dbgs() << "Dispatch to Load Buffer Completion Cycles:\n");
  for (unsigned i = 0; i < DispatchToLoadBufferQueue.size(); i++) {
    DEBUG(dbgs() << DispatchToLoadBufferQueue[i].CompletionCycle <<  " ");
  }
  DEBUG(dbgs() << "\n");
}



void
DynamicAnalysis::PrintDispatchToStoreBuffer(){
  
  DEBUG(dbgs() << "Dispatch to Store Buffer Issue Cycles:\n");
  for (unsigned i = 0; i < DispatchToStoreBufferQueue.size(); i++) {
    DEBUG(dbgs() << DispatchToStoreBufferQueue[i].IssueCycle <<  " ");
  }
  DEBUG(dbgs() << "\n");
  
  DEBUG(dbgs() << "Dispatch to Store Buffer Completion Cycles:\n");
  for (unsigned i = 0; i < DispatchToStoreBufferQueue.size(); i++) {
    DEBUG(dbgs() << DispatchToStoreBufferQueue[i].CompletionCycle <<  " ");
  }
  DEBUG(dbgs() << "\n");
}



void
DynamicAnalysis::PrintDispatchToLineFillBuffer(){
  
  DEBUG(dbgs() << "Dispatch to Line Fill Buffer Issue Cycles:\n");
  for (unsigned i = 0; i < DispatchToLineFillBufferQueue.size(); i++) {
    DEBUG(dbgs() << DispatchToLineFillBufferQueue[i].IssueCycle << " ");
  }
  DEBUG(dbgs() << "\n");
  
  DEBUG(dbgs() << "Dispatch to Line Fill Buffer Completion Cycles:\n");
  for (unsigned i = 0; i < DispatchToLineFillBufferQueue.size(); i++) {
    DEBUG(dbgs() << DispatchToLineFillBufferQueue[i].CompletionCycle << " ");
  }
  DEBUG(dbgs() << "\n");
}



void
DynamicAnalysis::IncreaseInstructionFetchCycle(){
  
  bool OOOBufferFull = false;
  unsigned TreeChunk = 0;
  
  if (DispatchToLineFillBufferQueue.empty() == false) {
    if (InstructionsCountExtended[LFB_STALL]==0)
      FirstIssue[LFB_STALL] = true;
    if (FirstIssue[LFB_STALL]==true) {
      FirstNonEmptyLevel[LFB_STALL] = InstructionFetchCycle;
      FirstIssue[LFB_STALL] = false;
    }
    
    //FirstNonEmptyLevel[LFB_STALL] = (FirstNonEmptyLevel[LFB_STALL]==0)?InstructionFetchCycle:FirstNonEmptyLevel[LFB_STALL];
    InstructionsLastIssueCycle[LFB_STALL] =InstructionFetchCycle;
    FullOccupancyCyclesTree[InstructionFetchCycle/SplitTreeRange] = insert_node(InstructionFetchCycle, LFB_STALL, FullOccupancyCyclesTree[InstructionFetchCycle/SplitTreeRange]);
    InstructionsCountExtended[LFB_STALL]++;
  }
  
  if (node_size(DispatchToLoadBufferQueueTree) != 0) {
    //if (DispatchToLoadBufferQueue.empty() == false) {
    if (InstructionsCountExtended[LB_STALL]==0)
      FirstIssue[LB_STALL] = true;
    if (FirstIssue[LB_STALL]==true) {
      FirstNonEmptyLevel[LB_STALL] = InstructionFetchCycle;
      FirstIssue[LB_STALL] = false;
    }
    //FirstNonEmptyLevel[LB_STALL] = (FirstNonEmptyLevel[LB_STALL]==0)?InstructionFetchCycle:FirstNonEmptyLevel[LB_STALL];
    InstructionsLastIssueCycle[LB_STALL] =InstructionFetchCycle;
    FullOccupancyCyclesTree[InstructionFetchCycle/SplitTreeRange] = insert_node(InstructionFetchCycle, LB_STALL, FullOccupancyCyclesTree[InstructionFetchCycle/SplitTreeRange]);
    InstructionsCountExtended[LB_STALL]++;
  }
  
  if (DispatchToStoreBufferQueue.empty() == false) {
    if (InstructionsCountExtended[SB_STALL]==0)
      FirstIssue[SB_STALL] = true;
    if (FirstIssue[SB_STALL]==true) {
      FirstNonEmptyLevel[SB_STALL] = InstructionFetchCycle;
      FirstIssue[SB_STALL] = false;
    }
    
    //FirstNonEmptyLevel[SB_STALL] = (FirstNonEmptyLevel[SB_STALL]==0)?InstructionFetchCycle:FirstNonEmptyLevel[SB_STALL];
    InstructionsLastIssueCycle[SB_STALL] =InstructionFetchCycle;
    FullOccupancyCyclesTree[InstructionFetchCycle/SplitTreeRange] = insert_node(InstructionFetchCycle, SB_STALL,FullOccupancyCyclesTree[InstructionFetchCycle/SplitTreeRange]);
    InstructionsCountExtended[SB_STALL]++;
  }
  
  DEBUG(dbgs() << "_____________________ InstructionFetchCycle "<<InstructionFetchCycle<<"_____________________\n");
  
#ifdef MOO_BUFFERS
#ifdef DEBUG_OOO_BUFFERS
  PrintReservationStation();
  PrintReorderBuffer();
  PrintStoreBuffer();
  PrintLoadBuffer();
  PrintLineFillBuffer();
  PrintDispatchToStoreBuffer();
  PrintDispatchToLoadBuffer();
  PrintDispatchToLineFillBuffer();
#endif
#endif
  
  
  // Remove from Reservation Stations elements issued at fetch cycle
  if (ReservationStationSize > 0)
    RemoveFromReservationStation(InstructionFetchCycle);
  
  if (ReorderBufferSize > 0)
    RemoveFromReorderBuffer(InstructionFetchCycle);
  
  
  
#ifdef MOO_BUFFERS
  //Remove from Load, Store and Fill Line Buffers elements completed at issue cycle
  
  //RemoveFromLoadBuffer(InstructionFetchCycle);
  
  RemoveFromLoadBufferTree(InstructionFetchCycle);
  RemoveFromStoreBuffer(InstructionFetchCycle);
  RemoveFromLineFillBuffer(InstructionFetchCycle);
  //RemoveFromDispatchToLoadBufferQueue(InstructionFetchCycle);
  RemoveFromDispatchToStoreBufferQueue(InstructionFetchCycle);
  RemoveFromDispatchToLineFillBufferQueue(InstructionFetchCycle);
  // Insert into LB, SB and LFB the instructions from the dispatch queue.
  //DispatchToLoadBuffer(InstructionFetchCycle);
  DispatchToLoadBufferTree(InstructionFetchCycle);
  DispatchToStoreBuffer(InstructionFetchCycle);
  DispatchToLineFillBuffer(InstructionFetchCycle);
#endif
  
#ifdef MOO_BUFFERS
#ifdef DEBUG_OOO_BUFFERS
  PrintReservationStation();
  PrintReorderBuffer();
  PrintStoreBuffer();
  PrintLoadBuffer();
  PrintLineFillBuffer();
  PrintDispatchToStoreBuffer();
  PrintDispatchToLoadBuffer();
  PrintDispatchToLineFillBuffer();
#endif
#endif
  // If Reservation station is full
  if (ReservationStationIssueCycles.size() == ReservationStationSize && ReservationStationSize > 0) {
    
    // Advance InstructionFetchCyle untill min issue cycle
    OOOBufferFull = true;
    uint64_t CurrentInstructionFetchCycle = InstructionFetchCycle;
    InstructionFetchCycle = GetMinIssueCycleReservationStation();
    
    if (InstructionFetchCycle>CurrentInstructionFetchCycle+1)
      FirstNonEmptyLevel[RS_STALL] = (FirstNonEmptyLevel[RS_STALL]==0)?CurrentInstructionFetchCycle+1:FirstNonEmptyLevel[RS_STALL];
    
    
    for (uint64_t i = CurrentInstructionFetchCycle+1; i< InstructionFetchCycle; i++) {
      TreeChunk =i/SplitTreeRange;
      if (TreeChunk >= (unsigned)FullOccupancyCyclesTree.size()) {
        for (unsigned j = FullOccupancyCyclesTree.size(); j<= TreeChunk; j++) {
          DEBUG(dbgs() << "Iserting element in FullOccupancyCyclesTree");
          FullOccupancyCyclesTree.push_back(NULL);
        }
      }
      FullOccupancyCyclesTree[TreeChunk] = insert_node(i, RS_STALL,FullOccupancyCyclesTree[TreeChunk]);
      InstructionsCountExtended[RS_STALL]++;
      InstructionsLastIssueCycle[RS_STALL] =i;
    }
    
#ifdef DEBUG_OOO_BUFFERS
    DEBUG(dbgs() << "Updating InstructionFetchCycle to "<<InstructionFetchCycle <<" \n");
#endif
  }
  if (ReorderBufferCompletionCycles.size() == ReorderBufferSize && ReorderBufferSize > 0) {
    //Advance InstructionFetchCycle to the head of the buffer
    OOOBufferFull = true;
    uint64_t CurrentInstructionFetchCycle = InstructionFetchCycle;
    InstructionFetchCycle = max(InstructionFetchCycle, ReorderBufferCompletionCycles.front());
    if (InstructionFetchCycle>CurrentInstructionFetchCycle+1) {
      FirstNonEmptyLevel[ROB_STALL] = (FirstNonEmptyLevel[ROB_STALL]==0)?CurrentInstructionFetchCycle+1:FirstNonEmptyLevel[ROB_STALL];
    }
    
    
    for (uint64_t i = CurrentInstructionFetchCycle+1; i< InstructionFetchCycle; i++) {
      // Get the node, if any, corresponding to this issue cycle.
      TreeChunk =i/SplitTreeRange;
      if (TreeChunk >= (unsigned)FullOccupancyCyclesTree.size()) {
        for (unsigned j = FullOccupancyCyclesTree.size(); j<= TreeChunk; j++) {
          DEBUG(dbgs() << "Iserting element in FullOccupancyCyclesTree");
          FullOccupancyCyclesTree.push_back(NULL);
        }
      }
      
      FullOccupancyCyclesTree[TreeChunk] = insert_node(i, ROB_STALL, FullOccupancyCyclesTree[TreeChunk] );
      InstructionsCountExtended[ROB_STALL]++;
      InstructionsLastIssueCycle[ROB_STALL] =i;
    }
#ifdef DEBUG_OOO_BUFFERS
    DEBUG(dbgs() << "Updating InstructionFetchCycle to "<<InstructionFetchCycle <<" \n");
#endif
  }
  if (OOOBufferFull==true) {
    // Remove from Reservation Stations elements issued at fetch cycle
#ifdef DEBUG_OOO_BUFFERS
    PrintReservationStation();
    PrintReorderBuffer();
#ifdef MOO_BUFFERS
    PrintStoreBuffer();
    PrintLoadBuffer();
    PrintLineFillBuffer();
    PrintDispatchToStoreBuffer();
    PrintDispatchToLoadBuffer();
    PrintDispatchToLineFillBuffer();
#endif
#endif
    
    RemoveFromReservationStation(InstructionFetchCycle);
    RemoveFromReorderBuffer(InstructionFetchCycle);
    
#ifdef MOO_BUFFERS
    //Remove from Load, Store and Fill Line Buffers elements completed at issue cycle
    // RemoveFromLoadBuffer(InstructionFetchCycle);
    RemoveFromLoadBufferTree(InstructionFetchCycle);
    RemoveFromStoreBuffer(InstructionFetchCycle);
    RemoveFromLineFillBuffer(InstructionFetchCycle);
    //  RemoveFromDispatchToLoadBufferQueue(InstructionFetchCycle);
    RemoveFromDispatchToStoreBufferQueue(InstructionFetchCycle);
    RemoveFromDispatchToLineFillBufferQueue(InstructionFetchCycle);
    // Insert into LB, SB and LFB the instructions from the dispatch queue.
    //  DispatchToLoadBuffer(InstructionFetchCycle);
    DispatchToLoadBufferTree(InstructionFetchCycle);
    DispatchToStoreBuffer(InstructionFetchCycle);
    DispatchToLineFillBuffer(InstructionFetchCycle);
#endif
    
#ifdef DEBUG_OOO_BUFFERS
    PrintReservationStation();
    PrintReorderBuffer();
#ifdef MOO_BUFFERS
    PrintStoreBuffer();
    PrintLoadBuffer();
    PrintLineFillBuffer();
    PrintDispatchToStoreBuffer();
    PrintDispatchToLoadBuffer();
    PrintDispatchToLineFillBuffer();
#endif
#endif
  }
  RemainingInstructionsFetch = InstructionFetchBandwidth;
  if (OOOBufferFull==false && InstructionFetchBandwidth != INF)
    InstructionFetchCycle++;
  
  BuffersOccupancy[RS_STALL-RS_STALL] += ReservationStationIssueCycles.size();
  BuffersOccupancy[ROB_STALL-RS_STALL] += ReorderBufferCompletionCycles.size();
  //BuffersOccupancy[LB_STALL-RS_STALL] += LoadBufferCompletionCycles.size();
  BuffersOccupancy[LB_STALL-RS_STALL] += node_size(LoadBufferCompletionCyclesTree);
  BuffersOccupancy[SB_STALL-RS_STALL] += StoreBufferCompletionCycles.size();
  BuffersOccupancy[LFB_STALL-RS_STALL] += LineFillBufferCompletionCycles.size();
  
  
}


//===----------------------------------------------------------------------===//
//          Main method for analysis of the instruction properties
//                    (dependences, reuse, etc.)
//===----------------------------------------------------------------------===//


// Handling instructions dependences with def-use chains.
// Whenever there is a def, and we know the issue cycle (IssueCycle )of the def,
// update all the uses of that definition with IssueCycle+1.
// The issue cycle of an instruction is hence the max of the issue cycles of its
// operands, but the issue cycle of its operands does not have to be determined,
// already contains the right value because they are uses of a previous definition.

#ifdef INTERPRETER
void
DynamicAnalysis::analyzeInstruction(Instruction &I, ExecutionContext &SF,  GenericValue * visitResult)
#else
void
DynamicAnalysis::analyzeInstruction(Instruction &I, uint64_t addr)
#endif
{
  
  
  int k = 0;
  int Distance;
  int NextCacheLineExtendedInstructionType;
  int InstructionType = getInstructionType(I);
  bool isLoad =true;
  CacheLineInfo Info;
  uint64_t CacheLine = 0;
  uint64_t MemoryAddress = 0;
  uint64_t LoadCacheLine;
  uint64_t StoreCacheLine;
  uint64_t NextCacheLine;
  uint64_t NextCacheLineIssueCycle;
  uint64_t InstructionIssueCycle, OriginalInstructionIssueCycle, LastAccess;
  uint64_t InstructionIssueFetchCycle = 0, InstructionIssueLoadBufferAvailable = 0,
  InstructionIssueLineFillBufferAvailable = 0, InstructionIssueStoreBufferAvailable = 0,
  InstructionIssueAGUAvailable = 0, InstructionIssueDataDeps = 0, InstructionIssueCacheLineAvailable = 0,
  InstructionIssueMemoryModel = 0, InstructionIssueStoreAGUAvailable = 0, InstructionIssueLoadAGUAvailable = 0,
  InstructionIssuePortAvailable = 0, InstructionIssueThroughputAvailable = 0, InstructionIssueCycleFirstTimeAvailable = 0;
  uint Latency = 0;
  uint LatencyPrefetch = 0;
  //Aux vars for handling arguments of a call instruction
  CallSite CS;
  Function *F;
  std::vector<Value *> ArgVals;
  unsigned NumArgs;
  unsigned ExtendedInstructionType = InstructionType;
  unsigned ExecutionResource = 0;
  
  TotalInstructions++;
  
  // PREPRATION FOR VECTOR CODE
  
  bool IsVectorInstruction = false;
  unsigned NElementsVector = 1;
  
  // Determine instruction width
  int NumOperands = I.getNumOperands();
  if (NumOperands > 0) {
    int OperandPosition =  (I.getOpcode()==Instruction::Store)?1:0;
    Type * Ty = I.getOperand(OperandPosition)->getType();
    if(PointerType* PT = dyn_cast<PointerType>(Ty)){
      if (PT->getElementType()->getTypeID()== Type::VectorTyID)
        NElementsVector = PT->getElementType()->getVectorNumElements();
    }
    if (Ty->getTypeID()==Type::VectorTyID) { // For arithmetic
      IsVectorInstruction = true;
      NElementsVector = Ty->getVectorNumElements();
    }
  }
  
#ifdef MICROSCHEDULING
  if(dyn_cast<TerminatorInst>(&I)){
    DEBUG(dbgs() << "New Basic Block\n");
    BasicBlockLookAhead++;
    if (BasicBlockLookAhead ==1) {
      BasicBlockLookAhead = 0;
      BasicBlockBarrier = 0;
      for(j=0; j< 6; j++){
        BasicBlockBarrier = max ((unsigned long)BasicBlockBarrier, DAGLevelsOccupancy[j].size());
      }
      DEBUG(dbgs() << "BasicBlockBarrier "<<BasicBlockBarrier<<"\n");
    }
  }
#endif
  
  if (WarmCache && rep == 0) {
    
    if (InstructionType >= 0) {
      
      switch (I.getOpcode()) {
        case Instruction::Load:{
#ifdef INTERPRETER
          //Transform visitResult to uint64_t
          SmallString <128> StrVal;
          raw_svector_ostream OS(StrVal);
          OS << visitResult;
          MemoryAddress = strtol(OS.str().str().c_str(),NULL,16);
#else
          MemoryAddress = addr;
#endif
          
          CacheLine = MemoryAddress >> BitsPerCacheLine;
          Info = getCacheLineInfo(CacheLine);
          
#ifdef DEBUG_MEMORY_TRACES
          DEBUG(dbgs() << "MemoryAddress " << MemoryAddress << "\n");
          DEBUG(dbgs() << "CacheLine " << CacheLine << "\n");
#endif
          
          //Code for reuse calculation
          Distance =  ReuseDistance(Info.LastAccess, TotalInstructions, CacheLine);
          
          Info.LastAccess = TotalInstructions;
          insertCacheLineLastAccess(CacheLine, Info.LastAccess );
          //   insertMemoryAddressIssueCycle(MemoryAddress, TotalInstructions);
          //   updateReuseDistanceDistribution(Distance, InstructionIssueCycle);
          ExtendedInstructionType = GetExtendedInstructionType(Instruction::Load, Distance);
        }
          break;
        case Instruction::Store:{
#ifdef INTERPRETER
          SmallString <128> StrVal;
          raw_svector_ostream OS(StrVal);
          OS << visitResult;
          MemoryAddress = strtol(OS.str().str().c_str(),NULL,16);
#else
          MemoryAddress = addr;
#endif
          CacheLine = MemoryAddress >> BitsPerCacheLine;
          Info = getCacheLineInfo(CacheLine);
          
#ifdef DEBUG_MEMORY_TRACES
          DEBUG(dbgs() << "MemoryAddress " << MemoryAddress << "\n");
          DEBUG(dbgs() << "CacheLine " << CacheLine << "\n");
#endif
          
          Distance = ReuseDistance(Info.LastAccess, TotalInstructions, CacheLine);
          
          Info.LastAccess = TotalInstructions;
          insertCacheLineLastAccess(CacheLine, Info.LastAccess );
          //     insertMemoryAddressIssueCycle(MemoryAddress, TotalInstructions);
          //  updateReuseDistanceDistribution(Distance, InstructionIssueCycle);
          ExtendedInstructionType = GetExtendedInstructionType(Instruction::Store, Distance);
        }
          break;
          
        default:
          break;
      }
      
      // ============================ SPATIAL PREFETCHER ==============================
      
      if (SpatialPrefetcher && (I.getOpcode() ==Instruction::Load || I.getOpcode() ==Instruction::Store)&&
          (ExtendedInstructionType > PrefetchDispatch && !(ExecutionUnit[ExtendedInstructionType] == PrefetchLevel))/*ExtendedInstructionType > L1_STORE_NODE*/ /*&& (CacheLine %2) == 0*/
          /*  &&(ExtendedInstructionType == MEM_LOAD_NODE || ExtendedInstructionType == MEM_STORE_NODE )*/) {
        
        NextCacheLine = CacheLine+1;
        
        //Get reuse distance of NextCacheLine
        Info = getCacheLineInfo(NextCacheLine);
        Distance =  ReuseDistance(Info.LastAccess, TotalInstructions, NextCacheLine, true);
        NextCacheLineExtendedInstructionType = GetMemoryInstructionType(Distance, MemoryAddress);
        
        ExecutionResource = ExecutionUnit[NextCacheLineExtendedInstructionType];
        
        //   if (!(WarmCache && rep == 0)){
#ifdef DEBUG_PREFETCHER
        DEBUG(dbgs() << "CacheLine " << CacheLine << "\n");
        DEBUG(dbgs() << "NextCacheLine " << NextCacheLine << "\n");
        DEBUG(dbgs() << "Execution Resource  " << ResourcesNames[ExecutionResource] << "\n");
        DEBUG(dbgs() << "PrefetchTarget  " << PrefetchTarget << "\n");
        DEBUG(dbgs() << "PrefetchLevel  " << PrefetchLevel << "\n");
        
#endif
        //}
        // Only bring data from memory to the die, not for example, from LLC to L2
        // if (ExecutionResource == MEM_LOAD_CHANNEL || ExecutionResource == MEM_STORE_CHANNEL) {
        if (ExecutionResource > PrefetchTarget && ExecutionResource >= PrefetchDestination) {
          // if (!(WarmCache && rep == 0)){
#ifdef DEBUG_PREFETCHER
          DEBUG(dbgs() << "Prefetching next cache line which is in "<<ResourcesNames[ExecutionResource]<<"\n");
#endif
          //}
          Info.LastAccess = TotalInstructions;
          insertCacheLineLastAccess(NextCacheLine, Info.LastAccess );
        }
      }
    }
  }else{
    
    
    
    //================= Update Fetch Cycle, remove insts from buffers =========//
    // EVERY INSTRUCTION IN THE RESERVATION STATION IS ALSO IN THE REORDER BUFFER
    DEBUG(dbgs()<<  I<< " ("<< &I <<")\n");
    if (InstructionType >= 0) {
      //   if (ReservationStationIssueCycles.size() == (unsigned)ReservationStationSize) {
      if (RemainingInstructionsFetch == 0 || /*RemainingInstructionsFetch == INF||*/
          (ReorderBufferCompletionCycles.size() == (unsigned)ReorderBufferSize && ReorderBufferSize != 0)
          || (ReservationStationIssueCycles.size() == (unsigned)ReservationStationSize && ReservationStationSize != 0)){
        
        IncreaseInstructionFetchCycle();
        
      }
    }
    
    //==================== Handle special cases ===============================//
    switch (I.getOpcode()) {
        // Dependences through PHI nodes
      case Instruction::Br:
      case Instruction::IndirectBr:
      case Instruction::Switch:{
#ifdef DEBUG_PHI_NODE
        DEBUG(dbgs() << "Loop over all of the PHI nodes in the current block\n");
#endif
#ifdef INTERPRETER
        // Loop over all of the PHI nodes in the current block, reading their inputs.
        SF.CurInst = SF.CurBB->begin();
        for (unsigned i = 0; PHINode *PN = dyn_cast<PHINode>(SF.CurInst);
             ++SF.CurInst, ++i)
        /*
         #else
         auto it = I.getParent()->begin();
         for (unsigned i = 0; PHINode *PN = dyn_cast<PHINode>(it);
         ++it, ++i)
         
         #endif
         */          {
           //Value *Predecesor = PN->getIncomingValue(PN->getBasicBlockIndex(I.getParent()));
           // The PHI node was a use of its predecessor. Hence, its entry in the map
           //contains the correct value of the InstructionIssueCycle
           // InstructionIssueCycle = getInstructionValueIssueCycle(Predecesor); -> WRONG!!
           InstructionIssueCycle = max(max(InstructionFetchCycle,BasicBlockBarrier),getInstructionValueIssueCycle(PN));
#ifdef DEBUG_PHI_NODE
           DEBUG(dbgs() << "PHI Node " << PN << "\n");
           DEBUG(dbgs() << "InstructionValueIssueCycle of PHI Node " << InstructionIssueCycle << "\n");
#endif
           
#ifdef INTERPRETER
           // Iterate through the uses of the PHI node
           for(Value::use_iterator i = PN->use_begin(), ie = PN->use_end(); i!=ie; ++i){
#ifdef DEBUG_PHI_NODE
             DEBUG(dbgs() << "Use of the PHI node " << *i << "\n");
#endif
             if (dyn_cast<PHINode>(*i)) {
               insertInstructionValueIssueCycle(*i, InstructionIssueCycle, true);
             }else{
               insertInstructionValueIssueCycle(*i, InstructionIssueCycle);
             }
             
             // insertInstructionValueIssueCycle(*i, InstructionIssueCycle);
           }
           /*
            #else
            // Iterate through the uses of the PHI node
            for (User *U : PN->users()) {
            //        if (Instruction *i = dyn_cast<Instruction>(U)) {
            #ifdef DEBUG_PHI_NODE
            DEBUG(dbgs() << "Use of the PHI node " << U << "\n");
            #endif
            if (dyn_cast<PHINode>(U)) {
            insertInstructionValueIssueCycle(U, InstructionIssueCycle, true);
            }else{
            insertInstructionValueIssueCycle(U, InstructionIssueCycle);
            }
            
            // insertInstructionValueIssueCycle(*i, InstructionIssueCycle);
            //}
            }
            */
#endif
         }
#endif
        InstructionIssueCycle = max(max(InstructionFetchCycle,BasicBlockBarrier),getInstructionValueIssueCycle(&I)); // This is the branch instrucion
        
        //Iterate over the uses of the generated value
#ifdef INTERPRETER
        for(Value::use_iterator i = I.use_begin(), ie = I.use_end(); i!=ie; ++i){
          insertInstructionValueIssueCycle(*i, InstructionIssueCycle+1/*???*/);
        }
#else
        for (User *U : I.users()) {
          // if (Instruction *i = dyn_cast<Instruction>(U)) {
          insertInstructionValueIssueCycle(U, InstructionIssueCycle+1/*???*/);
          
          //}
        }
#endif
      }
        
        break;
      case Instruction::PHI:
#ifndef INTERPRETER
        InstructionIssueCycle = max(max(InstructionFetchCycle,BasicBlockBarrier),getInstructionValueIssueCycle(&I)); // This is the branch instrucion
#ifdef DEBUG_PHI_NODE
        DEBUG(dbgs() << "Executing PHI Node. Iterate over it uses.\n");
#endif
        // Iterate through the uses of the PHI node
        for (User *U : I.users()) {
          //        if (Instruction *i = dyn_cast<Instruction>(U)) {
#ifdef DEBUG_PHI_NODE
          DEBUG(dbgs() << "Use of the PHI node " << U << "\n");
#endif
          if (dyn_cast<PHINode>(U)) {
            insertInstructionValueIssueCycle(U, InstructionIssueCycle, true);
          }else{
            insertInstructionValueIssueCycle(U, InstructionIssueCycle);
          }
          
          // insertInstructionValueIssueCycle(*i, InstructionIssueCycle);
          //}
        }
        
#endif
        break;
        // Dependences through the arguments of a method call
      case Instruction::Call:
        CS = CallSite(&I);
        F = CS.getCalledFunction();
        // Loop over the arguments of the called function --- From Execution.cpp
        NumArgs = CS.arg_size();
        ArgVals.reserve(NumArgs);
        for (CallSite::arg_iterator i = CS.arg_begin(),
             e = CS.arg_end(); i != e; ++i) {
          Value *V = *i;
          ArgVals.push_back(V);
        }
        InstructionIssueCycle =max(max(InstructionFetchCycle,BasicBlockBarrier),getInstructionValueIssueCycle(&I));
        
        break;
        
        //-------------------- Memory Dependences -------------------------------//
      case Instruction::Load:
        if (InstructionType >= 0) {
          
          isLoad = true;
#ifdef INTERPRETER
          //Transform visitResult to uint64_t
          SmallString <128> StrVal;
          raw_svector_ostream OS(StrVal);
          OS << visitResult;
          // OS.str() returns StringRef
          // StringRef.str() returns the contents as a std::string
          // std::string.c_str() converts the string into the const char* required by strtol
          MemoryAddress = strtol(OS.str().str().c_str(),NULL,16);
#else
          MemoryAddress = addr;
#endif
          CacheLine = MemoryAddress >> BitsPerCacheLine;
          
#ifdef DEBUG_MEMORY_TRACES
          DEBUG(dbgs() << "MemoryAddress " << MemoryAddress << "\n");
          DEBUG(dbgs() << "CacheLine " << CacheLine << "\n");
#endif
          
          LoadCacheLine = CacheLine;
          Info = getCacheLineInfo(LoadCacheLine);
          
          //Code for reuse calculation
          Distance =  ReuseDistance(Info.LastAccess, TotalInstructions, CacheLine);
          updateReuseDistanceDistribution(Distance, InstructionIssueCycle);
          
          // Get the new instruction type depending on the reuse distance
          //ExtendedInstructionType = GetMemoryInstructionType(Distance, MemoryAddress);
          ExtendedInstructionType = GetExtendedInstructionType(Instruction::Load, Distance);
          ExecutionResource = ExecutionUnit[ExtendedInstructionType];
          Latency =ExecutionUnitsLatency[ExecutionResource];
          // UpdateInstructionCount(InstructionType,ExtendedInstructionType, NElementsVector, IsVectorInstruction);
          if (IsVectorInstruction && ExecutionResource!= FP_SHUFFLE){
            InstructionsCount[InstructionType]=InstructionsCount[InstructionType]+NElementsVector;
          }else
            InstructionsCount[InstructionType]++;
          
#ifdef DEBUG_REUSE_DISTANCE
          DEBUG(dbgs() << "ExtendedInstructionType " << ExtendedInstructionType << "\n");
          DEBUG(dbgs() << "Load latency "<< Latency	 << "\n");
#endif
          
          InstructionIssueFetchCycle = InstructionFetchCycle;
          
          if (ExtendedInstructionType >= L1_LOAD_NODE)
            InstructionIssueCacheLineAvailable = Info.IssueCycle;
          
          insertCacheLineLastAccess(LoadCacheLine, TotalInstructions);
          
#ifdef MOO_BUFFERS
          //Calculate issue cycle depending on buffer Occupancy.
          if (LoadBufferSize > 0) {
            
            if (node_size(LoadBufferCompletionCyclesTree) == LoadBufferSize) {
              // if (LoadBufferCompletionCycles.size() == LoadBufferSize) { // If the load buffer is full
              
              InstructionIssueLoadBufferAvailable = FindIssueCycleWhenLoadBufferTreeIsFull();
              //              InstructionIssueLoadBufferAvailable = FindIssueCycleWhenLoadBufferIsFull();
              
              // If, moreover, the instruction has to go to the LineFillBuffer...
              if (ExtendedInstructionType >= L2_LOAD_NODE && LineFillBufferSize > 0) {
                if (LineFillBufferCompletionCycles.size() == (unsigned)LineFillBufferSize) {
                  InstructionIssueLineFillBufferAvailable = FindIssueCycleWhenLineFillBufferIsFull();
                }
              }
            }else{ // If the Load Buffer is not fulll...
              if (ExtendedInstructionType >= L2_LOAD_NODE && LineFillBufferSize > 0) { // If it has to go to the LFS...
                
                if (LineFillBufferCompletionCycles.size() == LineFillBufferSize || !DispatchToLineFillBufferQueue.empty() ) {
                  InstructionIssueLineFillBufferAvailable = FindIssueCycleWhenLineFillBufferIsFull();
                }else{ // There is space on both
                  // Do nothing -> Instruction Issue Cycle is not affected by LB or LFB Occupancy
                  //Later, insert in both
                }
              }else{ // It does not have to go to LFB...
                //Do nothing (insert into LoadBuffer later, afte knowing IssueCycle
                // depending on BW availability. Right not IssueCycle is not affected)
              }
            }
          }
          
#endif
          // If there is Load buffer, the instruction can be dispatched as soon as
          // the buffer is available. Otherwise, both the AGU and the execution resource
          // must be available
          InstructionIssueDataDeps = getMemoryAddressIssueCycle(MemoryAddress);
          
          // New for memory model
          if (x86MemoryModel)
            InstructionIssueMemoryModel = LastLoadIssueCycle;
          
          InstructionIssueCycle = max(max(max(max(max(InstructionIssueFetchCycle, InstructionIssueLoadBufferAvailable),
                                                  InstructionIssueLineFillBufferAvailable),
                                              InstructionIssueDataDeps), InstructionIssueCacheLineAvailable),
                                      InstructionIssueMemoryModel);
          
          // Once all previous constraints have been satisfied, check AGU availability, if any
          
          DEBUG(dbgs() << "*********** Checking availability in AGUs *******************\n");
          
          //First, check in dedicated AGUs.
          if (nLoadAGUs > 0) {
            InstructionIssueLoadAGUAvailable = FindNextAvailableIssueCycle(InstructionIssueCycle, LOAD_ADDRESS_GENERATION_UNIT, LOAD_AGU_NODE);
            
          }
          
          // Check in shared (loads/stores) AGUs if any, and if there is no available in
          // dedicated AGU
          if (!(nLoadAGUs > 0 && InstructionIssueLoadAGUAvailable == InstructionIssueCycle) &&  nAGUs > 0) {
            InstructionIssueAGUAvailable = FindNextAvailableIssueCycle(InstructionIssueCycle, ADDRESS_GENERATION_UNIT, AGU_NODE);
          }
          
          // Insert but check that there are AGUs.
          if (nLoadAGUs > 0 && InstructionIssueLoadAGUAvailable >= InstructionIssueAGUAvailable) {
            InsertNextAvailableIssueCycle(InstructionIssueLoadAGUAvailable, LOAD_ADDRESS_GENERATION_UNIT, LOAD_AGU_NODE);
          }else{
            if (nAGUs > 0) {
              InsertNextAvailableIssueCycle(InstructionIssueAGUAvailable, ADDRESS_GENERATION_UNIT,AGU_NODE);
            }
          }
          
          //Store specific AGU
          if (nLoadAGUs > 0) {
            InstructionIssueCycle = max(InstructionIssueCycle, min(InstructionIssueAGUAvailable, InstructionIssueLoadAGUAvailable));
          }else{
            InstructionIssueCycle = max(InstructionIssueCycle, InstructionIssueAGUAvailable);
          }
          
          
          DEBUG(dbgs() << "*********** Checking availability in Ports *******************\n");
          
          unsigned Port = 0;
          
          if (LoadBufferSize>0) {
            // Find next available issue cycle in the port as soon as the store
            // buffer is available (and all previous constraints have been satisfied)
            InstructionIssuePortAvailable = InstructionIssueCycle;
            DEBUG(dbgs () << "ExtendedInstructionType " << NodesNames[ExtendedInstructionType] << "\n");
            DEBUG(dbgs () << "ExecutionResource " << ResourcesNames[ExecutionResource] << "\n");
            for (unsigned i = 0; i < DispatchPort[ExtendedInstructionType].size(); i++) {
              DEBUG(dbgs() << "Finding availability in port " << ExecutionPort[DispatchPort[ExtendedInstructionType][i]] << "\n");
              InstructionIssuePortAvailable = FindNextAvailableIssueCycle(InstructionIssueCycle, ExecutionPort[DispatchPort[ExtendedInstructionType][i]], PortNodes[DispatchPort[ExtendedInstructionType][i]]);
              
              if (InstructionIssuePortAvailable!= InstructionIssueCycle){
                if (i==0) {
                  InstructionIssueCycleFirstTimeAvailable = InstructionIssuePortAvailable;
                }else{
                  InstructionIssuePortAvailable = min(InstructionIssuePortAvailable, InstructionIssueCycleFirstTimeAvailable);
                  if (InstructionIssuePortAvailable == InstructionIssueCycleFirstTimeAvailable) {
                    Port = i;
                    if (InstructionIssuePortAvailable == InstructionIssueCycle)
                      break;
                  }
                }
              }else{
                // If Node is NULL, it is available for sure.
                DEBUG(dbgs() << "Port is "<<DispatchPort[ExtendedInstructionType][i] <<"\n");
                Port = i;
                break;
              }
            }
            
            InsertNextAvailableIssueCycle(InstructionIssuePortAvailable, ExecutionPort[DispatchPort[ExtendedInstructionType][Port]],PortNodes[DispatchPort[ExtendedInstructionType][Port]]);
            
            InstructionIssueCycle = max(InstructionIssueCycle, InstructionIssuePortAvailable);
            
            // Finally, find and insert issue cycle in the corresponding store resource
            DEBUG(dbgs() << "*********** Checking availability in Resource *******************\n");
            
            InstructionIssueThroughputAvailable = FindNextAvailableIssueCycle(InstructionIssueCycle, ExecutionResource, ExtendedInstructionType);
            InsertNextAvailableIssueCycle(InstructionIssueThroughputAvailable, ExecutionResource,ExtendedInstructionType);
          }else{
            //If there is no load buffer, there must be available cycle in both, the dispatch port
            // and the resource
            InstructionIssueThroughputAvailable = FindNextAvailableIssueCyclePortAndThroughtput(InstructionIssueCycle,ExtendedInstructionType, NElementsVector);
          }
          InstructionIssueCycle = max(InstructionIssueCycle, InstructionIssueThroughputAvailable);
          
#ifdef DEBUG_ISSUE_CYCLE
          DEBUG(dbgs() << "======== Instruction Issue Cycle (fetch cycle)"<< InstructionIssueFetchCycle	 << "========\n");
          DEBUG(dbgs() << "======== Instruction Issue Cycle (LB availability)"<< InstructionIssueLoadBufferAvailable	 << "========\n");
          DEBUG(dbgs() << "======== Instruction Issue Cycle (LFB availability)"<< InstructionIssueLineFillBufferAvailable	 << "========\n");
          DEBUG(dbgs() << "======== Instruction Issue Cycle (cache line available)"<< InstructionIssueCacheLineAvailable	 << "========\n");
          DEBUG(dbgs() << "======== Instruction Issue Cycle (data deps)"<< 	InstructionIssueDataDeps << "========\n");
          DEBUG(dbgs() << "======== Instruction Issue Cycle (memory model Principle 1) "<< InstructionIssueMemoryModel	 << "========\n");
          DEBUG(dbgs() << "======== Instruction Issue Cycle (AGU Availability)"<< InstructionIssueAGUAvailable	 << "========\n");
          DEBUG(dbgs() << "======== Instruction Issue Cycle (Port Availability)"<< InstructionIssuePortAvailable	 << "========\n");
          DEBUG(dbgs() << "======== Instruction Issue Cycle (Throughput Availability)"<< InstructionIssueThroughputAvailable	 << "========\n");
          DEBUG(dbgs() << "__________________Instruction Issue Cycle "<< InstructionIssueCycle << "__________________\n");
#endif
        }
        
        break;
        // The Store can execute as soon as the value being stored is calculated
      case Instruction::Store:
        if (InstructionType >= 0) {
          isLoad = false;
#ifdef INTERPRETER
          SmallString <128> StrVal;
          raw_svector_ostream OS(StrVal);
          OS << visitResult;
          MemoryAddress = strtol(OS.str().str().c_str(),NULL,16);
#else
          MemoryAddress = addr;
#endif
          CacheLine = MemoryAddress >> BitsPerCacheLine;
          
          StoreCacheLine = CacheLine;
          LastAccess = getCacheLineLastAccess(StoreCacheLine);
          Info = getCacheLineInfo(StoreCacheLine);
          Distance = ReuseDistance(LastAccess, TotalInstructions, CacheLine);
          ExtendedInstructionType = GetExtendedInstructionType(Instruction::Store, Distance);
          ExecutionResource = ExecutionUnit[ExtendedInstructionType];
          Latency =ExecutionUnitsLatency[ExecutionResource];
          // Do not update anymore, udpate in InsertNextAvailableIssueCycle so that
          // ports are also updated. But need to update the InstructionsCount
          // UpdateInstructionCount(InstructionType, ExtendedInstructionType, NElementsVector, IsVectorInstruction);
          if (IsVectorInstruction && ExecutionResource!= FP_SHUFFLE){
            InstructionsCount[InstructionType]=InstructionsCount[InstructionType]+NElementsVector;
          }else
            InstructionsCount[InstructionType]++;
          
#ifdef DEBUG_MEMORY_TRACES
          DEBUG(dbgs() << "MemoryAddress " << MemoryAddress << "\n");
          DEBUG(dbgs() << "CacheLine " << CacheLine << "\n");
#endif
#ifdef DEBUG_REUSE_DISTANCE
          DEBUG(dbgs() << "Store Last Access "<< LastAccess	 << "\n");
          DEBUG(dbgs() << "Store latency "<< Latency	 << "\n");
#endif
          
          InstructionIssueFetchCycle = InstructionFetchCycle;
          
          if (ExtendedInstructionType >= L1_STORE_NODE)
            InstructionIssueCacheLineAvailable = Info.IssueCycle;
          
          InstructionIssueDataDeps = getInstructionValueIssueCycle(&I);
          
#ifdef MOO_BUFFERS
          //Calculate issue cycle depending on buffer Occupancy.
          if (StoreBufferSize > 0) {
            if (StoreBufferCompletionCycles.size() == StoreBufferSize) { // If the store buffer is full
              InstructionIssueStoreBufferAvailable = FindIssueCycleWhenStoreBufferIsFull();
              // If, moreover, the instruction has to go to the LineFillBuffer...
              if (ExtendedInstructionType >= L2_LOAD_NODE && LineFillBufferSize > 0) {
                if (LineFillBufferCompletionCycles.size() == (unsigned)LineFillBufferSize) {
                  InstructionIssueLineFillBufferAvailable = FindIssueCycleWhenLineFillBufferIsFull();
                }
              }
              
            }else{ // If the Store Buffer is not fulll...
              if (ExtendedInstructionType >= L2_LOAD_NODE && LineFillBufferSize > 0) { // If it has to go to the LFS...
                
                if (LineFillBufferCompletionCycles.size() == LineFillBufferSize || !DispatchToLineFillBufferQueue.empty() ) {
                  InstructionIssueLineFillBufferAvailable = FindIssueCycleWhenLineFillBufferIsFull();
                }else{ // There is space on both
                  // Do nothing -> Instruction Issue Cycle is not affected by LB or LFB Occupancy
                  //Later, insert in both
                }
              }else{ // It does not have to go to LFB...
                //Do nothing (insert into LoadBuffer later, afte knowing IssueCycle
                // depending on BW availability. Right not IssueCycle is not affected)
              }
            }
          }
#endif
          
          // New for memory model
          if (x86MemoryModel) {
            // Writes are not reordered with other writes
            DEBUG(dbgs() << "LastStoreIssueCycle " << LastStoreIssueCycle << "\n");
            DEBUG(dbgs() << "LastLoadIssueCycle " << LastLoadIssueCycle << "\n");
            InstructionIssueMemoryModel = LastStoreIssueCycle;
            // Writes are not reordered with earlier reads
            // The memory-ordering model ensures that a store by a processor may not occur before a previous load by the same processor.
            InstructionIssueMemoryModel = max(InstructionIssueMemoryModel, LastLoadIssueCycle);
          }
          
          InstructionIssueCycle = max(max(max(max(InstructionIssueFetchCycle, InstructionIssueStoreBufferAvailable), InstructionIssueDataDeps), InstructionIssueCacheLineAvailable),InstructionIssueMemoryModel);
          
          // Once all previous constraints have been satisfied, check AGU availability, if any
          DEBUG(dbgs() << "*********** Checking availability in AGUs *******************\n");
          
          //First, check in dedicated AGUs.
          if (nStoreAGUs > 0) {
            InstructionIssueStoreAGUAvailable = FindNextAvailableIssueCycle(InstructionIssueCycle, STORE_ADDRESS_GENERATION_UNIT, STORE_AGU_NODE);
          }
          
          // Check in shared (loads/stores) AGUs if any, and if there is no available in
          // dedicated AGU
          if (!(nStoreAGUs > 0 && InstructionIssueStoreAGUAvailable == InstructionIssueCycle) &&  nAGUs > 0) {
            InstructionIssueAGUAvailable = FindNextAvailableIssueCycle(InstructionIssueCycle, ADDRESS_GENERATION_UNIT, AGU_NODE);
          }
          
          // Insert but check that there are AGUs.
          if (nStoreAGUs > 0 && InstructionIssueStoreAGUAvailable >= InstructionIssueAGUAvailable) {
            InsertNextAvailableIssueCycle(InstructionIssueStoreAGUAvailable, STORE_ADDRESS_GENERATION_UNIT,  STORE_AGU_NODE);
          }else{
            if (nAGUs > 0) {
              InsertNextAvailableIssueCycle(InstructionIssueAGUAvailable, ADDRESS_GENERATION_UNIT,AGU_NODE);
            }
          }
          
          //Store specific AGU
          
          if (nStoreAGUs > 0) {
            InstructionIssueCycle = max(InstructionIssueCycle, min(InstructionIssueAGUAvailable, InstructionIssueStoreAGUAvailable));
          }else{
            InstructionIssueCycle = max(InstructionIssueCycle, InstructionIssueAGUAvailable);
          }
          
          
          // When a cache line is writen does not impact when in can be loaded again.
          updateReuseDistanceDistribution(Distance, InstructionIssueCycle);
          //insertCacheLineInfo(StoreCacheLine, Info);
          insertCacheLineLastAccess(StoreCacheLine, TotalInstructions );
          
          DEBUG(dbgs() << "CacheLine " << CacheLine << "\n");
          DEBUG(dbgs() << "StoreCacheLine " << StoreCacheLine << "\n");
          
          
          DEBUG(dbgs() << "*********** Checking availability in Ports *******************\n");
          // If there is a store buffer, the dipatch cycle might be different from
          // the issue (execution) cycle.
          unsigned Port = 0;
          if (StoreBufferSize>0) {
            // Find next available issue cycle in the port as soon as the store
            // buffer is available (and all previous constraints have been satisfied)
            InstructionIssuePortAvailable = InstructionIssueCycle;
            DEBUG(dbgs () << "ExtendedInstructionType " << NodesNames[ExtendedInstructionType] << "\n");
            DEBUG(dbgs () << "ExecutionResource " << ResourcesNames[ExecutionResource] << "\n");
            for (unsigned i = 0; i < DispatchPort[ExtendedInstructionType].size(); i++) {
              DEBUG(dbgs() << "Finding availability in port " << ResourcesNames[ExecutionPort[DispatchPort[ExtendedInstructionType][i]]] << "\n");
              InstructionIssuePortAvailable = FindNextAvailableIssueCycle(InstructionIssueCycle, ExecutionPort[DispatchPort[ExtendedInstructionType][i]], PortNodes[DispatchPort[ExtendedInstructionType][i]]);
              if (InstructionIssuePortAvailable!= InstructionIssueCycle){
                if (i==0) {
                  InstructionIssueCycleFirstTimeAvailable = InstructionIssuePortAvailable;
                }else{
                  InstructionIssuePortAvailable = min(InstructionIssuePortAvailable, InstructionIssueCycleFirstTimeAvailable);
                  if (InstructionIssuePortAvailable == InstructionIssueCycleFirstTimeAvailable) {
                    Port = i;
                    if (InstructionIssuePortAvailable == InstructionIssueCycle)
                      break;
                  }
                }
              }else{
                // If Node is NULL, it is available for sure.
                DEBUG(dbgs() << "Port is "<<DispatchPort[ExtendedInstructionType][i] <<"\n");
                Port = i;
                break;
              }            }
            
            
            InsertNextAvailableIssueCycle(InstructionIssuePortAvailable, ExecutionPort[DispatchPort[ExtendedInstructionType][Port]],PortNodes[DispatchPort[ExtendedInstructionType][Port]]);
            
            InstructionIssueCycle = max(InstructionIssueCycle, InstructionIssuePortAvailable);
            
            // Finally, calculate issue cycle depending on resource availability
            DEBUG(dbgs() << "*********** Checking availability in Resource *******************\n");
            
            InstructionIssueThroughputAvailable = FindNextAvailableIssueCycle(InstructionIssueCycle, ExecutionResource, ExtendedInstructionType);
            
            InsertNextAvailableIssueCycle(InstructionIssueThroughputAvailable, ExecutionResource,ExtendedInstructionType);
            
          }else{
            InstructionIssueThroughputAvailable= FindNextAvailableIssueCyclePortAndThroughtput(InstructionIssueCycle,ExtendedInstructionType, NElementsVector);
          }
          InstructionIssueCycle = max(InstructionIssueCycle, InstructionIssueThroughputAvailable);
#ifdef DEBUG_ISSUE_CYCLE
          DEBUG(dbgs() << "======== Instruction Issue Cycle (fetch cycle) "<< InstructionIssueFetchCycle	 << " ========\n");
          DEBUG(dbgs() << "======== Instruction Issue Cycle (SB availability) "<< InstructionIssueStoreBufferAvailable	 << " ========\n");
          DEBUG(dbgs() << "======== Instruction Issue Cycle (AGU Availability) "<< InstructionIssueAGUAvailable	 << " ========\n");
          DEBUG(dbgs() << "======== Instruction Issue Cycle (cache line available) "<< InstructionIssueCacheLineAvailable	 << " ========\n");
          DEBUG(dbgs() << "======== Instruction Issue Cycle (data deps)"<< 	InstructionIssueDataDeps << " ========\n");
          DEBUG(dbgs() << "======== Instruction Issue Cycle (memory model Principles 2 and 3) "<< InstructionIssueMemoryModel	 << " ========\n");
          DEBUG(dbgs() << "======== Instruction Issue Cycle (AGU Availability) "<< InstructionIssueAGUAvailable	 << "========\n");
          DEBUG(dbgs() << "======== Instruction Issue Cycle (Port Availability) "<< InstructionIssuePortAvailable	 << "========\n");
          DEBUG(dbgs() << "======== Instruction Issue Cycle (Throughput Availability) "<< InstructionIssueThroughputAvailable	 << "========\n");
          DEBUG(dbgs() << "__________________Instruction Issue Cycle "<< InstructionIssueCycle << "__________________\n");
#endif
        }
        break;
        
      case  Instruction::Ret:
        // Determine the uses of the returned value outside the funcion
        //(i.e., the uses of the calling function)
        // Check http://llvm.org/docs/ProgrammersManual.html for a lot
        // of info about how iterate through functions, bbs, etc.
        F = I.getParent()->getParent();
        InstructionIssueCycle = max(max(InstructionFetchCycle,BasicBlockBarrier),getInstructionValueIssueCycle(&I));
#ifdef INTERPRETER
        for (Value::use_iterator IT = F->use_begin(), ET = F->use_end(); IT != ET; ++IT) {
          // Iterate over the users of the uses of the function
          for(Value::use_iterator it = (*IT)->use_begin(), ite = (*IT)->use_end(); it!=ite; ++it){
            insertInstructionValueIssueCycle(*it, InstructionIssueCycle);
          }
        }
#else
        
        for (User *U : F->users()) {
          //  if (Instruction *Inst = dyn_cast<Instruction>(U)) {
          for (User *UI : U->users()) {
            // if (Instruction *i = dyn_cast<Instruction>(UI)) {
            insertInstructionValueIssueCycle(UI, InstructionIssueCycle);
            
            // }
          }
          //}
        }
#endif
        break;
        
        //-------------------------General case------------------------------//
      default:
        if (InstructionType == 0) {
          OriginalInstructionIssueCycle = getInstructionValueIssueCycle(&I);
          InstructionIssueCycle = max(max(InstructionFetchCycle,BasicBlockBarrier),OriginalInstructionIssueCycle);
          ExtendedInstructionType = GetExtendedInstructionType(I.getOpcode());
          // UpdateInstructionCount(InstructionType, ExtendedInstructionType,  NElementsVector, IsVectorInstruction);
          if (IsVectorInstruction && ExecutionResource!= FP_SHUFFLE){
            InstructionsCount[InstructionType]=InstructionsCount[InstructionType]+NElementsVector;
          }else
            InstructionsCount[InstructionType]++;
          
          Latency = ExecutionUnitsLatency[ExtendedInstructionType];
          
          if (InstructionIssueCycle > OriginalInstructionIssueCycle) {
            NInstructionsStalled[ExtendedInstructionType]++;
          }
          
          DEBUG(dbgs() << "*********** Checking availability in Resource *******************\n");
          
          InstructionIssueThroughputAvailable= FindNextAvailableIssueCyclePortAndThroughtput(InstructionIssueCycle,ExtendedInstructionType, NElementsVector);
          
          InstructionIssueCycle= max(InstructionIssueCycle, InstructionIssueThroughputAvailable);
#ifdef DEBUG_ISSUE_CYCLE
          DEBUG(dbgs() << "========Original Instruction Issue Cycle (data deps)"<< OriginalInstructionIssueCycle	 << "========\n");
          DEBUG(dbgs() << "========Original Instruction Issue Cycle (fetch cycle) "<< InstructionFetchCycle	 << "========\n");
          DEBUG(dbgs() << "__________________Instruction Issue Cycle "<< InstructionIssueCycle << "__________________\n");
#endif
        }
        break;
    }
    
    
    
    
    if (InstructionType >= 0) {
      
      uint64_t NewInstructionIssueCycle = InstructionIssueCycle;
      
      //======================= NEW CODE ==========================================//
      // If Available tree is empty or IssueCycle > InstructionsLastIssueCycle,
      // then insert available cycle with Occupancy 1
      if (x86MemoryModel) {
        if (I.getOpcode() ==Instruction::Load) {
          LastLoadIssueCycle = NewInstructionIssueCycle;
        }else{
          if (I.getOpcode() ==Instruction::Store)
            LastStoreIssueCycle = NewInstructionIssueCycle;
        }
      }
      
      // A load can execute as soon as all its operands are available, i.e., all
      // the values that are being loaded. If the same value is loaded again, without
      // having been used in between (Read After Read dependence), then the next load
      // can only be issued after the first one has finished.
      // This only applies to memory accesses > L1. If we access a cache line at cycle
      // X which is in L3, e.g., it has a latency of 30. The next  time this cache line
      // is accessed it is in L1, but it is inconsistent to assume that it can be
      // loaded also at cycle X and have a latency of 4 cycles.
      
      ExecutionResource=ExecutionUnit[ExtendedInstructionType];
      
      if (I.getOpcode() ==Instruction::Load && RARDependences && ExtendedInstructionType > L1_LOAD_NODE &&
          ExecutionUnitsLatency[ExecutionResource] > ExecutionUnitsLatency[L1_LOAD_CHANNEL]){
        // if (Distance < 0) {
        Info = getCacheLineInfo(LoadCacheLine);
        Info.IssueCycle = NewInstructionIssueCycle+Latency;
        insertCacheLineInfo(LoadCacheLine, Info);
        // }else
        insertMemoryAddressIssueCycle(MemoryAddress, NewInstructionIssueCycle+Latency);
      }
      
      if (I.getOpcode() == Instruction::Store && ExtendedInstructionType > L1_STORE_NODE
          && ExecutionUnitsLatency[ExecutionResource] > ExecutionUnitsLatency[L1_LOAD_CHANNEL]) {
        //   if (Distance < 0 ){
        DEBUG(dbgs() << "Inserting issue cycle " << NewInstructionIssueCycle+Latency << " for cache line " << StoreCacheLine << "\n");
        Info = getCacheLineInfo(StoreCacheLine);
        Info.IssueCycle = NewInstructionIssueCycle+Latency;
        insertCacheLineInfo(StoreCacheLine, Info);
        //  }else
        insertMemoryAddressIssueCycle(MemoryAddress, NewInstructionIssueCycle+Latency);
      }
      
      // =========================== SPATIAL PREFETCHER ======================================
      
      if (SpatialPrefetcher && (I.getOpcode() ==Instruction::Load || I.getOpcode() ==Instruction::Store)&& ExtendedInstructionType > PrefetchDispatch && !(ExecutionUnit[ExtendedInstructionType] == PrefetchLevel)/*&& (CacheLine %2) == 0*/
          /*  && (ExtendedInstructionType == MEM_LOAD_NODE || ExtendedInstructionType == MEM_STORE_NODE )*/) {
        NextCacheLine = CacheLine+1;
        
        //Get reuse distance of NextCacheLine
        Info = getCacheLineInfo(NextCacheLine);
        Distance =  ReuseDistance(Info.LastAccess, TotalInstructions, NextCacheLine, true);
        NextCacheLineExtendedInstructionType = GetMemoryInstructionType(Distance, MemoryAddress);
        
        
        ExecutionResource = ExecutionUnit[NextCacheLineExtendedInstructionType];
        LatencyPrefetch =  ExecutionUnitsLatency[ExecutionResource]-ExecutionUnitsLatency[PrefetchDestination];
        
        
#ifdef DEBUG_PREFETCHER
        DEBUG(dbgs() << "CacheLine " << CacheLine << "\n");
        DEBUG(dbgs() << "NextCacheLine " << NextCacheLine << "\n");
        DEBUG(dbgs() << "Execution Resource (for bandwidth consumption) " << ResourcesNames[ExecutionResource] << "\n");
        DEBUG(dbgs() << "Latency (for bandwidth consumption) " << LatencyPrefetch << "\n");
        
#endif
        
        // Prefetch every time there is a miss (not necesarly an access to memory), but only if the prefetched
        //data is in memoty.
        if (ExecutionResource > PrefetchTarget && ExecutionResource >= PrefetchDestination) {
          InstructionsCountExtended[NextCacheLineExtendedInstructionType]++;
          if (IsVectorInstruction && ExecutionResource!= FP_SHUFFLE){
            InstructionsCount[InstructionType]=InstructionsCount[InstructionType]+NElementsVector;
          }else
            InstructionsCount[InstructionType]++;
          // UpdateReuseDistribution
          NextCacheLineIssueCycle = FindNextAvailableIssueCycle(NewInstructionIssueCycle, ExecutionResource, NextCacheLineExtendedInstructionType);
          
          updateReuseDistanceDistribution(Distance, NextCacheLineIssueCycle);
          
#ifdef DEBUG_PREFETCHER
          DEBUG(dbgs() << "Prefetching next cache line at cycle " << NextCacheLineIssueCycle << "\n");
#endif
          
          InsertNextAvailableIssueCycle(NextCacheLineIssueCycle, ExecutionResource, NextCacheLineExtendedInstructionType, 1, true);
          
          Info.IssueCycle = NextCacheLineIssueCycle+LatencyPrefetch;
          Info.LastAccess = TotalInstructions;
          insertCacheLineInfo(NextCacheLine, Info);
          DEBUG(dbgs() << "Inserting issue cycle " <<  NextCacheLineIssueCycle+LatencyPrefetch << " for cache line " << NextCacheLine << "\n");
          
        }
      }
      //======================= END NEW CODE ==========================================//
      
      //Iterate over the uses of the generated value (except for GetElementPtr)
      if(I.getOpcode() != Instruction::GetElementPtr){
#ifdef INTERPRETER
        for(Value::use_iterator i = I.use_begin(), ie = I.use_end(); i!=ie; ++i){
#ifdef DEBUG_DEPS_FUNCTION_CALL
          DEBUG(dbgs() << "Setting use  "<< *i << " to "<<  NewInstructionIssueCycle+Latency<<"\n");
#endif
          if (dyn_cast<PHINode>(*i)) {
            insertInstructionValueIssueCycle(*i, NewInstructionIssueCycle+Latency, true);
          }else{
            insertInstructionValueIssueCycle(*i, NewInstructionIssueCycle+Latency);
          }
          if(dyn_cast<CallInst>(*i)){
#ifdef DEBUG_DEPS_FUNCTION_CALL
            DEBUG(dbgs() << "The use is a call to function\n");
#endif
            CS = CallSite(*i);
            F = CS.getCalledFunction();
            // Loop over the arguments of the called function --- From Execution.cpp
            NumArgs = CS.arg_size();
            ArgVals.reserve(NumArgs);
            for (CallSite::arg_iterator j = CS.arg_begin(),
                 e = CS.arg_end(); j != e; ++j) {
              Value *V = *j;
              ArgVals.push_back(V);
            }
            
            // Make sure it is an LLVM-well-defined funciton
            if (static_cast<Function*>(F)) {
              for (Function::arg_iterator AI = F->arg_begin(), E = F->arg_end();
                   AI != E; ++AI, ++k){
#ifdef DEBUG_DEPS_FUNCTION_CALL
                DEBUG(dbgs() << "Iterate through the arguments of the call\n");
#endif
                /*   Latency = (&AI->getOpcode() == Instruction::Load || (ArgVals[j])->getOpcode()
                 == Instruction::Store)? MEM_LATENCY:FLOP_LATENCY;*/
#ifdef DEBUG_DEPS_FUNCTION_CALL
                DEBUG(dbgs() << "Argument "<< ArgVals[k] <<"\n");
#endif
                /* //Iterate through the uses of the argument?
                 insertInstructionValueIssueCycle(AI, InstructionIssueCycle+1); // +1???
                 */
#ifdef DEBUG_DEPS_FUNCTION_CALL
                DEBUG(dbgs() << "Iterate through the uses of the argument\n");
#endif
                if( ArgVals[k] == &I){
#ifdef DEBUG_DEPS_FUNCTION_CALL
                  DEBUG(dbgs() << "Argument equal to current instruction\n");
#endif
                  for(Value::use_iterator vi = (*AI).use_begin(), vie = (*AI).use_end(); vi!=vie; ++vi){
#ifdef DEBUG_DEPS_FUNCTION_CALL
                    DEBUG(dbgs() << "Use of the argument "<< *vi <<"\n");
#endif
                    insertInstructionValueIssueCycle(*vi,NewInstructionIssueCycle+Latency);
                  }
                }
              }
            }
          }
        }
#else
        // No interpreter: new way of iterating through the uses of an instruction.
        // TODO: Fix also iterating through the arguments
        for (User *U : I.users()) {
          //  if (Instruction *i = dyn_cast<Instruction>(U)) {
          // for(Value::use_iterator i = I.use_begin(), ie = I.use_end(); i!=ie; ++i){
          
#ifdef DEBUG_DEPS_FUNCTION_CALL
          DEBUG(dbgs() << "Setting use  "<< U << " to "<<  NewInstructionIssueCycle+Latency<<"\n");
#endif
          if (dyn_cast<PHINode>(U)) {
            insertInstructionValueIssueCycle(U, NewInstructionIssueCycle+Latency, true);
          }else{
            insertInstructionValueIssueCycle(U, NewInstructionIssueCycle+Latency);
          }
          if(dyn_cast<CallInst>(U)){
#ifdef DEBUG_DEPS_FUNCTION_CALL
            DEBUG(dbgs() << "The use is a call to function\n");
#endif
            CS = CallSite(U);
            F = CS.getCalledFunction();
            // Loop over the arguments of the called function --- From Execution.cpp
            NumArgs = CS.arg_size();
            ArgVals.reserve(NumArgs);
            for (CallSite::arg_iterator j = CS.arg_begin(),
                 e = CS.arg_end(); j != e; ++j) {
              Value *V = *j;
              ArgVals.push_back(V);
            }
            
            // Make sure it is an LLVM-well-defined funciton
            if (static_cast<Function*>(F)) {
              for (Function::arg_iterator AI = F->arg_begin(), E = F->arg_end();
                   AI != E; ++AI, ++k){
#ifdef DEBUG_DEPS_FUNCTION_CALL
                DEBUG(dbgs() << "Iterate through the arguments of the call\n");
#endif
                /*   Latency = (&AI->getOpcode() == Instruction::Load || (ArgVals[j])->getOpcode()
                 == Instruction::Store)? MEM_LATENCY:FLOP_LATENCY;*/
#ifdef DEBUG_DEPS_FUNCTION_CALL
                DEBUG(dbgs() << "Argument "<< ArgVals[k] <<"\n");
#endif
                /* //Iterate through the uses of the argument?
                 insertInstructionValueIssueCycle(AI, InstructionIssueCycle+1); // +1???
                 */
#ifdef DEBUG_DEPS_FUNCTION_CALL
                DEBUG(dbgs() << "Iterate through the uses of the argument\n");
#endif
                if( ArgVals[k] == &I){
#ifdef DEBUG_DEPS_FUNCTION_CALL
                  DEBUG(dbgs() << "Argument equal to current instruction\n");
#endif
                  for(Value::use_iterator vi = (*AI).use_begin(), vie = (*AI).use_end(); vi!=vie; ++vi){
#ifdef DEBUG_DEPS_FUNCTION_CALL
                    DEBUG(dbgs() << "Use of the argument "<< *vi <<"\n");
#endif
                    insertInstructionValueIssueCycle(*vi,NewInstructionIssueCycle+Latency);
                  }
                }
              }
            }
          }
          // }
        }
#endif
      }
      
      //========================= Update Parallelism Distribution ===================//
      
#ifdef ILP_DISTRIBUTION
      if (InstructionType >= 0) { // Consider only selected instructions types
        if(ParallelismDistribution.empty()){
          for (uint i = 0; i< Latency; i++) {
            vector<unsigned> v;
            ParallelismDistribution.push_back(v);
            for (j=0; j<N_INST_TYPES; j++)
              ParallelismDistribution[InstructionIssueCycle+i].push_back(0);
          }
        }else{
          uint DistributionSize = ParallelismDistribution.size();
          if(DistributionSize <= InstructionIssueCycle+Latency-1){ // Is not empty, but distribution has to be extended
            uint extraSlots = max(InstructionIssueCycle+Latency-DistributionSize,(unsigned long long)1);
            for (uint i = 0; i< extraSlots; i++) {
              vector<unsigned> v;
              ParallelismDistribution.push_back(v);
              for (j=0; j<N_INST_TYPES; j++)
                ParallelismDistribution[DistributionSize+i].push_back(0);
            }
          }
        }
        for (uint i = 0; i< Latency; i++)
          ParallelismDistribution[InstructionIssueCycle+i][InstructionType]++;
      }
#endif
      
      //---------------------- End of Update Parallelism Distribution--------------------//
      
      // ------------ Work with limited instruction issue window ----------------//
      
      //When InstructionFetchBandwidth is INF, remaining instructions to fetch
      // is -1, but still load and stores must be inserted into the OOO buffers
      if (RemainingInstructionsFetch > 0 || RemainingInstructionsFetch == INF ) {
        
        if (RemainingInstructionsFetch > 0)
          RemainingInstructionsFetch--;
        
        uint64_t CycleInsertReservationStation =0 ;
        if (I.getOpcode() ==Instruction::Load) {
          // If LB is not full, they go directly to the LB and to the RS
          // If LB is INF, this comparison is false. But still
          // we need to check wether RS is INF
          
          if(node_size(LoadBufferCompletionCyclesTree) == LoadBufferSize && LoadBufferSize > 0){
            // if(LoadBufferCompletionCycles.size() == LoadBufferSize && LoadBufferSize > 0){
            
            // Put in the reservation station, but only if RS exists
            //   CycleInsertReservationStation = FindIssueCycleWhenLoadBufferIsFull();
            CycleInsertReservationStation = FindIssueCycleWhenLoadBufferTreeIsFull();
            ReservationStationIssueCycles.push_back(CycleInsertReservationStation);
            
            //Put in the DispatchToLoadBufferQueue
            if (DispatchToLoadBufferQueueTree == NULL) {
              MaxDispatchToLoadBufferQueueTree =CycleInsertReservationStation;
            }else{
              MaxDispatchToLoadBufferQueueTree = max(MaxDispatchToLoadBufferQueueTree,CycleInsertReservationStation );
              
            }
            
            DispatchToLoadBufferQueueTree = insert_node(NewInstructionIssueCycle+Latency,CycleInsertReservationStation, DispatchToLoadBufferQueueTree);
            
            
            /*
             InstructionDispatchInfo DispathInfo;
             DispathInfo.IssueCycle = CycleInsertReservationStation;
             DispathInfo.CompletionCycle = NewInstructionIssueCycle+Latency;
             DispatchToLoadBufferQueue.push_back(DispathInfo);
             */
#ifdef DEBUG_OOO_BUFFERS
            DEBUG(dbgs() << "Inserting  "<< NewInstructionIssueCycle << " to DispatchToLoadBufferQueue\n");
#endif
            
            // If, moreover, the instruction has to go to the LineFillBuffer...
            if (ExtendedInstructionType >= L2_LOAD_NODE && LineFillBufferSize > 0) {
              if (LineFillBufferCompletionCycles.size() == LineFillBufferSize || !DispatchToLineFillBufferQueue.empty()) {
                InstructionDispatchInfo DispathInfo;
                DispathInfo.IssueCycle = FindIssueCycleWhenLineFillBufferIsFull();
                DispathInfo.CompletionCycle = NewInstructionIssueCycle+Latency;
                DispatchToLineFillBufferQueue.push_back(DispathInfo);
#ifdef DEBUG_OOO_BUFFERS
                DEBUG(dbgs() << "Inserting  "<< DispathInfo.IssueCycle  << " to DispatchToLineFillBufferQueue\n");
#endif
              }else{ // There is space on both
#ifdef DEBUG_OOO_BUFFERS
                DEBUG(dbgs() << "Inserting  "<< NewInstructionIssueCycle+Latency << " to Line Fill Buffer\n");
#endif
                LineFillBufferCompletionCycles.push_back(NewInstructionIssueCycle+Latency);
              }
            }
          }else{
            //If LB is not full
            if (node_size(LoadBufferCompletionCyclesTree) != LoadBufferSize && LoadBufferSize > 0) {
              // if (LoadBufferCompletionCycles.size() != LoadBufferSize && LoadBufferSize > 0) {
              
#ifdef DEBUG_OOO_BUFFERS
              DEBUG(dbgs() << "Inserting  "<< NewInstructionIssueCycle+Latency << " to LoadBuffer\n");
#endif
              //Insert into LB
              // LoadBufferCompletionCycles.push_back(NewInstructionIssueCycle+Latency);
              if (node_size(LoadBufferCompletionCyclesTree) == 0) {
                MinLoadBuffer =NewInstructionIssueCycle+Latency;
              }else{
                MinLoadBuffer = min(MinLoadBuffer,NewInstructionIssueCycle+Latency);
              }
              LoadBufferCompletionCyclesTree = insert_node(NewInstructionIssueCycle+Latency, LoadBufferCompletionCyclesTree);
              if (ExtendedInstructionType >= L2_LOAD_NODE && LineFillBufferSize != 0) { // If it has to go to the LFS...
                
                if (LineFillBufferCompletionCycles.size() == LineFillBufferSize || !DispatchToLineFillBufferQueue.empty()) {
                  InstructionDispatchInfo DispathInfo;
                  DispathInfo.IssueCycle = FindIssueCycleWhenLineFillBufferIsFull();
                  DispathInfo.CompletionCycle = NewInstructionIssueCycle+Latency;
                  DispatchToLineFillBufferQueue.push_back(DispathInfo);
#ifdef DEBUG_OOO_BUFFERS
                  DEBUG(dbgs() << "Inserting  "<< DispathInfo.IssueCycle  << " to DispatchToLineFillBufferQueue\n");
#endif
                }else{ // There is space on both
#ifdef DEBUG_OOO_BUFFERS
                  DEBUG(dbgs() << "Inserting  "<< NewInstructionIssueCycle+Latency << " to Line Fill Buffer\n");
#endif
                  LineFillBufferCompletionCycles.push_back(NewInstructionIssueCycle+Latency);
                }
              }else{ // It does not have to go to LFB...
                // Insert into LoadBuffer, what we have already done.
              }
            }else{
              //If LB is zero.... Insert into into RS, if it exists
              if (ReservationStationSize > 0) {
                CycleInsertReservationStation = NewInstructionIssueCycle;
                ReservationStationIssueCycles.push_back(CycleInsertReservationStation);
              }
            }
          }
        }else{
          if (I.getOpcode() ==Instruction::Store){
            if (StoreBufferCompletionCycles.size()==StoreBufferSize  && StoreBufferSize > 0 ) {
              
              CycleInsertReservationStation = FindIssueCycleWhenStoreBufferIsFull();
              ReservationStationIssueCycles.push_back(CycleInsertReservationStation);
              InstructionDispatchInfo DispathInfo;
              //  DispathInfo.IssueCycle = NewInstructionIssueCycle;
              DispathInfo.IssueCycle = FindIssueCycleWhenStoreBufferIsFull();
              DispathInfo.CompletionCycle = NewInstructionIssueCycle+Latency;
              DispatchToStoreBufferQueue.push_back(DispathInfo);
#ifdef DEBUG_OOO_BUFFERS
              DEBUG(dbgs() << "Inserting  "<< NewInstructionIssueCycle << " to DispatchToStoreBufferQueue\n");
#endif
            }else{ // If it is not full
              if (StoreBufferCompletionCycles.size()!=StoreBufferSize  && StoreBufferSize > 0 ) {
#ifdef DEBUG_OOO_BUFFERS
                DEBUG(dbgs() << "Inserting  "<< NewInstructionIssueCycle+Latency << " to StoreBuffer\n");
#endif
                StoreBufferCompletionCycles.push_back(NewInstructionIssueCycle+Latency);
              }else{
                // If StoreBufferSize == 0, insert into RS if it exists
                if (ReservationStationSize > 0) {
                  CycleInsertReservationStation = NewInstructionIssueCycle;
                  ReservationStationIssueCycles.push_back(CycleInsertReservationStation);
                }
              }
            }
          }else{
            // Not load nor store -> Insert into RS if its size is > -1
            if (ReservationStationSize > 0) {
              CycleInsertReservationStation = NewInstructionIssueCycle;
              ReservationStationIssueCycles.push_back(CycleInsertReservationStation);
            }
          }
        }
        
        
        //dbgs() << "Inserting into RS\n";
        if (InOrderExecution) {
          ReorderBufferCompletionCycles.push_back(NewInstructionIssueCycle);
        }else{
          if (ReorderBufferSize > 0) {
            ReorderBufferCompletionCycles.push_back(NewInstructionIssueCycle+Latency);
          }
        }
        
#ifdef DEBUG_OOO_BUFFERS
        if (ReservationStationSize > 0 && CycleInsertReservationStation >0)
          DEBUG(dbgs() << "Inserting  "<< CycleInsertReservationStation << " to ReservationStationIssueCycles\n");
        if (ReorderBufferSize > 0)
          DEBUG(dbgs() << "Inserting  "<< NewInstructionIssueCycle +Latency << " to ReorderBufferCompletionCycles\n");
#endif
      }
    }
  }
  //dbgs() << "Next\n";
}



//===----------------------------------------------------------------------===//
//                        Print statistics
//===----------------------------------------------------------------------===//

void
DynamicAnalysis::printHeaderStat(string Header){
  
  dbgs() << "//===--------------------------------------------------------------===//\n";
  dbgs() << "//                     "<<Header <<"                                    \n";
  dbgs() << "//===--------------------------------------------------------------===//\n";
}



void
DynamicAnalysis::finishAnalysis(){
  
  
  
  bool PrintWarning = false;
  unsigned long long TotalSpan = 0;
  uint64_t TotalStallSpan = 0;
  uint64_t PairSpan = 0;
  unsigned Span = 0;
  bool MergeArithmeticOps = false;
  float Performance = 0;
  vector<int> ResourcesInCriticalPath;
  uint64_t Total;
  uint64_t T1, T2, OverlapCycles;
  vector<int> TmpResourcesVector;
  vector<int> compResources;
  vector<int> memResources;
  vector<uint64_t> TmpSpanVector;
  vector<uint64_t> ResourcesSpan(nExecutionUnits+nPorts+nAGUs + nLoadAGUs+nStoreAGUs + nBuffers);
  vector<uint64_t> ResourcesTotalStallSpanVector(nExecutionUnits+nPorts+nAGUs + nLoadAGUs+nStoreAGUs + nBuffers);
  vector< vector<uint64_t> > ResourcesResourcesNoStallSpanVector(nExecutionUnits, vector<uint64_t>(nExecutionUnits));
  vector< vector<uint64_t> > ResourcesResourcesSpanVector(nExecutionUnits, vector<uint64_t>(nExecutionUnits));
  vector< vector<uint64_t> > ResourcesStallSpanVector(nExecutionUnits, vector<uint64_t>(nExecutionUnits));
  vector< vector<uint64_t> > StallStallSpanVector(nBuffers, vector<uint64_t>(nBuffers));
  vector< vector<uint64_t> > ResourcesIssueStallSpanVector(nExecutionUnits, vector<uint64_t>(nBuffers));
  
#ifdef DEBUG_OOO_BUFFERS
  DEBUG(dbgs() << "Starting while loop\n";
        DEBUG(dbgs() << "Sise of RS "<< ReservationStationIssueCycles.size()<<"\n");
        DEBUG(dbgs() << "Sise of ROB "<<ReorderBufferCompletionCycles.size()<<"\n");
        DEBUG(dbgs() << "Sise of LB "<< LineFillBufferCompletionCycles.size() <<"\n");
        DEBUG(dbgs() << "Sise of SB "<< StoreBufferCompletionCycles.size() <<"\n");
        DEBUG(dbgs() << "Sise of LFB "<< LineFillBufferCompletionCycles.size() <<"\n");
        DEBUG(dbgs() << "______________________________________________________\n");
#endif
        
        
        // Increase FetchCycle until all buffers are empty
        while (ReservationStationIssueCycles.size() != 0 || ReorderBufferCompletionCycles.size() != 0 || LoadBufferCompletionCycles.size() != 0 || StoreBufferCompletionCycles.size() != 0 || LineFillBufferCompletionCycles.size() != 0) {
          // In IncreaseInstructionFetchCycle(), InstructionFetchCycle only increases when
          // RS or ROB are full. But in this case, they may not get full, but we just
          // want to empty them
          InstructionFetchCycle++;
          IncreaseInstructionFetchCycle();
          
#ifdef DEBUG_OOO_BUFFERS
          
          PrintReservationStation();
          PrintReorderBuffer();
          PrintStoreBuffer();
          PrintLoadBuffer();
          PrintLineFillBuffer();
          
          DEBUG(dbgs() << "Size of RS "<< ReservationStationIssueCycles.size()<<"\n");
          DEBUG(dbgs() << "Size of ROB "<<ReorderBufferCompletionCycles.size()<<"\n");
          DEBUG(dbgs() << "Size of LB "<< LineFillBufferCompletionCycles.size() <<"\n");
          DEBUG(dbgs() << "Size of SB "<< StoreBufferCompletionCycles.size() <<"\n");
          DEBUG(dbgs() << "Size of LFB "<< LineFillBufferCompletionCycles.size() <<"\n");
          DEBUG(dbgs() << "______________________________________________________\n");
#endif
          
        }
        
#ifdef DEBUG_OOO_BUFFERS
        
        DEBUG(dbgs() << "Size of RS "<< ReservationStationIssueCycles.size()<<"\n");
        DEBUG(dbgs() << "Size of ROB "<<ReorderBufferCompletionCycles.size()<<"\n");
        DEBUG(dbgs() << "Size of LB "<< LineFillBufferCompletionCycles.size() <<"\n");
        DEBUG(dbgs() << "Size of SB "<< StoreBufferCompletionCycles.size() <<"\n");
        DEBUG(dbgs() << "Size of LFB "<< LineFillBufferCompletionCycles.size() <<"\n");
#endif
        
        
        
        
        for (unsigned i = 0; i < nCompNodes; i++)
        compResources.push_back(i);
        
        for (unsigned i = N_MEM_NODES_START ; i <= N_MEM_NODES_END; i++)
        memResources.push_back(i);
        
        for (unsigned j = 0; j < nExecutionUnits + nAGUs + nPorts + nBuffers; j++){
          LastIssueCycleVector.push_back(GetLastIssueCycle(j, false));
          
          if (InstructionFetchCycle != 0 && LastIssueCycleVector[j] > InstructionFetchCycle) {
            report_fatal_error("LastIssueCycle > InstructionFetchCycle for resource\n");
          }
        }
        for (unsigned j=0; j< nExecutionUnits; j++) {
          TmpResourcesVector.clear();
          TmpResourcesVector.push_back(j);
          IssueSpan[j] = CalculateIssueSpan(TmpResourcesVector);
        }
        
        
        
        // We have this loop in case we want to print out stats with all computation nodes
        // merged, needed to generate the original roofline plots. But if we don't need
        // them, we just print the data without merging FP ops.
        for (int i = 0; i < 1 ; i++) {
          
          DEBUG(dbgs() << "i " << i << "\n");
          
          if (i==1)
            MergeArithmeticOps = true;
          
          if (MergeArithmeticOps) {
            InstructionsCountExtended[FP_ADDER] = InstructionsCountExtended[FP_ADDER]+ InstructionsCountExtended[FP_MULTIPLIER]+InstructionsCountExtended[FP_DIVIDER];
            
            TmpResourcesVector.clear();
            TmpResourcesVector.push_back(FP_ADDER);
            TmpResourcesVector.push_back(FP_MULTIPLIER);
            TmpResourcesVector.push_back(FP_DIVIDER);
            
            IssueSpan[FP_ADDER] = CalculateIssueSpan(TmpResourcesVector);
          }
          
          for (unsigned j=0; j< nExecutionUnits; j++) {
            if (!(MergeArithmeticOps && j==FP_MULTIPLIER) && !(MergeArithmeticOps && j==FP_DIVIDER)) {
              if (MergeArithmeticOps && j == FP_ADDER) {
                TmpResourcesVector.clear();
                TmpResourcesVector.push_back(j);
                TmpResourcesVector.push_back(j+1);
                TmpResourcesVector.push_back(j+2);
                Span =CalculateGroupSpan(TmpResourcesVector);
                ResourcesSpan[j] = Span;
              }else{
                if (!MergeArithmeticOps) { // Calculate only once
                  TmpResourcesVector.clear();
                  TmpResourcesVector.push_back(j);
                  DEBUG(dbgs() << "Calculating group span for resource " << j << "\n");
                  Span =CalculateGroupSpan(TmpResourcesVector);
                  DEBUG(dbgs() << "Span " << Span << "\n");
                  ResourcesSpan[j] = Span;
                }
              }
              
#ifdef ASSERT
              if (!MergeArithmeticOps) {
                TmpResourcesVector.clear();
                TmpResourcesVector.push_back(j);
                if(Span != CalculateGroupSpan(TmpResourcesVector))
                  report_fatal_error("Span ("+Twine(Span)+" differs from CalculateGroupSpan ("+Twine(CalculateGroupSpan(TmpResourcesVector))+")");
              }
#endif
              
            }
          }
          
          //Span for OOO buffers
          for (unsigned j = RS_STALL; j <= LFB_STALL; j++) {
            
            TmpResourcesVector.clear();
            TmpResourcesVector.push_back(j);
            IssueSpan[j] = CalculateIssueSpan(TmpResourcesVector);
            
            //Calculate span is an expensive operation. Therefore, wehenever we can, we
            // obtain the span from a different way.
            Span=InstructionsCountExtended[j];
            DEBUG(dbgs() << "Span  " << Span << "\n");
            ResourcesSpan[j] = Span;
            DEBUG(dbgs() << "Storing span  " <<  ResourcesSpan[j] << "\n");
#ifdef ASSERT
            if (!MergeArithmeticOps) {
              TmpResourcesVector.clear();
              TmpResourcesVector.push_back(j);
              uint64_t CalculateSpanResult = CalculateSpan(j);
              uint64_t CalculateGroupSpanResult = CalculateGroupSpan(TmpResourcesVector);
              DEBUG(dbgs() << "CalculateGroupSpanResult  " <<  CalculateGroupSpanResult << "\n");
              
              if (!( CalculateSpanResult== Span  &&  Span == CalculateGroupSpanResult))
                report_fatal_error("Spans differ: Span (" + Twine(Span)+"), CalculateSpan ("+Twine(CalculateSpanResult)+
                                   "), CalculateGroupSpan ("+Twine(CalculateGroupSpanResult)+")");
            }
#endif
          }
          
#ifdef ILP_DISTRIBUTION
          int DistributionLength = ParallelismDistribution.size();
          unsigned long SpanDistribution = 0;
          vector<uint64_t> InstructionsSpanDistribution;
          for (int i = 0; i< N_INST_TYPES; i++)
            InstructionsSpanDistribution.push_back(0);
          
          printHeaderStat("ILP distribution");
          for (i=0; i< DistributionLength; i++) {
            for(j=0; j< N_INST_TYPES; j++){
              uint64_t NInstructions = ParallelismDistribution[i][j];
              if (NInstructions != 0) {
                InstructionsSpanDistribution[j]++;
              }
            }
          }
          
          SpanDistribution = ParallelismDistribution.size();
          
          ParallelismDistribution.clear();
#endif
          
          //Reuse Distance Distribution
          printHeaderStat("Reuse Distance distribution");
          
#ifdef NORMAL_REUSE_DISTRIBUTION
          map <int,int>::iterator ReuseDistanceMapIt;
          for(ReuseDistanceMapIt= ReuseDistanceDistribution.begin();
              ReuseDistanceMapIt != ReuseDistanceDistribution.end(); ++ReuseDistanceMapIt){
            dbgs() << ReuseDistanceMapIt->first << " "<< ReuseDistanceMapIt->second<< "\n";
          }
#else
          map<int,map<uint64_t,uint> >::iterator ReuseDistanceExtendedMapIt;
          map<uint64_t,uint>::iterator AuxMapIt;
          for (ReuseDistanceExtendedMapIt = ReuseDistanceDistributionExtended.begin();
               ReuseDistanceExtendedMapIt != ReuseDistanceDistributionExtended.end();
               ++ReuseDistanceExtendedMapIt) {
            dbgs () << ReuseDistanceExtendedMapIt->first << " ";
            for (AuxMapIt= (ReuseDistanceExtendedMapIt->second).begin();
                 AuxMapIt != (ReuseDistanceExtendedMapIt->second).end();
                 ++AuxMapIt) {sdz
              dbgs() << AuxMapIt->first << " "<< AuxMapIt->second<< " ";
            }
            dbgs () << "\n";
          }
#endif
          dbgs() << "DATA_SET_SIZE\t" << node_size(ReuseTree) << "\n";
          
          printHeaderStat("Statistics");
          
          unsigned long long InstructionLatency = 0;
          unsigned IssueCycleGranularity = 1;
          uint64_t LastCycle =0;
          
          //================= Calculate total span ==========================//
          
          TmpResourcesVector.clear();
          
          for(unsigned j=0; j< nExecutionUnits; j++){
            
            if (MergeArithmeticOps) {
              if (j==FP_ADDER) {
                TmpResourcesVector.push_back(j);
                TmpResourcesVector.push_back(j+1);
                TmpResourcesVector.push_back(j+2);
                // If there are instructions of this type
                if (InstructionsCountExtended[j]>0 || InstructionsCountExtended[j+1]>0 || InstructionsCountExtended[j+2]>0) {
                  TotalSpan = CalculateGroupSpan(TmpResourcesVector);
                }
              }else{
                if (j>FP_DIVIDER) {
                  TmpResourcesVector.push_back(j);
                  // If there are instructions of this type
                  if (InstructionsCountExtended[j]>0) {
                    IssueCycleGranularity = IssueCycleGranularities[j];
                    InstructionLatency  =ExecutionUnitsLatency[j];
                    LastCycle = LastIssueCycleVector[j];
                    TotalSpan = max(LastCycle+InstructionLatency , TotalSpan);
                  }
                  
                }
              }
              
            }else{
              TmpResourcesVector.push_back(j);
              // If there are instructions of this type
              if (InstructionsCountExtended[j]>0) {
                
                IssueCycleGranularity = IssueCycleGranularities[j];
                
                InstructionLatency  =ExecutionUnitsLatency[j];
                
                // LastCycle = GetLastIssueCycle(GetExecutionResource(j), j);
                //LastCycle = GetLastIssueCycle(j);
                LastCycle = LastIssueCycleVector[j];
                TotalSpan = max(LastCycle+InstructionLatency, TotalSpan);
                
              }
            }
            
          }
          
          
          //Calculate Resources-total stall span
          
          for(unsigned i=0; i< nExecutionUnits; i++){
            
            if (!(MergeArithmeticOps && i==FP_MULTIPLIER) && !(MergeArithmeticOps && i==FP_DIVIDER)) {
              
              TmpResourcesVector.clear();
              
              
              if (MergeArithmeticOps && i==FP_ADDER) {
                TmpResourcesVector.push_back(i+1);
                TmpResourcesVector.push_back(i+2);
              }
              
              for(uint j=RS_STALL; j<= LFB_STALL; j++){
                if (InstructionsCountExtended[j]!=0) {
                  TmpResourcesVector.push_back(j);
                }
              }
              TmpResourcesVector.push_back(i);
              
              ResourcesTotalStallSpanVector[i]= CalculateGroupSpan(TmpResourcesVector);
              
            }
            
          }
          //==================== Print resource statistics =============================//
          
          dbgs() << "RESOURCE\tN_OPS_ISSUED\tSPAN\t\tISSUE-SPAN\tSTALL-SPAN\t\tSPAN-GAPS\t\tMAX DAG LEVEL OCCUPANCY\n";
          
          for(unsigned j=0; j< nExecutionUnits; j++){
            if (!(MergeArithmeticOps &&  j==FP_MULTIPLIER) && !(MergeArithmeticOps && j==FP_DIVIDER)){
              
              dbgs() << ResourcesNames[j]<< "\t\t"<<InstructionsCountExtended[j]<<"\t\t"<<ResourcesSpan[j]<<"\t\t"<<IssueSpan[j]<<
              "\t\t"<<ResourcesTotalStallSpanVector[j] <<"\t\t"<< SpanGaps[j]<<"\t\t"<< MaxOccupancy[j] << " \n";
              
            }
          }
          
          
          
          //==================== Print stall cycles =============================//
          
          printHeaderStat("Stall Cycles");
          
          dbgs() << "RESOURCE\tN_STALL_CYCLES\t\tAVERAGE_OCCUPANCY\n";
          
          for(int j=RS_STALL; j<= LFB_STALL; j++){
            
            dbgs() << ResourcesNames[j]<< "\t\t" << ResourcesSpan[j] << "\t\t" << BuffersOccupancy[j-RS_STALL]/TotalSpan<<"\n";
            
          }
          
          printHeaderStat("Span Only Stalls");
          TmpResourcesVector.clear();
          for (unsigned i = RS_STALL; i<=LFB_STALL; i++) {
            if (InstructionsCountExtended[i]> 0) {
              TmpResourcesVector.push_back(i);
            }
          }
          if (TmpResourcesVector.empty() == false) {
            TotalStallSpan = CalculateGroupSpan(TmpResourcesVector);
          }else{
            TotalStallSpan = 0;
          }
          dbgs() << TotalStallSpan << "\n";
          
          
          
          //==================== Print port Occupancy =============================//
          
          printHeaderStat("Port occupancy");
          
          dbgs() << "PORT\t\tDISPATCH CYCLES\n";
          
          for(int j=PORT_0; j<= PORT_4; j++){
            TmpResourcesVector.clear();
            TmpResourcesVector.push_back(j);
            dbgs() << ResourcesNames[j]<< "\t\t" << CalculateGroupSpan(TmpResourcesVector) << "\n";
          }
          
          if (!ReportOnlyPerformance) {
            
            //==================== Resource-Stall Span =============================//
            
            printHeaderStat("Resource-Stall Span");
            dbgs() << "RESOURCE";
            for(int j=RS_STALL; j<= LFB_STALL; j++){
              dbgs() << "\t"<<ResourcesNames[j];
            }
            dbgs() << "\n";
            
            for(unsigned i=0; i< nExecutionUnits; i++){
              if (!(MergeArithmeticOps &&  i==FP_MULTIPLIER) && !(MergeArithmeticOps && i==FP_DIVIDER)){
                dbgs() << ResourcesNames[i]<< "\t\t";
                for(uint j=RS_STALL; j<=LFB_STALL; j++){
                  if (InstructionsCountExtended[i]!= 0 && InstructionsCountExtended[j]!= 0 ) {
                    TmpResourcesVector.clear();
                    TmpResourcesVector.push_back(i);
                    if (MergeArithmeticOps &&  i==FP_ADDER) {
                      TmpResourcesVector.push_back(i+1);
                      TmpResourcesVector.push_back(i+2);
                    }
                    
                    TmpResourcesVector.push_back(j);
                    
                    PairSpan = CalculateGroupSpan(TmpResourcesVector);
                    
                  }else{
                    if (InstructionsCountExtended[i]==0) {
                      PairSpan = InstructionsCountExtended[j];
                    }else{
                      if (InstructionsCountExtended[j]== 0 ) {
                        PairSpan = ResourcesSpan[i];
                        
                      }
                    }
                  }
                  dbgs() << PairSpan << "\t";
                  ResourcesStallSpanVector[i][j-RS_STALL] = PairSpan; // Store the Span value
                }
                dbgs() << "\n";
              }
            }
            
            //==================== Resource-Stall Overlap =============================//
            
            printHeaderStat("Resource-Stall Overlap (0-1)");
            dbgs() << "RESOURCE";
            for(unsigned j=RS_STALL; j<= LFB_STALL; j++){
              dbgs() << "\t"<<ResourcesNames[j];
            }
            dbgs() << "\n";
            
            float OverlapPercetage;
            for(unsigned i=0; i< nExecutionUnits; i++){
              if (!(MergeArithmeticOps &&  i==FP_MULTIPLIER) && !(MergeArithmeticOps && i==FP_DIVIDER)){
                dbgs() << ResourcesNames[i]<< "\t\t";
                for(uint j=RS_STALL; j <= LFB_STALL; j++){
                  if (InstructionsCountExtended[i]!=0 && InstructionsCountExtended[j]!=0){
                    Total = ResourcesStallSpanVector[i][j-RS_STALL];
                    T1 = ResourcesSpan[i];
                    T2 = ResourcesSpan[j];
                    assert(Total <= T1+T2);
                    OverlapCycles =  T1+T2-Total;
                    OverlapPercetage = (float)OverlapCycles/(float(min(T1, T2)));
                    if (OverlapPercetage > 1.0) {
                      report_fatal_error("Overlap > 1.0");
                    }
                  }else{
                    OverlapPercetage = 0;
                  }
                  fprintf(stderr, " %1.3f ", OverlapPercetage);
                }
                dbgs() << "\n";
              }
            }
            
            //==================== ResourceIssue-Stall Span =============================//
            
            printHeaderStat("ResourceIssue-Stall Span");
            dbgs() << "RESOURCE";
            for(unsigned j=RS_STALL; j<= LFB_STALL; j++){
              dbgs() << "\t"<<ResourcesNames[j];
            }
            dbgs() << "\n";
            
            for(unsigned i=0; i< nExecutionUnits; i++){
              if (!(MergeArithmeticOps &&  i==FP_MULTIPLIER) && !(MergeArithmeticOps && i==FP_DIVIDER)){
                dbgs() << ResourcesNames[i]<< "\t\t";
                
                for(uint j=RS_STALL; j<=LFB_STALL; j++){
                  if (InstructionsCountExtended[i]!= 0 && InstructionsCountExtended[j]!= 0 ) {
                    TmpResourcesVector.clear();
                    TmpResourcesVector.push_back(i);
                    if ( MergeArithmeticOps &&  i==FP_ADD_NODE) {
                      TmpResourcesVector.push_back(i+1);
                      TmpResourcesVector.push_back(i+2);
                    }
                    TmpResourcesVector.push_back(j);
                    PairSpan = CalculateIssueSpan(TmpResourcesVector);
                    //PairSpan = CalculateGroupSpanUnitLatency(TmpResourcesVector);
                    ResourcesIssueStallSpanVector[i][j-RS_STALL] = PairSpan;
                    
                    
                    // #ifdef ASSERT
                    //if (!MergeArithmeticOps) {
                    //assert(PairSpan == CalculateGroupSpan(TmpResourcesVector,false, true));
                    // }
                    
                    // #endif
                    
                  }else{
                    if (InstructionsCountExtended[i]==0) {
                      PairSpan = InstructionsCountExtended[j];
                      ResourcesIssueStallSpanVector[i][j-RS_STALL] = PairSpan;
                    }else{
                      if (InstructionsCountExtended[j]== 0 ) {
                        PairSpan = IssueSpan[i];
                        ResourcesIssueStallSpanVector[i][j-RS_STALL] = PairSpan;
                        
                      }
                    }
                  }
                  dbgs() << PairSpan << "\t";
                }
                dbgs() << "\n";
              }
            }
            
            //==================== ResourceIssue-Stall Overlap =============================//
            
            printHeaderStat("ResourceIssue-Stall Overlap (0-1)");
            dbgs() << "RESOURCE";
            for(unsigned j=RS_STALL; j<= LFB_STALL; j++){
              dbgs() << "\t"<<ResourcesNames[j];
            }
            dbgs() << "\n";
            float OverlapPercentage;
            
            for(unsigned i=0; i< nExecutionUnits; i++){
              if (!(MergeArithmeticOps &&  i==FP_MULTIPLIER) && !(MergeArithmeticOps && i==FP_DIVIDER)){
                dbgs() << ResourcesNames[i]<< "\t\t";
                for(uint j=RS_STALL; j <= LFB_STALL; j++){
                  if (InstructionsCountExtended[i]!=0 && InstructionsCountExtended[j]!=0){
                    Total = ResourcesIssueStallSpanVector[i][j-RS_STALL];
                    T1 = IssueSpan[i];
                    T2 = InstructionsCountExtended[j];
                    assert(Total <= T1+T2);
                    OverlapCycles =  T1+T2-Total;
                    OverlapPercentage = (float)OverlapCycles/(float(min(T1, T2)));
                    if (OverlapPercentage > 1.0) {
                      report_fatal_error("Overlap > 1.0");
                    }
                  }else{
                    OverlapPercentage = 0;
                  }
                  fprintf(stderr, " %1.3f ", OverlapPercentage);
                }
                dbgs() << "\n";
              }
            }
            
            //==================== Resource-Resource Span =============================//
            
            printHeaderStat("Resource-Resource Span (resources span without stalls)");
            
            dbgs() << "RESOURCE";
            for(unsigned j=0; j< nExecutionUnits; j++){
              if (!(MergeArithmeticOps &&  j==FP_MULTIPLIER) && !(MergeArithmeticOps && j==FP_DIVIDER)){
                dbgs() << "\t"<<ResourcesNames[j];
              }
            }
            dbgs() << "\n";
            
            for(unsigned j=0; j< nExecutionUnits; j++){
              if (!(MergeArithmeticOps &&  j==FP_MULTIPLIER) && !(MergeArithmeticOps && j==FP_DIVIDER)){
                dbgs() << ResourcesNames[j]<< "\t\t";
                for(unsigned i=0; i< j; i++){
                  TmpResourcesVector.clear();
                  
                  if (!(MergeArithmeticOps &&  i==FP_MULTIPLIER) && !(MergeArithmeticOps && i==FP_DIVIDER)){
                    
                    if (InstructionsCountExtended[i]!= 0 && InstructionsCountExtended[j]!=0) {
                      TmpResourcesVector.push_back(i);
                      if(MergeArithmeticOps &&  i == FP_ADDER){
                        TmpResourcesVector.push_back(i+1);
                        TmpResourcesVector.push_back(i+2);
                      }
                      TmpResourcesVector.push_back(j);
                      PairSpan = CalculateGroupSpan(TmpResourcesVector);
                      
                    }else{
                      if(InstructionsCountExtended[i]==0){
                        PairSpan = ResourcesSpan[j];
                        
                      }else{
                        if(InstructionsCountExtended[j]==0){
                          PairSpan = ResourcesSpan[i];
                        }
                      }
                    }
                    
                    dbgs() << PairSpan << "\t";
                    
                    ResourcesResourcesNoStallSpanVector[j][i] = PairSpan;
                    
                  }
                }
                dbgs() << "\n";
              }
            }
            
            printHeaderStat("Resource-Resource Overlap Percentage (resources span without stall)");
            
            dbgs() << "RESOURCE";
            for(unsigned j=0; j< nExecutionUnits; j++){
              if (!(MergeArithmeticOps &&  j==FP_MULTIPLIER) && !(MergeArithmeticOps && j==FP_DIVIDER)){
                dbgs() << "\t"<<ResourcesNames[j];
              }
            }
            dbgs() << "\n";
            
            for(unsigned j=0; j< nExecutionUnits; j++){
              if (!(MergeArithmeticOps &&  j==FP_MULTIPLIER) && !(MergeArithmeticOps && j==FP_DIVIDER)){
                dbgs() << ResourcesNames[j]<< "\t\t";
                for(unsigned i=0; i< j; i++){
                  if (!(MergeArithmeticOps &&  i==FP_MULTIPLIER) && !(MergeArithmeticOps && i==FP_DIVIDER)){
                    if (InstructionsCountExtended[i]!= 0 && InstructionsCountExtended[j]!=0) {
                      Total = ResourcesResourcesNoStallSpanVector[j][i];
                      T1 = ResourcesSpan[j];
                      T2 = ResourcesSpan[i];
                      assert(Total <= T1+T2);
                      OverlapCycles =  T1+T2-Total;
                      OverlapPercetage = (float)OverlapCycles/(float(min(T1, T2)));
                      if (OverlapPercetage > 1.0) {
                        report_fatal_error("Overlap > 1.0");
                      }
                    }else{
                      OverlapPercetage = 0;
                    }
                    fprintf(stderr, " %1.3f ", OverlapPercetage);
                    
                  }
                }
                dbgs() << "\n";
              }
            }
            
            printHeaderStat("Resource-Resource Span (resources span with stalls)");
            
            vector<int> StallsVector;
            
            dbgs() << "RESOURCE";
            for(unsigned j=0; j< nExecutionUnits; j++){
              if (!(MergeArithmeticOps &&  j==FP_MULTIPLIER) && !(MergeArithmeticOps && j==FP_DIVIDER)){
                dbgs() << "\t"<<ResourcesNames[j];
              }
            }
            dbgs() << "\n";
            
            for(unsigned j=0; j< nExecutionUnits; j++){
              if (!(MergeArithmeticOps &&  j==FP_MULTIPLIER) && !(MergeArithmeticOps && j==FP_DIVIDER)){
                dbgs() << ResourcesNames[j]<< "\t\t";
                for(unsigned i=0; i< j; i++){
                  TmpResourcesVector.clear();
                  for (unsigned k = RS_STALL; k <= LFB_STALL; k++) {
                    TmpResourcesVector.push_back(k);
                  }
                  
                  if (!(MergeArithmeticOps &&  i==FP_MULTIPLIER) && !(MergeArithmeticOps && i==FP_DIVIDER)){
                    
                    if (InstructionsCountExtended[i]!= 0 && InstructionsCountExtended[j]!=0) {
                      
                      TmpResourcesVector.push_back(i);
                      if(MergeArithmeticOps &&  i == FP_ADDER){
                        TmpResourcesVector.push_back(i+1);
                        TmpResourcesVector.push_back(i+2);
                      }
                      
                      TmpResourcesVector.push_back(j);
                      PairSpan = CalculateGroupSpan(TmpResourcesVector);
                      
                    }else{
                      if(InstructionsCountExtended[i]==0){
                        PairSpan = TotalStallSpan;
                        
                      }else{
                        if(InstructionsCountExtended[j]==0){
                          PairSpan = ResourcesTotalStallSpanVector[i];
                          
                        }
                      }
                    }
                    
                    dbgs() << PairSpan << "\t";
                    ResourcesResourcesSpanVector[j][i] = PairSpan;
                  }
                }
                dbgs() << "\n";
              }
            }
            
            printHeaderStat("Resource-Resource Overlap Percentage (resources span with stall)");
            
            dbgs() << "RESOURCE";
            for(unsigned j=0; j< nExecutionUnits; j++){
              if (!(MergeArithmeticOps &&  j==FP_MULTIPLIER) && !(MergeArithmeticOps && j==FP_DIVIDER))
                dbgs() << "\t"<<ResourcesNames[j];
            }
            dbgs() << "\n";
            
            for(unsigned j=0; j< nExecutionUnits; j++){
              if (!(MergeArithmeticOps &&  j==FP_MULTIPLIER) && !(MergeArithmeticOps && j==FP_DIVIDER)){
                dbgs() << ResourcesNames[j]<< "\t\t";
                for(unsigned i=0; i< j; i++){
                  if (!(MergeArithmeticOps &&  i==FP_MULTIPLIER) && !(MergeArithmeticOps && i==FP_DIVIDER)){
                    if (InstructionsCountExtended[i]!= 0 && InstructionsCountExtended[j]!=0) {
                      Total = ResourcesResourcesSpanVector[j][i];
                      T1 = ResourcesTotalStallSpanVector[j];
                      T2 = ResourcesTotalStallSpanVector[i];
                      
                      
                      assert(Total <= T1+T2);
                      OverlapCycles =  T1+T2-Total;
                      OverlapPercetage = (float)OverlapCycles/(float(min(T1, T2)));
                      if (OverlapPercetage > 1.0) {
                        report_fatal_error("Overlap > 1.0");
                      }
                    }else{
                      OverlapPercetage = 0;
                    }
                    fprintf(stderr, " %1.3f ", OverlapPercetage);
                    
                  }
                }
                dbgs() << "\n";
              }
            }
            
            printHeaderStat("Stall-Stall Span");
            
            dbgs() << "RESOURCE";
            for(unsigned j=RS_STALL; j<=LFB_STALL; j++){
              dbgs() << "\t"<<ResourcesNames[j];
            }
            dbgs() << "\n";
            
            for(unsigned j=RS_STALL; j<= LFB_STALL; j++){
              dbgs() << ResourcesNames[j]<< "\t\t";
              for(unsigned i=RS_STALL; i< j; i++){
                if (InstructionsCountExtended[j]!= 0 && InstructionsCountExtended[i]!=0) {
                  
                  TmpResourcesVector.clear();
                  TmpResourcesVector.push_back(j);
                  TmpResourcesVector.push_back(i);
                  PairSpan = CalculateGroupSpan(TmpResourcesVector);
                  
                }else{
                  if(InstructionsCountExtended[i]==0){
                    PairSpan = ResourcesSpan[j];
                  }else{
                    if(InstructionsCountExtended[j]==0){
                      PairSpan = ResourcesSpan[i];
                    }
                  }
                }
                
                dbgs() << PairSpan << "\t";
                StallStallSpanVector[j-RS_STALL][i-RS_STALL] = PairSpan;
              }
              dbgs() << "\n";
            }
            
            printHeaderStat("Stall-Stall Overlap Percentage ");
            
            dbgs() << "RESOURCE";
            for(unsigned j=RS_STALL; j <= LFB_STALL; j++){
              dbgs() << "\t"<<ResourcesNames[j];
            }
            dbgs() << "\n";
            
            for(unsigned j=RS_STALL; j<= LFB_STALL; j++){
              dbgs() << ResourcesNames[j]<< "\t\t";
              for(unsigned i=RS_STALL; i< j; i++){
                if (InstructionsCountExtended[j]!= 0 && InstructionsCountExtended[i]!=0) {
                  Total = StallStallSpanVector[j-RS_STALL][i-RS_STALL];
                  T1 = ResourcesSpan[j];
                  T2 = ResourcesSpan[i];
                  assert(Total <= T1+T2);
                  OverlapCycles =  T1+T2-Total;
                  OverlapPercetage = (float)OverlapCycles/(float(min(T1, T2)));
                  
                }else{
                  OverlapPercetage = 0;
                }
                fprintf(stderr, " %1.3f ", OverlapPercetage);
                
              }
              dbgs() << "\n";
            }
            
            
            printHeaderStat("Bottlenecks");
            dbgs() << "Bottleneck\tISSUE\tLAT\t";
            for(int j=RS_STALL; j<= LFB_STALL; j++){
              dbgs() << ResourcesNames[j] << "\t";
            }
            dbgs() << "\n";
            uint64_t Work;
            
            for(unsigned i=0; i< nExecutionUnits; i++){
              
              // Work is always the total number of floating point operations... Otherwise it makes
              // no sense to compare with the performance for memory nodes which is calcualted
              // with total work
              Work = InstructionsCount[0];
              if (!(MergeArithmeticOps &&  i==FP_MULTIPLIER) && !(MergeArithmeticOps && i==FP_DIVIDER)){
                
                dbgs() << ResourcesNames[i]<< "\t\t";
                if(IssueSpan[i]>0){
                  Performance = (float)Work/((float)IssueSpan[i]);
                  fprintf(stderr, " %1.3f ", Performance);
                }else
                  dbgs() << INF<<"\t";
                if(ResourcesSpan[i]>0){
                  Performance = (float)Work/((float)ResourcesSpan[i]);
                  fprintf(stderr, " %1.3f ", Performance);
                }else
                  dbgs() << INF<<"\t";
                // dbgs() << "inf\t";
                for(unsigned j=0; j< nBuffers; j++){
                  if(ResourcesIssueStallSpanVector[i][j] >0 &&  ResourcesSpan[j+RS_STALL] !=0 ){
                    Performance = (float)Work/((float)ResourcesIssueStallSpanVector[i][j]);
                    fprintf(stderr, " %1.3f ", Performance);
                  }else
                    dbgs() << INF<<"\t";
                  
                }
                dbgs() << "\n";
              }
            }
          }
          
          printHeaderStat("Execution Times Breakdowns");
          unsigned MinExecutionTime;
          unsigned IssueEffects;
          unsigned LatencyEffects;
          unsigned StallEffects;
          float Throughput = 0;
          dbgs() << "RESOURCE\tMIN-EXEC-TIME\tISSUE-EFFECTS\tLATENCY-EFFECTS\tSTALL-EFFECTS\tTOTAL\n";
          
          for(unsigned i=0; i< nExecutionUnits; i++){
            if (!(MergeArithmeticOps &&  i==FP_MULTIPLIER) && !(MergeArithmeticOps && i==FP_DIVIDER)){
              
              if (InstructionsCountExtended[i]==0) {
                MinExecutionTime = 0;
                LatencyEffects = 0;
                IssueEffects = 0;
                StallEffects = ResourcesTotalStallSpanVector[i];
              }else{
                if (ExecutionUnitsParallelIssue[i]==INF) {
                  if (ExecutionUnitsThroughput[i]==INF) {
                    Throughput = INF;
                  }else{
                    Throughput = ExecutionUnitsThroughput[i];
                    dbgs() << "Setting thoughput to " << ExecutionUnitsThroughput[i] << "\n";
                  }
                }else{
                  if (ExecutionUnitsThroughput[i]==INF) {
                    Throughput = ExecutionUnitsParallelIssue[i];
                  }else{
                    Throughput = ExecutionUnitsThroughput[i]*ExecutionUnitsParallelIssue[i];
                  }
                }
                
                if (i < nCompExecutionUnits) {
                  if (Throughput == INF) {
                    MinExecutionTime = 1;
                  }else{
                    MinExecutionTime = (unsigned)ceil(InstructionsCountExtended[i]/Throughput);
                  }
                }else{
                  if (Throughput == INF) {
                    MinExecutionTime = 1;
                  }else
                    MinExecutionTime = (unsigned)ceil(InstructionsCountExtended[i]*AccessGranularities[i]/(Throughput));
                  dbgs() << "InstructionsCountExtended[i] " << InstructionsCountExtended[i] << "\n";
                  dbgs() << "Throughput " << Throughput << "\n";
                  dbgs() << "MinExecutionTime " << MinExecutionTime << "\n";
                }
                
                if (Throughput==INF && IssueSpan[i]==1 ) {
                  IssueEffects = 0;
                  
                }else{
                  if (IssueSpan[i] < MinExecutionTime) {
                    PrintWarning = true;
                    IssueSpan[i] = MinExecutionTime;
                    //report_fatal_error("IssueSpan < Min execution time");
                  }
                  IssueEffects = IssueSpan[i] - MinExecutionTime;
                }
                LatencyEffects = ResourcesSpan[i] - IssueSpan[i];
                StallEffects = ResourcesTotalStallSpanVector[i] - ResourcesSpan[i];
                
              }
              
              dbgs() << ResourcesNames[i]<< "\t\t";
              dbgs() << " " << MinExecutionTime;
              //  fprintf(stderr, " %1.3f ", MinExecutionTime);
              dbgs() << "\t";
              dbgs() << " " << IssueEffects;
              // fprintf(stderr, " %1.3f ", IssueEffects);
              dbgs() << "\t";
              dbgs() << " " << LatencyEffects;
              // fprintf(stderr, " %1.3f ", LatencyEffects);
              dbgs() << "\t";
              dbgs() << " " << StallEffects;
              // fprintf(stderr, " %1.3f ", StallEffects);
              if (MinExecutionTime + IssueEffects + LatencyEffects +  StallEffects != ResourcesTotalStallSpanVector[i]) {
                report_fatal_error("Breakdown of execution time does not match total execution time\n");
                
              }else{
                dbgs() << "\t"<< ResourcesTotalStallSpanVector[i]<<"\n";
              }
            }
          }
          
          
          
          
          printHeaderStat("TOTAL");
          dbgs() << "TOTAL FLOPS"<< "\t"<<InstructionsCount[0] <<"\t\t"<<CalculateGroupSpan(compResources)<<" \n";
          dbgs() << "TOTAL MOPS"<< "\t"<<InstructionsCount[1]<<"\t\t"<<CalculateGroupSpan(memResources)<<" \n";
          dbgs() << "TOTAL"<< "\t\t"<<InstructionsCount[0] +InstructionsCount[1]<<"\t\t"<<TotalSpan<<" \n";
          Performance = (float)InstructionsCount[0]/((float)TotalSpan);
          fprintf(stderr, "PERFORMANCE %1.3f\n", Performance);
          if(PrintWarning == true)
            dbgs() << "WARNING: IssueSpan < MinExecutionTime\n";
          
#ifdef ILP_DISTRIBUTION
          if(TotalSpan != SpanDistribution)
            dbgs() << "WARNING: Total Span differs! \n";
          
          for (int j = 0; j< N_INST_TYPES; j++){
            if(InstructionsSpan[j] != InstructionsSpanDistribution[j]){
              dbgs() << "WARNING: Per type span differs!\n";
              dbgs() << "Span distribution type "<< j<<": "<< InstructionsSpanDistribution[j]<<"\n";
            }
          }
          
#endif
          
        }
        
        // TODO: I should really try to clean up all memory
        // Deallocate memory
        for (unsigned i = 0; i< FullOccupancyCyclesTree.size(); i++) {
          FullOccupancyCyclesTree[i]=NULL;
        }
  
  
  }
  
