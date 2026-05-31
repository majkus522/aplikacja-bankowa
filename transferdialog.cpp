#include "transferdialog.h"
#include "ui_transferdialog.h"
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QMessageBox>

TransferDialog::TransferDialog(int senderAccountId, double maxBalance, QWidget *parent) 
    : QDialog(parent), ui(new Ui::TransferDialog), m_senderAccountId(senderAccountId), m_maxBalance(maxBalance) 
{
    ui->setupUi(this);
    setWindowTitle("Nowy przelew");

    // Konfiguracja limitu kwoty na podstawie aktualnego stanu konta
    ui->spinAmount->setMinimum(0.01);
    ui->spinAmount->setMaximum(m_maxBalance);
    ui->spinAmount->setSingleStep(10.0);
}

TransferDialog::~TransferDialog() {
    delete ui;
}

void TransferDialog::on_btnConfirm_clicked() {
    QString targetAccNum = ui->editTarget->text().trimmed();
    QString transferTitle = ui->editTitle->text().trimmed();
    double amount = ui->spinAmount->value();

    if (targetAccNum.length() != 26) {
        QMessageBox::warning(this, "Błąd", "Numer konta musi mieć 26 cyfr!");
        return;
    }
    if (transferTitle.isEmpty()) {
        transferTitle = "Przelew bankowy";
    }

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.transaction()) return;

    // 1. Szukanie odbiorcy
    QSqlQuery query;
    query.prepare("SELECT id FROM accounts WHERE account_number = :targetNum");
    query.bindValue(":targetNum", targetAccNum);
    
    if (!query.exec() || !query.next()) {
        QMessageBox::warning(this, "Błąd", "Numer konta odbiorcy nie istnieje!");
        db.rollback();
        return;
    }
    int targetAccountId = query.value(0).toInt();

    if (targetAccountId == m_senderAccountId) {
        QMessageBox::warning(this, "Błąd", "Nie możesz przelać środków na to samo konto!");
        db.rollback();
        return;
    }

    // 2. Pobranie z konta nadawcy
    QSqlQuery deductQuery;
    deductQuery.prepare("UPDATE accounts SET balance = balance - :amount WHERE id = :senderId");
    deductQuery.bindValue(":amount", amount);
    deductQuery.bindValue(":senderId", m_senderAccountId);
    if (!deductQuery.exec()) { db.rollback(); return; }

    // 3. Depozyt u odbiorcy
    QSqlQuery addQuery;
    addQuery.prepare("UPDATE accounts SET balance = balance + :amount WHERE id = :targetId");
    addQuery.bindValue(":amount", amount);
    addQuery.bindValue(":targetId", targetAccountId);
    if (!addQuery.exec()) { db.rollback(); return; }

    // 4. Logowanie historii
    QSqlQuery logQuery;
    logQuery.prepare("INSERT INTO transactions (sender_account_id, receiver_account_id, amount, title) "
                     "VALUES (:sender, :receiver, :amount, :title)");
    logQuery.bindValue(":sender", m_senderAccountId);
    logQuery.bindValue(":receiver", targetAccountId);
    logQuery.bindValue(":amount", amount);
    logQuery.bindValue(":title", transferTitle);
    
    if (logQuery.exec() && db.commit()) {
        QMessageBox::information(this, "Sukces", "Przelew wysłany!");
        accept(); // Zamyka dialog z kodem sukcesu (QDialog::Accepted)
    } else {
        db.rollback();
        QMessageBox::critical(this, "Błąd", "Transakcja odrzucona.");
    }
}

void TransferDialog::on_btnCancel_clicked() {
    reject(); // Zamyka dialog z kodem anulowania (QDialog::Rejected)
}