#ifndef MGEDGE_H
#define MGEDGE_H

/// @file MgEdge.h
/// @brief MgEdge のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2020 Yusuke Matsunaga
/// All rights reserved.

#include "ym/UdGraph.h"


BEGIN_NAMESPACE_YM_UDGRAPH

class MgNode;

//////////////////////////////////////////////////////////////////////
/// @class MgEdge MgEdge.h "MgEdge.h"
/// @brief MatchGraph の枝を表すクラス
//////////////////////////////////////////////////////////////////////
class MgEdge
{
public:

  /// @brief 番号
  int id;

  /// @brief 頂点1
  MgNode* node1;

  /// @brief 頂点2
  MgNode* node2;

  /// @brief 重み
  int weight;

  /// @brief 選択されている時 true となるフラグ
  bool selected{false};

  /// @brief 反対側の頂点を返す．
  MgNode*
  alt_node(MgNode* node)
  {
    return ( node == node1 ) ? node2 : node1;
  }

};

/// @brief 枝を比較するファンクタクラス
class EdgeLt
{
public:

  bool
  operator()(const MgEdge* left,
	     const MgEdge* right)
  {
    return left->weight > right->weight;
  }
};

END_NAMESPACE_YM_UDGRAPH

#endif // MGEDGE_H
