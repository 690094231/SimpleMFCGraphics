// GraphicDoc.h : interface of the CGraphicDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPHICDOC_H__B957CE6F_4E8C_4F51_818A_CF27BE0625F1__INCLUDED_)
#define AFX_GRAPHICDOC_H__B957CE6F_4E8C_4F51_818A_CF27BE0625F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGraphicDoc : public CDocument
{
protected: // create from serialization only
	CGraphicDoc();
	DECLARE_DYNCREATE(CGraphicDoc)

// Attributes
public:
	//���ڴ�λͼ�ļ���
	int numquad; //��ɫ����
	int flagOpen;//��־�Ƿ��bmp
	BITMAPINFO* pbi;  //��¼ͼ��ϸ��
	BYTE* lpbuf;  //ͼ������
	RGBQUAD* quad;  //��ɫ��
	BITMAPFILEHEADER bf;//�ļ�ͷ
	BITMAPINFOHEADER bi;  //��Ϣͷ 
	CString FilePath;
	CString FileExt;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphicDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGraphicDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGraphicDoc)
	afx_msg void OnFileOpen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHICDOC_H__B957CE6F_4E8C_4F51_818A_CF27BE0625F1__INCLUDED_)
