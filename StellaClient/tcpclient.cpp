#include "tcpclient.h"

TcpClient::TcpClient(QTextEdit* _log_textedit, QObject *parent)
    : m_log_textedit(_log_textedit), QObject{parent}
{
}

TcpClient::~TcpClient()
{
    delete_image();
}

/// @brief 获取当前时间
const QString TcpClient::get_time()
{
    return QDateTime::currentDateTime().toString("HH:mm:ss");
}

void TcpClient::get_ip(const QString& _ip)
{
    m_host = _ip;
}

/// @brief 连接服务器函数
void TcpClient::connect_server()
{
    QAbstractSocket::SocketState socket_state{m_client_socket.state()};
    if (socket_state == QAbstractSocket::ConnectedState)
    {
        m_log_textedit->append(QString("[%1] %2").arg(get_time(), "已连接，请勿重复点击"));
    }
    else
    {
        m_client_socket.connectToHost(m_host, m_port);
        if (!m_client_socket.waitForConnected(100))
        {
            m_log_textedit->append(QString("[%1] %2").arg(get_time(),"连接服务器失败！请检查网络状态"));
        }
        else
        {
            m_log_textedit->append(QString("[%1] %2").arg(get_time(),"连接服务器成功！请勿断开"));
        }
    }
}

void TcpClient::stop()
{
    m_client_socket.disconnectFromHost();
    if (m_client_socket.state() == QAbstractSocket::ConnectedState)
    {
        m_client_socket.waitForDisconnected();
    }
}

bool TcpClient::get_connect_status()
{
    return m_client_socket.state() == QAbstractSocket::ConnectedState? true : false;
}

/// @brief 发送请求
void TcpClient::send_request(const Client_t::InfoHandle& _info_handle)
{
    QAbstractSocket::SocketState socket_state{m_client_socket.state()};
    if (socket_state == QAbstractSocket::ConnectedState)
    {
        int8_t request_info{static_cast<int8_t>(_info_handle)};
        QByteArray request_buffer{};
        request_buffer.append(request_info);
        qint64 written_bytes = m_client_socket.write(request_buffer);
        if (written_bytes == -1)
        {
            m_log_textedit->append(QString("[%1] %2").arg(get_time(),"请求发送失败，请检查是否连接至服务器！"));
        }
        if (!m_client_socket.waitForBytesWritten(1000))
        {
            m_log_textedit->append(QString("[%1] %2").arg(get_time(),"数据发送超时，请重试！"));
        }
    }
    else
    {
        m_log_textedit->append(QString("[%1] %2").arg(get_time(),"未连接至服务器！"));
    }
}

/// @brief 接收请求
void TcpClient::recv_request()
{
    QAbstractSocket::SocketState socket_state{m_client_socket.state()};
    QAbstractSocket::SocketError socket_error{m_client_socket.error()};
    if (socket_state == QAbstractSocket::ConnectedState)
    {
        QByteArray recv_buffer{m_client_socket.readAll()};
        if (recv_buffer.size() < 1 && socket_error != QAbstractSocket::UnknownSocketError)
        {
            if (socket_error == QAbstractSocket::SocketTimeoutError || socket_error == QAbstractSocket::SocketResourceError)
            {
                QThread::msleep(100);
            }
            else
            {
                m_log_textedit->append(QString("[%1] %2").arg(get_time(),"客户端请求接收异常！"));
            }
        }
        else if (recv_buffer.size() == 0)
        {
            /*None*/
        }
        else
        {
            int8_t recv_info{recv_buffer[0]};
            Server_t::InfoHandle info_handle{static_cast<Server_t::InfoHandle>(recv_info)};
            switch (info_handle)
            {
            case Server_t::InfoHandle::READY_SEND_IMAGE:
            {
                if(recv_image())
                {
                    send_request(Client_t::InfoHandle::REQUEST_RECEIVE_DATA);
                }
                break;
            }
            case Server_t::InfoHandle::READY_SEND_DATA:
            {
                if(recv_stu())
                {
                    send_request(Client_t::InfoHandle::REQUEST_RECEIVE_MAP);
                }
                break;
            }
            case Server_t::InfoHandle::READY_SEND_MAP:
            {
                if(recv_map())
                {
                    table_handle = true;
                }
                break;
            }
            case Server_t::InfoHandle::SERVER_READY_RECV_DATA:
            {
                if (!send_stu())
                {
                    send_request(Client_t::InfoHandle::TRANSPORT_ERROR);
                }
                break;
            }
            case Server_t::InfoHandle::SERVER_REQUEST_RECV_MAP:
            {
                send_request(Client_t::InfoHandle::CLIENT_READY_SEND_MAP);
                if (!send_map())
                {
                    send_request(Client_t::InfoHandle::TRANSPORT_ERROR);
                }
                break;
            }
            case Server_t::InfoHandle::TRANSPORT_ERROR:
            {
                m_log_textedit->append(QString("[%1] %2").arg(TcpClient::get_time(),"服务器异常!"));
                break;
            }
            default:
                break;
            }
        }
    }
}

/// @brief 接收图片
bool TcpClient::recv_image()
{
    QFile file{m_image_path};
    if (!file.open(QIODevice::WriteOnly))
    {
        m_log_textedit->append(QString("[%1] %2").arg(TcpClient::get_time(), "图像缓存路径异常，未能打开文件！"));
        return false;
    }
    qint64 file_size{},bytes_read{};
    if (m_client_socket.waitForReadyRead())
    {
        bytes_read = m_client_socket.read(reinterpret_cast<char*>(&file_size), sizeof(file_size));
        if (bytes_read != sizeof(file_size))
        {
            m_log_textedit->append(QString("[%1] %2").arg(TcpClient::get_time(), "接收图片完整性校验失败！"));
            file.close();
            return false;
        }
    }
    else
    {
        m_log_textedit->append(QString("[%1] %2").arg(TcpClient::get_time(), "客户端未接收到数据，请重试！"));
        file.close();
        return false;
    }
    QByteArray recv_image_buffer{};
    qint64 total_bytes_received{};
    while (total_bytes_received < file_size)
    {
        if (!m_client_socket.waitForReadyRead())
        {
            m_log_textedit->append(QString("[%1] %2").arg(TcpClient::get_time(), "客户端未接收到数据，请重试！"));
            file.close();
            return false;
        }

        recv_image_buffer = m_client_socket.readAll();
        if (recv_image_buffer.isEmpty())
        {
            m_log_textedit->append(QString("[%1] %2").arg(TcpClient::get_time(), "图像接收异常，请检查发送图像完整性！"));
            file.close();
            return false;
        }

        qint64 bytes_written = file.write(recv_image_buffer);
        if (bytes_written == -1)
        {
            m_log_textedit->append(QString("[%1] %2").arg(TcpClient::get_time(), "写入文件失败！"));
            file.close();
            return false;
        }
        total_bytes_received += bytes_written;
    }
    file.close();
    qDebug() << "Total image size received: " << total_bytes_received;
    return true;
}

/// @brief 接收结构体
bool TcpClient::recv_stu()
{
    if(!m_client_socket.waitForReadyRead())
    {
        return false;
    }
    else
    {
        QByteArray recv_area_buffer{};
        recv_area_buffer = m_client_socket.readAll();
        qDebug() << "recv_area_buffer : " << recv_area_buffer.size();
        if (recv_area_buffer.isEmpty())
        {
            m_log_textedit->append(QString("[%1] %2").arg(TcpClient::get_time(),"客户端接收数据异常，请尝试重新获取！"));
            return false;
        }
        else
        {
            m_area = st_tf::Area{};
            std::string recv_data(recv_area_buffer.data(), recv_area_buffer.size());
            str2area(recv_data, m_area);
            return true;
        }
    }
}

bool TcpClient::recv_map()
{
    if(!m_client_socket.waitForReadyRead())
    {
        return false;
    }
    else
    {
        QByteArray recv_map_buffer{};
        recv_map_buffer = m_client_socket.readAll();
        qDebug() << "recv_map_buffer : " << recv_map_buffer.size();
        if (recv_map_buffer.isEmpty())
        {
            m_log_textedit->append(QString("[%1] %2").arg(TcpClient::get_time(),"客户端接收配置参数异常，请尝试重新获取！"));
            return false;
        }
        else
        {
            m_params_buff.clear();
            std::string recv_data(recv_map_buffer.data(), recv_map_buffer.size());
            str2map(recv_data, m_params_buff);
            m_log_textedit->append(QString("[%1] %2").arg(TcpClient::get_time(),"图像及配置参数加载完成"));
            return true;
        }
    }
}

/// @brief 发送结构体
bool TcpClient::send_stu()
{
    std::mutex _mutex{};
    _mutex.lock();
    std::string serialzed_str{area2str(m_area)};
    _mutex.unlock();
    const char* send_buffer{serialzed_str.c_str()};
    std::size_t send_size{serialzed_str.size()};
    qint64 written_bytes = m_client_socket.write(send_buffer, send_size);
    qDebug() << "written_area_bytes" << written_bytes;
    if (written_bytes == -1)
    {
        m_log_textedit->append(QString("[%1] %2").arg(TcpClient::get_time(),"客户端上传数据失败，请重试！"));
        return false;
    }
    else if (written_bytes != static_cast<qint64>(send_size))
    {
        m_log_textedit->append(QString("[%1] %2").arg(TcpClient::get_time(),"客户端上传数据异常，请检查数据完整性！"));
        return false;
    }
    return true;
}

bool TcpClient::send_map()
{
    std::string map_str{};
    if (!map2str(m_params_buff, map_str))
    {
        qDebug() << "map2str error : " << strerror(errno) << '\n';
        return false;
    }
    qint64 written_bytes = m_client_socket.write(map_str.c_str(), map_str.size());
    qDebug() << "written_map_bytes" << written_bytes;
    if (written_bytes == -1)
    {
        m_log_textedit->append(QString("[%1] %2").arg(TcpClient::get_time(),"客户端上传配置参数失败，请重试！"));
        return false;
    }
    else if (written_bytes != static_cast<qint64>(map_str.size()))
    {
        m_log_textedit->append(QString("[%1] %2").arg(TcpClient::get_time(),"客户端上传数据异常，请检查数据完整性！"));
        return false;
    }
    else
    {
        m_log_textedit->append(QString("[%1] %2").arg(TcpClient::get_time(),"数据及配置参数提交成功！"));
        return true;
    }
}

void TcpClient::delete_image()
{
    if (!m_image_path.isEmpty() && QFile::exists(m_image_path))
    {
        QFile::remove(m_image_path);
    }
}







