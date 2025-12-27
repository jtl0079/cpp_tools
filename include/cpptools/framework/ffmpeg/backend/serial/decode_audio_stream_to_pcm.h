#pragma once



#include <vector>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswresample/swresample.h>
#include <libavutil/avutil.h>
#include <libavutil/opt.h>
#include <libavutil/channel_layout.h>
#include <libavutil/samplefmt.h>
}

namespace cpptools::framework::ffmpeg::backend::serial {



	bool decode_audio_stream_to_pcm(
		std::vector<uint8_t>& pcm,
		AVFormatContext* fmtCtx,
		AVCodecContext* codecCtx,
		SwrContext* swrCtx,
		AVPacket* pkt,
		AVFrame* frame,
		int streamIndex,
		int outSampleRate = 44100);


	bool decode_audio_stream_to_pcm(
		std::vector<uint8_t>& pcm,
		AVFormatContext* fmtCtx,
		AVCodecContext* codecCtx,
		SwrContext* swrCtx,
		int streamIndex,
		int outSampleRate);


}