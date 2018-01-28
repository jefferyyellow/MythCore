#include "stdafx.h"  
#include "GridCell.h"  
#include "GridCtrl.h"  
#include "GridCellButton.h"  
#include "../TaskEditorView.h"
#include "../MainFrm.h"
#ifdef _DEBUG  
#define new DEBUG_NEW  
#undef THIS_FILE  
static char THIS_FILE[] = __FILE__;
#endif  

IMPLEMENT_DYNCREATE(CGridCellButton, CGridCell)
CGridCellButton::CGridCellButton(void)
{
	m_bPushing = FALSE;
}
CGridCellButton::~CGridCellButton(void)
{
}
BOOL CGridCellButton::Draw(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBkgnd /* = TRUE */)
{
	m_rect = rect;
	pDC->SetBkMode(TRANSPARENT);
	rect.DeflateRect(GetMargin(), 0);
	CFont* pOldFont = pDC->SelectObject(GetFontObject());
	pDC->DrawFrameControl(rect, DFC_BUTTON, m_bPushing?DFCS_BUTTONPUSH | DFCS_PUSHED:DFCS_BUTTONPUSH);  
	COLORREF ColorCurrent = pDC->GetTextColor();
	pDC->SetTextColor(::GetSysColor(COLOR_BTNTEXT));
	pDC->DrawText(GetText(), -1, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
	pDC->SetTextColor(ColorCurrent);
	return TRUE;
}

void CGridCellButton::OnClickDown(CPoint PointCellRelative)
{
	m_bPushing = true;
}

void CGridCellButton::OnClick(CPoint PointCellRelative)
{
	m_bPushing = false;
	GetGrid()->InvalidateRect(m_rect);

	//CTaskEditorView* pView = (CTaskEditorView*)((CMainFrame*)AfxGetMainWnd())->MDIGetActive()->GetActiveView();
	//if (NULL == pView)
	//{
	//	return;
	//}
	//CCellID cell = pView->m_pCondGrid->GetFocusCell();

	//if (cell.row == pView->m_pCondGrid->GetRowCount() - 1)
	//{
	//	pView->m_pCondGrid->SetRowCount(pView->m_pCondGrid->GetRowCount() + 1);
	//	pView->m_pCondGrid->SetItemText(pView->m_pCondGrid->GetRowCount() - 1, 0, _T("新的一行"));
	//}
	//else
	//{
	//	pView->m_pCondGrid->InsertRow(_T("新的一行"), cell.row + 1);
	//}

	//if (!pView->m_pCondGrid->SetCellType(cell.row + 1, 0, RUNTIME_CLASS(CGridCellCombo)))
	//{
	//	continue;
	//}

	//CStringArray tStringOption;
	//for (int nOptionNum = 0; nOptionNum < rOptionList.size(); ++nOptionNum)
	//{
	//	tStringOption.Add(rOptionList[nOptionNum].mDes.c_str());
	//}

	//CGridCellCombo *pCell = (CGridCellCombo*)m_pGrid->GetCell(i, j);
	//pCell->SetOptions(tStringOption);
	//pCell->SetStyle(CBS_DROPDOWN);


	//pView->m_pCondGrid->Invalidate();
}