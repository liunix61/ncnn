// Tencent is pleased to support the open source community by making ncnn available.
//
// Copyright (C) 2022 THL A29 Limited, a Tencent company. All rights reserved.
//
// Licensed under the BSD 3-Clause License (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at
//
// https://opensource.org/licenses/BSD-3-Clause
//
// Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#include "pass_level2.h"

namespace pnnx {

class torch_norm : public GraphRewriterPass
{
public:
    const char* match_pattern_graph() const
    {
        return R"PNNXIR(7767517
6 5
pnnx.Input              input       0 1 input
prim::Constant          op_0        0 1 dim value=%dim
prim::Constant          op_1        0 1 p value=%p
prim::Constant          op_2        0 1 keepdim value=%keepdim
aten::norm              op_3        4 1 input p dim keepdim out
pnnx.Output             output      1 0 out
)PNNXIR";
    }

    const char* type_str() const
    {
        return "torch.norm";
    }
};

class torch_norm_2 : public torch_norm
{
public:
    const char* match_pattern_graph() const
    {
        return R"PNNXIR(7767517
7 6
pnnx.Input              input       0 1 input
prim::Constant          op_0        0 1 dim value=%dim
prim::Constant          op_1        0 1 p value=%p
prim::Constant          op_2        0 1 keepdim value=%keepdim
prim::Constant          op_3        0 1 dtype value=*
aten::linalg_vector_norm op_4       5 1 input p dim keepdim dtype out
pnnx.Output             output      1 0 out
)PNNXIR";
    }
};

class torch_norm_dims : public torch_norm
{
public:
    const char* match_pattern_graph() const
    {
        return R"PNNXIR(7767517
6 5
pnnx.Input              input       0 1 input
prim::Constant          op_0        0 1 dim value=%dim
prim::Constant          op_1        0 1 p value=%p
prim::Constant          op_2        0 1 keepdim value=%keepdim
aten::norm              op_3        4 1 input p dim keepdim out
pnnx.Output             output      1 0 out
)PNNXIR";
    }
};

class torch_norm_dims_2 : public torch_norm
{
public:
    const char* match_pattern_graph() const
    {
        return R"PNNXIR(7767517
7 6
pnnx.Input              input       0 1 input
prim::Constant          op_0        0 1 dim value=%dim
prim::Constant          op_1        0 1 p value=%p
prim::Constant          op_2        0 1 keepdim value=%keepdim
prim::Constant          op_3        0 1 dtype value=*
aten::linalg_vector_norm op_4       5 1 input p dim keepdim dtype out
pnnx.Output             output      1 0 out
)PNNXIR";
    }
};

REGISTER_GLOBAL_PNNX_GRAPH_REWRITER_PASS(torch_norm, 90)
REGISTER_GLOBAL_PNNX_GRAPH_REWRITER_PASS(torch_norm_2, 90)
REGISTER_GLOBAL_PNNX_GRAPH_REWRITER_PASS(torch_norm_dims, 90)
REGISTER_GLOBAL_PNNX_GRAPH_REWRITER_PASS(torch_norm_dims_2, 90)

class torch_norm_fro : public GraphRewriterPass
{
public:
    const char* match_pattern_graph() const
    {
        return R"PNNXIR(7767517
5 4
pnnx.Input              input       0 1 input
prim::Constant          op_0        0 1 dim value=%dim
prim::Constant          op_1        0 1 keepdim value=%keepdim
aten::frobenius_norm    op_2        3 1 input dim keepdim out
pnnx.Output             output      1 0 out
)PNNXIR";
    }

    const char* type_str() const
    {
        return "torch.norm";
    }

    void write(Operator* op, const std::map<std::string, Parameter>& captured_params) const
    {
        op->params["dim"] = captured_params.at("dim");
        op->params["p"] = "fro";
        op->params["keepdim"] = captured_params.at("keepdim");
    }
};

class torch_norm_fro_dims : public torch_norm_fro
{
public:
    const char* match_pattern_graph() const
    {
        return R"PNNXIR(7767517
6 5
pnnx.Input              input       0 1 input
prim::Constant          op_0        0 1 dim value=%dim
prim::Constant          op_1        0 1 p value=%p
prim::Constant          op_2        0 1 keepdim value=%keepdim
aten::norm              op_3        4 1 input p dim keepdim out
pnnx.Output             output      1 0 out
)PNNXIR";
    }
};

REGISTER_GLOBAL_PNNX_GRAPH_REWRITER_PASS(torch_norm_fro, 90)
REGISTER_GLOBAL_PNNX_GRAPH_REWRITER_PASS(torch_norm_fro_dims, 90)

} // namespace pnnx
