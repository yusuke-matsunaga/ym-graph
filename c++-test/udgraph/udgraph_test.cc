
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
  ASSERT_EQ( 0, edge0.first );
  ASSERT_EQ( 1, edge0.second );

  auto& edge1 = graph.edge(1);
  ASSERT_EQ( 2, edge1.first );
  ASSERT_EQ( 3, edge1.second );
}

TEST(UdGraphTest, read_dimacs)
{
  string filename = string(TESTDATA_DIR) + string("/anna.col");
  ifstream s(filename);
  ASSERT_TRUE( static_cast<bool>(s) );

  UdGraph graph = UdGraph::read_dimacs(s);

  ASSERT_EQ( 138, graph.node_num() );
  ASSERT_EQ( 986, graph.edge_num() );

  ostringstream obuf;
  graph.write_dimacs(obuf);

  istringstream s1(obuf.str());
  UdGraph graph2 = UdGraph::read_dimacs(s1);
  ASSERT_EQ( 138, graph2.node_num() );

  ostringstream obuf2;
  graph2.write_dimacs(obuf2);

  EXPECT_EQ( obuf2.str(), obuf.str() );
}

END_NAMESPACE_YM
