/*********************************************************\
 *  File: PLDynVarStringTreeItem.cpp                     *
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
#include <PLCore/Base/Var/DynVar.h>
#include <PLCore/Base/Var/VarDesc.h>
#include "PLFrontendQt/QtStringAdapter.h"
#include "PLFrontendQt/DataModels/PLTreeItems/PLDynVarStringTreeItem.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLFrontendQt {
namespace DataModels {


PLDynVarStringTreeItem::PLDynVarStringTreeItem(PLCore::DynVar *dynVar, PLDynVarTreeItemTypes::DynVarTreeItemTypes dynVarType, TreeItemBase *parent) : TreeItemBase(2, parent),
	m_dynVar(dynVar),
	m_varName(dynVar ? QtStringAdapter::PLToQt(dynVar->GetDesc()->GetName()): ""),
	m_typeName(dynVar ? QtStringAdapter::PLToQt(dynVar->GetTypeName()): ""),
	m_dynVarType(dynVarType)
{
	SetFlags(1, Qt::ItemIsEditable);
	
	QString tooltip(QObject::tr("<table>"
							"<tr><td bgcolor=#00ff00 colspan=\"2\">Attribute Information</td></tr>"
							"<tr><td>Name: </td><td>%1</td></tr>"
							"<tr><td>Type: </td><td>%2</td></tr>"
							"<tr><td>Default: </td><td>%3</td></tr>"
							"<tr><td>Description: </td><td>%4</td></tr>"
							"</table>").arg(m_varName,
											QtStringAdapter::PLToQt(dynVar->GetDesc()->GetTypeName()),
											QtStringAdapter::PLToQt(dynVar->GetDesc()->GetDefault()),
											QtStringAdapter::PLToQt(dynVar->GetDesc()->GetDescription())));
	// Add Tooltip info for column 0
	m_cToolTips.append(tooltip);
}

QVariant PLDynVarStringTreeItem::data(const int column, const int role)
{
	if (column == 1 && role == PLDynVarTreeItemTypes::DynVarItemTypeRole)
		return m_dynVarType;
	
	if (column < m_cToolTips.count() && role == Qt::ToolTipRole)
		return m_cToolTips[column];
	
	if (!IsInStandardRole(role))
		return QVariant();

	if (m_dynVar == nullptr)
		return QVariant();

	if (column == 0)
		return m_varName;
	else if (column == 1)
		return QtStringAdapter::PLToQt(m_dynVar->GetString());

	return QVariant();
}

bool PLDynVarStringTreeItem::setData(const int column, const QVariant &value, const int role)
{
	if (role != Qt::EditRole || column != 1)
		return false;

	QString strVal = value.value<QString>();
	m_dynVar->SetString(QtStringAdapter::QtToPL(strVal));
	return true;
}

bool PLDynVarStringTreeItem::IsInStandardRole(int role)
{
	return (role == Qt::DisplayRole || role == Qt::ToolTipRole || role == Qt::EditRole);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // DataModels
} // PLFrontendQt
