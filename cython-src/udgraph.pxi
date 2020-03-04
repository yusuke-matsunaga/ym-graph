
### @file udgraph.pxi
### @brief udgraph の cython インターフェイス
### @author Yusuke Matsunaga (松永 裕介)
###
### Copyright (C) 2018 Yusuke Matsunaga
### All rights reserved.

from libcpp.string cimport string
from libcpp.pair cimport pair
from libcpp.vector cimport vector
from CXX_UdGraph cimport UdGraph as CXX_UdGraph


### @brief UdGraph の Python バージョン
cdef class UdGraph :

    cdef CXX_UdGraph _this

    ### @brief 初期化
    def __init__(self, int node_num, edge_list = list()) :
        cdef int id1, id2
        self._this.resize(node_num)
        for id1, id2 in edge_list :
            self._this.add_edge(id1, id2)

    ### @brief ノード数を返す．
    @property
    def node_num(self) :
        return self._this.node_num()

    ### @brief 枝数を返す．
    @property
    def edge_num(self) :
        return self._this.edge_num()

    ### @brief 枝のリストを返す．
    def edge_list(self) :
        cdef int i
        cdef pair[int, int] c_edge
        for i in range(self.edge_num) :
            c_edge = self._this.edge(i)
            yield c_edge.first, c_edge.second

    ### @brief DIMACS 形式のファイルを読み込むクラスメソッド
    @staticmethod
    def read_dimacs(str filename) :
        cdef string c_str = filename.encode('UTF-8')
        cdef UdGraph graph = UdGraph()
        graph._this = CXX_UdGraph.read_dimacs(c_str)
        if graph._this.node_num() > 0 :
            return graph
        else :
            return None

    ### @brief DIMACS 形式でファイルに書き出す．
    def write_dimacs(self, str filename) :
        cdef string c_str = filename.encode('UTF-8')
        self._this.write_dimacs(c_str)

    ### @brief 彩色問題を解く
    def coloring(self, algorithm = None) :
        cdef string c_algorithm
        cdef pair[int, vector[int]] cmap_ans
        cdef vector[int] c_color_map
        cdef int nc
        if algorithm != None :
            c_algorithm = algorithm.encode('UTF-8')
        else :
            c_algorithm = string()
        cmap_ans = self._this.coloring(c_algorithm)
        nc = cmap_ans.first
        c_color_map = cmap_ans.second
        return nc, [ c_color_map[i] for i in range(self.node_num) ]
