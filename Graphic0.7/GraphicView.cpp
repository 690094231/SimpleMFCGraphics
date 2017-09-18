// GraphicView.cpp : implementation of the CGraphicView class
//

#include "StdAfx.h"
#include "Graphic.h"

#include "GraphicDoc.h"
#include "GraphicView.h"
#include "SettingDlg.h"
#include "RotateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;

#endif

	using namespace Gdiplus;
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
	ON_COMMAND(ID_IMPORTSAMPLE, OnImportsample)
	ON_COMMAND(ID_GRAY, OnGray)
	ON_COMMAND(ID_ALL, OnAll)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_ZOOM, OnZoom)
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
	m_nAngle = 0;
	m_nRotateModel = 0;
	imagePointX = 100;
	imagePointY = 100;
}

CGraphicView::~CGraphicView()
{
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

void CGraphicView::OnDraw(CDC* pDC)
{
	CGraphicDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here

//˫����ʵ�ּ���
/*C++ code*/
//	RECT rc;
//	GetClientRect(&rc);
//	Bitmap bmp(int(rc.right),int(rc.bottom));

//	Graphics bmpGraphics(&bmp);
//	bmpGraphics.SetSmoothingMode(SmoothingModeAntiAlias);

/*Drawing on bitmap*/
//	SolidBrush bkBrush(Color(0,0,0));
//	bmpGraphics.FillRectangle(&bkBrush,0,0,rc.right,rc.bottom);

/*Drawing on DC*/
//	Graphics graphics(pDC->m_hDC);
/*Important! Create a CacheBitmap object for quick drawing*/
//	CachedBitmap cachedBmp(&bmp,&graphics);
//	graphics.DrawCachedBitmap(&cachedBmp,0,0);






    //GDI+ʹ�ó���
//	Graphics graphics(pDC->m_hDC);
//	Pen pen(Color(255, 0, 255));
//	graphics.DrawLine(&pen, 0, 0, 200, 100);
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
	CClientDC dc(this);
	CBrush brush(m_Color);
	CGraphicDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	m_ptOrigin=point;
	m_bDraw=true;

	
	//��ȡ��ĻDC
	HDC hDC=::GetDC(NULL);
	//��ȡ��ǰ���RGBֵ
	COLORREF clr=::GetPixel(hDC, point.x, point.y); 
	Gdiplus::Graphics graphics(hDC);

	//�ı����ָ����״
	HCURSOR hCur;
	switch(m_nDrawType){
	case 1://��
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
			dc.SelectObject(brush);
			ExtFloodFill(dc,point.x,point.y,clr,FLOODFILLSURFACE);
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
	CView::OnLButtonDown(nFlags, point);
}

void CGraphicView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//����������豸����
	CClientDC dc(this);
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
	    pOldBrush= dc.SelectObject(pBrush);
	}else{
		brush.CreateSolidBrush(m_Color);
		pOldBrush=dc.SelectObject(&brush);
	}

	
    //�����߿����1��ʵ��/����/����
	LOGBRUSH logBrush;  
	logBrush.lbStyle = BS_SOLID;  
	logBrush.lbColor = m_Color;  
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
	dc.SelectObject(&pen1);
	if(m_LineStyle==1)dc.SelectObject(&pen2);		//ѡ�񻭱�
	else if(m_LineStyle==2)dc.SelectObject(&pen3);
	else if(m_LineStyle==3)dc.SelectObject(&pen4);
	else if(m_LineStyle!=0)dc.SelectObject(&pen5);		//ѡ�񻭱�
	

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
		//Simplebrush
		CBrush brush(m_Color);
	    //���û�ˢ��������ҷ�������γɵľ������� 
        dc.FillRect(CRect(m_ptOrigin,point),&brush);
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
		dc.FillRect(CRect(m_ptOrigin,point),&brush);
	}
	if(m_nDrawType==9){
		//Transparentbrush
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

    //�����߿����1��ʵ��/����/����
	LOGBRUSH logBrush;  
	logBrush.lbStyle = BS_SOLID;  
	logBrush.lbColor = m_Color;  
	CPen pen1(m_LineStyle,m_LineWidth,m_Color);//��������
	CPen pen2(PS_DASH | PS_GEOMETRIC | PS_ENDCAP_ROUND,m_LineWidth,&logBrush);//��������
	CPen pen3(PS_DOT | PS_GEOMETRIC | PS_ENDCAP_ROUND,m_LineWidth,&logBrush);//��������
	dc.SelectObject(&pen1);
	if(m_LineStyle==1)dc.SelectObject(&pen2);		//ѡ�񻭱�
	else  if(m_LineStyle!=0)dc.SelectObject(&pen3);		//ѡ�񻭱�

	if(m_bDraw==true)
		switch(m_nDrawType){
			case 5://SECTOR
				dc.MoveTo(m_ptOrigin);                  
				dc.LineTo(point);
				break;
			case 6://Polyline
				dc.MoveTo(m_ptOrigin);                  
				dc.LineTo(point);
				//�޸��߶ε����        
				m_ptOrigin=point;
				break;
			case 10://erase
				CBrush brush(RGB(255,255,255));
				CPoint pt1;
				CPoint pt2;
				pt1.x=point.x-10;
				pt1.y=point.y-10;
				pt2.x=point.x+10;
				pt2.y=point.y+10;
				dc.FillRect(CRect(pt1,pt2),&brush);
				break;
		}
	//�ָ��豸����

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


BOOL CGraphicView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	// nothing
	 //return CView::OnEraseBkgnd(pDC);
	return true;
}

void CGraphicView::OnImportsample() 
{
	// TODO: Add your command handler code here
	
	CDC *pDC=GetDC();
	Graphics graphics(pDC->GetSafeHdc());
	Image image(L"res\\lena.bmp");
	CRect rect;
	this->GetClientRect(&rect);
	pDC->FillSolidRect(&rect,RGB(255,255,255));
	graphics.DrawImage(&image, imagePointX, imagePointX);


	//initial
	m_nRotateModel=0;
	m_nAngle=0;
	imagePointX=100;
	imagePointY=100;




	ReleaseDC(pDC);
}


void CGraphicView::OnAll() 
{
	// TODO: Add your command handler code here
	CDC* pDC = GetDC();
	Graphics graph(pDC->GetSafeHdc());
	Image image(L"res\\lena.bmp");
	CRect rect;
	GetClientRect(rect);  // ��ô��ڻ������Ĵ�С
	graph.DrawImage(&image, 0, 0, rect.Width(),rect.Height()); // ����ͼ��



	ReleaseDC(pDC);
}

void CGraphicView::OnZoom() 
{
	// TODO: Add your command handler code here
	CDC* pDC = GetDC();
	Graphics graph(pDC->GetSafeHdc());
	Image image(L"res\\lena.bmp");
	graph.DrawImage(&image, imagePointX,imagePointY,image.GetWidth(),image.GetHeight()); // ����ͼ��




	ReleaseDC(pDC);
}

void CGraphicView::OnRotate() 
{
	// TODO: Add your command handler code here
	CRotateDlg dlg;
	dlg.m_nRotateModel=m_nRotateModel;
	dlg.m_nAngle=m_nAngle;
	if(IDOK==dlg.DoModal()){
		m_nRotateModel+=dlg.m_nRotateModel;
		m_nAngle+=dlg.m_nAngle;
	}

	CDC *pDC=GetDC();
	Graphics graphics(pDC->GetSafeHdc());
	Image image(L"res\\lena.bmp");
	graphics.TranslateTransform(1.0f*(imagePointX+100),1.0f*(imagePointY+100)); //Դ���ƶ�����ת����
	
	
	//��ת
	if(!m_nRotateModel)graphics.RotateTransform(1.0f*m_nAngle); 
	else graphics.RotateTransform(1.0f*(1080-m_nAngle)); 

	graphics.TranslateTransform(1.0f*(0-(imagePointX+100)), 1.0f*(0-(imagePointY+100)));//��ԭԴ��

    CRect rect;
	this->GetClientRect(&rect);
	pDC->FillSolidRect(&rect,RGB(255,255,255));
    //��ĳ�������ʾͼ��
    graphics.DrawImage(&image, imagePointX, imagePointY);




	ReleaseDC(pDC);
}


void CGraphicView::OnGray() 
{
	// TODO: Add your command handler code here
	CDC *pDC=GetDC();
	Graphics graphics(pDC->GetSafeHdc());
	Image image(L"res\\lena.bmp");
	CRect rect;
	this->GetClientRect(&rect);
	pDC->FillSolidRect(&rect,RGB(255,255,255));

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

	Rect destRect(imagePointX,imagePointX,image.GetWidth(),image.GetHeight());
	graphics.DrawImage(&image, destRect, 0,0,image.GetWidth(),image.GetHeight(),UnitPixel,&attr);
	


	ReleaseDC(pDC);
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

void CGraphicView::OnErase() 
{
	// TODO: Add your command handler code here
	m_nDrawType=10;
}

void CGraphicView::OnFillarea() 
{
	// TODO: Add your command handler code here
	m_nDrawType=11;
}




/////////////////////////////////Addtion Functions
void CGraphicView::FillAreaqaq(CPoint point,COLORREF originColor)
{
   
   // BFS is so slow
   /*
   int x,y,l,r,ll,rr,tx,ty;
   //��������豸����
   CClientDC dc(this);
   HDC hDC=::GetDC(NULL);
   RECT rc;
   GetClientRect(&rc);
   dc.SetPixel(point,m_Color);
   l=r=0;
   Pts[l]=point;
   while(l<=r){
     ll=l%Pnum;
	 x=Pts[ll].x;
	 y=Pts[ll].y;
     for(int i=0;i<4;i++){
	   tx=x+fx[i];
	   ty=y+fy[i];
	   CPoint xp(tx,ty);
	   if(tx>=0&&ty>=0&&tx<=rc.bottom&&ty<=rc.right&&originColor==::GetPixel(hDC,tx,ty)){
		    dc.SetPixel(xp,m_Color);
			++r;rr=r%Pnum;
			Pts[rr]=xp;
	   }
	 }
	 l++;
   }
   */
}

void CGraphicView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	
///////////////////////////////////////////////////////////////////////////////
//����ͼƬ

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
	  Invalidate(false);
	  return;
	}
	// Do not call CView::OnPaint() for painting messages
	//WRONG!!
	//Invalidate(true);



	OnDraw(&dc); //������OnDraw   VC6.0
	// TODO: Add your message handler code here
	
	// Do not call CView::OnPaint() for painting messages

}
