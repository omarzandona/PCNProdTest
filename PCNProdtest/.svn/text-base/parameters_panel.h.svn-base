#ifndef PARAMETERS_PANEL_H
#define PARAMETERS_PANEL_H

#include <QDialog>
#include <QFileDialog>
#include <QAbstractButton>
#include "structures.h"

namespace Ui {
class ParametersPanel;
}

class ParametersPanel : public QDialog
{
    Q_OBJECT
    
public:
    explicit ParametersPanel(QWidget *parent = 0);
    ~ParametersPanel();

public slots:
    void setDefault(QAbstractButton* button);
    void setParameters( tParameters param );
    void changeCalibrationPath();
    void changeReportPath();
    
signals:
  void sendParameters( tParameters param );

private:
    Ui::ParametersPanel *ui;
};

#endif // PARAMETERS_PANEL_H
