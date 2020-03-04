#ifndef BIPARTITEGRAPH_H
#define BIPARTITEGRAPH_H

/// @file BipartiteGraph.h
/// @brief BipartiteGraph のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2020 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class BgEdge BgGraph.h "ym/BgGraph.h"
/// @brief BipartiteGraph の枝を表すクラス
//////////////////////////////////////////////////////////////////////
class BgEdge
{
public:

  /// @brief コンストラクタ
  /// @param[in] l_id 左側のノード番号
  /// @param[in] r_id 右側のノード番号
  /// @param[in] w 枝の重み
  BgEdge(int l_id,
	 int r_id,
	 int w);

  /// @brief デストラクタ
  ~BgEdge() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 左側のノード番号を返す．
  int
  left_id() const;

  /// @brief 右側のノード番号を返す．
  int
  right_id() const;

  /// @brief 重みを返す．
  int
  weight() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 左側のノード番号
  int mLeftId;

  // 右側のノード番号
  int mRightId;

  // 重み
  int mWeight;

};


//////////////////////////////////////////////////////////////////////
/// @class BipartiteGraph BipartiteGraph.h "ym/BipartiteGraph.h"
/// @brief 二部グラフを表すクラス
//////////////////////////////////////////////////////////////////////
class BipartiteGraph
{
public:

  /// @brief コンストラクタ
  /// @param[in] l_num 左側のノード数
  /// @param[in] r_num 右側のノード数
  /// @param[in] edge_list 枝のリスト
  BipartiteGraph(int l_num,
		 int r_num,
		 const vector<BgEdge> edge_list);

  /// @brief デストラクタ
  ~BipartiteGraph() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 左側のノード数
  int
  left_num() const;

  /// @brief 右側のノード数
  int
  right_num() const;

  /// @brief 枝のリストを返す．
  const vector<BgEdge>&
  edge_list() const;

  /// @brief 二部グラフの最大マッチングを求める．
  /// @return 重みの和とマッチに選ばれた枝のリストを返す．
  tuple<int, vector<BgEdge>>
  maximum_matching() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 左側のノード数
  int mLeftNum;

  // 右側のノード数
  int mRightNum;

  // 枝のリスト
  vector<BgEdge> mEdgeList;
};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] l_id 左側のノード番号
// @param[in] r_id 右側のノード番号
// @param[in] w 枝の重み
inline
BgEdge::BgEdge(int l_id,
	       int r_id,
	       int w) :
  mLeftId{l_id},
  mRightId{r_id},
  mWeight{w}
{
}

// @brief 左側のノード番号を返す．
inline
int
BgEdge::left_id() const
{
  return mLeftId;
}

// @brief 右側のノード番号を返す．
inline
int
BgEdge::right_id() const
{
  return mRightId;
}

// @brief 重みを返す．
inline
int
BgEdge::weight() const
{
  return mWeight;
}

// @brief コンストラクタ
// @param[in] l_num 左側のノード数
// @param[in] r_num 右側のノード数
// @param[in] edge_list 枝のリスト
inline
BipartiteGraph::BipartiteGraph(int l_num,
			       int r_num,
			       const vector<BgEdge> edge_list) :
  mLeftNum{l_num},
  mRightNum{r_num},
  mEdgeList{edge_List}
{
}

// @brief 左側のノード数
inline
int
BipartiteGraph::left_num() const
{
  return mLeftNum;
}

// @brief 右側のノード数
inline
int
BipartiteGraph::right_num() const
{
  return mRightNum;
}

// @brief 枝のリストを返す．
inline
const vector<BgEdge>&
BipartiteGraph::edge_list() const
{
  return mEdgeList;
}

END_NAMESPACE_YM

#endif // BIPARTITEMATCH_H
