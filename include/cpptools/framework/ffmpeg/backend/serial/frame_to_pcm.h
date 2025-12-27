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

	
	void frame_to_pcm(
		AVFrame* frame,
		SwrContext* swrCtx,
		std::vector<uint8_t>& pcm,
		std::vector<uint8_t>& temp,
		int inRate,
		int outRate,
		int channels,
		int bytesPerSample
	);


}