/*********************************************************\
 *  File: TabsAndMenuWindow.cpp                          *
 *
 *  Copyright (C) 2002-2013 The PixelLight Team (http://www.pixellight.org/)
 *
 *  This file is part of PixelLight.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 *  and associated documentation files (the "Software"), to deal in the Software without
 *  restriction, including without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all copies or
 *  substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 *  BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 *  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
\*********************************************************/


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLGui/Gui/Gui.h>
#include <PLGui/Widgets/Controls/MenuBar.h>
#include <PLGui/Widgets/Controls/Bitmap.h>
//#include <PLGui/Widgets/Containers/TabContainer.h>
#include <PLGui/Widgets/Containers/Border.h>
#include <PLGui/Widgets/Containers/Splitter.h>
#include <PLGui/Widgets/Containers/ScrollWidget.h>
#include <PLGui/Themes/Theme.h>
#include "TabsAndMenu/TabsAndMenuWindow.h"

// [DEBUG]
#include <PLCore/System/System.h>
#include <PLCore/System/Console.h>


//[-------------------------------------------------------]
//[ Compiler settings                                     ]
//[-------------------------------------------------------]
PL_WARNING_PUSH
PL_WARNING_DISABLE(4355) // "'this' : used in base member initializer list"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLGui;


//[-------------------------------------------------------]
//[ Class implementation                                  ]
//[-------------------------------------------------------]
pl_implement_class(TabsAndMenuWindow)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
TabsAndMenuWindow::TabsAndMenuWindow(Widget *pParent) : Window(pParent),
	EventHandlerCreateWindow(&TabsAndMenuWindow::OnTabContainerCreateWindow, this),
	EventHandlerMenuItemClicked(&TabsAndMenuWindow::OnMenuItemClicked, this),
	m_pMenuBar(nullptr),
	m_pTabContainer(nullptr),
	m_nNextWindow(0),
	m_cMenu(*GetGui())
{
	// Set layout
	GetContentWidget()->SetLayout("LayoutBox", "Orientation=Vertical");
	GetContentWidget()->GetLayoutHints().SetSpacing(0);

	// Set title
	SetTitle("PLGui Test");

	// Destroy window on close
	AddModifier("PLGui::ModClose", "CloseWindow=1");

	// Create main menu
	m_cMenu.SetText("&Menu");
	m_cMenu.SetIcon(GetTheme()->GetDefaultIcon());

	// Add items
	m_cMenu.AddItem("&Alpha", GetTheme()->GetDefaultIcon());
	m_cMenu.AddItem("&Beta",  GetTheme()->GetDefaultIcon());
	m_cMenu.AddItem("&Gamma", GetTheme()->GetDefaultIcon());
	m_cMenu.AddItem("&Delta", GetTheme()->GetDefaultIcon());
	m_cMenu.AddSeparator();
	m_cMenu.AddSubMenu(&m_cMenu, false);

	// Create menu bar
	m_pMenuBar = new MenuBar(GetContentWidget());
	m_pMenuBar->SignalItemClicked.Connect(EventHandlerMenuItemClicked);
	m_pMenuBar->SetMenu(&m_cMenu);
	m_pMenuBar->SetVisible(true);

	// Create tab bar
	/*
	m_pTabContainer = new TabContainer(GetContentWidget());
	m_pTabContainer->EventCreateWindow.Connect(&EventHandlerCreateWindow);
//	m_pTabContainer->SetSide(SideTop);
	m_pTabContainer->SetSide(SideLeft);
	m_pTabContainer->SetOptions(TabOptionStaticSize | TabOptionCreateTab | TabOptionCloseTab);
	m_pTabContainer->SetVisible(true);

	// Add some tabs
	for (int i=0; i<10; i++) {
		// Create new window
		OnTabContainerCreateWindow();
	}
	*/

	// Add a border widget to the first window
	/*
	Widget *pWindow = m_pTabContainer->GetWindows().Get(0);
	Border *pBorder = new Border(pWindow->GetContentWidget());
	pBorder->GetLayoutHints().SetSizeX(SizeHint(SizeHint::Percent, 50));
	pBorder->GetLayoutHints().SetSizeY(SizeHint(SizeHint::Pixel, 200));
	pBorder->SetVisible(true);

	// Add a splitter to the second window
	pWindow = m_pTabContainer->GetWindows().Get(1);
	Splitter *pSplitter = new Splitter(pWindow->GetContentWidget());
	pSplitter->SetSizeOfSide(SideLeft, SizeHint(SizeHint::Percent, 10));
	pSplitter->SetVisible(true);

		// Add a second splitter
		pWindow = m_pTabContainer->GetWindows().Get(1);
		Splitter *pSplitter2 = new Splitter(pSplitter->GetSide(SideRight));
		pSplitter2->SetOrientation(Vertical);
		pSplitter2->SetSizeOfSide(SideTop, SizeHint(SizeHint::Percent, 10));
		pSplitter2->SetVisible(true);

	// Add a scroll-widget to the third window
	pWindow = m_pTabContainer->GetWindows().Get(2);
	ScrollWidget *pScrollWidget = new ScrollWidget(pWindow->GetContentWidget());
	pScrollWidget->GetLayoutHints().SetSizeX(SizeHint(SizeHint::Percent, 50));
	pScrollWidget->GetLayoutHints().SetSizeY(SizeHint(SizeHint::Pixel, 200));
	pScrollWidget->SetVisible(true);

		// Add a bitmap
		Bitmap *pBitmap = new Bitmap(pScrollWidget->GetContentWidget());
		Image cImage(*GetGui());
		cImage.Load("../Data/background.jpg");
		pBitmap->SetImage(cImage);
		pBitmap->SetVisible(true);
	*/

	// Set size
	SetSize(Vector2i(640, 480));

	// Make not resizable and disable maximize button
	SetResizable(false);
	SetSystemButtons(SystemButtonMinimize | SystemButtonClose);
}

/**
*  @brief
*    Destructor
*/
TabsAndMenuWindow::~TabsAndMenuWindow()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when a new tab shall be created
*/
void TabsAndMenuWindow::OnTabContainerCreateWindow()
{
	/*
	// Create new window
	Widget *pWindow = new ContainerWidget(m_pTabContainer->GetContentWidget());

	// Set title and icon
	pWindow->SetTitle(String("Dummy ") + m_nNextWindow);
	pWindow->SetIcon(GetTheme()->GetDefaultIcon());

	// Set color: red, green, blue, yellow, red, ...
	if (m_nNextWindow%4 == 0) pWindow->GetContentWidget()->SetBackgroundColor(Color4::Red);
	if (m_nNextWindow%4 == 1) pWindow->GetContentWidget()->SetBackgroundColor(Color4::Green);
	if (m_nNextWindow%4 == 2) pWindow->GetContentWidget()->SetBackgroundColor(Color4::Blue);
	if (m_nNextWindow%4 == 3) pWindow->GetContentWidget()->SetBackgroundColor(Color4::Yellow);

	// Add window
	m_pTabContainer->AddWindow(pWindow);

	// And select the new window
	m_pTabContainer->SetSelection(pWindow);

	// Increase window ID
	m_nNextWindow++;
	*/
}

/**
*  @brief
*    Called when a menu item has been clicked
*/
void TabsAndMenuWindow::OnMenuItemClicked(MenuItem *pItem)
{
	// [DEBUG]
	System::GetInstance()->GetConsole().Print(pItem->GetText() + '\n');
}


//[-------------------------------------------------------]
//[ Compiler settings                                     ]
//[-------------------------------------------------------]
PL_WARNING_POP
