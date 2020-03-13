#ifndef MGNODE_H
#define MGNODE_H

/// @file MgNode.h
/// @brief MgNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2020 Yusuke Matsunaga
/// All rights reserved.

#include "ym/UdGraph.h"


BEGIN_NAMESPACE_YM_UDGRAPH

class MgEdge;

//////////////////////////////////////////////////////////////////////
/// @class MgNode MgNode.h "MgNode.h"
/// @brief MatchGraph のノードを表すクラス
//////////////////////////////////////////////////////////////////////
class MgNode
{
public:

  // 番号
  int id;

  // 枝のリスト
  vector<MgEdge*> edge_list;

  // 現在の値のリスト
  vector<int> value_list;

  // 増加路上の枝のリスト
  vector<MgEdge*> alt_edge_list;

  // 選択されている枝を返す．
  MgEdge*
  selected_edge() const;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

END_NAMESPACE_YM_UDGRAPH

#endif // MGNODE_H
