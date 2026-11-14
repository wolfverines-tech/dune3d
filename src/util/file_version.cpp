#include "file_version.hpp"
#include "nlohmann/json.hpp"
#include "logger/logger.hpp"
#include "util/uuid.hpp"

namespace dune3d {

FileVersion::FileVersion(unsigned int a) : m_app(a), m_file(a)
{
}

FileVersion::FileVersion(unsigned int a, unsigned int f) : m_app(a), m_file(f)
{
}

FileVersion::FileVersion(unsigned int a, const json &j) : m_app(a), m_file(j.value("version", 0))
{
}

void FileVersion::serialize(json &j) const
{
    if (m_app)
        j["version"] = m_app;
}

void FileVersion::update_file_from_app()
{
    m_file = m_app;
}

const std::string FileVersion::learn_more_markup =
        "<a href=\"https://docs.dune3d.org/en/latest/version.html\">En savoir plus</a>";

std::string FileVersion::get_message() const
{
    if (m_app > m_file) {
        return "Ce document a été créé avec une version plus ancienne de Dune 3D. L'enregistrer le mettra au format "
               "le plus récent, qui peut être incompatible avec les anciennes versions de Dune 3D. "
               + learn_more_markup;
    }
    else if (m_file > m_app) {
        return "Ce document a été créé avec une version plus récente de Dune 3D. Certains éléments peuvent ne pas "
               "s'afficher correctement. Pour le préserver, ce document a été ouvert en lecture seule. "
               + learn_more_markup;
    }
    else {
        return "";
    }
}
} // namespace dune3d
