#include "toolbutton.h"
#include <QStylePainter>
#include <QStyleOptionToolButton>


ToolButton::ToolButton( QWidget* const parent )
    : QToolButton( parent )
{
}


/**
 * See: https://bugreports.qt.io/browse/QTBUG-2036
 */
void ToolButton::paintEvent( QPaintEvent* )
{
    QStylePainter p( this );
    QStyleOptionToolButton opt;
    initStyleOption( &opt );
    opt.features &= (~ QStyleOptionToolButton::HasMenu);
    p.drawComplexControl( QStyle::CC_ToolButton, opt );
}

