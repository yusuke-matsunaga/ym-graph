
/// @file bigraph_test.cc
/// @brief bigraph_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "gtest/gtest.h"
#include "ym/BiGraph.h"


BEGIN_NAMESPACE_YM

TEST(BiGraphTest, constructor1)
{
  // 空のコンストラクタのテスト
  BiGraph graph;

  ASSERT_EQ( 0, graph.node1_num() );
  ASSERT_EQ( 0, graph.node2_num() );
  ASSERT_EQ( 0, graph.edge_num() );
}

TEST(BiGraphTest, constructor2)
{
  // ノード数を指定したコンストラクタのテスト
  int node1_num = 10;
  int node2_num = 10;
  BiGraph graph(node1_num, node2_num);

  graph.add_edge(0, 1);
  graph.add_edge(3, 2);

  ASSERT_EQ( node1_num, graph.node1_num() );
  ASSERT_EQ( node2_num, graph.node2_num() );
  ASSERT_EQ( 2, graph.edge_num() );

  auto& edge0 = graph.edge(0);
  ASSERT_EQ( 0, edge0.id1 );
  ASSERT_EQ( 1, edge0.id2 );

  auto& edge1 = graph.edge(1);
  ASSERT_EQ( 3, edge1.id1 );
  ASSERT_EQ( 2, edge1.id2 );
}

TEST(BiGraphTest, max_match1)
{
  int n1 = 4;
  int n2 = 4;
  vector<BiGraph::Edge> edge_list{{0, 0}, {0, 1},
				  {1, 0}, {1, 1}, {1, 2},
				  {2, 1}, {2, 2},
				  {3, 1}, {3, 2}, {3, 3}};
  BiGraph graph{n1, n2, edge_list};

  auto match = graph.max_matching();

  ASSERT_EQ( 4, match.size() );
  ASSERT_EQ( 0, match[0] );
  ASSERT_EQ( 3, match[1] );
  ASSERT_EQ( 6, match[2] );
  ASSERT_EQ( 9, match[3] );
}

TEST(BiGraphTest, max_match2)
{
  int n1 = 2;
  int n2 = 2;
  vector<BiGraph::Edge> edge_list{{0, 0, 1},
				  {1, 0, 3},
				  {1, 1, 1}};
  BiGraph graph{n1, n2, edge_list};

  auto match = graph.max_matching();

  ASSERT_EQ( 1, match.size() );
  ASSERT_EQ( 1, match[0] );
}

END_NAMESPACE_YM
