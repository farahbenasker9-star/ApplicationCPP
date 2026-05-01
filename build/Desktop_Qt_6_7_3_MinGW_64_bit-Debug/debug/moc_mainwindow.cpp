/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../mainwindow.h"
#include <QtNetwork/QSslError>
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSPreviewDialogENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSPreviewDialogENDCLASS = QtMocHelpers::stringData(
    "PreviewDialog"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSPreviewDialogENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

Q_CONSTINIT const QMetaObject PreviewDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_CLASSPreviewDialogENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSPreviewDialogENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSPreviewDialogENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<PreviewDialog, std::true_type>
    >,
    nullptr
} };

void PreviewDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    (void)_o;
    (void)_id;
    (void)_c;
    (void)_a;
}

const QMetaObject *PreviewDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PreviewDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSPreviewDialogENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int PreviewDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    return _id;
}
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSMainWindowENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSMainWindowENDCLASS = QtMocHelpers::stringData(
    "MainWindow",
    "on_btn_nav_employes_clicked",
    "",
    "on_btn_nav_client_clicked",
    "on_btn_nav_contrat_clicked",
    "on_btn_nav_poubelle_clicked",
    "on_btn_nav_equipements_clicked",
    "on_btn_nav_stock_clicked",
    "on_btn_nav_chatbot_clicked",
    "on_btn_dash_poubelle_clicked",
    "on_btn_dash_employe_clicked",
    "on_btn_dash_client_clicked",
    "on_btn_dash_contrat_clicked",
    "on_btn_dash_equipement_clicked",
    "on_btn_dash_stock_clicked",
    "on_btn_ajouter_poubelle_clicked",
    "on_btn_modifier_poubelle_clicked",
    "on_btn_supprimer_poubelle_clicked",
    "on_tab_poubelle_clicked",
    "QModelIndex",
    "index",
    "on_cb_etat_poubelle_currentIndexChanged",
    "on_btn_tri_poubelle_clicked",
    "on_le_recherche_poubelle_textChanged",
    "arg1",
    "on_btn_pdf_poubelle_clicked",
    "on_btn_ajouter_produit_clicked",
    "on_btn_supprimer_produit_clicked",
    "on_btn_modifier_produit_clicked",
    "on_cb_statut_currentIndexChanged",
    "on_tab_produit_clicked",
    "viderFormulaire",
    "on_le_recherche_3_textChanged",
    "on_btn_trier_2_clicked",
    "on_tabWidget_Stock_currentChanged",
    "on_btn_pdf_produit_clicked",
    "refreshHistorique",
    "renderHistorique",
    "typeFiltre",
    "QTextBrowser*",
    "targetFeed",
    "searchKeyword",
    "on_le_search_ajout_textChanged",
    "on_le_search_modif_textChanged",
    "on_le_search_suppr_textChanged",
    "on_le_search_resv_textChanged",
    "on_le_search_vente_textChanged",
    "contrat_onAjouterClicked",
    "contrat_onModifierClicked",
    "contrat_onSupprimerClicked",
    "contrat_onExporterClicked",
    "contrat_onTabClicked",
    "contrat_onTabDoubleClicked",
    "contrat_onRechercheTextChanged",
    "contrat_onTriClicked",
    "contrat_validateID",
    "contrat_validateDates",
    "contrat_validateFloats",
    "contrat_validateDescription",
    "contrat_onGenererClicked",
    "contrat_onImprimerClicked",
    "contrat_onEmailClicked",
    "contrat_updatePreviewClicked",
    "equipement_onAjouterClicked",
    "equipement_onModifierClicked",
    "equipement_onSupprimerClicked",
    "equipement_onExporterClicked",
    "equipement_onTabClicked",
    "equipement_onRechercheTextChanged",
    "equipement_onTriClicked",
    "equipement_onCINChanged",
    "equipement_onStatutChanged",
    "equipement_onNouveauteChanged",
    "equipement_validateType",
    "equipement_validateFabricant",
    "equipement_validateStatut",
    "equipement_validateDateSuivMaint",
    "employe_onAjouterClicked",
    "employe_onModifierClicked",
    "employe_onSupprimerClicked",
    "employe_onTabClicked",
    "employe_onRechercheTextChanged",
    "employe_onTriClicked",
    "employe_onPdfClicked",
    "employe_onRefreshClicked",
    "employe_onGenererContratClicked",
    "handle_rfid_scan",
    "uid",
    "onBtnAjouterClicked",
    "onBtnModifierClicked",
    "onBtnSupprimerClicked",
    "onTableClicked",
    "onRechercheTextChanged",
    "text",
    "rafraichirAffichage"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSMainWindowENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      84,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  518,    2, 0x08,    1 /* Private */,
       3,    0,  519,    2, 0x08,    2 /* Private */,
       4,    0,  520,    2, 0x08,    3 /* Private */,
       5,    0,  521,    2, 0x08,    4 /* Private */,
       6,    0,  522,    2, 0x08,    5 /* Private */,
       7,    0,  523,    2, 0x08,    6 /* Private */,
       8,    0,  524,    2, 0x08,    7 /* Private */,
       9,    0,  525,    2, 0x08,    8 /* Private */,
      10,    0,  526,    2, 0x08,    9 /* Private */,
      11,    0,  527,    2, 0x08,   10 /* Private */,
      12,    0,  528,    2, 0x08,   11 /* Private */,
      13,    0,  529,    2, 0x08,   12 /* Private */,
      14,    0,  530,    2, 0x08,   13 /* Private */,
      15,    0,  531,    2, 0x08,   14 /* Private */,
      16,    0,  532,    2, 0x08,   15 /* Private */,
      17,    0,  533,    2, 0x08,   16 /* Private */,
      18,    1,  534,    2, 0x08,   17 /* Private */,
      21,    1,  537,    2, 0x08,   19 /* Private */,
      22,    0,  540,    2, 0x08,   21 /* Private */,
      23,    1,  541,    2, 0x08,   22 /* Private */,
      25,    0,  544,    2, 0x08,   24 /* Private */,
      26,    0,  545,    2, 0x08,   25 /* Private */,
      27,    0,  546,    2, 0x08,   26 /* Private */,
      28,    0,  547,    2, 0x08,   27 /* Private */,
      29,    1,  548,    2, 0x08,   28 /* Private */,
      30,    1,  551,    2, 0x08,   30 /* Private */,
      31,    0,  554,    2, 0x08,   32 /* Private */,
      32,    1,  555,    2, 0x08,   33 /* Private */,
      33,    0,  558,    2, 0x08,   35 /* Private */,
      34,    1,  559,    2, 0x08,   36 /* Private */,
      35,    0,  562,    2, 0x08,   38 /* Private */,
      36,    0,  563,    2, 0x08,   39 /* Private */,
      37,    3,  564,    2, 0x08,   40 /* Private */,
      42,    1,  571,    2, 0x08,   44 /* Private */,
      43,    1,  574,    2, 0x08,   46 /* Private */,
      44,    1,  577,    2, 0x08,   48 /* Private */,
      45,    1,  580,    2, 0x08,   50 /* Private */,
      46,    1,  583,    2, 0x08,   52 /* Private */,
      47,    0,  586,    2, 0x08,   54 /* Private */,
      48,    0,  587,    2, 0x08,   55 /* Private */,
      49,    0,  588,    2, 0x08,   56 /* Private */,
      50,    0,  589,    2, 0x08,   57 /* Private */,
      51,    1,  590,    2, 0x08,   58 /* Private */,
      52,    1,  593,    2, 0x08,   60 /* Private */,
      53,    1,  596,    2, 0x08,   62 /* Private */,
      54,    0,  599,    2, 0x08,   64 /* Private */,
      55,    0,  600,    2, 0x08,   65 /* Private */,
      56,    0,  601,    2, 0x08,   66 /* Private */,
      57,    0,  602,    2, 0x08,   67 /* Private */,
      58,    0,  603,    2, 0x08,   68 /* Private */,
      59,    0,  604,    2, 0x08,   69 /* Private */,
      60,    0,  605,    2, 0x08,   70 /* Private */,
      61,    0,  606,    2, 0x08,   71 /* Private */,
      62,    0,  607,    2, 0x08,   72 /* Private */,
      63,    0,  608,    2, 0x08,   73 /* Private */,
      64,    0,  609,    2, 0x08,   74 /* Private */,
      65,    0,  610,    2, 0x08,   75 /* Private */,
      66,    0,  611,    2, 0x08,   76 /* Private */,
      67,    1,  612,    2, 0x08,   77 /* Private */,
      68,    1,  615,    2, 0x08,   79 /* Private */,
      69,    0,  618,    2, 0x08,   81 /* Private */,
      70,    1,  619,    2, 0x08,   82 /* Private */,
      71,    1,  622,    2, 0x08,   84 /* Private */,
      72,    1,  625,    2, 0x08,   86 /* Private */,
      73,    0,  628,    2, 0x08,   88 /* Private */,
      74,    0,  629,    2, 0x08,   89 /* Private */,
      75,    0,  630,    2, 0x08,   90 /* Private */,
      76,    0,  631,    2, 0x08,   91 /* Private */,
      77,    0,  632,    2, 0x08,   92 /* Private */,
      78,    0,  633,    2, 0x08,   93 /* Private */,
      79,    0,  634,    2, 0x08,   94 /* Private */,
      80,    1,  635,    2, 0x08,   95 /* Private */,
      81,    1,  638,    2, 0x08,   97 /* Private */,
      82,    0,  641,    2, 0x08,   99 /* Private */,
      83,    0,  642,    2, 0x08,  100 /* Private */,
      84,    0,  643,    2, 0x08,  101 /* Private */,
      85,    0,  644,    2, 0x08,  102 /* Private */,
      86,    1,  645,    2, 0x08,  103 /* Private */,
      88,    0,  648,    2, 0x08,  105 /* Private */,
      89,    0,  649,    2, 0x08,  106 /* Private */,
      90,    0,  650,    2, 0x08,  107 /* Private */,
      91,    1,  651,    2, 0x08,  108 /* Private */,
      92,    1,  654,    2, 0x08,  110 /* Private */,
      94,    0,  657,    2, 0x08,  112 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 19,   20,
    QMetaType::Void, QMetaType::Int,   20,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   24,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   20,
    QMetaType::Void, 0x80000000 | 19,   20,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   24,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   20,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 39, QMetaType::QString,   38,   40,   41,
    QMetaType::Void, QMetaType::QString,   24,
    QMetaType::Void, QMetaType::QString,   24,
    QMetaType::Void, QMetaType::QString,   24,
    QMetaType::Void, QMetaType::QString,   24,
    QMetaType::Void, QMetaType::QString,   24,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 19,   20,
    QMetaType::Void, 0x80000000 | 19,   20,
    QMetaType::Void, QMetaType::QString,   24,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 19,   20,
    QMetaType::Void, QMetaType::QString,   24,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   20,
    QMetaType::Void, QMetaType::Int,   20,
    QMetaType::Void, QMetaType::Int,   20,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 19,   20,
    QMetaType::Void, QMetaType::QString,   24,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   87,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 19,   20,
    QMetaType::Void, QMetaType::QString,   93,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_CLASSMainWindowENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSMainWindowENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSMainWindowENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MainWindow, std::true_type>,
        // method 'on_btn_nav_employes_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_btn_nav_client_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_btn_nav_contrat_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_btn_nav_poubelle_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_btn_nav_equipements_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_btn_nav_stock_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_btn_nav_chatbot_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_btn_dash_poubelle_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_btn_dash_employe_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_btn_dash_client_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_btn_dash_contrat_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_btn_dash_equipement_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_btn_dash_stock_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_btn_ajouter_poubelle_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_btn_modifier_poubelle_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_btn_supprimer_poubelle_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_tab_poubelle_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QModelIndex &, std::false_type>,
        // method 'on_cb_etat_poubelle_currentIndexChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_btn_tri_poubelle_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_le_recherche_poubelle_textChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'on_btn_pdf_poubelle_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_btn_ajouter_produit_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_btn_supprimer_produit_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_btn_modifier_produit_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_cb_statut_currentIndexChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_tab_produit_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QModelIndex &, std::false_type>,
        // method 'viderFormulaire'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_le_recherche_3_textChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'on_btn_trier_2_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_tabWidget_Stock_currentChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_btn_pdf_produit_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'refreshHistorique'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'renderHistorique'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<QTextBrowser *, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'on_le_search_ajout_textChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'on_le_search_modif_textChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'on_le_search_suppr_textChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'on_le_search_resv_textChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'on_le_search_vente_textChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'contrat_onAjouterClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'contrat_onModifierClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'contrat_onSupprimerClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'contrat_onExporterClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'contrat_onTabClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QModelIndex &, std::false_type>,
        // method 'contrat_onTabDoubleClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QModelIndex &, std::false_type>,
        // method 'contrat_onRechercheTextChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'contrat_onTriClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'contrat_validateID'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'contrat_validateDates'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'contrat_validateFloats'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'contrat_validateDescription'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'contrat_onGenererClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'contrat_onImprimerClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'contrat_onEmailClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'contrat_updatePreviewClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'equipement_onAjouterClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'equipement_onModifierClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'equipement_onSupprimerClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'equipement_onExporterClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'equipement_onTabClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QModelIndex &, std::false_type>,
        // method 'equipement_onRechercheTextChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'equipement_onTriClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'equipement_onCINChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'equipement_onStatutChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'equipement_onNouveauteChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'equipement_validateType'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'equipement_validateFabricant'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'equipement_validateStatut'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'equipement_validateDateSuivMaint'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'employe_onAjouterClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'employe_onModifierClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'employe_onSupprimerClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'employe_onTabClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QModelIndex &, std::false_type>,
        // method 'employe_onRechercheTextChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'employe_onTriClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'employe_onPdfClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'employe_onRefreshClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'employe_onGenererContratClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'handle_rfid_scan'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onBtnAjouterClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onBtnModifierClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onBtnSupprimerClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onTableClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QModelIndex &, std::false_type>,
        // method 'onRechercheTextChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'rafraichirAffichage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_btn_nav_employes_clicked(); break;
        case 1: _t->on_btn_nav_client_clicked(); break;
        case 2: _t->on_btn_nav_contrat_clicked(); break;
        case 3: _t->on_btn_nav_poubelle_clicked(); break;
        case 4: _t->on_btn_nav_equipements_clicked(); break;
        case 5: _t->on_btn_nav_stock_clicked(); break;
        case 6: _t->on_btn_nav_chatbot_clicked(); break;
        case 7: _t->on_btn_dash_poubelle_clicked(); break;
        case 8: _t->on_btn_dash_employe_clicked(); break;
        case 9: _t->on_btn_dash_client_clicked(); break;
        case 10: _t->on_btn_dash_contrat_clicked(); break;
        case 11: _t->on_btn_dash_equipement_clicked(); break;
        case 12: _t->on_btn_dash_stock_clicked(); break;
        case 13: _t->on_btn_ajouter_poubelle_clicked(); break;
        case 14: _t->on_btn_modifier_poubelle_clicked(); break;
        case 15: _t->on_btn_supprimer_poubelle_clicked(); break;
        case 16: _t->on_tab_poubelle_clicked((*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 17: _t->on_cb_etat_poubelle_currentIndexChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 18: _t->on_btn_tri_poubelle_clicked(); break;
        case 19: _t->on_le_recherche_poubelle_textChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 20: _t->on_btn_pdf_poubelle_clicked(); break;
        case 21: _t->on_btn_ajouter_produit_clicked(); break;
        case 22: _t->on_btn_supprimer_produit_clicked(); break;
        case 23: _t->on_btn_modifier_produit_clicked(); break;
        case 24: _t->on_cb_statut_currentIndexChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 25: _t->on_tab_produit_clicked((*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 26: _t->viderFormulaire(); break;
        case 27: _t->on_le_recherche_3_textChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 28: _t->on_btn_trier_2_clicked(); break;
        case 29: _t->on_tabWidget_Stock_currentChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 30: _t->on_btn_pdf_produit_clicked(); break;
        case 31: _t->refreshHistorique(); break;
        case 32: _t->renderHistorique((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QTextBrowser*>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3]))); break;
        case 33: _t->on_le_search_ajout_textChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 34: _t->on_le_search_modif_textChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 35: _t->on_le_search_suppr_textChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 36: _t->on_le_search_resv_textChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 37: _t->on_le_search_vente_textChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 38: _t->contrat_onAjouterClicked(); break;
        case 39: _t->contrat_onModifierClicked(); break;
        case 40: _t->contrat_onSupprimerClicked(); break;
        case 41: _t->contrat_onExporterClicked(); break;
        case 42: _t->contrat_onTabClicked((*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 43: _t->contrat_onTabDoubleClicked((*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 44: _t->contrat_onRechercheTextChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 45: _t->contrat_onTriClicked(); break;
        case 46: _t->contrat_validateID(); break;
        case 47: _t->contrat_validateDates(); break;
        case 48: _t->contrat_validateFloats(); break;
        case 49: _t->contrat_validateDescription(); break;
        case 50: _t->contrat_onGenererClicked(); break;
        case 51: _t->contrat_onImprimerClicked(); break;
        case 52: _t->contrat_onEmailClicked(); break;
        case 53: _t->contrat_updatePreviewClicked(); break;
        case 54: _t->equipement_onAjouterClicked(); break;
        case 55: _t->equipement_onModifierClicked(); break;
        case 56: _t->equipement_onSupprimerClicked(); break;
        case 57: _t->equipement_onExporterClicked(); break;
        case 58: _t->equipement_onTabClicked((*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 59: _t->equipement_onRechercheTextChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 60: _t->equipement_onTriClicked(); break;
        case 61: _t->equipement_onCINChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 62: _t->equipement_onStatutChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 63: _t->equipement_onNouveauteChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 64: _t->equipement_validateType(); break;
        case 65: _t->equipement_validateFabricant(); break;
        case 66: _t->equipement_validateStatut(); break;
        case 67: _t->equipement_validateDateSuivMaint(); break;
        case 68: _t->employe_onAjouterClicked(); break;
        case 69: _t->employe_onModifierClicked(); break;
        case 70: _t->employe_onSupprimerClicked(); break;
        case 71: _t->employe_onTabClicked((*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 72: _t->employe_onRechercheTextChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 73: _t->employe_onTriClicked(); break;
        case 74: _t->employe_onPdfClicked(); break;
        case 75: _t->employe_onRefreshClicked(); break;
        case 76: _t->employe_onGenererContratClicked(); break;
        case 77: _t->handle_rfid_scan((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 78: _t->onBtnAjouterClicked(); break;
        case 79: _t->onBtnModifierClicked(); break;
        case 80: _t->onBtnSupprimerClicked(); break;
        case 81: _t->onTableClicked((*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 82: _t->onRechercheTextChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 83: _t->rafraichirAffichage(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 32:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 1:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QTextBrowser* >(); break;
            }
            break;
        }
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSMainWindowENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 84)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 84;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 84)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 84;
    }
    return _id;
}
QT_WARNING_POP
