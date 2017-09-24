#pragma once

/********************************************************************
	created:	19/12/2011   14:39
	filename: 	XTreeCtrl.h
	author:		jeffery

	purpose:	����֧�ֹ��������϶�����չ������ؼ�,
				�������ϵ��ļ���<<VC���������Ϳؼ��ڵ��϶�������ʵ��>>
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
	UINT          m_TimerTicks;      //��������Ķ�ʱ����������ʱ��
	UINT          m_nScrollTimerID;  //��������Ķ�ʱ��
	CPoint        m_HoverPoint;      //���λ��
	UINT          m_nHoverTimerID;   //������ж�ʱ��
	DWORD         m_dwDragStart;     //������������һ�̵�ʱ��
	BOOL          m_bDragging;       //��ʶ�Ƿ������϶�������
	CImageList*   m_pDragImage;      //�϶�ʱ��ʾ��ͼ���б�
	HTREEITEM     m_hItemDragS;      //���϶��ı�ǩ
	HTREEITEM     m_hItemDragD;      //�����϶��ı�ǩ

public:
	afx_msg void OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

public:
	//������Ŀ
	HTREEITEM CopyItem(HTREEITEM hItem, HTREEITEM htiNewParent, HTREEITEM htiAfter);
	//������֧
	HTREEITEM CopyBranch(HTREEITEM htiBranch, HTREEITEM htiNewParent, HTREEITEM htiAfter);

};


