
/// @file dimacs_test.cc
/// @brief dimacs_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "gtest/gtest.h"
#include "ym/UdGraph.h"


BEGIN_NAMESPACE_YM

TEST(UdGraphDimacsTest, read_dimacs)
{
  string filename = string(TESTDATA_DIR) + string("/anna.col");
  ifstream s(filename);
  ASSERT_COND( static_cast<bool>(s) );

  UdGraph graph = UdGraph::read_dimacs(s);

  ASSERT_EQ( 138, graph.node_num() );
  ASSERT_EQ( 986, graph.edge_num() );

  ostringstream obuf;
  graph.write_dimacs(obuf);

  istringstream s1(obuf.str());
  UdGraph graph2 = UdGraph::read_dimacs(s1);
  ASSERT_EQ( 130, graph2.node_num() );

  ostringstream obuf2;
  graph2.write_dimacs(obuf2);

  EXPECT_EQ( obuf2.str(), obuf.str() );
}

END_NAMESPACE_YM
