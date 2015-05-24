#include "MainWindow.hpp"
#include "ui_mainwindow.h"
#include "Canvas.hpp"
#include "DrawDialogFactory.hpp"
#include "Circle.hpp"
#include "Rectangle.hpp"
#include "Line.hpp"
#include "SelectionTool.hpp"
#include "DrawCircleTool.hpp"
#include "DrawRectangleTool.hpp"
#include "DrawLineTool.hpp"
#include "PropertyColorButton.hpp"
#include "PropertySpinBox.hpp"
#include "GlobalDrawProperties.hpp"
#include "CommandStack.hpp"
#include "ActiveSelection.hpp"
#include "DrawCommand.hpp"
#include "DeleteSelectedCommand.hpp"
#include "SendBackwardCommand.hpp"
#include "BringForwardCommand.hpp"
#include "BringToFrontCommand.hpp"
#include "SendToBackCommand.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_canvas = new Canvas(this);
    m_gp = &GlobalDrawProperties::getInstance();

    m_selectionTool = std::unique_ptr<SelectionTool>
            (new SelectionTool(m_canvas));
    m_drawCircleTool = std::unique_ptr<DrawCircleTool>
            (new DrawCircleTool(m_canvas));
    m_drawRectangleTool = std::unique_ptr<DrawRectangleTool>
            (new DrawRectangleTool(m_canvas));
    m_drawLineTool = std::unique_ptr<DrawLineTool>
            (new DrawLineTool(m_canvas));

    m_canvas->setActiveTool(m_selectionTool.get());

    PropertyColorButton *fillColorBtn =
            new PropertyColorButton(this, getCanvas(), QColor(200, 200, 200));

    PropertyColorButton *lineColorBtn =
            new PropertyColorButton(this, getCanvas(), QColor(0, 0, 0));

    PropertySpinBox *thicknessSpinBox =
            new PropertySpinBox(this, getCanvas(), 2);

    m_gp->setup(fillColorBtn, lineColorBtn, thicknessSpinBox);

    ui->VEProp->addRow("Fill Color", fillColorBtn);
    ui->VEProp->addRow("Line Color", lineColorBtn);
    ui->VEProp->addRow("Line Thickness", thicknessSpinBox);

    setCentralWidget(m_canvas);
}

Canvas *MainWindow::getCanvas()
{
    return m_canvas;
}

void MainWindow::setActiveTool(Tool *tool)
{
    m_activeTool = tool;
    m_canvas->setActiveTool(tool);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_canvas;
}

void MainWindow::on_actionCircle_triggered()
{
    Circle *c = new Circle();
    m_canvas->addVisualEntity(c);

    DrawDialog *d = DrawDialogFactory::CreateDrawDialog(this, c);
    d->exec();
    delete d;

    ActiveSelection::getInstance().deselectAll();
    c->setSelected(true);

    DrawCommand *comm = new DrawCommand(c);
    comm->addtoCommandStack();
}

void MainWindow::on_actionRectangle_triggered()
{
    Rectangle *r = new Rectangle();
    m_canvas->addVisualEntity(r);

    DrawDialog *d = DrawDialogFactory::CreateDrawDialog(this, r);
    d->exec();
    delete d;

    ActiveSelection::getInstance().deselectAll();
    r->setSelected(true);

    DrawCommand *comm = new DrawCommand(r);
    comm->addtoCommandStack();
}

void MainWindow::on_actionLine_triggered()
{
    Line *l = new Line();
    m_canvas->addVisualEntity(l);

    DrawDialog *d = DrawDialogFactory::CreateDrawDialog(this, l);
    d->exec();
    delete d;

    ActiveSelection::getInstance().deselectAll();
    l->setSelected(true);

    DrawCommand *comm = new DrawCommand(l);
    comm->addtoCommandStack();
}

void MainWindow::on_actionSelectionTool_triggered()
{
    uncheckAllToolbar();
    ui->actionSelectionTool->setChecked(true);
    setActiveTool(m_selectionTool.get());
}

void MainWindow::uncheckAllToolbar()
{
    ui->actionDrawLine->setChecked(false);
    ui->actionDrawCircle->setChecked(false);
    ui->actionDrawRectangle->setChecked(false);
    ui->actionSelectionTool->setChecked(false);
}

void MainWindow::on_actionDrawCircle_triggered()
{
    uncheckAllToolbar();
    ui->actionDrawCircle->setChecked(true);
    setActiveTool(m_drawCircleTool.get());
}

void MainWindow::on_actionDrawRectangle_triggered()
{
    uncheckAllToolbar();
    ui->actionDrawRectangle->setChecked(true);
    setActiveTool(m_drawRectangleTool.get());
}

void MainWindow::on_actionDrawLine_triggered()
{
    uncheckAllToolbar();
    ui->actionDrawLine->setChecked(true);
    setActiveTool(m_drawLineTool.get());
}

void MainWindow::on_actionUndo_triggered()
{
    CommandStack::getInstance().undo();
    m_canvas->repaint();
}

void MainWindow::on_actionRedo_triggered()
{
    CommandStack::getInstance().redo();
    m_canvas->repaint();
}

void MainWindow::on_actionDelete_triggered()
{
    if (ActiveSelection::getInstance().getSize() == 0) return;

    DeleteSelectedCommand *dsc = new DeleteSelectedCommand();
    dsc->execute();
    dsc->addtoCommandStack();

    m_canvas->repaint();
}

void MainWindow::on_actionSend_Backward_triggered()
{
    if (!ChangeOrderCommand::canChangeOrder()) return;

    SendBackwardCommand *comm = new SendBackwardCommand();
    comm->execute();
    comm->addtoCommandStack();

    m_canvas->repaint();
}

void MainWindow::on_actionBring_Forward_triggered()
{
    if (!ChangeOrderCommand::canChangeOrder()) return;

    BringForwardCommand *comm = new BringForwardCommand();
    comm->execute();
    comm->addtoCommandStack();

    m_canvas->repaint();
}

void MainWindow::on_actionBring_To_Front_triggered()
{
    if (!ChangeOrderCommand::canChangeOrder()) return;

    BringToFrontCommand *comm = new BringToFrontCommand();
    comm->execute();
    comm->addtoCommandStack();

    m_canvas->repaint();
}

void MainWindow::on_actionSend_to_Back_triggered()
{
    if (!ChangeOrderCommand::canChangeOrder()) return;

    SendToBackCommand *comm = new SendToBackCommand();
    comm->execute();
    comm->addtoCommandStack();

    m_canvas->repaint();
}
