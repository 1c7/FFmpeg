## 2020-10-15 随便看看 ffmpeg

## 如何编译 doc/examples/ 里的代码，使其可以运行

```
./configure && make -j4 examples
```
https://stackoverflow.com/questions/51031918/ffmpeg-building-example-c-codes

## ./configure 做了什么
运行了目录下一个叫 configure 的 script (是 sh)
意义是什么？为什么一定要它？

https://en.wikipedia.org/wiki/Configure_script
run on a wide number of different computers.
As a common practice, all configure scripts are named configure
Usually, configure scripts are written for the Bourne shell(sh) but they may be written for execution in any desired shell.

## ./configure 的意义
1. 检测依赖是否都安装了


## 怎么用 .configure ？
可以看帮助文档
```
./congiure --help
```

## 缩写
* ret 一般是 return 的缩写

先声明
```c
int ret;
```

然后
```c
ret = 函数();
```

## 疑问
```
goto fail;
```
是干什么？

```
goto end;
```

## .d 文件是什么？
比如 doc/examples/avio_list_dir.d
```
doc/examples/avio_list_dir.o: doc/examples/avio_list_dir.c \
  libavcodec/avcodec.h libavutil/samplefmt.h libavutil/avutil.h \
  libavutil/common.h libavutil/attributes.h libavutil/macros.h \
  libavutil/version.h libavutil/avconfig.h libavutil/mem.h \
  libavutil/error.h libavutil/rational.h libavutil/mathematics.h \
  libavutil/intfloat.h libavutil/log.h libavutil/pixfmt.h \
  libavutil/buffer.h libavutil/cpu.h libavutil/channel_layout.h \
  libavutil/dict.h libavutil/frame.h libavutil/hwcontext.h \
  libavcodec/bsf.h libavcodec/codec_id.h libavcodec/codec_par.h \
  libavcodec/packet.h libavcodec/version.h libavcodec/codec.h \
  libavcodec/codec_desc.h libavformat/avformat.h libavformat/avio.h \
  libavformat/version.h
```
应该和编译有关，是依赖哪些文件


## Packet, AVPacket
https://www.cnblogs.com/wangguchangqing/p/5790705.html
对于视频（Video）来说，AVPacket通常包含一个压缩的Frame，而音频（Audio）则有可能包含多个压缩的Frame。并且，一个Packet有可能是空的，不包含任何压缩数据，只含有side data（side data，容器提供的关于Packet的一些附加信息。例如，在编码结束的时候更新一些流的参数）。
