#! /usr/bin/env python3

### @file bigraph_test.py
### @brief
### @author Yusuke Matsunaga (松永 裕介)
###
### Copyright (C) 2020 Yusuke Matsunaga
### All rights reserved.

import unittest
from pym_graph import BiGraph

# BiGraph のテスト用クラス
class BiGraphTest(unittest.TestCase) :

    def runTest(self) :
        self.test_bigraph()

    def test_bigraph(self) :

        edge_list = [(0, 0, 1),
                     (1, 1, 1),
                     (0, 1, 3)]
        bigraph = BiGraph(2, 2, edge_list)

        match = bigraph.max_matching()

        assert len(match) == 1
        assert match[0] == (0, 1)

if __name__ == '__main__' :

    unittest.main()
