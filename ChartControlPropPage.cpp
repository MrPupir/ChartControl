// ChartControlPropPage.cpp: реализация класса страницы свойств CChartControlPropPage.

#include "pch.h"
#include "framework.h"
#include "ChartControl.h"
#include "ChartControlPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CChartControlPropPage, COlePropertyPage)

// Схема сообщений

BEGIN_MESSAGE_MAP(CChartControlPropPage, COlePropertyPage)
END_MESSAGE_MAP()

// Инициализировать фабрика класса и guid

IMPLEMENT_OLECREATE_EX(CChartControlPropPage, "MFCACTIVEXCONT.ChartControlPropPage.1",
	0x2ec70dee,0x73d9,0x484f,0xb0,0x91,0x38,0xed,0xc3,0x18,0x51,0x04)

// CChartControlPropPage::CChartControlPropPageFactory::UpdateRegistry -
// Добавляет или удаляет записи системного реестра для CChartControlPropPage

BOOL CChartControlPropPage::CChartControlPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_CHARTCONTROL_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, nullptr);
}

// CChartControlPropPage::CChartControlPropPage — конструктор

CChartControlPropPage::CChartControlPropPage() :
	COlePropertyPage(IDD, IDS_CHARTCONTROL_PPG_CAPTION)
{
}

// CChartControlPropPage::DoDataExchange — перемещение данных между страницей и свойствами

void CChartControlPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}

// Обработчики сообщений CChartControlPropPage
