# Install

## Install Simple RTSP Server

Download Simple RTSP Server via : ```https://github.com/bluenviron/mediamtx/releases```

```
    wget https://github.com/bluenviron/mediamtx/releases/download/v1.9.3/mediamtx_v1.9.3_linux_amd64.tar.gz
    tar -xvzf mediamtx_v1.9.3_linux_amd64.tar.gz
```

## Run RTSP Server

```
    ./mediamtx
```

## Run RTSP Client

### Run by ffmpeg command line

```
    ffmpeg -f v4l2 -i /dev/video0 -c:v libx264 -f rtsp rtsp://localhost:8554/stream
```

### Build RTSP CLient

```
    make
```

### Run RTSP Client to push data into RTSP Server

```
    ./main
```

