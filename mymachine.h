#ifndef MYMACHINE_H
#define MYMACHINE_H

#include "qwt_picker_machine.h"
#include "qwt_event_pattern.h"

#include <QEvent>

class MyMachine : public QwtPickerMachine {
public:
    explicit MyMachine(): QwtPickerMachine(SelectionType::PointSelection), selection(SelectionType::PointSelection) {}
    ~MyMachine() {};
    QList<Command> transition(const QwtEventPattern& pattern, const QEvent* ev) override;
private:
    SelectionType selection = SelectionType::NoSelection;
    int state = 0;
    QList<Command> commands;
};
#endif // MYMACHINE_H
