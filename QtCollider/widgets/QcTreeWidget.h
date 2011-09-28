/************************************************************************
*
* Copyright 2011 Jakob Leben (jakob.leben@gmail.com)
*
* This file is part of SuperCollider Qt GUI.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
************************************************************************/

#ifndef QC_TREE_WIDGET_H
#define QC_TREE_WIDGET_H

#include "../Common.h"
#include "../QObjectProxy.h"
#include "../safeptr.hpp"

#include <VMGlobals.h>
#include <PyrObject.h>

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QApplication>

using namespace QtCollider;

class QcTreeWidget : public QTreeWidget
{
public:

  class Item : public QTreeWidgetItem
  {
  public:
    enum { Type = QTreeWidgetItem::UserType };
    Item()
      : QTreeWidgetItem( Item::Type ), _safePtr(this) {}
    Item( const QStringList & strings )
      : QTreeWidgetItem( strings, Item::Type ), _safePtr(this) {}
    ~Item() { _safePtr.invalidate(); }
    SafePtr<Item> safePtr() const { return _safePtr; }
    static SafePtr<Item> safePtr( QTreeWidgetItem* );
    static void initialize( VMGlobals *, PyrObject *, const SafePtr<Item> & );
    static int finalize( VMGlobals *, PyrObject * );
  private:
    SafePtr<Item> _safePtr;
  };

  typedef SafePtr<Item> ItemPtr;

  Q_OBJECT
  Q_PROPERTY( VariantList columns READ columns WRITE setColumns )
  Q_PROPERTY( QcTreeWidget::ItemPtr currentItem READ currentItem WRITE setCurrentItem );

public:

  Q_INVOKABLE QcTreeWidget::ItemPtr item( const QcTreeWidget::ItemPtr & parent, int index );
  Q_INVOKABLE QcTreeWidget::ItemPtr parentItem( const QcTreeWidget::ItemPtr & );
  Q_INVOKABLE int indexOfItem( const QcTreeWidget::ItemPtr & );

  Q_INVOKABLE QcTreeWidget::ItemPtr addItem
  ( const QcTreeWidget::ItemPtr & parent, const VariantList & data );

  Q_INVOKABLE QcTreeWidget::ItemPtr insertItem
  ( const QcTreeWidget::ItemPtr & parent, int index, const VariantList & data );

  Q_INVOKABLE void removeItem( const QcTreeWidget::ItemPtr & );

  Q_INVOKABLE VariantList strings( const QcTreeWidget::ItemPtr & );
  Q_INVOKABLE void setText( const QcTreeWidget::ItemPtr &, int column, const QString & );
  Q_INVOKABLE void setColor( const QcTreeWidget::ItemPtr &, int column, const QColor & );
  Q_INVOKABLE void setTextColor( const QcTreeWidget::ItemPtr &, int column, const QColor & );

  Q_INVOKABLE QWidget * itemWidget( const QcTreeWidget::ItemPtr &, int column );
  Q_INVOKABLE void setItemWidget( const QcTreeWidget::ItemPtr &, int column, QObjectProxy * );
  Q_INVOKABLE void removeItemWidget( const QcTreeWidget::ItemPtr &, int column );

  Q_INVOKABLE void sort( int column, bool descending );

Q_SIGNALS:

  void action();
  void returnPressed();

public:

  QcTreeWidget();

  ItemPtr currentItem() const;
  void setCurrentItem( const ItemPtr & );

  VariantList columns() const;
  void setColumns( const VariantList & );

private Q_SLOTS:

  void onCurrentItemChanged();

private:

  bool eventFilter( QObject *, QEvent * );

  QTreeWidgetItem * _itemOnPress;
  bool _emitAction;
};

Q_DECLARE_METATYPE( QcTreeWidget::ItemPtr );

#endif