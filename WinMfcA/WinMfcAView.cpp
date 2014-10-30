
// WinMfcAView.cpp : CWinMfcAView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "WinMfcA.h"
#endif

#include "WinMfcADoc.h"
#include "WinMfcAView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWinMfcAView

IMPLEMENT_DYNCREATE(CWinMfcAView, CView)

BEGIN_MESSAGE_MAP(CWinMfcAView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CWinMfcAView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CWinMfcAView ����/����

CWinMfcAView::CWinMfcAView()
{
	// TODO: �ڴ˴���ӹ������

}

CWinMfcAView::~CWinMfcAView()
{
}

BOOL CWinMfcAView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CWinMfcAView ����

void CWinMfcAView::OnDraw(CDC* /*pDC*/)
{
	CWinMfcADoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CWinMfcAView ��ӡ


void CWinMfcAView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CWinMfcAView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CWinMfcAView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CWinMfcAView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CWinMfcAView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CWinMfcAView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CWinMfcAView ���

#ifdef _DEBUG
void CWinMfcAView::AssertValid() const
{
	CView::AssertValid();
}

void CWinMfcAView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWinMfcADoc* CWinMfcAView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWinMfcADoc)));
	return (CWinMfcADoc*)m_pDocument;
}
#endif //_DEBUG


// CWinMfcAView ��Ϣ�������
