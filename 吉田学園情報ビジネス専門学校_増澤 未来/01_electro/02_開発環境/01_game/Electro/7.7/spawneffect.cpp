//===================================================
//
//    エネミークラスの処理[enemy.cpp]
//    Author:増澤 未来
//
//====================================================

//**********************************
//インクルード
//**********************************
#include "spawneffect.h"
#include "particle.h"

//**********************************
//マクロ定義
//**********************************
#define PARTICLE_SIZE D3DXVECTOR3(50.0f,50.0f,0.0f) // パーティクルサイズ
#define PARTICLE_COL D3DXCOLOR(1.0f,1.0f,1.0f,1.0f) // パーティクル初期カラー
#define PARTICLE_DISTANCE 80                        // パーティクルの出現距離
#define PARTICLE_SPEED 4                            // パーティクルの移動スピード
#define PARTICLE_LIFE 20                            // パーティクルの有効フレーム
#define PARTICLE_RATE (1.0f/(float)PARTICLE_LIFE)      // 係数

#define COLOR_YELLOW D3DXCOLOR(1.0f,1.0f,0.0f,1.0f)	// 黄色
#define COLOR_PURPLE D3DXCOLOR(1.0f,0.0f,1.0f,1.0f) // 紫
#define COLOR_RED D3DXCOLOR(1.0f,0.0f,0.0f,1.0f)    // 赤

//==================================
// コンストラクタ
//==================================
CEnemySpawn::CEnemySpawn() :CScene(OBJTYPE_PARTICLE)
{
	// メンバ変数のクリア
	memset(&m_pParticle, 0, sizeof(m_pParticle));
	m_enemyType = CEnemy::ENEMYTYPE_YELLOW;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCntFrame = 0;
}

//==================================
// デストラクタ
//==================================
CEnemySpawn::~CEnemySpawn()
{
}

//==================================
// クリエイト
//==================================
CEnemySpawn * CEnemySpawn::Create(const D3DXVECTOR3 pos, const CEnemy::ENEMYTYPE type)
{
	// メモリの確保
	CEnemySpawn *pEnemySpawn = new CEnemySpawn;

	// 座標の設定
	pEnemySpawn->m_pos = pos;
	// タイプ設定
	pEnemySpawn->m_enemyType = type;
	
	// 初期化
	pEnemySpawn->Init();
	
	return pEnemySpawn;
}


//==================================
// 初期化処理
//==================================
HRESULT CEnemySpawn::Init(void)
{
	// パーティクルを六つ生成する
	for (int nCntPart = 0; nCntPart < ENEMYSPAWN_PARTICLE_NUM; nCntPart++)
	{
		// 座標の設定
		D3DXVECTOR3 partPos;
		// 6方向
		partPos.x = m_pos.x + cosf(nCntPart* D3DXToRadian(360 / ENEMYSPAWN_PARTICLE_NUM))*PARTICLE_DISTANCE;
		partPos.y = m_pos.y + sinf(nCntPart* D3DXToRadian(360 / ENEMYSPAWN_PARTICLE_NUM))*PARTICLE_DISTANCE;
		partPos.z = m_pos.z;

		// 中心に向かって移動
		D3DXVECTOR3 partMove;
		partMove = m_pos - partPos;
		D3DXVec3Normalize(&partMove, &partMove);
		// スピードをかける
		partMove *= PARTICLE_SPEED;

		// パーティクルの生成
		m_pParticle[nCntPart] = CParticle::Create(partPos, partMove, PARTICLE_SIZE, PARTICLE_LIFE, PARTICLE_COL);
		// 現在の色
		m_col = PARTICLE_COL;
	}

	m_nCntFrame = 0;
	return S_OK;
}

//==================================
// 終了処理
//==================================
void CEnemySpawn::Uninit(void)
{
	// 終了処理
	for (int nCntPart = 0; nCntPart < ENEMYSPAWN_PARTICLE_NUM; nCntPart++)
	{
		if (m_pParticle[nCntPart] != NULL)
		{
			//m_pParticle[nCntPart]->Uninit();
			m_pParticle[nCntPart] = NULL;
		}
	}
	Release();
}

//==================================
// 更新処理
//==================================
void CEnemySpawn::Update(void)
{
	
	for (int nCntPart = 0; nCntPart < ENEMYSPAWN_PARTICLE_NUM; nCntPart++)
	{
		// 角度を0度に向かって収束させる

		// 角度の取得
		float fAngele = m_pParticle[nCntPart]->GetAngle();
		// 0度に向かって
		fAngele += (0.0f - fAngele)*PARTICLE_RATE;
		// 角度のセット
		m_pParticle[nCntPart]->SetAngle(fAngele);

		// 白から敵の種類によって色を分ける

		if (m_nCntFrame >= 8)
		{
			// 敵の種類で分岐
			switch (m_enemyType)
			{
			case CEnemy::ENEMYTYPE_YELLOW:
				// 黄色
				m_col.r += (COLOR_YELLOW.r - m_col.r)*PARTICLE_RATE;
				m_col.g += (COLOR_YELLOW.g - m_col.g)*PARTICLE_RATE;
				m_col.b += (COLOR_YELLOW.b - m_col.b)*PARTICLE_RATE;

				break;
			case CEnemy::ENEMYTYPE_PURPLE:
				// 紫
				m_col.r += (COLOR_PURPLE.r - m_col.r)*PARTICLE_RATE;
				m_col.g += (COLOR_PURPLE.g - m_col.g)*PARTICLE_RATE;
				m_col.b += (COLOR_PURPLE.b - m_col.b)*PARTICLE_RATE;
				break;
			case CEnemy::ENEMYTYPE_RED:
				// 赤
				m_col.r += (COLOR_RED.r - m_col.r)*PARTICLE_RATE;
				m_col.g += (COLOR_RED.g - m_col.g)*PARTICLE_RATE;
				m_col.b += (COLOR_RED.b - m_col.b)*PARTICLE_RATE;
				break;
			default:
				break;
			}
			// 色のセット
			m_pParticle[nCntPart]->SetColor(m_col);
		}
	}

	// カウントを進める
	m_nCntFrame++;

	// カウントがパーティクルのライフよりちょっと低いとき
	if (m_nCntFrame >= PARTICLE_LIFE - 2)
	{
		// エネミーの生成
		CEnemy::Create(m_pos, m_enemyType);
		// 終了処理
		Uninit();
	}

	
}

//==================================
// 描画処理
//==================================
void CEnemySpawn::Draw(void)
{

}