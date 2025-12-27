
#include <cpptools/framework/ffmpeg/backend/serial/AudioDecode_open.h>

#include <cpptools/framework/ffmpeg/backend/serial/CodecContext_init.h>
#include <cpptools/framework/ffmpeg/backend/serial/FormatContext_init.h>
#include <cpptools/framework/ffmpeg/backend/serial/SwrContext_Init.h>

#include <iostream>

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
		int outSampleRate
	) {
		namespace cffmpeg = cpptools::framework::ffmpeg::backend::serial;

		// ---------- validate ----------
		if (!streamIndex) {
			return false;
		}

		// ---------- format ----------
		if (!cffmpeg::FormatContext_init(fmtCtx, path)) {
			return false;
		}

		// ---------- find audio stream ----------
		*streamIndex = av_find_best_stream(
			fmtCtx,
			AVMEDIA_TYPE_AUDIO,
			-1, -1, nullptr, 0
		);
		if (*streamIndex < 0) {
			return false;
		}

		audioStream = fmtCtx->streams[*streamIndex];

		// ---------- decoder ----------
		codec = avcodec_find_decoder(audioStream->codecpar->codec_id);
		if (!codec) {
			return false;
		}

		if (!cffmpeg::CodecContext_init(codecCtx, audioStream, codec)) {
			return false;
		}

		// ---------- resampler ----------
		if (!cffmpeg::SwrContext_Init(swrCtx, codecCtx, outSampleRate)) {
			return false;
		}

		// ---------- packet / frame ----------
		pkt = av_packet_alloc();
		frame = av_frame_alloc();
		if (!pkt || !frame) {
			return false;
		}

		return true;
	}

}