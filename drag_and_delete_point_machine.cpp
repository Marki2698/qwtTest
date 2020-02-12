#include "drag_and_delete_point_machine.h"
#include <iostream>
#include <QDebug>

QList<QwtPickerMachine::Command> DragAndDeletePointMachine::transition(
    const QwtEventPattern &eventPattern, const QEvent *event )
{
    QList<QwtPickerMachine::Command> cmdList;

    switch ( event->type() )
    {
        case QEvent::MouseButtonPress:
        {
            if ( eventPattern.mouseMatch( QwtEventPattern::MouseSelect5,
                (const QMouseEvent *)( event ) ) )
            {
                if (QwtPickerMachine::state() == 0) {
                    cmdList += Begin;
                    cmdList += Append;
                    leftWithCTRL = true;
                    setState(1);
                }
            } else
                if ( eventPattern.mouseMatch( QwtEventPattern::MouseSelect2,
                    (const QMouseEvent *)( event ) ) )
                {
                    rightClicked = true;
                    if ( QwtPickerMachine::state() == 0 )
                    {
                        cmdList += Begin;
                        cmdList += Append;
                        cmdList += Remove;

                        setState( 1 );
                    }
                } else
            if ( eventPattern.mouseMatch( QwtEventPattern::MouseSelect1,
                (const QMouseEvent *)( event ) ) )
            {
                if ( QwtPickerMachine::state() == 0 )
                {
                    cmdList += Begin;
                    cmdList += Append;
                    cmdList += Move;
                    setState( 1 );
                }
            }
            break;
        }
        case QEvent::MouseMove:
        case QEvent::Wheel:
        {
            if (leftWithCTRL && QwtPickerMachine::state() != 0) {
                cmdList += Move;
            }
            break;
        }
        case QEvent::MouseButtonRelease:
        {
            if ( QwtPickerMachine::state() != 0 )
            {
                if (rightClicked) cmdList += Remove;
                cmdList += End;
                setState( 0 );
                leftWithCTRL = false;
                rightClicked = false;
            }
            break;
        }
        default:
            break;
    }

    return cmdList;
}

