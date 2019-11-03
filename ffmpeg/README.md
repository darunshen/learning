# a vedio player

## description

this is a vedio player for linux , will convert pixel format from yuv to rgb in gpu and using opengl with rendering and displaying.

## supported pixel format

1. yuv420

## note

1. need to compile libx264 and libx265 from source:  
    git clone https://code.videolan.org/videolan/x264.git  
    cd x264/  
    ./configure --enable-shared  
    make -j12&& sudo make install  
  
    hg clone http://hg.videolan.org/x265  
    cd x265/build/linux  
    sudo apt  install cmake-curses-gui  
    ./make-Makefiles.bash  
    make -j12 && sudo make install  

## building and using

1. apt-get install (freeglut+....)
2. mkdir ${workspaceFolder}/build
3. cd ${workspaceFolder}/build
4. cmake ..
5. make
6. cd src
7. ./media_player rtmp://127.0.0.1:1935/stream/test
8. ./media_player /home/bigrain/Documents/code/my_project/learning/ffmpeg/build/55.mp4
