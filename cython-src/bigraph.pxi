
### @file bigraph.pxi
### @brief bigraph の cython インターフェイス
### @author Yusuke Matsunaga (松永 裕介)
###
### Copyright (C) 2020 Yusuke Matsunaga
### All rights reserved.

from libcpp.string cimport string
from CXX_BiGraph cimport BiGraph as CXX_BiGraph


### @brief BiGraph の Python バージョン
cdef class BiGraph :

    cdef CXX_BiGraph _this

    ### @brief 初期化
    def __init__(self, int node1_num, int node2_num, edge_list = list()) :
        cdef int id1, id2, w
        self._this.resize(node1_num, node2_num)
        for edge in edge_list :
            if len(edge) == 2 :
                id1, id2 = edge
                w = 1
            elif len(edge) == 3 :
                id1, id2, w = edge
            else :
                assert False
            self._this.add_edge(id1, id2, w)

    ### @brief 頂点1の数を返す．
    @property
    def node1_num(self) :
        return self._this.node1_num()

    ### @brief 頂点2の数を返す．
    @property
    def node2_num(self) :
        return self._this.node2_num()

    ### @brief 枝の数を返す．
    @property
    def edge_num(self) :
        return self._this.edge_num()

    ### @brief 枝のリストを返す．
    def edge_list(self) :
        cdef int id1, id2, w
        for i in range(self.edge_num) :
            id1 = self._this.edge_id1(i)
            id2 = self._this.edge_id2(i)
            w = self._this.edge_weight(i)
            yield id1, id2, w

    ### @brief ファイルから読み込む．
    @staticmethod
    def read(str filename) :
        cdef string c_str = filename.encode('UTF-8')
        cdef BiGraph graph = BiGraph()
        graph._this = CXX_BiGraph.read(c_str)
        if graph._this.node1_num() > 0 :
            return graph
        else :
            return None

    ### @brief ファイルに書き出す．
    def write(self, str filename) :
        cdef string c_str = filename.encode('UTF-8')
        self._this.write(c_str)

    ### @brief 最大マッチングを求める．
    def max_matching(self) :
        cdef vector[int] c_pos_list
        cdef int n
        cdef int pos
        c_pos_list = self._this.max_matching()
        n = c_pos_list.size()
        ans = list()
        for i in range(n) :
            pos = c_pos_list[i]
            id1 = self._this.edge_id1(pos)
            id2 = self._this.edge_id2(pos)
            ans.append( (id1, id2) )
        return ans
