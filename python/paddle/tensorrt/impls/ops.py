# Copyright (c) 2024 PaddlePaddle Authors. All Rights Reserved
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
import tensorrt as trt

from paddle.tensorrt.register import converter_registry

ops_type_map = {
    "pd_op.sqrt": trt.UnaryOperation.SQRT,
    "pd_op.sqrt_": trt.UnaryOperation.SQRT,
    "pd_op.floor": trt.UnaryOperation.FLOOR,
}


@converter_registry.register("pd_op.sqrt", trt_version="8.x")
@converter_registry.register("pd_op.sqrt_", trt_version="8.x")
@converter_registry.register("pd_op.floor", trt_version="8.x")
def sqrt_converter(network, paddle_op, inputs):
    input_tensor = inputs[0]
    layer = network.add_unary(input_tensor, ops_type_map[paddle_op.name()])
    return layer.get_output(0)
