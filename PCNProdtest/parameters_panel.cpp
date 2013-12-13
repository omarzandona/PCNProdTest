#include "parameters_panel.h"
#include "ui_parameters_panel.h"

ParametersPanel::ParametersPanel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParametersPanel)
{
    ui->setupUi(this);

    connect( this->ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(setDefault(QAbstractButton*)) );
    connect( this->ui->pushButtonCalibrationPath, SIGNAL(clicked()), this, SLOT(changeCalibrationPath()));
    connect( this->ui->pushButtonReportPath, SIGNAL(clicked()), this, SLOT(changeReportPath()));
}

ParametersPanel::~ParametersPanel()
{
    delete ui;
}

void ParametersPanel::setParameters( tParameters param ){
    // General
    this->ui->lineEditCalibrationPath->setText( param.paramGeneral.pathCalibrationDat );
    this->ui->lineEditReportPath->setText( param.paramGeneral.pathReport );
    //this->ui->lineEditSerialTest->setText( param.paramGeneral.serialString );

    // Advanced
    this->ui->doubleSpinBoxFocus->setValue( param.paramCheck.FOCUS_MEASURAMENT );
    this->ui->spinBoxMinNumPixelCentroid->setValue( param.paramCheck.min_num_pixel_for_centroid );
    this->ui->spinBoxNumPixelForNode->setValue( param.paramCheck.min_num_pixel_for_node);
    this->ui->spinBoxMaxRowDelta->setValue( param.paramCheck.max_row_delta );
}

void ParametersPanel::setDefault(QAbstractButton* button){
    tParameters param;
    QDialogButtonBox::StandardButton stdButton = this->ui->buttonBox->standardButton(button);
    switch (stdButton) {
    case QDialogButtonBox::Save:
        // General
        param.paramGeneral.pathCalibrationDat = this->ui->lineEditCalibrationPath->text();
        param.paramGeneral.pathReport = this->ui->lineEditReportPath->text();
        //param.paramGeneral.serialString = this->ui->lineEditSerialTest->text();
        // Advanced
        param.paramCheck.FOCUS_MEASURAMENT = this->ui->doubleSpinBoxFocus->value();
        param.paramCheck.min_num_pixel_for_centroid = this->ui->spinBoxMinNumPixelCentroid->value();
        param.paramCheck.min_num_pixel_for_node = this->ui->spinBoxNumPixelForNode->value();
        param.paramCheck.max_row_delta = this->ui->spinBoxMaxRowDelta->value();
        emit sendParameters( param );
        break;
    case QDialogButtonBox::RestoreDefaults:
        // General
        if ( this->ui->tabWidget->currentIndex() == 0 ){
            this->ui->lineEditCalibrationPath->setText( param.paramGeneral.pathCalibrationDat );
            this->ui->lineEditReportPath->setText( param.paramGeneral.pathReport );
        }else{
            //this->ui->lineEditSerialTest->setText( param.paramGeneral.serialString );

            // Advanced
            this->ui->doubleSpinBoxFocus->setValue( param.paramCheck.FOCUS_MEASURAMENT );
            this->ui->spinBoxMinNumPixelCentroid->setValue( param.paramCheck.min_num_pixel_for_centroid );
            this->ui->spinBoxNumPixelForNode->setValue( param.paramCheck.min_num_pixel_for_node);
            this->ui->spinBoxMaxRowDelta->setValue( param.paramCheck.max_row_delta );
        }
        break;
    }
}

void ParametersPanel::changeCalibrationPath(){
    QString path = QFileDialog::getExistingDirectory(this, "Select the folder to calibration path", this->ui->lineEditCalibrationPath->text());
    if ( !path.isEmpty() )
        this->ui->lineEditCalibrationPath->setText( path );
}

void ParametersPanel::changeReportPath(){
    QString path = QFileDialog::getExistingDirectory(this, "Select the folder to report path", this->ui->lineEditReportPath->text());
    if ( !path.isEmpty() )
        this->ui->lineEditReportPath->setText( path );
}
