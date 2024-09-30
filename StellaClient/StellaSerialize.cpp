#include "StellaSerialize.h"

bool map2str(const std::map<std::string, std::string>& src, std::string& dst)
{
    if (src.empty())
    {
        std::cerr << __func__ << " : Map is empty" << '\n';
        return false;
    }
    std::string temp_str{};
    for (const auto& [_k, _v] : src)
    {
        if (_k.empty())
        {
            temp_str += "null" + std::string(":");
        }
        else
        {
            temp_str += _k + std::string(":");
        }
        if (_v.empty())
        {
            temp_str += "null" + std::string(",");
        }
        else
        {
            temp_str += _v + std::string(",");
        }
    }
    dst = temp_str;
    return true;
}

bool str2map(const std::string& src, std::map<std::string, std::string>& dst)
{
    if (src.empty())
    {
#ifdef __DEBUG
        std::cerr << __func__ << " : Src is empty" << '\n';
#endif
        for (const auto& [_k, _v] : dst)
        {
            dst[_k] = "null";
        }
        return true;
    }
    std::string temp_src{src};
    std::size_t pos{temp_src.find(",")};
    while (pos != temp_src.npos)
    {
        std::string temp{temp_src.substr(0, pos)};
        std::size_t subpos{temp.find(":")};
        if (subpos == temp.npos)
        {
            return false;
        }
        std::string key{temp.substr(0, subpos)};
        std::string value{temp.substr(subpos + 1, temp.size() - subpos - 1)};
        dst[key] = value;
        temp_src = temp_src.substr(pos + 1, temp_src.size() - pos - 1);
        pos = temp_src.find(",");
    }
    std::size_t subpos{temp_src.find(":")};
    if (subpos == temp_src.npos)
    {
        return false;
    }
    std::string key{temp_src.substr(0, subpos)};
    std::string value{temp_src.substr(subpos + 1, temp_src.size() - subpos - 1)};
    dst[key] = value;
    return true;
}

std::string area2str(const st_tf::Area& _area)
{
    std::ostringstream oss{};
    oss << static_cast<int>(_area.area_no) << ' '
        << static_cast<int>(_area.junc_no) << ' '
        << static_cast<int>(_area.entry_or_exit) << ' '
        << static_cast<int>(_area.direction_type) << ' ';
    // lanes
    oss << _area.lanes.size() << ' ';
    for (const auto& lane : _area.lanes)
    {
        // line_ids
        oss << lane.line_ids[0] << ' ' << lane.line_ids[1] << ' ';
        // mask_points
        oss << lane.mask_points.size() << ' ';
        for (const auto& point : lane.mask_points)
        {
            oss << point.x << ' ' << point.y << ' ';
        }
        // arrow and lane_id
        oss << static_cast<int>(lane.arrow) << ' '
            << static_cast<int>(lane.lane_id) << ' ';
    }
    // sections
    oss << _area.sections.size() << ' ';
    for (const auto& section : _area.sections)
    {
        // line_id
        oss << section.line_id << ' ';
        // lane_id and sec_no
        oss << static_cast<int>(section.lane_id) << ' '
            << static_cast<int>(section.sec_no) << ' ';
    }
    // line_map
    oss << _area.line_map.size() << ' ';
    for (const auto& line_pair : _area.line_map)
    {
        int line_id = line_pair.first;
        const st_tf::Line& line = line_pair.second;
        // line_id and points
        oss << line_id << ' ' << line.points.size() << ' ';
        for (const auto& point : line.points)
        {
            oss << point.x << ' ' << point.y << ' ';
        }
        // line_id within the Line structure
        oss << line.line_id << ' ';
    }
    return oss.str();
}

void str2area(const std::string& _data, st_tf::Area& _area)
{
    std::istringstream iss(_data);
    int temp{};
    // 读取区域信息
    iss >> temp;
    _area.area_no = static_cast<unsigned char>(temp);
    iss >> temp;
    _area.junc_no = static_cast<unsigned char>(temp);
    iss >> temp;
    _area.entry_or_exit = static_cast<st_tf::AreaType>(temp);
    iss >> temp;
    _area.direction_type = static_cast<st_tf::DirectionType>(temp);
    // 读取 lanes
    std::size_t lanes_size{};
    iss >> lanes_size;
    _area.lanes.resize(lanes_size);
    for (auto& lane : _area.lanes)
    {
        // 读取 line_ids
        iss >> lane.line_ids[0] >> lane.line_ids[1];
        // 读取 mask_points
        std::size_t mask_points_size{};
        iss >> mask_points_size;
        lane.mask_points.resize(mask_points_size);
        for (auto& point : lane.mask_points)
        {
            iss >> point.x >> point.y;
        }
        // 读取 arrow 和 lane_id
        iss >> temp;
        lane.arrow = static_cast<st_tf::ArrowType>(temp);
        iss >> temp;
        lane.lane_id = static_cast<st_tf::LaneID>(temp);
    }
    // 读取 sections
    std::size_t sections_size{};
    iss >> sections_size;
    _area.sections.resize(sections_size);
    for (auto& section : _area.sections)
    {
        // 读取 line_id
        iss >> section.line_id;
        // 读取 lane_id
        iss >> temp;
        section.lane_id = static_cast<st_tf::LaneID>(temp);
        // 读取 sec_no
        iss >> temp;
        section.sec_no = static_cast<unsigned char>(temp);
    }
    // 读取 line_map
    std::size_t line_map_size{};
    iss >> line_map_size;
    for (std::size_t i = 0; i < line_map_size; ++i)
    {
        int line_id{};
        st_tf::Line line;
        // 读取 line_id 和 points
        iss >> line_id;
        std::size_t points_size{};
        iss >> points_size;
        line.points.resize(points_size);
        for (auto& point : line.points)
        {
            iss >> point.x >> point.y;
        }
        // 读取 line.line_id
        iss >> line.line_id;
        // 插入 line_map
        _area.line_map.emplace(line_id, std::move(line));
    }
}
