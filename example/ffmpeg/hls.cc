extern "C"{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
#include <libavutil/error.h>
}
#include <iostream>

const char* mError(int en)
{
    static char errbuf[256];
    av_strerror(en, errbuf, sizeof(errbuf));
    return errbuf;
}

int main(int argc, char* argv[]) {
    if(argc!=3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_hls_playlist>\n";
        return -1;
    }
    int ret = 0;
    // 1. 打开输⼊⽂件，并初始化输⼊格式化对象
    AVFormatContext *inputContext = nullptr, *outputContext = nullptr;//上下文对象初始化成空
    ret=avformat_open_input(&inputContext, argv[1], nullptr, nullptr);
    if(ret<0) {
        std::cout<<"打开输入文件失败："<<mError(ret)<<std::endl;
        return -1;
    }
    // 2. 查找输⼊视频参数
    ret=avformat_find_stream_info(inputContext, nullptr);
    if(ret<0) {
        std::cout<<"解析输入文件元信息失败："<<mError(ret)<<std::endl;
        return -1;
    }
    // 3. 申请输出格式化上下文对象
    ret=avformat_alloc_output_context2(&outputContext, nullptr, "hls", argv[2]);
    if(ret<0) {
        std::cout<<"创建输出上下文对象失败："<<mError(ret)<<std::endl;
        return -1;
    }
    // 4. 遍历输⼊媒体流
    for(unsigned int i=0;i<inputContext->nb_streams;i++) {
        // a. 为输出对象申请媒体流
        AVStream *inStream = inputContext->streams[i];
        AVStream *outStream = avformat_new_stream(outputContext, nullptr);
        // b. 从输⼊媒体流复制解码器参数到输出媒体流中
        ret=avcodec_parameters_copy(outStream->codecpar, inStream->codecpar);
        if(ret<0) {
            std::cout<<"复制解码器参数失败："<<mError(ret)<<std::endl;
            return -1;
        }
    }
    // 5. 设置分⽚字典选项
    AVDictionary* options = nullptr;
    av_dict_set_int(&options, "hls_time", 10, 0);
    av_dict_set(&options, "url","/vidio/",0);
    av_dict_set(&options, "hls_playlist_type", "vod", 0);
    // 6. 通过输出格式化对象，输出媒体头部信息
    ret=avformat_write_header(outputContext, &options);
    if(ret<0) {
        std::cout<<"写入输出文件头信息失败："<<mError(ret)<<std::endl;
        return -1;
    }
    // 7. 遍历输⼊流中的数据帧
    AVPacket pkt;
    while(av_read_frame(inputContext, &pkt)>=0) {
        AVStream *inStream = inputContext->streams[pkt.stream_index];
        AVStream *outStream = outputContext->streams[pkt.stream_index];
        // a. 将数据包中的时间戳，从输⼊流的时间基转换为输出流的时间基的时间戳
        if(pkt.pts==AV_NOPTS_VALUE){
            // i. 若数据帧的显⽰时间戳⽆效(AV_NOPTS_VALUE)，则默认为从0开始的默认时间基
            pkt.pts = pkt.dts = av_rescale_q(0, AV_TIME_BASE_Q, inStream->time_base);
        }
        av_packet_rescale_ts(&pkt, inStream->time_base, outStream->time_base);
        // b. 将数据帧写⼊输出格式化对象中
        ret=av_interleaved_write_frame(outputContext, &pkt);
        if(ret<0) {
            std::cout<<"写入输出文件数据帧失败："<<mError(ret)<<std::endl;
            return -1;
        }
        // c. 释放帧结构资源
        av_packet_unref(&pkt);
    }
    // 8. 通过输出格式化对象，输出媒体尾部信息
    ret=av_write_trailer(outputContext);
    if(ret<0) {
        std::cout<<"写入输出文件尾部信息失败："<<mError(ret)<<std::endl;
        return -1;  
    }
    // 9. 释放资源：字典选项，输⼊格式化对象，输出格式化对象
    av_dict_free(&options);
    avformat_free_context(outputContext);
    avformat_close_input(&inputContext);
    return 0;
}