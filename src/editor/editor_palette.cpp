// Barre d'outils à icônes (palette) et propositions d'outils contextuelles
#include "editor.hpp"
#include "dune3d_appwindow.hpp"
#include "core/tool_id.hpp"
#include "action/action_id.hpp"
#include "action/action.hpp"
#include "action/action_catalog.hpp"
#include "canvas/canvas.hpp"
#include "document/document.hpp"
#include "document/entity/entity.hpp"
#include "document/group/group.hpp"
#include "util/action_label.hpp"
#include "in_tool_action/in_tool_action.hpp"
#include "in_tool_action/in_tool_action_catalog.hpp"
#include "core/tool.hpp"

namespace dune3d {

namespace {
struct PaletteItem {
    ActionToolID id;
    const char *icon;
    const char *label; // libellé court pour les propositions
};

struct PaletteSection {
    const char *title;
    std::vector<PaletteItem> items;
};

const std::vector<PaletteSection> &get_palette()
{
    static const std::vector<PaletteSection> palette = {
            {"Fichier",
             {
                     {ActionID::SAVE, "d3-save", "Enregistrer"},
                     {ActionID::EXPORT_SOLID_MODEL_STL, "d3-export_stl", "Exporter STL"},
                     {ActionID::EXPORT_SOLID_MODEL_OBJ, "d3-export_stl", "Exporter OBJ"},
                     {ActionID::EXPORT_SOLID_MODEL_STEP, "d3-export_stl", "Exporter STEP"},
             }},
            {"Dessin",
             {
                     {ToolID::DRAW_CONTOUR, "d3-contour", "Contour"},
                     {ToolID::DRAW_LINE_2D, "d3-line2d", "Ligne"},
                     {ToolID::DRAW_RECTANGLE, "d3-rectangle", "Rectangle"},
                     {ToolID::DRAW_CIRCLE_2D, "d3-circle", "Cercle"},
                     {ToolID::DRAW_ARC_2D, "d3-arc", "Arc"},
                     {ToolID::DRAW_REGULAR_POLYGON, "d3-polygon", "Polygone"},
                     {ToolID::DRAW_BEZIER_2D, "d3-bezier", "Bézier"},
                     {ToolID::DRAW_POINT_2D, "d3-point", "Point"},
                     {ToolID::DRAW_TEXT, "d3-text", "Texte"},
                     {ToolID::DRAW_LINE_3D, "d3-line3d", "Ligne 3D"},
                     {ToolID::DRAW_WORKPLANE, "d3-workplane", "Plan de travail"},
                     {ToolID::IMPORT_PICTURE, "d3-picture", "Image"},
             }},
            {"Édition",
             {
                     {ActionID::UNDO, "d3-undo", "Annuler"},
                     {ActionID::REDO, "d3-redo", "Rétablir"},
                     {ToolID::DELETE, "d3-delete", "Supprimer"},
                     {ToolID::MOVE, "d3-move", "Déplacer"},
                     {ToolID::ROTATE, "d3-rotate", "Pivoter"},
                     {ToolID::TOGGLE_CONSTRUCTION, "d3-construction", "Construction"},
             }},
            {"Contraintes",
             {
                     {ActionID::CONSTRAIN_MENU, "d3-constrain_menu", "Contraindre…"},
                     {ToolID::CONSTRAIN_DISTANCE, "d3-distance", "Distance"},
                     {ToolID::CONSTRAIN_DISTANCE_HORIZONTAL, "d3-dist_h", "Dist. horizontale"},
                     {ToolID::CONSTRAIN_DISTANCE_VERTICAL, "d3-dist_v", "Dist. verticale"},
                     {ToolID::CONSTRAIN_DIAMETER, "d3-diameter", "Diamètre"},
                     {ToolID::CONSTRAIN_RADIUS, "d3-radius", "Rayon"},
                     {ToolID::CONSTRAIN_ANGLE, "d3-angle", "Angle"},
                     {ToolID::CONSTRAIN_HORIZONTAL, "d3-horizontal", "Horizontale"},
                     {ToolID::CONSTRAIN_VERTICAL, "d3-vertical", "Verticale"},
                     {ToolID::CONSTRAIN_PARALLEL, "d3-parallel", "Parallèles"},
                     {ToolID::CONSTRAIN_PERPENDICULAR, "d3-perpendicular", "Perpendiculaires"},
                     {ToolID::CONSTRAIN_POINT_ON_POINT, "d3-coincident", "Point sur point"},
                     {ToolID::CONSTRAIN_POINT_ON_LINE, "d3-point_on_line", "Point sur ligne"},
                     {ToolID::CONSTRAIN_POINT_ON_CIRCLE, "d3-point_on_circle", "Point sur cercle"},
                     {ToolID::CONSTRAIN_MIDPOINT, "d3-midpoint", "Milieu"},
                     {ToolID::CONSTRAIN_SYMMETRIC_LINE, "d3-symmetric", "Symétrie"},
                     {ToolID::CONSTRAIN_EQUAL_LENGTH, "d3-equal", "Longueurs égales"},
                     {ToolID::CONSTRAIN_EQUAL_RADIUS, "d3-equal_radius", "Rayons égaux"},
                     {ToolID::CONSTRAIN_ARC_LINE_TANGENT, "d3-tangent", "Tangence"},
                     {ToolID::MEASURE_DISTANCE, "d3-measure", "Mesurer"},
                     {ActionID::SELECT_UNDERCONSTRAINED, "d3-underconstrained", "Éléments libres"},
             }},
            {"Groupes",
             {
                     {ActionID::CREATE_GROUP_SKETCH, "d3-grp_sketch", "Esquisse"},
                     {ActionID::CREATE_GROUP_EXTRUDE, "d3-extrude", "Extrusion"},
                     {ActionID::CREATE_GROUP_LATHE, "d3-lathe", "Tournage"},
                     {ActionID::CREATE_GROUP_REVOLVE, "d3-revolve", "Révolution"},
                     {ActionID::CREATE_GROUP_FILLET, "d3-fillet", "Congé"},
                     {ActionID::CREATE_GROUP_CHAMFER, "d3-chamfer", "Chanfrein"},
                     {ActionID::CREATE_GROUP_LINEAR_ARRAY, "d3-linear_array", "Réseau linéaire"},
                     {ActionID::CREATE_GROUP_POLAR_ARRAY, "d3-polar_array", "Réseau circulaire"},
                     {ActionID::CREATE_GROUP_MIRROR_HORIZONTAL, "d3-mirror_h", "Symétrie horiz."},
                     {ActionID::CREATE_GROUP_MIRROR_VERTICAL, "d3-mirror_v", "Symétrie vert."},
                     {ActionID::CREATE_GROUP_SOLID_MODEL_OPERATION, "d3-solid_op", "Opération"},
                     {ActionID::CREATE_GROUP_PIPE, "d3-pipe", "Balayage"},
                     {ActionID::CREATE_GROUP_CLONE, "d3-clone", "Clone"},
             }},
            {"Vue",
             {
                     {ActionID::VIEW_ALL, "d3-view_all", "Vue de départ"},
                     {ActionID::VIEW_TOGGLE_PERSP_ORTHO, "d3-persp_ortho", "Persp./ortho"},
                     {ActionID::ALIGN_VIEW_TO_CURRENT_WORKPLANE, "d3-align_wp", "Face au plan"},
                     {ActionID::VIEW_FRONT, "d3-view_front", "Face"},
                     {ActionID::VIEW_TOP, "d3-view_top", "Dessus"},
                     {ActionID::VIEW_RIGHT, "d3-view_right", "Droite"},
                     {ActionID::TOGGLE_SOLID_MODEL, "d3-toggle_solid", "Volume"},
             }},
    };
    return palette;
}

const std::vector<PaletteItem> &get_extra_items()
{
    static const std::vector<PaletteItem> items = {
            {ToolID::CONSTRAIN_DISTANCE_ALIGNED, "d3-distance", "Dist. alignée"},
            {ToolID::CONSTRAIN_DISTANCE_3D, "d3-distance", "Distance 3D"},
            {ToolID::CONSTRAIN_POINT_LINE_DISTANCE, "d3-distance", "Dist. point/ligne"},
            {ToolID::CONSTRAIN_POINT_PLANE_DISTANCE, "d3-distance", "Dist. point/plan"},
            {ToolID::CONSTRAIN_HORIZONTAL_AUTO, "d3-horizontal", "Horizontale"},
            {ToolID::CONSTRAIN_VERTICAL_AUTO, "d3-vertical", "Verticale"},
            {ToolID::CONSTRAIN_SYMMETRIC_HORIZONTAL, "d3-symmetric", "Sym. horizontale"},
            {ToolID::CONSTRAIN_SYMMETRIC_VERTICAL, "d3-symmetric", "Sym. verticale"},
            {ToolID::CONSTRAIN_CURVE_CURVE_TANGENT, "d3-tangent", "Tangence"},
            {ToolID::CONSTRAIN_BEZIER_LINE_TANGENT, "d3-tangent", "Tangence"},
            {ToolID::CONSTRAIN_POINT_ON_BEZIER, "d3-point_on_line", "Point sur courbe"},
            {ToolID::CONSTRAIN_POINT_IN_WORKPLANE, "d3-point_on_line", "Point dans le plan"},
            {ToolID::CONSTRAIN_POINT_IN_PLANE, "d3-point_on_line", "Point dans le plan"},
            {ToolID::CONSTRAIN_LINE_POINTS_PERPENDICULAR, "d3-perpendicular", "Perpendiculaire"},
            {ToolID::CONSTRAIN_SAME_ORIENTATION, "d3-parallel", "Même orientation"},
            {ToolID::CONSTRAIN_WORKPLANE_NORMAL, "d3-workplane", "Normale du plan"},
            {ToolID::CONSTRAIN_LOCK_ROTATION, "d3-rotate", "Bloquer la rotation"},
            {ToolID::MEASURE_DISTANCE_HORIZONTAL, "d3-measure", "Mesurer"},
            {ToolID::MEASURE_DISTANCE_VERTICAL, "d3-measure", "Mesurer"},
            {ToolID::MEASURE_DISTANCE_ALIGNED, "d3-measure", "Mesurer"},
            {ToolID::MEASURE_DIAMETER, "d3-measure", "Mesurer Ø"},
            {ToolID::MEASURE_RADIUS, "d3-measure", "Mesurer R"},
            {ToolID::MEASURE_ANGLE, "d3-measure", "Mesurer l'angle"},
            {ToolID::MEASURE_POINT_LINE_DISTANCE, "d3-measure", "Mesurer"},
            {ToolID::ENTER_DATUM, "d3-enter_datum", "Modifier la cote"},
    };
    return items;
}

const PaletteItem *find_palette_item(ActionToolID id)
{
    for (const auto &sec : get_palette()) {
        for (const auto &it : sec.items) {
            if (it.id == id)
                return &it;
        }
    }
    for (const auto &it : get_extra_items()) {
        if (it.id == id)
            return &it;
    }
    return nullptr;
}

bool is_draw_tool(ToolID id)
{
    switch (id) {
    case ToolID::DRAW_CONTOUR:
    case ToolID::DRAW_LINE_2D:
    case ToolID::DRAW_RECTANGLE:
    case ToolID::DRAW_CIRCLE_2D:
    case ToolID::DRAW_ARC_2D:
    case ToolID::DRAW_REGULAR_POLYGON:
    case ToolID::DRAW_BEZIER_2D:
    case ToolID::DRAW_POINT_2D:
    case ToolID::DRAW_TEXT:
    case ToolID::DRAW_LINE_3D:
        return true;
    default:
        return false;
    }
}
} // namespace

std::string Editor::get_palette_tooltip(ActionToolID id) const
{
    std::string tip = action_catalog.at(id).name.full;
    if (m_action_connections.contains(id)) {
        const auto keys = key_sequences_to_string(m_action_connections.at(id).key_sequences);
        if (keys.size())
            tip += "  (" + keys + ")";
    }
    return tip;
}

bool Editor::palette_item_can_begin(ActionToolID id, const std::set<SelectableRef> &sel)
{
    if (!m_core.has_documents())
        return false;
    if (!m_action_connections.contains(id))
        return false;
    if (auto tool = std::get_if<ToolID>(&id))
        return m_core.tool_can_begin(*tool, sel).get_can_begin();
    else
        return get_action_sensitive(std::get<ActionID>(id));
}

void Editor::palette_trigger(ActionToolID id)
{
    if (!force_end_tool())
        return;
    // Le menu « Contraindre » s'ouvre normalement sous la souris : on le place au centre de la vue
    if (id == ActionToolID{ActionID::CONSTRAIN_MENU}) {
        m_last_x = get_canvas().get_width() / 2;
        m_last_y = get_canvas().get_height() / 3;
    }
    trigger_action(id);
}

void Editor::create_tool_palette()
{
    auto outer = Gtk::make_managed<Gtk::ScrolledWindow>();
    outer->set_policy(Gtk::PolicyType::NEVER, Gtk::PolicyType::AUTOMATIC);
    outer->set_vexpand(true);
    outer->add_css_class("dune3d-palette");

    auto vbox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL, 2);
    vbox->set_margin_start(4);
    vbox->set_margin_end(4);
    vbox->set_margin_top(4);
    vbox->set_margin_bottom(4);

    for (const auto &sec : get_palette()) {
        auto title = Gtk::make_managed<Gtk::Label>(sec.title);
        title->set_xalign(0);
        title->add_css_class("dune3d-palette-title");
        vbox->append(*title);

        auto grid = Gtk::make_managed<Gtk::Grid>();
        grid->set_column_spacing(1);
        grid->set_row_spacing(1);
        int i = 0;
        for (const auto &it : sec.items) {
            if (!m_action_connections.contains(it.id))
                continue;
            auto bu = Gtk::make_managed<Gtk::Button>();
            auto img = Gtk::make_managed<Gtk::Image>();
            img->set_from_icon_name(it.icon);
            img->set_pixel_size(22);
            bu->set_child(*img);
            bu->set_has_frame(false);
            bu->add_css_class("dune3d-palette-button");
            bu->set_tooltip_text(get_palette_tooltip(it.id));
            const auto id = it.id;
            bu->signal_clicked().connect([this, id] { palette_trigger(id); });
            grid->attach(*bu, i % 3, i / 3);
            m_palette_buttons.emplace(id, bu);
            i++;
        }
        vbox->append(*grid);
    }
    outer->set_child(*vbox);
    m_palette = outer;
    m_win.add_tool_palette(*outer);

    // Barre de propositions, en haut de la vue 3D
    m_suggestion_revealer = Gtk::make_managed<Gtk::Revealer>();
    m_suggestion_revealer->set_transition_type(Gtk::RevealerTransitionType::SLIDE_DOWN);
    m_suggestion_revealer->set_halign(Gtk::Align::CENTER);
    m_suggestion_revealer->set_valign(Gtk::Align::START);
    m_suggestion_revealer->set_margin_top(8);
    auto sbox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL, 4);
    sbox->add_css_class("osd");
    sbox->add_css_class("dune3d-suggestion-bar");
    m_suggestion_title = Gtk::make_managed<Gtk::Label>("");
    m_suggestion_title->add_css_class("dune3d-suggestion-title");
    sbox->append(*m_suggestion_title);
    m_suggestion_box = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL, 2);
    sbox->append(*m_suggestion_box);
    m_suggestion_revealer->set_child(*sbox);
    m_suggestion_revealer->set_reveal_child(false);
    m_win.add_canvas_overlay(*m_suggestion_revealer);

    // mises à jour
    get_canvas().signal_selection_changed().connect([this] { queue_palette_update(); });
    m_core.signal_rebuilt().connect([this] { queue_palette_update(); });
    m_core.signal_documents_changed().connect([this] { queue_palette_update(); });
    queue_palette_update();
}

void Editor::send_in_tool_action(InToolActionID action)
{
    if (!m_core.tool_is_active())
        return;
    ToolArgs args;
    args.type = ToolEventType::ACTION;
    args.action = action;
    ToolResponse r = m_core.tool_update(args);
    tool_process(r);
}

void Editor::create_export_menu()
{
    auto menu_button = Gtk::make_managed<Gtk::MenuButton>();
    auto box = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL, 6);
    auto img = Gtk::make_managed<Gtk::Image>();
    img->set_from_icon_name("d3-export");
    box->append(*img);
    box->append(*Gtk::make_managed<Gtk::Label>("Exporter"));
    menu_button->set_child(*box);
    menu_button->set_tooltip_text("Exporter le modèle (STL, OBJ, STEP, DXF…)");
    auto popover = Gtk::make_managed<Gtk::Popover>();
    auto vbox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL, 2);
    const std::vector<std::pair<ActionID, const char *>> items = {
            {ActionID::EXPORT_SOLID_MODEL_STL, "Volume en STL (impression 3D)"},
            {ActionID::EXPORT_SOLID_MODEL_OBJ, "Volume en OBJ (maillage)"},
            {ActionID::EXPORT_SOLID_MODEL_STEP, "Volume en STEP (autre logiciel de CAO)"},
            {ActionID::EXPORT_ALL_SOLID_MODELS_STEP, "Tous les corps en STEP"},
            {ActionID::EXPORT_DXF_CURRENT_GROUP, "Esquisse courante en DXF"},
            {ActionID::EXPORT_PATHS_IN_CURRENT_GROUP, "Tracés du groupe courant (SVG)"},
            {ActionID::EXPORT_PROJECTION, "Projection du volume (SVG)"},
    };
    for (const auto &[act, label] : items) {
        if (!m_action_connections.contains(act))
            continue;
        auto bu = Gtk::make_managed<Gtk::Button>(label);
        bu->set_has_frame(false);
        if (auto lbl = dynamic_cast<Gtk::Label *>(bu->get_child()))
            lbl->set_xalign(0);
        bu->signal_clicked().connect([this, act, popover] {
            popover->popdown();
            if (force_end_tool())
                trigger_action(act);
        });
        attach_action_sensitive(*bu, act);
        vbox->append(*bu);
    }
    auto hint = Gtk::make_managed<Gtk::Label>();
    hint->set_markup("<small>Sans volume dans le groupe courant, c'est le dernier\nvolume du corps qui est exporté.</small>");
    hint->add_css_class("dim-label");
    hint->set_margin_top(6);
    hint->set_xalign(0);
    vbox->append(*hint);
    popover->set_child(*vbox);
    menu_button->set_popover(*popover);
    m_win.add_header_end_widget(*menu_button);
}

void Editor::queue_palette_update()
{
    if (m_palette_update_conn.connected())
        return;
    m_palette_update_conn = Glib::signal_idle().connect([this] {
        update_tool_palette();
        update_suggestions();
        return false;
    });
}

void Editor::update_tool_palette()
{
    if (!m_palette)
        return;
    const bool has_docs = m_core.has_documents();
    m_palette->set_visible(has_docs);
    if (!has_docs || m_core.tool_is_active())
        return;
    const auto sel = get_canvas().get_selection();
    for (auto &[id, bu] : m_palette_buttons) {
        bu->set_sensitive(palette_item_can_begin(id, sel));
    }
}

void Editor::update_suggestions()
{
    if (!m_suggestion_revealer)
        return;
    if (!m_core.has_documents()) {
        m_suggestion_revealer->set_reveal_child(false);
        return;
    }
    if (m_core.tool_is_active()) {
        update_tool_action_buttons();
        return;
    }

    constexpr size_t max_suggestions = 6;
    std::vector<ActionToolID> suggestions;
    auto add = [this, &suggestions](ActionToolID id, const std::set<SelectableRef> &sel) {
        if (suggestions.size() >= max_suggestions)
            return;
        if (std::ranges::find(suggestions, id) != suggestions.end())
            return;
        if (palette_item_can_begin(id, sel))
            suggestions.push_back(id);
    };

    const auto sel = get_canvas().get_selection();
    std::string title = "Étape suivante :";

    // 1. contraintes et mesures applicables à la sélection (même logique que le menu du clic droit)
    if (sel.size()) {
        bool has_measure = false;
        for (const auto grp : {ActionGroup::CONSTRAIN, ActionGroup::MEASURE}) {
            for (const auto &[id, it_cat] : action_catalog) {
                if (suggestions.size() >= max_suggestions - 1)
                    break;
                if (it_cat.group != grp || (it_cat.flags & ActionCatalogItem::FLAGS_NO_MENU))
                    continue;
                if (!m_action_connections.contains(id))
                    continue;
                if (grp == ActionGroup::MEASURE && has_measure)
                    continue;
                if (auto tool = std::get_if<ToolID>(&id)) {
                    auto r = m_core.tool_can_begin(*tool, sel);
                    if (r.can_begin == ToolBase::CanBegin::YES && r.is_specific) {
                        suggestions.push_back(id);
                        if (grp == ActionGroup::MEASURE)
                            has_measure = true;
                    }
                }
            }
        }
        if (suggestions.size()) {
            title = "Pour la sélection :";
            add(ToolID::ENTER_DATUM, sel);
        }
    }

    // 2. étape suivante logique selon le groupe courant et le dernier outil utilisé
    if (suggestions.size() < 3) {
        auto &doc = m_core.get_current_document();
        const auto group_uu = m_core.get_current_group();
        const auto &group = doc.get_group(group_uu);
        const std::set<SelectableRef> nosel;
        switch (group.get_type()) {
        case GroupType::REFERENCE:
            add(ActionID::CREATE_GROUP_SKETCH, nosel);
            add(ToolID::DRAW_WORKPLANE, nosel);
            break;
        case GroupType::SKETCH: {
            bool has_entities = false;
            for (const auto &[uu, en] : doc.m_entities) {
                if (en->m_group == group_uu) {
                    has_entities = true;
                    break;
                }
            }
            if (!has_entities) {
                for (auto id : {ToolID::DRAW_CONTOUR, ToolID::DRAW_RECTANGLE, ToolID::DRAW_CIRCLE_2D,
                                ToolID::DRAW_REGULAR_POLYGON, ToolID::DRAW_TEXT})
                    add(id, nosel);
            }
            else {
                add(ActionID::CREATE_GROUP_EXTRUDE, nosel);
                add(ActionID::CREATE_GROUP_LATHE, nosel);
                add(ActionID::CREATE_GROUP_REVOLVE, nosel);
                add(ActionID::SELECT_UNDERCONSTRAINED, nosel);
                if (m_suggest_last_tool && is_draw_tool(*m_suggest_last_tool))
                    add(*m_suggest_last_tool, nosel);
                for (auto id : {ToolID::DRAW_CIRCLE_2D, ToolID::DRAW_RECTANGLE, ToolID::DRAW_CONTOUR})
                    add(id, nosel);
            }
            break;
        }
        default:
            add(ActionID::CREATE_GROUP_FILLET, nosel);
            add(ActionID::CREATE_GROUP_CHAMFER, nosel);
            add(ActionID::CREATE_GROUP_SKETCH, nosel);
            add(ActionID::CREATE_GROUP_LINEAR_ARRAY, nosel);
            add(ActionID::CREATE_GROUP_POLAR_ARRAY, nosel);
            add(ActionID::CREATE_GROUP_MIRROR_HORIZONTAL, nosel);
            add(ActionID::EXPORT_SOLID_MODEL_STL, nosel);
            break;
        }
    }

    m_tool_buttons_shown = false;
    if (suggestions == m_suggestions_shown && std::string(m_suggestion_title->get_text()) == title) {
        m_suggestion_revealer->set_reveal_child(suggestions.size() > 0);
        return;
    }
    m_suggestions_shown = suggestions;
    m_suggestion_title->set_text(title);

    while (auto child = m_suggestion_box->get_first_child())
        m_suggestion_box->remove(*child);

    for (const auto id : suggestions) {
        auto bu = Gtk::make_managed<Gtk::Button>();
        bu->add_css_class("osd");
        bu->add_css_class("dune3d-suggestion-button");
        auto box = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL, 5);
        const auto item = find_palette_item(id);
        if (item) {
            auto img = Gtk::make_managed<Gtk::Image>();
            img->set_from_icon_name(item->icon);
            img->set_pixel_size(20);
            box->append(*img);
        }
        auto label = Gtk::make_managed<Gtk::Label>(item ? item->label : action_catalog.at(id).name.menu);
        box->append(*label);
        bu->set_child(*box);
        bu->set_tooltip_text(get_palette_tooltip(id));
        bu->signal_clicked().connect([this, id] { palette_trigger(id); });
        m_suggestion_box->append(*bu);
    }
    m_suggestion_revealer->set_reveal_child(suggestions.size() > 0);
}

void Editor::update_tool_action_buttons()
{
    m_suggestions_shown.clear();
    m_tool_buttons_shown = true;
    m_suggestion_title->set_text("Outil en cours :");
    while (auto child = m_suggestion_box->get_first_child())
        m_suggestion_box->remove(*child);

    size_t n = 0;
    // « Terminer » et « Annuler » d'abord, puis les autres actions au clavier
    std::vector<const ActionLabelInfo *> infos;
    for (const auto &it : m_in_tool_action_label_infos) {
        if (it.action1 == InToolActionID::RMB)
            infos.insert(infos.begin(), &it);
        else if (it.action1 != InToolActionID::LMB && it.action1 != InToolActionID::NONE)
            infos.push_back(&it);
    }
    for (auto it : infos) {
        if (n >= 6)
            break;
        std::string label = it->label;
        if (label.empty() && in_tool_action_catalog.contains(it->action1))
            label = in_tool_action_catalog.at(it->action1).name;
        if (label.empty())
            continue;
        if (label.size())
            label[0] = std::toupper(static_cast<unsigned char>(label[0]));
        auto bu = Gtk::make_managed<Gtk::Button>();
        bu->add_css_class("osd");
        bu->add_css_class("dune3d-suggestion-button");
        auto box = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL, 5);
        if (it->action1 == InToolActionID::RMB) {
            bu->add_css_class("dune3d-validate-button");
            auto img = Gtk::make_managed<Gtk::Image>();
            img->set_from_icon_name("d3-ok");
            img->set_pixel_size(18);
            box->append(*img);
        }
        else if (it->action1 == InToolActionID::CANCEL) {
            bu->add_css_class("dune3d-cancel-button");
            auto img = Gtk::make_managed<Gtk::Image>();
            img->set_from_icon_name("d3-cancel");
            img->set_pixel_size(18);
            box->append(*img);
        }
        box->append(*Gtk::make_managed<Gtk::Label>(label));
        bu->set_child(*box);
        const auto action = it->action1;
        bu->signal_clicked().connect([this, action] { send_in_tool_action(action); });
        m_suggestion_box->append(*bu);
        n++;
    }
    m_suggestion_revealer->set_reveal_child(n > 0);
}

} // namespace dune3d
