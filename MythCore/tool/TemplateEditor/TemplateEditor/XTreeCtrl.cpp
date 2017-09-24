// XTreeCtrl.cpp : 实现文件
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



// CXTreeCtrl 消息处理程序



void CXTreeCtrl::OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	//如果是无意拖曳，则放弃操作
	if( (GetTickCount() - m_dwDragStart) < DRAG_DELAY )
		return;

	// 拖动的源节点
	m_hItemDragS = pNMTreeView->itemNew.hItem;

	// 得到源节点数据
	CTreeItemData* pData = (CTreeItemData*)GetItemData(m_hItemDragS);
	if (NULL == pData)
	{
		return;
	}
	// 如果节点不是模板数据或者过滤器，不能拖动
	if (pData->GetItemType() != TEMP_ITEM_DATA && pData->GetItemType() != TEMP_ITEM_FILTER)
	{
		return;
	}


	m_hItemDragD = NULL;

	//得到用于拖动时显示的图象列表
	m_pDragImage = CreateDragImage( m_hItemDragS );

	if( !m_pDragImage )

		return;

	m_bDragging = true;

	m_pDragImage->BeginDrag ( 0,CPoint(8,8) );

	CPoint pt = pNMTreeView->ptDrag;

	ClientToScreen( &pt );

	m_pDragImage->DragEnter ( this,pt ); //"this"将拖曳动作限制在该窗口

	SetCapture();

	m_nScrollTimerID = (UINT)SetTimer( 2,40,NULL );

	//GetParent()->s(TVN_BEGINDRAG, WPARAM(pNMHDR), LPARAM(pResult));
	*pResult = 0;
}

void CXTreeCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	HTREEITEM  hItem;
	UINT       flags;

	//检测鼠标敏感定时器是否存在,如果存在则删除,删除后再定时
	if( m_nHoverTimerID )
	{
		KillTimer( m_nHoverTimerID );
		m_nHoverTimerID = 0;
	}
	m_nHoverTimerID = (UINT)SetTimer( 1,800,NULL );  //定时为 0.8 秒则自动展开
	m_HoverPoint = point;

	if( m_bDragging )
	{
		CPoint  pt = point;
		CImageList::DragMove( pt );

		//鼠标经过时高亮显示
		CImageList::DragShowNolock( false );  //避免鼠标经过时留下难看的痕迹
		if( (hItem = HitTest(point,&flags)) != NULL )
		{
			SelectDropTarget( hItem );
			m_hItemDragD = hItem;
		}
		CImageList::DragShowNolock( true );

		//当条目被拖曳到左边缘时，将条目放在根下
		CRect  rect;
		GetClientRect( &rect );
		if( point.x < rect.left + 20 )
			m_hItemDragD = NULL;
	}

	CTreeCtrl::OnMouseMove(nFlags, point);}

void CXTreeCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CTreeCtrl::OnLButtonUp(nFlags, point);
	
	// 如果是拖动状态
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
		// 目标节点不能是数据，文件夹或者文件模板
		if (pDestData->GetItemType() == TEMP_ITEM_DATA 
			|| pDestData->GetItemType() == TEMP_ITEM_DIR
			|| pDestData->GetItemType() == TEMP_ITEM_ORIGIN)
		{
			return;
		}

		// 源和目标节点是否在同一个模板表里
		if (!CCatalogTree::GetSingletonPtr()->IsSameDataTable(m_hItemDragS, m_hItemDragD))
		{
			return;
		}
		
		

	/*	if (pData->GetItemType() != TEMP_ITEM_DATA && pData->GetItemType() != TEMP_ITEM_FILTER)
		{
			return;
		}*/

		//如果是由父节点拖向子节点
		while( (htiParent = GetParentItem(htiParent)) != NULL )
		{
			if( htiParent == m_hItemDragS )
			{
				//建立一个临时节点以完成操作
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//处理无意拖曳
	m_dwDragStart = GetTickCount();
	CTreeCtrl::OnLButtonDown(nFlags, point);
}

void CXTreeCtrl::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//鼠标敏感节点
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
	//处理拖曳过程中的滚动问题
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
			//向上滚动
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
			//向下滚动
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


//拷贝条目
HTREEITEM CXTreeCtrl::CopyItem(HTREEITEM hItem, HTREEITEM htiNewParent, HTREEITEM htiAfter)
{
	TV_INSERTSTRUCT  tvstruct;
	HTREEITEM        hNewItem;
	CString          sText;

	//得到源条目的信息
	tvstruct.item.hItem = hItem;
	tvstruct.item.mask  = TVIF_CHILDREN|TVIF_HANDLE|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	GetItem( &tvstruct.item );
	sText = GetItemText( hItem );
	tvstruct.item.cchTextMax = sText.GetLength ();
	tvstruct.item.pszText    = sText.LockBuffer ();

	//将条目插入到合适的位置
	tvstruct.hParent         = htiNewParent;
	tvstruct.hInsertAfter    = htiAfter;
	tvstruct.item.mask       = TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT;
	hNewItem = InsertItem( &tvstruct );
	sText.ReleaseBuffer ();

	//限制拷贝条目数据和条目状态
	SetItemData( hNewItem,GetItemData(hItem) );
	SetItemState( hNewItem,GetItemState(hItem,TVIS_STATEIMAGEMASK),TVIS_STATEIMAGEMASK);

	return hNewItem;
}

//拷贝分支
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
