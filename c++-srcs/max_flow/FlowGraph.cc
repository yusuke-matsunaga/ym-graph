
/// @file FlowGraph.cc
/// @brief FlowGraph の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2020 Yusuke Matsunaga
/// All rights reserved.


#include "ym/FlowGraph.h"
#include "ResidualGraph.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

vector<int>
find_path(ResidualGraph& graph)
{

}

END_NONAMESPACE

// @brief max-flow 問題を解く．
// @param[in] start 問題の始点
// @param[in] end 問題の終点
// @return 全体のフロー量と各枝に流れるフローのリストを返す．
tuple<int, vector<int>>
FlowGraph::max_flow(int start,
		    int end) const
{
  ResidualGraph RG(*this, start, end);

  int ne = RG.edge_num();
  while ( true ) {
    // RG 上の増加路を見つける．
    auto path = find_path(RG);
    if ( path.size() == 0 ) {
      // 見つからなかった．
      break;
    }
    // 増加路のフローを更新する．
    update_flow(RG, path, flow);
  }
}

END_NAMESPACE_YM
