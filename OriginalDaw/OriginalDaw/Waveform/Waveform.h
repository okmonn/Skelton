#pragma once
#include <memory>

class Application;
class Sound;

class Waveform
{
public:
	// コンストラクタ
	Waveform(std::weak_ptr<Application>app, std::weak_ptr<Sound>sound);
	// デストラクタ
	~Waveform();

	// 描画
	void Draw();

	// 処理
	void UpData(void);

private:
	// アプリケーション
	std::unique_ptr<Application>app;

	// サウンド
	std::weak_ptr<Sound>sound;
};
