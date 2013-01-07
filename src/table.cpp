/*-----------------------------------------------------------------------------
Copyright © 2013 Luke Salisbury
This software is provided 'as-is', without any express or implied warranty. In 
no event will the authors be held liable for any damages arising from the use 
of this software.

Permission is granted to anyone to use this software for any purpose, including 
commercial applications, and to alter it and redistribute it freely, subject to 
the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim 
   that you wrote the original software. If you use this software in a product, 
   an acknowledgment in the product documentation would be appreciated but is 
   not required.
2. Altered source versions must be plainly marked as such, and must not be 
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
-----------------------------------------------------------------------------*/
#include "table.hpp"
#include <QDebug>
TableModel::TableModel(QObject *parent)	 : QAbstractTableModel(parent)
{

}


int TableModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return listOfPairs.size();
}

int TableModel::columnCount(const QModelIndex &parent) const
{
	 Q_UNUSED(parent);
	 return 6;
}

 QVariant TableModel::data(const QModelIndex &index, int role) const
 {
	if (!index.isValid())
		return QVariant();

	 if (index.row() >= listOfPairs.size() || index.row() < 0)
		return QVariant();

	TableItem item = listOfPairs.at(index.row());
	if ( index.column() == 0 && role == Qt::DecorationRole )
		return item.img;
	else if (index.column() == 1 && role == Qt::DisplayRole )
		return item.id;
	else if (index.column() == 2 && role == Qt::DisplayRole )
		return item.replace_id;
	else if (index.column() == 3 && role == Qt::DisplayRole )
		return item.remove;
	else if (index.column() == 4 && role == Qt::DisplayRole )
		return item.mask;
	else if (index.column() == 5 && role == Qt::DisplayRole )
		return item.flipmode;
	return QVariant();
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::SizeHintRole && section == 0)
		return QSize(32,32);

	if (role != Qt::DisplayRole)
		return QVariant();
	if (orientation == Qt::Horizontal) {
		switch (section) {
			case 0:
				return tr("Image");
			case 1:
				return tr("ID");
			case 2:
				return tr("replace");
			case 3:
				return tr("remove");
			case 4:
				return tr("mask");
			case 5:
				return tr("rotate");
			default:
				return QVariant();
		}
	}
	return QVariant();
}

bool TableModel::insertRows(int position, int rows, const QModelIndex &index)
{
	Q_UNUSED(index);
	beginInsertRows(QModelIndex(), position, position+rows-1);

	for (int row=0; row < rows; row++) {
		TableItem item;
		listOfPairs.insert(position, item);
	}

	endInsertRows();
	return true;
}

bool TableModel::removeRows(int position, int rows, const QModelIndex &index)
{
	Q_UNUSED(index);
	beginRemoveRows(QModelIndex(), position, position+rows-1);

	for (int row=0; row < rows; ++row) {
		listOfPairs.removeAt(position);
	}

	endRemoveRows();
	return true;
}

bool TableModel::setImage(const QModelIndex &index, const QImage * value)
{
	if (index.isValid() ) {
		int row = index.row();

		TableItem p = listOfPairs.value(row);
		p.img = QPixmap::fromImage(*value);
		listOfPairs.replace(row, p);
		emit(dataChanged(index, index));

		return true;
	}
	return false;
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (index.isValid()) {
		int row = index.row();

		TableItem p = listOfPairs.value(row);

		if ( role == Qt::EditRole )
		{
			if (index.column() == 2)
				p.replace_id = value.toString();
			else if (index.column() == 3)
				p.remove = value.toBool();
			else if (index.column() == 4 || index.column() == 5)
				p.mask = value.toUInt();
			else	if (index.column() == 0)
				return true;
			else
				return false;
		}
		else if ( role == Qt::DisplayRole && index.column() == 1 )
		{
			p.id = value.toString();
		}
		listOfPairs.replace(row, p);
		emit(dataChanged(index, index));

		return true;
	}
	return false;
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::ItemIsEnabled;
	return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

QList< TableItem > TableModel::getList()
{
	return listOfPairs;
}
