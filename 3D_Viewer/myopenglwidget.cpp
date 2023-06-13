#include "myopenglwidget.h"

MyOpenGLWidget::MyOpenGLWidget(QWidget* parent) : QOpenGLWidget{parent} {
  this->scale = 1;
  this->line_thickness = 0.1F;
  this->vertex_thickness = 10;
  this->model = new Model;
  this->renderingPaused = false;
  this->model->count_of_peaks = 0;
  this->linesColor.setNamedColor("violet");
  this->vertexColor.setNamedColor("lawngreen");
  this->status = 0;
}

MyOpenGLWidget::~MyOpenGLWidget() { delete model; }

void MyOpenGLWidget::listening_to_a_lines_signal(bool state) {
    this->line_type = state;
}

//=--------------------------------------------------------

void MyOpenGLWidget::mousePressEvent(QMouseEvent *event) {
   last_x = event->pos().x();
   last_y = event->pos().y();
   last_z = event->pos().x();
}

void MyOpenGLWidget::mouseMoveEvent(QMouseEvent* event) {
    float mouse_speed = 0.5f;
  if (event->buttons() & Qt::LeftButton) {

      int x = event->pos().x() - last_x;
        int y = event->pos().y() - last_y;
        modelRotation_y = prev_angle_y - ModCelil(x * mouse_speed);
        modelRotation_x = prev_angle_x - ModCelil(y * mouse_speed);
        modelRotation_x = NormalizeAngle(modelRotation_x);
        modelRotation_y = NormalizeAngle(modelRotation_y);
        emit angleChanged_x(modelRotation_x);
        emit angleChanged_y(modelRotation_y);
        last_x = event->pos().x();
        last_y = event->pos().y();
  } else if(event->buttons() & Qt::MiddleButton) {
        int z = event->pos().x() - last_z;
        modelRotation_z = prev_angle_z - ModCelil(z * mouse_speed);
        modelRotation_z = NormalizeAngle(modelRotation_z);
        emit angleChanged_z(modelRotation_z);
        last_z = event->pos().x();
  } else if(event->buttons() & Qt::RightButton) {
      mouse_speed = 0.2f;

      int x = event->pos().x() - last_x;
      int y = event->pos().y() - last_y;
      modelRotation_x = prev_trans_x - ModCelil(x * mouse_speed);
      modelRotation_y = prev_trans_y + ModCelil(y * mouse_speed);
      translate_x(modelRotation_x);
      translate_y(modelRotation_y);

      last_x = event->pos().x();
      last_y = event->pos().y();
    }
}
//=--------------------------------------------------------

void MyOpenGLWidget::matrix_to_array() {
//  this->array.push_back(0.0);
//  this->array.push_back(0.0);
//  this->array.push_back(0.0);
  for (int i = 1; i <= this->model->count_of_peaks; i++) {
    for (int j = 0; j < 3; j++) {
      this->array.push_back(this->model->matrix_peaks[i][j]);
    }
  }
}

int NormalizeAngle(int angle) {
  while (angle < -180) angle += 360;
  while (angle > 180) angle -= 360;
  return angle;
}
#define COLORIZE(FUNK, COLLOR) FUNK(COLLOR.redF(),COLLOR.greenF(),COLLOR.blueF(),COLLOR.alphaF());

void MyOpenGLWidget::initializeGL() {
    widgetWidth = this->width();
    widgetHeight = this->height();
}

void MyOpenGLWidget::paintGL()
{
    COLORIZE(glClearColor,backgroundColor);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    this->width();
        float aspectRatio = static_cast<float>(widgetWidth) / widgetHeight;
    if(projection == kPROJORT) {
        if (aspectRatio > 1.0f) {
            glOrtho(-10.0 * aspectRatio, 10.0 * aspectRatio, -10.0, 10.0, -10.0, 10.0);
        } else {
            glOrtho(-10.0, 10.0, -10.0 / aspectRatio, 10.0 / aspectRatio, -10.0, 10.0);
        }
    } else {
        float fov = 45.0 * M_PI / 180;  // 60 угол в градусах
        float heapHeight = 10 / (2 * tan(fov / 2));
        if (aspectRatio > 1.0f) {
            glFrustum(-10.0 * aspectRatio, 10.0 * aspectRatio, -10.0, 10.0, heapHeight, 100);
        } else {
            glFrustum(-10.0, 10.0, -10.0 / aspectRatio, 10.0 / aspectRatio, heapHeight, 100);
        }
        glTranslated(0, 0, -heapHeight * 2);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLineWidth(line_thickness); // Устанавливаем толщину линий
    glPointSize(vertex_thickness); // Устанавливаем размер точек

    if (line_type == kLINEDASHD) { // включает пунктирность линий
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(1, 0x3F); // дефолтные значения (1, 0x00FF)
    } else {
        glDisable(GL_LINE_STIPPLE); // выключает пунктирность линий
    }

    glScaled(scale, scale, scale); // задаем скейл

    if (renderingPaused == false) {
        this->array.clear();
        this->matrix_to_array();
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_DOUBLE, 0, array.data());
        if (point_type != kNOPOINT) {
            if (point_type == kROUNDPOINT) {
                glEnable(GL_POINT_SMOOTH); // включаем округление точек
            } else {
                glDisable(GL_POINT_SMOOTH); // отключаем округление точек (точки отображаются квадратными)
            }
            glDrawArrays(GL_POINTS, 0, array.size() / 3);   // Сама отрисовка точек
        }
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_DOUBLE, 0, array.data());
    if (point_type != kNOPOINT) {
        if (point_type == kROUNDPOINT) {
            glEnable(GL_POINT_SMOOTH); // включаем округление точек
        } else {
            glDisable(GL_POINT_SMOOTH); // отключаем округление точек (точки отображаются квадратными)
        }
        COLORIZE(glColor4f,vertexColor);
        glDrawArrays(GL_POINTS, 0, array.size() / 3);   // Сама отрисовка точек

    }
    COLORIZE(glColor4f,linesColor);

        for (std::size_t i = 1; i <= model->count_of_poligons; i++) {
            glDrawElements(GL_LINES, (unsigned)model->array_for_edjes[i], GL_UNSIGNED_INT, model->matrix_poligons[i]);
        }
    }

    glDisableClientState(GL_VERTEX_ARRAY);

    // Вызов функции обновления экрана
    update();
}

#define ANGLE_IN_RAD(A) (0.01745329251 * (float)(prev_angle_##A - angle))

void MyOpenGLWidget::rotate_x(int angle) {
    rotate_xyz(ANGLE_IN_RAD(x), 0.0f, 0.0f);
    prev_angle_x = angle;
}

void MyOpenGLWidget::rotate_y(int angle) {
    rotate_xyz(0.0f, ANGLE_IN_RAD(y), 0.0f);
    prev_angle_y = angle;
}

void MyOpenGLWidget::rotate_z(int angle) {
    rotate_xyz(0.0f, 0.0f, ANGLE_IN_RAD(z));
    prev_angle_z = angle;
}

void MyOpenGLWidget::rotate_xyz(float angle_x, float angle_y, float angle_z) {
    rotate_model(this->model, angle_x, angle_y, angle_z);
}

void MyOpenGLWidget::translate_x(int angle) {
    translate_xyz(prev_trans_x-angle, 0.0f, 0.0f);
    prev_trans_x = angle;
}

void MyOpenGLWidget::translate_y(int angle) {
    translate_xyz(0.0f, prev_trans_y-angle, 0.0f);
    prev_trans_y = angle;
}

void MyOpenGLWidget::translate_z(int angle) {
    translate_xyz(0.0f, 0.0f, prev_trans_z-angle);
    prev_trans_z = angle;
}

void MyOpenGLWidget:: translate_xyz(float angle_x, float angle_y, float angle_z) {
    move_to_xyz(this->model, angle_x, angle_y, angle_z);
}

void MyOpenGLWidget:: scale_change(int number) {
    scale = number/10.0;
}

int ModCelil(float num) {
    return num < 0? floor(num) : ceil(num);
}

void MyOpenGLWidget::setProjection(int value) {
    projection = value;
    emit projectionChanged(projection);
}

void MyOpenGLWidget::setLineThickness(float value) {
    line_thickness = value;
    emit lineThicknessChanged(line_thickness);
}

void MyOpenGLWidget::setVertexThickness(int value) {
    vertex_thickness = value;
    emit vertexThicknessChanged(vertex_thickness);
}

void MyOpenGLWidget::setBackgroundColor(QColor color) {
    backgroundColor = color;
    emit backgroundColorChanged(backgroundColor);
}

void MyOpenGLWidget::setVertexColor(QColor color) {
    vertexColor = color;
    emit vertexColorChanged(vertexColor);
}

void MyOpenGLWidget::setLinesColor(QColor color) {
    linesColor = color;
    emit linesColorChanged(linesColor);
}

void MyOpenGLWidget::setLineType(char value) {
    line_type = value;
    emit lineTypeChanged(static_cast<int>(line_type));
}

void MyOpenGLWidget::setPointType(char value) {
    point_type = value;
    emit pointTypeChanged(static_cast<int>(point_type));
}

float MyOpenGLWidget::getLineThickness() const {
    return line_thickness;
}

int MyOpenGLWidget::getVertexThickness() const {
    return vertex_thickness;
}

QColor MyOpenGLWidget::getBackgroundColor() const {
    return backgroundColor;
}

QColor MyOpenGLWidget::getVertexColor() const {
    return vertexColor;
}

QColor MyOpenGLWidget::getLinesColor() const {
    return linesColor;
}

char MyOpenGLWidget::getLineType() const {
    return line_type;
}

char MyOpenGLWidget::getPointType() const {
    return point_type;
}

int MyOpenGLWidget::getProjection() const {
    return projection;
}
