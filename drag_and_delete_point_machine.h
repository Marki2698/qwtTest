#ifndef MYMACHINE_H
#define MYMACHINE_H

#include "qwt_picker_machine.h"
#include "qwt_event_pattern.h"

#include <QEvent>

class DragAndDeletePointMachine : public QwtPickerMachine {
public:
    explicit DragAndDeletePointMachine(): QwtPickerMachine(SelectionType::PointSelection), selection(SelectionType::PointSelection) {}
    ~DragAndDeletePointMachine() {};
    QList<Command> transition(const QwtEventPattern& pattern, const QEvent* ev) override;
private:
    SelectionType selection = SelectionType::NoSelection;
    int state = 0;
    QList<Command> commands;
    bool leftWithCTRL = false;
    bool rightClicked = false;
};
#endif // MYMACHINE_H
