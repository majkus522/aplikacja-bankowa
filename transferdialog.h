#ifndef TRANSFERDIALOG_H
#define TRANSFERDIALOG_H

#include <QDialog>

namespace Ui { class TransferDialog; }

class TransferDialog : public QDialog {
    Q_OBJECT

public:
    explicit TransferDialog(int senderAccountId, double maxBalance, QWidget *parent = nullptr);
    ~TransferDialog();

private slots:
    void on_btnConfirm_clicked();
    void on_btnCancel_clicked();

private:
    Ui::TransferDialog *ui;
    int m_senderAccountId;
    double m_maxBalance;
};

#endif // TRANSFERDIALOG_H