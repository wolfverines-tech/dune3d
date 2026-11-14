#include "constraint.hpp"
#include "nlohmann/json.hpp"
#include "all_constraints.hpp"
#include "document/document.hpp"
#include "util/json_util.hpp"

namespace dune3d {

// void to_json(json& j, const UUID& uu) {
//    j = json{{"name", p.name}, {"address", p.address}, {"age", p.age}};
//}

Constraint::Constraint(const UUID &uu) : m_uuid(uu)
{
}

std::string Constraint::get_type_name(Type type)
{
    switch (type) {
    case Type::EQUAL_LENGTH:
        return "Longueurs égales";
    case Type::EQUAL_RADIUS:
        return "Rayons égaux";
    case Type::PARALLEL:
        return "Parallèle";
    case Type::POINTS_COINCIDENT:
        return "Points coïncidents";
    case Type::POINT_ON_LINE:
        return "Point sur ligne";
    case Type::POINT_ON_CIRCLE:
        return "Point sur cercle";
    case Type::SAME_ORIENTATION:
        return "Même orientation";
    case Type::HORIZONTAL:
        return "Horizontale";
    case Type::VERTICAL:
        return "Verticale";
    case Type::POINT_DISTANCE:
        return "Distance";
    case Type::POINT_DISTANCE_ALIGNED:
        return "Distance alignée";
    case Type::WORKPLANE_NORMAL:
        return "Normale du plan de travail";
    case Type::POINT_DISTANCE_HORIZONTAL:
        return "Distance horizontale";
    case Type::POINT_DISTANCE_VERTICAL:
        return "Distance verticale";
    case Type::MIDPOINT:
        return "Milieu";
    case Type::DIAMETER:
        return "Diamètre";
    case Type::RADIUS:
        return "Rayon";
    case Type::ARC_LINE_TANGENT:
        return "Tangence arc/ligne";
    case Type::ARC_ARC_TANGENT:
        return "Tangence courbe/courbe";
    case Type::LINE_POINTS_PERPENDICULAR:
        return "Perpendicularité ligne/points";
    case Type::LINES_PERPENDICULAR:
        return "Perpendiculaire";
    case Type::LINES_ANGLE:
        return "Angle";
    case Type::POINT_IN_PLANE:
        return "Point dans le plan";
    case Type::POINT_LINE_DISTANCE:
        return "Distance point/ligne";
    case Type::POINT_PLANE_DISTANCE:
        return "Distance point/plan";
    case Type::LOCK_ROTATION:
        return "Verrouiller la rotation";
    case Type::POINT_IN_WORKPLANE:
        return "Point dans le plan de travail";
    case Type::SYMMETRIC_HORIZONTAL:
        return "Symétrie horizontale";
    case Type::SYMMETRIC_VERTICAL:
        return "Symétrie verticale";
    case Type::SYMMETRIC_LINE:
        return "Symétrie par rapport à une ligne";
    case Type::BEZIER_LINE_TANGENT:
        return "Tangence Bézier/ligne";
    case Type::BEZIER_BEZIER_TANGENT_SYMMETRIC:
        return "Tangence symétrique Bézier/Bézier";
    case Type::POINT_ON_BEZIER:
        return "Point sur Bézier";
    case Type::BEZIER_BEZIER_SAME_CURVATURE:
        return "Même courbure Bézier/Bézier";
    case Type::BEZIER_ARC_SAME_CURVATURE:
        return "Même courbure Bézier/arc";
    default:
        return "Contrainte";
    }
}

std::string Constraint::get_type_name() const
{
    return get_type_name(get_type());
}

Constraint::Constraint(const UUID &uu, const json &j) : m_uuid(uu)
{
    if (j.contains("group"))
        j.at("group").get_to(m_group);
}

NLOHMANN_JSON_SERIALIZE_ENUM(Constraint::Type,
                             {
                                     {Constraint::Type::INVALID, "invalid"},
                                     {Constraint::Type::POINTS_COINCIDENT, "points_coincident"},
                                     {Constraint::Type::PARALLEL, "parallel"},
                                     {Constraint::Type::POINT_ON_LINE, "point_on_line"},
                                     {Constraint::Type::POINT_ON_CIRCLE, "point_on_circle"},
                                     {Constraint::Type::EQUAL_LENGTH, "equal_length"},
                                     {Constraint::Type::EQUAL_RADIUS, "equal_radius"},
                                     {Constraint::Type::VERTICAL, "vertical"},
                                     {Constraint::Type::HORIZONTAL, "horizontal"},
                                     {Constraint::Type::POINT_DISTANCE, "point_distance"},
                                     {Constraint::Type::POINT_DISTANCE_ALIGNED, "point_distance_aligned"},
                                     {Constraint::Type::SAME_ORIENTATION, "same_orientation"},
                                     {Constraint::Type::WORKPLANE_NORMAL, "workplane_normal"},
                                     {Constraint::Type::POINT_DISTANCE_HORIZONTAL, "point_distance_horizontal"},
                                     {Constraint::Type::POINT_DISTANCE_VERTICAL, "point_distance_vertical"},
                                     {Constraint::Type::MIDPOINT, "midpoint"},
                                     {Constraint::Type::RADIUS, "radius"},
                                     {Constraint::Type::DIAMETER, "diameter"},
                                     {Constraint::Type::ARC_ARC_TANGENT, "arc_arc_tangent"},
                                     {Constraint::Type::ARC_LINE_TANGENT, "arc_line_tangent"},
                                     {Constraint::Type::LINE_POINTS_PERPENDICULAR, "line_points_perpendicular"},
                                     {Constraint::Type::LINES_PERPENDICULAR, "lines_perpendicular"},
                                     {Constraint::Type::LINES_ANGLE, "lines_angle"},
                                     {Constraint::Type::POINT_IN_PLANE, "point_in_plane"},
                                     {Constraint::Type::POINT_LINE_DISTANCE, "point_line_distance"},
                                     {Constraint::Type::POINT_PLANE_DISTANCE, "point_plane_distance"},
                                     {Constraint::Type::LOCK_ROTATION, "lock_rotation"},
                                     {Constraint::Type::POINT_IN_WORKPLANE, "point_in_workplane"},
                                     {Constraint::Type::SYMMETRIC_HORIZONTAL, "symmetric_horizontal"},
                                     {Constraint::Type::SYMMETRIC_VERTICAL, "symmetric_vertical"},
                                     {Constraint::Type::SYMMETRIC_LINE, "symmetric_line"},
                                     {Constraint::Type::BEZIER_LINE_TANGENT, "bezier_line_tangent"},
                                     {Constraint::Type::BEZIER_BEZIER_TANGENT_SYMMETRIC,
                                      "bezier_bezier_tangent_symmetric"},
                                     {Constraint::Type::POINT_ON_BEZIER, "point_on_bezier"},
                                     {Constraint::Type::BEZIER_BEZIER_SAME_CURVATURE, "bezier_bezier_same_curvature"},
                                     {Constraint::Type::BEZIER_ARC_SAME_CURVATURE, "bezier_arc_same_curvature"},
                             })

json Constraint::serialize() const
{
    return json{{"type", get_type()}, {"group", m_group}};
}

std::unique_ptr<Constraint> Constraint::new_from_json(const UUID &uu, const json &j)
{
    const auto type = j.at("type").get<Type>();
    switch (type) {
    case Type::POINTS_COINCIDENT:
        return std::make_unique<ConstraintPointsCoincident>(uu, j);
    case Type::PARALLEL:
        return std::make_unique<ConstraintParallel>(uu, j);
    case Type::POINT_ON_LINE:
        return std::make_unique<ConstraintPointOnLine>(uu, j);
    case Type::POINT_ON_CIRCLE:
        return std::make_unique<ConstraintPointOnCircle>(uu, j);
    case Type::EQUAL_LENGTH:
        return std::make_unique<ConstraintEqualLength>(uu, j);
    case Type::EQUAL_RADIUS:
        return std::make_unique<ConstraintEqualRadius>(uu, j);
    case Type::SAME_ORIENTATION:
        return std::make_unique<ConstraintSameOrientation>(uu, j);
    case Type::HORIZONTAL:
        return std::make_unique<ConstraintHorizontal>(uu, j);
    case Type::VERTICAL:
        return std::make_unique<ConstraintVertical>(uu, j);
    case Type::POINT_DISTANCE:
        return std::make_unique<ConstraintPointDistance>(uu, j);
    case Type::WORKPLANE_NORMAL:
        return std::make_unique<ConstraintWorkplaneNormal>(uu, j);
    case Type::POINT_DISTANCE_HORIZONTAL:
        return std::make_unique<ConstraintPointDistanceHorizontal>(uu, j);
    case Type::POINT_DISTANCE_VERTICAL:
        return std::make_unique<ConstraintPointDistanceVertical>(uu, j);
    case Type::MIDPOINT:
        return std::make_unique<ConstraintMidpoint>(uu, j);
    case Type::DIAMETER:
        return std::make_unique<ConstraintDiameter>(uu, j);
    case Type::RADIUS:
        return std::make_unique<ConstraintRadius>(uu, j);
    case Type::ARC_LINE_TANGENT:
        return std::make_unique<ConstraintArcLineTangent>(uu, j);
    case Type::ARC_ARC_TANGENT:
        return std::make_unique<ConstraintArcArcTangent>(uu, j);
    case Type::LINE_POINTS_PERPENDICULAR:
        return std::make_unique<ConstraintLinePointsPerpendicular>(uu, j);
    case Type::LINES_PERPENDICULAR:
        return std::make_unique<ConstraintLinesPerpendicular>(uu, j);
    case Type::LINES_ANGLE:
        return std::make_unique<ConstraintLinesAngle>(uu, j);
    case Type::POINT_IN_PLANE:
        return std::make_unique<ConstraintPointInPlane>(uu, j);
    case Type::POINT_LINE_DISTANCE:
        return std::make_unique<ConstraintPointLineDistance>(uu, j);
    case Type::POINT_PLANE_DISTANCE:
        return std::make_unique<ConstraintPointPlaneDistance>(uu, j);
    case Type::LOCK_ROTATION:
        return std::make_unique<ConstraintLockRotation>(uu, j);
    case Type::POINT_IN_WORKPLANE:
        return std::make_unique<ConstraintPointInWorkplane>(uu, j);
    case Type::SYMMETRIC_HORIZONTAL:
        return std::make_unique<ConstraintSymmetricHorizontal>(uu, j);
    case Type::SYMMETRIC_VERTICAL:
        return std::make_unique<ConstraintSymmetricVertical>(uu, j);
    case Type::SYMMETRIC_LINE:
        return std::make_unique<ConstraintSymmetricLine>(uu, j);
    case Type::POINT_DISTANCE_ALIGNED:
        return std::make_unique<ConstraintPointDistanceAligned>(uu, j);
    case Type::BEZIER_LINE_TANGENT:
        return std::make_unique<ConstraintBezierLineTangent>(uu, j);
    case Type::BEZIER_BEZIER_TANGENT_SYMMETRIC:
        return std::make_unique<ConstraintBezierBezierTangentSymmetric>(uu, j);
    case Type::POINT_ON_BEZIER:
        return std::make_unique<ConstraintPointOnBezier>(uu, j);
    case Type::BEZIER_BEZIER_SAME_CURVATURE:
        return std::make_unique<ConstraintBezierBezierSameCurvature>(uu, j);
    case Type::BEZIER_ARC_SAME_CURVATURE:
        return std::make_unique<ConstraintBezierArcSameCurvature>(uu, j);
    case Type::INVALID:
        throw std::runtime_error("unknown constraint type " + j.at("type").get<std::string>());
    }
    throw std::runtime_error("unknown constraint type");
}

bool Constraint::is_valid(const Document &doc) const
{
    for (auto &entity : get_referenced_entities()) {
        if (!doc.m_entities.contains(entity))
            return false;
    }
    return true;
}

std::set<UUID> Constraint::get_referenced_entities() const
{
    std::set<UUID> r;
    for (const auto &enp : get_referenced_entities_and_points()) {
        r.insert(enp.entity);
    }
    return r;
}

bool Constraint::replace_entity(const UUID &old_entity, const UUID &new_entity)
{
    auto enps = get_referenced_entities_and_points();
    bool r = false;
    for (const auto &enp : enps) {
        if (enp.entity == old_entity) {
            if (replace_point(enp, {new_entity, enp.point}))
                r = true;
        }
    }
    return r;
}

Constraint::~Constraint() = default;
} // namespace dune3d
