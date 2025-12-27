#include <cpptools/framework/ffmpeg/backend/serial/frame_to_pcm.h>

#include <iostream>
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
	) {
		int outSamples = static_cast<int>(
			av_rescale_rnd(
				swr_get_delay(swrCtx, inRate) +
				frame->nb_samples,
				outRate,
				inRate,
				AV_ROUND_UP
			));

		int needed = outSamples * channels * bytesPerSample;

		if (static_cast<int>(temp.size()) < needed)
			temp.resize(needed);

		uint8_t* outArr[1] = { temp.data() };

		int converted =
			swr_convert(
				swrCtx,
				outArr,
				outSamples,
				(const uint8_t**)frame->data,
				frame->nb_samples
			);

		if (converted > 0) {
			int dataSize = converted * channels * bytesPerSample;
			pcm.insert(pcm.end(), temp.begin(), temp.begin() + dataSize);
		}
	}


}