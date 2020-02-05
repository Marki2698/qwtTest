#include "mymachine.h"


QList<QwtPickerMachine::Command> MyMachine::transition(const QwtEventPattern &pattern, const QEvent *ev) {
    int machineState = QwtPickerMachine::state();

    switch (ev->type()) {
    case QEvent::MouseButtonPress:
        {
            // for right click
            if (pattern.mouseMatch(QwtEventPattern::MouseSelect2, (const QMouseEvent*)ev)) {
                if (machineState != Command::Begin) commands += Command::Remove;
            }
            // for left click
            else if (pattern.mouseMatch(QwtEventPattern::MouseSelect1, (const QMouseEvent*)ev)) {
                if (machineState == Command::Begin) {
                    commands += Command::Begin;
                    commands += Command::Append;
                    setState(Command::Append);
                }
            }
       }
       break;
    case QEvent::MouseMove:
    case QEvent::Wheel:
    {
        // for left click with ctrl key
        if (machineState != 0 && pattern.mouseMatch(QwtEventPattern::MouseSelect2, (const QMouseEvent*)(ev))) {
            commands += Move;
        }
    }
    break;
    case QEvent::MouseButtonRelease:
        {
            if (machineState != 0) {
                commands += Command::End;
                setState(Command::Begin);
            }
        }
        break;
    default:
        break;
    }
    return commands;
}
