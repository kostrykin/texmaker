/***************************************************************************
 *   copyright       : (C) 2003-2009 by Pascal Brachet                     *
 *   addons by Luis Silvestre                                              *
 *   http://www.xm1math.net/texmaker/                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include <QDebug>
#include <QLocale>
#include <QDir>
#include <QFileOpenEvent>
#include <QSettings>
#include <QFontDatabase>
#include "texmakerapp.h"


TexmakerApp* TexmakerApp::theAppInstance = 0;


TexmakerApp::TexmakerApp( const QString& appId, unsigned int argc, const char** argv )
    : QtSingleApplication( appId, reinterpret_cast< int& >( argc ), const_cast< char** >( argv ) ) // FIXME: screw Qt's type qualifiers!
    , mw( 0 )
    , language( QString( QLocale::system().name() ) )
{
    theAppInstance = this;
}


TexmakerApp::~TexmakerApp()
{
    SaveSettings();
}


void TexmakerApp::makeTranslation( const QString& lang )
{
    const QString locale = lang.length() < 2 ? "en" : lang;
    foreach( QTranslator* tr, translatorsList ) 
    {
        removeTranslator( tr );
        delete tr;
    }
    translatorsList.clear();

    QTranslator* const   appTranslator = new QTranslator( this );
    QTranslator* const basicTranslator = new QTranslator( this );

    const QString transdir =
        #if defined(Q_OS_UNIX) && !defined(Q_OS_MAC)
        #ifdef USB_VERSION
            QCoreApplication::applicationDirPath();
        #else
            PREFIX"/share/texmaker";
        #endif
        #endif
        #if defined(Q_OS_MAC)
            QCoreApplication::applicationDirPath() + "/../Resources";
        #endif
        #if defined(Q_OS_WIN32)
            QCoreApplication::applicationDirPath();
        #endif

    if( appTranslator->load( QString( "texmaker_" ) + locale, transdir ) )
    {
        installTranslator( appTranslator );
        translatorsList.append( appTranslator );
    }
    if( basicTranslator->load( QString( "qt_" ) + locale, transdir ) ) 
    {
        installTranslator( basicTranslator );
        translatorsList.append( basicTranslator );
    }
}


/**
 * Initializes a new Texmaker instance using the commandline arguments `args`.
 */
void TexmakerApp::init( QStringList args )
{
    /* In contrast to the `ReadSettings` in `texmaker.cpp`,
     * this here only loads the language setting.
     */
    ReadSettings();
    makeTranslation( language );

    QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(":/fonts/DejaVuSansCondensed.ttf"));
    QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(":/fonts/DejaVuSansCondensed-Bold.ttf"));
    QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(":/fonts/DejaVuSansCondensed-Oblique.ttf"));
    
    mw.reset( new Texmaker() );
    if( args.isEmpty() )
    {
        /* No commandline arguments were given,
         * thus restore the last session.
         */
        mw->loadSession();
    }
    else
    {
        /* Start a new session and load the given files,
         * by interpreting the following commandline arguments scheme:
         *
         *      [ <file> [ -line N | -master ]* ]+
         */
        mw->startNewSession();
        for( QStringList::Iterator it = ++( args.begin() ); it != args.end(); it++ )
        {
            if( ( *it )[ 0 ] != '-')
            {
                mw->load( *it );
            }
            else
            if( *it == "-master" )
            {
                mw->ToggleMode();
            }
            else
            if( ( *it == "-line" ) && ( ++it != args.end() ) )
            {
                mw->setLine( *it );
            }
        }
    }

    /* Quit as soon as the last window was closed.
     */ 
    connect( this, SIGNAL( lastWindowClosed() ), this, SLOT( quit() ) );
}


bool TexmakerApp::event ( QEvent * event )
{
if ( event->type() == QEvent::ApplicationActivate )
{
if (mw.get()) mw->mainWindowActivated();
}
// else if ( event->type() == QEvent::ApplicationDeactivate )
// {
//   qDebug() << "ApplicationDeactivate";
// }
#if defined(Q_OS_MAC)
if (event->type() == QEvent::FileOpen) 
    {
    QFileOpenEvent *oe = static_cast<QFileOpenEvent *>(event);
    if (mw) mw->load(oe->file());
    else MacFile=oe->file();
    }
#endif
return QApplication::event(event);
}


void TexmakerApp::ReadSettings() // TODO: make these re-usable
{
#ifdef USB_VERSION
QSettings *config=new QSettings(QCoreApplication::applicationDirPath()+"/texmakerapp.ini",QSettings::IniFormat); //for USB-stick version
#else
QSettings *config=new QSettings(QSettings::IniFormat,QSettings::UserScope,"xm1","texmakerapp");
#endif

language=config->value("Language",QString(QLocale::system().name())).toString();
}

void TexmakerApp::SaveSettings()
{
#ifdef USB_VERSION
QSettings config(QCoreApplication::applicationDirPath()+"/texmakerapp.ini",QSettings::IniFormat); //for USB-stick version 
#else
QSettings config(QSettings::IniFormat,QSettings::UserScope,"xm1","texmakerapp");
#endif
config.setValue( "Language",language);
}
