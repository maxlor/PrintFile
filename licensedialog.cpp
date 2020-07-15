#include "licensedialog.h"
#include "ui_licensedialog.h"


LicenseDialog::LicenseDialog(QWidget *parent) : QDialog(parent), ui(new Ui::LicenseDialog) {
    ui->setupUi(this);
}


LicenseDialog::~LicenseDialog() {
    delete ui;
}


void LicenseDialog::setText(const QString &text) {
    ui->textEdit->setText(text);
}


