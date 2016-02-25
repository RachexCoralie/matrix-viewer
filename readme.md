# Description
This is a graphical frontend in Qt4/C++ to OpenCV matrices format

![MatrixViewer](doc/img/main.png)

# Build and Run

* required packages: cmake; qt5 (core, widgets); libopencv-dev (2.4.11)
* optionnal packages: python-opencv

## Linux

>     git clone git://github.com/crep4ever/matrix-viewer.git
>     cd matrix-viewer
>     mkdir build && cd build
>     cmake -DCMAKE_BUILD_TYPE=Release .. && make
>     sudo make install
>     matrix-viewer


## CMake options

* CMAKE_BUILD_TYPE [Debug, Release]
* OpenCV_DIR <Path>: absolute path to custom OpenCV build directory (windows)


# Integration with gdb
* copy the script misc/matrix-viewer-gdb.py in a local directory
* from a gdb shell, run: "source /path/to/matrix-viewer-gdb.py"
* the gdb command "mdv m" will launch matrix-viewer to open the cv::Mat 'm'

# Contact
* romain.goffe@gmail.com
