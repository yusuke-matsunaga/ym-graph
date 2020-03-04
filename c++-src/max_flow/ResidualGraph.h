#ifndef RESIDUALGRAPH_H
#define RESIDUALGRAPH_H

/// @file ResidualGraph.h
/// @brief ResidualGraph のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2020 Yusuke Matsunaga
/// All rights reserved.

#include "ym/FlowGraph.h"


/// @brief max_flow 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_MAXFLOW \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsMaxFlow)

/// @brief mincov 用の名前空間の終了
#define END_NAMESPACE_YM_MAXFLOW \
END_NAMESPACE(nsMaxFlow) \
END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_MAXFLOW

class RgNode;

//////////////////////////////////////////////////////////////////////
/// @class RgEdge ResidualGraph.h "ResidualGraph.h"
/// @brief ResidualGraph の枝を表すクラス
//////////////////////////////////////////////////////////////////////
class RgEdge
{
public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] from 始点のノード
  /// @param[in] to 終点のノード
  /// @param[in] cap 容量
  RgEdge(int id,
	 RgNode& from,
	 RgNode& to,
	 int cap);

  /// @brief デストラクタ
  ~RgEdge() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  int
  id() const;

  /// @brief 始点のノードを返す．
  RgNode&
  from() const;

  /// @brief 終点のノードを返す．
  RgNode&
  to() const;

  /// @brief 容量を返す．
  int
  cap() const;

  /// @brief フローを返す．
  int
  flow() const;

  /// @brief フローを増やす
  /// @param[in] delta フローの増分
  ///
  /// delta が負の場合には減る．
  void
  change_flow(int delta);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  int mId;

  // 始点のノード
  RgNode& mFrom;

  // 終点のノード
  RgNode& mTo;

  // 容量
  int mCap;

  // フロー
  int mFlow;

};


//////////////////////////////////////////////////////////////////////
/// @class RgNode ResidualGraph.h "ResidualGraph.h"
/// @brief ResidualGraph のノードを表すクラス
//////////////////////////////////////////////////////////////////////
class RgNode
{
public:

  /// @brief コンストラクタ
  RgNode() = default;

  /// @brief コピーコンストラクタは禁止
  RgNode(const RgNode& src) = delete;

  /// @brief コピー代入も禁止
  RgNode&
  operator=(const RgNode& src) = delete;

  /// @brief ムーブコンストラクタは禁止
  RgNode(RgNode&& src) = delete;

  /// @brief ムーブ代入も禁止
  RgNode&
  operator=(RgNode& src) = delete;

  /// @brief デストラクタ
  ~RgNode() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 出枝のリストを返す．
  const vector<RgEdge>&
  outedge_list() const;

  /// @brief 入枝のリストを返す．
  const vector<RgEdge>&
  inedge_list() const;

  /// @brief このノードに流せる増加路のフローを返す．
  int
  flow() const;

  /// @brief フロー情報を設定する．
  /// @param[in] flow フロー値
  /// @param[in] edge このフローを流している枝
  /// @param[in] forward 順方向の枝の時 true
  void
  set_flow(int flow,
	   RgEdge* edge,
	   bool foward);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出枝のリスト
  vector<RgEdge> mOutEdgeList;

  // 入枝のリスト
  vector<RgEdge> mInEdgeList;

  // フロー
  int mFlow{0};

  // フローを流している枝
  RgEdge* mEdge{nullptr};

  // 枝の向き
  bool mFoward{false};

};


//////////////////////////////////////////////////////////////////////
/// @class ResidualGraph ResidualGraph.h "ResidualGraph.h"
/// @brief 残余グラフを表すクラス
//////////////////////////////////////////////////////////////////////
class ResidualGraph
{
public:

  /// @brief コンストラクタ
  /// @param[in] src_graph 元となるフローグラフ
  /// @param[in] start 始点
  /// @param[in] end 終点
  ResidualGraph(const FlowGraph& src_graph,
		int start,
		int end);

  /// @brief デストラクタ
  ~ResidualGraph();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード数を返す．
  int
  node_num() const;

  /// @brief ノードを返す．
  /// @param[in] id ノード番号 ( 0 <= id < node_num() )
  const RgNode&
  node(int id);

  /// @brief 始点のノードを返す．
  const RgNode&
  start_node();

  /// @brief 終点のノードを返す．
  const RgNode&
  end_node();

  /// @brief 枝数を返す．
  int
  edge_num() const;

  /// @brief 枝を返す．
  /// @param[in] id 枝番号 ( 0 <= id < edge_num() )
  const RgEdge&
  edge(int id) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノードのリスト
  vector<RgNode> mNodeList;

  // 始点のノード
  const RgNode& mStartNode;

  // 終点のノード
  const RgNode& mEndNode;

  // 枝のリスト
  vector<RgEdge*> mEdgeList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] from 始点のノード
// @param[in] to 終点のノード
// @param[in] cap 容量
inline
RgEdge::RgEdge(int id,
	       RgNode& from,
	       RgNode& to,
	       int cap) :
  mId{id},
  mFrom{from},
  mTo{to},
  mCap{cap}
{
}

// @brief ID番号を返す．
inline
int
RgEdge::id() const
{
  return mId;
}

// @brief 始点のノードを返す．
inline
const RgNode&
RgEdge::from() const
{
  return mFrom;
}

// @brief 終点のノードを返す．
inline
const RgNode&
RgEdge::to() const
{
  return mTo;
}

// @brief 容量を返す．
inline
int
RgEdge::cap() const
{
  return mCap;
}

// @brief フローを返す．
inline
int
RgEdge::flow() const
{
  return mFlow;
}

// @brief フローを増やす
// @param[in] delta フローの増分
//
// delta が負の場合には減る．
inline
void
RgEdge::change_flow(int delta)
{
  mFLow += delta;
}

// @brief 出枝のリストを返す．
inline
const vector<RgEdge>&
RgNode::outedge_list() const
{
  return mOutEdgeList;
}

// @brief 入枝のリストを返す．
inline
const vector<RgEdge>&
RgNode::inedge_list() const
{
  return mInEdgeList;
}

// @brief このノードに流せる増加路のフローを返す．
inline
int
RgNode::flow() const
{
  return mFlow;
}

// @brief フロー情報を設定する．
// @param[in] flow フロー値
// @param[in] edge このフローを流している枝
// @param[in] forward 順方向の枝の時 true
inline
void
RgNode::set_flow(int flow,
		 RgEdge* edge,
		 bool foward)
{
  mFlow = flow;
  mEdge = edge;
  mForward = forward;
}

// @brief ノード数を返す．
inline
int
ResidualGraph::node_num() const
{
  return mNodeList.size();
}

// @brief ノードを返す．
// @param[in] id ノード番号 ( 0 <= id < node_num() )
inline
const RgNode&
ResidualGraph::node(int id)
{
  ASSERT_COND( 0 <= id && id < node_num() );
  return mNodeList[id];
}

// @brief 始点のノードを返す．
inline
const RgNode&
ResidualGraph::start_node()
{
  return mStartNode;
}

// @brief 終点のノードを返す．
inline
const RgNode&
ResidualGraph::end_node()
{
  return mEndNode;
}

// @brief 枝数を返す．
inline
int
ResidualGraph::edge_num() const
{
  return mEdgeList.size();
}

// @brief 枝を返す．
// @param[in] id 枝番号 ( 0 <= id < edge_num() )
inline
const RgEdge&
ResidualGraph::edge(int id) const
{
  ASSERT_COND( 0 <= id && id < edge_num() );
  return *mEdgeList[id];
}

END_NAMESPACE_YM_MAXFLOW

#endif // RESIDUALGRAPH_H
