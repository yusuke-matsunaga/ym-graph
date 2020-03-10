
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

  return m.max_match();
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
MatchGraph::max_match()
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
vector<MatchGraph::Edge*>
MatchGraph::find_path()
{
  for ( auto node: mNode1List ) {
    node->alt_edge = nullptr;
    node->value = -1;
    node->index = -1;
  }

  Queue queue(mNode1List.size());
  for ( auto node1: mNode1List ) {
    // 選択された枝を持たないノードをキューに積む．
    if ( node1->selected_edge() == nullptr ) {
      queue.put(node1, nullptr, 0);
    }
  }

  int max_value = 0;
  vector<MatchGraph::Edge*> max_path;
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


//////////////////////////////////////////////////////////////////////
// クラス MatchGraph::Node
//////////////////////////////////////////////////////////////////////

BEGIN_NONAMESPACE

int debug_heap = false;

END_NONAMESPACE

// @brief コンストラクタ
// @param[in] max_size 最大サイズ
MatchGraph::Queue::Queue(int max_size) :
  mNum{0},
  mHeap(max_size)
{
}

// @brief ノードを積む．
// @param[in] node 対象のノード
inline
void
MatchGraph::Queue::put(MatchGraph::Node* node,
		       MatchGraph::Edge* edge,
		       int value)
{
  if ( node->value > value ) {
    // すでに設定された値のほうが大きかった．
    return;
  }

  if ( debug_heap ) {
    dump(cerr);
  }

  int pos = node->index;
  if ( pos == -1 ) {
    // 最後に追加する．
    pos = mNum;
    ++ mNum;
    locate(node, pos);
  }

  // それを適切な位置まで上げる．
  move_up(pos);

  if ( debug_heap ) {
    sanity_check();
  }
}

// @brief 先頭の要素を取り出す．
MatchGraph::Node*
MatchGraph::Queue::get_top()
{
  if ( debug_heap ) {
    dump(cerr);
  }

  // 先頭のノード
  auto node = mHeap[0];
  -- mNum;

  // 最後の要素を先頭に持ってくる．
  auto last = mHeap[mNum];
  locate(last, 0);

  // それを適当な位置まで下げる．
  move_down(0);

  if ( debug_heap ) {
    sanity_check();
  }

  return node;
}

// @brief 要素を適切な位置まで下げる．
// @param[in] pos 対象の位置
void
MatchGraph::Queue::move_down(int pos)
{
  while ( true ) {
    int l_pos = pos * 2 + 1;
    int r_pos = pos * 2 + 2;
    if ( l_pos >= mNum ) {
      // 子供を持たない場合
      break;
    }
    auto node = mHeap[pos];
    auto l_node = mHeap[l_pos];
    if ( r_pos >= mNum ) {
      // 左の子供のみを持つ場合
      if ( l_node->value > node->value ) {
	locate(node, l_pos);
	locate(l_node, pos);
      }
      break;
    }
    else {
      // 両方の子供を持つ場合
      auto r_node = mHeap[r_pos];
      if ( l_node->value > r_node->value ) {
	if ( l_node->value > node->value ) {
	  locate(node, l_pos);
	  locate(l_node, pos);
	  pos = l_pos;
	}
	else {
	  break;
	}
      }
      else {
	if ( r_node->value > node->value ) {
	  locate(node, r_pos);
	  locate(r_node, pos);
	  pos = r_pos;
	}
	else {
	  break;
	}
      }
    }
  }
}

// @brief 要素を適切な位置まで上げる．
// @param[in] pos 対象の位置
void
MatchGraph::Queue::move_up(int pos)
{
  while ( pos > 0 ) {
    int p_pos = (pos - 1) / 2;
    auto node = mHeap[pos];
    auto p_node = mHeap[p_pos];
    if ( node->value > p_node->value ) {
      locate(node, p_pos);
      locate(p_node, pos);
      pos = p_pos;
    }
    else {
      break;
    }
  }
}

void
MatchGraph::Queue::locate(MatchGraph::Node* node,
			  int index)
{
  mHeap[index] = node;
  node->index = index;
}

// @brief ヒープ木の条件を満たしているかチェックする．
void
MatchGraph::Queue::sanity_check()
{
  bool error = false;
  for ( int pos = 1; pos < mNum; ++ pos ) {
    int p_pos = (pos - 1) / 2;
    auto node = mHeap[pos];
    auto p_node = mHeap[p_pos];
    if ( node->value > p_node->value ) {
      cout << "ERROR in mMinHeap at " << pos << endl;
      cout << "node->value = " << node->value
	   << ", p_node->value = " << p_node->value << endl;
      error = true;
    }
  }
  if ( error ) {
    dump(cout);
    abort();
  }
}

// @brief 内容をダンプする．
// @param[in] s 出力先のストリーム
void
MatchGraph::Queue::dump(ostream& s) const
{
  for ( int i = 0; i < mNum; ++ i ) {
    auto node = mHeap[i];
    s << " " << node->id << ":" << node->value;
  }
  s << endl;
}

END_NAMESPACE_YM_BIGRAPH
