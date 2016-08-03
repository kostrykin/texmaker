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

#include "latexeditorview.h"
#include <QVBoxLayout>
#include <QFrame>
#include <QTextDocument>
#include <QTextCursor>
#include <QLineEdit>
#include <QTextBlock>


LatexEditorView::LatexEditorView( QWidget* const parent, const QFont& efont, bool svn, bool line, const QList< QColor >& edcolors
                                , const QList< QColor >& hicolors, bool inlinespelling, const QString& ignoredWords, Hunspell* const spellChecker
                                , bool tabspaces, int tabwidth, const QKeySequence& viewfocus, const QString& name, const QStringList& ulist )
    : QWidget( parent )
    , splitter( new MiniSplitter( this ) )
    , Stack( new QStackedWidget( this ) )
    , findwidget( new FindWidget( this->Stack ) )
    , replacewidget( new ReplaceWidget( this->Stack ) )
    , gotolinewidget( new GotoLineWidget( this->Stack ) )
{
    splitter->setOrientation( Qt::Vertical );
    
    QFrame* const framebis = new QFrame( splitter );
    framebis->setLineWidth( 0 );
    framebis->setFrameShape ( QFrame::NoFrame );
    framebis->setFrameShadow( QFrame::Plain   );
    framebis->setFrameStyle ( QFrame::NoFrame );
    
    QVBoxLayout* const mainlay = new QVBoxLayout( framebis );
    mainlay->setSpacing( 0 );
    mainlay->setMargin( 0 );
    
    QFrame* const frame = new QFrame( framebis );
    frame->setLineWidth( 1 );
    frame->setFrameShape ( QFrame::NoFrame );
    frame->setFrameShadow( QFrame::Plain   );
    frame->setFrameStyle ( QFrame::NoFrame );
    mainlay->addWidget( frame );
    
    const QFontMetrics fm( efont );
    this->lineNumberPalette.setColor( QPalette::Window, edcolors.at( 1 ) );

    editor = new LatexEditor( frame, efont, edcolors, hicolors, inlinespelling, ignoredWords, spellChecker, tabspaces, tabwidth, viewfocus, name, ulist );
    lineNumberWidget = new LineNumberWidget( editor, frame, svn );
    lineNumberWidget->setFont( efont );
    lineNumberWidget->setPalette( this->lineNumberPalette );
    lineNumberWidget->setAutoFillBackground( true );
    lineNumberWidget->setBackgroundRole( QPalette::Window );
    lineNumberWidget->setFixedWidth( fm.width( "00000" ) + 32 );

    QHBoxLayout* const lay = new QHBoxLayout( frame );
    lay->setSpacing( 0 );
    lay->setMargin( 0 );
    lay->addWidget( lineNumberWidget );
    lay->addWidget( editor );
    setFocusProxy( editor );
    setLineNumberWidgetVisible( line );
    
    Stack->setLineWidth( 0 );
    Stack->setFrameShape( QFrame::NoFrame );
    Stack->setFrameShadow( QFrame::Plain );
    
    Stack->addWidget( findwidget );
    findwidget->SetEditor( editor );
    connect( findwidget, SIGNAL( requestHide() ), Stack, SLOT( hide() ) );
    connect( findwidget, SIGNAL( requestExtension() ), this, SLOT( updateFind() ) );
    
    Stack->addWidget( replacewidget );
    replacewidget->SetEditor( editor );
    connect( replacewidget, SIGNAL( requestHide() ), Stack, SLOT( hide() ) );
    connect( replacewidget, SIGNAL( requestExtension() ), this, SLOT( updateReplace() ) );
    
    connect( editor, SIGNAL( textChanged() ), this, SLOT( updateInSelection() ) );
    
    Stack->addWidget( gotolinewidget );
    gotolinewidget->SetEditor( editor );
    connect( gotolinewidget, SIGNAL( requestHide() ), Stack, SLOT( hide() ) );
    
    Stack->hide();
    
    splitter->addWidget( framebis );
    splitter->addWidget( Stack );
    QVBoxLayout* const mainlayout = new QVBoxLayout( this );
    mainlayout->addWidget( splitter );
    mainlayout->setSpacing( 0 );
    mainlayout->setMargin( 0 );
}


LatexEditorView::~LatexEditorView()
{
}


void LatexEditorView::setLineNumberWidgetVisible( bool b )
{
    if( b )
    {
	lineNumberWidget->show();
    }
    else
    {
	lineNumberWidget->hide();
    }
}

void LatexEditorView::changeSettings( const QFont& new_font, bool svn, bool line )
{
    editor->changeFont( new_font );
    lineNumberWidget->setFont( new_font );

    const QFontMetrics fm( new_font );
    lineNumberWidget->setFixedWidth( fm.width( "00000" ) + 32 );
    lineNumberWidget->setSvn( svn );
    setLineNumberWidgetVisible( line );
}


void LatexEditorView::showFind()
{
    updateFind();
}


void LatexEditorView::updateFind()
{
    QList< int > sizes;
    sizes << height() - findwidget->height() << findwidget->height();

    splitter->setSizes( sizes );
    Stack->setCurrentWidget( findwidget );
    Stack->setMaximumHeight( findwidget->minimumSizeHint().height() );
    Stack->show();

    const QTextCursor& cursor = editor->textCursor();
    if( cursor.hasSelection() )
    {
        findwidget->ui.comboFind->lineEdit()->setText( cursor.selectedText() );
    }

    findwidget->ui.comboFind->setFocus();
    findwidget->ui.comboFind->lineEdit()->selectAll();
}


void LatexEditorView::showFindNext()
{
    QList< int > sizes;
    sizes << height() - findwidget->height() << findwidget->height();
    splitter->setSizes( sizes );

    Stack->setCurrentWidget( findwidget );
    Stack->setMaximumHeight( findwidget->minimumSizeHint().height() );
    Stack->show();

    if( !findwidget->ui.comboFind->lineEdit()->text().isEmpty() )
    {
        findwidget->doFind();
    }
}


void LatexEditorView::showReplace()
{
    updateReplace();
}


void LatexEditorView::updateReplace()
{
    QList< int > sizes;
    sizes << height() - replacewidget->height() << replacewidget->height();
    splitter->setSizes( sizes );

    Stack->setCurrentWidget( replacewidget );
    Stack->setMaximumHeight( replacewidget->minimumSizeHint().height() );
    Stack->show();

    const QTextCursor& cursor = editor->textCursor();
    if( cursor.hasSelection() )
    {
        replacewidget->ui.comboFind->lineEdit()->setText( cursor.selectedText() );
    }

    replacewidget->ui.comboFind->setFocus();
    replacewidget->ui.comboFind->lineEdit()->selectAll();
}


void LatexEditorView::showGoto()
{
    QList< int > sizes;
    sizes << height()-gotolinewidget->height() << gotolinewidget->height();
    splitter->setSizes( sizes );

    Stack->setCurrentWidget( gotolinewidget );
    Stack->setMaximumHeight( gotolinewidget->minimumSizeHint().height() );
    Stack->show();

    gotolinewidget->ui.spinLine->setFocus();
    gotolinewidget->ui.spinLine->setMinimum( 1 );
    gotolinewidget->ui.spinLine->setMaximum( editor->numoflines() );
    gotolinewidget->ui.spinLine->selectAll();
}


void LatexEditorView::updateInSelection()
{
    findwidget->ui.checkSelection->setChecked( false );
}

