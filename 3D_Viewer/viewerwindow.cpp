#include "viewerwindow.h"

#include "./ui_viewerwindow.h"
#include <QMetaObject>
ViewerWindow::ViewerWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ViewerWindow) {
  ui->setupUi(this);
  setup_sliders();
  set_background_color();
  connect(&timer, &QTimer::timeout, (this), &ViewerWindow::catch_signal);
  connect(ui->rotate_slider_y, SIGNAL(valueChanged(int)), ui->graph, SLOT(rotate_y(int)));
  connect(ui->graph, SIGNAL(projectionChanged(int)), ui->graph, SLOT(rotate_y(int)));

  connect(ui->rotate_slider_x, SIGNAL(valueChanged(int)), ui->graph, SLOT(rotate_x(int)));
  connect(ui->rotate_slider_z, SIGNAL(valueChanged(int)), ui->graph, SLOT(rotate_z(int)));
  connect(ui->translate_slider_x, SIGNAL(valueChanged(int)), ui->graph, SLOT(translate_x(int)));
  connect(ui->translate_slider_y, SIGNAL(valueChanged(int)), ui->graph, SLOT(translate_y(int)));
  connect(ui->translate_slider_z, SIGNAL(valueChanged(int)), ui->graph, SLOT(translate_z(int)));
  connect(ui->scale_slider, SIGNAL(valueChanged(int)), ui->graph, SLOT(scale_change(int)));
  connect(ui->choose_file_button, SIGNAL(pressed()), this, SLOT(choose_file_button_slot1()));
  connect(ui->choose_file_button, SIGNAL(released()), this, SLOT(choose_file_button_slot2()));
  connect(ui->save_jpeg, SIGNAL(pressed()), this, SLOT(save_jpeg_slot1()));
  connect(ui->save_jpeg, SIGNAL(released()), this, SLOT(save_jpeg_slot2()));
  connect(ui->save_bmp, SIGNAL(pressed()), this, SLOT(save_bmp_slot1()));
  connect(ui->save_bmp, SIGNAL(released()), this, SLOT(save_bmp_slot2()));
  connect(ui->save_gif, SIGNAL(pressed()), this, SLOT(save_gif_slot1()));
  connect(ui->save_gif, SIGNAL(released()), this, SLOT(save_gif_slot2()));
  connect(ui->choose_line_color, SIGNAL(pressed()), this, SLOT(choose_line_color_slot1()));
  connect(ui->choose_line_color, SIGNAL(released()), this, SLOT(choose_line_color_slot2()));
  connect(ui->choose_dot_color, SIGNAL(pressed()), this, SLOT(choose_dot_color_slot1()));
  connect(ui->choose_dot_color, SIGNAL(released()), this, SLOT(choose_dot_color_slot2()));
  connect(ui->choose_background_color, SIGNAL(pressed()), this, SLOT(choose_background_color_slot1()));
  connect(ui->choose_background_color, SIGNAL(released()), this, SLOT(choose_background_color_slot2()));
  connect(ui->graph, SIGNAL(angleChanged_x(int)), this, SLOT(set_rotate_x(int)));
  connect(ui->graph, SIGNAL(angleChanged_y(int)), this, SLOT(set_rotate_y(int)));
  connect(ui->graph, SIGNAL(angleChanged_z(int)), this, SLOT(set_rotate_z(int)));
  connect(this, SIGNAL(checked_lines_signal(bool)), ui->graph,
          SLOT(listening_to_a_lines_signal(bool)));
   connect(ui->graph, SIGNAL(backgroundColorChanged(QColor)), this, SLOT(backgroundColorChanging(QColor)));
   connect(ui->graph, SIGNAL(vertexColorChanged(QColor)), this, SLOT(labelColorChanging(QColor)));
   connect(ui->graph, SIGNAL(linesColorChanged(QColor)), this, SLOT(lineColorChanging(QColor)));
   connect(ui->graph, SIGNAL(lineThicknessChanged(int)), this, SLOT(on_line_thickness_slider_valueChanged(int)));
   connect(ui->graph, SIGNAL(vertexThicknessChanged(int)), this, SLOT(on_dot_size_slider_valueChanged(int)));
   connect(ui->graph, SIGNAL(lineTypeChanged(int)), this, SLOT(changingLineType(int)));
   connect(ui->graph, SIGNAL(pointTypeChanged(int)), this, SLOT(changingPointType(int)));
   connect(ui->graph, SIGNAL(projectionChanged(int)), this, SLOT(changingProjection(int)));

  ui->parallel_projection->setChecked(true);
  ui->solid_line->setChecked(true);
  ui->none_dot->setChecked(true);
  ui->line_color_label->setAutoFillBackground(true);  // Включаем автоматическую заливку фона
  ui->dot_color_label->setAutoFillBackground(true);  // Включаем автоматическую заливку фона
  ui->background_color_label->setAutoFillBackground(true);  // Включаем автоматическую заливку фона

  QMetaObject::invokeMethod(ui->graph, "setVertexColor", Qt::QueuedConnection,
                            Q_ARG(QColor, ui->graph->vertexColor));
  QMetaObject::invokeMethod(ui->graph, "setBackgroundColor", Qt::QueuedConnection,
                            Q_ARG(QColor, ui->graph->backgroundColor));
  QMetaObject::invokeMethod(ui->graph, "setLinesColor", Qt::QueuedConnection,
                            Q_ARG(QColor, ui->graph->linesColor));

  ReadDataFromJson();

}

ViewerWindow::~ViewerWindow() { SaveDataInJson(); delete ui; }

void ViewerWindow::set_background_color() {
    QString projectDir = QCoreApplication::applicationDirPath();
    size_t index = projectDir.toStdString().find("src");
    projectDir.remove(index, projectDir.size());
    QString temp = "src/3D_Viewer/image.png";
    QString path = projectDir + temp;
    QString styleSheet = QString(".QWidget {background-image: url(%1);}").arg(path);
    ui->centralwidget->setStyleSheet(styleSheet);
}

#define SETUP_SLIDER_ROTATE(A)             \
  ui->rotate_slider_##A->setMaximum(180);  \
  ui->rotate_slider_##A->setMinimum(-180); \
  on_rotate_slider_##A##_valueChanged(0); \
  ui->rotate_slider_##A->setValue(ui->rotate_input_##A->text().toInt())

#define SETUP_SLIDER_TRANSLATE(A)             \
  ui->translate_slider_##A->setMaximum(100);  \
  ui->translate_slider_##A->setMinimum(-100); \
  on_translate_slider_##A##_valueChanged(0); \
  ui->translate_slider_##A->setValue(ui->translate_input_##A->text().toInt())

#define SETUP_SLIDER_THICK                   \
  ui->line_thickness_slider->setMaximum(20); \
  ui->line_thickness_slider->setMinimum(1);  \
  on_line_thickness_slider_valueChanged(1);  \

#define SETUP_SLIDER_SCALE           \
  ui->scale_slider->setMaximum(100); \
  ui->scale_slider->setMinimum(1);   \
  on_scale_slider_valueChanged(10);  \
  on_scale_input_editingFinished()

#define SETUP_SLIDER_DOT               \
  ui->dot_size_slider->setMaximum(20); \
  ui->dot_size_slider->setMinimum(1);  \
  on_dot_size_slider_valueChanged(5); \

void ViewerWindow::setup_sliders() {
  SETUP_SLIDER_TRANSLATE(x);
  SETUP_SLIDER_TRANSLATE(y);
  SETUP_SLIDER_TRANSLATE(z);
  SETUP_SLIDER_ROTATE(x);
  SETUP_SLIDER_ROTATE(y);
  SETUP_SLIDER_ROTATE(z);
  if (!ui->graph->status) {
      SETUP_SLIDER_THICK;
      SETUP_SLIDER_SCALE;
      SETUP_SLIDER_DOT;
  }
}

void ViewerWindow::return_to_default_settings() {
    SETUP_SLIDER_TRANSLATE(x);
    SETUP_SLIDER_TRANSLATE(y);
    SETUP_SLIDER_TRANSLATE(z);
    SETUP_SLIDER_ROTATE(x);
    SETUP_SLIDER_ROTATE(y);
    SETUP_SLIDER_ROTATE(z);
    SETUP_SLIDER_SCALE;
}

void ViewerWindow::choose_file_button_slot1() {
  ui->choose_file_button->setStyleSheet(
      "color: black; background-color: rgba(0, 0, 0, 0.4); border-radius: "
      "10px;");
}

void ViewerWindow::choose_file_button_slot2() {
  ui->choose_file_button->setStyleSheet(
      "color: black; background-color: rgba(0, 0, 0, 0.15); border-radius: "
      "10px;");
}

void ViewerWindow::save_jpeg_slot1() {
  ui->save_jpeg->setStyleSheet(
      "color: black; background-color: rgba(0, 0, 0, 0.4); border-radius: "
      "10px;");
}

void ViewerWindow::save_jpeg_slot2() {
  ui->save_jpeg->setStyleSheet(
      "color: black; background-color: rgba(0, 0, 0, 0.15); border-radius: "
      "10px;");
}

void ViewerWindow::save_bmp_slot1() {
  ui->save_bmp->setStyleSheet(
      "color: black; background-color: rgba(0, 0, 0, 0.4); border-radius: "
      "10px;");
}

void ViewerWindow::save_bmp_slot2() {
  ui->save_bmp->setStyleSheet(
      "color: black; background-color: rgba(0, 0, 0, 0.15); border-radius: "
      "10px;");
}

void ViewerWindow::save_gif_slot1() {
  ui->save_gif->setStyleSheet(
      "color: black; background-color: rgba(0, 0, 0, 0.4); border-radius: "
      "10px;");
}

void ViewerWindow::save_gif_slot2() {
  ui->save_gif->setStyleSheet(
      "color: black; background-color: rgba(0, 0, 0, 0.15); border-radius: "
      "10px;");
}

void ViewerWindow::choose_line_color_slot1() {
  ui->choose_line_color->setStyleSheet(
      "color: black; background-color: rgba(0, 0, 0, 0.4); border-radius: "
      "10px;");
}

void ViewerWindow:: lineColorChanging(QColor color) {
    palette.setColor(QPalette::Window, color);  // Задаем цвет фона
    ui->line_color_label->setPalette(palette);
}

void ViewerWindow::choose_line_color_slot2() {
  ui->choose_line_color->setStyleSheet(
      "color: black; background-color: rgba(0, 0, 0, 0.15); border-radius: "
      "10px;");
  t_color = QColorDialog::getColor(Qt::white, this, "Выберите цвет");
  if(t_color.isValid()) {
      ui->graph->setLinesColor(t_color);
  }
}

void ViewerWindow::choose_dot_color_slot1() {
  ui->choose_dot_color->setStyleSheet(
      "color: black; background-color: rgba(0, 0, 0, 0.4); border-radius: "
      "10px;");
}

void ViewerWindow:: labelColorChanging(QColor color) {
    palette.setColor(QPalette::Window, color);  // Задаем цвет фона
    ui->dot_color_label->setPalette(palette);
}

void ViewerWindow::choose_dot_color_slot2() {
  ui->choose_dot_color->setStyleSheet(
      "color: black; background-color: rgba(0, 0, 0, 0.15); border-radius: "
      "10px;");
  t_color = QColorDialog::getColor(Qt::white, this, "Выберите цвет");
  if(t_color.isValid()) {
      ui->graph->setVertexColor(t_color);
  }
}

void ViewerWindow::choose_background_color_slot1() {
  ui->choose_background_color->setStyleSheet(
      "color: black; background-color: rgba(0, 0, 0, 0.4); border-radius: "
      "10px;");
}

void ViewerWindow:: backgroundColorChanging(QColor color) {
    palette.setColor(QPalette::Window, color);  // Задаем цвет фона
    ui->background_color_label->setPalette(palette);
}

void ViewerWindow::choose_background_color_slot2() {
  ui->choose_background_color->setStyleSheet(
      "color: black; background-color: rgba(0, 0, 0, 0.15); border-radius: "
      "10px;");
  t_color = QColorDialog::getColor(Qt::white, this, "Выберите цвет");
  if(t_color.isValid()) {
      ui->graph->setBackgroundColor(t_color);
  }
}

void ViewerWindow::on_translate_slider_x_valueChanged(int value) {
  ui->translate_input_x->setText(QString::number(value));
}

void ViewerWindow::on_translate_slider_y_valueChanged(int value) {
  ui->translate_input_y->setText(QString::number(value));
}

void ViewerWindow::on_translate_slider_z_valueChanged(int value) {
  ui->translate_input_z->setText(QString::number(value));
}

void ViewerWindow::on_translate_input_x_editingFinished() {
  ui->translate_slider_x->setValue(ui->translate_input_x->text().toInt());
}

void ViewerWindow::on_translate_input_y_editingFinished() {
  ui->translate_slider_y->setValue(ui->translate_input_y->text().toInt());
}

void ViewerWindow::on_translate_input_z_editingFinished() {
  ui->translate_slider_z->setValue(ui->translate_input_z->text().toInt());
}

void ViewerWindow::on_rotate_slider_x_valueChanged(int value) {
  ui->rotate_input_x->setText(QString::number(value));
}

void ViewerWindow::on_rotate_slider_y_valueChanged(int value) {
  ui->rotate_input_y->setText(QString::number(value));
}

void ViewerWindow::on_rotate_slider_z_valueChanged(int value) {
  ui->rotate_input_z->setText(QString::number(value));
}

void ViewerWindow::on_rotate_input_x_editingFinished() {
  ui->rotate_slider_x->setValue(ui->rotate_input_x->text().toInt());
}

void ViewerWindow::on_rotate_input_y_editingFinished() {
  ui->rotate_slider_y->setValue(ui->rotate_input_y->text().toInt());
}

void ViewerWindow::on_rotate_input_z_editingFinished() {
  ui->rotate_slider_z->setValue(ui->rotate_input_z->text().toInt());
}

void ViewerWindow::on_line_thickness_slider_valueChanged(int value) {
  ui->line_thickness_input->setText(QString::number(value));
  ui->graph->line_thickness = value;
    on_line_thickness_input_editingFinished();
}

void ViewerWindow::on_scale_slider_valueChanged(int value) {
  ui->scale_input->setText(QString::number(value));
}

void ViewerWindow::on_dot_size_slider_valueChanged(int value) {
  ui->dot_size_input->setText(QString::number(value));
  ui->graph->vertex_thickness = value;
  on_dot_size_slider_editingFinished();
}

void ViewerWindow::on_scale_input_editingFinished() {
  ui->scale_slider->setValue(ui->scale_input->text().toInt());
}

void ViewerWindow::on_line_thickness_input_editingFinished() {
  ui->line_thickness_slider->setValue(ui->line_thickness_input->text().toInt());
}

void ViewerWindow::on_dot_size_slider_editingFinished() {
  ui->dot_size_slider->setValue(ui->dot_size_input->text().toInt());
}

void ViewerWindow::on_central_projection_clicked() {
  ui->graph->setProjection(kPROJCEN);
}

void ViewerWindow::on_parallel_projection_clicked() {
  ui->graph->setProjection(kPROJORT);
}

void ViewerWindow::on_save_jpeg_clicked() { make_screenshot(kJpeg); }

void ViewerWindow::on_save_bmp_clicked() { make_screenshot(kBmp); }

void ViewerWindow::make_screenshot(extension type) {
  static int counterJpeg{1}, counterBmp{1};
  QString projectDir = QCoreApplication::applicationDirPath();
  QString screenshotsDir = projectDir + QDir::separator() + "screenshots";
  QDir().mkpath(screenshotsDir);
  QDir directory(screenshotsDir);

  if (directory.isEmpty()) counterJpeg = counterBmp = 1;

  if (type == kJpeg) {
    ui->graph->grab().save(screenshotsDir + QDir::separator() + "screenshot" +
                           QString::number(counterJpeg++) + ".jpeg");
  } else if (type == kBmp) {
    ui->graph->grab().save(screenshotsDir + QDir::separator() + "screenshot" +
                           QString::number(counterBmp++) + ".bmp");
  }
}

QString ViewerWindow::get_path_for_gif() {
  QString projectDir = QCoreApplication::applicationDirPath();
  QString gifDir = projectDir + QDir::separator() + "gifs";
  QDir().mkpath(gifDir);
  return gifDir;
}

void ViewerWindow::catch_signal() {
  QString dir = get_path_for_gif();

  if (count) {
    this->frames[count] = ui->graph->grab().toImage();
  } else {
    QGifImage gif;
    for (int i = 49; i >= 0; i--) {
      gif.addFrame(frames[i], 10);
    }
    gif.save(dir + QDir::separator() + "aboba.gif");
    timer.stop();
  }
  count--;
}

void ViewerWindow::on_save_gif_clicked() {
  if (count == -1) {
    timer.start(100);
    count = 49;
  }
}

void ViewerWindow::on_dashed_line_clicked() { checked_lines(); }

void ViewerWindow::on_solid_line_clicked() { checked_lines(); }

void ViewerWindow::checked_lines() {
  bool state = ui->solid_line->isChecked() ? kLINESOLID : kLINEDASHD;
  emit checked_lines_signal(state);
}

void ViewerWindow::on_none_dot_clicked() {
  if (ui->none_dot->isChecked()) {
    ui->graph->point_type = kNOPOINT;
  }
}

void ViewerWindow::on_round_dot_clicked() {
  if (ui->round_dot->isChecked()) {
    ui->graph->point_type = kROUNDPOINT;
  }
}

void ViewerWindow::on_squared_dot_clicked() {
  if (ui->squared_dot->isChecked()) {
    ui->graph->point_type = kSQUAREPOINT;
  }
}
void ViewerWindow::set_rotate_x(int angle) {
   ui->rotate_slider_x->setValue(angle);
}

void ViewerWindow::set_rotate_y(int angle) {
   ui->rotate_slider_y->setValue(angle);
}

void ViewerWindow::set_rotate_z(int angle) {
   ui->rotate_slider_z->setValue(angle);
}

void ViewerWindow::on_choose_file_button_clicked()
{
    return_to_default_settings();
    if (ui->graph->status) {
        ui->graph->renderingPaused = true;
        ui->graph->array.clear();
        free_model(ui->graph->model);
        ui->graph->model = new Model;
    }

    ui->graph->file_path.clear();
    ui->graph->file_path = QFileDialog::getOpenFileName(this);

    if (!ui->graph->file_path.isEmpty()) {
        QByteArray utfString = ui->graph->file_path.toUtf8();
        const char* stringData1 = utfString.constData();
        parser(ui->graph->model, stringData1);
        ui->graph->renderingPaused = false;

        ui->number_of_vertices->setText(QString::number(ui->graph->model->count_of_peaks));
        ui->number_of_edges->setText(QString::number(ui->graph->model->count_of_poligons));
        ui->graph->status = 1;
    } else {
        ui->graph->status = 0;
    }
}

void ViewerWindow::SaveDataInJson() {
    QJsonObject jsonData;
    jsonData["projection"] = ui->graph->getProjection();
    jsonData["line_thickness"] = ui->graph->getLineThickness();
    jsonData["vertex_thickness"] = ui->graph->getVertexThickness();
    jsonData["background_color"] = ui->graph->getBackgroundColor().name();
    jsonData["vertex_color"] = ui->graph->getVertexColor().name();
    jsonData["lines_color"] = ui->graph->getLinesColor().name();
    jsonData["line_type"] = ui->graph->getLineType();
    jsonData["point_type"] = ui->graph->getPointType();

    QJsonDocument jsonDoc(jsonData);
    QString projectDir = QCoreApplication::applicationDirPath();
    QFile file(projectDir + "save.json");

    if (file.open(QIODevice::WriteOnly)) {
        file.write(jsonDoc.toJson());
        file.close();
    }
}

void ViewerWindow::ReadDataFromJson() {
    QString projectDir = QCoreApplication::applicationDirPath();
    QFile file(projectDir + "save.json");
    if (file.exists()) {
        if (file.open(QIODevice::ReadOnly)) {
            QTextStream stream(&file);
           QString jsonString = stream.readAll();
           QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
            QJsonObject jsonObject = jsonDoc.object();
            ui->graph->setProjection(jsonObject["projection"].toDouble());
            ui->graph->setLineThickness(jsonObject["line_thickness"].toDouble());
            ui->graph->setVertexThickness(jsonObject["vertex_thickness"].toInt());
            QMetaObject::invokeMethod(ui->graph, "setVertexColor", Qt::QueuedConnection,
                                      Q_ARG(QColor,QColor(jsonObject["vertex_color"].toString())));
            QMetaObject::invokeMethod(ui->graph, "setBackgroundColor", Qt::QueuedConnection,
                                      Q_ARG(QColor, QColor(jsonObject["background_color"].toString())));
            QMetaObject::invokeMethod(ui->graph, "setLinesColor", Qt::QueuedConnection,
                                      Q_ARG(QColor, QColor(jsonObject["lines_color"].toString())));
            ui->graph->setLineType(jsonObject["line_type"].toInt());
            ui->graph->setPointType(jsonObject["point_type"].toInt());
            file.close();
        }
    }
}

void ViewerWindow::changingLineType(int newLineType)
{
    switch(ui->graph->getLineType())
    {
        case kLINESOLID:
            ui->solid_line->setChecked(true);
        break;
        case kLINEDASHD:
            ui->dashed_line->setChecked(true);
        break;
    }
}

void ViewerWindow::changingPointType(int newPointType)
{
    switch(ui->graph->getPointType())
    {
        case kSQUAREPOINT:
            ui->squared_dot->setChecked(true);
        break;
        case kROUNDPOINT:
            ui->round_dot->setChecked(true);
        break;
        case kNOPOINT:
            ui->none_dot->setChecked(true);
        break;
    }
}

void ViewerWindow::changingProjection(int newProjection)
{
    switch(ui->graph->getProjection())
    {
        case kPROJORT:
            ui->parallel_projection->setChecked(true);
        break;
        case kPROJCEN:
            ui->central_projection->setChecked(true);
        break;
    }
}
