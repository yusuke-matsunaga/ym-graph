
/// @file udgraph_test.cc
/// @brief udgraph_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "gtest/gtest.h"
#include "ym/UdGraph.h"


BEGIN_NAMESPACE_YM

TEST(UdGraphTest, constructor1)
{
  // 空のコンストラクタのテスト
  UdGraph graph;

  ASSERT_EQ( 0, graph.node_num() );
  ASSERT_EQ( 0, graph.edge_num() );
}

TEST(UdGraphTest, constructor2)
{
  // ノード数を指定したコンストラクタのテスト
  int node_num = 10;
  UdGraph graph(node_num);

  graph.add_edge(0, 1);
  graph.add_edge(3, 2);

  ASSERT_EQ( node_num, graph.node_num() );
  ASSERT_EQ( 2, graph.edge_num() );

  auto& edge0 = graph.edge(0);
  ASSERT_EQ( 0, edge0.id1 );
  ASSERT_EQ( 1, edge0.id2 );

  auto& edge1 = graph.edge(1);
  ASSERT_EQ( 2, edge1.id1 );
  ASSERT_EQ( 3, edge1.id2 );
}

TEST(UdGraphTest, read_dimacs)
{
  string filename = string(TESTDATA_DIR) + string("/anna.col");
  ifstream s(filename);
  ASSERT_TRUE( static_cast<bool>(s) );

  UdGraph graph = UdGraph::read_dimacs(s);

  EXPECT_EQ( 138, graph.node_num() );
  EXPECT_EQ( 986, graph.edge_num() );

  ostringstream obuf;
  graph.write_dimacs(obuf);

  istringstream s1(obuf.str());
  UdGraph graph2 = UdGraph::read_dimacs(s1);
  EXPECT_EQ( 138, graph2.node_num() );

  ostringstream obuf2;
  graph2.write_dimacs(obuf2);

  EXPECT_EQ( obuf2.str(), obuf.str() );
}

TEST(UdGraphTest, max_matching1)
{
  vector<UdGraph::Edge> edge_list{{0, 2, 1},
				  {1, 2, 3},
				  {1, 3, 1}};
  UdGraph graph(4, edge_list);

  vector<int> match = graph.max_matching();

  EXPECT_EQ( 1, match.size() );
  EXPECT_EQ( 1, match[0] );
}

TEST(UdGraphTest, max_matching2)
{
  vector<UdGraph::Edge> edge_list{{0, 1, 2},
				  {0, 4, 2},
				  {0, 5, 1},
				  {1, 2, 1},
				  {1, 6, 2},
				  {1, 7, 1},
				  {2, 3, 2},
				  {2, 5, 1},
				  {3, 4, 1},
				  {3, 5, 1},
				  {3, 6, 2},
				  {3, 7, 2},
				  {5, 6, 2},
				  {6, 7, 1}};
  UdGraph graph(8, edge_list);

  vector<int> match = graph.max_matching();

  EXPECT_EQ( 4, match.size() );

  int w = 0;
  for ( int pos: match ) {
    w += edge_list[pos].weight;
  }
  EXPECT_EQ( 7, w );
}

END_NAMESPACE_YM
