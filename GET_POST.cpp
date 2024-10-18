#include <Siv3D.hpp>

void Main()
{
	// GETリクエストの設定
	const URL getUrl = U"http://localhost:8080/problem";//問題取得のアドレスに変更
	const HashTable<String, String> headers = {
		{ U"Content-Type", U"application/json" },
		{ U"Procon-Token", U"token1" }//ここでトークンの変更
	};
	const FilePath authSaveFilePath = U"auth_result.json";

	// POSTリクエストの設定
	const URL postUrl = U"http://localhost:8080/answer";//回答提出のアドレスに変更
	const FilePath jsonFilePath = U"C:/Users/藤澤怜央/Downloads/procon-server (1)/solution.json";//ここで回答のjsonファイルのPathを取得

	// ファイル存在確認
	if (!FileSystem::Exists(jsonFilePath))
	{
		Print << U"solution.json not found at: " << jsonFilePath;
		return;
	}

	// ユーザーによるGETリクエストの実行
	Print << U"Press Enter to send GET request.";
	while (System::Update())
	{
		if (KeyEnter.down())
		{
			if (const auto response = SimpleHTTP::Get(getUrl, headers, authSaveFilePath))
			{
				Console << U"------";
				Console << response.getStatusLine().rtrimmed();
				Console << U"status code: " << FromEnum(response.getStatusCode());
				Console << U"------";
				Console << response.getHeader().rtrimmed();
				Console << U"------";

				if (response.isOK())
				{
					Print << TextReader{ authSaveFilePath }.readAll();
				}
			}
			else
			{
				Print << U"GET request failed.";
			}
			break; // ループを抜ける
		}
	}

	// ユーザーによるPOSTリクエストの実行
	Print << U"Press Enter to send POST request.";
	while (System::Update())
	{
		if (KeyEnter.down())
		{
			const String jsonData = TextReader{ jsonFilePath }.readAll();
			const FilePath postSaveFilePath = U"post_result.json";

			// POSTリクエストを送信
			if (auto response = SimpleHTTP::Post(postUrl, headers, jsonData.narrow().data(), jsonData.size(), postSaveFilePath))
			{
				Console << U"------";
				Console << response.getStatusLine().rtrimmed();
				Console << U"status code: " << FromEnum(response.getStatusCode());
				Console << U"------";
				Console << response.getHeader().rtrimmed();
				Console << U"------";

				if (response.isOK())
				{
					Print << TextReader{ postSaveFilePath }.readAll();
				}
			}
			else
			{
				Print << U"POST request failed.";
			}
			break; // ループを抜ける

		}
	}

	while (System::Update())
	{
		// アプリケーションの更新処理
	}
}
