// ContentView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "ContentView.h"
#include "ManageOrder.h"
#include "NewOrder.h"
#include "SalesView.h"
// CContentView

IMPLEMENT_DYNCREATE(CContentView, CListView)

CContentView::CContentView()
{

}

CContentView::~CContentView()
{
}

BEGIN_MESSAGE_MAP(CContentView, CListView)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CContentView::OnNMDblclk)
END_MESSAGE_MAP()


// CContentView 진단입니다.

#ifdef _DEBUG
void CContentView::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void CContentView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG


// CContentView 메시지 처리기입니다.


BOOL CContentView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	/*
	main contents가 표시되는 CRightContainerView는
	기본적으로 listview의 형태를 띈다
	*/
	//cs.style |= LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL;

	
	return CListView::PreCreateWindow(cs);
}

void CContentView::DeleteContent(CListCtrl * m_list)
{
	/*
		컨텐츠 화면을 전부 지운다
	*/

	// 리스트 항목을 지운다

	CHeaderCtrl *p_headerCtrl;

	p_headerCtrl = m_list->GetHeaderCtrl();
	int columnCnt = 0;
	columnCnt = p_headerCtrl->GetItemCount();

	if (columnCnt > 0)
	{
		m_list->DeleteAllItems();
		for (int i = 0; i < columnCnt; i++)
			m_list->DeleteColumn(0);
	}

}

void CContentView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CUR_CONTENT = lHint;
	m_list = &GetListCtrl();

	// 데이터베이스에 연결
	CDatabase db_content;
	CRecordset recSet(&db_content);

	TRY
	{
		db_content.OpenEx(_T("DSN=UOS25;UID=UOS25;PWD=l27094824"));
	}
		CATCH(CException, e)
	{
		TCHAR errMSG[255];

		e->GetErrorMessage(errMSG, 255);
		AfxMessageBox(errMSG, MB_ICONERROR);


		if (db_content.IsOpen())
			db_content.Close();

	}
	END_CATCH

	

	switch (lHint)
	{
	case 0:
	{
		// UOS25 로고화면
		
		CClientDC pDC(this);//현재 다이얼로그의 타이틀바를 제외한 영역을 얻는다.
		CDC memDC;
		CPen pen;
		CBrush brush;
		pen.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
		brush.CreateSolidBrush(RGB(255, 255, 255));

		CPen *p = pDC.SelectObject(&pen);
		CBrush *b = pDC.SelectObject(&brush);

		pDC.Rectangle(0, 0, 1500, 800);
		pDC.SelectObject(p);
		pDC.SelectObject(b);
		
		memDC.CreateCompatibleDC(&pDC);//CDC와 CClinetDC를 연결해주는 구문

		CBitmap m_bitMain;
		m_bitMain.LoadBitmapW(IDB_LOGO);
		CBitmap *oldbm = memDC.SelectObject(&m_bitMain);
		//	pDC.StretchBlt(17, 23, 300, 300, &memDC, 0, 0, 350, 350, SRCCOPY);
		pDC.BitBlt(150, 100, 888, 396, &memDC, 0, 0, SRCCOPY);
		//bitblt함수를 사용하여 실제 bmp그림파일을 화면에 출력한다.
		//좌표 10,10위치에 300*300의 크기로 그림을 그린다.
		//원본그림의 왼쪽 위 포인트를 0,0으로 설정한다.
		//bmp파일을 사용하므로 모든 출력은 픽셀을 기준으로 한다.

		break;
	}
	case 1:	
	{
		// 주문 관리
		// m_list = &GetListCtrl();
		m_list->ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL, 0);
		m_list->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);


		int idx = 1;
		recSet.Open(CRecordset::dynaset, L"select distinct order_date, order_id from order_list order by order_id");

		if(m_list->GetItemCount() > 0)
			DeleteContent(m_list);

		m_list->InsertColumn(idx, L"주문 날짜", LVCFMT_CENTER, 400);

		while (!recSet.IsEOF())
		{
			CString order_date = L" ", order_list;
			recSet.GetFieldValue(_T("ORDER_DATE"), order_date);
			AfxExtractSubString(order_date, order_date, 0, ' ');
			order_list.Format(L"주문번호 %d | %s", idx+1000, order_date);

			m_list->InsertItem(idx, order_list, 30);
			m_list->SetItemText(idx, 0, order_list);
			idx++;
			recSet.MoveNext();
		}

		recSet.Close();
		m_list->InsertColumn(idx, L"+ 새 주문 하기", LVCFMT_LEFT, 400);
		break;
	}
	case 2:
	{
		// 반품 관리
		if (m_list->GetItemCount() > 0)
			DeleteContent(m_list);

		m_list->ModifyStyle(LVS_TYPEMASK, 0);
		
		// Create Button
		//CButton * btn1;
		//btn1->Create(L"My Button", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(300, 300, 300, 300), pSender, 103);
		//btn1->ShowWindow(SW_SHOW);
		
		break;
	}
	case 3:
	{
		// 판매 관리
		//sales_view = new CSalesView();
		//sales_view->Create(NULL, NULL, WS_CHILD, CRect(0,0,0,0), pSender, 200, this);
		break;
	}
	case 4:
	{
		// 입고 관리
		break;
	}
	case 5:
	{
		// 출고 관리
		break;
	}
	case 6:
	{
		// 물품 등록
		break;
	}
	case 7:
	{
		// 물품 삭제
		break;
	}
	case 8:
	{
		// 수입 관리
		break;
	}
	case 9:
	{
		// 지출 관리
		break;
	}
	case 10:
	{
		// 직원 고용
		break;
	}
	case 11:
	{
		// 직원 해고
		break;
	}
	default:
		break;
	}

	return;
}


void CContentView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	
	//m_list = &GetListCtrl();
	//m_list->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	
	// 컬럼을 보이지 않도록 하는 스타일
	//m_list->ModifyStyle(0, LVS_NOCOLUMNHEADER);
}

/*
	리스트를 더블클릭할 경우 페이지에 따라 필요한 다이얼로그 띄운다
*/
void CContentView::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>( pNMHDR );
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	switch (CUR_CONTENT)
	{
	case 1:
	{
		if(pNMItemActivate->iItem)
		if (pNMItemActivate->iItem != -1)
		{
			// 클릭된 행 넘버값을 받아온다
			NM_LISTVIEW * pNMListView = (NM_LISTVIEW*) pNMHDR;
			int cur_idx = pNMListView->iItem;

			
			// idx번째 행의 맨 앞 값을 받아와 현재 날짜 객체로 저장
			// 선택된 항목의 CString값을 받아오는 코드입니다

			// current_date = m_list->GetItemText(cur_idx, 0);
			
			// 다이얼로그 생성
			dlg_manage_order = new CManageOrder(this, cur_idx);
			dlg_manage_order->Create(CManageOrder::IDD);
			dlg_manage_order->ShowWindow(SW_SHOW);

		}
		else
		{
			dlg_new_order = new NewOrder();
			dlg_new_order->Create(NewOrder::IDD);
			dlg_new_order->ShowWindow(SW_SHOW);
		}
		break;
	}

	default:
		break;
	}

	*pResult = 0;
}