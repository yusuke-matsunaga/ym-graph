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

  // 番号
  int id;

  // 頂点1
  MgNode* node1;

  // 頂点2
  MgNode* node2;

  // 重み
  int weight;

  // 選択されている時 true となるフラグ
  bool selected{false};

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
