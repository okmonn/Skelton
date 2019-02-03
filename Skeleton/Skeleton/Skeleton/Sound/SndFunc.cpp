#include "SndFunc.h"
#include "../Useful/Useful.h"
#include <algorithm>

// short型のオーバーフローの防止
#define OVERFLLOW_SHORT 32768.0f

// char型のオーバーフローの防止
#define OVERFLLOW_CHAR 127.0f

// RIFFの読み込み
int snd::LoadRIFF(RIFF & riff, FILE * file)
{
	if (file == nullptr)
	{
		return -1;
	}

	fread(&riff, sizeof(RIFF), 1, file);
	if (use::CheckChar("RIFF", riff.id, _countof(riff.id)) == false
		|| use::CheckChar("WAVE", riff.type, _countof(riff.type)) == false)
	{
		fclose(file);
		return -1;
	}

	return 0;
}

// FMTの読み込み
int snd::LoadFMT(FMT & fmt, FILE * file)
{
	if (file == nullptr)
	{
		return -1;
	}

	fread(&fmt, sizeof(FMT), 1, file);
	if (use::CheckChar("fmt ", fmt.id, _countof(fmt.id)) == false)
	{
		fclose(file);
		return -1;
	}

	//拡張部分の読み込み
	std::vector<unsigned char>extended(fmt.size - (sizeof(fmt) - sizeof(fmt.id) - sizeof(fmt.size)));
	if (extended.size() > 0)
	{
		fread(extended.data(), sizeof(unsigned char) * extended.size(), 1, file);
	}

	return 0;
}

// DATAの読み込み
int snd::LoadDATA(DATA & data, FILE * file)
{
	if (file == nullptr)
	{
		return -1;
	}

	//ダミー宣言 
	std::string chunkID("1234");
	while (true)
	{
		fread(&chunkID[0], sizeof(unsigned char) * chunkID.size(), 1, file);

		//DATAチャンク発見
		if (chunkID == "data")
		{
			break;
		}
		//その他のチャンク
		else
		{
			unsigned long size = 0;
			fread(&size, sizeof(size), 1, file);

			std::vector<unsigned char>data(size);
			fread(data.data(), sizeof(unsigned char) * data.size(), 1, file);
		}
	}

	//DATAの読み込み 
	data.id = chunkID;
	fread(&data.size, sizeof(data.size), 1, file);

	return 0;
}

// 8ビット波形の読み込み
int snd::LoadWave8(std::vector<float>& data, FILE * file)
{
	if (file == nullptr)
	{
		return -1;
	}
	std::vector<unsigned char>tmp(data.size());
	fread(tmp.data(), sizeof(unsigned char) * tmp.size(), 1, file);
	data.assign(tmp.begin(), tmp.end());
	std::for_each(data.begin(), data.end(), [&](float& i)->void {
		i /= OVERFLLOW_CHAR;
	});

	return 0;
}

// 16ビット波形の読み込み
int snd::LoadWave16(std::vector<float>& data, FILE * file)
{
	if (file == nullptr)
	{
		return -1;
	}
	std::vector<short>tmp(data.size());
	fread(tmp.data(), sizeof(short) * tmp.size(), 1, file);
	data.assign(tmp.begin(), tmp.end());
	std::for_each(data.begin(), data.end(), [&](float& i)->void {
		i /= OVERFLLOW_SHORT;
	});

	return 0;
}

std::vector<float> snd::Test(const std::vector<float>& input)
{
	//閾値
	const float thd = 0.5f;
	std::vector<float>real, imag;
	use::FFT(input, real, imag);

	unsigned int index = 0;
	std::vector<float>a(real.size());
	std::for_each(a.begin(), a.end(), [&](float& i)->void {
		i = sqrt(real[index] * real[index] + imag[index] * imag[index]);
		++index;
	});
	index = 0;
	std::vector<float>b(imag.size());
	std::for_each(b.begin(), b.end(), [&](float& i)->void {
		i = (float)atan2(static_cast<int>(imag[index]), static_cast<int>(real[index]));
		++index;
	});

	index = 0;
	std::for_each(real.begin(), real.end(), [&](float& i)->void {
		a[index] -= thd;
		if (a[index] < 0.0f)
		{
			a[index] = 0.0f;
		}
		i = a[index] * cos(b[index]);
		++index;
	});

	index = 0;
	std::for_each(imag.begin(), imag.end(), [&](float& i)->void {
		i = a[index] * sin(b[index]);
		++index;
	});

	return use::IFFT(real, imag, input.size());
}
