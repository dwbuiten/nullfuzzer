nullfuzzer
----------

A simple library for fuzzing unchecked calls to allocation functions. Currently requires
the end-user application to use a prefixed set of allocation functions. Could be extended
to be injected via `LD_PRELOAD` pretty easily (example [here](https://gist.github.com/dwbuiten/7101755)).
It should work on Windows, but untested on anything except Linux. It should be fully
thread-safe.

**Building**

Requires a compiler with C11 atomics, a pthreads implementation, and a copy of `libbackrace`
installed somewhere the compiler can see.

`libbacktrace` is part of GCC and can be installed as follows:

```sh
curl -O <gcc mirror>/pub/gcc/releases/gcc-X.Y.Z/gcc-X.Y.Z.tar.xz
tar zxf gcc-X.Y.Z.tar.xz
cd gcc-X.Y.Z/libbacktrace
./configure
make
sudo install backtrace.h /some/path/include
sudo install .libs/libbacktrace.a /some/path/lib
```

This is pretty ugly, but I'm not included to care enough to make it less so, without
a good reason yet.

`libnullfuzzer` can be built via:

```sh
make FUNCPREFIX=myprefix
sudo make install PREFIX=/some/path
```

You may need to overrive the `CC` and/or `AR` environment variables if your default C
compiler is not the one you wish to use.

The default `FUNCPREFIX` is `fuzz`.

**Usage**

Currently only tested with [FFmpeg](https://ffmpeg.org), via:

```sh
./configure --malloc-prefix=fuzz_ --extra-libs="-lnullfuzzer -lbacktrace -lpthread"
```

Probabability of allocation failure can be controlled with the `FUZZ_FAILPROB` environment
variable. The default is 1-in-`10000`. The seed for `srand` can be controlled with the
`FUZZ_SEED` environment variable.

An example failure/bug output will look like this:

```
[user@host ffmpeg]$ FUZZ_SEED=1512508002 ./ffmpeg_g -i input.mp4 -f null -
ffmpeg version N-89396-gd1266d9 Copyright (c) 2000-2017 the FFmpeg developers
  built with clang version 5.0.0 (tags/RELEASE_500/final)
  configuration: --extra-libs='-lnullfuzzer -lbacktrace -lpthread' --malloc-prefix=fuzz_ --cc=clang
  libavutil      56.  5.100 / 56.  5.100
  libavcodec     58.  6.102 / 58.  6.102
  libavformat    58.  2.103 / 58.  2.103
  libavdevice    58.  0.100 / 58.  0.100
  libavfilter     7.  6.100 /  7.  6.100
  libswscale      5.  0.101 /  5.  0.101
  libswresample   3.  0.101 /  3.  0.101
Input #0, mov,mp4,m4a,3gp,3g2,mj2, from '../502.mp4':
  Metadata:
    major_brand     : mp42
    minor_version   : 0
    compatible_brands: mp42mp41isomavc1
    creation_time   : 2016-01-03T19:52:42.000000Z
  Duration: 00:01:04.04, start: 0.000000, bitrate: 1311 kb/s
    Stream #0:0(und): Video: h264 (High) (avc1 / 0x31637661), yuv420p(tv, bt709), 1280x720 [SAR 1:1 DAR 16:9], 1234 kb/s, 25 fps, 25 tbr, 25 tbn, 50 tbc (default)
    Metadata:
      creation_time   : 2016-01-03T19:52:42.000000Z
      handler_name    : L-SMASH Video Handler
      encoder         : AVC Coding
    Stream #0:1(und): Audio: aac (LC) (mp4a / 0x6134706D), 48000 Hz, stereo, fltp, 146 kb/s (default)
    Metadata:
      creation_time   : 2016-01-03T19:52:42.000000Z
      handler_name    : L-SMASH Audio Handler
FAILED. Iteration = 1030, Seed = 1512508002.
av_malloc:87 in libavutil/mem.c
ff_frame_thread_init:798 in libavcodec/pthread_frame.c
avcodec_open2:809 in libavcodec/utils.c
init_input_stream:2937 in fftools/ffmpeg.c
transcode_init:3675 in fftools/ffmpeg.c
transcode:4608 in fftools/ffmpeg.c
main:4843 in fftools/ffmpeg.c
Segmentation fault
```

The backtrace shows where exactly the allocation failed. This same command can be run under
GDB/LLDB to find out where the NULL derefernce occurred.
