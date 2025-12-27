#include <cpptools/framework/ffmpeg/backend/serial/decode_audio_stream_to_pcm.h>
#include <cpptools/framework/ffmpeg/backend/serial/frame_to_pcm.h>
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


	bool decode_audio_stream_to_pcm(
		std::vector<uint8_t>& pcm,
		AVFormatContext* fmtCtx,
		AVCodecContext* codecCtx,
		SwrContext* swrCtx,
		AVPacket* pkt,
		AVFrame* frame,
		int streamIndex,
		int outSampleRate 
	) {
		namespace cffmpeg = cpptools::framework::ffmpeg::backend::serial;

		if (!fmtCtx || !codecCtx || !swrCtx)
			return false;
		const int inRate = codecCtx->sample_rate;
		const int channels = codecCtx->ch_layout.nb_channels;
		const AVSampleFormat outFmt = AV_SAMPLE_FMT_S16;
		const int bytesPerSample = av_get_bytes_per_sample(outFmt);

		std::vector<uint8_t> temp;
		temp.reserve(8192 * channels * bytesPerSample);

		pcm.clear();
		pcm.reserve(1048576); // 1024 * 1024, 先给 1MB，减少 realloc

		/* ================= decode loop ================= */

		while (av_read_frame(fmtCtx, pkt) >= 0) {

			if (pkt->stream_index != streamIndex) {
				av_packet_unref(pkt);
				continue;
			}

			if (avcodec_send_packet(codecCtx, pkt) < 0) {
				av_packet_unref(pkt);
				continue;
			}

			av_packet_unref(pkt);

			while (avcodec_receive_frame(codecCtx, frame) == 0) {

				cffmpeg::frame_to_pcm(
					frame,
					swrCtx,
					pcm,
					temp,
					inRate,
					outSampleRate,
					channels,
					bytesPerSample
				);
			}
		}

		/* ================= flush decoder ================= */

		int ret = avcodec_send_packet(codecCtx, nullptr);
		if (ret < 0)
			return false;

		while (avcodec_receive_frame(codecCtx, frame) == 0) {

			cffmpeg::frame_to_pcm(
				frame,
				swrCtx,
				pcm,
				temp,
				inRate,
				outSampleRate,
				channels,
				bytesPerSample
			);
		}

		/* ================= flush swr ================= */   // ★ CHANGED

		int outSamples = static_cast<int>(
			av_rescale_rnd(
				swr_get_delay(swrCtx, inRate),
				outSampleRate,
				inRate,
				AV_ROUND_UP
			));

		if (outSamples > 0) {
			int needed = outSamples * channels * bytesPerSample;
			if ((int)temp.size() < needed)
				temp.resize(needed);

			uint8_t* outArr[1] = { temp.data() };

			int converted = swr_convert(
				swrCtx,
				outArr,
				outSamples,
				nullptr,
				0
			);

			if (converted > 0) {
				int dataSize = converted * channels * bytesPerSample;
				pcm.insert(pcm.end(), temp.begin(), temp.begin() + dataSize);
			}
		}


		return true;
	}

	bool decode_audio_stream_to_pcm(
		std::vector<uint8_t>& pcm,
		AVFormatContext* fmtCtx,
		AVCodecContext* codecCtx,
		SwrContext* swrCtx,
		int streamIndex,
		int outSampleRate
	) {
		if (!fmtCtx || !codecCtx || !swrCtx)
			return false;

		AVPacket* pkt = av_packet_alloc();
		AVFrame* frame = av_frame_alloc();

		if (!pkt || !frame) {
			av_packet_free(&pkt);
			av_frame_free(&frame);
			return false;
		}

		bool ok = decode_audio_stream_to_pcm(
			pcm,
			fmtCtx,
			codecCtx,
			swrCtx,
			pkt,
			frame,
			streamIndex,
			outSampleRate
		);

		av_packet_free(&pkt);
		av_frame_free(&frame);

		return ok;
	}

}