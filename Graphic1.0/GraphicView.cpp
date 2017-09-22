// GraphicView.cpp : implementation of the CGraphicView class
//

#include "StdAfx.h"
#include "Graphic.h"

#include "GraphicDoc.h"
#include "GraphicView.h"
#include "SettingDlg.h"
#include "RotateDlg.h"
#include "ZoomDlg.h"
#include "ConfirmDlg.h"
#include "TranslateTrans.h"
#include "TextDlg.h"
#include "COLOR_DATA.h"
#include <cstdlib>
#include <vector>
#include <cmath>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;

#endif

	using namespace Gdiplus;
	using namespace std;
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
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_ERASE, OnErase)
	ON_COMMAND(ID_FILLAREA, OnFillarea)
	ON_COMMAND(ID_ROTATE, OnRotate)
	ON_WM_PAINT()
	ON_COMMAND(ID_GRAY, OnGray)
	ON_COMMAND(ID_ALL, OnAll)
	ON_COMMAND(ID_ZOOM, OnZoom)
	ON_COMMAND(ID_TRANSLATE, OnTranslate)
	ON_COMMAND(ID_GAUSS, OnGauss)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(IDD_TEXT, OnText)
	ON_WM_CREATE()
	ON_COMMAND(IDD_MEDIAN, OnMedian)
	ON_COMMAND(IDD_BILATERAL, OnBilateral)
	ON_COMMAND(IDD_SIMPLE_NO_SCALE, OnSimpleNoScale)
	ON_COMMAND(IDD_NOT, OnNot)
	ON_COMMAND(IDD_LEVELFILP, OnLevelfilp)
	ON_COMMAND(IDD_VERTICALFILP, OnVerticalfilp)
	ON_COMMAND(IDD_LAPLACE, OnLaplace)
	ON_COMMAND(IDD_SOBEL, OnSobel)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(IDD_GUN, OnGun)
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
	m_LineWidth=5;
	m_Color=RGB(50,200,100);
	m_nFILLMODEL=1;
	m_nAngle=0;
	m_nRotateModel=0;
	imagePointX=0;
	imagePointY=0;
	m_TranslateLevel=0;
	m_TranslateVertical=0;
	m_rTrans=1;
	m_cTrans=1;
	isGray=false;
	imageConfirm=false;
	isAll=false;
}


CGraphicView::~CGraphicView()
{
}

void CGraphicView::initial(){
	//initial
	m_nDrawType=0;
	m_bDraw=false;
	m_LineStyle=0;
	m_LineWidth=5;
	m_Color=RGB(50,200,100);
	m_nFILLMODEL=1;
	m_nAngle=0;
	m_nRotateModel=0;
	imagePointX=0;
	imagePointY=0;
	m_TranslateLevel=0;
	m_TranslateVertical=0;
	m_rTrans=1;
	m_cTrans=1;
	isGray=false;
	imageConfirm=false;
	isAll=false;
}

BOOL CGraphicView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	// �̶����ָ����ʽ��MoveMouseʱ����
    cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS,NULL,(HBRUSH)GetStockObject(WHITE_BRUSH),NULL);

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CGraphicView drawing


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
//cancel system background erase function
BOOL CGraphicView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	// nothing
	//return CView::OnEraseBkgnd(pDC);
	return true;
}

int CGraphicView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	/*
    TEXTMETRIC tm;  
    MemDC.GetTextMetrics(&tm);//��ȡ��ǰ�ַ��߶ȺͿ��ֵ��������������Ĵ�С  
    CreateSolidCaret(tm.tmAveCharWidth/8,tm.tmHeight);//���������  
    ShowCaret();//��ʾ�����  
  
    SetTimer(1,50, NULL);//���ö�ʱ����Ϣ��1��ʾ��ʱ����ʶ����50��ʾ50ms����һ����Ϣ��NULLָ���ǽ�����Ϣ���в���WM_TIMER��Ϣ  

	*/
	// TODO: Add your specialized creation code here

	return 0;
}

inline void CGraphicView::NormalTrans(){
//��ȱ��
/*
	CDC* pDC = GetDC();
	Graphics graphics(pDC->GetSafeHdc());

	pDC->BitBlt(0, 0 ,nWidth,nHeight,&MemDC,0,0,SRCCOPY);
	//ƽ��
	graphics.TranslateTransform((float)m_TranslateLevel, (float)m_TranslateVertical); 
	//����
	graphics.ScaleTransform(m_rTrans,m_cTrans);
  	//Դ���ƶ�����ת����
	graphics.TranslateTransform(1.0f*(imagePointX+m_ZoomRow/2),1.0f*(imagePointY+m_ZoomColumn/2));
	//��ת
	if(!m_nRotateModel)graphics.RotateTransform(1.0f*m_nAngle); 
	else graphics.RotateTransform(1.0f*(1080-m_nAngle));
	//��ԭԴ��	
	graphics.TranslateTransform(1.0f*(0-(imagePointX+m_ZoomRow/2)), 1.0f*(0-(imagePointY+m_ZoomColumn/2)));

	ReleaseDC(pDC);
*/
}

void CGraphicView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CClientDC dc(this);
	CBrush brush(m_Color);
	CGraphicDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
    CDC *pDC=GetDC();
    CRect rect;
    this->GetClientRect(&rect);

	m_ptOrigin=point;
	m_bDraw=true;
    
	
	//��ȡ��ĻDC
	HDC hDC=pDC->GetSafeHdc();
	//��ȡ��ǰ���RGBֵ
	COLORREF clr=::GetPixel(hDC, point.x, point.y); 
	Gdiplus::Graphics graphics(MemDC.GetSafeHdc());

	//�ı����ָ����״
	HCURSOR hCur;
	switch(m_nDrawType){
	case 1://��
		MemDC.SetPixel(point,m_Color); 
		pDC->BitBlt(0, 0 ,nWidth,nHeight,&MemDC,0,0,SRCCOPY);
		break;   
	case 2://ֱ��
	case 3://����
    case 4://��Բ
    case 5://����
		hCur=LoadCursor(NULL,IDC_CROSS);
		::SetCursor(hCur);
		break;
	case 10://��Ƥ��
		hCur=LoadCursor(NULL,IDC_NO);
		::SetCursor(hCur);
	    break;
	case 11:
		//���������á��������е㿨
		if(clr!=m_Color){
			MemDC.SelectObject(brush);									
			ExtFloodFill(MemDC,point.x,point.y,clr,FLOODFILLSURFACE);
			pDC->BitBlt(0, 0 ,nWidth,nHeight,&MemDC,0,0,SRCCOPY);
		}
		break;
	/*fail
	case 12://rotate

		if (!pDoc) return;
		//using GDI+
		CRect rect;
		this->GetClientRect(&rect);
		Bitmap bm(rect.Width(),rect.Height(),PixelFormat24bppRGB);
		bm.RotateFlip(RotateFlipType(Rotate90FlipNone));
		Graphics myg2(dc);
		CDC *pDC=GetDC();
		CDC dcCompatible;
        dcCompatible.CreateCompatibleDC( pDC );  // �����뵱ǰDC��pDC�����ݵ�DC
        dcCompatible.SelectObject( &bm );
		pDC->FillSolidRect(&rect,RGB(0,0,0));
		pDC->BitBlt( 0, 0, rect.Width(), rect.Height(), &dcCompatible, 0, 0, SRCCOPY );


		break;
	*/
	}


	ReleaseDC(pDC);
	CView::OnLButtonDown(nFlags, point);
}

void CGraphicView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//����������豸����
	CClientDC dc(this);
	CDC* pDC=GetDC();
    CRect rect;
    this->GetClientRect(&rect);

	m_bDraw=false;

	//�ָ����ָ����״
	HCURSOR hCur=LoadCursor(NULL,IDC_ARROW);
    ::SetCursor(hCur);

	CBrush brush;
	CBrush *pOldBrush;

	//ʵ��/����
	if(m_nFILLMODEL==1){
	    CBrush *pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
       //���ջ�ˢѡ���豸������
	    pOldBrush= MemDC.SelectObject(pBrush);
	}else{
		brush.CreateSolidBrush(m_Color);
		pOldBrush=MemDC.SelectObject(&brush);
	}

	
    //�����߿����1��ʵ��/����/����/�㻮��/˫�㻮��
	LOGBRUSH logBrush;  
	logBrush.lbStyle = BS_SOLID; //��ˢ��ʽ
	logBrush.lbColor = m_Color;  //��ˢ��ɫ
	//����ʵ�߻���
	CPen pen1(m_LineStyle,m_LineWidth,m_Color);
	//�������߻���
	CPen pen2(PS_DASH | PS_GEOMETRIC | PS_ENDCAP_ROUND,m_LineWidth,&logBrush);
	//�������߻���
	CPen pen3(PS_DOT | PS_GEOMETRIC | PS_ENDCAP_ROUND,m_LineWidth,&logBrush);
	//�����㻮�߻���
	CPen pen4(PS_DASHDOT | PS_GEOMETRIC | PS_ENDCAP_ROUND,m_LineWidth,&logBrush);
	//����˫�㻮�߻���
	CPen pen5(PS_DASHDOTDOT | PS_GEOMETRIC | PS_ENDCAP_ROUND,m_LineWidth,&logBrush);
	MemDC.SelectObject(&pen1);
	if(m_LineStyle==1)MemDC.SelectObject(&pen2);		//ѡ�񻭱�
	else if(m_LineStyle==2)MemDC.SelectObject(&pen3);
	else if(m_LineStyle==3)MemDC.SelectObject(&pen4);
	else if(m_LineStyle!=0)MemDC.SelectObject(&pen5);		//ѡ�񻭱�
	

	switch (m_nDrawType){
	  case 2:/*����ֱ��*/
		  MemDC.MoveTo(m_ptOrigin);
		  /*����MoveTo�����ƶ���ԭ��*/
		  MemDC.LineTo(point);
		  /*����LineTo�������Ƶ��յ�*/
		  break;
	  case 3:/*���ƾ���*/
		  MemDC.Rectangle(CRect(m_ptOrigin,point));
		  //�ָ���ǰ�Ļ�ˢ
		  MemDC.SelectObject(pOldBrush);
		  break;
	  case 4:/*������Բ*/
		  MemDC.Ellipse(CRect(m_ptOrigin,point));
		  //�ָ���ǰ�Ļ�ˢ
		  MemDC.SelectObject(pOldBrush);
		  break;
	}

	//-_-  case���޷���������
	if(m_nDrawType==7){
		//Simplebrush
		CBrush brush(m_Color);
	    //���û�ˢ��������ҷ�������γɵľ������� 
        MemDC.FillRect(CRect(m_ptOrigin,point),&brush);
	}
	if(m_nDrawType==8){
		//Bitmapbrush
		//����λͼ����
		CBitmap bitmap;
		//����λͼ��Դ
		bitmap.LoadBitmap(IDB_BITMAP1);
		//����λͼ��ˢ
		CBrush brush(&bitmap);
		//���ú�ɫ��ˢ��������ҷ�������γɵľ�������
		MemDC.FillRect(CRect(m_ptOrigin,point),&brush);
	}
	if(m_nDrawType==9){
		//Transparentbrush
		//����һ���ջ�ˢ
		CBrush *pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
		//���ջ�ˢѡ���豸������
		CBrush *pOldBrush = MemDC.SelectObject(pBrush);
		//����һ������
		MemDC.Rectangle(CRect(m_ptOrigin,point));
		//�ָ���ǰ�Ļ�ˢ
		MemDC.SelectObject(pOldBrush);
	}
	pDC->BitBlt(0, 0 ,nWidth,nHeight,&MemDC,0,0,SRCCOPY);
////??
	OnPrepareDC(&MemDC);
	MemDC.DPtoLP(&m_ptOrigin);
	MemDC.DPtoLP(&point);


	ReleaseDC(pDC);
	CView::OnLButtonUp(nFlags, point);
}

void CGraphicView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//����������豸����
	CClientDC dc(this);
	CDC *pDC=GetDC();
    CRect rect;
    this->GetClientRect(&rect);

	CBrush brush;
	CBrush *pOldBrush;

	//ʵ��/����
	if(m_nFILLMODEL==1){
	    CBrush *pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
       //���ջ�ˢѡ���豸������
	    pOldBrush=pDC->SelectObject(pBrush);
	}else{
		brush.CreateSolidBrush(m_Color);
		pOldBrush=pDC->SelectObject(&brush);
	}

    //�����߿����1��ʵ��/����/����/�㻮��/˫�㻮��
	LOGBRUSH logBrush;  
	logBrush.lbStyle = BS_SOLID; //��ˢ��ʽ
	logBrush.lbColor = m_Color;  //��ˢ��ɫ
	//����ʵ�߻���
	CPen pen1(m_LineStyle,m_LineWidth,m_Color);
	//�������߻���
	CPen pen2(PS_DASH | PS_GEOMETRIC | PS_ENDCAP_ROUND,m_LineWidth,&logBrush);
	//�������߻���
	CPen pen3(PS_DOT | PS_GEOMETRIC | PS_ENDCAP_ROUND,m_LineWidth,&logBrush);
	//�����㻮�߻���
	CPen pen4(PS_DASHDOT | PS_GEOMETRIC | PS_ENDCAP_ROUND,m_LineWidth,&logBrush);
	//����˫�㻮�߻���
	CPen pen5(PS_DASHDOTDOT | PS_GEOMETRIC | PS_ENDCAP_ROUND,m_LineWidth,&logBrush);
	MemDC.SelectObject(&pen1);pDC->SelectObject(&pen1);
	if(m_LineStyle==1)MemDC.SelectObject(&pen2),pDC->SelectObject(&pen2);
	else if(m_LineStyle==2)MemDC.SelectObject(&pen3),pDC->SelectObject(&pen3);
	else if(m_LineStyle==3)MemDC.SelectObject(&pen4),pDC->SelectObject(&pen4);
	else if(m_LineStyle!=0)MemDC.SelectObject(&pen5),pDC->SelectObject(&pen5);	

	if(m_bDraw==true){
		switch(m_nDrawType){
			case 2://Line
				pDC->BitBlt(0, 0 ,nWidth,nHeight,&MemDC,0,0,SRCCOPY);
				pDC->MoveTo(m_ptOrigin);  
				pDC->LineTo(point);  
				break;
			case 3://Rectangle
				pDC->BitBlt(0, 0 ,nWidth,nHeight,&MemDC,0,0,SRCCOPY);
				pDC->Rectangle(CRect(m_ptOrigin,point));
				//�ָ���ǰ�Ļ�ˢ
				pDC->SelectObject(pOldBrush);
				break;
			case 4:
			    pDC->BitBlt(0, 0 ,nWidth,nHeight,&MemDC,0,0,SRCCOPY);
				pDC->Ellipse(CRect(m_ptOrigin,point));
				//�ָ���ǰ�Ļ�ˢ
				pDC->SelectObject(pOldBrush);
				break;
			case 5://SECTOR
				MemDC.MoveTo(m_ptOrigin);                  
				MemDC.LineTo(point);
			    pDC->BitBlt(0, 0 ,nWidth,nHeight,&MemDC,0,0,SRCCOPY);
				break;
			case 6://Polyline
				MemDC.MoveTo(m_ptOrigin);                  
				MemDC.LineTo(point);
				//�޸��߶ε����        
				m_ptOrigin=point;
				pDC->BitBlt(0, 0 ,nWidth,nHeight,&MemDC,0,0,SRCCOPY); 
				break;
			case 10://erase
				CBrush brush(RGB(255,255,255));
				CPoint pt1;
				CPoint pt2;
				pt1.x=point.x-m_LineWidth*2;
				pt1.y=point.y-m_LineWidth*2;
				pt2.x=point.x+m_LineWidth*2;
				pt2.y=point.y+m_LineWidth*2;
				MemDC.FillRect(CRect(pt1,pt2),&brush);
				pDC->BitBlt(0, 0 ,nWidth,nHeight,&MemDC,0,0,SRCCOPY);  
				break;
		}

		//-_-  case���޷���������
		if(m_nDrawType==7){
			//Simplebrush
			pDC->BitBlt(0, 0 ,nWidth,nHeight,&MemDC,0,0,SRCCOPY);
			CBrush brush(m_Color);
			//���û�ˢ��������ҷ�������γɵľ������� 
			pDC->FillRect(CRect(m_ptOrigin,point),&brush);
		}
		if(m_nDrawType==8){
			//Bitmapbrush
			pDC->BitBlt(0, 0 ,nWidth,nHeight,&MemDC,0,0,SRCCOPY);
			//����λͼ����
			CBitmap bitmap;
			//����λͼ��Դ
			bitmap.LoadBitmap(IDB_BITMAP1);
			//����λͼ��ˢ
			CBrush brush(&bitmap);
			//���ú�ɫ��ˢ��������ҷ�������γɵľ�������
			pDC->FillRect(CRect(m_ptOrigin,point),&brush);
		}
		if(m_nDrawType==9){
			//Transparentbrush
			pDC->BitBlt(0, 0 ,nWidth,nHeight,&MemDC,0,0,SRCCOPY);
			//����һ���ջ�ˢ
			CBrush *pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
			//���ջ�ˢѡ���豸������
			CBrush *pOldBrush = pDC->SelectObject(pBrush);
			//����һ������
			pDC->Rectangle(CRect(m_ptOrigin,point));
			//�ָ���ǰ�Ļ�ˢ
			pDC->SelectObject(pOldBrush);
		}
		if(m_nDrawType==12){
		  sprayGun(point);		
		}
	}
	//�ָ��豸����
	CView::OnMouseMove(nFlags, point);
	ReleaseDC(pDC);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//���α任
void CGraphicView::OnLevelfilp() 
{
	// TODO: Add your command handler code here
	if(imageConfirm){
		AfxMessageBox("ͼ���ѹ̶���");
		return;
	}
	CDC* pDC = GetDC();
	Graphics graphics(pDC->GetSafeHdc());
    CRect rect;
    this->GetClientRect(&rect);

	CGraphicDoc* pDoc = GetDocument();//�õ��ĵ�ָ��,ע��,�ĵ����������빤�����йص�!!��ͬ�ĳ���һ��.
	ASSERT_VALID(pDoc);

	if(pDoc->flagOpen==0)
	{
	  AfxMessageBox("���ȴ�ͼƬ!");
    }
	else
	{
	  m_pImg->RotateFlip(RotateFlipType(RotateNoneFlipX));
	  
	  NormalTrans();
	  pDC->BitBlt(0, 0 ,nWidth,nHeight,&MemDC,0,0,SRCCOPY);
	  //ƽ��
	  graphics.TranslateTransform((float)m_TranslateLevel, (float)m_TranslateVertical); 
	  //����
	  graphics.ScaleTransform(m_rTrans,m_cTrans);
  	  //Դ���ƶ�����ת����
	  graphics.TranslateTransform(1.0f*(imagePointX+m_ZoomRow/2),1.0f*(imagePointY+m_ZoomColumn/2));
	  //��ת
	  if(!m_nRotateModel)graphics.RotateTransform(1.0f*m_nAngle); 
	  else graphics.RotateTransform(1.0f*(1080-m_nAngle));
	  //��ԭԴ��	
	  graphics.TranslateTransform(1.0f*(0-(imagePointX+m_ZoomRow/2)), 1.0f*(0-(imagePointY+m_ZoomColumn/2)));

	  graphics.DrawImage(m_pImg, imagePointX,imagePointY); // ����ͼ��
	
      
	}
	ReleaseDC(pDC);
}

void CGraphicView::OnVerticalfilp() 
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	if(imageConfirm){
		AfxMessageBox("ͼ���ѹ̶���");
		return;
	}
	CDC* pDC = GetDC();
	Graphics graphics(pDC->GetSafeHdc());
    CRect rect;
    this->GetClientRect(&rect);

	CGraphicDoc* pDoc = GetDocument();//�õ��ĵ�ָ��,ע��,�ĵ����������빤�����йص�!!��ͬ�ĳ���һ��.
	ASSERT_VALID(pDoc);

	if(pDoc->flagOpen==0)
	{
	  AfxMessageBox("���ȴ�ͼƬ!");
    }
	else
	{
	  m_pImg->RotateFlip(RotateFlipType(RotateNoneFlipY));
	  
	  NormalTrans();
	  pDC->BitBlt(0, 0 ,nWidth,nHeight,&MemDC,0,0,SRCCOPY);
	  //ƽ��
	  graphics.TranslateTransform((float)m_TranslateLevel, (float)m_TranslateVertical); 
	  //����
	  graphics.ScaleTransform(m_rTrans,m_cTrans);
  	  //Դ���ƶ�����ת����
	  graphics.TranslateTransform(1.0f*(imagePointX+m_ZoomRow/2),1.0f*(imagePointY+m_ZoomColumn/2));
	  //��ת
	  if(!m_nRotateModel)graphics.RotateTransform(1.0f*m_nAngle); 
	  else graphics.RotateTransform(1.0f*(1080-m_nAngle));
	  //��ԭԴ��	
	  graphics.TranslateTransform(1.0f*(0-(imagePointX+m_ZoomRow/2)), 1.0f*(0-(imagePointY+m_ZoomColumn/2)));

	  graphics.DrawImage(m_pImg, imagePointX,imagePointY); // ����ͼ��
	
      
	}
	ReleaseDC(pDC);
}

void CGraphicView::OnAll() 
{
	// TODO: Add your command handler code here
	if(imageConfirm){
		AfxMessageBox("ͼ���ѹ̶���");
		return;
	}
    CDC* pDC = GetDC();
	Graphics graphics(pDC->GetSafeHdc());
    CRect rect;
    this->GetClientRect(&rect);

	CGraphicDoc* pDoc = GetDocument();//�õ��ĵ�ָ��,ע��,�ĵ����������빤�����йص�!!��ͬ�ĳ���һ��.
	ASSERT_VALID(pDoc); 
	if(pDoc->flagOpen==0)
	{
	  AfxMessageBox("���ȴ�ͼƬ!");
	}else
	{
	  isAll=true;
	  pDC->BitBlt(0, 0 ,nWidth,nHeight,&MemDC,0,0,SRCCOPY);

	  graphics.DrawImage(m_pImg, 0, 0, rect.Width(),rect.Height()); // ����ͼ��
	}

	ReleaseDC(pDC);
}

void CGraphicView::OnZoom() 
{
	// TODO: Add your command handler code here
	if(imageConfirm){
		AfxMessageBox("ͼ���ѹ̶���");
		return;
	}
	CDC* pDC = GetDC();
	Graphics graphics(pDC->GetSafeHdc());
    CRect rect;
    this->GetClientRect(&rect);

	CGraphicDoc* pDoc = GetDocument();//�õ��ĵ�ָ��,ע��,�ĵ����������빤�����йص�!!��ͬ�ĳ���һ��.
	ASSERT_VALID(pDoc);

	if(pDoc->flagOpen==0)
	{
	  AfxMessageBox("���ȴ�ͼƬ!");
    }
	else
	{
	  CZoomDlg dlg;
	  dlg.m_ZoomRow=m_ZoomRow;
	  dlg.m_ZoomColumn=m_ZoomColumn;
	  if(IDOK==dlg.DoModal()){
		isAll=false;
		m_rTrans=(float)dlg.m_ZoomRow/(float)m_ZoomRow;
	    m_cTrans=(float)dlg.m_ZoomColumn/(float)m_ZoomColumn;

		NormalTrans();
		pDC->BitBlt(0, 0 ,nWidth,nHeight,&MemDC,0,0,SRCCOPY);
		//ƽ��
		graphics.TranslateTransform((float)m_TranslateLevel, (float)m_TranslateVertical); 
		//����
		graphics.ScaleTransform(m_rTrans,m_cTrans);
  		//Դ���ƶ�����ת����
		graphics.TranslateTransform(1.0f*(imagePointX+m_ZoomRow/2),1.0f*(imagePointY+m_ZoomColumn/2));
		//��ת
		if(!m_nRotateModel)graphics.RotateTransform(1.0f*m_nAngle); 
		else graphics.RotateTransform(1.0f*(1080-m_nAngle));
		//��ԭԴ��	
		graphics.TranslateTransform(1.0f*(0-(imagePointX+m_ZoomRow/2)), 1.0f*(0-(imagePointY+m_ZoomColumn/2)));

		graphics.DrawImage(m_pImg, imagePointX,imagePointY); // ����ͼ��
	
	  }
      
	}
	ReleaseDC(pDC);
}

void CGraphicView::OnRotate() 
{
	// TODO: Add your command handler code here
	if(imageConfirm){
		AfxMessageBox("ͼ���ѹ̶���");
		return;
	}
	CDC* pDC = GetDC();
	Graphics graphics(pDC->GetSafeHdc());

    CRect rect;
    this->GetClientRect(&rect);

	CGraphicDoc* pDoc = GetDocument();//�õ��ĵ�ָ��,ע��,�ĵ����������빤�����йص�!!��ͬ�ĳ���һ��.
	ASSERT_VALID(pDoc);

	if(pDoc->flagOpen==0)
	{
	  AfxMessageBox("���ȴ�ͼƬ!");
    }
	else
	{
	  CRotateDlg dlg;
	  dlg.m_nRotateModel=m_nRotateModel;
  	  dlg.m_nAngle=0;
	  if(IDOK==dlg.DoModal()){
		isAll=false;
		m_nRotateModel=dlg.m_nRotateModel;
		if(m_nRotateModel==0)m_nAngle+=dlg.m_nAngle;
		else m_nAngle-=dlg.m_nAngle;

		NormalTrans();
		pDC->BitBlt(0, 0 ,nWidth,nHeight,&MemDC,0,0,SRCCOPY);
		//ƽ��
		graphics.TranslateTransform((float)m_TranslateLevel, (float)m_TranslateVertical); 
		//����
		graphics.ScaleTransform(m_rTrans,m_cTrans);
  		//Դ���ƶ�����ת����
		graphics.TranslateTransform(1.0f*(imagePointX+m_ZoomRow/2),1.0f*(imagePointY+m_ZoomColumn/2));
		//��ת
		if(!m_nRotateModel)graphics.RotateTransform(1.0f*m_nAngle); 
		else graphics.RotateTransform(1.0f*(1080-m_nAngle));
		//��ԭԴ��	
		graphics.TranslateTransform(1.0f*(0-(imagePointX+m_ZoomRow/2)), 1.0f*(0-(imagePointY+m_ZoomColumn/2)));


		//��ĳ�������ʾͼ��
		graphics.DrawImage(m_pImg, imagePointX, imagePointY);

	  }
	}
	ReleaseDC(pDC);
}

void CGraphicView::OnTranslate() 
{
	// TODO: Add your command handler code here
	if(imageConfirm){
		AfxMessageBox("ͼ���ѹ̶���");
		return;
	}
	CDC* pDC = GetDC();
	Graphics graphics(pDC->GetSafeHdc());
    CRect rect;
    this->GetClientRect(&rect);

	CGraphicDoc* pDoc = GetDocument();//�õ��ĵ�ָ��,ע��,�ĵ����������빤�����йص�!!��ͬ�ĳ���һ��.
	ASSERT_VALID(pDoc);

	if(pDoc->flagOpen==0)
	{
	  AfxMessageBox("���ȴ�ͼƬ!");
    }
	else
	{
	  CTranslateTrans dlg;
	  dlg.m_TranslateLevel=0;
	  dlg.m_TranslateVertical=0;
	  if(IDOK==dlg.DoModal()){
		isAll=false;
		if(dlg.m_signal1==0)m_TranslateLevel+=dlg.m_TranslateLevel;
		else m_TranslateLevel-=dlg.m_TranslateLevel;
		if(dlg.m_signal2==0)m_TranslateVertical+=dlg.m_TranslateVertical;
		else m_TranslateVertical-=dlg.m_TranslateVertical;
		
		NormalTrans();
		pDC->BitBlt(0, 0 ,nWidth,nHeight,&MemDC,0,0,SRCCOPY);
		//ƽ��
		graphics.TranslateTransform((float)m_TranslateLevel, (float)m_TranslateVertical); 
		//����
		graphics.ScaleTransform(m_rTrans,m_cTrans);
  		//Դ���ƶ�����ת����
		graphics.TranslateTransform(1.0f*(imagePointX+m_ZoomRow/2),1.0f*(imagePointY+m_ZoomColumn/2));
		//��ת
		if(!m_nRotateModel)graphics.RotateTransform(1.0f*m_nAngle); 
		else graphics.RotateTransform(1.0f*(1080-m_nAngle));
		//��ԭԴ��	
		graphics.TranslateTransform(1.0f*(0-(imagePointX+m_ZoomRow/2)), 1.0f*(0-(imagePointY+m_ZoomColumn/2)));

		graphics.DrawImage(m_pImg, imagePointX,imagePointY); // ����ͼ��
	
	  }
	}
	ReleaseDC(pDC);
}



void CGraphicView::OnGray() 
{
	// TODO: Add your command handler code here
	if(imageConfirm){
		AfxMessageBox("ͼ���ѹ̶���");
		return;
	}
	CDC *pDC=GetDC();
	Graphics graphics(pDC->GetSafeHdc());
    CRect rect;
    this->GetClientRect(&rect);

	CGraphicDoc* pDoc = GetDocument();//�õ��ĵ�ָ��,ע��,�ĵ����������빤�����йص�!!��ͬ�ĳ���һ��.
	ASSERT_VALID(pDoc);

	if(pDoc->flagOpen==0)
	{
	  AfxMessageBox("���ȴ�ͼƬ!");
    }
	else
	{
	  isAll=false;
	  isGray=true;
	  
	  NormalTrans();
	  pDC->BitBlt(0, 0 ,nWidth,nHeight,&MemDC,0,0,SRCCOPY);
	  //ƽ��
	  graphics.TranslateTransform((float)m_TranslateLevel, (float)m_TranslateVertical); 
	  //����
	  graphics.ScaleTransform(m_rTrans,m_cTrans);
  	  //Դ���ƶ�����ת����
	  graphics.TranslateTransform(1.0f*(imagePointX+m_ZoomRow/2),1.0f*(imagePointY+m_ZoomColumn/2));
	  //��ת
	  if(!m_nRotateModel)graphics.RotateTransform(1.0f*m_nAngle); 
	  else graphics.RotateTransform(1.0f*(1080-m_nAngle));
	  //��ԭԴ��	
	  graphics.TranslateTransform(1.0f*(0-(imagePointX+m_ZoomRow/2)), 1.0f*(0-(imagePointY+m_ZoomColumn/2)));

	  ColorMatrix colorMatrix=
	  {
		 0.299f,0.299f,0.299f,0,0,
		0.587f,0.587f,0.587f,0,0,
		0.114f,0.114f,0.114f,0,0,
		0,0,0,1,0,
		0,0,0,0,1
	  };

	  ImageAttributes attr;
	  attr.SetColorMatrix(&colorMatrix);
	
	  Rect destRect(imagePointX,imagePointY,m_pImg->GetWidth(),m_pImg->GetHeight());
	  graphics.DrawImage(m_pImg, destRect, 0,0,m_pImg->GetWidth(),m_pImg->GetHeight(),UnitPixel,&attr);
	}

	ReleaseDC(pDC);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ͼ�������߶����---------------------------------
//��˹ƽ��  
void CGraphicView::OnGauss()   
{
	// TODO: Add your command handler code here
	CDC *pDC=GetDC(); 
	HDC hDC= pDC ->GetSafeHdc();
	Graphics graphics(pDC->GetSafeHdc());
    CRect rect;
    this->GetClientRect(&rect);
 
//������ʱͼƬ����ƽ����ͼ
	CDC temCDC;
	CBitmap bm;
	bm.CreateCompatibleBitmap(pDC,nWidth,nHeight);

	//���������Ļ��ʾ���ݵ��ڴ���ʾ�豸
	temCDC.CreateCompatibleDC(pDC);
	//���潨��һ������Ļ��ʾ���ݵ�λͼ������λͼ�Ĵ�С������ô��ڵĴ�С
	//��λͼѡ�뵽�ڴ���ʾ�豸��
	//ֻ��ѡ����λͼ���ڴ���ʾ�豸���еط���ͼ������ָ����λͼ��
	CBitmap *pOldBit=temCDC.SelectObject(&bm);
	temCDC.BitBlt(0, 0 ,rect.Width(),rect.Height(), pDC, 0, 0, SRCCOPY); 

	CString filepath("temporary.bmp");
	CFile file;
	if(GetFileAttributes(filepath)!=-1){
	   file.Remove(filepath);
	}
	USES_CONVERSION;      //��cstringת��Ϊconst wchar*
    WCHAR* pBuf = T2W((LPCTSTR)filepath);
    Bitmap bmp((HBITMAP)bm, NULL);//����bitmap
    CLSID Clsid;
	GetEncoderClsid(L"image/bmp", &Clsid);
    bmp.Save(pBuf, &Clsid, NULL);
	IplImage* img_src = 0;  
    char* pictureImage = filepath.GetBuffer(filepath.GetLength());  
    img_src = cvLoadImage(pictureImage, 1);  
	IplImage* img_dst=cvCreateImage(cvGetSize(img_src),IPL_DEPTH_8U,3);//����һ��sizeΪimage,��ͨ��8λ�Ĳ�ɫͼ
	cvSmooth(img_src,img_dst,CV_GAUSSIAN, 3, 3); 
	CvvImage img;
	img.CopyOf(img_dst); //����ͼƬ
	img.DrawToHDC(hDC,&rect); // ��ͼƬ���Ƶ���ʾ�ؼ���ָ��������   
	temCDC.DeleteDC();
    bm.DeleteObject();
	

/////////////////////////////////////////////
	/*slow
	CDC *pDC=GetDC();
	Graphics graphics(MemDC.GetSafeHdc());
    CRect rect;
    this->GetClientRect(&rect);
	//��ȡ��ĻDC
	HDC hDC=::GetDC(NULL);

	COLORREF *pCopy;
	pCopy=new COLORREF[rect.Width()*rect.Height()];
	int nRadius=2;
	for(int ny=0;ny<rect.Width();++ny)
      for (int nx=0;nx<rect.Height();++nx){  
		std::vector<COLOR_DATA> cdList;  
        cdList.reserve(200);  
        COLOR_DATA cd;  
        double dTotal=0;  
        for (int m=nx-nRadius; m<=nx+nRadius;++m){
          if (m<0 || m>=nWidth ) continue;  
            for (int n=ny-nRadius;n<=ny+nRadius;++n){  
                    if (n<0 || n>=nHeight ) continue;  
                    cd.dDistance=GAUSS_FUN(sqrt((double)((m-nx)*(m-nx)+(n-ny)*(n-ny))), sigma);  
                    dTotal+=cd.dDistance;
					cd.dwColor=::GetPixel(hDC, nx, ny); 
                    cdList.push_back(cd);  
			}  
		}
         if ( cdList.size()>0 )  
         {//���������������������������ص�ļ�Ȩƽ��ֵ  
	    	std::vector<COLOR_DATA>::const_iterator itor=cdList.begin();  
            double r=0, g=0, b=0;  
            for ( ; itor!=cdList.end(); ++itor )   
            {  
               double dRate=itor->dDistance/dTotal;//�������ĵ�ԽԶ��ȨֵԽС  
               r+=GetRValue(itor->dwColor)*dRate;  
               g+=GetGValue(itor->dwColor)*dRate;  
               b+=GetBValue(itor->dwColor)*dRate;  
            }  
            *(pCopy+nx*rect.Width()+ny)=RGB((int)r, (int)g, (int)b);  
         }  
	  }
    for(int y=0;y<rect.Width();++y)
      for (int x=0;x<rect.Height();++x)
	    pDC->SetPixel(x,y,*(pCopy+x*rect.Width()+y));
    
    delete[] pCopy;
	ReleaseDC(pDC);*/

	CopyToMemDC();
}

//��ֵ�˲�
void CGraphicView::OnMedian() 
{
	// TODO: Add your command handler code here
	CDC *pDC=GetDC(); 
	HDC hDC= pDC ->GetSafeHdc();
	Graphics graphics(pDC->GetSafeHdc());
    CRect rect;
    this->GetClientRect(&rect);
 
//������ʱͼƬ����ƽ����ͼ
	CDC temCDC;
	CBitmap bm;
	bm.CreateCompatibleBitmap(pDC,nWidth,nHeight);

	//���������Ļ��ʾ���ݵ��ڴ���ʾ�豸
	temCDC.CreateCompatibleDC(pDC);
	//���潨��һ������Ļ��ʾ���ݵ�λͼ������λͼ�Ĵ�С������ô��ڵĴ�С
	//��λͼѡ�뵽�ڴ���ʾ�豸��
	//ֻ��ѡ����λͼ���ڴ���ʾ�豸���еط���ͼ������ָ����λͼ��
	CBitmap *pOldBit=temCDC.SelectObject(&bm);
	temCDC.BitBlt(0, 0 ,rect.Width(),rect.Height(), pDC, 0, 0, SRCCOPY); 

	CString filepath("temporary.bmp");
	CFile file;
	if(GetFileAttributes(filepath)!=-1){
	   file.Remove(filepath);
	}
	USES_CONVERSION;      //��cstringת��Ϊconst wchar*
    WCHAR* pBuf = T2W((LPCTSTR)filepath);
    Bitmap bmp((HBITMAP)bm, NULL);//����bitmap
    CLSID Clsid;
	GetEncoderClsid(L"image/bmp", &Clsid);
    bmp.Save(pBuf, &Clsid, NULL);
	IplImage* img_src = 0;  
    char* pictureImage = filepath.GetBuffer(filepath.GetLength());  
    img_src = cvLoadImage(pictureImage, 1);  
	IplImage* img_dst=cvCreateImage(cvGetSize(img_src),IPL_DEPTH_8U,3);//����һ��sizeΪimage,��ͨ��8λ�Ĳ�ɫͼ
	cvSmooth(img_src,img_dst,CV_MEDIAN, 3, 3); 
	CvvImage img;
	img.CopyOf(img_dst); //����ͼƬ
	img.DrawToHDC(hDC,&rect); // ��ͼƬ���Ƶ���ʾ�ؼ���ָ��������   
	temCDC.DeleteDC();
    bm.DeleteObject();

	CopyToMemDC();
}

//˫���˲�
void CGraphicView::OnBilateral() 
{
	// TODO: Add your command handler code here
	CDC *pDC=GetDC(); 
	HDC hDC= pDC ->GetSafeHdc();
	Graphics graphics(pDC->GetSafeHdc());
    CRect rect;
    this->GetClientRect(&rect);
 
//������ʱͼƬ����ƽ����ͼ
	CDC temCDC;
	CBitmap bm;
	bm.CreateCompatibleBitmap(pDC,nWidth,nHeight);

	//���������Ļ��ʾ���ݵ��ڴ���ʾ�豸
	temCDC.CreateCompatibleDC(pDC);
	//���潨��һ������Ļ��ʾ���ݵ�λͼ������λͼ�Ĵ�С������ô��ڵĴ�С
	//��λͼѡ�뵽�ڴ���ʾ�豸��
	//ֻ��ѡ����λͼ���ڴ���ʾ�豸���еط���ͼ������ָ����λͼ��
	CBitmap *pOldBit=temCDC.SelectObject(&bm);
	temCDC.BitBlt(0, 0 ,rect.Width(),rect.Height(), pDC, 0, 0, SRCCOPY); 

	CString filepath("temporary.bmp");
	CFile file;
	if(GetFileAttributes(filepath)!=-1){
	   file.Remove(filepath);
	}
	USES_CONVERSION;      //��cstringת��Ϊconst wchar*
    WCHAR* pBuf = T2W((LPCTSTR)filepath);
    Bitmap bmp((HBITMAP)bm, NULL);//����bitmap
    CLSID Clsid;
	GetEncoderClsid(L"image/bmp", &Clsid);
    bmp.Save(pBuf, &Clsid, NULL);
	IplImage* img_src = 0;  
    char* pictureImage = filepath.GetBuffer(filepath.GetLength());  
    img_src = cvLoadImage(pictureImage, 1);  
	IplImage* img_dst=cvCreateImage(cvGetSize(img_src),IPL_DEPTH_8U,3);//����һ��sizeΪimage,��ͨ��8λ�Ĳ�ɫͼ
	cvSmooth(img_src,img_dst,CV_BILATERAL, 3, 3); 
	CvvImage img;
	img.CopyOf(img_dst); //����ͼƬ
	img.DrawToHDC(hDC,&rect); // ��ͼƬ���Ƶ���ʾ�ؼ���ָ��������   
	temCDC.DeleteDC();
    bm.DeleteObject();

	CopyToMemDC();
}

void CGraphicView::OnSimpleNoScale() 
{
	// TODO: Add your command handler code here
	CDC *pDC=GetDC(); 
	HDC hDC= pDC ->GetSafeHdc();
	Graphics graphics(pDC->GetSafeHdc());
    CRect rect;
    this->GetClientRect(&rect);
 
//������ʱͼƬ����ƽ����ͼ
	CDC temCDC;
	CBitmap bm;
	bm.CreateCompatibleBitmap(pDC,nWidth,nHeight);

	//���������Ļ��ʾ���ݵ��ڴ���ʾ�豸
	temCDC.CreateCompatibleDC(pDC);
	//���潨��һ������Ļ��ʾ���ݵ�λͼ������λͼ�Ĵ�С������ô��ڵĴ�С
	//��λͼѡ�뵽�ڴ���ʾ�豸��
	//ֻ��ѡ����λͼ���ڴ���ʾ�豸���еط���ͼ������ָ����λͼ��
	CBitmap *pOldBit=temCDC.SelectObject(&bm);
	temCDC.BitBlt(0, 0 ,rect.Width(),rect.Height(), pDC, 0, 0, SRCCOPY); 

	CString filepath("temporary.bmp");
	CFile file;
	if(GetFileAttributes(filepath)!=-1){
	   file.Remove(filepath);
	}
	USES_CONVERSION;      //��cstringת��Ϊconst wchar*
    WCHAR* pBuf = T2W((LPCTSTR)filepath);
    Bitmap bmp((HBITMAP)bm, NULL);//����bitmap
    CLSID Clsid;
	GetEncoderClsid(L"image/bmp", &Clsid);
    bmp.Save(pBuf, &Clsid, NULL);
	IplImage* img_src = 0;  
    char* pictureImage = filepath.GetBuffer(filepath.GetLength());  
    img_src = cvLoadImage(pictureImage, 1);  
	IplImage* img_dst=cvCreateImage(cvGetSize(img_src),IPL_DEPTH_8U,3);//����һ��sizeΪimage,��ͨ��8λ�Ĳ�ɫͼ
	cvSmooth(img_src,img_dst,CV_BLUR_NO_SCALE, 3, 3); 
	CvvImage img;
	img.CopyOf(img_dst); //����ͼƬ
	img.DrawToHDC(hDC,&rect); // ��ͼƬ���Ƶ���ʾ�ؼ���ָ��������   
	temCDC.DeleteDC();
    bm.DeleteObject();

	CopyToMemDC();
}

//��ɫ
void CGraphicView::OnNot() 
{
	// TODO: Add your command handler code here
	CDC *pDC=GetDC(); 
	HDC hDC= pDC ->GetSafeHdc();
	Graphics graphics(pDC->GetSafeHdc());
    CRect rect;
    this->GetClientRect(&rect);
 
//������ʱͼƬ���ڷ�ɫ��ͼ
	CDC temCDC;
	CBitmap bm;
	bm.CreateCompatibleBitmap(pDC,nWidth,nHeight);

	//���������Ļ��ʾ���ݵ��ڴ���ʾ�豸
	temCDC.CreateCompatibleDC(pDC);
	//���潨��һ������Ļ��ʾ���ݵ�λͼ������λͼ�Ĵ�С������ô��ڵĴ�С
	//��λͼѡ�뵽�ڴ���ʾ�豸��
	//ֻ��ѡ����λͼ���ڴ���ʾ�豸���еط���ͼ������ָ����λͼ��
	CBitmap *pOldBit=temCDC.SelectObject(&bm);
	temCDC.BitBlt(0, 0 ,rect.Width(),rect.Height(), pDC, 0, 0, SRCCOPY); 

	CString filepath("temporary.bmp");
	CFile file;
	if(GetFileAttributes(filepath)!=-1){
	   file.Remove(filepath);
	}
	USES_CONVERSION;      //��cstringת��Ϊconst wchar*
    WCHAR* pBuf = T2W((LPCTSTR)filepath);
    Bitmap bmp((HBITMAP)bm, NULL);//����bitmap
    CLSID Clsid;
	GetEncoderClsid(L"image/bmp", &Clsid);
    bmp.Save(pBuf, &Clsid, NULL);
	IplImage* img_src = 0;  
    char* pictureImage = filepath.GetBuffer(filepath.GetLength());  
    img_src = cvLoadImage(pictureImage, 1);  
	IplImage* img_dst=cvCreateImage(cvGetSize(img_src),IPL_DEPTH_8U,3);//����һ��sizeΪimage,��ͨ��8λ�Ĳ�ɫͼ
	cvNot(img_src,img_dst);
	CvvImage img;
	img.CopyOf(img_dst); //����ͼƬ
	img.DrawToHDC(hDC,&rect); // ��ͼƬ���Ƶ���ʾ�ؼ���ָ��������   
	temCDC.DeleteDC();
    bm.DeleteObject();

	CopyToMemDC();
}

//LAPLACEͼ����
void CGraphicView::OnLaplace() 
{
	// TODO: Add your command handler code here
	CDC *pDC=GetDC(); 
	HDC hDC= pDC ->GetSafeHdc();
	Graphics graphics(pDC->GetSafeHdc());
    CRect rect;
    this->GetClientRect(&rect);
 
//������ʱͼƬ�����񻯻�ͼ
	CDC temCDC;
	CBitmap bm;
	bm.CreateCompatibleBitmap(pDC,nWidth,nHeight);

	//���������Ļ��ʾ���ݵ��ڴ���ʾ�豸
	temCDC.CreateCompatibleDC(pDC);
	//���潨��һ������Ļ��ʾ���ݵ�λͼ������λͼ�Ĵ�С������ô��ڵĴ�С
	//��λͼѡ�뵽�ڴ���ʾ�豸��
	//ֻ��ѡ����λͼ���ڴ���ʾ�豸���еط���ͼ������ָ����λͼ��
	CBitmap *pOldBit=temCDC.SelectObject(&bm);
	temCDC.BitBlt(0, 0 ,rect.Width(),rect.Height(), pDC, 0, 0, SRCCOPY); 

	CString filepath("temporary.bmp");
	CFile file;
	if(GetFileAttributes(filepath)!=-1){
	   file.Remove(filepath);
	}
	USES_CONVERSION;      //��cstringת��Ϊconst wchar*
    WCHAR* pBuf = T2W((LPCTSTR)filepath);
    Bitmap bmp((HBITMAP)bm, NULL);//����bitmap
    CLSID Clsid;
	GetEncoderClsid(L"image/bmp", &Clsid);
    bmp.Save(pBuf, &Clsid, NULL);
    IplImage* img_src = 0;  
    char* pictureImage = filepath.GetBuffer(filepath.GetLength());  
    img_src = cvLoadImage(pictureImage, 1);  
	IplImage* img_dst=cvCreateImage(cvGetSize(img_src),IPL_DEPTH_32F,3);//����һ��sizeΪimage,��ͨ��8λ�Ĳ�ɫͼ

	cvLaplace(img_src,img_dst);
	
	CvvImage img;
	img.CopyOf(img_dst); //����ͼƬ
	img.DrawToHDC(hDC,&rect); // ��ͼƬ���Ƶ���ʾ�ؼ���ָ��������   
	temCDC.DeleteDC();
    bm.DeleteObject();

	CopyToMemDC();
}

void CGraphicView::OnSobel() 
{
	// TODO: Add your command handler code here
	CDC *pDC=GetDC(); 
	HDC hDC= pDC ->GetSafeHdc();
	Graphics graphics(pDC->GetSafeHdc());
    CRect rect;
    this->GetClientRect(&rect);
 
//������ʱͼƬ�����񻯻�ͼ
	CDC temCDC;
	CBitmap bm;
	bm.CreateCompatibleBitmap(pDC,nWidth,nHeight);

	//���������Ļ��ʾ���ݵ��ڴ���ʾ�豸
	temCDC.CreateCompatibleDC(pDC);
	//���潨��һ������Ļ��ʾ���ݵ�λͼ������λͼ�Ĵ�С������ô��ڵĴ�С
	//��λͼѡ�뵽�ڴ���ʾ�豸��
	//ֻ��ѡ����λͼ���ڴ���ʾ�豸���еط���ͼ������ָ����λͼ��
	CBitmap *pOldBit=temCDC.SelectObject(&bm);
	temCDC.BitBlt(0, 0 ,rect.Width(),rect.Height(), pDC, 0, 0, SRCCOPY); 

	CString filepath("temporary.bmp");
	CFile file;
	if(GetFileAttributes(filepath)!=-1){
	   file.Remove(filepath);
	}
	USES_CONVERSION;      //��cstringת��Ϊconst wchar*
    WCHAR* pBuf = T2W((LPCTSTR)filepath);
    Bitmap bmp((HBITMAP)bm, NULL);//����bitmap
    CLSID Clsid;
	GetEncoderClsid(L"image/bmp", &Clsid);
    bmp.Save(pBuf, &Clsid, NULL);
    IplImage* img_src = 0;  
    char* pictureImage = filepath.GetBuffer(filepath.GetLength());  
    img_src = cvLoadImage(pictureImage, 1);  
	IplImage* img_dst=cvCreateImage(cvGetSize(img_src),IPL_DEPTH_32F,3);//����һ��sizeΪimage,��ͨ��8λ�Ĳ�ɫͼ

	cvSobel(img_src,img_dst,1,0,3);
	
	CvvImage img;
	img.CopyOf(img_dst); //����ͼƬ
	img.DrawToHDC(hDC,&rect); // ��ͼƬ���Ƶ���ʾ�ؼ���ָ��������   
	temCDC.DeleteDC();
    bm.DeleteObject();

	CopyToMemDC();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGraphicView::imageDraw(){
	CDC* pDC = GetDC();
	Graphics graphics(MemDC.GetSafeHdc());
    CRect rect;
    this->GetClientRect(&rect);

	if(isAll){

	  CopyToMemDC();

	  ReleaseDC(pDC);
	  return;
	}
	NormalTrans();
	pDC->BitBlt(0, 0 ,nWidth,nHeight,&MemDC,0,0,SRCCOPY);
	//ƽ��
	graphics.TranslateTransform((float)m_TranslateLevel, (float)m_TranslateVertical); 
	//����
	graphics.ScaleTransform(m_rTrans,m_cTrans);
  	//Դ���ƶ�����ת����
	graphics.TranslateTransform(1.0f*(imagePointX+m_ZoomRow/2),1.0f*(imagePointY+m_ZoomColumn/2));
	//��ת
	if(!m_nRotateModel)graphics.RotateTransform(1.0f*m_nAngle); 
	else graphics.RotateTransform(1.0f*(1080-m_nAngle));
	//��ԭԴ��	
	graphics.TranslateTransform(1.0f*(0-(imagePointX+m_ZoomRow/2)), 1.0f*(0-(imagePointY+m_ZoomColumn/2)));

	if(isGray){
	  ColorMatrix colorMatrix=
	  {
		 0.299f,0.299f,0.299f,0,0,
		0.587f,0.587f,0.587f,0,0,
		0.114f,0.114f,0.114f,0,0,
		0,0,0,1,0,
		0,0,0,0,1
	  };

	  ImageAttributes attr;
	  attr.SetColorMatrix(&colorMatrix);
	
	  Rect destRect(imagePointX,imagePointY,m_pImg->GetWidth(),m_pImg->GetHeight());
	  graphics.DrawImage(m_pImg, destRect, 0,0,m_pImg->GetWidth(),m_pImg->GetHeight(),UnitPixel,&attr);
	}

	
	pDC->BitBlt(0, 0 ,nWidth,nHeight,&MemDC,0,0,SRCCOPY);

	ReleaseDC(pDC);
}

void CGraphicView::imageConfirmnation(){
	CGraphicDoc* pDoc = GetDocument();//�õ��ĵ�ָ��,ע��,�ĵ����������빤�����йص�!!��ͬ�ĳ���һ��.
	ASSERT_VALID(pDoc);
	if(!imageConfirm&&pDoc->flagOpen!=0){
		CConfirmDlg dlg;
		if(dlg.DoModal()==IDOK){
			imageConfirm=true;
			imageDraw();
		}
	}
}

void CGraphicView::OnDot() 
{
	// TODO: Add your command handler code here
	imageConfirmnation();

	m_nDrawType=1;
}

void CGraphicView::OnLine() 
{
	// TODO: Add your command handler code here
	imageConfirmnation();

	m_nDrawType=2;
}

void CGraphicView::OnRectangle() 
{
	// TODO: Add your command handler code here
	imageConfirmnation();

	m_nDrawType=3;
}


void CGraphicView::OnEllipse() 
{
	// TODO: Add your command handler code here
	imageConfirmnation();

	m_nDrawType=4;
}


void CGraphicView::OnSector() 
{
	// TODO: Add your command handler code here
	imageConfirmnation();

	m_nDrawType=5;
}


void CGraphicView::OnPolyline() 
{
	// TODO: Add your command handler code here
	imageConfirmnation();

	m_nDrawType=6;
}

void CGraphicView::OnSimplebrush() 
{
	// TODO: Add your command handler code here
	imageConfirmnation();

	m_nDrawType=7;
}

void CGraphicView::OnBitmapbrush() 
{
	// TODO: Add your command handler code here
	imageConfirmnation();

	m_nDrawType=8;
}

void CGraphicView::OnTransparentbrush() 
{
	// TODO: Add your command handler code here
	imageConfirmnation();
	
	m_nDrawType=9;
}

void CGraphicView::OnErase() 
{
	// TODO: Add your command handler code here
	imageConfirmnation();

	m_nDrawType=10;
}

void CGraphicView::OnFillarea() 
{
	// TODO: Add your command handler code here
	imageConfirmnation();

	m_nDrawType=11;
}

void CGraphicView::OnGun() 
{
	// TODO: Add your command handler code here
	imageConfirmnation();

	m_nDrawType=12;
}

void CGraphicView::OnText() 
{
	// TODO: Add your command handler code here
	CDC* pDC = GetDC();
	Graphics graphics1(MemDC.GetSafeHdc());
	Graphics graphics2(pDC->GetSafeHdc());
    CRect rect;
    this->GetClientRect(&rect);
    CTextDlg  dlg;
	if(dlg.DoModal()==IDOK)
		if(dlg.m_Textx>rect.Width()||dlg.m_Texty>rect.Height()){
			AfxMessageBox("Out of Range!");
		}else pDC->TextOut (dlg.m_Textx,dlg.m_Texty,dlg.m_Content);
	

/*	CFontDialog dlg; 
    if(IDOK==dlg.DoModal()) 
    { 
		CDC* pDC=GetDC();
        //if(m_font.m_hObject) 
            //m_font.DeleteObject(); 
        m_font.CreateFontIndirect(dlg.m_cf.lpLogFont); 
        m_strFontName=dlg.m_cf.lpLogFont->lfFaceName; 
        CFont *pOldFont=pDC->SelectObject(&m_font);
        pDC->TextOut(0,0,m_strFontName); 
        pDC->SelectObject(pOldFont); 
		m_font.DeleteObject();
		ReleaseDC(pDC);
    }*/
}

//////////////////////////////////////

void CGraphicView::OnDraw(CDC* pDC)
{
	
	CGraphicDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	CRect rect;
	this->GetClientRect(&rect);

	/*
	//��Ҫ!!!!��ʱ�ͷ�CBitmap (�������ͷ�CDC)
	if (MemBitmap.m_hObject != NULL){
	   MemDC.DeleteDC();
       MemBitmap.DeleteObject();
	}*/
    if (MemBitmap.m_hObject == NULL){
		nWidth=rect.Width();
		nHeight=rect.Height();
		MemBitmap.CreateCompatibleBitmap(pDC,nWidth,nHeight);

		//���������Ļ��ʾ���ݵ��ڴ���ʾ�豸
		MemDC.CreateCompatibleDC(pDC);
		//���潨��һ������Ļ��ʾ���ݵ�λͼ������λͼ�Ĵ�С������ô��ڵĴ�С
		//��λͼѡ�뵽�ڴ���ʾ�豸��
		//ֻ��ѡ����λͼ���ڴ���ʾ�豸���еط���ͼ������ָ����λͼ��
		CBitmap *pOldBit=MemDC.SelectObject(&MemBitmap);
        MemDC.FillSolidRect(0,0,rect.Width(),rect.Height(),RGB(255,255,255));
	}else{
		pDC->BitBlt(0, 0 ,nWidth,nHeight,&MemDC,0,0,SRCCOPY); 
		if(rect.Width()>nWidth&&rect.Height()>nHeight)CopyToMemDC();
		else if(rect.Width()>nWidth||rect.Height()>nHeight){
			MemDC.DeleteDC();
			MemBitmap.DeleteObject();
			nWidth=max(nWidth,rect.Width());
			nHeight=max(nHeight,rect.Height());
			MemBitmap.CreateCompatibleBitmap(pDC,nWidth,nHeight);
	
			//���������Ļ��ʾ���ݵ��ڴ���ʾ�豸
			MemDC.CreateCompatibleDC(pDC);
			//���潨��һ������Ļ��ʾ���ݵ�λͼ������λͼ�Ĵ�С������ô��ڵĴ�С
			//��λͼѡ�뵽�ڴ���ʾ�豸��
			//ֻ��ѡ����λͼ���ڴ���ʾ�豸���еط���ͼ������ָ����λͼ��
			CBitmap *pOldBit=MemDC.SelectObject(&MemBitmap);
			MemDC.BitBlt(0, 0 ,nWidth,nHeight, pDC, 0, 0, SRCCOPY); 

		}
	}
	pDC->TextOut (rect.Width()-260,rect.Height()-15,"Hello��Welcome to SimpleMFCGraphic��");
//    MemDC.MoveTo(0,0);
//    MemDC.LineTo(100,100);


//˫����ʵ�ּ���
/*C++ code*/
//	RECT rc;
//	GetClientRect(&rc);
//	Bitmap bmp(int(rc.right),int(rc.bottom));

//	Graphics bmpGraphics(&bmp);
//	bmpGraphics.SetSmoothingMode(SmoothingModeAntiAlias);

/*Drawing on DC*/
//	Graphics graphics(pDC->m_hDC);
/*Important! Create a CacheBitmap object for quick drawing*/
//	CachedBitmap cachedBmp(&bmp,&graphics);
//	graphics.DrawCachedBitmap(&cachedBmp,0,0);






    //GDI+ʹ�ó���
//	Graphics graphics(MemDC.GetSafeHdc());
//	Pen pen(Color(255, 0, 255));
//	graphics.DrawLine(&pen, 0, 0, 200, 100);

}

//OnPaint ��OnDraw�߶����  �ȵ��� OnPaint �ٵ��� OnDraw
void CGraphicView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	
///////////////////////////////////////////////////////////////////////////////
//����ͼƬ

	CGraphicDoc* pDoc = GetDocument();//�õ��ĵ�ָ��,ע��,�ĵ����������빤�����йص�!!��ͬ�ĳ���һ��.
	ASSERT_VALID(pDoc); 


	//�ֶ����
	CDC* pDC=GetDC();
    CRect rect;
    this->GetClientRect(&rect);
	pDC->FillSolidRect(&rect,RGB(255,255,255));
	OnDraw(&dc); //������OnDraw   VC6.0

	//�Ƿ��Ѵ�ĳ��BMP�ļ�
	if(pDoc->flagOpen==1)
	{	
		initial();
		Load((LPCTSTR)(pDoc->FilePath));
		//���������ʾDIB
		/*SetDIBitsToDevice(dc.m_hDC,  //DIB��������豸������
			imagePointX,               //�豸��������λͼ�����ʼ�߼�x��ַ
			imagePointY,               //�豸��������λͼ�����ʼ�߼�x��ַ 
			pDoc->bi.biWidth,  //DIB�Ŀ��
			pDoc->bi.biHeight, //DIB�ĸ߶�
			0,                 //DIB��ʼ��ȡ������������ݵ�xλ��
			0,                 //DIB��ʼ��ȡ������������ݵ�yλ��
			0,                 //DIB�����ص�ˮƽ�к�,����ӦlpBits�ڴ滺������һ������
			pDoc->bi.biHeight, //DIB����������Ӧ��������lpBits��ָ�ڴ滺�����е�����
			pDoc->lpbuf,       //�����������ݵ��ڴ滺������ָ��
			pDoc->pbi,        //ָ���ʼ���˵�BITMAPINFO���ݽṹ��ָ�룬������λͼ�Ĵ�С��ɫ������
			DIB_RGB_COLORS);   //ָ������ʾ����ɫ*/
      Graphics graphics(MemDC.GetSafeHdc());

	  m_ZoomRow=m_pImg->GetWidth();
	  m_ZoomColumn=m_pImg->GetHeight();

	  graphics.DrawImage(m_pImg, imagePointX, imagePointY);
	  pDC->BitBlt(0, 0 ,nWidth,nHeight,&MemDC,0,0,SRCCOPY);


	  pDoc->flagOpen=2;
	  //Invalidate(false);
	  return;
	}
	if(pDoc->flagOpen==2){
      Graphics graphics(pDC->GetSafeHdc());

	  pDC->BitBlt(0, 0 ,nWidth,nHeight,&MemDC,0,0,SRCCOPY);
	  graphics.DrawImage(m_pImg, imagePointX, imagePointY);
	}
	// Do not call CView::OnPaint() for painting messages
	//WRONG!!
	//Invalidate(true);

	// TODO: Add your message handler code here
	
	// Do not call CView::OnPaint() for painting messages

	
	ReleaseDC(pDC);
}

bool CGraphicView::Load( LPCTSTR pszFileName )
{
	ASSERT( pszFileName != NULL );

	CFile file;
	DWORD dwSize;

        // ���ļ�
	if ( !file.Open( pszFileName,
		CFile::modeRead | 
		CFile::shareDenyWrite ) )
	{
		TRACE( _T( "Load (file): Error opening file %s\n" ), pszFileName );
		return FALSE;
	};

        // �����ļ���С����HGLOBAL�ڴ�
	dwSize = (DWORD)file.GetLength();
	HGLOBAL hGlobal = GlobalAlloc( GMEM_MOVEABLE | GMEM_NODISCARD, dwSize );
	if ( !hGlobal )
	{
		TRACE( _T( "Load (file): Error allocating memory\n" ) );
		return FALSE;
	};

	char *pData = reinterpret_cast<char*>(GlobalLock(hGlobal));
	if ( !pData )
	{
		TRACE( _T( "Load (file): Error locking memory\n" ) );
		GlobalFree( hGlobal );
		return FALSE;
	};

        // ���ļ����ݶ���HGLOBAL�ڴ���
	TRY
	{
		file.Read( pData, dwSize );
	}
	CATCH( CFileException, e );                                          
	{
		TRACE( _T( "Load (file): An exception occured while reading the file %s\n"),
			pszFileName );
		GlobalFree( hGlobal );
		e->Delete();
		file.Close();
		return FALSE;
	}
	END_CATCH

	GlobalUnlock( hGlobal );
	file.Close();

        // ����hGlobal�ڴ��е����ݴ���stream
	IStream *pStream = NULL;
	if ( CreateStreamOnHGlobal( hGlobal, TRUE, &pStream ) != S_OK )
	{
		return FALSE;
	}

	m_pImg = Image::FromStream( pStream );
     ASSERT( m_pImg != NULL );

	// Ҫ������һ�䣬������GlobalAlloc������hGlobal�ڴ�û�б��ͷţ������ڴ�й¶������
	// CreateStreamOnHGlobal�ڶ�������������ΪTRUE�����Ե���pStream->Release()���Լ�����
	// ��hGlobal�ڴ棨����msdn��CreateStreamOnHGlobal��˵����
	pStream->Release();
    return true;
}


int CGraphicView::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
   UINT num= 0;
   UINT size= 0;

   ImageCodecInfo* pImageCodecInfo= NULL;

   GetImageEncodersSize(&num, &size);
   if(size== 0)
   {
    return -1;
   }
   pImageCodecInfo= (ImageCodecInfo*)(malloc(size));
   if(pImageCodecInfo== NULL)
   {
    return -1;
   }

   GetImageEncoders(num, size, pImageCodecInfo);

   for(UINT j=0; j< num; ++j)
   {
    if(wcscmp(pImageCodecInfo[j].MimeType, format)== 0)
	{
     *pClsid= pImageCodecInfo[j].Clsid;
     free(pImageCodecInfo);
     return j;
	}
   }

  free(pImageCodecInfo);
  return -1;
}



void CGraphicView::OnFileSave() 
{
	// TODO: Add your command handler code here
	CGraphicDoc* pDoc = GetDocument();//�õ��ĵ�ָ��,ע��,�ĵ����������빤�����йص�!!��ͬ�ĳ���һ��.
	ASSERT_VALID(pDoc);
	if(pDoc->flagOpen==0){
	  OnFileSaveAs();
	  return;
	}
	
	CDC* pDC=GetDC();
    CRect rect;
    this->GetClientRect(&rect);

	CopyToMemDC();

	CFile file;
	file.Remove(pDoc->FilePath);
	USES_CONVERSION;      //��cstringת��Ϊconst wchar*
    WCHAR* pBuf = T2W((LPCTSTR)pDoc->FilePath);
    Bitmap bm((HBITMAP)MemBitmap, NULL);//����bitmap
    CLSID Clsid; 
	//�ļ�����path�е��йغ�������޹ض�
	if(pDoc->FileExt==_T("bmp"))GetEncoderClsid(L"image/bmp", &Clsid); 
	else if(pDoc->FileExt==_T("jpg"))GetEncoderClsid(L"image/jpg", &Clsid); 
	else if(pDoc->FileExt==_T("jpeg"))GetEncoderClsid(L"image/jpeg", &Clsid); 
	else if(pDoc->FileExt==_T("png"))GetEncoderClsid(L"image/png", &Clsid); 
    bm.Save(pBuf, &Clsid, NULL);

	ReleaseDC(pDC);
}

void CGraphicView::OnFileSaveAs() 
{
	// TODO: Add your command handler code here
	LPCTSTR lpszFilter="BMP Files(*.bmp)|*.bmp|JPG Files(*.jpg)|*.bmp|JPEG Files(*.jpeg)|*.jpeg|PNG Files(*.png)|*.bmp|";
	CFileDialog  dlg(FALSE,lpszFilter,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,lpszFilter,NULL);
	
	CDC* pDC=GetDC();
    CRect rect;
    this->GetClientRect(&rect);

	if (dlg.DoModal()!=IDOK)return;

	CopyToMemDC();

	CString filepath=dlg.GetPathName();
	CString fileExt=dlg.GetFileExt();
	CFile file;
	USES_CONVERSION;      //��cstringת��Ϊconst wchar*
    WCHAR* pBuf = T2W((LPCTSTR)filepath);
    Bitmap bm((HBITMAP)MemBitmap, NULL);//����bitmap
    CLSID Clsid; 
	//�ļ�����path�е��йغ�������޹ض�
	if(fileExt==_T("bmp"))GetEncoderClsid(L"image/bmp", &Clsid); 
	else if(fileExt==_T("jpg"))GetEncoderClsid(L"image/jpg", &Clsid); 
	else if(fileExt==_T("jpeg"))GetEncoderClsid(L"image/jpeg", &Clsid); 
	else if(fileExt==_T("png"))GetEncoderClsid(L"image/png", &Clsid); 
    bm.Save(pBuf, &Clsid, NULL);

	ReleaseDC(pDC);
}

void CGraphicView::OnFileNew() 
{
	// TODO: Add your command handler code here
	CGraphicDoc* pDoc = GetDocument();//�õ��ĵ�ָ��,ע��,�ĵ����������빤�����йص�!!��ͬ�ĳ���һ��.
	ASSERT_VALID(pDoc);

	initial();
    CRect rect;
    this->GetClientRect(&rect);
	CDC* pDC=GetDC();

	pDoc->FilePath="";
	pDoc->FileExt="";
	pDoc->flagOpen=0;

	CopyToMemDC();

	MemDC.FillSolidRect(0,0,rect.Width(),rect.Height(),RGB(255,255,255));
	pDC->FillSolidRect(0,0,rect.Width(),rect.Height(),RGB(255,255,255));

	ReleaseDC(pDC);
}

void CGraphicView::CopyToMemDC(){
	CDC *pDC=GetDC();
    CRect rect;
    this->GetClientRect(&rect);

	MemDC.DeleteDC();
	MemBitmap.DeleteObject();
	nWidth=rect.Width();
	nHeight=rect.Height();
	MemBitmap.CreateCompatibleBitmap(pDC,nWidth,nHeight);

	//���������Ļ��ʾ���ݵ��ڴ���ʾ�豸
	MemDC.CreateCompatibleDC(pDC);
	//���潨��һ������Ļ��ʾ���ݵ�λͼ������λͼ�Ĵ�С������ô��ڵĴ�С
	//��λͼѡ�뵽�ڴ���ʾ�豸��
	//ֻ��ѡ����λͼ���ڴ���ʾ�豸���еط���ͼ������ָ����λͼ��
	CBitmap *pOldBit=MemDC.SelectObject(&MemBitmap);
	MemDC.BitBlt(0, 0 ,nWidth,nHeight, pDC, 0, 0, SRCCOPY); 

	ReleaseDC(pDC);
}

//��ǹ  ���������Ϊ�������
void CGraphicView::sprayGun(CPoint point){
   CDC *pDC=GetDC();
   srand(time(0));
   float r,c;
   for(int i=0;i<30*m_LineWidth;i++){
     c=(float)(rand()%36000)/100;
     r=(float)(rand()%(m_LineWidth*200))/100;
     CPoint randompoint;
     randompoint.x=(int)r*cos(c)+point.x;
     randompoint.y=(int)r*sin(c)+point.y;
	 MemDC.SetPixel(randompoint,m_Color);
	 pDC->SetPixel(randompoint,m_Color);
   }
   ReleaseDC(pDC);
}
