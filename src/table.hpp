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
#ifndef TABLE_HPP
#define TABLE_HPP

#include <QAbstractTableModel>
#include <QList>
#include <QImage>

#include "tableitem.hpp"

class TableModel : public QAbstractTableModel
{
	 Q_OBJECT

 public:
	 TableModel(QObject *parent=0);

	 int rowCount(const QModelIndex &parent) const;
	 int columnCount(const QModelIndex &parent) const;
	 QVariant data(const QModelIndex &index, int role) const;
	 QVariant headerData(int section, Qt::Orientation orientation, int role) const;
	 Qt::ItemFlags flags(const QModelIndex &index) const;
	 bool setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole);
	 bool insertRows(int position, int rows, const QModelIndex &index=QModelIndex());
	 bool removeRows(int position, int rows, const QModelIndex &index=QModelIndex());
	 QList< TableItem > getList();
	 bool setImage(const QModelIndex &index, const QImage *value);
 private:
	 QList< TableItem > listOfPairs;
 };

#endif // TABLE_HPP
