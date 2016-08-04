#ifndef TOOLBUTTON_H
#define TOOLBUTTON_H


#include <QToolButton>


/**
 * QToolButton, which doesn't show an arrow, when it has a dropdown menu assigned.
 *
 * This is the expected behaviour of tool buttons in Elementary OS.
 */
class ToolButton : public QToolButton
{

public:

    ToolButton( QWidget* const parent = 0 );

protected:

    virtual void paintEvent( QPaintEvent* );

};


#endif

