#pragma once
#include <vector>
#include <memory>
#include <thread>

class Application;
class Sound;
class Waveform;
class Characteristic;

class Mixer
{
public:
	// �R���X�g���N�^
	Mixer();
	// �f�X�g���N�^
	~Mixer();

	// ����
	void Run(void);

private:
	// ������
	void Init(void);

	// �`��
	void Draw(void);

	// ����
	void UpData(void);

	// �T�E���h�̔g�`���g���̕`��
	void DrawWave(void);

	// ���g�������̕`��
	void DrawChara(void);


	// �A�v���P�[�V����
	std::shared_ptr<Application>app;
	
	// �T�E���h
	std::shared_ptr<Sound>sound;

	// �g�`
	std::unique_ptr<Waveform>wave;

	// ���g������
	std::unique_ptr<Characteristic>chara;

	// �Đ��t���O
	bool playFlag;

	// �X���b�h�t���O
	bool threadFlag;

	// �X���b�h
	std::vector<std::thread>th;
};