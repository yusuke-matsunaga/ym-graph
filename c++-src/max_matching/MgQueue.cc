
/// @file MgQueue.cc
/// @brief MgQueue の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2020 Yusuke Matsunaga
/// All rights reserved.


#include "MgQueue.h"
#include "MgNode.h"


BEGIN_NAMESPACE_YM_UDGRAPH

BEGIN_NONAMESPACE

bool debug_heap = false;

END_NONAMESPACE

// @brief コンストラクタ
// @param[in] max_size 最大サイズ
MgQueue::MgQueue(int max_size) :
  mNum{0},
  mHeap(max_size, nullptr)
{
}

// @brief ノードを積む．
// @param[in] node 対象のノード
// @param[in] alt_edge 代わりの枝
// @param[in] value 値
void
MgQueue::put(MgNode* node,
	     MgEdge* alt_edge,
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
MgNode*
MgQueue::get_top()
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

// @brief 内容をダンプする．
// @param[in] s 出力先のストリーム
void
MgQueue::dump(ostream& s) const
{
  for ( int i = 0; i < mNum; ++ i ) {
    auto node = mHeap[i];
    s << " " << node->id << ":" << node->value;
  }
  s << endl;
}

// @brief 要素を適切な位置まで下げる．
// @param[in] pos 対象の位置
void
MgQueue::move_down(int pos)
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
MgQueue::move_up(int pos)
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

// @brief 要素を配置する．
void
MgQueue::locate(MgNode* node,
		int index)
{
  mHeap[index] = node;
  node->index = index;
}

// @brief ヒープ木の条件を満たしているかチェックする．
void
MgQueue::sanity_check()
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

END_NAMESPACE_YM_UDGRAPH
