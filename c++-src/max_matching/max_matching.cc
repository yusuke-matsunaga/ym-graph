
/// @file MatchGraph.cc
/// @brief MatchGraph の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2020 Yusuke Matsunaga
/// All rights reserved.


#include "ym/UdGraph.h"
#include "MgNode.h"
#include "MgEdge.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_YM_UDGRAPH


//////////////////////////////////////////////////////////////////////
// MgNode の関数
//////////////////////////////////////////////////////////////////////

// 選択されている枝を返す．
inline
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

BEGIN_NONAMESPACE

// @brief パスを復元する．
vector<MgEdge*>
make_path(MgNode* node1,
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

// @brief 重み最大の交互路を見つける．
vector<MgEdge*>
find_path(const vector<MgNode*>& node_list,
	  const vector<MgEdge*>& edge_list)
{
  // 両端が選択されていない枝の中で重みが最大のものを選ぶ．
  for ( auto edge: edge_list ) {
    if ( edge->node1->selected_edge() == nullptr &&
	 edge->node2->selected_edge() == nullptr ) {
      return vector<MgEdge*>{edge};
    }
  }

  // 以降は片方の端が選択されたノードのみとなる．

  for ( auto node: node_list ) {
    node->value_list.clear();
    node->alt_edge_list.clear();
  }

  int n = node_list.size();

  // 奇数番目の頂点を入れるキュー
  vector<MgNode*> queue1;
  queue1.reserve(n);

  for ( auto node: node_list ) {
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

END_NONAMESPACE


// @brief 最大重みマッチングを求める．
// @return マッチングに選ばれた枝番号のリストを返す．
vector<int>
UdGraph::max_matching(const string& algorithm) const
{
  vector<MgNode*> node_list(node_num());
  for ( int i: Range(node_num()) ) {
    auto node = new MgNode{i};
    node_list[i] = node;
  }

  vector<MgEdge*> edge_list(edge_num());
  for ( int i: Range(edge_num()) ) {
    int id1 = edge_id1(i);
    int id2 = edge_id2(i);
    int w = edge_weight(i);
    auto node1 = node_list[id1];
    auto node2 = node_list[id2];
    auto edge = new MgEdge{i, node1, node2, w};
    edge_list[i] = edge;
    node1->edge_list.push_back(edge);
    node2->edge_list.push_back(edge);
  }

  // mEdgeList を重みの降順にソートする．
  sort(edge_list.begin(), edge_list.end(), EdgeLt());

  // 増加路がなくなるまで繰り返す．
  for ( ; ; ) {
    // 選択されていない頂点から始まる
    // 交互路を見つける．
    auto alt_path = find_path(node_list, edge_list);
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
  ans.reserve(edge_list.size());
  for ( auto edge: edge_list ) {
    if ( edge->selected ) {
      ans.push_back(edge->id);
    }
  }

  for ( auto node: node_list ) {
    delete node;
  }
  for ( auto edge: edge_list ) {
    delete edge;
  }

  return ans;
}

END_NAMESPACE_YM_UDGRAPH
