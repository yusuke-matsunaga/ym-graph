
/// @file MatchGraph.cc
/// @brief MatchGraph の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2020 Yusuke Matsunaga
/// All rights reserved.


#include "MatchGraph.h"
#include "MgNode.h"
#include "MgEdge.h"
#include "MgQueue.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_YM_UDGRAPH

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
    auto node1 = mNodeList[id1];
    auto node2 = mNodeList[id2];
    int w = src.edge_weight(i);
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
  // 初期マッチを求める．
  for ( auto edge: mEdgeList ) {
    if ( edge->node1->selected_edge() == nullptr &&
	 edge->node2->selected_edge() == nullptr ) {
      edge->selected = true;
    }
  }

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
  for ( auto node: mNodeList ) {
    node->alt_edge = nullptr;
    node->value = -1;
    node->index = -1;
  }

  MgQueue queue(mNodeList.size());
  for ( auto node: mNodeList ) {
    // 選択された枝を持たないノードをキューに積む．
    if ( node->selected_edge() == nullptr ) {
      queue.put(node, nullptr, 0);
    }
  }

  int max_value = 0;
  vector<MgEdge*> max_path;
  bool not_found = true;
  while ( not_found && queue.num() > 0 ) {
    // value が最大のノードを取り出す．
    auto node1 = queue.get_top();
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
	  max_path = make_path(edge1, node1);
	  not_found = false;
	}
      }
      else {
	auto node3 = edge2->node1;
	queue.put(node3, edge1, value2 - edge2->weight);
      }
    }
  }

  return max_path;
}

// @brief パスを復元する．
vector<MgEdge*>
MatchGraph::make_path(MgEdge* edge1,
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
