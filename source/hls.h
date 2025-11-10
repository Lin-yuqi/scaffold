extern "C"{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
#include <libavutil/error.h>
}
#include <memory>
#include <string>
#include <vector>
#include <iostream>


namespace linhls {
    //不用宏防止命名空间冲突
    const std::string HLS_EXTM3U = "#EXTM3U";
    const std::string HLS_VERSION = "#EXT-X-VERSION:";
    const std::string HLS_TARGETDURATION = "#EXT-X-TARGETDURATION:";
    const std::string HLS_SEQUENCE = "EXT-X-MEDIA-SEQUENCE:";
    const std::string HLS_PLAYLIST_TYPE = "EXT-X-PLAYLIST-TYPE";
    const std::string HLS_INDEPENDENT_SEGMENTS = "#EXT-X-INDEPENDENT-SEGMENTS";
    const std::string HLS_ENDLIST = "#EXT-X-ENDLIST";
    const std::string HLS_EXTINF = "#EXTINF:";

    class M3U8Info {
    //m3u8信息管理类
    public:
        using ptr =std::shared_ptr<M3U8Info>;
        using StrPair = std::pair<std::string, std::string>;
        M3U8Info(const std::string& filename);
        bool parse();
        bool write();
        std::vector<StrPair>& pieces();
        std::vector<std::string>& headers();
    private:
        std::string _filename;
        std::vector<StrPair> _pieces; //存储片段信息
        std::vector<std::string> _headers; //存储头信息
    };

    struct hls_settings{
        size_t hls_time;
        std::string playlist_type;
        std::string base_url;
        
    };

    class HLSTranscoder {
        public:
            using ptr = std::shared_ptr<HLSTranscoder>;
            HLSTranscoder(const hls_settings& settings);
            bool transcode(const std::string& input, const std::string& output);
        private:
            const char * avError(int err_code);
            hls_settings _settings;
    };
}