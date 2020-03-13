
/// @file MatchGraph.cc
/// @brief MatchGraph の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2020 Yusuke Matsunaga
/// All rights reserved.


#include "ym/BiGraph.h"
#include "MatchGraph.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_YM_BIGRAPH

// @brief 最大重みマッチングを求める．
// @return マッチング結果の枝番号のリストを返す．
vector<int>
BiGraph::max_matching() const
{
  MatchGraph m{*this};

  return m.max_matching();
}

// @brief コンストラクタ
// @param[in] src 元のグラフ
MatchGraph::MatchGraph(const BiGraph& src) :
  mNode1List(src.node1_num()),
  mNode2List(src.node2_num()),
  mEdgeList(src.edge_num())
{
  for ( int i: Range(src.node1_num()) ) {
    auto node = new Node{i};
    mNode1List[i] = node;
  }
  for ( int i: Range(src.node2_num()) ) {
    auto node = new Node{i};
    mNode2List[i] = node;
  }
  for ( int i: Range(src.edge_num()) ) {
    int id1 = src.edge_id1(i);
    int id2 = src.edge_id2(i);
    auto node1 = mNode1List[id1];
    auto node2 = mNode2List[id2];
    int w = src.edge_weight(i);
    auto edge = new Edge{i, node1, node2, w};
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
  for ( auto node: mNode1List ) {
    delete node;
  }
  for ( auto node: mNode2List ) {
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
vector<MatchGraph::Edge*>
MatchGraph::find_path()
{
  // 両端がオープンな枝が有ったらそれが増加路
  for ( auto edge: mEdgeList ) {
    if ( edge->node1->selected_edge() == nullptr &&
	 edge->node2->selected_edge() == nullptr ) {
      return vector<MatchGraph::Edge*>{edge};
    }
  }

  // 以降は必ず選択枝を含む奇数長の経路となる．

  for ( auto node: mNode1List ) {
    node->value = - numeric_limits<int>::max();
    node->alt_edge = nullptr;
  }

  vector<MatchGraph::Node*> queue1;
  queue1.reserve(mNode1List.size());
  for ( auto node1: mNode1List ) {
    // 選択された枝を持たないノードをキューに積む．
    if ( node1->selected_edge() == nullptr ) {
      node1->value = 0;
      node1->alt_edge = nullptr;
      queue1.push_back(node1);
    }
  }

  int max_value = 0;
  MatchGraph::Edge* max_edge = nullptr;
  MatchGraph::Node* max_node = nullptr;
  bool found = false;
  while ( !found && queue1.size() > 0 ) {
    vector<MatchGraph::Node*> queue2;
    vector<bool> in_queue(mNode1List.size(), false);
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
    return vector<MatchGraph::Edge*>{};
  }
  else {
    return make_path(max_edge, max_node);
  }
}

// @brief パスを復元する．
vector<MatchGraph::Edge*>
MatchGraph::make_path(MatchGraph::Edge* edge1,
		      MatchGraph::Node* node1)
{
  auto edge2 = node1->selected_edge();
  if ( edge2 == nullptr ) {
    return vector<Edge*>{edge1};
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

END_NAMESPACE_YM_BIGRAPH
