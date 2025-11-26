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
			dataFile.find_last_of("/\\") + 1,
			dataFile.rfind(".json") - dataFile.find_last_of("/\\") - 1
		);		//ファイル名(〇〇/△△.json→△△)
		std::string name = _comParticleFileName + "_" + fileName;	//名前(複合名_ファイル名)
		std::string relativePath = _comParticleFileName + "/" + fileName;	//データの相対パス
		//パーティクルを生成
		particles_[fileName].particle = std::make_unique<Particle>();
		particles_[fileName].particle->Initialize(name, relativePath);
		particles_[fileName].particle->emitter_.isPlay = false;
		//発生開始時間&終了時間のセット
		json data = JsonUtil::GetJsonData(folderPath + "/" + fileName);
		particles_[fileName].startTime = data["StartTime"];
		particles_[fileName].endTime = data["EndTime"];
		//全体の尺のうち長ければ更新
		totalDuration_ = std::max(totalDuration_, particles_[fileName].endTime);
	}
}

void CombinedParticle::Update() {
	//再生フラグがオンなら
	if (isPlay_) {
		//タイマーをカウント
		timer_ += kDeltaTime;
		//全てのパーティクルを走査
		for (auto& [key, particleInfo] : particles_) {
			//再生フラグがオフの時
			if (!particleInfo.particle->emitter_.isPlay) {
				//タイマーがstartTime~endTimeの間にある時
				if (timer_ > particleInfo.startTime && timer_ < particleInfo.endTime) {
					//パーティクルをオンにする
					particleInfo.particle->emitter_.isPlay = true;
				}
			}
			//再生フラグがオンの時
			else {
				//タイマーがstartTime~endTimeの外にある時
				if (timer_ < particleInfo.startTime || timer_ > particleInfo.endTime) {
					//パーティクルをオフにする
					particleInfo.particle->emitter_.isPlay = false;
				}
			}
		}
		//タイマーが全体の尺を超過したら
		if (timer_ > totalDuration_) {
			//再生フラグをオフにする
			isPlay_ = false;
			//全てのパーティクルを走査
			for (auto& [key, particleInfo] : particles_) {
				//全てのパーティクルを停止させる
				particleInfo.particle->emitter_.isPlay = false;
			}
		}
	}
}

std::vector<std::string> CombinedParticle::GetAllHandleName() {
	std::vector<std::string> result;
	//全てのパーティクルを走査
	for (auto& [key, particleInfo] : particles_) {
		//キーをresultに格納
		result.push_back(key);
	}
	return result;
}

std::unordered_map<std::string, json> CombinedParticle::GetParams() {
	std::unordered_map<std::string, json> result;
	//全パーティクルを走査
	for (auto& [key, particleInfo] : particles_) {
		//keyを指定してパラメーターを格納
		result[key].push_back(particleInfo.particle->GetParam());
	}

	return result;
}

void CombinedParticle::SetBaseTransform(const TransformEuler& transform) {
	//パーティクルを走査
	for(auto& [key, particleInfo] : particles_) {
		//基準トランスフォームをセット
		particleInfo.particle->emitter_.transform.translate += transform.translate;
	}
}

void CombinedParticle::SetParams(const std::unordered_map<std::string, json>& _params) {
	//取得したパラメーターを走査
	for (auto& [key, param] : _params) {
		//キーを指定してパラメーターをセット
		particles_[key].particle->SetParam(param);
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
	//パーティクルの初期化
	newParticle.particle->Initialize(ParticleManager::GetInstance()->GenerateName(name_), _fileName);
	//パーティクルをコンテナに追加
	std::string name = _fileName.substr(0, _fileName.rfind(".json"));	//ファイル名(.json抜き)
	particles_[name] = std::move(newParticle);
	return name;
}

void CombinedParticle::RemoveParticle(const std::string& _handleName) {
	//指定されたIDのパーティクルが存在するかチェック
	auto it = particles_.find(_handleName);
	if (it != particles_.end()) {
		//存在する場合は削除
		particles_.erase(it);
	}
}
