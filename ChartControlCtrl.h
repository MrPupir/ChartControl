#pragma once

class CChartControlCtrl : public COleControl
{
	DECLARE_DYNCREATE(CChartControlCtrl)

public:
	CChartControlCtrl();
	virtual ~CChartControlCtrl();

	VARIANT m_vData;
	VARIANT m_vColors;
	CArray<CString, CString> m_labels;
	CArray<double, double> m_values;

	double m_coefA;
	double m_coefB;
	CString m_chartTitle;
	BOOL m_bShowLabels;

	BOOL m_bEnable3D;
	BOOL m_bShowGrid;
	OLE_COLOR m_clrGrid;
	OLE_COLOR m_clrAxis;
	OLE_COLOR m_clrBack;

	DECLARE_OLECREATE_EX(CChartControlCtrl)
	DECLARE_OLETYPELIB(CChartControlCtrl)
	DECLARE_PROPPAGEIDS(CChartControlCtrl)
	DECLARE_OLECTLTYPE(CChartControlCtrl)

	afx_msg void OnClear();
	afx_msg VARIANT GetColors();
	afx_msg void SetColors(const VARIANT& varColors);
	afx_msg void LoadData(const VARIANT& varRows);
	afx_msg void ExportToExcel();
	afx_msg void ExportToWord();
	afx_msg BSTR GetChartTitle();
	afx_msg void SetChartTitle(LPCTSTR lpszNewValue);
	afx_msg BOOL GetShowLabels();
	afx_msg void SetShowLabels(BOOL bNewValue);
	afx_msg double GetCoefA();
	afx_msg void SetCoefA(double newValue);
	afx_msg double GetCoefB();
	afx_msg void SetCoefB(double newValue);

	afx_msg BOOL GetEnable3D();
	afx_msg void SetEnable3D(BOOL bNewValue);
	afx_msg BOOL GetShowGrid();
	afx_msg void SetShowGrid(BOOL bNewValue);
	afx_msg OLE_COLOR GetGridColor();
	afx_msg void SetGridColor(OLE_COLOR nNewValue);
	afx_msg OLE_COLOR GetAxisColor();
	afx_msg void SetAxisColor(OLE_COLOR nNewValue);
	afx_msg OLE_COLOR GetBackColor();
	afx_msg void SetBackColor(OLE_COLOR nNewValue);
	afx_msg void Sort(BOOL asc);

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	void FireOnChartClick() { FireEvent(1, EVENT_PARAM(VTS_NONE)); }
	void FireOnExportFinished() { FireEvent(2, EVENT_PARAM(VTS_NONE)); }

public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	afx_msg void AboutBox();
	DECLARE_EVENT_MAP()

	void RebuildCacheFromVariant();
	COLORREF GetColorForIndex(int idx);

public:
	enum {
	};
};