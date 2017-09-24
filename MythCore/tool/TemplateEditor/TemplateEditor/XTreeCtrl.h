#pragma once

/********************************************************************
	created:	19/12/2011   14:39
	filename: 	XTreeCtrl.h
	author:		jeffery

	purpose:	用于支持过滤器的拖动，扩展后的树控件,
				来自网上的文件：<<VC程序中树型控件节点拖动的完美实现>>
*********************************************************************/
// CXTreeCtrl

class CXTreeCtrl : public CTreeCtrl
{
	DECLARE_DYNAMIC(CXTreeCtrl)

public:
	CXTreeCtrl();
	virtual ~CXTreeCtrl();

protected:
	DECLARE_MESSAGE_MAP()

protected:
	UINT          m_TimerTicks;      //处理滚动的定时器所经过的时间
	UINT          m_nScrollTimerID;  //处理滚动的定时器
	CPoint        m_HoverPoint;      //鼠标位置
	UINT          m_nHoverTimerID;   //鼠标敏感定时器
	DWORD         m_dwDragStart;     //按下鼠标左键那一刻的时间
	BOOL          m_bDragging;       //标识是否正在拖动过程中
	CImageList*   m_pDragImage;      //拖动时显示的图象列表
	HTREEITEM     m_hItemDragS;      //被拖动的标签
	HTREEITEM     m_hItemDragD;      //接受拖动的标签

public:
	afx_msg void OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

public:
	//拷贝条目
	HTREEITEM CopyItem(HTREEITEM hItem, HTREEITEM htiNewParent, HTREEITEM htiAfter);
	//拷贝分支
	HTREEITEM CopyBranch(HTREEITEM htiBranch, HTREEITEM htiNewParent, HTREEITEM htiAfter);

};


