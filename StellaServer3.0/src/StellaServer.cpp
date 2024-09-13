#include "StellaServer.h"

bool map2str(const std::map<std::string, std::string> &src, std::string &dst)
{
    if (src.empty())
    {
        std::cerr << __func__ << " : Map is empty" << '\n';
        return false;
    }
    std::string temp_str{};
    if (src.size() == 0)
    {
        return false;
    }
    for (const auto &item : src)
    {
        temp_str += (item.first + std::string(":") + item.second + std::string(","));
    }
    dst = temp_str;
    return true;
}

bool str2map(const std::string &src, std::map<std::string, std::string> &dst)
{
    if (src.empty())
    {
#ifdef __DEBUG
        std::cerr << __func__ << " : Src is empty" << '\n';
#endif
        for (const auto &key : dst)
        {
            dst[key.first] = "0";
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

std::string area2str(const st_tf::Area &_area)
{
    std::ostringstream oss{};
    oss << static_cast<int>(_area.area_no) << ' '
        << static_cast<int>(_area.junc_no) << ' '
        << static_cast<int>(_area.entry_or_exit) << ' '
        << static_cast<int>(_area.direction_type) << ' ';
    // lanes
    oss << _area.lanes.size() << ' ';
    for (const auto &lane : _area.lanes)
    {
        // line_ids
        oss << lane.line_ids[0] << ' ' << lane.line_ids[1] << ' ';
        // mask_points
        oss << lane.mask_points.size() << ' ';
        for (const auto &point : lane.mask_points)
        {
            oss << point.x << ' ' << point.y << ' ';
        }
        // arrow and lane_id
        oss << static_cast<int>(lane.arrow) << ' '
            << static_cast<int>(lane.lane_id) << ' ';
    }
    // sections
    oss << _area.sections.size() << ' ';
    for (const auto &section : _area.sections)
    {
        // line_id
        oss << section.line_id << ' ';
        // lane_id and sec_no
        oss << static_cast<int>(section.lane_id) << ' '
            << static_cast<int>(section.sec_no) << ' ';
    }
    // line_map
    oss << _area.line_map.size() << ' ';
    for (const auto &line_pair : _area.line_map)
    {
        int line_id = line_pair.first;
        const st_tf::Line &line = line_pair.second;
        // line_id and points
        oss << line_id << ' ' << line.points.size() << ' ';
        for (const auto &point : line.points)
        {
            oss << point.x << ' ' << point.y << ' ';
        }
        // line_id within the Line structure
        oss << line.line_id << ' ';
    }
    return oss.str();
}

void str2area(const std::string &_data, st_tf::Area &_area)
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
    for (auto &lane : _area.lanes)
    {
        // 读取 line_ids
        iss >> lane.line_ids[0] >> lane.line_ids[1];
        // 读取 mask_points
        std::size_t mask_points_size{};
        iss >> mask_points_size;
        lane.mask_points.resize(mask_points_size);
        for (auto &point : lane.mask_points)
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
    for (auto &section : _area.sections)
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
        for (auto &point : line.points)
        {
            iss >> point.x >> point.y;
        }
        // 读取 line.line_id
        iss >> line.line_id;
        // 插入 line_map
        _area.line_map.emplace(line_id, std::move(line));
    }
}

TcpServer::TcpServer() noexcept
{
    // 启动次数句柄
    std::uint8_t init_handle{};
    // 如果启动失败 重试3次
    while (init_handle < 3)
    {
        if (!init_socket())
        {
            ++init_handle;
            std::this_thread::sleep_for(std::chrono::milliseconds(400));
            continue;
        }
        else if (init_handle == 3)
        {
            std::cerr << "Server startup exception, please check !" << '\n';
            if (m_listenfd >= 0)
            {
                close(m_listenfd);
            }
            std::exit(-1);
        }
        else
        {
            std::cerr << "The server has been turned on ..." << '\n';
            break;
        }
    }
}

TcpServer::~TcpServer()
{
    m_atomic_handle = false;
    m_cv.notify_all();
    for (auto &t : m_threads)
    {
        if (t.joinable())
        {
            t.join();
        }
    }
    close(m_sockfd);
    close(m_listenfd);
    std::cerr << "The server has been shut down" << '\n';
}

bool TcpServer::get_status() const noexcept
{
    return m_atomic_handle;
}

void TcpServer::run() noexcept
{
    try
    {
        m_threads.emplace_back(std::thread{std::bind(&TcpServer::accept_client, this)});
        m_threads.emplace_back(std::thread{std::bind(&TcpServer::recv_request, this)});
    }
    catch (const std::exception &e)
    {
        std::cerr << __func__ << ":" << e.what() << '\n';
        close_server();
    }
    catch (...)
    {
        std::cerr << __func__ << ":" << "Exception:unknown" << '\n';
        close_server();
    }
}

bool TcpServer::init_socket() noexcept
{
    try
    {
        m_listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (m_listenfd == -1)
        {
            std::cerr << "Failed to create socket :" << strerror(errno) << '\n';
            return false;
        }
        int optval{1};
        if (setsockopt(m_listenfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
        {
            /// @bug Warning
            std::cout << "Reuse address failed :" << strerror(errno) << '\n';
        }
        if (setsockopt(m_listenfd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval)) == -1)
        {
            /// @bug Warning
            std::cout << "Reuse port failed :" << strerror(errno) << '\n';
        }
        int32_t flags{fcntl(m_listenfd, F_GETFL, 0)};
        if (flags == -1)
        {
            std::cerr << "Get socket flags failed :" << strerror(errno) << '\n';
            return false;
        }
        if (fcntl(m_listenfd, F_SETFL, flags | O_NONBLOCK) == -1)
        {
            std::cerr << "Set non-blocking flags failed :" << strerror(errno) << '\n';
            return false;
        }
        if (bind(m_listenfd, reinterpret_cast<sockaddr *>(&m_server_addr), sizeof(m_server_addr)) == -1)
        {
            std::cerr << "Binding socket failed :" << strerror(errno) << '\n';
            return false;
        }
        if (listen(m_listenfd, 1) == -1)
        {
            std::cerr << "Server listen unstart failed:" << strerror(errno) << '\n';
            return false;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << __func__ << ":" << e.what();
        close_server();
    }
    catch (...)
    {
        std::cerr << __func__ << ":" << "Exception:unknown";
        close_server();
    }
    return true;
}

void TcpServer::accept_client() noexcept
{
    while (true)
    {
        {
            std::unique_lock<std::mutex> lock(m_mtx);
            while (m_atomic_bool && m_atomic_handle)
            {
                m_cv.wait(lock);
            }
            if (!m_atomic_handle)
            {
                break;
            }
        }
        socklen_t client_size{sizeof(m_client_addr)};
        if ((m_sockfd = accept(m_listenfd, reinterpret_cast<sockaddr *>(&m_client_addr), &client_size)) == -1)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            }
            else
            {
                std::cerr << "Client connection exception : " << strerror(errno) << '\n';
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            }
        }
        else
        {
            char client_ip[INET_ADDRSTRLEN]{};
            inet_ntop(AF_INET, &m_client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
            u_short client_port{ntohs(m_client_addr.sin_port)};
            std::printf("Client connection successfully -> { IP: %s, Port: %hu }\n", client_ip, client_port);
            m_atomic_bool = true;
            m_cv.notify_one();
        }
    }
}

void TcpServer::recv_request() noexcept
{
    while (true)
    {
        {
            std::unique_lock<std::mutex> lock(m_mtx);
            while (!m_atomic_bool && m_atomic_handle)
            {
                m_cv.wait(lock);
            }
            if (!m_atomic_handle)
            {
                break;
            }
        }
        char recv_buffer[REQUEST_SIZE]{};
        ssize_t recv_bytes = recv(m_sockfd, recv_buffer, sizeof(recv_buffer), MSG_DONTWAIT);
        if (recv_bytes < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            }
            else
            {
                std::cerr << "Server Protocol reception failed : " << strerror(errno) << '\n';
                close(m_sockfd);
                m_atomic_bool = false;
                m_cv.notify_one();
            }
        }
        else if (recv_bytes == 0)
        {
            std::cerr << "The client has lost connection : " << strerror(errno) << '\n';
            close(m_sockfd);
            m_atomic_bool = false;
            m_cv.notify_one();
        }
        else
        {
            Client_t::InfoHandle info_handle = static_cast<Client_t::InfoHandle>(recv_buffer[0]);
            resolve_request(info_handle);
        }
    }
}

void TcpServer::resolve_request(const Client_t::InfoHandle &_info) noexcept
{
    try
    {
        switch (_info)
        {
        // 客户端请求接收图片
        case Client_t::InfoHandle::REQUEST_RECEIVE_IMAGE:
        {
            send_request(Server_t::InfoHandle::READY_SEND_IMAGE);
            std::this_thread::sleep_for(std::chrono::milliseconds(400));
            if (!send_image(m_path_list))
            {
                return;
            }
            break;
        }
        case Client_t::InfoHandle::REQUEST_RECEIVE_DATA:
        {
            send_request(Server_t::InfoHandle::READY_SEND_DATA);
            std::this_thread::sleep_for(std::chrono::milliseconds(400));
            if (!send_area(m_area))
            {
                return;
            }
            break;
        }
        case Client_t::InfoHandle::REQUEST_RECEIVE_MAP:
        {
            send_request(Server_t::InfoHandle::READY_SEND_MAP);
            std::this_thread::sleep_for(std::chrono::milliseconds(400));
            if (!send_map(m_params_buff))
            {
                return;
            }
            break;
        }
        // 接收来自客户端发送的结构体
        case Client_t::InfoHandle::CLIENT_REQUEST_SEND_DATA:
        {
            send_request(Server_t::InfoHandle::SERVER_READY_RECV_DATA);
            std::this_thread::sleep_for(std::chrono::milliseconds(400));
            if (recv_area())
            {
                send_request(Server_t::InfoHandle::SERVER_REQUEST_RECV_MAP);
            }
            break;
        }
        case Client_t::InfoHandle::CLIENT_READY_SEND_MAP:
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(400));
            if (!recv_map())
            {
                send_request(Server_t::InfoHandle::TRANSPORT_ERROR);
            }
            break;
        }
        case Client_t::InfoHandle::TRANSPORT_ERROR:
        {
            std::cerr << "Client abnormal : " << strerror(errno) << '\n';
            break;
        }
        default:
            return;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << __func__ << ":" << e.what() << '\n';
        std::exit(-1);
    }
    catch (...)
    {
        std::cerr << __func__ << ":" << "Exception:unknown" << '\n';
        std::exit(-1);
    }
}

void TcpServer::send_request(const Server_t::InfoHandle &_info) noexcept
{
    try
    {
        char respond_buffer[REQUEST_SIZE]{};
        respond_buffer[0] = static_cast<char>(_info);
        {
            ssize_t send_bytes = send(m_sockfd, respond_buffer, sizeof(respond_buffer), MSG_DONTWAIT);
            if (send_bytes < 0)
            {
                std::cerr << "Server failed to send protocol : " << strerror(errno) << '\n';
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << __func__ << ":" << e.what() << '\n';
    }
    catch (...)
    {
        std::cerr << __func__ << ":" << "Exception:unknown" << '\n';
    }
}

void TcpServer::set_items(const std::string &_paths, const std::map<string, string> &_params, const st_tf::Area &_area) noexcept
{
    std::lock_guard<std::mutex> lock{m_mtx};
    m_path_list = _paths;
    m_params_buff = _params;
    m_area = _area;
}

void TcpServer::recv_params(std::map<std::string, std::string> &_params, st_tf::Area &_area) noexcept
{
    std::lock_guard<std::mutex> lock{m_mtx};
    _params = m_params_buff;
    _area = m_area;
}

bool TcpServer::send_image(const std::string &_img_path) noexcept
{
    FILE *fq{nullptr};
    char image_buffer[1024]{};
    std::size_t sz{};
    try
    {
        if ((fq = fopen(_img_path.c_str(), "rb")) == nullptr)
        {
            std::cerr << "Open image path error : " << strerror(errno) << '\n';
            return false;
        }
        fseek(fq, 0, SEEK_END);
        long file_size{ftell(fq)};
        rewind(fq);
        if (write(m_sockfd, &file_size, sizeof(file_size)) != sizeof(file_size))
        {
            std::cerr << "Send file size error : " << strerror(errno) << '\n';
            fclose(fq);
            return false;
        }
        while (!feof(fq))
        {
            std::size_t len = fread(image_buffer, 1, sizeof(image_buffer), fq);
            if (len != write(m_sockfd, image_buffer, len))
            {
                return false;
            }
            sz += len;
        }
        fclose(fq);
        std::cerr << "send image size : " << sz << std::endl;
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << __func__ << ":" << e.what() << '\n';
        return false;
    }
    catch (...)
    {
        std::cerr << __func__ << ":" << "Exception:unknown" << '\n';
        return false;
    }
}

bool TcpServer::send_area(const st_tf::Area &_area) noexcept
{
    std::mutex _mutex{};
    try
    {
        // 序列化结构体
        _mutex.lock();
        std::string serialized_str = area2str(m_area);
        _mutex.unlock();
        const char *send_buffer{serialized_str.c_str()};
        std::size_t send_size{serialized_str.size()};
        // 发送结构体
        ssize_t area_bytes = send(m_sockfd, send_buffer, send_size, MSG_DONTWAIT);
        if (area_bytes < 0)
        {
            std::cerr << "Send area error : " << strerror(errno) << '\n';
            return false;
        }
#ifdef __DEBUG
        else
        {
            std::cerr << "Send area'size : " << area_bytes << '\n';
            return true;
        }
#endif
    }
    catch (const std::exception &e)
    {
        std::cerr << __func__ << ":" << e.what() << '\n';
        return false;
    }
    catch (...)
    {
        std::cerr << __func__ << ":" << "Exception:unknown" << '\n';
        return false;
    }
}

bool TcpServer::send_map(const std::map<std::string, std::string> &_params_buff) noexcept
{
    std::string map_str{};
    try
    {
        if (!map2str(_params_buff, map_str))
        {
            std::cerr << "map2str error : " << strerror(errno) << '\n';
            return false;
        }
        ssize_t send_bytes = send(m_sockfd, map_str.data(), map_str.size(), MSG_DONTWAIT);
        if (send_bytes < 0)
        {
            std::cerr << "Send map error : " << strerror(errno) << '\n';
            return false;
        }
#ifdef __DEBUG
        else
        {
            std::cerr << "Send map'size : " << send_bytes << '\n';
            return true;
        }
#endif
    }
    catch (const std::exception &e)
    {
        std::cerr << __func__ << ":" << e.what() << '\n';
        return false;
    }
    catch (...)
    {
        std::cerr << __func__ << ":" << "Exception:unknown" << '\n';
        return false;
    }
}

bool TcpServer::recv_area() noexcept
{
    std::vector<char> area_buffer(BUFFER_SIZE);
    try
    {
        while (true)
        {
            ssize_t read_bytes = recv(m_sockfd, area_buffer.data(), BUFFER_SIZE, MSG_DONTWAIT);
            if (read_bytes < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
            {
                continue;
            }
            else if (read_bytes < 0 && !(errno == EAGAIN || errno == EWOULDBLOCK))
            {
                std::cerr << "Recv area error : " << strerror(errno) << '\n';
                return false;
            }
            else if (read_bytes == 0)
            {
                return false;
            }
            else
            {
#ifdef __DEBUG
                std::cerr << "Recv area_buffer's size : " << read_bytes << '\n';
#endif
                std::string recv_data(area_buffer.data(), read_bytes);
                std::mutex _mutex{};
                _mutex.lock();
                str2area(recv_data, m_area);
                _mutex.unlock();
                return true;
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << __func__ << ":" << e.what() << '\n';
        return false;
    }
    catch (...)
    {
        std::cerr << __func__ << ":" << "Exception:unknown" << '\n';
        return false;
    }
}

bool TcpServer::recv_map() noexcept
{
    char map_buffer[BUFFER_SIZE]{};
    try
    {
        while (true)
        {
            ssize_t read_bytes = recv(m_sockfd, map_buffer, BUFFER_SIZE, MSG_DONTWAIT);
            if (read_bytes < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
            {
                continue;
            }
            else if (read_bytes < 0 && !(errno == EAGAIN || errno == EWOULDBLOCK))
            {
                std::cerr << "Recv map error : " << strerror(errno) << '\n';
                return false;
            }
            else if (read_bytes == 0)
            {
                return false;
            }
            else
            {
#ifdef __DEBUG
                std::cerr << "Recv map's size : " << read_bytes << '\n';
#endif
                m_params_buff.clear();
                std::mutex _mutex{};
                _mutex.lock();
                std::string recv_data(map_buffer, read_bytes);
                str2map(recv_data, m_params_buff);
                _mutex.unlock();
                return true;
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << __func__ << ":" << e.what() << '\n';
        return false;
    }
    catch (...)
    {
        std::cerr << __func__ << ":" << "Exception:unknown" << '\n';
        return false;
    }
}

void TcpServer::close_server() noexcept
{
    if (m_listenfd >= 0)
    {
        close(m_listenfd);
    }
    if (m_sockfd >= 0)
    {
        close(m_sockfd);
    }
    std::exit(-1);
}
