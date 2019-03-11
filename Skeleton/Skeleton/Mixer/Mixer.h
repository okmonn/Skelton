#pragma once
#include "../Vector2.h"
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <unordered_map>

class Application;
class Sound;
class Mouse;
class ParamMane;
class Wave;
class Characteristic;

class Mixer
{
public:
	// �R���X�g���N�^
	Mixer();
	// �f�X�g���N�^
	~Mixer();

	// ���s
	void Execution(void);

private:
	// �`��
	void Draw(void);

	// �Đ��Ǘ�
	void Play(void);

	// ����
	void UpData(void);

	// �g�`�̕`��
	void DrawWave(void);

	// �����̕`��
	void DrawChara(void);


	// �A�v���P�[�V����
	std::shared_ptr<Application>app;

	//�T�E���h
	std::shared_ptr<Sound>sound;

	// �}�E�X
	std::shared_ptr<Mouse>mouse;

	// �p�����[�^�}�l�[�W���[
	std::unique_ptr<ParamMane>param;

	// �g�`
	std::unique_ptr<Wave>wave;
	
	// ����
	std::unique_ptr<Characteristic>chara;

	// �摜ID
	int image;

	// �Đ��t���O
	bool play;

	// �X���b�h�t���O
	bool threadFlag;

	// �X���b�h
	std::vector<std::thread>th;
};