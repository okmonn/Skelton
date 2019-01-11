#pragma once
#include <memory>

class Window;
class Queue;
class List;
class Fence;
class Swap;

class Application
{
public:
	// �f�X�g���N�^
	~Application();

	// �C���X�^���X�ϐ��̎擾
	static Application& Get(void) {
		static Application instance;
		return instance;
	}

	// ���b�Z�[�W�̊m�F
	bool CheckMsg(void);

	// �L�[����
	bool CheckKey(const int& key);

private:
	// �R���X�g���N�^
	Application();
	Application(const Application&) = delete;
	void operator=(const Application&) = delete;

	// �N���X�̐���
	void Create(void);


	// �E�B���h�E
	std::shared_ptr<Window>win;

	// �L���[
	std::shared_ptr<Queue>queue;

	// ���X�g
	std::shared_ptr<List>list;

	// �t�F���X
	std::shared_ptr<Fence>fence;

	// �X���b�v
	std::shared_ptr<Swap>swap;
};