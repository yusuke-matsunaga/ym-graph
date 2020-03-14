#! /usr/bin/env python3

### @file max_matching_test.py
### @brief UdGraph.max_matching() のテスト
### @author Yusuke Matsunaga (松永 裕介)
###
### Copyright (C) 2020 Yusuke Matsunaga
### All rights reserved.

import unittest
from pym_graph import UdGraph

# UdGraph.max_matching() のテスト用クラス
class MaxMatchingTest(unittest.TestCase) :

    def runTest(self) :
        self.test_max_matching()

    def test_max_matching(self) :

        edge_list = [(0, 2, 1),
                     (1, 2, 3),
                     (1, 3, 1)]

        graph = UdGraph(2, 2, edge_list)

        match = graph.max_matching()

        assert len(match) == 1
        assert match[0] == (0, 1)


    def test_max_matching2(self) :

        edge_list = [(0, 1, 2),
                     (0, 4, 2),
                     (0, 5, 1),
                     (1, 2, 1),
                     (1, 6, 2),
                     (1, 7, 1),
                     (2, 3, 2),
                     (2, 5, 1),
                     (3, 4, 1),
                     (3, 5, 1),
                     (3, 6, 2),
                     (3, 7, 2),
                     (5, 6, 2),
                     (6, 7, 1)]

        graph = UdGraph(7, edge_list)

        match = graph.max_matching()

        assert len(match) == 4
        w = 0
        for pos in match :
            w += graph.edge_weight(pos)
        assert w == 7
