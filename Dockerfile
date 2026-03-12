FROM ubuntu:22.04

RUN apt-get update && apt-get install -y g++

WORKDIR /app

COPY . .

RUN mkdir -p build

RUN g++ -c stb/im_mat.cpp -o build/im_mat.o
RUN g++ -c stb/mat_im.cpp -o build/mat_im.o
RUN g++ -c Gussian/gussian.cpp -o build/gussian.o
RUN g++ -c sobel/sobel.cpp -o build/sobel.o
RUN g++ -c non_maximum_suppression/nms.cpp -o build/nms.o
RUN g++ -c Threshold/threshold.cpp -o build/threshold.o
RUN g++ -c hysteresis/hy.cpp -o build/hy.o
RUN g++ -c morphology/dilation.cpp -o build/dilation.o
RUN g++ -c morphology/erosion.cpp -o build/erosion.o

RUN ar rcs build/libedge.a build/im_mat.o build/mat_im.o build/gussian.o build/sobel.o build/nms.o build/threshold.o build/hy.o build/dilation.o build/erosion.o

RUN g++ main.cpp build/libedge.a -o edge_app

ENTRYPOINT ["./edge_app"]