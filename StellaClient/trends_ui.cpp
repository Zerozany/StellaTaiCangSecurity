#include "widget.h"

void Widget::ui2stu(st_tf::Area& _area)
{
    _area.area_no = static_cast<uchar>(area_number_edit->text().toUShort());
    _area.junc_no = static_cast<uchar>(load_number_edit->text().toUShort());
    _area.entry_or_exit = static_cast<st_tf::AreaType>(intersection_type_combox->currentIndex());
    _area.direction_type = static_cast<st_tf::DirectionType>(area_direction_combox->currentIndex());
}

void Widget::stu2ui(const st_tf::Area& _area)
{
    area_number_edit->setText(QString::number(_area.area_no));
    load_number_edit->setText(QString::number(_area.junc_no));
    intersection_type_combox->setCurrentIndex(static_cast<int>(_area.entry_or_exit));
    area_direction_combox->setCurrentIndex(static_cast<int>(_area.direction_type));
}

void Widget::ui2map(std::map<std::string, std::string>& _params)
{
    if (!params_table)
    {
        return;
    }
    _params.clear();
    int rowCount{params_table->rowCount()};
    for (int row{}; row < rowCount; ++row)
    {
        QTableWidgetItem* nameItem{params_table->item(row, 0)};
        QTableWidgetItem* valueItem{params_table->item(row, 1)};
        if (nameItem && valueItem)
        {
            std::string key{nameItem->text().toStdString()};
            std::string value{valueItem->text().toStdString()};
            if(value == "")
            {
                value = "null";
            }
            _params[key] = value;
        }
    }
}

void Widget::map2ui(const std::map<std::string, std::string>& _params)
{
    std::size_t col{_params.size()};
    params_table->setRowCount(col);
    std::size_t row{};
    for(const auto& pair : _params)
    {
        QTableWidgetItem *name_item{new QTableWidgetItem(QString::fromStdString(pair.first))};
        QTableWidgetItem *value_item{new QTableWidgetItem(QString::fromStdString(pair.second))};
        // qDebug() << QString::fromStdString(pair.first) << " " << QString::fromStdString(pair.second);
        name_item->setFlags(name_item->flags() & ~Qt::ItemIsEditable);
        params_table->setItem(row, 0, name_item);
        params_table->setItem(row, 1, value_item);
        ++row;
    }
}


