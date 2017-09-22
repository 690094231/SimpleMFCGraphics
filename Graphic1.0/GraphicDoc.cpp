// GraphicDoc.cpp : implementation of the CGraphicDoc class
//

#include "stdafx.h"
#include "Graphic.h"

#include "GraphicDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraphicDoc

IMPLEMENT_DYNCREATE(CGraphicDoc, CDocument)

BEGIN_MESSAGE_MAP(CGraphicDoc, CDocument)
	//{{AFX_MSG_MAP(CGraphicDoc)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraphicDoc construction/destruction

CGraphicDoc::CGraphicDoc()
{
	// TODO: add one-time construction code here
	flagOpen=0;
}

CGraphicDoc::~CGraphicDoc()
{
}

BOOL CGraphicDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CGraphicDoc serialization

void CGraphicDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CGraphicDoc diagnostics

#ifdef _DEBUG
void CGraphicDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGraphicDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGraphicDoc commands

void CGraphicDoc::OnFileOpen() 
{
	// TODO: Add your command handler code here
	LPCTSTR lpszFilter="BMP Files(*.bmp)|*.bmp|JPG Files(*.jpg)|*.bmp|JPEG Files(*.jpeg)|*.jpeg|PNG Files(*.png)|*.bmp|";
	CFileDialog dlg(TRUE,lpszFilter,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,lpszFilter,NULL);
	CString filepath;
	CFile file;

	//���ļ��Ի���
	if(dlg.DoModal()==IDOK){
		filepath=dlg.GetPathName();
		FilePath=filepath;
		FileExt=dlg.GetFileExt();
		if(file.Open(filepath,CFile::modeRead|CFile::shareDenyNone,NULL)==0)
		{
			//��ȡ�ļ�ʧ��
			AfxMessageBox("�޷����ļ���",MB_OK,0);
			return;
		}
		//��ȡ�ļ�ͷ
		file.Read(&bf,sizeof(bf));
	/*	//�ж��Ƿ���BMP�ļ�
		if(bf.bfType!=0x4d42)//'BM'
		{
			AfxMessageBox("��BMP�ļ���",MB_OK,0);
			return;
		}*/
		//�ж��ļ��Ƿ���
		if(file.GetLength()!=bf.bfSize)
		{
			AfxMessageBox("�ļ����𻵣����飡",MB_OK,0);
			return;
		}
		//���ļ���Ϣͷ
		file.Read(&bi,sizeof(bi));
		//�����ɫ����Ŀ
		numquad=0;
		if(bi.biBitCount<24)
		{
			numquad=1<<bi.biBitCount;
		}
		//Ϊͼ����Ϣpbi����ռ�
		pbi=(BITMAPINFO*)HeapAlloc(GetProcessHeap(),0,sizeof(BITMAPINFOHEADER)+numquad*sizeof(RGBQUAD));
		memcpy(pbi,&bi,sizeof(bi));
		quad=(RGBQUAD*)((BYTE*)pbi+sizeof(BITMAPINFOHEADER));
		//��ȡ��ɫ��
		if(numquad!=0)
		{
			file.Read(quad,sizeof(RGBQUAD)*numquad);
		}
		//Ϊͼ����������ռ�
		bi.biSizeImage=bf.bfSize-bf.bfOffBits;
		lpbuf=(BYTE*)HeapAlloc(GetProcessHeap(),0,bi.biSizeImage);
		//��ȡͼ������
		file.Read(lpbuf,bi.biSizeImage);
		//ͼ���ȡ��ϣ��ر��ļ������ñ�־
		file.Close();
		flagOpen=1;
		this->UpdateAllViews(NULL);
	}
}


//Backup
/*
void CGraphicDoc::OnFileOpen() 
{
	// TODO: Add your command handler code here
	LPCTSTR lpszFilter="BMP Files(*.bmp)|*.bmp|�κ��ļ�|*.*||";
	CFileDialog dlg(TRUE,lpszFilter,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,lpszFilter,NULL);
	CString filepath;
	CFile file;

	//���ļ��Ի���
	if(dlg.DoModal()==IDOK){
		filepath=dlg.GetPathName();
		FilePath=filepath;
		if(file.Open(filepath,CFile::modeRead|CFile::shareDenyNone,NULL)==0)
		{
			//��ȡ�ļ�ʧ��
			AfxMessageBox("�޷����ļ���",MB_OK,0);
			return;
		}
		//��ȡ�ļ�ͷ
		file.Read(&bf,sizeof(bf));
		//�ж��Ƿ���BMP�ļ�
		if(bf.bfType!=0x4d42)//'BM'
		{
			AfxMessageBox("��BMP�ļ���",MB_OK,0);
			return;
		}
		//�ж��ļ��Ƿ���
		if(file.GetLength()!=bf.bfSize)
		{
			AfxMessageBox("�ļ����𻵣����飡",MB_OK,0);
			return;
		}
		//���ļ���Ϣͷ
		file.Read(&bi,sizeof(bi));
		//�����ɫ����Ŀ
		numquad=0;
		if(bi.biBitCount<24)
		{
			numquad=1<<bi.biBitCount;
		}
		//Ϊͼ����Ϣpbi����ռ�
		pbi=(BITMAPINFO*)HeapAlloc(GetProcessHeap(),0,sizeof(BITMAPINFOHEADER)+numquad*sizeof(RGBQUAD));
		memcpy(pbi,&bi,sizeof(bi));
		quad=(RGBQUAD*)((BYTE*)pbi+sizeof(BITMAPINFOHEADER));
		//��ȡ��ɫ��
		if(numquad!=0)
		{
			file.Read(quad,sizeof(RGBQUAD)*numquad);
		}
		//Ϊͼ����������ռ�
		bi.biSizeImage=bf.bfSize-bf.bfOffBits;
		lpbuf=(BYTE*)HeapAlloc(GetProcessHeap(),0,bi.biSizeImage);
		//��ȡͼ������
		file.Read(lpbuf,bi.biSizeImage);
		//ͼ���ȡ��ϣ��ر��ļ������ñ�־
		file.Close();
		flagOpen=1;
		this->UpdateAllViews(NULL);
	}
}


void CGraphicDoc::OnFileSaveAs() 
{
	// TODO: Add your command handler code here
	LPCTSTR lpszFilter="BMP Files(*.bmp)|*.bmp|JPG Files(*.jpg)|*.bmp|JPEG Files(*.jpeg)|*.jpeg|PNG Files(*.png)|*.bmp|";
	CFileDialog  dlg(FALSE,lpszFilter,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,lpszFilter,NULL);
	if (dlg.DoModal()!=IDOK)return;
	CFile file;
	CFileException fe;
	if (!file.Open(dlg.GetPathName(),CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive, &fe))
	{
		// ʧ��
		ReportSaveLoadException(dlg.GetPathName(), &fe, TRUE, AFX_IDP_INVALID_FILENAME);
		return;
	}
	file.SeekToBegin();
	file.Write(&bf,sizeof(bf));
	file.Write(&bi,sizeof(bi));
	if(numquad!=0)
	{
		file.Write(quad,sizeof(RGBQUAD)*numquad);
	}
	file.WriteHuge(lpbuf,bi.biSizeImage);
	file.Close(); 
}
*/