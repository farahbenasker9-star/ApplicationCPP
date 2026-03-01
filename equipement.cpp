#include "equipement.h"
#include "ui_mainwindow.h"

// ─── Constructor ──────────────────────────────────────────────────────────────
Equipement::Equipement(Ui::MainWindow *ui, QWidget *parent)
    : QObject(parent),
    ui(ui),
    parentWidget(parent),
    model(new QSqlQueryModel(this)),
    selectedEquipmentId(-1),
    sortAscending(true),           // ← add
    sortColumn("EQUIPMENT_ID")     // ← add
{
    // ── 1. Create error indicator labels dynamically ──────────────────────────
    //    Each label sits to the right of its field inside infoScrollContent.
    //    We parent them to infoScrollContent so they move with the scroll area.
    //
    //    Field geometries (from .ui):
    //      Eq_CIN          x=160  y=30   w=141  h=26
    //      Eq_type         x=160  y=70   w=117  h=40
    //      Eq_Fab          x=160  y=120  w=121  h=40
    //      Eq_Statut       x=160  y=170  w=151  h=40
    //      Eq_Next_Maint   x=160  y=270  w=161  h=37
    // ─────────────────────────────────────────────────────────────────────────

    QWidget *fc = formContainer();

    auto makeErr = [&](int x, int y) -> QLabel* {
        QLabel *lbl = new QLabel("✕", fc);
        lbl->setGeometry(x, y, 22, 22);
        lbl->setStyleSheet(
            "QLabel { color: #D32F2F; font-weight: bold; font-size: 14px;"
            "background: transparent; border: none; }");
        lbl->setVisible(false);
        return lbl;
    };

    // Place each ✕ label just to the right of its widget
    errCIN      = makeErr(160 + 141 + 4,  33);
    errType     = makeErr(160 + 117 + 4,  76);
    errFab      = makeErr(160 + 121 + 4,  126);
    errStatut   = makeErr(160 + 151 + 4,  176);
    errNextMaint= makeErr(160 + 161 + 4,  275);

    QString dropFix =
        "QComboBox QAbstractItemView {"
        "    background-color: white;"
        "    color: #222222;"
        "    border: 1px solid #BDBDBD;"
        "    selection-background-color: #4CAF50;"
        "    selection-color: white;"
        "    outline: 0;"
        "}";

    ui->Eq_CIN->setStyleSheet(dropFix);
    ui->Eq_type->setStyleSheet(dropFix);
    ui->Eq_Fab->setStyleSheet(dropFix);
    ui->Eq_Statut->setStyleSheet(dropFix);

    QString viewStyle = "background-color: white; color: #222222;";

    ui->Eq_CIN->view()->setStyleSheet(viewStyle);
    ui->Eq_type->view()->setStyleSheet(viewStyle);
    ui->Eq_Fab->view()->setStyleSheet(viewStyle);
    ui->Eq_Statut->view()->setStyleSheet(viewStyle);

    // ── 2. Populate CIN combo box from DB ────────────────────────────────────
    chargerCIN();

    // ── 3. Load the table ────────────────────────────────────────────────────
    rafraichirTable();

    // ── 4. Connect buttons ───────────────────────────────────────────────────
    connect(ui->btn_ajouter_2,  &QPushButton::clicked, this, &Equipement::onAjouter);
    connect(ui->btn_modifier_2, &QPushButton::clicked, this, &Equipement::onModifier);
    connect(ui->btn_supprimer_2,&QPushButton::clicked, this, &Equipement::onSupprimer);

    connect(ui->tab_employes_2, &QTableView::clicked,
            this, &Equipement::onRowSelected);

    // Search / tri
    connect(ui->btn_tri_3, &QPushButton::clicked,
            this, &Equipement::onSort);
    connect(ui->le_recherche_2, &QLineEdit::textChanged,
            this, &Equipement::onSearch);

    // Real-time validation connections
    connect(ui->Eq_CIN,    qOverload<int>(&QComboBox::currentIndexChanged),
            this, &Equipement::onCINChanged);
    connect(ui->Eq_Statut, qOverload<int>(&QComboBox::currentIndexChanged),
            this, &Equipement::onStatutChanged);
}

// ─── Destructor ───────────────────────────────────────────────────────────────
Equipement::~Equipement() {}

// ─── formContainer ────────────────────────────────────────────────────────────
// Returns the QWidget named "infoScrollContent" which is the absolute-positioned
// parent of all form widgets.
QWidget* Equipement::formContainer()
{
    // infoScrollArea is inside infoGroupBox_5 on page_equipement
    QWidget *scrollContent = ui->infoScrollArea->widget();
    return scrollContent ? scrollContent : parentWidget;
}

// ─── chargerCIN ──────────────────────────────────────────────────────────────
// Queries the EMPLOYE table and fills Eq_CIN with "NOM PRENOM (CIN)" entries.
// A blank "-" entry at index 0 represents "no employee assigned".
void Equipement::chargerCIN()
{
    ui->Eq_CIN->blockSignals(true);
    ui->Eq_CIN->clear();
    ui->Eq_CIN->addItem("-", QVariant());   // index 0 → no CIN selected

    QSqlQuery q;
    q.prepare("SELECT CIN, NOM, PRENOM FROM EMPLOYE ORDER BY NOM, PRENOM");
    if (q.exec()) {
        while (q.next()) {
            QString cin    = q.value(0).toString();
            QString nom    = q.value(1).toString().trimmed();
            QString prenom = q.value(2).toString().trimmed();
            ui->Eq_CIN->addItem(cin, cin);
        }
    } else {
        qDebug() << "chargerCIN error:" << q.lastError().text();
    }

    ui->Eq_CIN->blockSignals(false);
}

// ─── clearForm ───────────────────────────────────────────────────────────────
void Equipement::clearForm()
{
    ui->Eq_CIN->setCurrentIndex(0);
    ui->Eq_type->setCurrentIndex(0);
    ui->Eq_Fab->setCurrentIndex(0);
    ui->Eq_Statut->setCurrentIndex(0);

    QDateTime now = QDateTime::currentDateTime();
    ui->Eq_Last_Maint_Date->setDateTime(now);
    ui->Eq_Next_Maint_Date->setDateTime(now);
    ui->Notes->clear();

    selectedEquipmentId = -1;
    resetValidationStyles();
}

// ─── afficher ────────────────────────────────────────────────────────────────
QSqlQueryModel* Equipement::afficher()
{
    model->setQuery(
        "SELECT EQUIPMENT_ID AS \"ID\", "
        "       E.NOM || ' ' || E.PRENOM AS \"Employé\", "
        "       EQ.EQUIPMENT_TYPE AS \"Type\", "
        "       EQ.FABRICANT AS \"Fabricant\", "
        "       EQ.STATUT AS \"Statut\", "
        "       TO_CHAR(EQ.DATE_DER_MAINT, 'DD/MM/YYYY') AS \"Dern. Maint.\", "
        "       TO_CHAR(EQ.DATE_SUIV_MAINT,'DD/MM/YYYY') AS \"Proch. Maint.\", "
        "       EQ.NOTES AS \"Notes\" "
        "FROM   EQUIPMENT EQ "
        "LEFT JOIN EMPLOYE E ON EQ.CIN = E.CIN "
        "ORDER BY EQUIPMENT_ID"
        );

    if (model->lastError().isValid())
        qDebug() << "afficher error:" << model->lastError().text();

    return model;
}

// ─── rafraichirTable ─────────────────────────────────────────────────────────
void Equipement::rafraichirTable()
{
    ui->tab_employes_2->setModel(afficher());
    ui->tab_employes_2->resizeColumnsToContents();
    ui->tab_employes_2->horizontalHeader()->setStretchLastSection(true);
}

// ═════════════════════════════════════════════════════════════════════════════
//  VALIDATION
// ═════════════════════════════════════════════════════════════════════════════

// ─── setFieldError ───────────────────────────────────────────────────────────
void Equipement::setFieldError(QWidget *field, QLabel *errorLabel,
                               bool hasError, const QString &msg)
{
    Q_UNUSED(msg)
    if (hasError) {
        field->setStyleSheet(
            "border: 2px solid #D32F2F; border-radius: 5px; "
            "background-color: rgba(211,47,47,15);");
        errorLabel->setVisible(true);
    } else {
        field->setStyleSheet(
            "border: 2px solid #43A047; border-radius: 5px; "
            "background-color: rgba(67,160,71,15);");
        errorLabel->setVisible(false);
    }
}

// ─── resetValidationStyles ───────────────────────────────────────────────────
void Equipement::resetValidationStyles()
{
    const QString neutral = "";   // Let the stylesheet from .ui take over
    ui->Eq_CIN->setStyleSheet(neutral);
    ui->Eq_type->setStyleSheet(neutral);
    ui->Eq_Fab->setStyleSheet(neutral);
    ui->Eq_Statut->setStyleSheet(neutral);
    ui->Eq_Next_Maint_Date->setStyleSheet(neutral);

    errCIN->setVisible(false);
    errType->setVisible(false);
    errFab->setVisible(false);
    errStatut->setVisible(false);
    errNextMaint->setVisible(false);
}

// ─── validerFormulaire ───────────────────────────────────────────────────────
bool Equipement::validerFormulaire()
{
    bool ok = true;

    // ── Rule 1 : Type must not be "-" ────────────────────────────────────────
    bool typeErr = (ui->Eq_type->currentIndex() == 0);
    setFieldError(ui->Eq_type, errType, typeErr);
    if (typeErr) ok = false;

    // ── Rule 2 : Fabricant must not be "-" ───────────────────────────────────
    bool fabErr = (ui->Eq_Fab->currentIndex() == 0);
    setFieldError(ui->Eq_Fab, errFab, fabErr);
    if (fabErr) ok = false;

    // ── Rule 3 : Statut must not be "-" ──────────────────────────────────────
    bool statutErr = (ui->Eq_Statut->currentIndex() == 0);
    setFieldError(ui->Eq_Statut, errStatut, statutErr);
    if (statutErr) ok = false;

    // ── Rule 4 : CIN / Statut logical coherence ───────────────────────────────
    //    • CIN not chosen  → statut MUST be "Disponible"
    //    • CIN chosen      → statut MUST NOT be "Disponible"
    bool cinSelected = (ui->Eq_CIN->currentIndex() != 0);
    QString statut   = ui->Eq_Statut->currentText();

    if (!statutErr) {           // only check coherence if a statut was chosen
        if (!cinSelected && statut != "Disponible") {
            // No employee, but marked as unavailable/in maintenance → invalid
            setFieldError(ui->Eq_CIN,    errCIN,    true);
            setFieldError(ui->Eq_Statut, errStatut, true);
            errStatut->setToolTip("Sans employé assigné, le statut doit être 'Disponible'.");
            ok = false;
        } else if (cinSelected && statut == "Disponible") {
            // Employee assigned, but marked as available → illogical
            setFieldError(ui->Eq_CIN,    errCIN,    true);
            setFieldError(ui->Eq_Statut, errStatut, true);
            errCIN->setToolTip("Un équipement assigné à un employé ne peut pas être 'Disponible'.");
            ok = false;
        } else {
            // CIN coherence is fine
            if (!cinSelected) {
                // CIN combo stays neutral (no explicit error, no success style)
                ui->Eq_CIN->setStyleSheet("");
                errCIN->setVisible(false);
            } else {
                setFieldError(ui->Eq_CIN, errCIN, false);
            }
        }
    }

    // ── Rule 5 : Next maintenance date must be >= today ───────────────────────
    QDate today      = QDate::currentDate();
    QDate nextMaint  = ui->Eq_Next_Maint_Date->date();
    bool nextMaintErr = (nextMaint < today);
    setFieldError(ui->Eq_Next_Maint_Date, errNextMaint, nextMaintErr);
    if (nextMaintErr) {
        errNextMaint->setToolTip("La date de prochaine maintenance doit être supérieure ou égale à aujourd'hui.");
        ok = false;
    }

    return ok;
}

// ═════════════════════════════════════════════════════════════════════════════
//  SLOTS – Real-time feedback
// ═════════════════════════════════════════════════════════════════════════════

// ─── onCINChanged ────────────────────────────────────────────────────────────
// When CIN selection changes, suggest the correct default statut and re-validate.
void Equipement::onCINChanged(int index)
{
    // Auto-suggest statut to guide the user (only if statut is still "-")
    if (ui->Eq_Statut->currentIndex() == 0) {
        if (index == 0) {
            // No CIN → suggest Disponible
            ui->Eq_Statut->setCurrentText("Disponible");
        } else {
            // CIN chosen → suggest Non Disponible
            ui->Eq_Statut->setCurrentText("Non Disponible");
        }
    }
    // Re-run validation to update visual feedback
    validerFormulaire();
}

// ─── onStatutChanged ─────────────────────────────────────────────────────────
void Equipement::onStatutChanged(int /*index*/)
{
    validerFormulaire();
}

// ═════════════════════════════════════════════════════════════════════════════
//  SLOTS – CRUD
// ═════════════════════════════════════════════════════════════════════════════

// ─── onAjouter ───────────────────────────────────────────────────────────────
void Equipement::onAjouter()
{
    if (!validerFormulaire()) {
        QMessageBox::warning(parentWidget, "Saisie invalide",
                             "Veuillez corriger les champs en rouge avant de continuer.");
        return;
    }

    if (!ajouter()) {
        QMessageBox::critical(parentWidget, "Erreur DB",
                              "L'ajout de l'équipement a échoué.");
        return;
    }

    QMessageBox::information(parentWidget, "Succès",
                             "Équipement ajouté avec succès !");
    clearForm();
    rafraichirTable();
}

// ─── onModifier ──────────────────────────────────────────────────────────────
void Equipement::onModifier()
{
    if (selectedEquipmentId == -1) {
        QMessageBox::warning(parentWidget, "Aucune sélection",
                             "Veuillez sélectionner un équipement dans le tableau.");
        return;
    }

    if (!validerFormulaire()) {
        QMessageBox::warning(parentWidget, "Saisie invalide",
                             "Veuillez corriger les champs en rouge avant de continuer.");
        return;
    }

    if (!modifier()) {
        QMessageBox::critical(parentWidget, "Erreur DB",
                              "La modification a échoué.");
        return;
    }

    QMessageBox::information(parentWidget, "Succès",
                             "Équipement modifié avec succès !");
    clearForm();
    rafraichirTable();
}

// ─── onSupprimer ─────────────────────────────────────────────────────────────
void Equipement::onSupprimer()
{
    if (selectedEquipmentId == -1) {
        QMessageBox::warning(parentWidget, "Aucune sélection",
                             "Veuillez sélectionner un équipement à supprimer.");
        return;
    }

    int rep = QMessageBox::question(parentWidget, "Confirmer suppression",
                                    QString("Supprimer l'équipement ID %1 ?")
                                        .arg(selectedEquipmentId),
                                    QMessageBox::Yes | QMessageBox::No);
    if (rep != QMessageBox::Yes) return;

    QSqlQuery q;
    q.prepare("DELETE FROM EQUIPMENT WHERE EQUIPMENT_ID = :id");
    q.bindValue(":id", selectedEquipmentId);

    if (!q.exec()) {
        QMessageBox::critical(parentWidget, "Erreur DB", q.lastError().text());
        return;
    }

    QMessageBox::information(parentWidget, "Succès",
                             "Équipement supprimé avec succès !");
    clearForm();
    rafraichirTable();
}

// ─── onRowSelected ───────────────────────────────────────────────────────────
// Fills the form with data from the selected row.
void Equipement::onRowSelected(const QModelIndex &index)
{
    if (!index.isValid()) return;

    int row = index.row();

    // Column 0 → EQUIPMENT_ID
    selectedEquipmentId = model->index(row, 0).data().toInt();

    // Fetch full record from DB for exact values
    QSqlQuery q;
    q.prepare(
        "SELECT CIN, EQUIPMENT_TYPE, FABRICANT, STATUT, "
        "       DATE_DER_MAINT, DATE_SUIV_MAINT, NOTES "
        "FROM EQUIPMENT WHERE EQUIPMENT_ID = :id"
        );
    q.bindValue(":id", selectedEquipmentId);

    if (!q.exec() || !q.next()) {
        qDebug() << "onRowSelected error:" << q.lastError().text();
        return;
    }

    // CIN – find the combo entry whose userData matches the CIN
    QString cinDB = q.value(0).toString();
    int cinIdx = 0;
    if (!cinDB.isEmpty()) {
        for (int i = 1; i < ui->Eq_CIN->count(); ++i) {
            if (ui->Eq_CIN->itemData(i).toString() == cinDB) {
                cinIdx = i;
                break;
            }
        }
    }
    ui->Eq_CIN->blockSignals(true);
    ui->Eq_CIN->setCurrentIndex(cinIdx);
    ui->Eq_CIN->blockSignals(false);

    ui->Eq_type->setCurrentText(q.value(1).toString().trimmed());
    ui->Eq_Fab->setCurrentText(q.value(2).toString().trimmed());
    ui->Eq_Statut->setCurrentText(q.value(3).toString().trimmed());

    QDateTime lastMaint = q.value(4).toDateTime();
    QDateTime nextMaint = q.value(5).toDateTime();
    if (lastMaint.isValid()) ui->Eq_Last_Maint_Date->setDateTime(lastMaint);
    if (nextMaint.isValid()) ui->Eq_Next_Maint_Date->setDateTime(nextMaint);

    ui->Notes->setPlainText(q.value(6).toString());

    resetValidationStyles();
}

// ─── onSearch ────────────────────────────────────────────────────────────────
// Filters the table live as the user types. Respects the current sort order.
void Equipement::onSearch()
{
    QString text = ui->le_recherche_2->text().trimmed();

    QString whereClause = "";
    if (!text.isEmpty()) {
        whereClause = QString(
                          "WHERE UPPER(EQ.FABRICANT)       LIKE UPPER('%%%1%%') "
                          "OR    UPPER(EQ.EQUIPMENT_TYPE)  LIKE UPPER('%%%1%%') "
                          "OR    UPPER(EQ.STATUT)          LIKE UPPER('%%%1%%') "
                          "OR    UPPER(E.NOM)              LIKE UPPER('%%%1%%') "
                          "OR    UPPER(E.PRENOM)           LIKE UPPER('%%%1%%') "
                          ).arg(text);
    }

    QString direction = sortAscending ? "ASC" : "DESC";

    model->setQuery(QString(
                        "SELECT EQUIPMENT_ID                AS \"ID\", "
                        "       E.NOM || ' ' || E.PRENOM   AS \"Employé\", "
                        "       EQ.EQUIPMENT_TYPE          AS \"Type\", "
                        "       EQ.FABRICANT               AS \"Fabricant\", "
                        "       EQ.STATUT                  AS \"Statut\", "
                        "       TO_CHAR(EQ.DATE_DER_MAINT,  'DD/MM/YYYY') AS \"Dern. Maint.\", "
                        "       TO_CHAR(EQ.DATE_SUIV_MAINT, 'DD/MM/YYYY') AS \"Proch. Maint.\", "
                        "       EQ.NOTES                   AS \"Notes\" "
                        "FROM   EQUIPMENT EQ "
                        "LEFT JOIN EMPLOYE E ON EQ.CIN = E.CIN "
                        "%1 "
                        "ORDER BY %2 %3"
                        ).arg(whereClause, sortColumn, direction));

    ui->tab_employes_2->setModel(model);
    ui->tab_employes_2->resizeColumnsToContents();
    ui->tab_employes_2->horizontalHeader()->setStretchLastSection(true);
}

// ─── onSort ──────────────────────────────────────────────────────────────────
// Clicking "Trier" cycles ASC → DESC → ASC on the selected column.
// Updates the button text to show current direction.
void Equipement::onSort()
{
    QString champ = ui->comboBox_2->currentText();

    if      (champ == "Fabricant") sortColumn = "EQ.FABRICANT";
    else if (champ == "Statut")    sortColumn = "EQ.STATUT";
    else                           sortColumn = "EQ.EQUIPMENT_TYPE";

    // Toggle direction
    sortAscending = !sortAscending;

    // Update button label so the user knows which direction is active
    ui->btn_tri_3->setText(sortAscending ? "Trier ↑" : "Trier ↓");

    // Re-run the query (respects any active search text too)
    onSearch();
}

// ═════════════════════════════════════════════════════════════════════════════
//  DATABASE CRUD HELPERS
// ═════════════════════════════════════════════════════════════════════════════

// ─── ajouter ─────────────────────────────────────────────────────────────────
bool Equipement::ajouter()
{
    // Build CIN value (NULL if not selected)
    QVariant cinVal;
    if (ui->Eq_CIN->currentIndex() != 0)
        cinVal = ui->Eq_CIN->currentData();   // raw CIN stored as userData

    QSqlQuery q;
    q.prepare(
        "INSERT INTO EQUIPMENT "
        "  (EQUIPMENT_ID, CIN, EQUIPMENT_TYPE, FABRICANT, STATUT, "
        "   DATE_DER_MAINT, DATE_SUIV_MAINT, NOTES) "
        "VALUES "
        "  (SEQ_EQUIPMENT.NEXTVAL, :cin, :type, :fab, :statut, "
        "   :last, :next, :notes)"
        );

    q.bindValue(":cin",    cinVal.isNull() ? QVariant(QVariant::Int) : cinVal);
    q.bindValue(":type",   ui->Eq_type->currentText());
    q.bindValue(":fab",    ui->Eq_Fab->currentText());
    q.bindValue(":statut", ui->Eq_Statut->currentText());
    q.bindValue(":last",   ui->Eq_Last_Maint_Date->dateTime());
    q.bindValue(":next",   ui->Eq_Next_Maint_Date->dateTime());
    q.bindValue(":notes",  ui->Notes->toPlainText());

    if (!q.exec()) {
        qDebug() << "ajouter error:" << q.lastError().text();
        return false;
    }
    return true;
}

// ─── modifier ────────────────────────────────────────────────────────────────
bool Equipement::modifier()
{
    QVariant cinVal;
    if (ui->Eq_CIN->currentIndex() != 0)
        cinVal = ui->Eq_CIN->currentData();

    QSqlQuery q;
    q.prepare(
        "UPDATE EQUIPMENT SET "
        "  CIN            = :cin, "
        "  EQUIPMENT_TYPE = :type, "
        "  FABRICANT      = :fab, "
        "  STATUT         = :statut, "
        "  DATE_DER_MAINT = :last, "
        "  DATE_SUIV_MAINT= :next, "
        "  NOTES          = :notes "
        "WHERE EQUIPMENT_ID = :id"
        );

    q.bindValue(":cin",    cinVal.isNull() ? QVariant(QVariant::Int) : cinVal);
    q.bindValue(":type",   ui->Eq_type->currentText());
    q.bindValue(":fab",    ui->Eq_Fab->currentText());
    q.bindValue(":statut", ui->Eq_Statut->currentText());
    q.bindValue(":last",   ui->Eq_Last_Maint_Date->dateTime());
    q.bindValue(":next",   ui->Eq_Next_Maint_Date->dateTime());
    q.bindValue(":notes",  ui->Notes->toPlainText());
    q.bindValue(":id",     selectedEquipmentId);

    if (!q.exec()) {
        qDebug() << "modifier error:" << q.lastError().text();
        return false;
    }
    return true;
}

// ─── supprimer ───────────────────────────────────────────────────────────────
// (actual DELETE call is in onSupprimer for the confirmation dialog;
//  this stub is kept for direct programmatic use)
bool Equipement::supprimer()
{
    if (selectedEquipmentId == -1) return false;

    QSqlQuery q;
    q.prepare("DELETE FROM EQUIPMENT WHERE EQUIPMENT_ID = :id");
    q.bindValue(":id", selectedEquipmentId);

    if (!q.exec()) {
        qDebug() << "supprimer error:" << q.lastError().text();
        return false;
    }
    return true;
}
