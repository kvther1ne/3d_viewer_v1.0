#ifndef VIEWERWINDOW_H
#define VIEWERWINDOW_H

#include <QMainWindow>
#include "myopenglwidget.h"
#include <QFileDialog>
#include <QPixmap>
#include <QTimer>
#include <QWheelEvent>
#include <QThread>
#include <QColorDialog>
#include <QLabel>
#include <QPalette>

QT_BEGIN_NAMESPACE
namespace Ui { class ViewerWindow; }
QT_END_NAMESPACE

class ViewerWindow : public QMainWindow
{
    Q_OBJECT

public:

    ViewerWindow(QWidget *parent = nullptr);
    void make_screenshot(extension type);
    QString get_path_for_gif();
    ~ViewerWindow();
    void setup_sliders();
    void checked_lines();
    void set_background_color();

    QImage frames[50];
    QTimer timer;
    int count = -1;
    void on_dot_size_slider_editingFinished();

private slots:
    void on_translate_slider_x_valueChanged(int value);
    void on_translate_slider_y_valueChanged(int value);
    void on_translate_slider_z_valueChanged(int value);

    void on_rotate_slider_x_valueChanged(int value);
    void on_rotate_slider_y_valueChanged(int value);
    void on_rotate_slider_z_valueChanged(int value);

    void on_scale_slider_valueChanged(int value);

    void on_translate_input_x_editingFinished();
    void on_translate_input_y_editingFinished();
    void on_translate_input_z_editingFinished();

    void on_rotate_input_x_editingFinished();
    void on_rotate_input_y_editingFinished();
    void on_rotate_input_z_editingFinished();

    void on_scale_input_editingFinished();
    void on_line_thickness_input_editingFinished();

    void set_rotate_x(int angle);
    void set_rotate_y(int angle);
    void set_rotate_z(int angle);

    void on_line_thickness_slider_valueChanged(int value);
    void on_dot_size_slider_valueChanged(int value);

    void on_save_jpeg_clicked();

    void on_save_bmp_clicked();

    void choose_file_button_slot1();
    void choose_file_button_slot2();
    void save_jpeg_slot1();
    void save_jpeg_slot2();
    void save_bmp_slot1();
    void save_bmp_slot2();
    void save_gif_slot1();
    void save_gif_slot2();
    void choose_line_color_slot1();
    void choose_line_color_slot2();
    void choose_dot_color_slot1();
    void choose_dot_color_slot2();
    void choose_background_color_slot1();
    void choose_background_color_slot2();

    void on_central_projection_clicked();

    void on_parallel_projection_clicked();

    void catch_signal();

    void on_save_gif_clicked();

    void on_dashed_line_clicked();
    void on_solid_line_clicked();

    void on_none_dot_clicked();

    void on_round_dot_clicked();

    void on_squared_dot_clicked();

    void on_choose_file_button_clicked();
    void return_to_default_settings();
    void labelColorChanging(QColor color);
    void lineColorChanging(QColor color);
    void backgroundColorChanging(QColor color);
    void changingLineType(int newLineType);
    void changingPointType(int newPointType);
    void changingProjection(int newProjection);

signals:
    void checked_lines_signal(bool state);

private:
    QPalette palette;
    QColor t_color;
    void SaveDataInJson();
    void ReadDataFromJson();

    Ui::ViewerWindow *ui;
};

#endif // VIEWERWINDOW_H
