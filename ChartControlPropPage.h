#pragma once

// ChartControlPropPage.h: объявление класса страницы свойств CChartControlPropPage.


// CChartControlPropPage: реализацию см. в ChartControlPropPage.cpp.

class CChartControlPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CChartControlPropPage)
	DECLARE_OLECREATE_EX(CChartControlPropPage)

// Конструктор
public:
	CChartControlPropPage();

// Данные диалогового окна
	enum { IDD = IDD_PROPPAGE_CHARTCONTROL };

// Реализация
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Схемы сообщений
protected:
	DECLARE_MESSAGE_MAP()
};

