# TTXExtraMenus - TeraTermプラグイン

([English](./README.md))

TTXExtraMenusプラグインを使うとTeraTermに既存のメニュー項目をメニューバーに追加することができます。

組み込み系のエンジニアのの多くはUART (COMポート)アクセスのためにTeraTermを使用することが多いでしょう。  
その中でも、Clear Buffer、COMの設定／選択の使用頻度が高く、これらのメニューがメニューバーに設置されていると便利です。

またこのプラグインではCOMポートへの接続、切断の切り替えのためのConnectメニュー(ID 55200)もサポートしています。  
(本家のTeraTermで同等の機能は見つけることができず、独自のハックのような形ですが。)


## インストール方法

1. TeraTerm v5.3リリースをダウンロードして展開

   https://github.com/TeraTermProject/teraterm/releases/download/v5.3/teraterm-5.3.zip

2. プラグインをダウンロード、解凍し、`TTXExtraMenus.dll`を展開したディレクトリ`teraterm-5.3`にコピー。（`ttermpro.exe`が同ディレクトリにあること）

	https://github.com/hatomugi-cha/TTXExtraMenus/releases/download/v5.3-1.0/TTXExtraMenus-v5.3-1.0-x86.zip


## ディフォルトの追加メニュー

![TTXExtraMenus](doc/images/TTXExtraMenus-highlight.png)

* `Serial` - "Setup -> Serial port..."を呼び出す
* `Connect` - COMポートへの接続、切断の切り替え機能
* `Clear` - "Edit -> Clear buffer"を呼び出す


## 設定方法

TTXExtraMenus.dllをインストール後、ttermpro.exeを起動するとTERATERM.iniファイルに以下のディフォルト設定が書き込まれます。

    [ExtraMenus]
    Menu=Serial:50350, Connect:55200, Clear:50260
    Enabled=True    

* "メニューの名前:メニューID"のペアから成ります。
* [Tera Term - メニュー ID 表](https://teratermproject.github.io/manual/5/ja/reference/menu_id.html)


## ビルド方法

1. Windows11上でコマンドプロンプトを開き、以下のバッチファイルを実行してMicrosoft Visual Studio 2022の環境を有効にする。

       C:\"Program Files"\"Microsoft Visual Studio"\2022\Community\VC\Auxiliary\Build\vcvars32.bat

   もしくはWindowsメニューから`x86 Native Tools Command Prompt for VS 2022`を選択して実行。

2. プロント上でbuild.batを実行

       build.bat

   成果物はRelease\TTXExtraMenus.dllに生成され、out\teraterm-5.3\にもコピーされます。

3. （オプション）build-teraterm.batを実行することで`out\teraterm\teraterm\Release\`にttermpro.exeがビルドされます。


## 謝辞

TeraTermは組み込み機器との通信に不可欠なソフトウェアとして世界中で使われています。  
TeraTermに携わる開発者の皆様にお礼申し上げます。

## References

 - [Tera Termソースコード解説 - プラグインサポート](https://teratermproject.github.io/manual/5/ja/reference/sourcecode.html#plugin)
 - [Tera Term - メニュー ID 表](https://teratermproject.github.io/manual/5/ja/reference/menu_id.html)

## Author of TTXExtraMenus Plugin

hatomugi-cha

END
