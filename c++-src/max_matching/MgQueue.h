#ifndef MGQUEUE_H
#define MGQUEUE_H

/// @file MgQueue.h
/// @brief MgQueue のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2020 Yusuke Matsunaga
/// All rights reserved.

#include "ym/UdGraph.h"


BEGIN_NAMESPACE_YM_UDGRAPH

class MgNode;
class MgEdge;

//////////////////////////////////////////////////////////////////////
/// @class MgQueue MgQueue.h "MgQueue.h"
/// @brief MgNode のヒープ木を表すクラス
//////////////////////////////////////////////////////////////////////
class MgQueue
{
public:

  /// @brief コンストラクタ
  /// @param[in] max_size 最大サイズ
  MgQueue(int max_size);

  /// @brief デストラクタ
  ~MgQueue() = default;


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
  put(MgNode* node,
      MgEdge* alt_edge,
      int value);

  /// @brief 先頭の要素を取り出す．
  MgNode*
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
  locate(MgNode* node,
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
  vector<MgNode*> mHeap;

};

END_NAMESPACE_YM_UDGRAPH

#endif // MGQUEUE_H
