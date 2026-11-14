#include "preferences_window_misc.hpp"
#include "util/gtk_util.hpp"
#include "preferences.hpp"
#include "preferences_row.hpp"

namespace dune3d {

MiscPreferencesEditor::MiscPreferencesEditor(Preferences &prefs) : m_preferences(prefs)
{
    set_policy(Gtk::PolicyType::NEVER, Gtk::PolicyType::AUTOMATIC);
    auto box = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL, 20);
    box->set_halign(Gtk::Align::CENTER);
    box->show();
    box->set_margin(24);
    set_child(*box);

    {
        auto gr = Gtk::make_managed<PreferencesGroup>("Vue 3D");
        box->append(*gr);
        {
            auto r = Gtk::make_managed<PreferencesRowBool>(
                    "Activer les animations",
                    "Utiliser un modèle masse-ressort-amortisseur pour adoucir le zoom et les autres transitions comme la rotation",
                    m_preferences, m_preferences.canvas.enable_animations);
            gr->add_row(*r);
        }
        {
            auto r = Gtk::make_managed<PreferencesRowBool>(
                    "Faire briller les éléments sélectionnés", "Afficher un halo autour des éléments sélectionnés pour mieux les voir",
                    m_preferences, m_preferences.canvas.appearance.selection_glow);
            gr->add_row(*r);
        }
        {
            auto r = Gtk::make_managed<PreferencesRowBool>(
                    "Activer le calque d'erreur",
                    "Teinter la vue en rouge s'il y a des contraintes redondantes ou si l'esquisse est surcontrainte",
                    m_preferences, m_preferences.canvas.error_overlay);
            gr->add_row(*r);
        }
        {
            auto r = Gtk::make_managed<PreferencesRowBool>(
                    "Relier le peigne de courbure", "Tracer une ligne reliant les dents voisines du peigne de courbure",
                    m_preferences, m_preferences.canvas.connect_curvature_combs);
            gr->add_row(*r);
        }
        {
            auto r = Gtk::make_managed<PreferencesRowBoolButton>("Centre du zoom", "Où zoomer avec la souris ou le pavé tactile",
                                                                 "Curseur", "Centre de l'écran", m_preferences,
                                                                 m_preferences.canvas.zoom_to_cursor);
            gr->add_row(*r);
        }
        {
            static const std::vector<std::pair<RotationScheme, std::string>> rotation_schemes = {
                    {RotationScheme::DEFAULT, "Culbuteur"},
                    {RotationScheme::LEGACY, "Plateau tournant"},
                    {RotationScheme::ARCBALL, "Boule de commande"},
            };
            auto r = Gtk::make_managed<PreferencesRowEnum<RotationScheme>>(
                    "Mode de rotation", "Comment convertir le mouvement de la souris en rotation", m_preferences,
                    m_preferences.canvas.rotation_scheme, rotation_schemes);
            r->bind();
            gr->add_row(*r);
        }
    }
    {
        auto gr = Gtk::make_managed<PreferencesGroup>("Éditeur");
        box->append(*gr);
        {
            auto r = Gtk::make_managed<PreferencesRowBool>(
                    "Aperçu des contraintes", "Survoler les contraintes dans le menu contextuel pour prévisualiser leur effet",
                    m_preferences, m_preferences.editor.preview_constraints);
            gr->add_row(*r);
        }
        {
            auto r = Gtk::make_managed<PreferencesRowNumeric<int>>(
                    "Arrondi d'affichage des contraintes", "Nombre maximal de décimales affichées dans les valeurs de contrainte",
                    m_preferences, m_preferences.editor.constraint_value_rounding);
            r->get_spinbutton().set_range(0, 5);
            r->get_spinbutton().set_increments(1, 1);
            r->get_spinbutton().set_digits(0);
            r->bind();
            gr->add_row(*r);
        }
        {
            auto r = Gtk::make_managed<PreferencesRowEnum<EditorPreferences::TrailingZeros>>(
                    "Zéros de fin", "Gérer l'affichage des zéros de fin dans les valeurs de contrainte", m_preferences,
                    m_preferences.editor.constraint_trailing_zeros,
                    std::vector<std::pair<EditorPreferences::TrailingZeros, std::string>>{
                            {EditorPreferences::TrailingZeros::OFF, "Désactivé"},
                            {EditorPreferences::TrailingZeros::ONE_DECIMAL, "Garder 1 décimale"},
                            {EditorPreferences::TrailingZeros::ON, "Activé"},
                    });
            r->bind();
            gr->add_row(*r);
        }
    }
    {
        auto gr = Gtk::make_managed<PreferencesGroup>("Barre d'actions");
        box->append(*gr);
        gr->show();
        {
            auto r = Gtk::make_managed<PreferencesRowBool>(
                    "Utiliser la barre d'actions", "Afficher la barre d'actions pour accéder rapidement aux outils courants", m_preferences,
                    m_preferences.action_bar.enable);
            gr->add_row(*r);
        }
        /* {
             auto r = Gtk::manage(new PreferencesRowBool("Mémoriser la dernière action",
                                                         "Afficher dans le bouton la dernière action utilisée plutôt que celle par défaut",
                                                         preferences, preferences.action_bar.remember));
             gr->add_row(*r);
         }*/
        {
            auto r = Gtk::make_managed<PreferencesRowBool>("Toujours afficher", "Afficher aussi la barre d'actions quand un outil est actif",
                                                           m_preferences, m_preferences.action_bar.show_in_tool);
            gr->add_row(*r);
        }
    }
    {
        auto gr = Gtk::make_managed<PreferencesGroup>("Barre d'outils");
        box->append(*gr);
        {
            auto r = Gtk::make_managed<PreferencesRowBool>(
                    "Disposition verticale", "Afficher l'aide sur une ligne séparée plutôt qu'à droite des touches d'action",
                    m_preferences, m_preferences.tool_bar.vertical_layout);
            gr->add_row(*r);
        }
    }
    {
        auto gr = Gtk::make_managed<PreferencesGroup>("Apparence");
        box->append(*gr);
        {
            auto r = Gtk::make_managed<PreferencesRowBool>("Thème sombre", "Utiliser la variante sombre du thème si disponible",
                                                           m_preferences, m_preferences.canvas.dark_theme);
            gr->add_row(*r);
        }
    }
}

} // namespace dune3d
