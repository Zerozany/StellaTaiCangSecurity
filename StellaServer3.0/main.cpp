#include "StellaServer.h"

std::map<std::string, std::string> Params{
    {"Name", "Stella"},
    {"Port1", "130001"},
    {"Port2", "130002"},
    {"Port3", "130003"},
    {"Port4", "130004"},
};
std::string PathLists{R"(/home/zero/StellaServer3.0/test.jpg)"};
st_tf::Area AreaDemo{};

auto main(int argc, char const *argv[]) -> int
{
    TcpServer server{};
    server.set_items(PathLists, Params, AreaDemo);
    /// @brief 启动服务器
    server.run();
    /// @brief 获取服务器运行状态
    // std::cout << boolalpha << "server.get_status : " << server.get_status() << std::endl;

    while (true)
    {
        server.recv_params(Params, AreaDemo);
        // for (const auto &p : Params)
        // {
        //     std::cout << p.first << " : " << p.second << std::endl;
        // }
        // std::cout << "AreaDemo.area_no : " << static_cast<int>(AreaDemo.area_no) << '\n';
        // std::cout << "AreaDemo.junc_no : " << static_cast<int>(AreaDemo.junc_no) << '\n';
        // std::cout << "AreaDemo.entry_or_exit : " << static_cast<int>(AreaDemo.entry_or_exit) << '\n';
        // std::cout << "AreaDemo.direction_type : " << static_cast<int>(AreaDemo.direction_type) << '\n';
        // for (const auto &l : AreaDemo.lanes)
        // {
        //     for (const auto &i : l.lines)
        //     {
        //         for (const auto &s : i.points)
        //         {
        //             std::cout << "(" << s.x << ", " << s.y << ") " << '\n';
        //         }
        //     }
        // }
        // for (const auto &s : AreaDemo.sections)
        // {
        //     for (const auto &p : s.line.points)
        //     {
        //         std::cout << "(" << p.x << ", " << p.y << ") " << '\n';
        //     }
        // }
        // std::cout << "----------------------" << std::endl;
        // std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    return 0;
}
