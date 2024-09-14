#include "widget.h"

void Widget::connect_server_slot()
{
    if(ip_edit->text() != client.m_client_socket.peerAddress().toString())
    {
        client.stop();
        client.get_ip(ip_edit->text());
    }
    client.connect_server();
}

void Widget::get_data_slot()
{
    if (client.get_connect_status())
    {
        log_textedit->append(QString("[%1] %2").arg(client.get_time(),"客户端请求接收图像中，请稍后..."));
        client.send_request(Client_t::InfoHandle::REQUEST_RECEIVE_IMAGE);
    }
}

void Widget::send_data_slot()
{
    if (client.get_connect_status())
    {
        std::invoke(&Widget::ui2stu, this, client.m_area);
        std::invoke(&Widget::ui2map, this, client.m_params_buff);
        client.send_request(Client_t::InfoHandle::CLIENT_REQUEST_SEND_DATA);
    }
}

void Widget::__draw_invoke_slot()
{
    if (!recv_image.isNull() && client.get_connect_status())
    {
        std::invoke(&Widget::draw_invoke, this);
    }
}

void Widget::delete_lane_slot()
{
    for (auto it = client.m_area.lanes.begin(); it != client.m_area.lanes.end(); it++ )
    {
        if (it->lane_id == static_cast<st_tf::LaneID>(lanes_id_combox->currentIndex()))
        {
            it = client.m_area.lanes.erase(it);
            lane_id_tip_label->setStyleSheet("color:black; font-size:14px;");
            break;
        }
    }
    for (auto it = client.m_area.sections.begin(); it != client.m_area.sections.end(); )
    {
        if (it->lane_id == static_cast<st_tf::LaneID>(lanes_id_combox->currentIndex()))
        {
            it = client.m_area.sections.erase(it);
            section_sec_tip_label->setStyleSheet("color:black; font-size:14px;");
        }
        else
        {
            it++;
            section_sec_tip_label->setStyleSheet("color:red; font-size:14px;");
        }
    }
}

void Widget::save_lane_slot()
{
    if(select_line_combox1->currentText() == select_line_combox2->currentText())
    {
        log_textedit->append(QString("[%1] %2").arg(client.get_time(), "左右车道线序号不可相同！"));
        return;
    }
    for(const auto& l : client.m_area.sections)
    {
        if(select_line_combox1->currentText().toInt() == l.line_id || select_line_combox2->currentText().toInt() == l.line_id)
        {
            log_textedit->append(QString("[%1] %2").arg(client.get_time(), "车道线与截断面不可相同！"));
            return;
        }
    }
    for(auto& lane : client.m_area.lanes)
    {
        if(lane.lane_id == static_cast<st_tf::LaneID>(lanes_id_combox->currentIndex()))
        {
            log_textedit->append(QString("[%1] %2").arg(client.get_time(), "该车道ID已存在！"));
            return;
        }
    }
    st_tf::Lane _lane{};
    if(select_line_combox1->currentIndex() != -1 && select_line_combox2->currentIndex() != -1)
    {
        _lane.line_ids[0] = select_line_combox1->currentText().toInt();
        _lane.line_ids[1] = select_line_combox2->currentText().toInt();
    }
    _lane.arrow = static_cast<st_tf::ArrowType>(intersection_direction_combox->currentIndex());
    _lane.lane_id = static_cast<st_tf::LaneID>(lanes_id_combox->currentIndex());
    client.m_area.lanes.emplace_back(_lane);
    lane_id_tip_label->setStyleSheet("color:red; font-size:14px;");
}

void Widget::delete_section_slot()
{
    for (auto it = client.m_area.sections.begin(); it != client.m_area.sections.end(); it++)
    {
        if (it->sec_no == static_cast<int>(section_sec_combox->currentText().toInt()))
        {
            it = client.m_area.sections.erase(it);
            section_sec_tip_label->setStyleSheet("color:black; font-size:14px;");
            break;
        }
    }
}

void Widget::save_section_slot()
{
    for(const auto& l : client.m_area.lanes)
    {
        if(static_cast<int>(section_line_combox->currentText().toInt()) == l.line_ids[0] || static_cast<int>(section_line_combox->currentText().toInt()) == l.line_ids[1])
        {
            log_textedit->append(QString("[%1] %2").arg(client.get_time(), "截断面与车道线不可相同！"));
            return;
        }
    }
    for(auto& s : client.m_area.sections)
    {
        if(s.sec_no == static_cast<int>(section_sec_combox->currentText().toInt()))
        {
            log_textedit->append(QString("[%1] %2").arg(client.get_time(), "该序号截断线已存在！"));
            return;
        }
    }
    if(section_line_combox->count() < 0)
    {
        log_textedit->append(QString("[%1] %2").arg(client.get_time(), "请选择有效的横截面！"));
        return;
    }
    st_tf::Section _section{};
    _section.line_id = static_cast<int>(section_line_combox->currentText().toInt());
    _section.sec_no = static_cast<int>(section_sec_combox->currentText().toInt());
    _section.lane_id = static_cast<st_tf::LaneID>(lanes_id_combox->currentIndex());
    client.m_area.sections.emplace_back(_section);
    section_sec_tip_label->setStyleSheet("color:red; font-size:14px;");
}

void Widget::lane_and_auto_slot()
{
    for (auto it = client.m_area.lanes.begin(); it != client.m_area.lanes.end(); ++it)
    {
        if (it->lane_id == static_cast<st_tf::LaneID>(lanes_id_combox->currentIndex()))
        {
            intersection_direction_combox->setCurrentIndex(static_cast<int>(it->arrow));
            if(select_line_combox1->count() > 0 && select_line_combox2->count() > 0 && (select_line_combox1->count() == select_line_combox2->count()))
            {
                select_line_combox1->setCurrentText(QString::number(it->line_ids[0]));
                select_line_combox2->setCurrentText(QString::number(it->line_ids[1]));
            }
            lane_id_tip_label->setStyleSheet("color:red; font-size:14px;");
            break;
        }
        else
        {
            lane_id_tip_label->setStyleSheet("color:black; font-size:14px;");
        }
    }

}

void Widget::section_and_auto_slot()
{
    for(const auto& s : client.m_area.sections)
    {
        if(s.sec_no == static_cast<int>(section_sec_combox->currentText().toInt()))
        {
            if(section_line_combox->count() > 0)
            {
                section_line_combox->setCurrentText(QString::number(s.line_id));
            }
            if(section_id_combox->count() > 0)
            {
                section_id_combox->setCurrentIndex(static_cast<st_tf::LaneID>(s.lane_id));
            }
            section_sec_tip_label->setStyleSheet("color:red; font-size:14px;");
            break;
        }
        else
        {
            section_sec_tip_label->setStyleSheet("color:black; font-size:14px;");
        }
    }
}

void Widget::async_timer_slot()
{
    if(client.table_handle)
    {
        if(QFile::exists(client.m_image_path) && recv_image.load(client.m_image_path))
        {
            scaled_image = recv_image.scaled(image_win_label->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            image_win_label->setPixmap(QPixmap::fromImage(scaled_image));
        }
        std::invoke(&Widget::stu2ui, this, client.m_area);
        std::invoke(&Widget::map2ui, this, client.m_params_buff);
        std::invoke(&Widget::section_and_auto_slot, this);
        std::invoke(&Widget::hideOshow, this, true);
        client.table_handle = false;
    }
    if(!client.m_client_socket.state())
    {
        hideOshow(false);
        if(QFile::exists(client.m_image_path))
        {
            recv_image = QImage();
            image_win_label->setPixmap(QPixmap::fromImage(QImage()));
            client.delete_image();
        }
        client.m_area = st_tf::Area();
        client.m_params_buff.clear();
    }
    std::invoke(&Widget::addOdelkey, this, true, client.m_area);
}


