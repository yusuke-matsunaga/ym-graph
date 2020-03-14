
/// @file MatchGraph.cc
/// @brief MatchGraph の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2020 Yusuke Matsunaga
/// All rights reserved.


#include "ym/BiGraph.h"
#include "MgNode.h"
#include "MgEdge.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_YM_BIGRAPH


//////////////////////////////////////////////////////////////////////
// クラス MgNode
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
make_path(MgEdge* edge1,
	  MgNode* node1)
{
  auto edge2 = node1->selected_edge();
  if ( edge2 == nullptr ) {
    return vector<MgEdge*>{edge1};
  }
  else {
    auto node2 = edge2->node2;
    auto edge3 = node1->alt_edge;
    auto path = make_path(edge3, edge3->node1);
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
  // 両端がオープンな枝が有ったらそれが増加路
  for ( auto edge: edge_list ) {
    if ( edge->node1->selected_edge() == nullptr &&
	 edge->node2->selected_edge() == nullptr ) {
      return vector<MgEdge*>{edge};
    }
  }

  // 以降は必ず選択枝を含む奇数長の経路となる．

  for ( auto node: node_list ) {
    node->value = - numeric_limits<int>::max();
    node->alt_edge = nullptr;
  }

  vector<MgNode*> queue1;
  queue1.reserve(node_list.size());
  for ( auto node1: node_list ) {
    // 選択された枝を持たないノードをキューに積む．
    if ( node1->selected_edge() == nullptr ) {
      node1->value = 0;
      node1->alt_edge = nullptr;
      queue1.push_back(node1);
    }
  }

  int max_value = 0;
  MgEdge* max_edge = nullptr;
  MgNode* max_node = nullptr;
  bool found = false;
  while ( !found && queue1.size() > 0 ) {
    vector<MgNode*> queue2;
    vector<bool> in_queue(node_list.size(), false);
    for ( auto node1: queue1 ) {
      int value1 = node1->value;
      for ( auto edge1: node1->edge_list ) {
	if ( edge1->selected ) {
	  continue;
	}
	int value2 = value1 + edge1->weight;
	auto node2 = edge1->node2;
	auto edge2 = node2->selected_edge();
	if ( edge2 == nullptr ) {
	  if ( max_value < value2 ) {
	    max_value = value2;
	    max_edge = edge1;
	    max_node = node1;
	    found = true;
	  }
	}
	else {
	  auto node3 = edge2->node1;
	  int value3 = value2 - edge2->weight;
	  if ( in_queue[node3->id] ) {
	    if ( node3->value < value3 ) {
	      node3->value = value3;
	      node3->alt_edge = edge1;
	    }
	  }
	  else {
	    node3->value = value3;
	    node3->alt_edge = edge1;
	    queue2.push_back(node3);
	  }
	}
      }
    }
    queue1.swap(queue2);
  }
  if ( max_edge == nullptr ) {
    return vector<MgEdge*>{};
  }
  else {
    return make_path(max_edge, max_node);
  }
}

END_NONAMESPACE


// @brief 最大重みマッチングを求める．
// @return マッチング結果の枝番号のリストを返す．
vector<int>
BiGraph::max_matching() const
{
  vector<MgNode*> node1_list(node1_num());
  for ( int i: Range(node1_num()) ) {
    auto node = new MgNode{i};
    node1_list[i] = node;
  }

  vector<MgNode*> node2_list(node2_num());
  for ( int i: Range(node2_num()) ) {
    auto node = new MgNode{i};
    node2_list[i] = node;
  }

  vector<MgEdge*> edge_list(edge_num());
  for ( int i: Range(edge_num()) ) {
    int id1 = edge_id1(i);
    int id2 = edge_id2(i);
    auto node1 = node1_list[id1];
    auto node2 = node2_list[id2];
    int w = edge_weight(i);
    auto edge = new MgEdge{i, node1, node2, w};
    edge_list[i] = edge;
    node1->edge_list.push_back(edge);
    node2->edge_list.push_back(edge);
  }

  // mEdgeList を重みの降順にソートする．
  sort(edge_list.begin(), edge_list.end(), EdgeLt());

  //auto match = max_matching(node1_list, edge_list);
  // 増加路がなくなるまで繰り返す．
  for ( ; ; ) {
    // 選択されていない頂点から始まる
    // 交互路を見つける．
    auto alt_path = find_path(node1_list, edge_list);
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

  for ( auto node: node1_list ) {
    delete node;
  }
  for ( auto node: node2_list ) {
    delete node;
  }
  for ( auto edge: edge_list ) {
    delete edge;
  }

  return ans;
}

END_NAMESPACE_YM_BIGRAPH
