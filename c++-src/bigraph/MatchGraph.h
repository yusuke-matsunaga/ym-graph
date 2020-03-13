#ifndef MATCHGRAPH_H
#define MATCHGRAPH_H

/// @file MatchGraph.h
/// @brief MatchGraph のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2020 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BiGraph.h"


BEGIN_NAMESPACE_YM_BIGRAPH

//////////////////////////////////////////////////////////////////////
/// @class MatchGraph MatchGraph.h "MatchGraph.h"
/// @brief 2部グラフの最大マッチングを求めるためのクラス
//////////////////////////////////////////////////////////////////////
class MatchGraph
{
private:

  struct Node;

  /// @brief 枝を表すクラス
  struct Edge
  {
    // 番号
    int id;

    // 頂点1
    Node* node1;

    // 頂点2
    Node* node2;

    // 重み
    int weight;

    // 選択されている時 true となるフラグ
    bool selected{false};
  };

  /// @brief 枝の比較を行うファンクタ
  struct EdgeLt
  {
    bool
    operator()(const Edge* left,
	       const Edge* right)
    {
      return left->weight > right->weight;
    }
  };

  /// @brief ノードを表すクラス
  struct Node
  {
    // 番号
    int id;

    // 枝のリスト
    vector<Edge*> edge_list;

    // 現在の値
    int value{0};

    // ヒープ木上のインデックス
    int index{-1};

    // 増加路上の枝
    Edge* alt_edge{nullptr};

    // 選択されている枝を返す．
    Edge*
    selected_edge() const
    {
      for ( auto edge: edge_list ) {
	if ( edge->selected ) {
	  return edge;
	}
      }
      return nullptr;
    }
  };

  // Node->value をキーとしてヒープ木
  class Queue
  {
  public:

    /// @brief コンストラクタ
    /// @param[in] max_size 最大サイズ
    Queue(int max_size);

    /// @brief デストラクタ
    ~Queue() = default;


  public:
    //////////////////////////////////////////////////////////////////////
    // 外部インターフェイス
    //////////////////////////////////////////////////////////////////////

    /// @brief 要素数を返す．
    int
    num() const
    {
      return mNum;
    }

    /// @brief ノードを積む．
    /// @param[in] node 対象のノード
    /// @param[in] alt_edge 代わりの枝
    /// @param[in] value 値
    void
    put(Node* node,
	Edge* alt_edge,
	int value);

    /// @brief 先頭の要素を取り出す．
    Node*
    get_top();

    /// @brief 内容をダンプする．
    /// @param[in] s 出力先のストリーム
    void
    dump(ostream& s) const;


  private:
    //////////////////////////////////////////////////////////////////////
    // 内部で用いられる関数
    //////////////////////////////////////////////////////////////////////

    /// @brief 要素を適切な位置まで下げる．
    /// @param[in] pos 対象の位置
    void
    move_down(int pos);

    /// @brief 要素を適切な位置まで上げる．
    /// @param[in] pos 対象の位置
    void
    move_up(int pos);

    /// @brief 要素を配置する．
    void
    locate(Node* node,
	   int index);

    /// @brief ヒープ木の条件を満たしているかチェックする．
    void
    sanity_check();


  private:
    //////////////////////////////////////////////////////////////////////
    // データメンバ
    //////////////////////////////////////////////////////////////////////

    // 要素数
    int mNum;

    // ヒープ木
    vector<Node*> mHeap;

  };


public:

  /// @brief コンストラクタ
  /// @param[in] src 元のグラフ
  MatchGraph(const BiGraph& src);

  /// @brief デストラクタ
  ~MatchGraph();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 最大重みマッチングを求める．
  vector<int>
  max_matching();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 重み最大の交互路を見つける．
  vector<Edge*>
  find_path();

  /// @brief パスを復元する．
  vector<Edge*>
  make_path(Edge* edge,
	    Node* node);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 頂点1のリスト
  vector<Node*> mNode1List;

  // 頂点2のリスト
  vector<Node*> mNode2List;

  // 枝のリスト
  vector<Edge*> mEdgeList;

};

END_NAMESPACE_YM_BIGRAPH

#endif // MATCHGRAPH_H
