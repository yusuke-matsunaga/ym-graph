#ifndef YM_FLOWGRAPH_H
#define YM_FLOWGRAPH_H

/// @file FlowGraph.h
/// @brief FlowGraph のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2020 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class FgEdge FlowGraph.h "ym/FlowGraph.h"
/// @brief FlowGraph の枝を表すクラス
//////////////////////////////////////////////////////////////////////
class FgEdge
{
public:

  /// @brief コンストラクタ
  /// @param[in] from 始点のノード番号
  /// @param[in] to 終点のノード番号
  /// @param[in] cap 容量
  FgEdge(int from,
	 int to,
	 int cap);

  /// @brief デストラクタ
  ~FgEdge() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 始点のノード番号を返す．
  int
  from() const;

  /// @brief 終点のノード番号を返す．
  int
  to() const;

  /// @brief 容量を返す．
  int
  cap() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 始点
  int mFrom;

  // 終点
  int mTo;

  // 容量
  int mCap;

};


//////////////////////////////////////////////////////////////////////
/// @class FlowGraph FlowGraph.h "FlowGraph.h"
/// @brief flow graph を表すクラス
///
/// とは言っても問題を表すだけのシンプルなクラス
/// ノード数と枝のリストしか持たない．
//////////////////////////////////////////////////////////////////////
class FlowGraph
{
public:

  /// @brief コンストラクタ
  /// @param[in] node_num ノード数
  /// @param[in] edge_list 枝のリスト
  FlowGraph(int node_num,
	    const vector<FgEdge>& edge_list);

  /// @brief コピーコンストラクタ
  FlowGraph(const FlowGraph& src) = default;

  /// @brief コピー代入
  FlogGraph&
  operator=(const FlowGraph& src) = default;

  /// @brief ムーブコンストラクタ
  FLowGraph(FlowGraph&& src) = default;

  /// @brief ムーブ代入
  FlowGraph&
  operator=(FoowGraph&& src) = default;

  /// @brief デストラクタ
  ~FlowGraph() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード数を返す．
  int
  node_num() const;

  /// @brief 枝のリストを返す．
  const vector<FgEdge>&
  edge_list() const;

  /// @brief max-flow 問題を解く．
  /// @param[in] start 問題の始点
  /// @param[in] end 問題の終点
  /// @return 全体のフロー量と各枝に流れるフローのリストを返す．
  tuple<int, vector<int>>
  max_flow(int start,
	   int end) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード数
  int mNodeNum;

  // 枝のリスト
  vector<FgEdge> mEdgeList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] from 始点のノード番号
// @param[in] to 終点のノード番号
// @param[in] cap 容量
inline
FgEdge::FgEdge(int from,
	       int to,
	       int cap) :
  mFrom{from},
  mTo{to},
  mCap{cap}
{
}

// @brief 始点のノード番号を返す．
inline
int
FgEdge::from() const
{
  return mFrom;
}

// @brief 終点のノード番号を返す．
inline
int
FgEdge::to() const
{
  return mTo;
}

// @brief 容量を返す．
inline
int
FgEdge::cap() const
{
  return mCap;
}

// @brief コンストラクタ
// @param[in] node_num ノード数
// @param[in] edge_list 枝のリスト
inline
FlowGraph::FlowGraph(int node_num,
		     const vector<FgEdge>& edge_list) :
  mNodeNum{node_num},
  mEdgeList{edge_list}
{
}

// @brief ノード数を返す．
inline
int
FlowGraph::node_num() const
{
  return mNodeNum;
}

// @brief 枝のリストを返す．
inline
const vector<FgEdge>&
FlogGraph::edge_list() const
{
  return mEdgeList;
}

END_NAMESPACE_YM

#endif // YM_FLOWGRAPH_H
