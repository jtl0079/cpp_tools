#pragma once


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



	bool AudioDecode_open(
		const char* path,
		int* streamIndex,
		AVFormatContext*& fmtCtx,
		AVStream*& audioStream,
		const AVCodec*& codec,
		AVCodecContext*& codecCtx,
		SwrContext*& swrCtx,
		AVPacket*& pkt,
		AVFrame*& frame,
		int outSampleRate = 44100
	);


}