// Copyright (C) 2013, Romain Goffe <romain.goffe@gmail.com>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
// 02110-1301, USA.
//******************************************************************************
#include "matrix-model.hh"

#include <QDebug>
#include <QXmlStreamReader>
#include <QFile>

CMatrixModel::CMatrixModel()
  : QAbstractTableModel()
  , m_data()
{
}

CMatrixModel::~CMatrixModel()
{
}

cv::Mat CMatrixModel::data() const
{
  return m_data;
}

void CMatrixModel::setData(const cv::Mat & matrix)
{
  m_data = matrix;
  emit(dataChanged(QModelIndex(), QModelIndex()));
}

int CMatrixModel::rowCount(const QModelIndex & parent) const
{
  Q_UNUSED(parent);
  return data().rows;
}

int CMatrixModel::columnCount(const QModelIndex & parent) const
{
  Q_UNUSED(parent);
  return data().cols;
}

QVariant CMatrixModel::data(const QModelIndex & index, int role) const
{
  switch (role)
    {
    case Qt::DisplayRole:
      {
	cv::Vec3b pixel;
	switch(type())
	  {
	  case CV_8UC1:
	    return data().at< uchar >(index.row(), index.column());

	  case CV_8UC3:
	    pixel = data().at< cv::Vec3b >(index.row(), index.column());
	    return QString("%1 | %2 | %3").arg(pixel[0]).arg(pixel[1]).arg(pixel[2]);

	  case CV_32FC1:
	    return data().at< float >(index.row(), index.column());

	  case CV_64FC1:
	    return data().at< double >(index.row(), index.column());

	  default:
	    return QVariant();
	  }
	break;
      }

    default:
      data(index, Qt::DisplayRole);
    }

  return QVariant();
}

bool CMatrixModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
  switch (role)
    {
    case Qt::EditRole:
	data().at< double >(index.row(), index.column()) = value.toDouble();
	emit(dataChanged(QModelIndex(), QModelIndex()));
	return true;

    default:
      qDebug() << "setdata role not supported yet";
    }

 return false;
}

QVariant CMatrixModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role == Qt::DisplayRole)
    {
    if (orientation == Qt::Horizontal)
      {
	return section < m_horizontalHeaderLabels.size() ?
	  m_horizontalHeaderLabels.at(section) : QString::number(section);
      }
    else if(orientation == Qt::Vertical)
      {
	return section < m_verticalHeaderLabels.size() ?
	  m_verticalHeaderLabels.at(section) : QString::number(section);
      }
    }
  return QVariant();
}

Qt::ItemFlags CMatrixModel::flags(const QModelIndex & index) const
{
  (void) index;
  return Qt::ItemIsSelectable | Qt::ItemIsEditable  | Qt::ItemIsEnabled;
}


void CMatrixModel::sort(int column, Qt::SortOrder order)
{
  Q_UNUSED(column);
  const int cvOrder = (order == Qt::AscendingOrder) ? CV_SORT_ASCENDING : CV_SORT_DESCENDING;
  cv::sort(m_data, m_data, CV_SORT_EVERY_COLUMN | cvOrder);
  emit(dataChanged(QModelIndex(), QModelIndex()));
}

int CMatrixModel::channels() const
{
  return m_data.channels();
}

int CMatrixModel::type() const
{
  return m_data.type();
}

QString CMatrixModel::typeString() const
{
  switch (type())
    {
    case CV_8U:
      return "CV_8U";

    case CV_32F:
      return "CV_32F";

    case CV_64F:
      return "CV_64F";

    default:
      return tr("unknown");
    }
}

void CMatrixModel::setProfile(const QString & profile)
{
  // Parse profile for rows/columns labels
  m_horizontalHeaderLabels.clear();
  m_verticalHeaderLabels.clear();

  QFile file(profile);
  if (!file.open(QIODevice::ReadOnly))
    {
      qWarning() << tr("Can't open profile in read mode: %1") << profile;
      return;
    }

  QXmlStreamReader xml(&file);
  while (!xml.atEnd())
    {
      if (xml.readNext())
	{
	  if (xml.name() == "column")
	    {
	      QString label = xml.attributes().value("label").toString().simplified();
	      if (!label.isEmpty())
		m_horizontalHeaderLabels << label;
	    }
	  else if (xml.name() == "row")
	    {
	      QString label = xml.attributes().value("label").toString().simplified();
	      if (!label.isEmpty())
		m_verticalHeaderLabels << label;
	    }
	}
    }

  if (xml.hasError())
    {
      qWarning() << tr("Badly formed xml document: %1").arg(profile);
      qWarning() << tr("Error: %1").arg(xml.errorString());
    }

  file.close();

  emit(headerDataChanged(Qt::Horizontal, 0, m_horizontalHeaderLabels.size()));
  emit(headerDataChanged(Qt::Vertical, 0, m_verticalHeaderLabels.size()));
}
