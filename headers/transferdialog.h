#pragma once

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class TransferDialog;
}
QT_END_NAMESPACE

class TransferDialog : public QDialog
{
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