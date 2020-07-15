#ifndef LICENSEDIALOG_H
#define LICENSEDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class LicenseDialog; }
QT_END_NAMESPACE

class LicenseDialog : public QDialog {
    Q_OBJECT
public:
    LicenseDialog(QWidget *parent = nullptr);
    ~LicenseDialog();

    void setText(const QString &text);

private:
    Ui::LicenseDialog *ui;
};

#endif // LICENSEDIALOG_H
