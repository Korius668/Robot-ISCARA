#ifndef SCRIPTLIST_H
#define SCRIPTLIST_H

#define MAX_LINES 99

#include <QDialog>
#include <iostream>


namespace Ui {
class ScriptList;
}

class ScriptList : public QDialog
{
    Q_OBJECT

public:
    explicit ScriptList(QWidget *parent = nullptr);
    ~ScriptList();

private:
    Ui::ScriptList *ui;

    unsigned int linesAdded;

    void addLines();
    void deleteLines();
    void deleteRedundantLines();

private slots:
    void cursorAdjust();
    void numeratorUpdate();
    void renameButton();

};

#endif // SCRIPTLIST_H
