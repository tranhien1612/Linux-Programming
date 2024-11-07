#include <opencv2/highgui.hpp>
// #include <stdexcept.h>
#include <iostream>

extern "C"
{
#include <libswscale/swscale.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

#pragma comment(lib, "swscale.lib")
#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "avutil.lib")
#pragma comment(lib, "avformat.lib")

using namespace std;
using namespace cv;

#define STREAM_URL "rtsp://localhost:8554/stream"
int main(int argc, char* argv[]){
    // char* outUrl = "rtsp://localhost:8554/stream"; //"rtsp://0.0.0.0:8554/test";

    avcodec_register_all();
    av_register_all();
    avformat_network_init();

    SwsContext *vsc = NULL;
    AVFrame *yuv = NULL;
    AVCodecContext *vc = NULL;
    AVFormatContext *ic = NULL;

    try{
        // Step 1
        VideoCapture camera(0);
        if(!camera.isOpened()){
            printf("cam open failed!\n");
        }
        Mat frame;
        camera >> frame;

        int inWidth = camera.get(CAP_PROP_FRAME_WIDTH); //frame.cols;
        int inHeight = camera.get(CAP_PROP_FRAME_HEIGHT); //frame.rows;
        int fps = camera.get(CAP_PROP_FPS); //25;
        // cout << "Frame: " << inWidth << " " << inHeight << " " << fps << endl;

        // Step 2    
        vsc = sws_getCachedContext(vsc,
            inWidth, inHeight, AV_PIX_FMT_BGR24, //AV_PIX_FMT_GRAY8, //
            inWidth, inHeight, AV_PIX_FMT_YUV420P, //AV_PIX_FMT_YUV420P,
            SWS_BICUBIC, 0, 0, 0
        );

        if(!vsc){
            throw logic_error("sws_getCachedContext failed\n");
        }

        // Step 3
        yuv = av_frame_alloc();
        yuv->format = AV_PIX_FMT_YUV420P;
        yuv->width = inWidth;
        yuv->height = inHeight;
        yuv->pts = 0;

        int ret = av_frame_get_buffer(yuv, 32);
        if(ret != 0){
            char buf[1024] = {0};
            av_strerror(ret, buf, sizeof(buf) - 1);
            throw logic_error(buf);
        }

        // Step 4
        AVCodec *codec = avcodec_find_encoder(AV_CODEC_ID_H264);
        if(!codec){
            throw logic_error("Can't find h264 encoder");
        }

        vc = avcodec_alloc_context3(codec);
        if(!vc){
            throw logic_error("avcodec_alloc_context3 failed");
        }

        vc->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
        vc->codec_id = codec->id;
        vc->thread_count = 8;
        vc->bit_rate = 20 * 1024 * 8;
        vc->width = inWidth;
        vc->height = inHeight;
        vc->time_base = {1, fps};
        vc->framerate = {fps, 1};
        vc->gop_size = 50;
        vc->max_b_frames = 0;
        vc->pix_fmt = AV_PIX_FMT_YUV420P;
        ret = avcodec_open2(vc, 0, 0);
        if(ret != 0){
            char buf[1024] = {0};
            av_strerror(ret, buf, sizeof(buf) - 1);
            throw logic_error(buf);
        }

        // Step 5
        ret = avformat_alloc_output_context2(&ic, 0, "rtsp", STREAM_URL);
        if(ret != 0){
            char buf[1024] = {0};
            av_strerror(ret, buf, sizeof(buf) - 1);
            throw logic_error(buf);
        }
        AVStream *vs = avformat_new_stream(ic, codec);
        if(!vs){
            throw logic_error("avformat_new_stream failed");
        }
        vs->codecpar->codec_tag = 0;
        avcodec_parameters_from_context(vs->codecpar, vc);
        av_dump_format(ic, 0, STREAM_URL, 1);

        ret = avformat_write_header(ic, NULL);
        if(ret != 0){
            char buf[1024] = {0};
            av_strerror(ret, buf, sizeof(buf) - 1);
            throw logic_error(buf);
        }

        AVPacket pack;
        memset(&pack, 0, sizeof(pack));
        int vpts = 0;
        while(1){
            camera >> frame;
            uint8_t *indata[AV_NUM_DATA_POINTERS] = {0};
            indata[0] = frame.data;
            int insize[AV_NUM_DATA_POINTERS] = {0};
            insize[0] = frame.cols * frame.elemSize();
            int h = sws_scale(vsc, indata, insize, 0, frame.rows, yuv->data, yuv->linesize);
            if(h < 0){
                continue;
            }
            yuv->pts = vpts;
            vpts++;
            
            ret = avcodec_send_frame(vc, yuv);
            // if(ret != 0){
            //     continue;
            // }
            
            ret = avcodec_receive_packet(vc, &pack);
            // if (ret != 0 || pack.size > 0){
            //     cout << "*" << pack.size << endl;
            // }else{
            //     continue;
            // }

            int firstFrame = 0;
            if(pack.dts < 0 || pack.pts < 0 || pack.dts > pack.pts || firstFrame){
                firstFrame = 0;
                pack.dts = pack.pts =pack.duration = 0;
            }
            pack.pts = av_rescale_q(pack.pts, vc->time_base, vs->time_base);
            pack.dts = av_rescale_q(pack.dts, vc->time_base, vs->time_base);
            pack.duration = av_rescale_q(pack.duration, vc->time_base, vs->time_base);
            ret = av_interleaved_write_frame(ic, &pack);

        }
    }
    catch(logic_error &ex){
        if(vsc){
            sws_freeContext(vsc);
            vsc = NULL;
        }
        if(vc){
            avio_closep(&ic->pb);
            avcodec_free_context(&vc);
        }
        cerr << ex.what() << endl;
    }
    getchar();
    return 0;
}
