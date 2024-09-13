/*
 * 服务器头文件
 */
_Pragma("once");
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <fcntl.h>
#include <cstdint>
#include <thread>
#include <mutex>
#include <cstdio>
#include <condition_variable>
#include <atomic>
#include <map>
#include <sys/stat.h>
#include <fstream>
#include <sstream>
#include "StellaTrafficFlowType.h"

#define __DEBUG

/// @brief 映射 -> 字符串
bool map2str(const std::map<std::string, std::string> &src, std::string &dst);

/// @brief 字符串 -> 映射
bool str2map(const std::string &src, std::map<std::string, std::string> &dst);

/// @brief 序列化结构体
std::string area2str(const st_tf::Area &_area);

/// @brief 反序列化结构体
void str2area(const std::string &_data, st_tf::Area &_area);

/// @brief  服务器发送协议
namespace Server_t
{
    enum struct InfoHandle : std::int8_t
    {
        READY_SEND_IMAGE = 0,    // 服务器回应：已准备发送图片
        READY_SEND_DATA,         // 服务器回应：已准备发送数据
        READY_SEND_MAP,          // 服务器回应：已准备发送map
        SERVER_READY_RECV_DATA,  // 服务器回应：已准备接收数据
        SERVER_REQUEST_RECV_MAP, // 服务器请求：接收数据
        TRANSPORT_ERROR,         // 传输错误
    };
}

/// @brief 客户端发送协议
namespace Client_t
{
    enum struct InfoHandle : std::int8_t
    {
        REQUEST_RECEIVE_IMAGE = 0, // 客户端请求：接收图片
        REQUEST_RECEIVE_DATA,      // 客户端请求：接收数据
        REQUEST_RECEIVE_MAP,       // 客户端请求：接收map
        CLIENT_REQUEST_SEND_DATA,  // 客户端请求：发送数据
        CLIENT_READY_SEND_MAP,     // 客户端回应：已准备发送数
        TRANSPORT_ERROR,           // 传输错误
    };
}

/// @brief 协议缓存区字节数
constexpr std::size_t REQUEST_SIZE{16};

/// @brief Area缓冲区字节数
constexpr std::size_t BUFFER_SIZE{4096};

/// @brief 发送图片字节数
constexpr std::size_t IMAGE_SIZE{150000};

/// @brief 服务器对象
class TcpServer
{
public:
    explicit TcpServer() noexcept;

    TcpServer(const TcpServer & /*other*/) = delete;
    TcpServer &operator=(const TcpServer & /*other*/) = delete;
    TcpServer(TcpServer && /*other*/) = delete;
    TcpServer &operator=(TcpServer && /*other*/) = delete;

    ~TcpServer();

    /// @brief 获取服务器状态
    bool get_status() const noexcept;

    /// @brief 获取路径以及参数
    void set_items(const std::string &_paths, const std::map<string, string> &_params, const st_tf::Area &_area) noexcept;

    /// @brief 接收数据
    void recv_params(std::map<std::string, std::string> &_params, st_tf::Area &_area) noexcept;

    /// @brief 开始运行服务器
    void run() noexcept;

private:
    /// @brief 初始化服务器
    bool init_socket() noexcept;

    /// @brief 等待客户端连接线程
    void accept_client() noexcept;

    /// @brief 接收客户端协议线程
    void recv_request() noexcept;

    /// @brief 解析客户端协议
    void resolve_request(const Client_t::InfoHandle &_info) noexcept;

    /// @brief 向客户端发送协议
    void send_request(const Server_t::InfoHandle &_info) noexcept;

    /// @brief 向客户端发送图像
    bool send_image(const std::string &_img_path) noexcept;

    /// @brief 向客户端发送结构体
    bool send_area(const st_tf::Area &_area) noexcept;

    /// @brief 向客户端发送配置参数
    bool send_map(const std::map<std::string, std::string> &_params_buff) noexcept;

    /// @brief 接收客户端发送的结构体
    bool recv_area() noexcept;

    /// @brief 接收客户端发送的结构体
    bool recv_map() noexcept;

    /// @brief 异常处理：关闭监听以及交互套接字并终止程序
    [[noreturn]] void close_server() noexcept;

private:
    /// @brief 监听套接字
    std::int32_t m_listenfd{-1};

    /// @brief 交互套接字
    std::int32_t m_sockfd{-1};

    /// @brief 读取服务器端口配置
    u_short m_server_port{13000};

    /// @brief 服务器地址结构体
    struct sockaddr_in m_server_addr
    {
        .sin_family = AF_INET,
        .sin_port{htons(m_server_port)},
        .sin_addr{INADDR_ANY},
    };

    /// @brief 客户端地址结构体
    struct sockaddr_in m_client_addr
    {
        .sin_family = AF_INET,
        .sin_port{},
        .sin_addr{},
    };

    /// @brief 监听与交互线程切换句柄
    std::atomic<bool> m_atomic_bool{false};

    /// @brief 线程退出句柄
    std::atomic<bool> m_atomic_handle{true};

    /// @brief 互斥锁
    std::mutex m_mtx{};

    /// @brief 线程同步
    std::condition_variable m_cv{};

    /// @brief 监听与交互线程池
    std::vector<std::thread> m_threads{};

    /// @brief 配置参数容器
    std::map<std::string, std::string> m_params_buff{};

    /// @brief 路径列表
    std::string m_path_list{};

    /// @brief 临时缓冲 Area
    st_tf::Area m_area{};
};
