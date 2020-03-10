#ifndef BIGRAPH_H
#define BIGRAPH_H

/// @file BiGraph.h
/// @brief BiGraph のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2020 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


/// @brief bigraph 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_BIGRAPH \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsBiGraph)

/// @brief bigraph 用の名前空間の終了
#define END_NAMESPACE_YM_BIGRAPH \
END_NAMESPACE(nsBiGraph) \
END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_BIGRAPH

//////////////////////////////////////////////////////////////////////
/// @class BiGraph BiGraph.h "ym/BiGraph.h"
/// @brief bipartite graph を表すクラス
///
/// bipartite graph とは頂点が2つの集合V1とV2に分割されており，
/// 同じ集合に属する頂点間に枝を持たない無向グラフのこと．
//////////////////////////////////////////////////////////////////////
class BiGraph
{
public:

  /// @brief 枝を表すクラス
  struct Edge
  {
    // 頂点1側ののノード番号
    int id1;

    // 頂点2側ののノード番号
    int id2;

    // 重み
    int weight{1};
  };


public:

  /// @brief 空のコンストラクタ
  BiGraph() = default;

  /// @brief コンストラクタ
  /// @param[in] node1_num 頂点集合1の要素数
  /// @param[in] node2_num 頂点集合2の要素数
  /// @param[in] edge_list 枝のリスト
  BiGraph(int node1_num,
	  int node2_num,
	  const vector<Edge>& edge_list = vector<Edge>());

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  BiGraph(const BiGraph& src) = default;

  /// @brief コピー代入演算子
  /// @param[in] src コピー元のオブジェクト
  BiGraph&
  operator=(const BiGraph& src) = default;

  /// @brief ムーブコンストラクタ
  /// @param[in] src ムーブ元のオブジェクト
  BiGraph(BiGraph&& src) = default;

  /// @brief ムーブ代入演算子
  BiGraph&
  operator=(BiGraph&& src) = default;

  /// @brief デストラクタ
  ~BiGraph() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード数を設定する．
  /// @param[in] node1_num 頂点集合1の要素数
  /// @param[in] node2_num 頂点集合2の要素数
  ///
  /// 以前の内容はクリアされる．
  void
  resize(int node1_num,
	 int node2_num);

  /// @brief 枝を追加する．
  /// @param[in] id1, id2 枝の両端のノード番号
  /// @param[in] weight 枝の重み
  void
  add_edge(int id1,
	   int id2,
	   int weight = 1);


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取得する外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 頂点集合1の要素数を返す．
  int
  node1_num() const;

  /// @brief 頂点集合2の要素数を返す．
  int
  node2_num() const;

  /// @brief 枝の総数を返す．
  int
  edge_num() const;

  /// @brief 枝の情報を返す．
  /// @param[in] idx 枝番号 ( 0 <= idx < edge_num() )
  const Edge&
  edge(int idx) const;

  /// @brief 枝の端点1を返す．
  /// @param[in] idx 枝番号 ( 0 <= idx < edge_num() )
  int
  edge_id1(int idx) const;

  /// @brief 枝の端点2を返す．
  /// @param[in] idx 枝番号 ( 0 <= idx < edge_num() )
  int
  edge_id2(int idx) const;

  /// @brief 枝の重みを返す．
  /// @param[in] idx 枝番号 ( 0 <= idx < edge_num() )
  int
  edge_weight(int idx) const;

  /// @brief 枝のリストを返す．
  const vector<Edge>&
  edge_list() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ファイル入出力関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ストリームから読み込む．
  /// @param[in] s 入力ストリーム
  /// @return 結果のグラフを返す．
  static
  BiGraph
  read(istream& s);

  /// @brief ファイルから読み込む．
  /// @param[in] filename ファイル名
  /// @return 結果のグラフを返す．
  static
  BiGraph
  read(const string& filename);

  /// @brief ストリームに書き出す．
  /// @param[in] s 出力ストリーム
  void
  write(ostream& s) const;

  /// @brief ファイルに書き出す．
  /// @param[in] filename ファイル名
  void
  write(const string& filename);


public:
  //////////////////////////////////////////////////////////////////////
  // マッチングアルゴリズム
  //////////////////////////////////////////////////////////////////////

  /// @brief 最大重みマッチングを求める．
  /// @return マッチング結果の枝番号のリストを返す．
  vector<int>
  max_matching() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 頂点集合1の要素数
  int mNode1Num{0};

  // 頂点集合2の要素数
  int mNode2Num{0};

  // 枝のリスト
  vector<Edge> mEdgeList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ノード数を設定する．
// @param[in] node1_num 頂点集合1の要素数
// @param[in] node2_num 頂点集合2の要素数
//
// 以前の内容はクリアされる．
inline
void
BiGraph::resize(int node1_num,
		int node2_num)
{
  mNode1Num = node1_num;
  mNode2Num = node2_num;
  mEdgeList.clear();
}

// @brief 枝を追加する．
// @param[in] id1, id2 枝の両端のノード番号
// @param[in] weight 枝の重み
inline
void
BiGraph::add_edge(int id1,
		  int id2,
		  int weight)
{
  ASSERT_COND( 0 <= id1 && id1 < node1_num() );
  ASSERT_COND( 0 <= id2 && id2 < node2_num() );

  mEdgeList.push_back({id1, id2, weight});
}

// @brief 頂点集合1の要素数を返す．
inline
int
BiGraph::node1_num() const
{
  return mNode1Num;
}

// @brief 頂点集合2の要素数を返す．
inline
int
BiGraph::node2_num() const
{
  return mNode2Num;
}

// @brief 枝の総数を返す．
inline
int
BiGraph::edge_num() const
{
  return mEdgeList.size();
}

// @brief 枝の情報を返す．
// @param[in] idx 枝番号 ( 0 <= idx < edge_num() )
// @return id1, id2, weight のタプルを返す．
inline
const BiGraph::Edge&
BiGraph::edge(int idx) const
{
  ASSERT_COND( 0 <= idx && idx < edge_num() );
  return mEdgeList[idx];
}

// @brief 枝の端点1を返す．
// @param[in] idx 枝番号 ( 0 <= idx < edge_num() )
inline
int
BiGraph::edge_id1(int idx) const
{
  return edge(idx).id1;
}

// @brief 枝の端点2を返す．
// @param[in] idx 枝番号 ( 0 <= idx < edge_num() )
inline
int
BiGraph::edge_id2(int idx) const
{
  return edge(idx).id2;
}

// @brief 枝の重みを返す．
// @param[in] idx 枝番号 ( 0 <= idx < edge_num() )
inline
int
BiGraph::edge_weight(int idx) const
{
  return edge(idx).weight;
}

// @brief 枝のリストを返す．
inline
const vector<BiGraph::Edge>&
BiGraph::edge_list() const
{
  return mEdgeList;
}

END_NAMESPACE_YM_BIGRAPH

BEGIN_NAMESPACE_YM

using nsBiGraph::BiGraph;

END_NAMESPACE_YM

#endif // BIGRAPH_H
