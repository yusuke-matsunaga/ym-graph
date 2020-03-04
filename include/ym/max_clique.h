#ifndef MAX_CLIQUE_H
#define MAX_CLIQUE_H

/// @file max_clique.h
/// @brief max_clique のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2020 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


BEGIN_NAMESPACE_YM

/// @brief DIMACS 形式のファイルを読み込む．
/// @param[in] s 入力のストリーム
/// @return ノード数と枝のリストを返す．
///
/// * 枝は両端のノード番号のペアで表す．
pair<int, vector<pair<int, int>>>
read_dimacs(istream& s);

/// @brief DIMACS 形式のファイルを読み込む．
/// @param[in] filename ファイル名
/// @return ノード数と枝のリストを返す．
///
/// * 枝は両端のノード番号のペアで表す．
pair<int, vector<pair<int, int>>>
read_dimacs(const string& filename);

/// @brief DIMACS 形式のファイルを出力する．
/// @param[in] s 出力のストリーム
/// @param[in] node_num 対象のグラフのノード数
/// @param[in] edge_list 対象のグラフの枝のリスト
///
/// * 枝は両端のノード番号のペアで表す．
void
write_dimacs(ostream& s,
	     int node_num,
	     const vector<pair<int, int>>& edge_list);

/// @brief (最大)クリークを求める．
/// @param[in] node_num 対象のグラフのノード数
/// @param[in] edge_list 対象のグラフの枝のリスト
/// @param[in] algorithm アルゴリズム名
/// @return クリークの要素(ノード番号)を収める配列を返す．
///
/// * 枝は両端のノード番号のペアで表す．
extern
vector<int>
max_clique(int node_num,
	   const vector<pair<int, int>>& edge_list,
	   const string& algorithm = string());

/// @brief (最大)独立集合を求める．
/// @param[in] node_num 対象のグラフのノード数
/// @param[in] edge_list 対象のグラフの枝のリスト
/// @param[in] algorithm アルゴリズム名
/// @return 独立集合の要素(ノード番号)を収める配列を返す．
///
/// * 枝は両端のノード番号のペアで表す．
extern
int
independent_set(int node_num,
		const vector<pair<int, int>>& edge_list,
		const string& algorithm = string());

END_NAMESPACE_YM

#endif // MAX_CLIQUE_H
