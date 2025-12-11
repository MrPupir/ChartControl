#include "pch.h"
#include "framework.h"
#include "ChartControl.h"
#include "ChartControlCtrl.h"
#include "ChartControlPropPage.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CChartControlCtrl, COleControl)

BEGIN_MESSAGE_MAP(CChartControlCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CChartControlCtrl, COleControl)
	DISP_FUNCTION_ID(CChartControlCtrl, "Clear", 1, OnClear, VT_EMPTY, VTS_NONE)
	DISP_PROPERTY_EX_ID(CChartControlCtrl, "Colors", 2, GetColors, SetColors, VT_VARIANT)
	DISP_FUNCTION_ID(CChartControlCtrl, "LoadData", 3, LoadData, VT_EMPTY, VTS_VARIANT)
	DISP_FUNCTION_ID(CChartControlCtrl, "ExportToExcel", 5, ExportToExcel, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CChartControlCtrl, "ExportToWord", 6, ExportToWord, VT_EMPTY, VTS_NONE)
	DISP_PROPERTY_EX_ID(CChartControlCtrl, "ChartTitle", 7, GetChartTitle, SetChartTitle, VT_BSTR)
	DISP_PROPERTY_EX_ID(CChartControlCtrl, "ShowLabels", 8, GetShowLabels, SetShowLabels, VT_BOOL)
	DISP_PROPERTY_EX_ID(CChartControlCtrl, "CoefA", 9, GetCoefA, SetCoefA, VT_R8)
	DISP_PROPERTY_EX_ID(CChartControlCtrl, "CoefB", 10, GetCoefB, SetCoefB, VT_R8)
	DISP_PROPERTY_EX_ID(CChartControlCtrl, "Enable3D", 11, GetEnable3D, SetEnable3D, VT_BOOL)
	DISP_PROPERTY_EX_ID(CChartControlCtrl, "ShowGrid", 12, GetShowGrid, SetShowGrid, VT_BOOL)
	DISP_PROPERTY_EX_ID(CChartControlCtrl, "GridColor", 13, GetGridColor, SetGridColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CChartControlCtrl, "AxisColor", 14, GetAxisColor, SetAxisColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CChartControlCtrl, "BackColor", 15, GetBackColor, SetBackColor, VT_COLOR)
	DISP_FUNCTION_ID(CChartControlCtrl, "Sort", 16, Sort, VT_EMPTY, VTS_BOOL)
END_DISPATCH_MAP()

BEGIN_EVENT_MAP(CChartControlCtrl, COleControl)
	EVENT_CUSTOM_ID("OnChartClick", 1, FireOnChartClick, VTS_NONE)
	EVENT_CUSTOM_ID("OnExportFinished", 2, FireOnExportFinished, VTS_NONE)
END_EVENT_MAP()

BEGIN_PROPPAGEIDS(CChartControlCtrl, 1)
	PROPPAGEID(CChartControlPropPage::guid)
END_PROPPAGEIDS(CChartControlCtrl)

IMPLEMENT_OLECREATE_EX(CChartControlCtrl, "MFCACTIVEXCONTRO.ChartControlCtrl.1",
	0xc9c6d8d6, 0x5d04, 0x4057, 0xb8, 0x51, 0x38, 0x70, 0xf0, 0x83, 0xe2, 0xb9)

	IMPLEMENT_OLETYPELIB(CChartControlCtrl, _tlid, _wVerMajor, _wVerMinor)

	const IID IID_DChartControl = { 0x87610429,0x08e2,0x4a0c,{0xb7,0x18,0x84,0xfe,0x61,0xc2,0xe4,0x45} };
const IID IID_DChartControlEvents = { 0x8e67e331,0xfe8c,0x4e5d,{0xa7,0x64,0x69,0x2d,0x38,0xa6,0x84,0x0d} };

static const DWORD _dwChartControlOleMisc =
OLEMISC_ACTIVATEWHENVISIBLE |
OLEMISC_SETCLIENTSITEFIRST |
OLEMISC_INSIDEOUT |
OLEMISC_CANTLINKINSIDE |
OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CChartControlCtrl, IDS_CHARTCONTROL, _dwChartControlOleMisc)

BOOL CChartControlCtrl::CChartControlCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_CHARTCONTROL,
			IDB_CHARTCONTROL,
			afxRegApartmentThreading,
			_dwChartControlOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}

CChartControlCtrl::CChartControlCtrl()
{
	InitializeIIDs(&IID_DChartControl, &IID_DChartControlEvents);
	VariantInit(&m_vData);
	VariantInit(&m_vColors);
	m_coefA = 0.6;
	m_coefB = 0.4;
	m_chartTitle = _T("Chart Data");
	m_bShowLabels = TRUE;
	m_bEnable3D = TRUE;
	m_bShowGrid = TRUE;
	m_clrGrid = RGB(200, 200, 200);
	m_clrAxis = RGB(60, 60, 60);
	m_clrBack = RGB(255, 255, 255);
	m_sortOrder = SORT_NONE;
}

CChartControlCtrl::~CChartControlCtrl()
{
	VariantClear(&m_vData);
	VariantClear(&m_vColors);
}

void CChartControlCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);
	PX_String(pPX, _T("ChartTitle"), m_chartTitle, _T("Chart Data"));
	PX_Bool(pPX, _T("ShowLabels"), m_bShowLabels, TRUE);
	PX_Double(pPX, _T("CoefA"), m_coefA, 0.6);
	PX_Double(pPX, _T("CoefB"), m_coefB, 0.4);
	PX_Bool(pPX, _T("Enable3D"), m_bEnable3D, TRUE);
	PX_Bool(pPX, _T("ShowGrid"), m_bShowGrid, TRUE);
	PX_Color(pPX, _T("GridColor"), m_clrGrid, RGB(200, 200, 200));
	PX_Color(pPX, _T("AxisColor"), m_clrAxis, RGB(60, 60, 60));
	PX_Color(pPX, _T("BackColor"), m_clrBack, RGB(255, 255, 255));
}

void CChartControlCtrl::OnResetState()
{
	COleControl::OnResetState();
}

void CChartControlCtrl::RebuildCacheFromVariant()
{
	m_labels.RemoveAll();
	m_values.RemoveAll();

	if (m_vData.vt == (VT_ARRAY | VT_VARIANT))
	{
		SAFEARRAY* psa = m_vData.parray;
		LONG lBound = 0, uBound = -1;
		SafeArrayGetLBound(psa, 1, &lBound);
		SafeArrayGetUBound(psa, 1, &uBound);
		for (LONG i = lBound; i <= uBound; ++i)
		{
			VARIANT elem;
			VariantInit(&elem);
			SafeArrayGetElement(psa, &i, &elem);
			if (elem.vt == (VT_ARRAY | VT_VARIANT))
			{
				SAFEARRAY* rowArray = elem.parray;
				LONG rl, ru;
				SafeArrayGetLBound(rowArray, 1, &rl);
				SafeArrayGetUBound(rowArray, 1, &ru);
				VARIANT vlabel; VariantInit(&vlabel);
				LONG idx0 = rl;
				SafeArrayGetElement(rowArray, &idx0, &vlabel);
				CString label = (vlabel.vt == VT_BSTR) ? CString(vlabel.bstrVal) : CString(L"");
				VariantClear(&vlabel);

				double p1 = 0.0, p2 = 0.0;
				VARIANT vp;
				VariantInit(&vp);
				if (ru - rl + 1 >= 2) {
					LONG idx1 = rl + 1;
					SafeArrayGetElement(rowArray, &idx1, &vp);
					if (vp.vt == VT_R8) p1 = vp.dblVal;
					else if (vp.vt == VT_I4) p1 = (double)vp.lVal;
					VariantClear(&vp);
				}
				VariantInit(&vp);
				if (ru - rl + 1 >= 3) {
					LONG idx2 = rl + 2;
					SafeArrayGetElement(rowArray, &idx2, &vp);
					if (vp.vt == VT_R8) p2 = vp.dblVal;
					else if (vp.vt == VT_I4) p2 = (double)vp.lVal;
					VariantClear(&vp);
				}

				double value = m_coefA * p1 + m_coefB * p2;
				m_labels.Add(label);
				m_values.Add(value);
			}
			VariantClear(&elem);
		}
	}

	m_sortOrder = SORT_NONE;
	UpdateSortIndices();
}

void CChartControlCtrl::UpdateSortIndices()
{
	int n = (int)m_values.GetSize();
	m_sortedIndices.SetSize(n);

	for (int i = 0; i < n; i++) {
		m_sortedIndices[i] = i;
	}

	if (m_sortOrder == SORT_NONE) return;

	for (int i = 0; i < n - 1; i++)
	{
		for (int j = 0; j < n - i - 1; j++)
		{
			int idx1 = m_sortedIndices[j];
			int idx2 = m_sortedIndices[j + 1];

			double v1 = m_values[idx1];
			double v2 = m_values[idx2];

			bool bSwap = (m_sortOrder == SORT_ASC) ? (v1 > v2) : (v1 < v2);

			if (bSwap)
			{
				int temp = m_sortedIndices[j];
				m_sortedIndices[j] = m_sortedIndices[j + 1];
				m_sortedIndices[j + 1] = temp;
			}
		}
	}
}

COLORREF CChartControlCtrl::GetColorForIndex(int idx)
{
	static COLORREF defaults[] = { RGB(0x1F,0x77,0xB4), RGB(0xFF,0x7F,0x0E), RGB(0x2C,0xA0,0x2C) };

	if ((m_vColors.vt & VT_ARRAY) == 0)
		return defaults[idx % 3];

	SAFEARRAY* psa = m_vColors.parray;
	if (!psa) return defaults[idx % 3];

	LONG lb, ub;
	SafeArrayGetLBound(psa, 1, &lb);
	SafeArrayGetUBound(psa, 1, &ub);
	LONG count = ub - lb + 1;

	if (count <= 0)
		return defaults[idx % 3];

	LONG pos = lb + (idx % count);
	VARTYPE vtType = m_vColors.vt & VT_TYPEMASK;

	if (vtType == VT_I4 || vtType == VT_UI4 || vtType == VT_INT || vtType == VT_UINT)
	{
		long colorVal = 0;
		SafeArrayGetElement(psa, &pos, &colorVal);
		return (COLORREF)colorVal;
	}

	if (vtType == VT_VARIANT)
	{
		VARIANT v;
		VariantInit(&v);
		SafeArrayGetElement(psa, &pos, &v);

		COLORREF result = defaults[idx % 3];

		if (v.vt == VT_I4 || v.vt == VT_UI4 || v.vt == VT_INT || v.vt == VT_UINT)
			result = (COLORREF)v.ulVal;
		else if (v.vt == VT_R4)
			result = (COLORREF)((long)v.fltVal);
		else if (v.vt == VT_R8)
			result = (COLORREF)((long)v.dblVal);

		VariantClear(&v);
		return result;
	}

	return defaults[idx % 3];
}

void CChartControlCtrl::OnClear()
{
	VariantClear(&m_vData);
	VariantInit(&m_vData);
	m_labels.RemoveAll();
	m_values.RemoveAll();
	m_sortedIndices.RemoveAll();
	m_sortOrder = SORT_NONE;
	Invalidate();
}

VARIANT CChartControlCtrl::GetColors() { return m_vColors; }
void CChartControlCtrl::SetColors(const VARIANT& varColors) {
	VariantClear(&m_vColors);
	VariantInit(&m_vColors);
	VariantCopy(&m_vColors, const_cast<VARIANT*>(&varColors));
	SetModifiedFlag();
	Invalidate();
}

void CChartControlCtrl::LoadData(const VARIANT& varRows) {
	VariantClear(&m_vData);
	VariantInit(&m_vData);
	VariantCopy(&m_vData, const_cast<VARIANT*>(&varRows));
	RebuildCacheFromVariant();
	SetModifiedFlag();
	Invalidate();
}

BSTR CChartControlCtrl::GetChartTitle() { return m_chartTitle.AllocSysString(); }
void CChartControlCtrl::SetChartTitle(LPCTSTR lpszNewValue) {
	m_chartTitle = lpszNewValue;
	SetModifiedFlag();
	Invalidate();
}

BOOL CChartControlCtrl::GetShowLabels() { return m_bShowLabels; }
void CChartControlCtrl::SetShowLabels(BOOL bNewValue) {
	m_bShowLabels = bNewValue;
	SetModifiedFlag();
	Invalidate();
}

double CChartControlCtrl::GetCoefA() { return m_coefA; }
void CChartControlCtrl::SetCoefA(double newValue) {
	m_coefA = newValue;
	RebuildCacheFromVariant();
	SetModifiedFlag();
	Invalidate();
}

double CChartControlCtrl::GetCoefB() { return m_coefB; }
void CChartControlCtrl::SetCoefB(double newValue) {
	m_coefB = newValue;
	RebuildCacheFromVariant();
	SetModifiedFlag();
	Invalidate();
}

BOOL CChartControlCtrl::GetEnable3D() { return m_bEnable3D; }
void CChartControlCtrl::SetEnable3D(BOOL bNewValue) {
	m_bEnable3D = bNewValue;
	SetModifiedFlag();
	Invalidate();
}

BOOL CChartControlCtrl::GetShowGrid() { return m_bShowGrid; }
void CChartControlCtrl::SetShowGrid(BOOL bNewValue) {
	m_bShowGrid = bNewValue;
	SetModifiedFlag();
	Invalidate();
}

OLE_COLOR CChartControlCtrl::GetGridColor() { return m_clrGrid; }
void CChartControlCtrl::SetGridColor(OLE_COLOR nNewValue) {
	m_clrGrid = nNewValue;
	SetModifiedFlag();
	Invalidate();
}

OLE_COLOR CChartControlCtrl::GetAxisColor() { return m_clrAxis; }
void CChartControlCtrl::SetAxisColor(OLE_COLOR nNewValue) {
	m_clrAxis = nNewValue;
	SetModifiedFlag();
	Invalidate();
}

OLE_COLOR CChartControlCtrl::GetBackColor() { return m_clrBack; }
void CChartControlCtrl::SetBackColor(OLE_COLOR nNewValue) {
	m_clrBack = nNewValue;
	SetModifiedFlag();
	Invalidate();
}

void CChartControlCtrl::Sort(BOOL asc)
{
	if (asc)
	{
		m_sortOrder = SORT_ASC;
	}
	else
	{
		if (m_sortOrder == SORT_DESC)
		{
			m_sortOrder = SORT_NONE;
		}
		else
		{
			m_sortOrder = SORT_DESC;
		}
	}

	UpdateSortIndices();
	Invalidate();
}

void CChartControlCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	FireOnChartClick();
	COleControl::OnLButtonDown(nFlags, point);
}

void CChartControlCtrl::OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	COleControl::OnDraw(pdc, rcBounds, rcInvalid);

	CDC memDC;
	memDC.CreateCompatibleDC(pdc);
	CBitmap memBitmap;
	memBitmap.CreateCompatibleBitmap(pdc, rcBounds.Width(), rcBounds.Height());
	CBitmap* pOldBitmap = memDC.SelectObject(&memBitmap);

	CRect rc = rcBounds;

	CBrush bgBrush(TranslateColor(m_clrBack));
	memDC.FillRect(rc, &bgBrush);

	CFont titleFont;
	titleFont.CreatePointFont(140, L"Roboto Semibold");

	CFont axisFont;
	axisFont.CreatePointFont(80, L"Roboto");

	CFont* pOldFont = memDC.SelectObject(&titleFont);
	CRect titleRect = rc;
	titleRect.bottom = titleRect.top + 40;
	memDC.SetBkMode(TRANSPARENT);
	memDC.SetTextColor(RGB(50, 50, 50));
	memDC.DrawText(m_chartTitle, &titleRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	memDC.SelectObject(&axisFont);
	int oldGraphicsMode = SetGraphicsMode(memDC.m_hDC, GM_ADVANCED);

	int n = (int)m_values.GetSize();
	if (m_sortedIndices.GetSize() != n) UpdateSortIndices();

	CRect chartRect = rc;
	chartRect.top += 50;
	chartRect.left += 50;
	chartRect.right -= 20;

	int depth = m_bEnable3D ? 15 : 0;
	int totalWidth = chartRect.Width() - depth;
	int singleBarAreaWidth = (n > 0) ? (totalWidth / n) : 0;
	int barWidth = (int)(singleBarAreaWidth * 0.7);

	bool bRotateAll = false;
	if (singleBarAreaWidth < 25)
	{
		bRotateAll = true;
	}
	else
	{
		for (int i = 0; i < n; i++)
		{
			int rawIdx = m_sortedIndices[i];
			CSize sz = memDC.GetTextExtent(m_labels[rawIdx]);
			if (sz.cx > singleBarAreaWidth)
			{
				bRotateAll = true;
				break;
			}
		}
	}

	int maxLabelHeight = 0;
	if (bRotateAll)
	{
		for (int i = 0; i < n; i++)
		{
			int rawIdx = m_sortedIndices[i];
			CSize sz = memDC.GetTextExtent(m_labels[rawIdx]);
			if (sz.cx > maxLabelHeight) maxLabelHeight = sz.cx;
		}
	}
	else
	{
		for (int i = 0; i < n; i++)
		{
			int rawIdx = m_sortedIndices[i];
			CSize sz = memDC.GetTextExtent(m_labels[rawIdx]);
			if (sz.cy > maxLabelHeight) maxLabelHeight = sz.cy;
		}
	}

	int minBottom = 20;
	if (maxLabelHeight < minBottom) maxLabelHeight = minBottom;
	int limitH = rc.Height() / 3;
	if (maxLabelHeight > limitH) maxLabelHeight = limitH;

	chartRect.bottom = rc.bottom - maxLabelHeight - 10;

	double maxVal = 0;
	for (int i = 0; i < n; i++) {
		int rawIdx = m_sortedIndices[i];
		if (m_values[rawIdx] > maxVal) maxVal = m_values[rawIdx];
	}
	if (maxVal <= 0) maxVal = 10.0;

	double step = maxVal / 5.0;
	if (step <= 0) step = 1.0;

	CPen axisPen(PS_SOLID, 2, TranslateColor(m_clrAxis));
	CPen gridPen(PS_DOT, 1, TranslateColor(m_clrGrid));
	CPen* pOldPen = memDC.SelectObject(&axisPen);

	memDC.MoveTo(chartRect.left, chartRect.top);
	memDC.LineTo(chartRect.left, chartRect.bottom);
	memDC.LineTo(chartRect.right, chartRect.bottom);

	memDC.SetTextColor(RGB(0, 0, 0));

	for (int i = 0; i <= 5; i++)
	{
		double val = step * i;
		int y = chartRect.bottom - (int)((val / maxVal) * chartRect.Height());

		if (y < chartRect.top) y = chartRect.top;

		if (i > 0 && m_bShowGrid)
		{
			memDC.SelectObject(&gridPen);
			memDC.MoveTo(chartRect.left, y);
			memDC.LineTo(chartRect.right, y);
			memDC.SelectObject(&axisPen);
		}

		memDC.MoveTo(chartRect.left - 5, y);
		memDC.LineTo(chartRect.left, y);

		CString strVal;
		strVal.Format(L"%.1f", val);
		memDC.SetTextAlign(TA_RIGHT | TA_TOP);
		memDC.TextOutW(chartRect.left - 8, y - 6, strVal);
	}

	if (n > 0)
	{
		int spacing = singleBarAreaWidth - barWidth;
		int x = chartRect.left + spacing / 2;

		for (int i = 0; i < n; i++)
		{
			int rawIdx = m_sortedIndices[i];
			double v = m_values[rawIdx];
			int h = (int)((v / maxVal) * chartRect.Height());
			if (h < 1) h = 1;

			CRect rFront(x, chartRect.bottom - h, x + barWidth, chartRect.bottom);
			CPoint ptCenter = rFront.CenterPoint();

			COLORREF clr = GetColorForIndex(rawIdx);

			if (m_bEnable3D)
			{
				BYTE rVal = GetRValue(clr);
				BYTE gVal = GetGValue(clr);
				BYTE bVal = GetBValue(clr);
				COLORREF clrTop = RGB(min(255, rVal + 40), min(255, gVal + 40), min(255, bVal + 40));
				COLORREF clrSide = RGB(max(0, rVal - 40), max(0, gVal - 40), max(0, bVal - 40));

				CPoint ptsSide[4] = {
					CPoint(rFront.right, rFront.bottom),
					CPoint(rFront.right + depth, rFront.bottom - depth),
					CPoint(rFront.right + depth, rFront.top - depth),
					CPoint(rFront.right, rFront.top)
				};
				CBrush brSide(clrSide);
				CBrush* pOldBr = memDC.SelectObject(&brSide);
				memDC.Polygon(ptsSide, 4);

				CPoint ptsTop[4] = {
					CPoint(rFront.left, rFront.top),
					CPoint(rFront.left + depth, rFront.top - depth),
					CPoint(rFront.right + depth, rFront.top - depth),
					CPoint(rFront.right, rFront.top)
				};
				CBrush brTop(clrTop);
				memDC.SelectObject(&brTop);
				memDC.Polygon(ptsTop, 4);
				memDC.SelectObject(pOldBr);
			}

			CBrush brFront(clr);
			memDC.SelectObject(&brFront);
			memDC.Rectangle(rFront);

			CString strValue;
			strValue.Format(L"%.2f", v);
			strValue.TrimRight(L"0");
			strValue.TrimRight(L".");

			CSize szVal = memDC.GetTextExtent(strValue);

			bool bFitInsideHorz = (szVal.cx < barWidth - 2) && (h > szVal.cy + 4);
			bool bFitInsideVert = (szVal.cx < h - 4) && (barWidth > szVal.cy + 2);

			if (bFitInsideHorz)
			{
				memDC.SetTextColor(RGB(255, 255, 255));
				memDC.SetTextAlign(TA_CENTER | TA_TOP);
				memDC.TextOutW(ptCenter.x, ptCenter.y - szVal.cy / 2, strValue);
			}
			else if (bFitInsideVert)
			{
				memDC.SetTextColor(RGB(255, 255, 255));

				XFORM xForm;
				xForm.eM11 = 0.0f;
				xForm.eM12 = -1.0f;
				xForm.eM21 = 1.0f;
				xForm.eM22 = 0.0f;
				xForm.eDx = (float)ptCenter.x + (float)szVal.cy / 2.0f;
				xForm.eDy = (float)ptCenter.y;

				SetWorldTransform(memDC.m_hDC, &xForm);

				memDC.SetTextAlign(TA_CENTER | TA_BOTTOM);
				memDC.TextOutW(0, 0, strValue);

				XFORM xIdentity = { 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f };
				SetWorldTransform(memDC.m_hDC, &xIdentity);
			}
			else
			{
				memDC.SetTextColor(RGB(0, 0, 0));
				int topOffset = m_bEnable3D ? depth : 0;

				if (szVal.cx < singleBarAreaWidth)
				{
					memDC.SetTextAlign(TA_CENTER | TA_BOTTOM);
					memDC.TextOutW(ptCenter.x + (topOffset / 2), rFront.top - topOffset - 2, strValue);
				}
				else
				{
					XFORM xForm;
					xForm.eM11 = 0.0f;
					xForm.eM12 = -1.0f;
					xForm.eM21 = 1.0f;
					xForm.eM22 = 0.0f;
					xForm.eDx = (float)ptCenter.x + (float)(topOffset / 2) + (float)szVal.cy / 2.0f;
					xForm.eDy = (float)rFront.top - (float)topOffset - 2.0f;

					SetWorldTransform(memDC.m_hDC, &xForm);

					memDC.SetTextAlign(TA_LEFT | TA_BOTTOM);
					memDC.TextOutW(0, 0, strValue);

					XFORM xIdentity = { 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f };
					SetWorldTransform(memDC.m_hDC, &xIdentity);
				}
			}

			if (m_bShowLabels)
			{
				memDC.SetTextColor(RGB(0, 0, 0));
				CString label = m_labels[rawIdx];
				CSize szL = memDC.GetTextExtent(label);

				if (bRotateAll)
				{
					XFORM xForm;
					xForm.eM11 = 0.0f;
					xForm.eM12 = -1.0f;
					xForm.eM21 = 1.0f;
					xForm.eM22 = 0.0f;
					xForm.eDx = (float)x + (float)barWidth / 2.0f + (float)szL.cy / 3.0f;
					xForm.eDy = (float)chartRect.bottom + 5.0f;

					SetWorldTransform(memDC.m_hDC, &xForm);

					memDC.SetTextAlign(TA_RIGHT | TA_BOTTOM);
					memDC.TextOutW(0, 0, label);

					XFORM xIdentity = { 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f };
					SetWorldTransform(memDC.m_hDC, &xIdentity);
				}
				else
				{
					memDC.SetTextAlign(TA_CENTER | TA_TOP);
					memDC.TextOutW(x + barWidth / 2, chartRect.bottom + 5, label);
				}
			}

			x += singleBarAreaWidth;
		}
	}

	SetGraphicsMode(memDC.m_hDC, oldGraphicsMode);

	memDC.SelectObject(pOldPen);
	memDC.SelectObject(pOldFont);

	pdc->BitBlt(rcBounds.left, rcBounds.top, rcBounds.Width(), rcBounds.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pOldBitmap);
}

void CChartControlCtrl::ExportToExcel()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_values.GetSize() == 0) return;

	Excel::_ApplicationPtr pXL;
	if (FAILED(pXL.CreateInstance(__uuidof(Excel::Application))))
	{
		MessageBox(L"Excel not found", L"Error", MB_ICONERROR);
		return;
	}

	try
	{
		pXL->Visible[0] = VARIANT_TRUE;

		Excel::_WorkbookPtr pBook = pXL->Workbooks->Add((long)Excel::xlWorkbook);
		Excel::_WorksheetPtr pSheet = pBook->Worksheets->Add();
		pSheet->Name = _T("Report");

		long nRows = (long)m_values.GetSize();
		if (m_sortedIndices.GetSize() != nRows) UpdateSortIndices();

		Excel::RangePtr headerRange = pSheet->Range["A1"]["B1"];
		pSheet->Cells->Item[1][1] = _T("Category");
		pSheet->Cells->Item[1][2] = _T("Value");

		headerRange->Font->Bold = true;
		headerRange->Font->Name = _T("Times New Roman");
		headerRange->Font->Size = 11;
		headerRange->Font->Color = (long)RGB(255, 255, 255);
		headerRange->Interior->Color = (long)RGB(47, 117, 181);
		headerRange->HorizontalAlignment = Excel::xlCenter;
		headerRange->VerticalAlignment = Excel::xlCenter;
		headerRange->RowHeight = 25;

		for (long i = 0; i < nRows; i++)
		{
			int rawIdx = m_sortedIndices[i];
			long row = i + 2;
			pSheet->Cells->Item[row][1] = _variant_t(m_labels[rawIdx]);
			pSheet->Cells->Item[row][2] = _variant_t(m_values[rawIdx]);

			if (i % 2 != 0)
			{
				Excel::RangePtr cellLeft = pSheet->Cells->Item[row][1];
				Excel::RangePtr cellRight = pSheet->Cells->Item[row][2];
				Excel::RangePtr rowRange = pSheet->GetRange((IDispatch*)cellLeft, (IDispatch*)cellRight);
				rowRange->Interior->Color = (long)RGB(242, 242, 242);
			}
		}

		Excel::RangePtr pCellTopLeft = pSheet->Cells->Item[1][1];
		Excel::RangePtr pCellBottomRight = pSheet->Cells->Item[nRows + 1][2];
		Excel::RangePtr dataRange = pSheet->GetRange((IDispatch*)pCellTopLeft, (IDispatch*)pCellBottomRight);

		dataRange->Borders->LineStyle = (long)Excel::xlContinuous;
		dataRange->Borders->Weight = (long)Excel::xlThin;
		dataRange->Borders->Color = (long)RGB(200, 200, 200);

		dataRange->Font->Name = _T("Times New Roman");
		dataRange->VerticalAlignment = Excel::xlCenter;

		pSheet->Columns->AutoFit();
		Excel::RangePtr colB = pSheet->Columns->Item[2];
		colB->ColumnWidth = 15;

		dataRange->Select();
		Sleep(100);

		Excel::XlChartType chartType = m_bEnable3D ? Excel::xl3DColumn : Excel::xlColumnClustered;

		Excel::ShapePtr pShape = pSheet->Shapes->AddChart(chartType,
			_variant_t(250.0), _variant_t(20.0), _variant_t(500.0), _variant_t(300.0));

		for (int i = 0; i < 5; ++i)
		{
			Sleep(50);
			MSG msg;
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		pShape->Select();

		Excel::_ChartPtr pChart = pXL->ActiveChart;

		if (pChart != NULL)
		{
			try
			{
				CString rangeStr;
				rangeStr.Format(L"A1:B%d", nRows + 1);
				Excel::RangePtr pRange = pSheet->GetRange(_variant_t(rangeStr));

				Excel::ListObjectsPtr pListObjects = pSheet->ListObjects;

				if (pListObjects->Count > 0)
				{
					Excel::ListObjectPtr pList = pListObjects->Item[1];
					pList->Resize(pRange);
				}
				else
				{
					pListObjects->Add(
						Excel::xlSrcRange,
						(LPDISPATCH)pRange,
						vtMissing,
						Excel::xlYes
					);
				}
			}
			catch (...) {}

			try
			{
				pChart->PutHasLegend(0, VARIANT_FALSE);
			}
			catch (...) {}

			try
			{
				pChart->PutHasTitle(0, VARIANT_TRUE);
				pChart->ChartTitle->Text = _bstr_t(m_chartTitle);
			}
			catch (...) {}

			long axisTypes[] = { 1, 2, 3 };

			for (int k = 0; k < 3; k++)
			{
				try
				{
					Excel::AxisPtr pAxis = pChart->Axes(_variant_t(axisTypes[k]), Excel::xlPrimary);

					if (pAxis)
					{
						if (axisTypes[k] == 3)
						{
							pAxis->Delete();
						}
						else
						{
							pAxis->PutHasTitle(VARIANT_FALSE);
						}
					}
				}
				catch (...)
				{
					try
					{
						Excel::AxisPtr pAxis = pChart->Axes(_variant_t(axisTypes[k]), Excel::xlPrimary);
						if (pAxis->HasTitle)
						{
							pAxis->AxisTitle->Delete();
						}
					}
					catch (...) {}
				}
			}

			try
			{
				Excel::SeriesCollectionPtr pSeriesColl = pChart->SeriesCollection();
				if (pSeriesColl->Count > 0)
				{
					Excel::SeriesPtr pSeries = pSeriesColl->Item(1);
					for (long i = 0; i < nRows; i++)
					{
						try
						{
							Excel::PointPtr pPoint = pSeries->Points(i + 1);
							int rawIdx = m_sortedIndices[i];
							COLORREF clr = GetColorForIndex(rawIdx);
							pPoint->Interior->Color = (long)clr;
						}
						catch (...) {}
					}
				}
			}
			catch (...) {}

			try
			{
				Excel::SeriesCollectionPtr pSeriesColl = pChart->SeriesCollection();
				if (pSeriesColl->Count > 0)
				{
					Excel::SeriesPtr pSeries = pSeriesColl->Item(1);

					pSeries->HasDataLabels = VARIANT_TRUE;
					Excel::DataLabelsPtr pLabels = pSeries->DataLabels();

					pLabels->Position = Excel::xlLabelPositionCenter;

					pLabels->NumberFormat = _bstr_t(L"0.##");

					pLabels->Font->Color = (long)RGB(255, 255, 255);
					pLabels->Font->Bold = VARIANT_TRUE;
					pLabels->Font->Size = 10;
				}
			}
			catch (...) {}
		}

		FireOnExportFinished();
	}
	catch (_com_error& e)
	{
		MessageBox(e.ErrorMessage(), L"Excel Error", MB_ICONERROR);
	}
}

void CChartControlCtrl::ExportToWord()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_values.GetSize() == 0) return;

	Word::_ApplicationPtr pWord;
	if (FAILED(pWord.CreateInstance(__uuidof(Word::Application))))
	{
		MessageBox(L"Word not found", L"Error", MB_ICONERROR);
		return;
	}

	try
	{
		pWord->Visible = VARIANT_TRUE;

		Word::_DocumentPtr doc = pWord->Documents->Add();

		doc->Paragraphs->Last->Range->Font->Name = _T("Times New Roman");
		doc->Paragraphs->Last->Range->Font->Size = 20;
		doc->Paragraphs->Last->Range->Font->Bold = 1;
		doc->Paragraphs->Alignment = Word::wdAlignParagraphCenter;
		doc->Content->InsertAfter(_bstr_t(m_chartTitle + L"\n"));

		doc->Paragraphs->Last->Range->Font->Size = 12;
		doc->Paragraphs->Last->Range->Font->Bold = 0;
		doc->Paragraphs->Alignment = Word::wdAlignParagraphCenter;

		long nRows = (long)m_values.GetSize();
		if (m_sortedIndices.GetSize() != nRows) UpdateSortIndices();

		Word::TablePtr pTable = doc->Tables->Add(doc->Paragraphs->Last->Range, nRows + 1, 2);
		pTable->Borders->InsideLineStyle = Word::wdLineStyleSingle;
		pTable->Borders->OutsideLineStyle = Word::wdLineStyleSingle;

		pTable->Cell(1, 1)->Range->Text = "Category";
		pTable->Cell(1, 2)->Range->Text = "Value";

		pTable->Rows->Item(1)->Range->Font->Bold = 1;
		pTable->Rows->Item(1)->Range->Font->Color = Word::wdColorWhite;
		pTable->Rows->Item(1)->Shading->BackgroundPatternColor = Word::wdColorDarkBlue;

		for (long i = 0; i < nRows; i++)
		{
			int rawIdx = m_sortedIndices[i];
			long r = i + 2;
			pTable->Cell(r, 1)->Range->Text = _bstr_t(m_labels[rawIdx]);
			pTable->Cell(r, 2)->Range->Text = _bstr_t(m_values[rawIdx]);
		}

		pTable->Columns->Item(2)->Select();
		pWord->Selection->ParagraphFormat->Alignment = Word::wdAlignParagraphCenter;
		pTable->AutoFitBehavior(Word::wdAutoFitWindow);

		doc->Paragraphs->Add();
		doc->Paragraphs->Last->Range->Select();
		pWord->Selection->Collapse();

		long lChartType = m_bEnable3D ? 54 : 51;

		Word::InlineShapePtr pShape = doc->InlineShapes->AddChart((Office::XlChartType)lChartType);

		if (pShape)
		{
			Word::ChartPtr pChart = pShape->Chart;
			Word::ChartDataPtr pChartData = pChart->ChartData;

			try
			{
				pChartData->Activate();

				IDispatchPtr dispWb = pChartData->Workbook;
				Excel::_WorkbookPtr pWb = dispWb;

				if (pWb)
				{
					Excel::_WorksheetPtr pSheet = pWb->Worksheets->Item[1];
					pSheet->Cells->ClearContents();

					pSheet->Cells->Item[1][1] = _T("Category");
					pSheet->Cells->Item[1][2] = _T("Value");

					for (long i = 0; i < nRows; i++)
					{
						int rawIdx = m_sortedIndices[i];
						long row = i + 2;
						pSheet->Cells->Item[row][1] = _variant_t(m_labels[rawIdx]);
						pSheet->Cells->Item[row][2] = _variant_t(m_values[rawIdx]);
					}

					CString rangeStr;
					rangeStr.Format(L"A1:B%d", nRows + 1);
					Excel::RangePtr pRange = pSheet->GetRange(_variant_t(rangeStr));

					Excel::ListObjectsPtr pListObjects = pSheet->ListObjects;

					if (pListObjects->Count > 0)
					{
						Excel::ListObjectPtr pList = pListObjects->Item[1];
						pList->Resize(pRange);
					}
					else
					{
						pListObjects->Add(
							Excel::xlSrcRange,
							(LPDISPATCH)pRange,
							vtMissing,
							Excel::xlYes
						);
					}

					_bstr_t bstrSheetName = pSheet->Name;
					_bstr_t bstrAddress = pRange->Address[true][true][Excel::xlA1][false];
					CString strSourceData;
					strSourceData.Format(L"=\'%s\'!%s", (LPCTSTR)bstrSheetName, (LPCTSTR)bstrAddress);

					try
					{
						pChart->SetSourceData(_bstr_t(strSourceData), _variant_t((long)Excel::xlColumns));
					}
					catch (...)
					{
					}

					pWb->Application->Quit();
				}
			}
			catch (...) {}

			try { pChart->PutHasLegend(0, VARIANT_FALSE); }
			catch (...) {}

			try {
				pChart->PutHasTitle(0, VARIANT_TRUE);
				pChart->ChartTitle->Text = _bstr_t(m_chartTitle);
			}
			catch (...) {}

			long axisTypes[] = { 1, 2, 3 };
			for (int k = 0; k < 3; k++)
			{
				try {
					Excel::AxisPtr pAxis = pChart->Axes(_variant_t(axisTypes[k]), Word::xlPrimary);
					if (pAxis) {
						if (axisTypes[k] == 3) pAxis->Delete();
						else pAxis->PutHasTitle(VARIANT_FALSE);
					}
				}
				catch (...) {}
			}

			/*try
			{
				Excel::SeriesCollectionPtr pSeriesColl = pChart->FullSeriesCollection();

				if (pSeriesColl->Count > 0)
				{
					Excel::SeriesPtr pSeries = pSeriesColl->Item(1);

					for (long i = 0; i < nRows; i++)
					{
						try
						{
							Excel::PointPtr pPoint = pSeries->Points(i + 1);

							int rawIdx = m_sortedIndices[i];

							COLORREF clr = GetColorForIndex(rawIdx);
							pPoint->MarkerForegroundColor = (long)clr;
						}
						catch (_com_error& e)
						{
							MessageBox(e.ErrorMessage(), L"COM Error", MB_ICONERROR);
						}
					}
				}
			}
			catch (_com_error& e)
			{
				MessageBox(e.ErrorMessage(), L"COM Error 2222", MB_ICONERROR);
			}

			try
			{
				Excel::SeriesCollectionPtr pSeriesColl = pChart->SeriesCollection();
				if (pSeriesColl->Count > 0)
				{
					Excel::SeriesPtr pSeries = pSeriesColl->Item(1);

					pSeries->HasDataLabels = VARIANT_TRUE;
					Excel::DataLabelsPtr pLabels = pSeries->DataLabels();

					pLabels->Position = Excel::xlLabelPositionCenter;
					pLabels->NumberFormat = _bstr_t(L"0.##");
					pLabels->Font->Color = (long)RGB(255, 255, 255);
					pLabels->Font->Bold = VARIANT_TRUE;
					pLabels->Font->Size = 10;
				}
			}
			catch (...) {}*/
		}

		FireOnExportFinished();
	}
	catch (_com_error& e)
	{
		MessageBox(e.ErrorMessage(), L"Word Error", MB_ICONERROR);
	}
}

void CChartControlCtrl::AboutBox()
{
	CDialogEx dlgAbout(IDD_ABOUTBOX_CHARTCONTROL);
	dlgAbout.DoModal();
}