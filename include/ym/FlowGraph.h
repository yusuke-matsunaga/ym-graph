#ifndef YM_FLOWGRAPH_H
#define YM_FLOWGRAPH_H

/// @file FlowGraph.h
/// @brief FlowGraph のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2020 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


/// @brief udgraph 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_FLOWGRAPH \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsFlowGraph)

/// @brief udgraph 用の名前空間の終了
#define END_NAMESPACE_YM_FLOWGRAPH \
END_NAMESPACE(nsFlowGraph) \
END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_FLOWGRAPH

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

  /// @brief 枝を表すクラス
  struct Edge
  {
    // 始点
    int from;

    // 終点
    int to;

    // 容量
    int cap;
  };


public:

  /// @brief コンストラクタ
  /// @param[in] node_num ノード数
  /// @param[in] edge_list 枝のリスト
  FlowGraph(int node_num,
	    const vector<Edge>& edge_list);

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

  /// @brief 枝数を返す．
  int
  edge_num() const;

  /// @brief 枝を返す．
  /// @param[in] pos 枝番号 ( 0 <= pos < edge_num() )
  /// @return 枝を返す．
  const Edge&
  edge(int pos) const;

  /// @brief 枝の始点を返す．
  /// @param[in] pos 枝番号 ( 0 <= pos < edge_num() )
  int
  edge_from(int pos) const;

  /// @brief 枝の終点を返す．
  /// @param[in] pos 枝番号 ( 0 <= pos < edge_num() )
  int
  edge_to(int pos) const;

  /// @brief 枝の容量を返す．
  /// @param[in] pos 枝番号 ( 0 <= pos < edge_num() )
  int
  edge_cap(int pos) const;

  /// @brief 枝のリストを返す．
  const vector<Edge>&
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

// @brief 枝数を返す．
inline
int
FlowGraph::edge_num() const
{
  return mEdgeList.size();
}

// @brief 枝を返す．
// @param[in] pos 枝番号 ( 0 <= pos < edge_num() )
// @return 枝を返す．
inline
const FlowGraph::Edge&
FlowGraph::edge(int pos) const
{
  ASSERT_COND( 0 <= pos && pos < edge_num() );

  return mEdgeList[pos];
}

// @brief 枝の始点を返す．
// @param[in] pos 枝番号 ( 0 <= pos < edge_num() )
inline
int
FlowGraph::edge_from(int pos) const
{
  return edge(pos).from;
}

// @brief 枝の終点を返す．
// @param[in] pos 枝番号 ( 0 <= pos < edge_num() )
inline
int
FlowGraph::edge_to(int pos) const
{
  return edge(pos).to;
}

// @brief 枝の容量を返す．
// @param[in] pos 枝番号 ( 0 <= pos < edge_num() )
inline
int
FlowGraph::edge_cap(int pos) const
{
  return edge(pos).cap;
}

// @brief 枝のリストを返す．
inline
const vector<Edge>&
FlowGraph::edge_list() const
{
  return mEdgeList;
}

END_NAMESPACE_YM_FLOWGRAPH

BEGIN_NAMESPACE_YM

using nsFlowGraph::FlowGraph;

END_NAMESPACE_YM

#endif // YM_FLOWGRAPH_H
