/***************************************************************************
 *   copyright       : (C) 2003-2009 by Pascal Brachet                     *
 *   http://www.xm1math.net/texmaker/                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef TEXMAKERAPP_H
#define TEXMAKERAPP_H


#include <QTranslator>
#include <memory>
#include "texmaker.h"
#include "singleapp/qtsingleapplication.h"


class TexmakerApp : public QtSingleApplication
{

public:

    TexmakerApp( const QString& id, unsigned int argc, const char** argv );
    ~TexmakerApp();

    /* TODO: make these private
     */
    std::auto_ptr< Texmaker > mw;
    QList< QTranslator* > translatorsList;
    QString language;

    void init( QStringList args );  // This function does all the initialization instead of the constructor.
    void makeTranslation(const QString &lang);
    static TexmakerApp *instance();
    void ReadSettings();
    void SaveSettings();

private:

    static TexmakerApp *theAppInstance;

protected:

    bool event(QEvent *event);

#if defined(Q_OS_MAC)

    QString MacFile;

#endif

};


inline TexmakerApp *TexmakerApp::instance()
{
	return theAppInstance;
}
#endif
