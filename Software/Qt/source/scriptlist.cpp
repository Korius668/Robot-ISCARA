#include "scriptlist.h"
#include "ui_scriptlist.h"

ScriptList::ScriptList(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ScriptList)
{
    ui->setupUi(this);
    this->linesAdded = 0;

    connect(ui->pushButton, &QPushButton::clicked, this, &ScriptList::renameButton);
    //connect(ui->editor, &QPlainTextEdit::textChanged, this, &ScriptList::numeratorUpdate);
    connect(ui->editor, &QPlainTextEdit::cursorPositionChanged, this, &ScriptList::numeratorUpdate);

    numeratorUpdate();
}

ScriptList::~ScriptList()
{
    delete ui;
}

void ScriptList::numeratorUpdate()
{
    // CHECK BOUNDARIES
    if ( ui->editor->document()->blockCount() > MAX_LINES ) { deleteRedundantLines(); }

    // UPDATE NUMBER OF NUMERATOR POSITIONS
    if ( ui->editor->document()->blockCount() != linesAdded )
    {
        if ( ui->editor->document()->blockCount() > linesAdded )
            addLines();
        else
            deleteLines();
    }

    // UPDATE CURSOR POSITION
    cursorAdjust();
}

void ScriptList::addLines()
{
    QString number, text;
    while ( ui->editor->document()->blockCount() != linesAdded )
    {
        number = NULL;
        text = NULL;
        ui->numerator->moveCursor(QTextCursor::End);
        if ( ui->numerator->document()->blockCount() == linesAdded ) { ui->numerator->insertPlainText( "\n" ); }
        number = QString::number( ui->numerator->document()->blockCount() );
        for (unsigned int i = 0; i < 4 - number.size() * 2 ; i++) { text.append(" "); }
        text.append( number );
        text.append( " . " );
        ui->numerator->insertPlainText( text );
        linesAdded++;
    }
}

void ScriptList::deleteLines()
{
    while ( ui->editor->document()->blockCount() != linesAdded )
    {
        ui->numerator->moveCursor( QTextCursor::End);
        ui->numerator->moveCursor( QTextCursor::StartOfBlock, QTextCursor::KeepAnchor );
        ui->numerator->textCursor().removeSelectedText();
        ui->numerator->textCursor().deletePreviousChar();
        linesAdded--;
    }
}

void ScriptList::deleteRedundantLines()
{
    while ( ui->editor->document()->blockCount() > MAX_LINES )
    {
        ui->editor->moveCursor(QTextCursor::End);
        ui->editor->moveCursor(QTextCursor::StartOfBlock, QTextCursor::KeepAnchor);
        ui->editor->textCursor().removeSelectedText();
        ui->editor->textCursor().deletePreviousChar();
    }
}

void ScriptList::cursorAdjust()
{
    std::cout << ui->editor->textCursor().blockNumber() << std::endl;
    std::cout << ui->numerator->textCursor().blockNumber() << std::endl;

    QTextCursor newCursor = ui->numerator->textCursor();

    while ( ui->editor->textCursor().blockNumber() != newCursor.blockNumber() )
    {

    if ( ui->editor->textCursor().blockNumber() > newCursor.blockNumber() )
    {
        newCursor.movePosition(QTextCursor::NextBlock);
        std::cout << "Next: " << newCursor.blockNumber() << std::endl;
    }
    else if ( ui->editor->textCursor().blockNumber() < newCursor.blockNumber() )
        {
        newCursor.movePosition(QTextCursor::PreviousBlock);
        std::cout << "Prev: " << newCursor.blockNumber() << std::endl;
        }
    newCursor.movePosition(QTextCursor::EndOfBlock);
    }

    ui->numerator->setTextCursor( newCursor );

    std::cout << ui->editor->textCursor().blockNumber() << std::endl;
    std::cout << ui->numerator->textCursor().blockNumber() << "\n" << std::endl;

}

void ScriptList::renameButton()
{

}

