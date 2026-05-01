#include "contratgenerator.h"
#include <QTextDocument>
#include <QPrinter>
#include <QPageLayout>
#include <QApplication>
#include <QDesktopServices>
#include <QUrl>
#include <QFile>
#include <QDateTime>
#include <QDebug>

// Charge une image depuis les ressources Qt et la retourne en base64 data URI
static QString imageToBase64(const QString &resourcePath)
{
    QFile f(resourcePath);
    if (!f.open(QIODevice::ReadOnly)) return QString();
    QByteArray data = f.readAll();
    QString ext = resourcePath.split('.').last().toLower();
    return QString("data:image/%1;base64,%2").arg(ext, QString(data.toBase64()));
}

ContratGenerator::ContratGenerator()
{
}

// ═══════════════════════════════════════════════════════════════════════════════
// GÉNÉRATION HTML PRINCIPALE
// ═══════════════════════════════════════════════════════════════════════════════
QString ContratGenerator::generateContractHTML(const ContratData &data, bool forEmail)
{
    QString html = R"(
<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <style>
        * {
            margin: 0;
            padding: 0;
        }
        body {
            font-family: 'Segoe UI', 'Arial', sans-serif;
            line-height: 1.6;
            color: #333;
            background-color: #ffffff;
            padding: 10px;
        }
        .container {
            width: 100%;
            margin: 0;
            background-color: #ffffff;
            padding: 10px;
        }
        .header {
            text-align: center;
            margin-top: 40px;
            margin-bottom: 40px;
            border-bottom: 3px solid #1b5e20;
            padding-bottom: 20px;
        }
        .logo {
            font-size: 28px;
            font-weight: bold;
            color: #1b5e20;
            margin-bottom: 10px;
        }
        .document-title {
            font-size: 24px;
            font-weight: bold;
            color: #1b5e20;
            margin: 20px 0;
            text-transform: uppercase;
        }
        .document-subtitle {
            font-size: 14px;
            color: #666;
            font-style: italic;
        }
        .parties-section {
            margin: 30px 0;
            padding: 20px;
            background-color: #f9f9f9;
            border-left: 4px solid #1b5e20;
        }
        .party {
            margin: 15px 0;
        }
        .party-label {
            font-weight: bold;
            color: #1b5e20;
            margin-bottom: 5px;
        }
        .article {
            margin: 25px 0;
            padding: 15px;
            border-left: 3px solid #2d7c4d;
        }
        .article-title {
            font-weight: bold;
            font-size: 16px;
            color: #1b5e20;
            margin-bottom: 10px;
        }
        .article-content {
            margin-left: 20px;
            line-height: 1.8;
            color: #555;
        }
        .signature-section {
            margin-top: 50px;
            padding-top: 30px;
            border-top: 2px solid #1b5e20;
        }
        .signature-lines {
            display: flex;
            justify-content: space-between;
            margin-top: 30px;
        }
        .signature-box {
            text-align: center;
            width: 45%;
        }
        .signature-line {
            border-top: 1px solid #333;
            margin-top: 70px;
            margin-bottom: 5px;
        }
        .signature-name {
            font-weight: bold;
            font-size: 14px;
        }
        .footer {
            text-align: center;
            margin-top: 40px;
            font-size: 12px;
            color: #999;
            border-top: 1px solid #ddd;
            padding-top: 20px;
        }
        .highlight {
            background-color: #e8f5e9;
            padding: 10px;
            border-radius: 4px;
            margin: 10px 0;
        }
        table {
            width: 100%;
            margin: 15px 0;
            border-collapse: collapse;
        }
        th, td {
            padding: 10px;
            border: 1px solid #ddd;
            text-align: left;
        }
        th {
            background-color: #1b5e20;
            color: white;
        }
    </style>
</head>
<body>
    <div class="container">
        <!-- EN-TÊTE -->
        <div class="header">
            <div style="text-align: center; margin-bottom: 5px;">
                <img src=")";
    if (forEmail) {
        html += "cid:logo";
        html += R"(" alt="Ecocycle Logo" style="height:130px;">
            </div>)";
    } else {
        html += imageToBase64(":/new/prefix1/images/lougo.png");
        html += R"(" alt="Ecocycle Logo" style="height:100px; margin-bottom: -15px;">
            </div>)";
    }
    html += R"(
            <p style="text-align: center; color: #666; font-size: 12px;">Gestion Écologique - Durabilité &amp; Responsabilité</p>
        </div>

        <!-- TITRE DU DOCUMENT -->
        <div style="text-align: center;">
            <div class="document-title">CONTRAT D'EXCLUSIVITÉ</div>
            <div class="document-subtitle">)";
    
    html += "Version: <strong>" + data.type_exclusivite + "</strong> - ";
    html += "Gamme: <strong>" + data.gamme + "</strong>";
    
    html += R"(</div>
            <p style="margin-top: 10px; color: #999; font-size: 12px;">Généré le: )";
    html += QDate::currentDate().toString("dd/MM/yyyy");
    html += " - Contrat ID: " + QString::number(data.id_contrat);
    html += R"(</p>
        </div>

        <!-- PARTIES CONTRACTANTES -->
        <table width="100%" style="border: none; page-break-inside: avoid;"><tr><td style="border: none; padding: 0;">
        <div class="parties-section">
            <div class="document-title" style="font-size: 18px; margin: 0 0 15px 0;">PARTIES CONTRACTANTES</div>
            
            <div class="party">
                <div class="party-label">➤ CLIENT</div>
                <div style="margin-left: 20px;">
                    <p><strong>)";
    html += data.nom_client;
    html += "</strong></p>";
    html += "<p>Adresse: " + (data.adresse_client.isEmpty() ? "N/A" : data.adresse_client) + "</p>";
    html += "<p>T\u00e9l\u00e9phone: " + (data.tel_client.isEmpty() ? "N/A" : data.tel_client) + "</p>";
    if (!data.email_client.isEmpty()) {
        html += "<p>Email: " + data.email_client + "</p>";
    }
    
    html += R"(                </div>
            </div>

            <div class="party">
                <div class="party-label">➤ PRESTATAIRE</div>
                <div style="margin-left: 20px;">
                    <p><strong>Ecocycle</strong></p>
                    <p>Entreprise spécialisée en gestion écologique et recyclage</p>
                    <p>Adresse: Tunis, Tunisie</p>
                    <p>Tél: +216 71 XXX XXX</p>
                </div>
            </div>
        </div>
        </td></tr></table>

        <!-- ARTICLES DU CONTRAT -->
        <div style="margin-top: 40px;">
)";

    // Ajouter les articles selon le type de contrat
    html += generateArticlesForContractType(data);

    html += R"(
        </div>

        <!-- SECTION SIGNATURES -->
        <table width="100%" style="border: none; margin-top: 40px; page-break-inside: avoid;">
            <tr><td style="border: none; padding: 0;">
                <div class="signature-section" style="border-top: 2px solid #1b5e20; padding-top: 20px;">
                    <p style="font-weight: bold; margin-bottom: 20px;">SIGNATURES</p>
                    <p style="font-size: 12px; color: #666; margin-bottom: 30px;">
                        Les parties contractantes reconnaissent avoir lu et accepté les termes et conditions du présent contrat.
                    </p>
                    
                    <table width="100%" class="sig-table" style="border: none; margin-top: 10px;">
                        <tr>
                            <td width="50%" align="center" valign="top" style="border: none; text-align: center;">
                                <span style="font-size:12px; color:#999;">Signature du Client</span><br>
                                <b>)";
    html += data.nom_client;
    html += R"(</b><br><br><br>
                                <hr style="border:1px solid #333; width:60%; margin: 0 auto;">
                            </td>
                            <td width="50%" align="center" valign="top" style="border: none; text-align: center;">
                                <span style="font-size:12px; color:#999;">Représentant Légal</span><br>
                                <b>Ecocycle</b><br>
                                <img src=")";
    if (forEmail) {
        html += "cid:signature";
        html += R"(" alt="Signature" style="height:180px; margin-top: 5px;"><br>)";
    } else {
        html += imageToBase64(":/new/prefix1/images/signa.png");
        html += R"(" alt="Signature" style="height:90px; margin-top: 5px;"><br>)";
    }
    
    html += R"(
                                <hr style="border:1px solid #333; width:60%; margin: 0 auto;">
                            </td>
                        </tr>
                    </table>
                </div>
            </td></tr>
        </table>

        <!-- FOOTER -->
        <div class="footer">
            <p>Ce contrat d'exclusivité est établi en deux exemplaires originaux, conformément à la législation tunisienne.</p>
            <p style="margin-top: 10px; color: #ccc;">
                © 2026 Ecocycle - Tous droits réservés | Document généré automatiquement
            </p>
        </div>
    </div>
</body>
</html>
)";

    return html;
}

// ═══════════════════════════════════════════════════════════════════════════════
// GÉNÉRATION DES ARTICLES SELON LE TYPE
// ═══════════════════════════════════════════════════════════════════════════════
QString ContratGenerator::generateArticlesForContractType(const ContratData &data)
{
    QString articles;
    
    int duree = calculateDuration(data.date_debut, data.date_fin);
    
    // ARTICLE 1: OBJET DU CONTRAT
    articles += R"(
        <div class="article">
            <div class="article-title">Article 1 : OBJET DU CONTRAT</div>
            <div class="article-content">
                <p>)";
    
    if (data.type_exclusivite == "Totale") {
        articles += "Le CLIENT confère au PRESTATAIRE une exclusivité <strong>TOTALE</strong> pour la distribution, la revente "
                   "et la commercialisation des produits EcoCycleApp de la gamme " + data.gamme + " sur son territoire. "
                   "Le CLIENT s'engage à ne pas proposer de produits similaires ou concurrents durant la période du contrat.";
    } else {
        QString typeExCaps = data.type_exclusivite.toUpper();
        articles += "Le CLIENT confère au PRESTATAIRE une exclusivité <strong>" + typeExCaps + "</strong> pour la distribution et la "
                   "commercialisation des produits EcoCycleApp de la gamme " + data.gamme + ". Cette exclusivité ne s'applique "
                   "que aux catégories de produits mentionnées ci-après : " + data.produits_concernes + ".";
    }
    
    articles += R"(</p>
            </div>
        </div>
)";

    // ARTICLE 2: DURÉE DU CONTRAT
    articles += R"(
        <div class="article">
            <div class="article-title">Article 2 : DURÉE ET PÉRIODE DE VALIDITÉ</div>
            <div class="article-content">
                <p>Le présent contrat est établi pour une durée de <strong>)";
    articles += QString::number(duree) + " mois</strong>, à compter du <strong>" + formatDate(data.date_debut) 
              + "</strong> jusqu'au <strong>" + formatDate(data.date_fin) + "</strong>.</p>";
    
    articles += R"(
                <p style="margin-top: 10px;">À l'expiration de ce délai, le contrat sera automatiquement résilié sauf accord 
                express des deux parties pour son renouvellement.</p>
            </div>
        </div>
)";

    // ARTICLE 3: PRODUITS ET GAMMES
    articles += R"(
        <div class="article">
            <div class="article-title">Article 3 : PRODUITS ET GAMMES CONCERNÉS</div>
            <div class="article-content">
                <div class="highlight">
                    <p><strong>Gamme :</strong> )";
    articles += data.gamme + "</p>";
    articles += "<p><strong>Produits concernés :</strong> " + data.produits_concernes + "</p>";
    articles += R"(
                </div>
                <p style="margin-top: 10px;">Le PRESTATAIRE s'engage à maintenir un stock minimum suffisant pour satisfaire 
                la demande du marché et assurer une distribution efficace des produits énumérés ci-dessus.</p>
            </div>
        </div>
)";

    // ARTICLES COMMERCIAUX
    articles += generateCommercialArticles(data);
    
    // ARTICLES D'EXCLUSIVITÉ
    articles += generateExclusivityArticles(data);
    
    // ARTICLES GÉNÉRAUX
    articles += generateGeneralArticles(data);

    return articles;
}

// ═══════════════════════════════════════════════════════════════════════════════
// ARTICLES COMMERCIAUX
// ═══════════════════════════════════════════════════════════════════════════════
QString ContratGenerator::generateCommercialArticles(const ContratData &data)
{
    QString articles;

    // ARTICLE 4: CONDITIONS COMMERCIALES
    articles += R"(
        <div class="article">
            <div class="article-title">Article 4 : CONDITIONS COMMERCIALES ET PRIX</div>
            <div class="article-content">
                <table>
                    <tr>
                        <th>Élément</th>
                        <th>Valeur</th>
                    </tr>
                    <tr>
                        <td>Objectif d'achat annuel</td>
                        <td><strong>)";
    articles += QString::number(data.objectif_achat_annuel, 'f', 2) + " TND</strong></td>";
    articles += R"(
                    </tr>
                    <tr>
                        <td>Taux de remise accordé</td>
                        <td><strong>)";
    articles += QString::number(data.taux_remise_accorde, 'f', 2) + "%</strong></td>";
    articles += R"(
                    </tr>
                    <tr>
                        <td>Statut du contrat</td>
                        <td><strong>)";
    articles += data.statut_contrat + "</strong></td>";
    articles += R"(
                    </tr>
                </table>
                
                <p style="margin-top: 15px;">
                    <strong>Modalités de paiement :</strong> Le client s'engage à respecter un délai de paiement de 30 jours 
                    à compter de la livraison des produits, sauf accord préalable du PRESTATAIRE pour d'autres modalités.
                </p>
            </div>
        </div>
)";

    // ARTICLE 5: FACTURATION ET PAIEMENT
    articles += R"(
        <div class="article">
            <div class="article-title">Article 5 : FACTURATION ET CONDITIONS DE PAIEMENT</div>
            <div class="article-content">
                <ul style="margin-left: 30px; margin-top: 10px;">
                    <li>Les factures seront établies mensuellement ou selon les modalités convenues.</li>
                    <li>Un délai de paiement de <strong>30 jours nets</strong> est accordé à compter de la date de facturation.</li>
                    <li>Les retards de paiement entraîneront des pénalités selon la loi tunisienne en vigueur.</li>
                    <li>Les factures impayées après 60 jours donneront lieu à des actions en recouvrement.</li>
                </ul>
            </div>
        </div>
)";

    return articles;
}

// ═══════════════════════════════════════════════════════════════════════════════
// ARTICLES D'EXCLUSIVITÉ
// ═══════════════════════════════════════════════════════════════════════════════
QString ContratGenerator::generateExclusivityArticles(const ContratData &data)
{
    QString articles;

    articles += R"(
        <div class="article">
            <div class="article-title">Article 6 : CONDITIONS D'EXCLUSIVITÉ</div>
            <div class="article-content">
)";

    if (data.type_exclusivite == "Totale") {
        articles += R"(
                <p><strong>EXCLUSIVITÉ TOTALE :</strong></p>
                <ul style="margin-left: 30px; margin-top: 10px;">
                    <li>Le CLIENT devient le distributeur exclusif et unique pour la gamme spécifiée.</li>
                    <li>Le CLIENT s'interdit formellement de proposer des produits concurrents ou similaires sans l'accord écrit préalable du PRESTATAIRE.</li>
                    <li>Le CLIENT accepte que le territoire du contrat soit réservé exclusivement à sa distribution.</li>
                    <li>Toute violation de cette clause entrainera l'application de pénalités et la possible résiliation du contrat.</li>
                </ul>
)";
    } else {
        articles += R"(
                <p><strong>EXCLUSIVITÉ PARTIELLE :</strong></p>
                <ul style="margin-left: 30px; margin-top: 10px;">
                    <li>L'exclusivité s'applique uniquement aux catégories de produits mentionnées à l'Article 3.</li>
                    <li>Le CLIENT peut commercialiser des produits complémentaires ou non-concurrents.</li>
                    <li>Le PRESTATAIRE conserve le droit de distribuer d'autres produits EcoCycleApp via d'autres canaux.</li>
                    <li>Cette exclusivité ne s'applique pas à la vente directe du PRESTATAIRE.</li>
                </ul>
)";
    }

    articles += R"(
            </div>
        </div>

        <div class="article">
            <div class="article-title">Article 7 : OBLIGATIONS DU CLIENT</div>
            <div class="article-content">
                <p>Le CLIENT s'engage à :</p>
                <ul style="margin-left: 30px; margin-top: 10px;">
                    <li>Maintenir un niveau de ventes conforme aux objectifs d'achat annuel définis à l'Article 4.</li>
                    <li>Assurer une bonne présentation et promotion des produits EcoCycleApp.</li>
                    <li>Fournir des rapports mensuels sur les ventes et la distribution.</li>
                    <li>Respecter les normes de qualité et les standards EcoCycleApp.</li>
                    <li>Protéger la marque et l'image de EcoCycleApp sur son territoire.</li>
                </ul>
            </div>
        </div>

        <div class="article">
            <div class="article-title">Article 8 : OBLIGATIONS DU PRESTATAIRE (ECOCYCLEAPP)</div>
            <div class="article-content">
                <p>Le PRESTATAIRE s'engage à :</p>
                <ul style="margin-left: 30px; margin-top: 10px;">
                    <li>Fournir les produits dans les délais convenus.</li>
                    <li>Assurer la qualité et la conformité des produits livrés.</li>
                    <li>Fournir un support marketing et commercial au CLIENT.</li>
                    <li>Accorder la remise commerciale définie à l'Article 4.</li>
                    <li>Garantir l'exclusivité convenue dans le domaine territorial défini.</li>
                </ul>
            </div>
        </div>
)";

    return articles;
}

// ═══════════════════════════════════════════════════════════════════════════════
// ARTICLES GÉNÉRAUX
// ═══════════════════════════════════════════════════════════════════════════════
QString ContratGenerator::generateGeneralArticles(const ContratData &data)
{
    QString articles;

    articles += R"(
        <div class="article">
            <div class="article-title">Article 9 : RÉSILIATION ET RÉSILIATION ANTICIPÉE</div>
            <div class="article-content">
                <p><strong>Résiliation par défaut :</strong></p>
                <ul style="margin-left: 30px; margin-top: 10px;">
                    <li>Non-respect des objectifs de vente pendant 3 mois consécutifs.</li>
                    <li>Non-paiement des factures après 60 jours de retard.</li>
                    <li>Violation des conditions d'exclusivité.</li>
                    <li>Insolvabilité ou faillite du CLIENT.</li>
                </ul>
                <p style="margin-top: 15px;"><strong>Préavis de résiliation :</strong> Un préavis écrit de 30 jours est nécessaire.</p>
            </div>
        </div>

        <div class="article">
            <div class="article-title">Article 10 : CONFIDENTIALITÉ ET DONNÉES</div>
            <div class="article-content">
                <p>Les deux parties s'engagent à :</p>
                <ul style="margin-left: 30px; margin-top: 10px;">
                    <li>Traiter toutes les informations commerciales de manière confidentielle.</li>
                    <li>Ne pas divulguer les prix, remises ou conditions sans autorisation.</li>
                    <li>Se conformer à la loi tunisienne sur la protection des données.</li>
                </ul>
            </div>
        </div>

        <div class="article">
            <div class="article-title">Article 11 : CLAUSES PÉNALES</div>
            <div class="article-content">
                <div class="highlight">
                    <p>)";
    
    if (data.clause_penale.isEmpty()) {
        articles += "Les pénalités applicables en cas de manquement seront définies selon les usages commerciaux tunisiens.";
    } else {
        articles += data.clause_penale;
    }
    
    articles += R"(</p>
                </div>
            </div>
        </div>

        <div class="article">
            <div class="article-title">Article 12 : JURIDICTION ET LÉGISLATION</div>
            <div class="article-content">
                <p>Le présent contrat est soumis à la législation tunisienne. Tout litige découlant de ce contrat sera de la 
                compétence exclusive des tribunaux compétents de Tunis.</p>
            </div>
        </div>

        <div class="article">
            <div class="article-title">Article 13 : MODIFICATIONS ET ADDENDUMS</div>
            <div class="article-content">
                <p>Toute modification des termes du présent contrat devra faire l'objet d'un addendum écrit signé par les 
                deux parties contractantes.</p>
            </div>
        </div>
)";

    return articles;
}

// ═══════════════════════════════════════════════════════════════════════════════
// EXPORT PDF
// ═══════════════════════════════════════════════════════════════════════════════
bool ContratGenerator::exportToPDF(const ContratData &data, const QString &filePath)
{
    QString html = generateContractHTML(data);

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageMargins(QMarginsF(15, 15, 15, 15), QPageLayout::Millimeter);

    QTextDocument document;
    // Use Point units (1/72 inch): A4 = ~595×842 pt — avoids microscopic text
    document.setPageSize(printer.pageRect(QPrinter::Point).size());
    document.setHtml(html);
    document.print(&printer);

    qDebug() << "PDF exporté:" << filePath;
    return true;
}


// ═══════════════════════════════════════════════════════════════════════════════
// UTILITAIRES
// ═══════════════════════════════════════════════════════════════════════════════
QString ContratGenerator::formatDate(const QDate &date)
{
    QStringList months = {"", "janvier", "février", "mars", "avril", "mai", "juin",
                          "juillet", "août", "septembre", "octobre", "novembre", "décembre"};
    return QString::number(date.day()) + " " + months[date.month()] + " " + QString::number(date.year());
}

int ContratGenerator::calculateDuration(const QDate &debut, const QDate &fin)
{
    return debut.daysTo(fin) / 30; // Approximation en mois
}
