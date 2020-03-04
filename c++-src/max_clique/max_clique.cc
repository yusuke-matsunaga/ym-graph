
/// @file max_clique.cc
/// @brief max_clique の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014, 2015, 2018, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "ym/UdGraph.h"
#include "MclqSolver.h"


BEGIN_NAMESPACE_YM_UDGRAPH

// @brief (最大)クリークを求める．
// @param[in] algorithm アルゴリズム名
// @return クリークの要素(ノード番号)を収める配列を返す．
vector<int>
UdGraph::max_clique(const string& algorithm) const
{
  MclqSolver solver(graph);

  if ( algorithm == "exact" ) {
    return solver.exact(node_set);
  }
  else if ( algorithm == "greedy" ) {
    return solver.greedy(node_set);
  }
  else {
    // デフォルトフォールバック
    return solver.greedy(node_set);
  }
}

END_NAMESPACE_YM_UDGRAPH
