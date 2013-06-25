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
#ifndef __PREFERENCES_HH__
#define __PREFERENCES_HH__

#include <QDialog>
#include <QWidget>
#include <QScrollArea>
#include <QPushButton>

#include "config.hh"

class QListWidget;
class QListWidgetItem;
class QStackedWidget;

class QLabel;
class QLineEdit;
class QCheckBox;
class QSpinBox;
class CFileChooser;
class CMainWindow;

class QtGroupBoxPropertyBrowser;

/** \brief ConfigDialog handles the display of the config pages
 */
class ConfigDialog : public QDialog
{
  Q_OBJECT

public:
  /// Constructor.
  ConfigDialog(QWidget* parent=0);

  /*!
    Returns the parent widget (which is the main window of the application).
  */
  CMainWindow* parent() const;

public slots:
  /*!
    Changes the configuration page from \a previous to \a current.
  */
  void changePage(QListWidgetItem *current, QListWidgetItem *previous);

protected:
  /*!
    Saves all pages settings before closing.
  */
  void closeEvent(QCloseEvent *event);

private:
  void createIcons();

  QListWidget *m_contentsWidget;
  QStackedWidget *m_pagesWidget;
};

/** \brief Page is the base class for config page
 */
class Page : public QScrollArea
{
  Q_OBJECT
public:
  /// Constructor.
  Page(QWidget *parent=0);

  /*!
    Returns the parent widget (which is the ConfigDialog object).
  */
  ConfigDialog * parent() const;

  /*!
    Applies the layout \a layout to the current page.
  */
  void setLayout(QLayout *layout);

protected:
  /*!
    Saves settings before closing the page.
  */
  void closeEvent(QCloseEvent *event);

private:
  virtual void readSettings();
  virtual void writeSettings();

  QWidget *m_content;
};

/**
 * \class DisplayPage
 * \brief DisplayPage is the config page used to specify
 * which components should be displayed/hidden
 * \image html pref-display.png
 */
class DisplayPage : public Page
{
  Q_OBJECT

public:
  /// Constructor.
  DisplayPage(QWidget *parent=0);

private:
  void readSettings();
  void writeSettings();

  QCheckBox *m_statusBarCheckBox;
  QCheckBox *m_toolBarCheckBox;
};

/** \brief NetworkPage is the config page used to specify network options
 * \image html pref-network.png
 */
class NetworkPage : public Page
{
  Q_OBJECT

public:
  /// Constructor.
  NetworkPage(QWidget *parent=0);

private:
  void readSettings();
  void writeSettings();

  QLineEdit *m_hostname;
  QSpinBox *m_port;
  QLineEdit *m_user;
  QLineEdit *m_password;
};

#endif // __PREFERENCES_HH__
