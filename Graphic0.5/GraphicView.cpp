// GraphicView.cpp : implementation of the CGraphicView class
//

#include "StdAfx.h"
#include "Graphic.h"

#include "GraphicDoc.h"
#include "GraphicView.h"
#include "SettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraphicView

IMPLEMENT_DYNCREATE(CGraphicView, CView)

BEGIN_MESSAGE_MAP(CGraphicView, CView)
	//{{AFX_MSG_MAP(CGraphicView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_COMMAND(IDM_LINE, OnLine)
	ON_COMMAND(IDM_RECTANGLE, OnRectangle)
	ON_COMMAND(IDM_DOT, OnDot)
	ON_COMMAND(IDM_ELLIPSE, OnEllipse)
	ON_COMMAND(IDM_SECTOR, OnSector)
	ON_COMMAND(IDM_Polyline, OnPolyline)
	ON_WM_MOUSEMOVE()
	ON_COMMAND(IDM_SIMPLEBRUSH, OnSimplebrush)
	ON_COMMAND(IDM_BITMAPBRUSH, OnBitmapbrush)
	ON_COMMAND(IDM_TRANSPARENTBRUSH, OnTransparentbrush)
 	ON_COMMAND(IDM_COLOR, OnColor)
	ON_COMMAND(IDM_SETTING, OnSetting)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraphicView construction/destruction

CGraphicView::CGraphicView()
{
	// TODO: add construction code here
	m_nDrawType=0;
	m_bDraw=false;
	m_LineStyle=0;
	m_LineWidth=1;
	m_Color=RGB(50,200,100);
	m_nFILLMODEL=1;
}

CGraphicView::~CGraphicView()
{
}

BOOL CGraphicView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CGraphicView drawing

void CGraphicView::OnDraw(CDC* pDC)
{
	CGraphicDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here

  //GDI+ʹ�ó���
	Graphics graphics(pDC->m_hDC);
	Pen pen(Color(255, 0, 255));
	graphics.DrawLine(&pen, 0, 0, 200, 100);

}

/////////////////////////////////////////////////////////////////////////////
// CGraphicView printing

BOOL CGraphicView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGraphicView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGraphicView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CGraphicView diagnostics

#ifdef _DEBUG
void CGraphicView::AssertValid() const
{
	CView::AssertValid();
}

void CGraphicView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGraphicDoc* CGraphicView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGraphicDoc)));
	return (CGraphicDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGraphicView message handlers


void CGraphicView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_ptOrigin=point;
	m_bDraw=true;
	CView::OnLButtonDown(nFlags, point);
}

void CGraphicView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//����������豸����
	CClientDC dc(this);
	m_bDraw=false;

	CBrush brush;
	CBrush *pOldBrush;
	if(m_nFILLMODEL==1){
	   CBrush *pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
       //���ջ�ˢѡ���豸������
	    pOldBrush= dc.SelectObject(pBrush);
	}else{
		brush.CreateSolidBrush(m_Color);
		pOldBrush=dc.SelectObject(&brush);
	}
	CPen pen(m_LineStyle,m_LineWidth,m_Color);//��������
	dc.SelectObject(&pen);		//ѡ�񻭱�

	switch (m_nDrawType){
	  case 1:/*���Ƶ�*/ 
		  dc.SetPixel(point,m_Color); 
		  break;   
	  case 2:/*����ֱ��*/
		  dc.MoveTo(m_ptOrigin);
		  /*����MoveTo�����ƶ���ԭ��*/
		  dc.LineTo(point);
		  /*����LineTo�������Ƶ��յ�*/
		  break;
	  case 3:/*���ƾ���*/
		  dc.Rectangle(CRect(m_ptOrigin,point));
		  //�ָ���ǰ�Ļ�ˢ
		  dc.SelectObject(pOldBrush);
		  break;
	  case 4:/*������Բ*/
		  dc.Ellipse(CRect(m_ptOrigin,point));
		  //�ָ���ǰ�Ļ�ˢ
		  dc.SelectObject(pOldBrush);
		  break;
	}
	//-_-  case���޷���������
	if(m_nDrawType==7){
		CBrush brush(m_Color);
	    //���û�ˢ��������ҷ�������γɵľ������� 
        dc.FillRect(CRect(m_ptOrigin,point),&brush);
	}
	if(m_nDrawType==8){
		  //����λͼ����
		  CBitmap bitmap;
		  //����λͼ��Դ
		  bitmap.LoadBitmap(IDB_BITMAP1);
		  //����λͼ��ˢ
		  CBrush brush(&bitmap);
		  //���ú�ɫ��ˢ��������ҷ�������γɵľ�������
		  dc.FillRect(CRect(m_ptOrigin,point),&brush);
	}
	if(m_nDrawType==9){
		  //����һ���ջ�ˢ
		  CBrush *pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
		  //���ջ�ˢѡ���豸������
		  CBrush *pOldBrush = dc.SelectObject(pBrush);
		  //����һ������
		  dc.Rectangle(CRect(m_ptOrigin,point));
		  //�ָ���ǰ�Ļ�ˢ
		  dc.SelectObject(pOldBrush);
	}
//??
	OnPrepareDC(&dc);
	dc.DPtoLP(&m_ptOrigin);
	dc.DPtoLP(&point);

	CView::OnLButtonUp(nFlags, point);
}

void CGraphicView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//����������豸����
	CClientDC dc(this);
	//��������
	CPen pen (m_LineStyle,m_LineWidth, m_Color);
	//�Ѵ����Ļ���ѡ���豸����
	CPen *pOldpen=dc.SelectObject(&pen);

	if(m_bDraw==true)
		switch(m_nDrawType){
			case 5:
				dc.MoveTo(m_ptOrigin);                  
				dc.LineTo(point);
				break;
			case 6:
				dc.MoveTo(m_ptOrigin);                  
				dc.LineTo(point);
				//�޸��߶ε����        
				m_ptOrigin=point;
				break;
		}
	//�ָ��豸����
	dc.SelectObject(pOldpen);


	CView::OnMouseMove(nFlags, point);
}


void CGraphicView::OnSetting(){
	//TODO: Add your command handler code here
	SettingDlg dlg;
	dlg.m_LineWidth=m_LineWidth;
	dlg.m_LineStyle=m_LineStyle;
	dlg.m_nFILLMODEL=m_nFILLMODEL;
	if(IDOK==dlg.DoModal()){
		m_LineWidth=dlg.m_LineWidth;
		m_LineStyle=dlg.m_LineStyle;
		m_nFILLMODEL=dlg.m_nFILLMODEL;
	}
} 

void CGraphicView::OnColor()  {
	// TODO: Add your command handler code here
	CColorDialog dlg;
	dlg.m_cc.Flags|=CC_RGBINIT|CC_FULLOPEN;
	dlg.m_cc.rgbResult=RGB(50,200,100);
	if(IDOK==dlg.DoModal()){
		m_Color=dlg.m_cc.rgbResult; 
	}
}


///////////////////////////////////////////

void CGraphicView::OnDot() 
{
	// TODO: Add your command handler code here
	m_nDrawType=1;
}

void CGraphicView::OnLine() 
{
	// TODO: Add your command handler code here
	m_nDrawType=2;
}

void CGraphicView::OnRectangle() 
{
	// TODO: Add your command handler code here
	m_nDrawType=3;
}


void CGraphicView::OnEllipse() 
{
	// TODO: Add your command handler code here
	m_nDrawType=4;
}


void CGraphicView::OnSector() 
{
	// TODO: Add your command handler code here
	m_nDrawType=5;
}


void CGraphicView::OnPolyline() 
{
	// TODO: Add your command handler code here
	m_nDrawType=6;
}

void CGraphicView::OnSimplebrush() 
{
	// TODO: Add your command handler code here
	m_nDrawType=7;
}

void CGraphicView::OnBitmapbrush() 
{
	// TODO: Add your command handler code here
	m_nDrawType=8;
}

void CGraphicView::OnTransparentbrush() 
{
	// TODO: Add your command handler code here
	m_nDrawType=9;
}


void CGraphicView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CGraphicDoc* pDoc = GetDocument();//�õ��ĵ�ָ��,ע��,�ĵ����������빤�����йص�!!��ͬ�ĳ���һ��.
	ASSERT_VALID(pDoc); 
	
	//�Ƿ��Ѵ�ĳ��BMP�ļ�
	if(pDoc->flagOpen==1)
	{
		//���������ʾDIB
		SetDIBitsToDevice(dc.m_hDC,  //DIB��������豸������
			0,               //�豸��������λͼ�����ʼ�߼�x��ַ
			0,               //�豸��������λͼ�����ʼ�߼�x��ַ 
			pDoc->bi.biWidth,  //DIB�Ŀ��
			pDoc->bi.biHeight, //DIB�ĸ߶�
			0,                 //DIB��ʼ��ȡ������������ݵ�xλ��
			0,                 //DIB��ʼ��ȡ������������ݵ�yλ��
			0,                 //DIB�����ص�ˮƽ�к�,����ӦlpBits�ڴ滺������һ������
			pDoc->bi.biHeight, //DIB����������Ӧ��������lpBits��ָ�ڴ滺�����е�����
			pDoc->lpbuf,       //�����������ݵ��ڴ滺������ָ��
			pDoc->pbi,        //ָ���ʼ���˵�BITMAPINFO���ݽṹ��ָ�룬������λͼ�Ĵ�С��ɫ������
			DIB_RGB_COLORS);   //ָ������ʾ����ɫ
	  Invalidate(FALSE);
	}
	// Do not call CView::OnPaint() for painting messages
}
