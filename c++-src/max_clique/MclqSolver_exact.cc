﻿
/// @file MclqSolver_exact.cc
/// @brief MclqSolver の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014, 2015, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "MclqSolver.h"
#include "MclqNode.h"


BEGIN_NAMESPACE_YM_UDGRAPH

BEGIN_NONAMESPACE

struct NodeLt
{
  bool
  operator()(const pair<MclqNode*, int>& left,
	     const pair<MclqNode*, int>& right)
  {
    return left.second > right.second;
  }
};

int count;

int
mc_recur(const vector<MclqNode*>& selected_node_list,
	 const vector<MclqNode*>& rest_node_list,
	 int best_so_far,
	 vector<int>& node_set)
{
#if 0
  cout << "mc_recur(selected_node_list = " << selected_node_list.size()
       << ", rest_node_list = " << rest_node_list.size()
       << ", best_so_far = " << best_so_far << ")" << endl;
#endif

  if ( selected_node_list.size() + rest_node_list.size() <= best_so_far ) {
    return 0;
  }

  ++ count;
  if ( count >= 10000 ) {
    return 0;
  }

  if ( rest_node_list.empty() ) {
    node_set.clear();
    for ( auto node: selected_node_list ) {
      node_set.push_back(node->id());
    }
    return node_set.size();
  }

  unordered_set<int> mark;
  vector<pair<MclqNode*, int> > tmp_node_list;
  tmp_node_list.reserve(rest_node_list.size());
  for ( auto node1: rest_node_list ) {
    mark.insert(node1->id());
  }

  for ( auto node1: rest_node_list ) {
    int n = 0;
    for ( int j = 0; j < node1->adj_size(); ++ j ) {
      MclqNode* node2 = node1->adj_node(j);
      if ( mark.count(node2->id()) > 0 ) {
	++ n;
      }
    }
    tmp_node_list.push_back(make_pair(node1, n));
  }
  sort(tmp_node_list.begin(), tmp_node_list.end(), NodeLt());

  int max_val = best_so_far;
  for ( auto node_val_pair: tmp_node_list ) {
    auto node1 = node_val_pair.first;
    unordered_set<int> mark;
    for ( int i = 0; i < node1->adj_size(); ++ i ) {
      MclqNode* node2 = node1->adj_node(i);
      mark.insert(node2->id());
    }

    vector<MclqNode*> new_node_list;
    new_node_list.reserve(rest_node_list.size());

    for ( auto node2: rest_node_list ) {
      if ( mark.count(node2->id()) > 0 ) {
	new_node_list.push_back(node2);
      }
    }

    vector<MclqNode*> new_selected_node_list = selected_node_list;
    new_selected_node_list.push_back(node1);
    vector<int> tmp_node_set;
    mc_recur(new_selected_node_list, new_node_list, max_val, tmp_node_set);
    int val = tmp_node_set.size();
    if ( max_val < val ) {
      max_val = val;
      node_set = tmp_node_set;
    }
  }

#if 0
  cout << "mc_recur(selected_node_list = " << selected_node_list.size()
       << ", rest_node_list = " << rest_node_list.size()
       << ", best_so_far = " << best_so_far << ")" << endl
       << "  ==> " << max_val << endl;
#endif

  return max_val;
}

END_NONAMESPACE

// @brief 最大クリークを求める．
// @param[in] graph 対象のグラフ
// @param[out] node_set クリークの要素(ノード番号)を収める配列
// @return 要素数を返す．
int
MclqSolver::exact(vector<int>& node_set)
{
  node_set.clear();

  // 処理対象のノードを収めるリスト
  vector<MclqNode*> node_list;
  for ( int i = 0; i < mNodeNum; ++ i ) {
    auto node = &mNodeArray[i];
    node_list.push_back(node);
  }

  count = 0;
  mc_recur(vector<MclqNode*>(0), node_list, 0, node_set);

  return node_set.size();
}

END_NAMESPACE_YM_UDGRAPH
