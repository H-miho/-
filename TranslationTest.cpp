﻿#include <windows.h>
#include <iostream>
#include <speechapi_cxx.h>

using namespace std;
using namespace Microsoft::CognitiveServices::Speech;
using namespace Microsoft::CognitiveServices::Speech::Translation;

void SpeechTranslation()
{
	// サブスクリプションキーとサービスリージョンで Speech 構成のインスタンスを作成します
	// 下記の自分のサブスクリプションキーとサービスリージョンに編集しましょう (例 "XXXXX"->"japanwest")
	auto config = SpeechTranslationConfig::FromSubscription( "3284bce22af543b398aac4801fdc969b", "japaneast" );

	// 認識させたい言語を指定します (日本語="ja-JP", 英語="en-US")
	auto fromLanguage = "ja-JP";
	config->SetSpeechRecognitionLanguage( fromLanguage );

	// 上記の設定からスピーチ翻訳を作成します (ドイツ語="de", フランス語="fr", 中国語="zh" など)
	config->AddTargetLanguage( "en" );
	auto recognizer = TranslationRecognizer::FromConfig( config );
	cout << u8"翻訳元の言語で話してください\n";

	// スピーチ翻訳を開始し、しゃべり終わると翻訳された文字列が結果として戻ります
	// この API は沈黙が続くかしゃべり始めて 15 秒経過したときにしゃべり終わったと判断します
	// ※ RecognizeOnceAsync() はしゃべり終わったときに結果が戻るためコマンドや検索のようなシーンに向いています
	// ※ 連続的に翻訳させたい場合は StartContinuousRecognitionAsync() を使用するようにします
	auto result = recognizer->RecognizeOnceAsync().get();

	// 結果を確認します
	if ( result->Reason == ResultReason::TranslatedSpeech )
	{
		for( const auto& it : result->Translations )
		{
			cout << u8"翻訳結果 '" << it.first.c_str() << u8"': " << it.second.c_str() << std::endl;
		}
	}
	else if ( result->Reason == ResultReason::RecognizedSpeech )
	{
		cout << u8"認識結果: " << result->Text << u8" (翻訳できませんでした)" << std::endl;
	}
	else if ( result->Reason == ResultReason::NoMatch )
	{
		cout << u8"NOMATCH: 声を認識できませんでした" << std::endl;
	}
	else if ( result->Reason == ResultReason::Canceled )
	{
		auto cancellation = CancellationDetails::FromResult( result );
		cout << u8"CANCELED: 理由=" << (int) cancellation->Reason << std::endl;

		if ( cancellation->Reason == CancellationReason::Error )
		{
			cout << u8"CANCELED: エラーコード= " << (int) cancellation->ErrorCode << std::endl;
			cout << u8"CANCELED: エラー詳細= " << cancellation->ErrorDetails << std::endl;
			cout << u8"CANCELED: サブスクリプション情報を確認しましょう" << std::endl;
		}
	}
}

int wmain()
{
	SpeechTranslation();

	return 0;
}
