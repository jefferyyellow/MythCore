// XTreeCtrl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TemplateEditor.h"
#include "XTreeCtrl.h"
#include "CatalogTree.h"

#define   DRAG_DELAY   60

// CXTreeCtrl

IMPLEMENT_DYNAMIC(CXTreeCtrl, CTreeCtrl)

CXTreeCtrl::CXTreeCtrl()
{
	m_bDragging = false;
}

CXTreeCtrl::~CXTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CXTreeCtrl, CTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, &CXTreeCtrl::OnTvnBegindrag)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
END_MESSAGE_MAP()



// CXTreeCtrl ��Ϣ�������



void CXTreeCtrl::OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//�����������ҷ�����������
	if( (GetTickCount() - m_dwDragStart) < DRAG_DELAY )
		return;

	// �϶���Դ�ڵ�
	m_hItemDragS = pNMTreeView->itemNew.hItem;

	// �õ�Դ�ڵ�����
	CTreeItemData* pData = (CTreeItemData*)GetItemData(m_hItemDragS);
	if (NULL == pData)
	{
		return;
	}
	// ����ڵ㲻��ģ�����ݻ��߹������������϶�
	if (pData->GetItemType() != TEMP_ITEM_DATA && pData->GetItemType() != TEMP_ITEM_FILTER)
	{
		return;
	}


	m_hItemDragD = NULL;

	//�õ������϶�ʱ��ʾ��ͼ���б�
	m_pDragImage = CreateDragImage( m_hItemDragS );

	if( !m_pDragImage )

		return;

	m_bDragging = true;

	m_pDragImage->BeginDrag ( 0,CPoint(8,8) );

	CPoint pt = pNMTreeView->ptDrag;

	ClientToScreen( &pt );

	m_pDragImage->DragEnter ( this,pt ); //"this"����ҷ���������ڸô���

	SetCapture();

	m_nScrollTimerID = (UINT)SetTimer( 2,40,NULL );

	//GetParent()->s(TVN_BEGINDRAG, WPARAM(pNMHDR), LPARAM(pResult));
	*pResult = 0;
}

void CXTreeCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	HTREEITEM  hItem;
	UINT       flags;

	//���������ж�ʱ���Ƿ����,���������ɾ��,ɾ�����ٶ�ʱ
	if( m_nHoverTimerID )
	{
		KillTimer( m_nHoverTimerID );
		m_nHoverTimerID = 0;
	}
	m_nHoverTimerID = (UINT)SetTimer( 1,800,NULL );  //��ʱΪ 0.8 �����Զ�չ��
	m_HoverPoint = point;

	if( m_bDragging )
	{
		CPoint  pt = point;
		CImageList::DragMove( pt );

		//��꾭��ʱ������ʾ
		CImageList::DragShowNolock( false );  //������꾭��ʱ�����ѿ��ĺۼ�
		if( (hItem = HitTest(point,&flags)) != NULL )
		{
			SelectDropTarget( hItem );
			m_hItemDragD = hItem;
		}
		CImageList::DragShowNolock( true );

		//����Ŀ����ҷ�����Եʱ������Ŀ���ڸ���
		CRect  rect;
		GetClientRect( &rect );
		if( point.x < rect.left + 20 )
			m_hItemDragD = NULL;
	}

	CTreeCtrl::OnMouseMove(nFlags, point);}

void CXTreeCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CTreeCtrl::OnLButtonUp(nFlags, point);
	
	// ������϶�״̬
	if( m_bDragging )
	{
		m_bDragging = FALSE;
		CImageList::DragLeave( this );
		CImageList::EndDrag();
		ReleaseCapture();
		delete m_pDragImage;

		SelectDropTarget( NULL );

		if( m_hItemDragS == m_hItemDragD )
		{
			KillTimer( m_nScrollTimerID );
			return;
		}

		Expand( m_hItemDragD,TVE_EXPAND );
		HTREEITEM  htiParent = m_hItemDragD;

		CTreeItemData* pData = (CTreeItemData*)GetItemData(m_hItemDragS);
		if (NULL == pData)
		{
			return;
		}
		CTreeItemData* pDestData = (CTreeItemData*)GetItemData(m_hItemDragD);
		if (NULL == pDestData)
		{
			return;
		}
		// Ŀ��ڵ㲻�������ݣ��ļ��л����ļ�ģ��
		if (pDestData->GetItemType() == TEMP_ITEM_DATA 
			|| pDestData->GetItemType() == TEMP_ITEM_DIR
			|| pDestData->GetItemType() == TEMP_ITEM_ORIGIN)
		{
			return;
		}

		// Դ��Ŀ��ڵ��Ƿ���ͬһ��ģ�����
		if (!CCatalogTree::GetSingletonPtr()->IsSameDataTable(m_hItemDragS, m_hItemDragD))
		{
			return;
		}
		
		

	/*	if (pData->GetItemType() != TEMP_ITEM_DATA && pData->GetItemType() != TEMP_ITEM_FILTER)
		{
			return;
		}*/

		//������ɸ��ڵ������ӽڵ�
		while( (htiParent = GetParentItem(htiParent)) != NULL )
		{
			if( htiParent == m_hItemDragS )
			{
				//����һ����ʱ�ڵ�����ɲ���
				HTREEITEM  htiNewTemp = CopyBranch( m_hItemDragS,NULL,TVI_LAST );
				HTREEITEM  htiNew = CopyBranch( htiNewTemp,m_hItemDragD,TVI_LAST );
				DeleteItem( htiNewTemp );
				SelectItem( htiNew );
				KillTimer( m_nScrollTimerID );
				CCatalogTree::GetSingletonPtr()->SortTreeItem(m_hItemDragD);
				return;
			}
		}

		HTREEITEM  htiNew = CopyBranch( m_hItemDragS,m_hItemDragD,TVI_LAST );
		DeleteItem( m_hItemDragS );
		SelectItem( htiNew );
		CCatalogTree::GetSingletonPtr()->SortTreeItem(m_hItemDragD);
		CCatalogTree::GetSingletonPtr()->SaveFilterFile(m_hItemDragD);

		KillTimer( m_nScrollTimerID );
	}

}

void CXTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//����������ҷ
	m_dwDragStart = GetTickCount();
	CTreeCtrl::OnLButtonDown(nFlags, point);
}

void CXTreeCtrl::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//������нڵ�
	if( nIDEvent == m_nHoverTimerID )
	{
		KillTimer( m_nHoverTimerID );
		m_nHoverTimerID = 0;
		HTREEITEM  trItem = 0;
		UINT  uFlag = 0;
		trItem = HitTest( m_HoverPoint,&uFlag );
		if( trItem && m_bDragging )
		{
			SelectItem( trItem );
			Expand( trItem,TVE_EXPAND );
		}
	}
	//������ҷ�����еĹ�������
	else if( nIDEvent == m_nScrollTimerID )
	{
		m_TimerTicks++;
		CPoint  pt;
		GetCursorPos( &pt );
		CRect  rect;
		GetClientRect( &rect );
		ClientToScreen( &rect );

		HTREEITEM  hItem = GetFirstVisibleItem();

		if( pt.y < rect.top +10 )
		{
			//���Ϲ���
			int  slowscroll = 6 - (rect.top + 10 - pt.y )/20;
			if( 0 == (m_TimerTicks % ((slowscroll > 0) ? slowscroll : 1)) )
			{
				CImageList::DragShowNolock ( false );
				SendMessage( WM_VSCROLL,SB_LINEUP );
				SelectDropTarget( hItem );
				m_hItemDragD = hItem;
				CImageList::DragShowNolock ( true );
			}
		}
		else if( pt.y > rect.bottom - 10 )
		{
			//���¹���
			int  slowscroll = 6 - (pt.y - rect.bottom + 10)/20;
			if( 0 == (m_TimerTicks % ((slowscroll > 0) ? slowscroll : 1)) )
			{
				CImageList::DragShowNolock ( false );
				SendMessage( WM_VSCROLL,SB_LINEDOWN );
				int  nCount = GetVisibleCount();
				for( int i=0 ; i<nCount-1 ; i++ )
					hItem = GetNextVisibleItem( hItem );
				if( hItem )
					SelectDropTarget( hItem );
				m_hItemDragD = hItem;
				CImageList::DragShowNolock ( true );
			}
		}
	}
	else
		CTreeCtrl::OnTimer(nIDEvent);
}


//������Ŀ
HTREEITEM CXTreeCtrl::CopyItem(HTREEITEM hItem, HTREEITEM htiNewParent, HTREEITEM htiAfter)
{
	TV_INSERTSTRUCT  tvstruct;
	HTREEITEM        hNewItem;
	CString          sText;

	//�õ�Դ��Ŀ����Ϣ
	tvstruct.item.hItem = hItem;
	tvstruct.item.mask  = TVIF_CHILDREN|TVIF_HANDLE|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	GetItem( &tvstruct.item );
	sText = GetItemText( hItem );
	tvstruct.item.cchTextMax = sText.GetLength ();
	tvstruct.item.pszText    = sText.LockBuffer ();

	//����Ŀ���뵽���ʵ�λ��
	tvstruct.hParent         = htiNewParent;
	tvstruct.hInsertAfter    = htiAfter;
	tvstruct.item.mask       = TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT;
	hNewItem = InsertItem( &tvstruct );
	sText.ReleaseBuffer ();

	//���ƿ�����Ŀ���ݺ���Ŀ״̬
	SetItemData( hNewItem,GetItemData(hItem) );
	SetItemState( hNewItem,GetItemState(hItem,TVIS_STATEIMAGEMASK),TVIS_STATEIMAGEMASK);

	return hNewItem;
}

//������֧
HTREEITEM CXTreeCtrl::CopyBranch(HTREEITEM htiBranch, HTREEITEM htiNewParent, HTREEITEM htiAfter)
{
	HTREEITEM  hChild;
	HTREEITEM  hNewItem = CopyItem( htiBranch,htiNewParent,htiAfter );
	hChild = GetChildItem( htiBranch );

	while( hChild != NULL )
	{
		CopyBranch( hChild,hNewItem,htiAfter );
		hChild = GetNextSiblingItem( hChild );
	}

	return  hNewItem;
}
