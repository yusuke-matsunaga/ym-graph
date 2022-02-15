
/// @file max_flow.cc
/// @brief max_flow の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2020 Yusuke Matsunaga
/// All rights reserved.


#include "ym/FlowGraph.h"


BEGIN_NAMESPACE_YM

// @brief max-flow 問題を解く．
// @param[in] graph 対象のグラフ
// @param[in] start 問題の始点
// @param[in] end 問題の終点
// @return 全体のフロー量と各枝に流れるフローのリストを返す．
tuple<int, vector<FgFlow>>
max_flow(const FlowGraph& graph,
	 int start,
	 int end)
{
}

END_NAMESPACE_YM
