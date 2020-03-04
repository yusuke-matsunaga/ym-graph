
/// @file ResidualGraph.cc
/// @brief ResidualGraph の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2020 Yusuke Matsunaga
/// All rights reserved.


#include "ResidualGraph.h"


BEGIN_NAMESPACE_YM_MAXFLOW

// @brief コンストラクタ
// @param[in] src_graph 元となるフローグラフ
ResidualGraph::ResidualGraph(const FlowGraph& src_graph) :
  mNodeList(src_graph.node_num())
{
  mEdgeList.reserve(src_graph.edge_list().size());
  for ( auto& edge: src_graph.edge_list() ) {
    auto& node1 = mNodeList[edge.from()];
    auto& node2 = mNodeList[edge.to()];
    auto rg_edge = new RgEdge(mEdgeList.size(), node1, node2, edge.cap());
    node1.mOutEdgeList.push_back(rg_edge);
    node2.mInEdgeList.push_back(rg_edge);
    mEdgeList.push_back(rg_edge);
  }
}

// @brief デストラクタ
ResidualGraph::~ResidualGraph()
{
  for ( auto edge: mEdgeList ) {
    delete edge;
  }
}

END_NAMESPACE_YM_MAXFLOW
