/*********************************************************\
 *  File: DockWidget.cpp                                 *
 *
 *  Copyright (C) 2002-2012 The PixelLight Team (http://www.pixellight.org/)
 *
 *  This file is part of PixelLight.
 *
 *  PixelLight is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  PixelLight is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with PixelLight. If not, see <http://www.gnu.org/licenses/>.
\*********************************************************/


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <QtGui/qdockwidget.h>
#include "PLFrontendQt/DockWidget/DockWidgetManager.h"
#include "PLFrontendQt/DockWidget/DockWidget.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLFrontendQt {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(DockWidget)


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Simple helper class to inform the RTTI dock widget when the Qt dock widget gets destroyed
*/
class InternalQDockWidget : public QDockWidget {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cDockWidget
		*    RTTI dock widget we need to inform when we get destroyed
		*  @param[in] pQWidgetParent
		*    Pointer to parent Qt widget as parameter, can be a null pointer
		*/
		InternalQDockWidget(DockWidget &cDockWidget, QWidget *pQWidgetParent) : QDockWidget(pQWidgetParent),
			m_pDockWidget(&cDockWidget)
		{
			// By default, when someone is closing the dock widget Qt hides instead of deletes it
			// -> We could write
			//        setAttribute(Qt::WA_DeleteOnClose);	// Delete the dock widget instead of hiding it when someone is closing it
			//    to change this behaviour, but when "reopening" the previously made changes to the widget layout would be gone and
			//    we don't want this. So, long story short conclusion, don't change the default behaviour in here.
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~InternalQDockWidget()
		{
			// Inform the RTTI dock widget politely that we're now dead, then shoot at it...
			m_pDockWidget->m_pQDockWidget = nullptr;
			delete m_pDockWidget;
		}


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		DockWidget *m_pDockWidget;	/**< RTTI dock widget we need to inform when we get destroyed, always valid */


};


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
DockWidget::~DockWidget()
{
	// If there's a dock widget manager provided, unregister this dock widget
	if (m_pDockWidgetManager)
		m_pDockWidgetManager->UnregisterDockWidget(*this);

	// Destroy the encapsulated Qt dock widget, if it still exists
	if (m_pQDockWidget)
		delete m_pQDockWidget;
}

/**
*  @brief
*    Returns whether or not the encapsulated Qt dock widget is currently visible
*/
bool DockWidget::IsQDockWidgetVisible() const
{
	return m_pQDockWidget ? m_pQDockWidget->isVisible() : false;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
DockWidget::DockWidget(QWidget *pQWidgetParent, DockWidgetManager *pDockWidgetManager) :
	m_pQDockWidget(new InternalQDockWidget(*this, pQWidgetParent)),
	m_pDockWidgetManager(pDockWidgetManager)
{
	// If there's a dock widget manager provided, register this dock widget
	if (m_pDockWidgetManager)
		m_pDockWidgetManager->RegisterDockWidget(*this);
}

/**
*  @brief
*    Set dock widgets attribute value by using a given string value (dock widget broadcast, excludes the emitting dock widget)
*/
void DockWidget::SetDockWidgetsAttribute(const String &sName, const String &sValue)
{
	// If there's a dock widget manager provided, perform a dock widget manager broadcast
	if (m_pDockWidgetManager) {
		// Get list of registered dock widgets
		const Array<DockWidget*> &lstDockWidgets = m_pDockWidgetManager->m_lstDockWidgets;

		// Perform broadcast, but exclude this emitting dock widget
		for (uint32 i=0; i<lstDockWidgets.GetNumOfElements(); i++) {
			// Get the current dock widget, do the exclude test and broadcast
			DockWidget *pDockWidget = lstDockWidgets[i];
			if (pDockWidget != this)
				pDockWidget->SetAttribute(sName, sValue);
		}
	}
}

/**
*  @brief
*    Set dock widgets attribute value by using a given dynamic variable reference (dock widget broadcast, excludes the emitting dock widget)
*/
void DockWidget::SetDockWidgetsAttribute(const String &sName, const DynVar &cVar)
{
	// If there's a dock widget manager provided, perform a dock widget manager broadcast
	if (m_pDockWidgetManager) {
		// Get list of registered dock widgets
		const Array<DockWidget*> &lstDockWidgets = m_pDockWidgetManager->m_lstDockWidgets;

		// Perform broadcast, but exclude this emitting dock widget
		for (uint32 i=0; i<lstDockWidgets.GetNumOfElements(); i++) {
			// Get the current dock widget, do the exclude test and broadcast
			DockWidget *pDockWidget = lstDockWidgets[i];
			if (pDockWidget != this)
				pDockWidget->SetAttribute(sName, cVar);
		}
	}
}

/**
*  @brief
*    Set dock widgets attribute value by using a given dynamic variable pointer (dock widget broadcast, excludes the emitting dock widget)
*/
void DockWidget::SetDockWidgetsAttribute(const String &sName, const DynVar *pVar)
{
	// If there's a dock widget manager provided, perform a dock widget manager broadcast
	if (m_pDockWidgetManager) {
		// Get list of registered dock widgets
		const Array<DockWidget*> &lstDockWidgets = m_pDockWidgetManager->m_lstDockWidgets;

		// Perform broadcast, but exclude this emitting dock widget
		for (uint32 i=0; i<lstDockWidgets.GetNumOfElements(); i++) {
			// Get the current dock widget, do the exclude test and broadcast
			DockWidget *pDockWidget = lstDockWidgets[i];
			if (pDockWidget != this)
				pDockWidget->SetAttribute(sName, pVar);
		}
	}
}

/**
*  @brief
*    Set dock widgets attribute to it's default value (dock widget broadcast, excludes the emitting dock widget)
*/
void DockWidget::SetDockWidgetsAttributeDefault(const String &sName)
{
	// If there's a dock widget manager provided, perform a dock widget manager broadcast
	if (m_pDockWidgetManager) {
		// Get list of registered dock widgets
		const Array<DockWidget*> &lstDockWidgets = m_pDockWidgetManager->m_lstDockWidgets;

		// Perform broadcast, but exclude this emitting dock widget
		for (uint32 i=0; i<lstDockWidgets.GetNumOfElements(); i++) {
			// Get the current dock widget, do the exclude test and broadcast
			DockWidget *pDockWidget = lstDockWidgets[i];
			if (pDockWidget != this)
				pDockWidget->SetAttributeDefault(sName);
		}
	}
}

/**
*  @brief
*    Call dock widgets method with given dynamic parameters (dock widget broadcast, excludes the emitting dock widget)
*/
void DockWidget::CallDockWidgetsMethod(const String &sName, DynParams &cParams)
{
	// If there's a dock widget manager provided, perform a dock widget manager broadcast
	if (m_pDockWidgetManager) {
		// Get list of registered dock widgets
		const Array<DockWidget*> &lstDockWidgets = m_pDockWidgetManager->m_lstDockWidgets;

		// Perform broadcast, but exclude this emitting dock widget
		for (uint32 i=0; i<lstDockWidgets.GetNumOfElements(); i++) {
			// Get the current dock widget, do the exclude test and broadcast
			DockWidget *pDockWidget = lstDockWidgets[i];
			if (pDockWidget != this)
				pDockWidget->CallMethod(sName, cParams);
		}
	}
}

/**
*  @brief
*    Call dock widgets method with given constant dynamic parameters (dock widget broadcast, excludes the emitting dock widget)
*/
void DockWidget::CallDockWidgetsMethod(const String &sName, const DynParams &cParams)
{
	// If there's a dock widget manager provided, perform a dock widget manager broadcast
	if (m_pDockWidgetManager) {
		// Get list of registered dock widgets
		const Array<DockWidget*> &lstDockWidgets = m_pDockWidgetManager->m_lstDockWidgets;

		// Perform broadcast, but exclude this emitting dock widget
		for (uint32 i=0; i<lstDockWidgets.GetNumOfElements(); i++) {
			// Get the current dock widget, do the exclude test and broadcast
			DockWidget *pDockWidget = lstDockWidgets[i];
			if (pDockWidget != this)
				pDockWidget->CallMethod(sName, cParams);
		}
	}
}

/**
*  @brief
*    Call dock widgets method with parameters given as string (dock widget broadcast, excludes the emitting dock widget)
*/
void DockWidget::CallDockWidgetsMethod(const String &sName, const String &sParams)
{
	// If there's a dock widget manager provided, perform a dock widget manager broadcast
	if (m_pDockWidgetManager) {
		// Get list of registered dock widgets
		const Array<DockWidget*> &lstDockWidgets = m_pDockWidgetManager->m_lstDockWidgets;

		// Perform broadcast, but exclude this emitting dock widget
		for (uint32 i=0; i<lstDockWidgets.GetNumOfElements(); i++) {
			// Get the current dock widget, do the exclude test and broadcast
			DockWidget *pDockWidget = lstDockWidgets[i];
			if (pDockWidget != this)
				pDockWidget->CallMethod(sName, sParams);
		}
	}
}

/**
*  @brief
*    Call dock widgets method with parameters given as XML element (dock widget broadcast, excludes the emitting dock widget)
*/
void DockWidget::CallDockWidgetsMethod(const String &sName, const XmlElement &cElement)
{
	// If there's a dock widget manager provided, perform a dock widget manager broadcast
	if (m_pDockWidgetManager) {
		// Get list of registered dock widgets
		const Array<DockWidget*> &lstDockWidgets = m_pDockWidgetManager->m_lstDockWidgets;

		// Perform broadcast, but exclude this emitting dock widget
		for (uint32 i=0; i<lstDockWidgets.GetNumOfElements(); i++) {
			// Get the current dock widget, do the exclude test and broadcast
			DockWidget *pDockWidget = lstDockWidgets[i];
			if (pDockWidget != this)
				pDockWidget->CallMethod(sName, cElement);
		}
	}
}

/**
*  @brief
*    Set multiple dock widgets attribute values as a string at once (dock widget broadcast, excludes the emitting dock widget)
*/
void DockWidget::SetDockWidgetsValues(const String &sVars)
{
	// If there's a dock widget manager provided, perform a dock widget manager broadcast
	if (m_pDockWidgetManager) {
		// Get list of registered dock widgets
		const Array<DockWidget*> &lstDockWidgets = m_pDockWidgetManager->m_lstDockWidgets;

		// Perform broadcast, but exclude this emitting dock widget
		for (uint32 i=0; i<lstDockWidgets.GetNumOfElements(); i++) {
			// Get the current dock widget, do the exclude test and broadcast
			DockWidget *pDockWidget = lstDockWidgets[i];
			if (pDockWidget != this)
				pDockWidget->SetValues(sVars);
		}
	}
}

/**
*  @brief
*    Set dock widgets attribute values from XML (dock widget broadcast, excludes the emitting dock widget)
*/
void DockWidget::SetDockWidgetsValuesXml(const XmlElement &cElement)
{
	// If there's a dock widget manager provided, perform a dock widget manager broadcast
	if (m_pDockWidgetManager) {
		// Get list of registered dock widgets
		const Array<DockWidget*> &lstDockWidgets = m_pDockWidgetManager->m_lstDockWidgets;

		// Perform broadcast, but exclude this emitting dock widget
		for (uint32 i=0; i<lstDockWidgets.GetNumOfElements(); i++) {
			// Get the current dock widget, do the exclude test and broadcast
			DockWidget *pDockWidget = lstDockWidgets[i];
			if (pDockWidget != this)
				pDockWidget->SetValuesXml(cElement);
		}
	}
}

/**
*  @brief
*    Set all dock widgets attributes to default (dock widget broadcast, excludes the emitting dock widget)
*/
void DockWidget::SetDockWidgetsDefaultValues()
{
	// If there's a dock widget manager provided, perform a dock widget manager broadcast
	if (m_pDockWidgetManager) {
		// Get list of registered dock widgets
		const Array<DockWidget*> &lstDockWidgets = m_pDockWidgetManager->m_lstDockWidgets;

		// Perform broadcast, but exclude this emitting dock widget
		for (uint32 i=0; i<lstDockWidgets.GetNumOfElements(); i++) {
			// Get the current dock widget, do the exclude test and broadcast
			DockWidget *pDockWidget = lstDockWidgets[i];
			if (pDockWidget != this)
				pDockWidget->SetDefaultValues();
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLFrontendQt
