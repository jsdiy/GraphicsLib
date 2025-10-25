//	矩形領域を操作するクラス
//『昼夜逆転』工作室	http://jsdiy.starfree.jp
//	2024/04	@jsdiy

#include <Arduino.h>
#include "Rectangle.hpp"

/*
//同じ領域のインスタンスを作成する
Rectangle	Rectangle::Copy(void)
{
	return Rectangle(left, top, Width(), Height());
}
*/

//2つの領域（parentとchild）の論理積となる領域を求める
//使用方法:	innerRect = parent.Inner(x,y,child)
//引数:		x,y: childの左上頂点を乗せる、parent上の座標。負数も可。
//			child: parentに重ねようとするchild上の領域。child座標系。
//戻り値:	innerRect: parent座標系で示された、求める領域。
//			child: child座標系で示された、求める領域。
//注意:		重なる領域がある場合、引数childは当処理によって書き換えられる（戻り値となる）。
//			重なる領域がない場合、innerRect.IsAvailable()==falseである。
Rectangle	Rectangle::Inner(int16_t x, int16_t y, Rectangle& child)
{
	int16_t xChildR = (x + child.Width() - 1);	//xにchild.Leftを置いたときの、parent座標系でのchild.Right
	int16_t yChildB = (y + child.Height() - 1);	//yにchild.Top を置いたときの、parent座標系でのchild.Bottom

	//parentの領域外にchildを置こうとしている場合、何も処理しない（重なる部分がない）
	if ((right < x) || (bottom < y) || (xChildR < left) || (yChildB < top)) { return Rectangle(0, 0, 0, 0); }

	//Rectangle tempParent = this->Copy();
	//Rectangle tempChild = child.Copy();
	Rectangle tempParent = *this;
	Rectangle tempChild = child;

	if (x < left) { tempParent.left = left;	tempChild.left += (left - x); }	//[x_child [parent …
	else if (left < x) { tempParent.left = x;	tempChild.left = child.left; }	//[parent [x_child …
	else { tempParent.left = x;	tempChild.left = child.left; }	//(left == x)

	if (right < xChildR) { tempParent.right = right;	tempChild.right -= (xChildR - right); }	//… parent] x_child]
	else if (xChildR < right) { tempParent.right = xChildR;	tempChild.right = child.right; }	//… x_child] parent]
	else { tempParent.right = xChildR;	tempChild.right = child.right; }	//(xChildR == right)

	if (y < top) { tempParent.top = top;	tempChild.top += (top - y); }	//[y_child [parent …
	else if (top < y) { tempParent.top = y;	tempChild.top = child.top; }	//[parent [y_child …
	else { tempParent.top = y;	tempChild.top = child.top; }	//(top == y)

	if (bottom < yChildB) { tempParent.bottom = bottom;	tempChild.bottom -= (yChildB - bottom); }	//… parent] y_child]
	else if (yChildB < bottom) { tempParent.bottom = yChildB;	tempChild.bottom = child.bottom; }	//… y_child] parent]
	else { tempParent.bottom = yChildB;	tempChild.bottom = child.bottom; }	//(yChildB == bottom)

	Rectangle innerParent = Rectangle(tempParent.left, tempParent.top, tempParent.Width(), tempParent.Height());
	if (innerParent.IsAvailable()) { child.SetRect(tempChild.left, tempChild.top, tempChild.Width(), tempChild.Height()); }	//innerChildにあたる
	return innerParent;
}

//領域を移動する
void	Rectangle::Move(int16_t x, int16_t y)
{
	left += x;	right += x;
	top += y;	bottom += y;
}

/*	領域の論理積について
対象の矩形領域を画面領域で切り取る
↓(0,0)
+-------+ ←画面領域
|   +====--+
|   /////  | ←引数の領域(x,y,w,h)のうち、画面内にある部分の矩形領域が求まる。
|   +====--+	①画面領域の(0,0)を起点とした(x',y',w',h')が求まる（parent座標系）。
+-------+		②それと同時に、引数の領域についても①と同じ大きさの(x",y",w",h")が求まる（child座標系）。
*/
