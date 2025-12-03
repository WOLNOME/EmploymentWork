#include "CombinedParticle.h"
#include "ParticleManager.h"
#include <cassert>
#include <filesystem>
#undef min
#undef max

void CombinedParticle::Initialize(const std::string& _name, const std::string& _comParticleFileName) {
	//名前をセット
	name_ = _name;
	//フォルダパス
	std::string folderPath = "Resources/particles/" + _comParticleFileName;
	std::vector<std::string> dataFiles;
	//ディレクトリ内のファイルを列挙
	for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
		if (entry.is_regular_file()) {
			//拡張子が .json かを判別
			if (entry.path().extension() == ".json") {
				//データファイルコンテナに格納
				dataFiles.push_back(entry.path().generic_string());
			}
		}
	}
	//データファイルコンテナを使ってパーティクルのコンテナに内部データを移す
	for (const auto& dataFile : dataFiles) {
		//初期化
		std::string fileName = dataFile.substr(
			dataFile.find_last_of("/") + 1,
			dataFile.rfind(".json") - dataFile.find_last_of("/") - 1
		);		//ファイル名(〇〇/△△.json→△△)
		std::string name = _comParticleFileName + "_" + fileName;	//名前(複合名_ファイル名)
		std::string relativePath = _comParticleFileName + "/" + fileName;	//データの相対パス
		//パーティクルを生成
		SingleParticleInfo sParticle;

		sParticle.particle = std::make_unique<Particle>();
		sParticle.particle->Initialize(name, relativePath);
		sParticle.particle->emitter_.isPlay = false;
		//発生開始時間&終了時間のセット
		json data = JsonUtil::GetJsonData(folderPath + "/" + fileName);
		sParticle.startTime = data["StartTime"];
		sParticle.endTime = data["EndTime"];
		//全体の尺のうち長ければ更新
		totalDuration_ = std::max(totalDuration_, sParticle.endTime);

		//コンテナに格納
		particles_.push_back(std::move(sParticle));
	}
}

void CombinedParticle::Update() {
	//再生フラグがオンなら
	if (isPlay_) {
		//タイマーをカウント
		timer_ += kDeltaTime;
		//全てのパーティクルを走査
		for (auto& sParInfo : particles_) {
			//再生フラグがオフの時
			if (!sParInfo.particle->emitter_.isPlay) {
				//タイマーがstartTime~endTimeの間にある時
				if (timer_ > sParInfo.startTime && timer_ < sParInfo.endTime) {
					//パーティクルをオンにする
					sParInfo.particle->emitter_.isPlay = true;
				}
			}
			//再生フラグがオンの時
			else {
				//タイマーがstartTime~endTimeの外にある時
				if (timer_ < sParInfo.startTime || timer_ > sParInfo.endTime) {
					//パーティクルをオフにする
					sParInfo.particle->emitter_.isPlay = false;
				}
			}
		}
		//タイマーが全体の尺を超過したら
		if (timer_ > totalDuration_) {
			//再生フラグをオフにする
			isPlay_ = false;
			//全てのパーティクルを走査
			for (auto& sParInfo : particles_) {
				//全てのパーティクルを停止させる
				sParInfo.particle->emitter_.isPlay = false;
			}
		}
	}
}

std::vector<std::string> CombinedParticle::GetAllHandleName() {
	std::vector<std::string> result;
	//全てのパーティクルを走査
	for (auto& sParInfo : particles_) {
		//キーをresultに格納
		result.push_back(sParInfo.particle->GetName());
	}
	return result;
}

void CombinedParticle::SetBaseTransform(const TransformEuler& transform) {
	//パーティクルを走査
	for (auto& sParInfo : particles_) {
		//基準トランスフォームをセット
		sParInfo.particle->emitter_.transform.translate += transform.translate;
	}
}

std::string CombinedParticle::AddParticle(const std::string& _fileName, float _startTime, float _endTime) {
	//コンテナのサイズが最大数を超えていたらreturn
	if (particles_.size() >= kMaxCombinedParticles) {
		return "error";
	}
	//新しいパーティクルを作成
	SingleParticleInfo newParticle;
	newParticle.startTime = _startTime;
	newParticle.endTime = _endTime;
	newParticle.particle = std::make_unique<Particle>();
	//パーティクルの名前を決める(例 : fire.json→fire)
	std::string name = _fileName.substr(0, _fileName.rfind(".json"));
	//パーティクルの初期化
	newParticle.particle->Initialize(name, _fileName);
	//パーティクルをコンテナに追加
	particles_.push_back(std::move(newParticle));
	return name;
}

void CombinedParticle::RemoveParticle(const std::string& _handleName) {
	//パーティクルを走査
	for (auto& sParinfo : particles_) {
		//ハンドル名と一致するパーティクルを探す
		if (sParinfo.particle->GetName() == _handleName) {
			//見つかったらコンテナから削除してreturn
			particles_.erase(std::remove(particles_.begin(), particles_.end(), sParinfo), particles_.end());
			return;
		}
	}
}

const std::unordered_map<std::string, json> CombinedParticle::GetParams() {
	std::unordered_map<std::string, json> result;
	//全パーティクルを走査　(keyは.jsonを省いた形を想定)
	for (auto& sParInfo : particles_) {
		//keyを指定してパラメーターを格納
		result[sParInfo.particle->GetName()] = sParInfo.particle->GetParam();
	}

	return result;
}

void CombinedParticle::SetParams(const std::unordered_map<std::string, json>& _params) {
	//取得したパラメーターを走査 (keyは.jsonを省いた形を想定)
	for (auto& [key, param] : _params) {
		//パーティクルを走査
		for(auto& sParInfo : particles_) {
			//keyとパーティクル名が一致したら
			if (sParInfo.particle->GetName() == key) {
				//パラメーターをセット
				sParInfo.particle->SetParam(param);
				break;
			}
		}

		//ここへくる事は想定していない
		assert(false && "単パーティクルの名前変更時に反映されていない可能性があります");
	}
}
