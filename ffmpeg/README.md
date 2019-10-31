# a vedio player

## description

this is a vedio player for linux , will convert pixel format from yuv to rgb in gpu and using opengl with rendering and displaying.

## supported pixel format

1. yuv420

## building and using

1. apt-get install (freeglut+....)
2. mkdir ${workspaceFolder}/build
3. cd ${workspaceFolder}/build
4. cmake ..
5. make
6. cd src
7. ./media_player rtmp://127.0.0.1:1935/stream/test
8. ./media_player /home/bigrain/Documents/code/my_project/learning/ffmpeg/build/55.mp4
