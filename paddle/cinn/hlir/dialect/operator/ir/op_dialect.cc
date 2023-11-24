// Copyright (c) 2023 PaddlePaddle Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "paddle/cinn/hlir/dialect/operator/ir/op_dialect.h"
// NOTE(chenxi67): File cinn_op.h is generated by op_gen.py, see details in
// paddle/cinn/hlir/dialect/CMakeLists.txt.
#include "paddle/cinn/hlir/dialect/operator/ir/cinn_op.h"
#include "paddle/cinn/hlir/dialect/operator/ir/manual_op.h"
#include "paddle/cinn/hlir/dialect/operator/ir/op_attribute.h"

namespace cinn {
namespace dialect {

OperatorDialect::OperatorDialect(::pir::IrContext *context)
    : ::pir::Dialect(name(),
                     context,
                     ::pir::TypeId::get<cinn::dialect::OperatorDialect>()) {
  this->initialize();
}

void OperatorDialect::initialize() {
  // NOTE(chenxi67): GET_OP_LIST is defined in cinn_op.h which is
  // generated by op_gen.py, see details in
  // paddle/cinn/hlir/dialect/CMakeLists.txt.
  RegisterOps<
#define GET_OP_LIST
#include "paddle/cinn/hlir/dialect/operator/ir/cinn_op.cc"  // NOLINT
      >();
  RegisterOp<GroupOp>();
  RegisterOp<ConcatOp>();
  RegisterOp<SplitOp>();
  RegisterAttribute<GroupInfoAttribute>();
  RegisterAttribute<CUDAJITInfoAttribute>();
}

void OperatorDialect::PrintType(pir::Type type, std::ostream &os) const {}

void OperatorDialect::PrintAttribute(pir::Attribute attr,
                                     std::ostream &os) const {
  if (attr.isa<GroupInfoAttribute>()) {
    os << "(" << attr.dialect().name();
    os << '.';
    if (auto group_info_attr = attr.dyn_cast<GroupInfoAttribute>()) {
      const GroupInfo &data = group_info_attr.data();
      os << "GroupInfo)"
         << "[" << data.fn_name << "]";
    }
    { os << "<#AttrNotImplemented>"; }
  } else if (attr.isa<CUDAJITInfoAttribute>()) {
    auto cuda_jit_info = attr.dyn_cast<CUDAJITInfoAttribute>();

    os << "(" << cuda_jit_info.data().fn_ptr;
    os << ')';
  } else {
    PADDLE_THROW(phi::errors::Unimplemented(
        "cinn dialect only support GrupInfo and CUDAJITInfo"));
  }
}

void OperatorDialect::PrintOperation(pir::Operation *op,
                                     pir::IrPrinter &printer) const {
  if (auto group_op = op->dyn_cast<GroupOp>()) {
    group_op.Print(printer);
  } else {
    printer.PrintGeneralOperation(op);
  }
}

}  // namespace dialect
}  // namespace cinn

IR_DEFINE_EXPLICIT_TYPE_ID(cinn::dialect::OperatorDialect)
