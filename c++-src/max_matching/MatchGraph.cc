
/// @file MatchGraph.cc
/// @brief MatchGraph の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2020 Yusuke Matsunaga
/// All rights reserved.


#include "MatchGraph.h"
#include "MgNode.h"
#include "MgEdge.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_YM_UDGRAPH

// @brief 最大重みマッチングを求める．
// @return マッチングに選ばれた枝番号のリストを返す．
vector<int>
UdGraph::max_matching() const
{
  MatchGraph graph(*this);

  return graph.max_matching();
}

//////////////////////////////////////////////////////////////////////
// MatchGraph の関数
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] src 元のグラフ
MatchGraph::MatchGraph(const UdGraph& src) :
  mNodeList(src.node_num()),
  mEdgeList(src.edge_num())
{
  for ( int i: Range(src.node_num()) ) {
    auto node = new MgNode{i};
    mNodeList[i] = node;
  }
  for ( int i: Range(src.edge_num()) ) {
    int id1 = src.edge_id1(i);
    int id2 = src.edge_id2(i);
    int w = src.edge_weight(i);
    auto node1 = mNodeList[id1];
    auto node2 = mNodeList[id2];
    auto edge = new MgEdge{i, node1, node2, w};
    mEdgeList[i] = edge;
    node1->edge_list.push_back(edge);
    node2->edge_list.push_back(edge);
  }

  // mEdgeList を重みの降順にソートする．
  sort(mEdgeList.begin(), mEdgeList.end(), EdgeLt());
}

// @brief デストラクタ
MatchGraph::~MatchGraph()
{
  for ( auto node: mNodeList ) {
    delete node;
  }
  for ( auto edge: mEdgeList ) {
    delete edge;
  }
}

// @brief 最大重みマッチングを求める．
vector<int>
MatchGraph::max_matching()
{
  // 増加路がなくなるまで繰り返す．
  for ( ; ; ) {
    // 選択されていない頂点から始まる
    // 交互路を見つける．
    auto alt_path = find_path();
    if ( alt_path.size() == 0 ) {
      // 増加路がなければ今の状態が最大
      break;
    }

    // alt_path に従って選択情報を更新する．
    for ( auto edge: alt_path ) {
      edge->selected = !edge->selected;
    }
  }

  // 答のリストを作る．
  vector<int> ans;
  ans.reserve(mEdgeList.size());
  for ( auto edge: mEdgeList ) {
    if ( edge->selected ) {
      ans.push_back(edge->id);
    }
  }
  return ans;
}

// @brief 重み最大の交互路を見つける．
vector<MgEdge*>
MatchGraph::find_path()
{
  // 両端が選択されていない枝の中で重みが最大のものを選ぶ．
  for ( auto edge: mEdgeList ) {
    if ( edge->node1->selected_edge() == nullptr &&
	 edge->node2->selected_edge() == nullptr ) {
      return vector<MgEdge*>{edge};
    }
  }

  // 以降は片方の端が選択されたノードのみとなる．

  for ( auto node: mNodeList ) {
    node->value_list.clear();
    node->alt_edge_list.clear();
  }

  int n = mNodeList.size();

  // 奇数番目の頂点を入れるキュー
  vector<MgNode*> queue1;
  queue1.reserve(n);

  for ( auto node: mNodeList ) {
    // 選択された枝を持たないノードをキューに積む．
    if ( node->selected_edge() == nullptr ) {
      node->value_list.push_back(0);
      node->alt_edge_list.push_back(nullptr);
      queue1.push_back(node);
    }
  }

  bool found = false;
  int max_value = 0;
  MgNode* max_node = nullptr;
  MgEdge* max_edge = nullptr;
  int phase = 0;
  while ( !found && queue1.size() > 0 ) {
    vector<MgNode*> queue2;
    queue2.reserve(n);
    vector<bool> in_queue(n, false);
    for ( auto node1: queue1 ) {
      int value1 = node1->value_list[phase];
      for ( auto edge1: node1->edge_list ) {
	if ( edge1->selected ) {
	  continue;
	}
	// 選択されていない枝を選ぶ．
	int value2 = value1 + edge1->weight;
	auto node2 = edge1->alt_node(node1);
	auto edge2 = node2->selected_edge();
	if ( edge2 == nullptr ) {
	  // node2 は open node だった．
	  if ( max_value < value2 ) {
	    max_value = value2;
	    max_node = node2;
	    max_edge = edge1;
	    found = true;
	  }
	}
	else {
	  auto node3 = edge2->alt_node(node2);
	  int value3 = value2 - edge2->weight;
	  while ( node3->value_list.size() <= (phase + 1) ) {
	    node3->value_list.push_back(-numeric_limits<int>::max());
	    node3->alt_edge_list.push_back(nullptr);
	  }
	  if ( node3->value_list[phase + 1] < value3 ) {
	    node3->value_list[phase + 1] = value3;
	    node3->alt_edge_list[phase] = edge1;
	    if ( !in_queue[node3->id] ) {
	      queue2.push_back(node3);
	      in_queue[node3->id] = true;
	    }
	  }
	}
      }
    }
    ++ phase;
    queue1.swap(queue2);
  }

  if ( max_node == nullptr ) {
    return vector<MgEdge*>();
  }
  else {
    auto max_path = make_path(max_node, max_edge, phase - 1);
    return max_path;
  }
}

// @brief パスを復元する．
vector<MgEdge*>
MatchGraph::make_path(MgNode* node1,
		      MgEdge* edge1,
		      int phase)
{
  auto node2 = edge1->alt_node(node1);
  auto edge2 = node2->selected_edge();
  if ( edge2 == nullptr ) {
    return vector<MgEdge*>{edge1};
  }
  else {
    auto node3 = edge2->alt_node(node2);
    auto edge3 = node2->alt_edge_list[phase - 1];
    auto path = make_path(node3, edge3, phase - 1);
    path.push_back(edge2);
    path.push_back(edge1);
    return path;
  }
}


//////////////////////////////////////////////////////////////////////
// MgNode の関数
//////////////////////////////////////////////////////////////////////

// 選択されている枝を返す．
MgEdge*
MgNode::selected_edge() const
{
  for ( auto edge: edge_list ) {
    if ( edge->selected ) {
      return edge;
    }
  }
  return nullptr;
}

END_NAMESPACE_YM_UDGRAPH
