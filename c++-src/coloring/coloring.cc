
/// @file coloring.cc
/// @brief coloring の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/UdGraph.h"
#include "Dsatur.h"
#include "IsCov.h"
#include "Isx.h"
#include "Isx2.h"
#include "TabuCol.h"


BEGIN_NAMESPACE_YM

// dsatur で彩色問題を解く．
int
dsatur(const UdGraph& graph,
       vector<int>& color_map)
{
  nsUdGraph::Dsatur dsatsolver(graph, color_map);
  return dsatsolver.coloring(color_map);
}

// tabucol で彩色問題を解く．
int
tabucol(const UdGraph& graph,
	vector<int>& color_map)
{
  int k0 = dsatur(graph, color_map);
  int limit = 100000;
  int L = 9;
  double alpha = 0.6;
  int k1 = k0;
  for ( int k = k0; k > 0; -- k ) {
    nsUdGraph::TabuCol tabucol(graph, k);
    vector<int> color_map1;
    if ( tabucol.coloring(limit, L, alpha, color_map1) ) {
      k1 = k;
      color_map = color_map1;
    }
    else {
      break;
    }
  }
  return k1;
}

// @brief 彩色問題を解く
// @param[in] algorithm アルゴリズム名
// @return 彩色数とノードに対する彩色結果(=int)を収める配列(vector)を返す．
//
// 結果の配列のサイズは node_num()
pair<int, vector<int>>
UdGraph::coloring(const string& algorithm) const
{
  vector<int> color_map;
  int nc;
  if ( algorithm == "dsatur" ) {
    nc = dsatur(*this, color_map);
  }
  else if ( algorithm == "iscov" ) {
    nsUdGraph::IsCov iscsolver(*this);
    int c = iscsolver.covering(500, color_map);
    nsUdGraph::Dsatur dsatsolver(*this, color_map);
    nc = dsatsolver.coloring(color_map);
  }
  else if ( algorithm == "isx" ) {
    nsUdGraph::Isx isxsolver(*this);
    int c = isxsolver.coloring(500, color_map);
    //cout << "isx end: c = " << c << endl;
    nsUdGraph::Dsatur dsatsolver(*this, color_map);
    //return dsatsolver.coloring(color_map);
    nc = dsatsolver.coloring(color_map);
    //cout << "dsatur end: c = " << nc << endl;
  }
  else if ( algorithm == "isx2" ) {
    nsUdGraph::Isx2 isxsolver(*this);
    int c = isxsolver.coloring(500, color_map);
    //cout << "isx2 end: c = " << c << endl;
    nsUdGraph::Dsatur dsatsolver(*this, color_map);
    //return dsatsolver.coloring(color_map);
    nc = dsatsolver.coloring(color_map);
    //cout << "dsatur end: c = " << nc << endl;
  }
  else if ( algorithm == "tabucol" ) {
    nc = tabucol(*this, color_map);
  }
  else {
    // デフォルトフォールバック
    nc = dsatur(*this, color_map);
  }
  return {nc, color_map};
}

END_NAMESPACE_YM
