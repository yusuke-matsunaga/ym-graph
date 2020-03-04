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

  /// @brief コンストラクタ
  /// @param[in] node_num ノード数
  /// @param[in] edge_list 枝のリスト
  explicit
  UdGraph(int node_num = 0,
	  const vector<pair<int, int>>& edge_list = vector<pair<int, int>>());

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
  const pair<int, int>&
  edge(int idx) const;

  /// @brief 全ての枝のリストを返す．
  const vector<pair<int, int>>&
  edge_list() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ファイル入出力を行う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief DIMACS 形式のファイルを読み込む．
  /// @param[in] s 入力のストリーム
  /// @return 読み込んだグラフを返す．
  static
  UdGraph
  read_dimacs(istream& s);

  /// @brief DIMACS 形式のファイルを読み込む．
  /// @param[in] filename 入力のファイル名
  /// @return 読み込んだグラフを返す．
  static
  UdGraph
  read_dimacs(const string& filename);

  /// @brief 内容を DIMACS 形式で出力する．
  /// @param[in] s 出力のストリーム
  void
  write_dimacs(ostream& s) const;

  /// @brief 内容を DIMACS 形式で出力する．
  /// @param[in] filename ファイル名
  void
  write_dimacs(const string& filename) const;


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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード数
  int mNodeNum;

  // 枝の実体の配列
  vector<pair<int, int>> mEdgeList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

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

// @brief 反射の時に true を返す．
//
// 反射とはすべてのノードに自己ループがあること
inline
bool
UdGraph::is_reflective() const
{
}

// @brief 枝の情報を返す．
// @param[in] idx 枝番号 ( 0 <= idx < edge_num() )
// @return 枝を返す．
inline
const pair<int, int>&
UdGraph::edge(int idx) const
{
  ASSERT_COND( 0 <= idx && idx < edge_num() );
  return mEdgeList[idx];
}

// @brief 全ての枝のリストを返す．
inline
const vector<pair<int, int>>&
UdGraph::edge_list() const
{
  return mEdgeList;
}

END_NAMESPACE_YM_UDGRAPH

BEGIN_NAMESPACE_YM

using nsUdGraph::UdGraph;

END_NAMESPACE_YM

#endif // YM_UDGRAPH_H
