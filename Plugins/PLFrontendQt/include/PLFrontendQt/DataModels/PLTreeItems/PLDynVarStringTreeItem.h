/*********************************************************\
 *  File: PLDynVarStringTreeItem.h                       *
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


#ifndef __PLFRONTENDQT_PLDYNVARSTRINGTREEITEM_H__
#define __PLFRONTENDQT_PLDYNVARSTRINGTREEITEM_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLFrontendQt/DataModels/TreeItemBase.h"
#include "PLFrontendQt/DataModels/PLTreeItems/PLDynVarTreeItemTypes.h"
#include <QtCore/QStringList>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLCore {
	class DynVar;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLFrontendQt {
namespace DataModels {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Dynamic variable tree item: String
*/
class PLFRONTENDQT_API PLDynVarStringTreeItem : public TreeItemBase {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		explicit PLDynVarStringTreeItem(PLCore::DynVar *dynVar, PLDynVarTreeItemTypes::DynVarTreeItemTypes dynVarType = PLDynVarTreeItemTypes::String, TreeItemBase *parent = nullptr);
		virtual QVariant data(const int column, const int role) override;
		virtual bool setData(const int column, const QVariant &value, const int role) override;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		bool IsInStandardRole(int role);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		PLCore::DynVar 								*m_dynVar;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		QString										 m_varName;
		QString										 m_typeName;
		QStringList 								 m_cToolTips;
		PLDynVarTreeItemTypes::DynVarTreeItemTypes	 m_dynVarType;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // DataModels
} // PLFrontendQt


#endif // __PLFRONTENDQT_PLDYNVARSTRINGTREEITEM_H__
