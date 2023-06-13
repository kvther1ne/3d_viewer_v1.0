# 3d_viewer_v1.0

## Реализация приложения 3D Viewer v1.0.

В данном проекте наша команда реализовала на языке программирования Си программу для просмотра 3D моделей в каркасном виде (3D Viewer). Сами модели загружаются из файлов формата .obj и имеется возможность просматривать их на экране с возможностью вращения, масштабирования и перемещения.

Формат файла .obj - это простой формат данных, который представляет только трехмерную геометрию, а именно положение каждой вершины, положение UV координат текстуры каждой вершины, нормали вершин и грани, которые определяют каждый многоугольник как список вершин и вершин текстуры. Координаты obj не имеют единиц измерения, но файлы obj могут содержать информацию о масштабе в удобочитаемой строке комментариев.

Аффинное преобразование - отображение плоскости или пространства в себя, при котором параллельные прямые переходят в параллельные прямые, пересекающиеся — в пересекающиеся, скрещивающиеся — в скрещивающиеся. 
Преобразование плоскости называется аффинным если оно взаимно однозначно и образом любой прямой является прямая. Преобразование (отображение) называется взаимно однозначным (биективным), если оно переводит разные точки в разные, и в каждую точку переходит какая-то точка.

Разработана программа для визуализации каркасной модели в трехмерном пространстве:
- Сборка программы настроена с помощью Makefile со стандартным набором целей для GNU-программ: all, install, uninstall, clean, dvi, dist, tests, gcov_report
- Обеспечено покрытие unit-тестами модулей, связанных с загрузкой моделей и аффинными преобразованиями
- В программе реализован графический пользовательский интерфейс на базе любой GUI-библиотеки Qt
- Программа должна предоставляет возможность: загружать каркасную модель из файла формата obj (поддержка только списка вершин и поверхностей); перемещать модель на заданное расстояние относительно осей X, Y, Z; поворачивать модель на заданный угол относительно своих осей X, Y, Z; масштабировать модель на заданное значение
- Графический пользовательский интерфейс содержит: кнопку для выбора файла с моделью и поле для вывода его названия; зону визуализации каркасной модели; кнопки и поля ввода для перемещения модели; кнопки и поля ввода для поворота модели; кнопки и поля ввода для масштабирования модели; информацию о загруженной модели - название файла, кол-во вершин и ребер
- Программа корректно обрабатывает и позволяет пользователю просматривать модели с деталями до 100, 1000, 10 000, 100 000, 1 000 000 вершин без зависания (зависание - это бездействие интерфейса более 0,5 секунды)
- Программа позволяет настраивать тип проекции (параллельная и центральная)
- Программа позволяет настраивать тип (сплошная, пунктирная), цвет и толщину ребер, способ отображения (отсутствует, круг, квадрат), цвет и размер вершин
- Программа позволяет выбирать цвет фона
- Настройки сохраняются между перезапусками программы
- Программа позволяеь сохранять полученные ("отрендеренные") изображения в файл в форматах bmp и jpeg
- Программа позволяет по специальной кнопке записывать небольшие "скринкасты" - текущие пользовательские аффинные преобразования загруженного объекта в gif-анимацию (640x480, 10fps, 5s)
