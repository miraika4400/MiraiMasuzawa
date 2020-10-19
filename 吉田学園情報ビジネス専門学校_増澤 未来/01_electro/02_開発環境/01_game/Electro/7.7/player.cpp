
//===================================================
//
//    プレイヤークラスの処理[player.cpp]
//    Author:増澤 未来
//
//====================================================

//**********************************
//インクルード
//**********************************
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "keyboard.h"
#include "joypad.h"
#include "bullet.h"
#include "sound.h"
#include "mouse.h"
#include "cursor.h"
#include "camera.h"
#include "stage.h"
#include "game.h"
#include "fade.h"
#include "bomb.h"
#include "enemy.h"
#include "bulletex.h"
#include "effect.h"
#include "particle.h"
#include "boss.h"

//*****************************
// マクロ定義
//*****************************
#define PLAYER_SPEED 5.0f         // 移動速度
#define PLAYER_SIZE 50.0f         // サイズ
#define PLAYER_BULLET_SPEED 10    // アニメーション速度
#define PLAYER_ATTACK_COUNT 8     // 攻撃のインターバル
#define PLAYER_STATE_COUNT 10     // ステート変化時のカウント
#define PLAYER_GAMEOVER_COUNT 120 // ゲームオーバー時画面遷移までのカウント
#define PLAYER_MOVE_RATE 0.1f     // 移動の完成の係数
#define PLAYER_MAX_REMAIN 3       // 残機の最大数
#define PLAYER_BULLET_MP 2        // 一発の消費MP
#define SLOW_TIME 40              // 死亡時のスロー演出の長さ
#define DEATH_PARTICLE_NUM 50     // 死亡時のパーティクル数
#define PLAYER_TEXTURE_PATH "./data/Textures/player001.png" // テクスチャのパス

//**********************************
//静的メンバ変数宣言
//**********************************
LPDIRECT3DTEXTURE9 CPlayer::m_pTexture = NULL;
bool CPlayer::m_bAlive = false;

//==================================
// コンストラクタ
//==================================
CPlayer::CPlayer() :CScene3d(OBJTYPE_PLAYER)
{
	// クリア
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCntBullet = 0;
	m_nStateCount = 0;
	m_state = STATE_SPAWN;
	m_bBomb = false;
	m_nRemain = PLAYER_MAX_REMAIN;
	m_fMp = PLAYER_MAX_MP;
	m_nLife = PLAYER_MAX_HP;
	m_fRotAngle = 0;
	m_fRotAngleDist = 0;
}

//==================================
// デストラクタ
//==================================
CPlayer::~CPlayer()
{
}

//==================================
// クリエイト
//==================================
CPlayer * CPlayer:: Create(const D3DXVECTOR3 pos)
{
	//メ モリの確保
	CPlayer *pPlayer;
	pPlayer = new CPlayer;
	
	// 初期化
	pPlayer->Init(pos);

	// 座標の設定
	pPlayer->SetPos(pos);
	// オブジェクトタイプの設定
	pPlayer->SetObjType(OBJTYPE_PLAYER);

	return pPlayer;
}

//==================================
// テクスチャのロード
//==================================
HRESULT CPlayer::Load(void)
{

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, PLAYER_TEXTURE_PATH, &m_pTexture);

	return S_OK;
}

//==================================
// テクスチャのアンロード
//==================================
void CPlayer::Unload(void)
{
	// テクスチャの解放
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//==================================
// 初期化処理
//==================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos)
{
	if (FAILED(CScene3d::Init()))
	{
		return E_FAIL;
	}
	
	// テクスチャ割り当て
	BindTexture(m_pTexture);

	// サイズの設定
	SetSize(D3DXVECTOR3(PLAYER_SIZE, PLAYER_SIZE, 0.0f));

	// メンバ変数初期化
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCntBullet = 0;
	m_nStateCount = 0;
	m_state = STATE_SPAWN;
	m_bAlive = true;
	m_fMp = PLAYER_MAX_MP;
	m_nLife = PLAYER_MAX_HP;

	// 角度の初期化
	m_fRotAngle = 90;
	m_fRotAngleDist = 90;
	SetAngle(90);

	return S_OK;
}

//==================================
// 終了処理
//==================================
void CPlayer::Uninit(void)
{
	CScene3d::Uninit();
	m_bAlive = false;
}

//==================================
// 更新処理
//==================================
void CPlayer::Update(void)
{
	//DirectinManager();
	if (m_bAlive)
	{


		// コントロールの管理
		ControlManager();

		static float fBulletAngle = -90.0f;// 弾を飛ばす方向

		// 座標の取得
		D3DXVECTOR3 pos = GetPos();

		// 移動量を足す
		pos += m_move;

		SetPos(pos);

		// ステージ座標の取得
		D3DXVECTOR3 stagePos = STAGE_POS;
		// 距離の計算
		float fDistanceStage = sqrtf(powf(pos.x - stagePos.x, 2) + powf(pos.y - stagePos.y, 2));

		// ステージ端の設定
		if (fDistanceStage + PLAYER_SIZE / 2 > STAGE_SIZE.x / 2)
		{
			float fAngle = atan2f(pos.y - stagePos.y, pos.x - stagePos.x);

			pos = D3DXVECTOR3(cosf(fAngle)*(STAGE_SIZE.x / 2 - PLAYER_SIZE / 2), sinf(fAngle)*(STAGE_SIZE.x / 2 - PLAYER_SIZE / 2), 0.0f);

			//m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}

		if (CBoss::IsAlive())
		{// ボスが生きているとき
			

			// ボス座標の取得
			D3DXVECTOR3 bossPos = CGame::GetBoss()->GetPos();
			// ボスサイズの取得
			D3DXVECTOR3 bossSize = CGame::GetBoss()->GetSize();
			// 距離の計算
			float fDistanceBoss = sqrtf(powf(pos.x - bossPos.x, 2) + powf(pos.y - bossPos.y, 2));

			// ボスとの当たり判定
			if (fDistanceBoss - PLAYER_SIZE / 2 < bossSize.x / 2)
			{
				// ダメージ判定
				if (m_state == STATE_NORMAL)
				{
					HitAttack(1);
				}

				// ボスに埋まらないように
				float fAngle = atan2f(pos.y - bossPos.y, pos.x - bossPos.x);
				pos = D3DXVECTOR3(cosf(fAngle)*(bossSize.x / 2 + PLAYER_SIZE / 2), sinf(fAngle)*(bossSize.x / 2 + PLAYER_SIZE / 2), 0.0f);
			}
		}

		// MPの自然回復
		RecoveryMp(0.025f);

		// 座標のセット
		SetPos(pos);
	}
	// ステートの管理
	StateManager();
}

//==================================
// 描画処理
//==================================
void CPlayer::Draw(void)
{
	if (m_bAlive)
	{
		CScene3d::Draw();
	}
}

//==================================
// 弾ヒット時
//==================================
void CPlayer::HitAttack(int nDamage)
{
	// ライフを減らす
	m_nLife -= nDamage;

	if (m_nLife > 0)
	{// ライフが残っている
		m_state = STATE_DAMAGE;
	}
	else
	{// ライフが0以下

		// ライフが0以下にならないように
		m_nLife = 0;
		
		// 死亡状態に
		m_bAlive = false;
		// ゲームオーバーステート
		m_state = STATE_GAMEOVER;
	
		CManager::GetSound()->Play(CSound::LABEL_SE_DEATH);
	}
}

//==================================
// HP回復処理
//==================================
void CPlayer::RecoveryLife(const int nRecovery)
{
	m_nLife += nRecovery;
	if (m_nLife > PLAYER_MAX_HP)
	{
		m_nLife = PLAYER_MAX_HP;
	}
}

//==================================
// MP回復処理
//==================================
void CPlayer::RecoveryMp(float fRecoveryMp)
{
	m_fMp += fRecoveryMp;
	if (m_fMp > PLAYER_MAX_MP)
	{
		m_fMp = PLAYER_MAX_MP;
	}
}

//==================================
// ステート管理
//==================================
void CPlayer::StateManager(void)
{
	// ステートで分岐
	switch (m_state)
	{
	case STATE_NORMAL:
		// ノーマルステート

		// カウントの初期化
		m_nStateCount = 0;
		break;

	case STATE_DAMAGE:
		// ダメージステート

		// カウントを進める
		m_nStateCount++;

		// 一定カウント進んだら通常ステートに戻す
		if (m_nStateCount >= PLAYER_STATE_COUNT * 6)
		{
			m_nStateCount = 0;
			m_state = STATE_NORMAL;
		}
		if (m_nStateCount % (PLAYER_STATE_COUNT * 2) == 0)
		{// 通常カラー
			SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else if (m_nStateCount % PLAYER_STATE_COUNT == 0)
		{// 赤くする
			SetColor(D3DXCOLOR(1.0f, 0.3f, 0.3f, 1.0f));
		}

		break;

	case STATE_SPAWN:
		// スポーンステート
		
		// カウントを進める
		m_nStateCount++;

		// 一定カウント進んだら通常ステートに戻す
		if (m_nStateCount >= PLAYER_STATE_COUNT * 12)
		{
			m_nStateCount = 0;
			m_state = STATE_NORMAL;
		}
		
		if (m_nStateCount % (PLAYER_STATE_COUNT * 2) == 0)
		{// 通常カラー
			SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else if (m_nStateCount % PLAYER_STATE_COUNT == 0)
		{// 半透明
			SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
		}

		break;

	case STATE_GAMEOVER:
		// ゲームオーバー
		m_nStateCount++;
		if (m_nStateCount <= 50)
		{
			// スロー演出
			Sleep(SLOW_TIME);

			int nRadSize = rand() % 20 + 20;
			// パーティクル生成
			CParticle::Create(GetPos(),
				D3DXVECTOR3(rand() % 10 - 5, rand() % 10 - 5, 0.0f),
				D3DXVECTOR3(nRadSize, nRadSize, 0.0f),
				SLOW_TIME + 20,
				D3DXCOLOR((float)(rand() % 100) / 100.0f, 1.0f, (float)(rand() % 100) / 100.0f, 0.8f),
				CParticle::PARTICLE_CIRCLE);
			/*(CParticle::PARTICLE_TYPE)(rand() % CParticle::PARTICLE_MAX)*/

		}
		if (m_nStateCount > PLAYER_GAMEOVER_COUNT)
		{
			// シーン 遷移
			CManager::GetFade()->SetFade(CManager::MODE_RESULT);
			
		}
		break;
	default:
		break;
	}
}


//==================================
// キャラコントロールの管理
//==================================
void CPlayer::ControlManager(void)
{
	//移動量の目標値
	D3DXVECTOR3 moveDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// スティックの取得
	DIJOYSTATE js = CInputJoypad::GetStick(0);

	if (CManager::GetKeyboard()->GetKeyPress(DIK_W) || js.lY <= -600)
	{// ↑移動
		m_fRotAngleDist = 90.0f;
		moveDest.y = PLAYER_SPEED;


		if (CManager::GetKeyboard()->GetKeyPress(DIK_A) || js.lX <= -600)
		{
			m_fRotAngleDist = 135.0f;

			moveDest.y = sinf(45) * PLAYER_SPEED;
			moveDest.x = cosf(45) * -PLAYER_SPEED;
		}
		if (CManager::GetKeyboard()->GetKeyPress(DIK_D) || js.lX >= 600)
		{
			m_fRotAngleDist = 45.0f;

			moveDest.y = sinf(45) * PLAYER_SPEED;
			moveDest.x = cosf(45) * PLAYER_SPEED;
		}
	}
	else if (CManager::GetKeyboard()->GetKeyPress(DIK_S) || js.lY >= 600)
	{// ↓移動

		m_fRotAngleDist = -90.0f;

		moveDest.y = -PLAYER_SPEED;

		if (CManager::GetKeyboard()->GetKeyPress(DIK_A) || js.lX <= -600)
		{
			m_fRotAngleDist = -135.0f;

			moveDest.y = sinf(45) * -PLAYER_SPEED;
			moveDest.x = cosf(45) * -PLAYER_SPEED;
		}
		if (CManager::GetKeyboard()->GetKeyPress(DIK_D) || js.lX >= 600)
		{
			m_fRotAngleDist = -45.0f;

			moveDest.y = sinf(45) * -PLAYER_SPEED;
			moveDest.x = cosf(45) * PLAYER_SPEED;
		}
	}
	else if (CManager::GetKeyboard()->GetKeyPress(DIK_A) || js.lX <= -600)
	{// ←移動
		m_fRotAngleDist = 180.0f;
		moveDest.x = -PLAYER_SPEED;

	}
	else if (CManager::GetKeyboard()->GetKeyPress(DIK_D) || js.lX >= 600)
	{// →移動
		m_fRotAngleDist = 0.0f;
		moveDest.x = PLAYER_SPEED;
	}

	// 回転しすぎないように差を調整
	while (m_fRotAngleDist - m_fRotAngle > 180.0f)
	{
		m_fRotAngleDist -= 360.0f;
	}
	while (m_fRotAngleDist - m_fRotAngle < -180.0f)
	{
		m_fRotAngleDist += 360.0f;
	}

	m_fRotAngle += (m_fRotAngleDist - m_fRotAngle)*0.1f;
	SetAngle((float)m_fRotAngle);
	//m_bAnim = true;

	D3DXVECTOR3 cursorPos = CGame::GetCursor()->GetPos();

	// カーソルの角度
	float fCoursorAngle = atan2f(SCREEN_HEIGHT / 2 - cursorPos.y, cursorPos.x - SCREEN_WIDTH / 2);

	//moveDest = D3DXVECTOR3(cosf(fCoursorAngle)*PLAYER_SPEED, sinf(fCoursorAngle)*PLAYER_SPEED, 0.0f);

	// 慣性
	m_move += (moveDest - m_move) * PLAYER_MOVE_RATE;

	if (CManager::GetMouse()->GetMouseTrigger(0) || CManager::GetJoypad()->GetJoystickTrigger(7, 0))
	{
		m_nCntBullet = PLAYER_ATTACK_COUNT - 1;
	}
	// マウスで弾の発射
	if (CManager::GetMouse()->GetMousePress(0)/* || CManager::GetJoypad()->GetJoystickPress(7, 0)*/)
	{
		// 弾のインターバルのカウント
		m_nCntBullet++;
		if (m_nCntBullet > PLAYER_ATTACK_COUNT)
		{
			// MPの確認
			if (m_fMp >= PLAYER_BULLET_MP)
			{
				// MPを減らす
				m_fMp -= PLAYER_BULLET_MP;
				if (m_fMp <= 0)
				{
					m_fMp = 0;
				}

				D3DXVECTOR3 cursorPos = CGame::GetCursor()->GetPos();

				// 弾の発射角度
				float fBulletAngle = atan2f(SCREEN_HEIGHT / 2 - cursorPos.y, cursorPos.x - SCREEN_WIDTH / 2);

				// 弾の生成
				CBullet::Create(
					GetPos(),
					D3DXVECTOR3(cosf(fBulletAngle)*PLAYER_BULLET_SPEED, sinf(fBulletAngle)*PLAYER_BULLET_SPEED, 0.0f),
					60,
					CBullet::BULLETUSER_PLAYER,
					D3DXCOLOR(0.5f, 1.0f, 0.5f, 1.0f));


				// SEの再生
				CManager::GetSound()->Play(CSound::LABEL_SE_SHOT);

			}
			// 弾のインターバルの初期化
			m_nCntBullet = 0;

		}
	}
	// ジョイスティックで弾の発射

	if (js.lRz >= 10 || js.lRz <= -10 || js.lZ >= 10 || js.lZ <= -10)
	{
		// 弾のインターバルのカウント
		m_nCntBullet++;
		if (m_nCntBullet > PLAYER_ATTACK_COUNT)
		{
			// MPの確認
			if (m_fMp >= PLAYER_BULLET_MP)
			{
				// MPを減らす
				m_fMp -= PLAYER_BULLET_MP;
				if (m_fMp <= 0)
				{
					m_fMp = 0;
				}

				D3DXVECTOR3 bulletMove = D3DXVECTOR3(cosf(45)*js.lZ, -(sinf(45)*js.lRz), 0.0f);
				D3DXVec3Normalize(&bulletMove, &bulletMove);
				// 弾の生成
				CBullet::Create(
					GetPos(),
					bulletMove*PLAYER_BULLET_SPEED,
					60,
					CBullet::BULLETUSER_PLAYER,
					D3DXCOLOR(0.5f, 1.0f, 0.5f, 1.0f));


				// SEの再生
				CManager::GetSound()->Play(CSound::LABEL_SE_SHOT);

			}
			// 弾のインターバルの初期化
			m_nCntBullet = 0;

		}
	}

	// ボムが使用できるか
	if (m_bBomb)
	{
		if (CManager::GetMouse()->GetMouseTrigger(1) || CManager::GetJoypad()->GetJoystickTrigger(6, 0))
		{
			// ボムの生成
			CBomb::Create(GetPos(),500.0f);

			m_bBomb = false;
		}
	}
}
