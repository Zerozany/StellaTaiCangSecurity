#include "widget.h"

st_tf::Line buffer_line{};

void Widget::save_points(QMouseEvent *event)
{
    if(!recv_image.isNull())
    {
        // 将UI坐标系转换为相对于QImage的坐标系
        int x = event->pos().x() - image_win_label->pos().x();
        int y = event->pos().y() - image_win_label->pos().y();
        // 检查点击是否在QImage的范围内
        if (x >= 0 && x < image_win_label->size().width() && y >= 0 && y < image_win_label->size().height())
        {
            QPoint pos
            {
                static_cast<int>(0.5f + x * (recv_image.width() / static_cast<float>(image_win_label->width()))),
                static_cast<int>(0.5f + y * (recv_image.height() / static_cast<float>(image_win_label->height())))
            };
            if (event->button() == Qt::LeftButton)
            {
                buffer_line.points.emplace_back(cv::Point(pos.x(), pos.y()));
            }
        }
        update();
    }
}

void Widget::save_line(QKeyEvent *event, st_tf::Area& _area)
{
    int press_value{event->key()};
    if (press_value >= Qt::Key_0 && press_value <= Qt::Key_9)
    {
        std::string line_key = std::to_string(press_value - Qt::Key_0);
        if(buffer_line.points.size() > 1)
        {
            _area.line_map[std::stoi(line_key)] = buffer_line;
            buffer_line.points.clear();
        }
        else if(_area.line_map.count(std::stoi(line_key)) && buffer_line.points.size() == 0)
        {
            for(const auto& lane : _area.lanes)
            {
                if(std::stoi(line_key) == lane.line_ids[0] || std::stoi(line_key) == lane.line_ids[1])
                {
                    log_textedit->append(QString("[%1] %2").arg(client.get_time(), "请先绘制车道线所代替线段！"));
                    return;
                }
            }
            for(const auto& section : _area.sections)
            {
                if(std::stoi(line_key)  == section.line_id)
                {
                    log_textedit->append(QString("[%1] %2").arg(client.get_time(), "请先绘制车道线所代替线段！"));
                    return;
                }
            }
            _area.line_map.erase(std::stoi(line_key));
        }
    }
    update();
}

void Widget::draw_image(const st_tf::Area &_area)
{
    scaled_image = recv_image.scaled(image_win_label->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation).copy();
    // 创建 QPainter 对象并关联到qimage上
    QPainter painter{&scaled_image};
    // 设置画笔的抗锯齿属性
    painter.setRenderHint(QPainter::Antialiasing, true);
    // 绘制点
    if(buffer_line.points.size() > 0)
    {
        painter.setPen(QPen(Qt::yellow, 5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        for (const auto &point : buffer_line.points)
        {
            // 反转计算回原始图像坐标
            int originalX = static_cast<int>(point.x / (recv_image.width() / static_cast<float>(image_win_label->width())));
            int originalY = static_cast<int>(point.y / (recv_image.height() / static_cast<float>(image_win_label->height())));
            // 在原始图像坐标系中绘制点
            painter.drawPoint(QPoint(originalX, originalY));
        }
    }
    if (_area.line_map.size() > 0)
    {
        painter.setPen(QPen(Qt::yellow, 5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        for (const auto &[key, line] : _area.line_map)
        {
            for (const auto& point : line.points)
            {
                // 反转计算回原始图像坐标
                int originalX = static_cast<int>(point.x / (recv_image.width() / static_cast<float>(image_win_label->width())));
                int originalY = static_cast<int>(point.y / (recv_image.height() / static_cast<float>(image_win_label->height())));
                // 在原始图像坐标系中绘制点
                painter.drawPoint(QPoint(originalX, originalY));
            }
            // 计算line.points[1]在原始图像坐标系中的位置
            int textX = static_cast<int>(line.points[1].x / (recv_image.width() / static_cast<float>(image_win_label->width())));
            int textY = static_cast<int>(line.points[1].y / (recv_image.height() / static_cast<float>(image_win_label->height())));
            // 在转换后的坐标上绘制文本
            painter.setFont(QFont("Arial", 15));
            painter.drawText(QPoint(textX, textY), QString::number(key));
        }
    }
    // 绘制线段
    if (buffer_line.points.size() > 0)
    {
        painter.setPen(QPen(QColor("#00F5FF"), 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        for (std::size_t i{1}; i < buffer_line.points.size(); ++i)
        {
            QPoint start
            {
                static_cast<int>(buffer_line.points[i - 1].x / (recv_image.width() / static_cast<float>(image_win_label->width()))),
                static_cast<int>(buffer_line.points[i - 1].y / (recv_image.height() / static_cast<float>(image_win_label->height())))
            };
            QPoint end
            {
                static_cast<int>(buffer_line.points[i].x / (recv_image.width() / static_cast<float>(image_win_label->width()))),
                static_cast<int>(buffer_line.points[i].y / (recv_image.height() / static_cast<float>(image_win_label->height())))
            };
            painter.drawLine(start, end);
        }
    }
    if(_area.line_map.size() > 0)
    {
        painter.setPen(QPen(QColor("#00F5FF"), 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        for (const auto[key, line] : _area.line_map)
        {
            for (std::size_t i{1}; i < line.points.size(); ++i)
            {
                QPoint start
                {
                    static_cast<int>(line.points[i - 1].x / (recv_image.width() / static_cast<float>(image_win_label->width()))),
                    static_cast<int>(line.points[i - 1].y / (recv_image.height() / static_cast<float>(image_win_label->height())))
                };
                QPoint end
                {
                    static_cast<int>(line.points[i].x / (recv_image.width() / static_cast<float>(image_win_label->width()))),
                    static_cast<int>(line.points[i].y / (recv_image.height() / static_cast<float>(image_win_label->height())))
                };
                painter.drawLine(start, end);
            }
        }
    }
    if (_area.lanes.size() > 0)
    {
        painter.setPen(QPen(Qt::blue, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        for (const auto& lane : _area.lanes)
        {
            for (const auto& line_num : lane.line_ids)
            {
                const auto& line = _area.line_map.at(line_num);
                if (line.points.size() > 1)
                {
                    for (std::size_t i{1}; i < line.points.size(); ++i)
                    {
                        QPoint start
                        {
                            static_cast<int>(line.points[i - 1].x / (recv_image.width() / static_cast<float>(image_win_label->width()))),
                            static_cast<int>(line.points[i - 1].y / (recv_image.height() / static_cast<float>(image_win_label->height())))
                        };
                        QPoint end
                        {
                            static_cast<int>(line.points[i].x / (recv_image.width() / static_cast<float>(image_win_label->width()))),
                            static_cast<int>(line.points[i].y / (recv_image.height() / static_cast<float>(image_win_label->height())))
                        };
                        painter.drawLine(start, end);
                    }
                }
            }
            if (sizeof(lane.line_ids) / sizeof(int) > 1)
            {
                const auto& line1 = _area.line_map.at(lane.line_ids[0]);
                const auto& line2 = _area.line_map.at(lane.line_ids[1]);
                if (line1.points.size() > 1 && line2.points.size() > 1)
                {
                    int lX = static_cast<int>(line1.points[1].x / (recv_image.width() / static_cast<float>(image_win_label->width())));
                    int lY = static_cast<int>(line1.points[1].y / (recv_image.height() / static_cast<float>(image_win_label->height())));
                    int rX = static_cast<int>(line2.points[1].x / (recv_image.width() / static_cast<float>(image_win_label->width())));
                    int rY = static_cast<int>(line2.points[1].y / (recv_image.height() / static_cast<float>(image_win_label->height())));
                    painter.setFont(QFont("微软雅黑", 25));
                    painter.drawText(QPoint((lX + rX) / 2, (lY + rY) / 2), QString::number(lane.lane_id));
                }
            }
        }
    }
    if( _area.sections.size() > 0)
    {
        painter.setPen(QPen(Qt::red, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        for (const auto &section : _area.sections)
        {
            for (std::size_t i{1}; i < _area.line_map.at(section.line_id).points.size(); ++i)
            {
                QPoint start
                {
                    static_cast<int>(_area.line_map.at(section.line_id).points[i - 1].x / (recv_image.width() / static_cast<float>(image_win_label->width()))),
                    static_cast<int>(_area.line_map.at(section.line_id).points[i - 1].y / (recv_image.height() / static_cast<float>(image_win_label->height())))
                };
                QPoint end
                {
                    static_cast<int>(_area.line_map.at(section.line_id).points[i].x / (recv_image.width() / static_cast<float>(image_win_label->width()))),
                    static_cast<int>(_area.line_map.at(section.line_id).points[i].y / (recv_image.height() / static_cast<float>(image_win_label->height())))
                };
                painter.drawLine(start, end);
            }
            int lX = static_cast<int>(_area.line_map.at(section.line_id).points[1].x / (recv_image.width() / static_cast<float>(image_win_label->width())));
            int lY = static_cast<int>(_area.line_map.at(section.line_id).points[1].y / (recv_image.height() / static_cast<float>(image_win_label->height())));
            painter.setFont(QFont("微软雅黑", 25));
            painter.drawText(QPoint(lX, lY), QString::number(section.lane_id));
        }
    }
    image_win_label->setPixmap(QPixmap::fromImage(scaled_image));
}

void Widget::draw_invoke()
{
    if(buffer_line.points.size() > 0)
    {
        buffer_line.points.pop_back();
        if(buffer_line.points.size() == 1)
        {
            buffer_line.points.pop_back();
        }
    }
    update();
}

void Widget::addOdelkey(const bool& handle, const st_tf::Area& _area)
{
    if (handle)
    {
        QStringList keys_in_map;
        for (const auto& [_key, _] : _area.line_map)
        {
            QString key_str = QString::number(_key);
            keys_in_map.append(key_str);

            if (select_line_combox1->findText(key_str) == -1)
            {
                select_line_combox1->addItem(key_str);
            }
            if (select_line_combox2->findText(key_str) == -1)
            {
                select_line_combox2->addItem(key_str);
            }
            if (section_line_combox->findText(key_str) == -1)
            {
                section_line_combox->addItem(key_str);
            }
        }
        for (int i = 0; i < select_line_combox1->count(); ++i)
        {
            QString current_text = select_line_combox1->itemText(i);
            if (!keys_in_map.contains(current_text))
            {
                select_line_combox1->removeItem(i);
                --i;
            }
        }
        for (int i = 0; i < select_line_combox2->count(); ++i)
        {
            QString current_text = select_line_combox2->itemText(i);
            if (!keys_in_map.contains(current_text))
            {
                select_line_combox2->removeItem(i);
                --i;
            }
        }
        for (int i = 0; i < section_line_combox->count(); ++i)
        {
            QString current_text = section_line_combox->itemText(i);
            if (!keys_in_map.contains(current_text))
            {
                section_line_combox->removeItem(i);
                --i;
            }
        }
    }
}




