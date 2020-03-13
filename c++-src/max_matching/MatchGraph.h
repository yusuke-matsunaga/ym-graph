#ifndef MATCHGRAPH_H
#define MATCHGRAPH_H

/// @file MatchGraph.h
/// @brief MatchGraph のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2020 Yusuke Matsunaga
/// All rights reserved.

#include "ym/UdGraph.h"
#include "MgNode.h"
#include "MgEdge.h"


BEGIN_NAMESPACE_YM_UDGRAPH

//////////////////////////////////////////////////////////////////////
/// @class MatchGraph MatchGraph.h "MatchGraph.h"
/// @brief 最大マッチングを求めるためのクラス
//////////////////////////////////////////////////////////////////////
class MatchGraph
{
public:

  /// @brief コンストラクタ
  /// @param[in] src 元のグラフ
  MatchGraph(const UdGraph& src);

  /// @brief デストラクタ
  ~MatchGraph();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 最大重みマッチングを求める．
  vector<int>
  max_matching();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 重み最大の交互路を見つける．
  vector<MgEdge*>
  find_path();

  /// @brief パスを復元する．
  vector<MgEdge*>
  make_path(MgNode* node,
	    MgEdge* edge,
	    int phase);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 頂点のリスト
  vector<MgNode*> mNodeList;

  // 枝のリスト
  vector<MgEdge*> mEdgeList;

};

END_NAMESPACE_YM_UDGRAPH

#endif // MATCHGRAPH_H
