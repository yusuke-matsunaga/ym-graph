#ifndef YM_UDGRAPH_H
#define YM_UDGRAPH_H

/// @file ym/UdGraph.h
/// @brief UdGraph のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015, 2016, 2018, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "ym_config.h"


/// @brief udgraph 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_UDGRAPH \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsUdGraph)

/// @brief udgraph 用の名前空間の終了
#define END_NAMESPACE_YM_UDGRAPH \
END_NAMESPACE(nsUdGraph) \
END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_UDGRAPH

//////////////////////////////////////////////////////////////////////
/// @class UdGraph UdGraph.h "ym/UdGraph.h"
/// @brief 一般的な無向グラフを表すクラス
///
/// - Undirected Graph の略
/// - 枝(ノード番号の対)のリストを持つ．
/// - このクラスはインターフェイス用のもので凝ったデータ構造は持っていない．
//////////////////////////////////////////////////////////////////////
class UdGraph
{
public:

  /// @brief 枝を表すクラス
  ///
  /// 主にコンストラクタの初期化リストで
  /// 使うことを考えている．
  struct Edge
  {
    /// @brief 両端のノード番号
    ///
    /// id1 が小さくなるように正規化する．
    int id1, id2;

    /// @brief 枝の重み
    int weight{1};
  };


public:

  /// @brief 空のコンストラクタ
  UdGraph() = default;

  /// @brief コンストラクタ
  /// @param[in] node_num ノード数
  /// @param[in] edge_list 枝のリスト
  explicit
  UdGraph(int node_num,
	  const vector<Edge>& edge_list = vector<Edge>());

  /// @brief コピーコンストラクタ
  UdGraph(const UdGraph& src) = default;

  // @brief コピー代入演算子
  UdGraph&
  operator=(const UdGraph& src) = default;

  /// @brief ムーブコンストラクタ
  UdGraph(UdGraph&& src) = default;

  /// @brief ムーブ代入演算子
  UdGraph&
  operator=(UdGraph&& src) = default;

  /// @brief デストラクタ
  ~UdGraph() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード数を設定する．
  /// @param[in] node_num ノード数
  ///
  /// 以前の内容はクリアされる．
  void
  resize(int node_num);

  /// @brief 枝を追加する．
  /// @param[in] id1, id2 枝の両端のノード番号
  /// @param[in] weight 枝の重み(省略時は1)
  ///
  /// - id1 と id2 の範囲チェックは行う．
  /// - 重複チェックは行わない．
  void
  add_edge(int id1,
	   int id2,
	   int weight = 1);


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得する外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード数を得る．
  int
  node_num() const;

  /// @brief 枝の総数を返す．
  int
  edge_num() const;

  /// @brief 反射の時に true を返す．
  ///
  /// 反射とはすべてのノードに自己ループがあること
  bool
  is_reflective() const;

  /// @brief 枝の情報を返す．
  /// @param[in] idx 枝番号 ( 0 <= idx < edge_num() )
  /// @return 枝を返す．
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

  /// @brief 全ての枝のリストを返す．
  const vector<Edge>&
  edge_list() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ファイル入出力を行う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief DIMACS 形式のファイルを読み込む．
  /// @param[in] s 入力のストリーム
  /// @return 読み込んだグラフを返す．
  ///
  /// 枝の重みは全て1になる．
  static
  UdGraph
  read_dimacs(istream& s);

  /// @brief DIMACS 形式のファイルを読み込む．
  /// @param[in] filename 入力のファイル名
  /// @return 読み込んだグラフを返す．
  ///
  /// 枝の重みは全て1になる．
  static
  UdGraph
  read_dimacs(const string& filename);

  /// @brief 内容を DIMACS 形式で出力する．
  /// @param[in] s 出力のストリーム
  ///
  /// 枝の重みは無視される．
  void
  write_dimacs(ostream& s) const;

  /// @brief 内容を DIMACS 形式で出力する．
  /// @param[in] filename ファイル名
  ///
  /// 枝の重みは無視される．
  void
  write_dimacs(const string& filename) const;

  /// @brief 独自形式のファイルを読み込む．
  /// @param[in] s 入力のストリーム
  ///
  /// dump() で出力した形式に対応している．
  static
  UdGraph
  restore(istream& s);

  /// @brief 独自形式のファイルを読み込む．
  /// @param[in] filename 入力ファイル名
  ///
  /// dump() で出力した形式に対応している．
  static
  UdGraph
  restore(const string& filename);

  /// @brief 独自形式でファイルに出力する．
  /// @param[in] s 出力のストリーム
  void
  dump(ostream& s) const;

  /// @brief 独自形式でファイルに出力する．
  /// @param[in] filename 出力ファイル名
  void
  dump(const string& filename) const;


public:
  //////////////////////////////////////////////////////////////////////
  // グラフアルゴリズム
  //////////////////////////////////////////////////////////////////////

  /// @brief 彩色問題を解く
  /// @param[in] algorithm アルゴリズム名
  /// @return 彩色数とノードに対する彩色結果(=int)を収める配列(vector)を返す．
  ///
  /// 結果の配列のサイズは node_num()
  pair<int, vector<int>>
  coloring(const string& algorithm = string()) const;

  /// @brief (最大)独立集合を求める．
  /// @param[in] algorithm アルゴリズム名
  /// return 独立集合の要素(ノード番号)を収める配列を返す．
  vector<int>
  independent_set(const string& algorithm = string()) const;

  /// @brief (最大)クリークを求める．
  /// @param[in] algorithm アルゴリズム名
  /// @return クリークの要素(ノード番号)を収める配列を返す．
  vector<int>
  max_clique(const string& algorithm = string()) const;

  /// @brief 最大重みマッチングを求める．
  /// @param[in] algorithm アルゴリズム名
  /// @return マッチングに選ばれた枝番号のリストを返す．
  vector<int>
  max_matching(const string& algorithm = string()) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード数
  int mNodeNum{0};

  // 枝の実体の配列
  vector<Edge> mEdgeList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ノード数を設定する．
// @param[in] node_num ノード数
//
// 以前の内容はクリアされる．
inline
void
UdGraph::resize(int node_num)
{
  mNodeNum = node_num;
  mEdgeList.clear();
}

// @brief 枝を追加する．
// @param[in] id1, id2 枝の両端のノード番号
// @param[in] weight 枝の重み
//
// - id1 と id2 の範囲チェックは行う．
// - 重複チェックは行わない．
inline
void
UdGraph::add_edge(int id1,
		  int id2,
		  int weight)
{
  ASSERT_COND( 0 <= id1 && id1 < node_num() );
  ASSERT_COND( 0 <= id2 && id2 < node_num() );

  // 常に id1 <= id2 になるように順序を正規化する．
  if ( id1 > id2 ) {
    swap(id1, id2);
  }
  mEdgeList.push_back({id1, id2, weight});
}

// @brief ノード数を得る．
inline
int
UdGraph::node_num() const
{
  return mNodeNum;
}

// @brief 枝の総数を返す．
inline
int
UdGraph::edge_num() const
{
  return mEdgeList.size();
}

// @brief 枝の情報を返す．
// @param[in] idx 枝番号 ( 0 <= idx < edge_num() )
// @return 枝を返す．
inline
const UdGraph::Edge&
UdGraph::edge(int idx) const
{
  ASSERT_COND( 0 <= idx && idx < edge_num() );
  return mEdgeList[idx];
}

// @brief 枝の端点1を返す．
// @param[in] idx 枝番号 ( 0 <= idx < edge_num() )
inline
int
UdGraph::edge_id1(int idx) const
{
  return edge(idx).id1;
}

// @brief 枝の端点2を返す．
// @param[in] idx 枝番号 ( 0 <= idx < edge_num() )
inline
int
UdGraph::edge_id2(int idx) const
{
  return edge(idx).id2;
}

// @brief 枝の重みを返す．
// @param[in] idx 枝番号 ( 0 <= idx < edge_num() )
inline
int
UdGraph::edge_weight(int idx) const
{
  return edge(idx).weight;
}

// @brief 全ての枝のリストを返す．
inline
const vector<UdGraph::Edge>&
UdGraph::edge_list() const
{
  return mEdgeList;
}

END_NAMESPACE_YM_UDGRAPH

BEGIN_NAMESPACE_YM

using nsUdGraph::UdGraph;

END_NAMESPACE_YM

#endif // YM_UDGRAPH_H
