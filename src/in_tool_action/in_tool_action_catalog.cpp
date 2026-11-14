#include "in_tool_action_catalog.hpp"
#include "core/tool_id.hpp"

namespace dune3d {

const std::map<InToolActionID, InToolActionCatalogItem> in_tool_action_catalog = {
        {InToolActionID::LMB, {"placer", ToolID::NONE, InToolActionCatalogItem::FLAGS_NO_PREFERENCES}},
        {InToolActionID::RMB, {"annuler", ToolID::NONE, InToolActionCatalogItem::FLAGS_NO_PREFERENCES}},
        {InToolActionID::ARC_MODE, {"mode arc", ToolID::DRAW_ARC_2D, InToolActionCatalogItem::FLAGS_DEFAULT}},
        {InToolActionID::TOGGLE_ARC, {"basculer l'arc", ToolID::DRAW_CONTOUR, InToolActionCatalogItem::FLAGS_DEFAULT}},
        {InToolActionID::TOGGLE_BEZIER,
         {"basculer Bézier", ToolID::DRAW_CONTOUR, InToolActionCatalogItem::FLAGS_DEFAULT}},
        {InToolActionID::FLIP_ARC, {"inverser l'arc", ToolID::DRAW_ARC_2D, InToolActionCatalogItem::FLAGS_DEFAULT}},
        {InToolActionID::TOGGLE_CONSTRUCTION,
         {"basculer construction", ToolID::NONE, InToolActionCatalogItem::FLAGS_DEFAULT}},
        {InToolActionID::TOGGLE_COINCIDENT_CONSTRAINT,
         {"basculer la contrainte de coïncidence", ToolID::NONE, InToolActionCatalogItem::FLAGS_DEFAULT}},
        {InToolActionID::TOGGLE_HV_CONSTRAINT,
         {"basculer la contrainte horizontale/verticale", ToolID::DRAW_CONTOUR, InToolActionCatalogItem::FLAGS_DEFAULT}},
        {InToolActionID::TOGGLE_TANGENT_CONSTRAINT,
         {"basculer la contrainte de tangence", ToolID::DRAW_CONTOUR, InToolActionCatalogItem::FLAGS_DEFAULT}},
        {InToolActionID::CLEAR_EDGES, {"effacer les arêtes", ToolID::SELECT_EDGES, InToolActionCatalogItem::FLAGS_DEFAULT}},
        {InToolActionID::CLEAR_SPINE_ENTITIES,
         {"effacer les entités de trajectoire", ToolID::SELECT_SPINE_ENTITIES, InToolActionCatalogItem::FLAGS_DEFAULT}},
        {InToolActionID::N_SIDES_INC,
         {"augmenter le nombre de côtés", ToolID::DRAW_REGULAR_POLYGON, InToolActionCatalogItem::FLAGS_DEFAULT}},
        {InToolActionID::N_SIDES_DEC,
         {"diminuer le nombre de côtés", ToolID::DRAW_REGULAR_POLYGON, InToolActionCatalogItem::FLAGS_DEFAULT}},
        {InToolActionID::ENTER_N_SIDES,
         {"saisir le nombre de côtés", ToolID::DRAW_REGULAR_POLYGON, InToolActionCatalogItem::FLAGS_DEFAULT}},
        {InToolActionID::TOGGLE_RECTANGLE_MODE,
         {"basculer le mode rectangle", ToolID::DRAW_RECTANGLE, InToolActionCatalogItem::FLAGS_DEFAULT}},
        {InToolActionID::TOLERANCE_INC,
         {"augmenter la tolérance", ToolID::CREATE_COINCIDENT_CONSTRAINTS, InToolActionCatalogItem::FLAGS_DEFAULT}},
        {InToolActionID::TOLERANCE_DEC,
         {"diminuer la tolérance", ToolID::CREATE_COINCIDENT_CONSTRAINTS, InToolActionCatalogItem::FLAGS_DEFAULT}},
        {InToolActionID::ENTER_TOLERANCE,
         {"saisir la tolérance", ToolID::CREATE_COINCIDENT_CONSTRAINTS, InToolActionCatalogItem::FLAGS_DEFAULT}},
        {InToolActionID::ROTATE_X, {"pivoter autour de l'axe X", ToolID::NONE, InToolActionCatalogItem::FLAGS_DEFAULT}},
        {InToolActionID::ROTATE_Y, {"pivoter autour de l'axe Y", ToolID::NONE, InToolActionCatalogItem::FLAGS_DEFAULT}},
        {InToolActionID::ROTATE_Z, {"pivoter autour de l'axe Z", ToolID::NONE, InToolActionCatalogItem::FLAGS_DEFAULT}},
        {InToolActionID::TOGGLE_LOCK_ROTATION_CONSTRAINT,
         {"basculer la contrainte de verrouillage de rotation", ToolID::NONE, InToolActionCatalogItem::FLAGS_DEFAULT}},
        {InToolActionID::TOGGLE_AUTO_NORMAL,
         {"basculer la rotation automatique", ToolID::DRAW_WORKPLANE, InToolActionCatalogItem::FLAGS_DEFAULT}},

};


#define LUT_ITEM(x) {#x, InToolActionID::x}

const LutEnumStr<InToolActionID> in_tool_action_lut = {
        LUT_ITEM(LMB),
        LUT_ITEM(RMB),
        LUT_ITEM(ARC_MODE),
        LUT_ITEM(FLIP_ARC),
        LUT_ITEM(TOGGLE_ARC),
        LUT_ITEM(TOGGLE_BEZIER),
        LUT_ITEM(TOGGLE_CONSTRUCTION),
        LUT_ITEM(TOGGLE_COINCIDENT_CONSTRAINT),
        LUT_ITEM(TOGGLE_HV_CONSTRAINT),
        LUT_ITEM(TOGGLE_TANGENT_CONSTRAINT),
        LUT_ITEM(CLEAR_EDGES),
        LUT_ITEM(N_SIDES_INC),
        LUT_ITEM(N_SIDES_DEC),
        LUT_ITEM(ENTER_N_SIDES),
        LUT_ITEM(TOGGLE_RECTANGLE_MODE),
        LUT_ITEM(CLEAR_SPINE_ENTITIES),
        LUT_ITEM(TOLERANCE_DEC),
        LUT_ITEM(TOLERANCE_INC),
        LUT_ITEM(ENTER_TOLERANCE),
        LUT_ITEM(ROTATE_X),
        LUT_ITEM(ROTATE_Y),
        LUT_ITEM(ROTATE_Z),
        LUT_ITEM(TOGGLE_LOCK_ROTATION_CONSTRAINT),
        LUT_ITEM(TOGGLE_AUTO_NORMAL),
};


} // namespace dune3d
